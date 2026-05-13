import os
import json
import glob
import re
import time
import random
from datetime import datetime
from urllib.parse import urljoin, urlparse, parse_qsl, urlencode, urlunparse
from bs4 import BeautifulSoup
from playwright.sync_api import sync_playwright

try:
    from cf_contest_scraper import (
        launch_system_chromium,
        looks_like_cloudflare,
        save_debug_html,
        find_problem_link_in_tag,
        clean_text,
        ensure_problem_entry,
        extract_target_index_from_spoiler_title,
        replace_problem_index,
        parse_problem_url,
        parse_problem_heading_fallback,
        WITH_HINT_DIR,
        WITHOUT_HINT_DIR,
        BASE_URL,
    )
except ImportError:
    print("❌ 导入失败！请确保本脚本与 cf_contest_scraper.py 放在同一个文件夹下。")
    raise SystemExit(1)


# ============================================================
# 设计原则：尽量学习 cf_contest_scraper.py 的浏览器行为，而不是“更聪明”。
#
# 1. 不使用 launch_persistent_context；
# 2. 不使用 .cf_chrome_profile；
# 3. 不手动设置 viewport / timezone / locale；
# 4. 每场比赛单独启动 browser/new_context/new_page，用完关闭；
# 5. 只保留 MathJax 拦截，和原 cf_contest_scraper.py 一致；
# 6. 解析层稍微增强：支持 solution sketch / analysis 等题解链接；
# 7. 题解页只等待/轮询，不反复 reload 硬撞。
# ============================================================

FORCE_LOCALE_EN = True
EDITORIAL_MIN_TEXT_LEN = 80

# 失败队列策略：失败后不在原地重试，而是放到下一轮。
# 第一轮处理完所有比赛后，才会等待一段较长时间并开始下一轮。
MAX_ATTEMPTS_PER_CONTEST = 3
RETRY_ROUND_SLEEP_MIN_SECONDS = 60
RETRY_ROUND_SLEEP_MAX_SECONDS = 120
FAILED_QUEUE_FILE = "fix_tutorial_failed_queue.json"

TUTORIAL_KEYWORDS = [
    "tutorial",
    "editorial",
    "solution",
    "solutions",
    "solution sketches",
    "sketch",
    "analysis",
    "разбор",
]

BAD_BLOG_KEYWORDS = [
    "announcement",
    "update",
    "invitation",
    "editorial announcement",
]


def polite_sleep(min_seconds=3, max_seconds=5, reason=""):
    wait_time = random.uniform(min_seconds, max_seconds)
    if reason:
        print(f"⏳ {reason}，等待 {wait_time:.2f} 秒...")
    else:
        print(f"⏳ 等待 {wait_time:.2f} 秒...")
    time.sleep(wait_time)


def force_locale_en(url: str) -> str:
    if not FORCE_LOCALE_EN:
        return url
    parsed = urlparse(url)
    query = dict(parse_qsl(parsed.query, keep_blank_values=True))
    query["locale"] = "en"
    return urlunparse(parsed._replace(query=urlencode(query)))




def write_failed_queue_file(items, note=""):
    """
    将当前仍需稍后重试的比赛写到本地 JSON。
    这个文件主要用于观察/恢复：即使脚本中断，下次重新运行时也会重新扫描 broken files，
    所以不强依赖这个文件继续工作。
    """
    payload = {
        "updated_at": datetime.now().strftime("%Y-%m-%d %H:%M:%S"),
        "note": note,
        "max_attempts_per_contest": MAX_ATTEMPTS_PER_CONTEST,
        "items": [],
    }

    for item in items:
        contest_id = item.get("contest_id")
        filepaths = item.get("filepaths", [])
        payload["items"].append(
            {
                "contest_id": contest_id,
                "next_attempt": item.get("attempt", 1),
                "file_count": len(filepaths),
                "reason": item.get("reason", ""),
                "files": [os.path.basename(path) for path in filepaths],
            }
        )

    with open(FAILED_QUEUE_FILE, "w", encoding="utf-8") as f:
        json.dump(payload, f, ensure_ascii=False, indent=4)

    print(f"🧾 已更新失败队列文件: {FAILED_QUEUE_FILE}，剩余 {len(payload['items'])} 场待重试。")


def clear_failed_queue_file():
    if os.path.exists(FAILED_QUEUE_FILE):
        os.remove(FAILED_QUEUE_FILE)
        print(f"🧹 已清空失败队列文件: {FAILED_QUEUE_FILE}")

def score_blog_link(text: str, href: str) -> int:
    lower = (text or "").strip().lower()
    if "/blog/entry/" not in href:
        return -10**9

    score = 0
    if "tutorial" in lower:
        score += 100
    if "editorial" in lower:
        score += 95
    if "solution sketches" in lower:
        score += 90
    elif "solution" in lower:
        score += 80
    if "sketch" in lower:
        score += 70
    if "analysis" in lower:
        score += 60
    if "разбор" in lower:
        score += 60

    # Announcement / Invitation 通常也是 blog entry，但不是题解。
    if any(bad in lower for bad in BAD_BLOG_KEYWORDS):
        score -= 100

    return score


def extract_tutorial_url_from_html(html_content: str):
    soup = BeautifulSoup(html_content, "html.parser")
    candidates = []

    for a_tag in soup.find_all("a", href=True):
        href = a_tag.get("href", "")
        if "/blog/entry/" not in href:
            continue

        text = a_tag.get_text(" ", strip=True)
        full_url = urljoin(BASE_URL, href)
        candidates.append((score_blog_link(text, href), text, full_url))

    candidates.sort(reverse=True, key=lambda x: x[0])

    if candidates:
        best_score, best_text, best_url = candidates[0]
        if best_score > 0:
            print(f"🎯 成功找到题解链接: {best_url}  [{best_text}]")
            return best_url

        print("⚠️ 找到了 blog 链接，但没有明显题解关键词。候选如下：")
        for score, text, url in candidates[:5]:
            print(f"   - score={score:>4} text={text!r} url={url}")

    return None


def get_tutorial_url_like_original(page, contest_url, contest_id=None):
    """
    学习 cf_contest_scraper.py 的 get_tutorial_url：
    page.goto -> page.content -> BeautifulSoup 扫 a[href]

    不做 selector 探测，不 about:blank，不重试 reload。
    """
    contest_url = force_locale_en(contest_url)
    print(f"🔍 正在访问比赛主页: {contest_url}")

    try:
        page.goto(contest_url, wait_until="domcontentloaded", timeout=60000)
        html_content = page.content()

        if looks_like_cloudflare(html_content):
            print("❌ 比赛主页返回的是 Cloudflare 验证页，跳过本场。")
            save_debug_html(f"debug_contest_cloudflare_{contest_id or 'unknown'}.html", html_content)
            return None

        tutorial_url = extract_tutorial_url_from_html(html_content)
        if tutorial_url:
            return tutorial_url

        print("❌ 未能在页面找到题解 Tutorial / Editorial / Solution 链接。")
        save_debug_html(f"debug_no_tutorial_link_{contest_id or 'unknown'}.html", html_content)
        return None

    except Exception as e:
        print(f"❌ 访问比赛页面失败: {e}")
        return None


def parse_editorial_html(html_content: str):
    soup = BeautifulSoup(html_content, "html.parser")
    content_div = soup.find("div", class_="ttypography")
    if not content_div:
        return None

    problems_data = {}
    current_key = "Problem_Unknown"
    current_problem_info = None
    current_heading = "Problem_Unknown"

    title_regex = re.compile(
        r"^("
        r"[A-Z]\d?\.?\s+.*?"
        r"|\d{3,6}[A-Z]\d?\s*[-–—.:]\s*.*"
        r"|Problem\s+[A-Z]\d?\.?\s*.*"
        r"|Task\s+[A-Z]\d?\.?\s*.*"
        r"|Div\d?[A-Z]\d?\.?\s*.*"
        r")$",
        re.IGNORECASE,
    )

    ensure_problem_entry(problems_data, current_key, problem_info=None, editorial_heading=current_heading)

    for tag in content_div.find_all(True):
        text = tag.get_text(strip=True)
        is_new_problem = False
        problem_info = None

        if tag.name in ["h2", "h3", "h4"] and text:
            problem_info = find_problem_link_in_tag(tag)
            if problem_info or (title_regex.match(text) and len(text) < 120):
                is_new_problem = True

        elif tag.name in ["p", "div"] and tag.find("strong"):
            problem_info = find_problem_link_in_tag(tag)
            if (problem_info and len(text) < 160) or (text and title_regex.match(text) and len(text) < 120):
                is_new_problem = True

        elif tag.name == "a" and "/problem/" in tag.get("href", ""):
            problem_info = find_problem_link_in_tag(tag)
            if text and len(text) < 120:
                is_new_problem = True

        if is_new_problem:
            current_heading = clean_text(text)
            if problem_info:
                current_problem_info = problem_info
                current_key = problem_info["problem_url"]
            else:
                current_problem_info = None
                current_key = f"UNKNOWN::{current_heading}"

            ensure_problem_entry(
                problems_data,
                current_key,
                problem_info=current_problem_info,
                editorial_heading=current_heading,
            )

        elif tag.name == "div" and "spoiler" in tag.get("class", []):
            ensure_problem_entry(
                problems_data,
                current_key,
                problem_info=current_problem_info,
                editorial_heading=current_heading,
            )

            title_tag = tag.find("b", class_="spoiler-title") or tag.find(class_="spoiler-title")
            content_tag = tag.find("div", class_="spoiler-content") or tag.find(class_="spoiler-content")

            if title_tag and content_tag:
                title_text = title_tag.get_text(strip=True)
                content_text = content_tag.get_text(separator="\n", strip=True)
                lower_title = title_text.lower()

                target_key = current_key
                target_problem_info = current_problem_info
                target_index = extract_target_index_from_spoiler_title(title_text)

                if target_index and current_problem_info:
                    if target_index != current_problem_info["problem_index"]:
                        new_url = replace_problem_index(current_problem_info["problem_url"], target_index)
                        new_info = parse_problem_url(new_url)
                        if new_info:
                            target_key = new_info["problem_url"]
                            target_problem_info = new_info

                ensure_problem_entry(
                    problems_data,
                    target_key,
                    problem_info=target_problem_info,
                    editorial_heading=current_heading,
                )

                if "hint" in lower_title:
                    problems_data[target_key]["hints"].append({"title": title_text, "content": content_text})
                elif any(kw in lower_title for kw in ["solution", "tutorial", "editorial", "implementation"]):
                    problems_data[target_key]["solutions"].append({"title": title_text, "content": content_text})

    return problems_data


def scrape_cf_dynamic_like_original(page, tutorial_url, contest_id=None):
    """
    学习 cf_contest_scraper.py 的 scrape_cf_dynamic：
    page.goto -> 等待 20 秒 -> page.content -> 解析。

    只额外加一个温和轮询：如果还是 Tutorial is loading...，每 5 秒看一次，
    不 reload，不反复刷新。
    """
    tutorial_url = force_locale_en(tutorial_url)
    print(f"🚀 正在加载题解页面: {tutorial_url}")

    try:
        page.goto(tutorial_url, wait_until="domcontentloaded", timeout=60000)
        print("⏳ 等待 20 秒，让 JS/AJAX 加载真正的题解内容...")
        page.wait_for_timeout(20000)

        waited = 0
        max_extra_wait = 90
        html_content = ""

        while waited <= max_extra_wait:
            html_content = page.content()

            if looks_like_cloudflare(html_content):
                print("❌ 题解页面返回的是 Cloudflare 验证页。")
                save_debug_html(f"debug_editorial_cloudflare_{contest_id or 'unknown'}.html", html_content)
                return None

            soup = BeautifulSoup(html_content, "html.parser")
            content_div = soup.find("div", class_="ttypography")
            content_text = content_div.get_text(" ", strip=True) if content_div else ""

            if content_div and "Tutorial is loading..." not in content_text and len(content_text) >= EDITORIAL_MIN_TEXT_LEN:
                if waited:
                    print(f"✅ 题解真实内容加载完毕！额外等待 {waited} 秒")
                return parse_editorial_html(html_content)

            if waited == max_extra_wait:
                break

            print("⏳ 题解仍在 loading，继续等 5 秒；不刷新页面。")
            page.wait_for_timeout(5000)
            waited += 5

        print("❌ 等待后仍未拿到真实题解正文，跳过本场。")
        save_debug_html(f"debug_tutorial_still_loading_{contest_id or 'unknown'}.html", html_content)
        return None

    except Exception as e:
        print(f"❌ 页面加载失败: {e}")
        return None


def find_broken_files():
    broken_files = []
    print("🔍 正在扫描带有 'Tutorial is loading...' 的损坏文件...")
    for d in [WITH_HINT_DIR, WITHOUT_HINT_DIR]:
        if not os.path.exists(d):
            continue
        for filepath in glob.glob(os.path.join(d, "*.json")):
            try:
                with open(filepath, "r", encoding="utf-8") as f:
                    content = f.read()
                if "Tutorial is loading..." in content:
                    broken_files.append(filepath)
            except Exception:
                pass
    print(f"🎯 共发现 {len(broken_files)} 个损坏文件需要修复。")
    return broken_files


def build_scraped_map(data, contest_id):
    scraped_map = {}
    for idx, (problem_key, content) in enumerate(data.items(), start=1):
        problem_info = content.get("problem_info")
        editorial_heading = content.get("editorial_heading", "")
        if problem_info:
            p_code = f"CF{problem_info['contest_id']}{problem_info['problem_index']}"
        else:
            p_code, _ = parse_problem_heading_fallback(editorial_heading, contest_id, idx)
        scraped_map[p_code] = content
    return scraped_map


def update_one_file(filepath, scraped_map):
    filename = os.path.basename(filepath)
    broken_code = filename.split(" ")[0]

    if broken_code not in scraped_map:
        print(f"⚠️ 抓到了页面，但未能匹配到题目: {broken_code}")
        return False

    new_content = scraped_map[broken_code]
    new_hints = new_content.get("hints", [])
    new_solutions = new_content.get("solutions", [])

    if "Tutorial is loading..." in json.dumps(new_solutions, ensure_ascii=False):
        print(f"⚠️ {filename} 重新抓取后仍然包含 Loading，跳过更新。")
        return False

    with open(filepath, "r", encoding="utf-8") as f:
        old_data = json.load(f)

    old_data["hints"] = new_hints
    old_data["solutions"] = new_solutions

    has_hint = bool(new_hints)
    current_dir = os.path.dirname(filepath)
    expected_dir = WITH_HINT_DIR if has_hint else WITHOUT_HINT_DIR
    os.makedirs(expected_dir, exist_ok=True)

    if current_dir != expected_dir:
        new_filepath = os.path.join(expected_dir, filename)
        with open(new_filepath, "w", encoding="utf-8") as f:
            json.dump(old_data, f, ensure_ascii=False, indent=4)
        os.remove(filepath)
        print(f"✨ 完美修复并挪窝: {filename} -> {os.path.basename(expected_dir)}")
    else:
        with open(filepath, "w", encoding="utf-8") as f:
            json.dump(old_data, f, ensure_ascii=False, indent=4)
        print(f"✨ 完美修复原文件: {filename}")

    return True


def process_one_contest(p, contest_id, filepaths, tutorial_cache):
    """
    核心：严格学习 cf_contest_scraper.py 的 process_contest：
    每个 contest 单独 browser/new_context/page，然后关闭 browser。

    返回值：
        (ok_count, should_retry, reason)
    只有“没拿到比赛主页/题解页真实数据”这类网络或 CF 风控型失败才进入重试队列；
    如果题解已抓到但题目匹配失败，一般是解析逻辑问题，原地/延迟重试帮助不大。
    """
    browser = None
    context = None

    try:
        browser = launch_system_chromium(p)
        context = browser.new_context()

        context.route("**/*mathjax*", lambda route: route.abort())
        context.route("**/*MathJax*", lambda route: route.abort())

        page = context.new_page()

        contest_url = f"{BASE_URL}/contest/{contest_id}"
        tutorial_url = get_tutorial_url_like_original(page, contest_url, contest_id=contest_id)

        if not tutorial_url:
            reason = "未拿到题解链接，可能是比赛主页 Cloudflare / 页面加载异常 / 题解链接未出现"
            print(f"❌ Contest {contest_id} 未拿到题解链接，本场放入稍后重试队列。")
            return 0, True, reason

        tutorial_url = force_locale_en(tutorial_url)

        if tutorial_url in tutorial_cache:
            print(f"♻️ 复用已抓取过的题解数据: {tutorial_url}")
            data = tutorial_cache[tutorial_url]
        else:
            data = scrape_cf_dynamic_like_original(page, tutorial_url, contest_id=contest_id)
            if data:
                tutorial_cache[tutorial_url] = data

        if not data:
            reason = "未拿到真实题解正文，可能是题解页 Cloudflare / Tutorial is loading / AJAX 超时"
            print(f"❌ 修复失败：未能获取到 Contest {contest_id} 的真实数据，本场放入稍后重试队列。")
            return 0, True, reason

        scraped_map = build_scraped_map(data, contest_id)

        ok_count = 0
        for filepath in filepaths:
            if update_one_file(filepath, scraped_map):
                ok_count += 1

        if ok_count == 0:
            reason = "题解已抓到，但没有任何文件匹配成功；更可能是解析/题号匹配问题，不自动重试"
            print(f"⚠️ Contest {contest_id} 抓取成功但更新 0/{len(filepaths)} 个文件，不进入重试队列。")
            return 0, False, reason

        print(f"✅ Contest {contest_id} 完成：成功更新 {ok_count}/{len(filepaths)} 个文件。")
        return ok_count, False, "success"

    except Exception as e:
        reason = f"处理过程异常: {e}"
        print(f"❌ Contest {contest_id} 处理过程异常，本场放入稍后重试队列: {e}")
        return 0, True, reason

    finally:
        if context is not None:
            try:
                context.close()
            except Exception:
                pass
        if browser is not None:
            try:
                browser.close()
            except Exception:
                pass


def run_fixer():
    broken_files = find_broken_files()
    if not broken_files:
        print("🎉 你的数据集非常完美，不需要修复！")
        clear_failed_queue_file()
        return

    contest_map = {}
    for filepath in broken_files:
        filename = os.path.basename(filepath)
        match = re.match(r"^CF(\d+)[A-Za-z0-9]", filename)
        if match:
            contest_id = match.group(1)
            contest_map.setdefault(contest_id, []).append(filepath)

    contest_items = sorted(contest_map.items(), key=lambda item: int(item[0]), reverse=True)
    print(f"📦 共涉及 {len(contest_items)} 场比赛。")
    print(
        f"🧺 失败队列已启用：失败比赛不会原地重试，而是进入下一轮；"
        f"每场最多尝试 {MAX_ATTEMPTS_PER_CONTEST} 次。"
    )

    tutorial_cache = {}
    total_ok = 0
    final_failed = []

    current_round = [
        {
            "contest_id": contest_id,
            "filepaths": filepaths,
            "attempt": 1,
            "reason": "initial",
        }
        for contest_id, filepaths in contest_items
    ]

    with sync_playwright() as p:
        round_id = 1
        while current_round:
            print("\n" + "#" * 60)
            print(f"🔁 开始第 {round_id} 轮，本轮队列中共有 {len(current_round)} 场比赛。")

            next_round = []

            for item in current_round:
                contest_id = item["contest_id"]
                filepaths = item["filepaths"]
                attempt = item["attempt"]

                print("\n" + "=" * 50)
                print(
                    f"🛠 开始修复 Contest {contest_id} "
                    f"(影响 {len(filepaths)} 个文件，尝试 {attempt}/{MAX_ATTEMPTS_PER_CONTEST})"
                )

                ok_count, should_retry, reason = process_one_contest(
                    p,
                    contest_id,
                    filepaths,
                    tutorial_cache,
                )
                total_ok += ok_count

                if should_retry:
                    if attempt < MAX_ATTEMPTS_PER_CONTEST:
                        retry_item = {
                            "contest_id": contest_id,
                            "filepaths": filepaths,
                            "attempt": attempt + 1,
                            "reason": reason,
                        }
                        next_round.append(retry_item)
                        print(
                            f"📌 Contest {contest_id} 已放入下一轮队列："
                            f"下次尝试 {attempt + 1}/{MAX_ATTEMPTS_PER_CONTEST}。原因：{reason}"
                        )
                    else:
                        failed_item = {
                            "contest_id": contest_id,
                            "filepaths": filepaths,
                            "attempt": attempt,
                            "reason": reason,
                        }
                        final_failed.append(failed_item)
                        print(
                            f"🧊 Contest {contest_id} 已达到最大尝试次数，"
                            f"本次不再继续撞 CF。原因：{reason}"
                        )

                # 学习原 cf_contest_scraper.py：每场处理完后短暂停顿。
                polite_sleep(10, 15, "本场比赛处理结束，进入下一场前")

            if next_round:
                write_failed_queue_file(
                    next_round + final_failed,
                    note=f"第 {round_id} 轮结束后保存；next_round 会在本次运行稍后继续尝试，final_failed 供下次运行参考。",
                )

                print(
                    f"\n🧺 第 {round_id} 轮结束：{len(next_round)} 场比赛进入下一轮延迟重试。"
                )
                polite_sleep(
                    RETRY_ROUND_SLEEP_MIN_SECONDS,
                    RETRY_ROUND_SLEEP_MAX_SECONDS,
                    "进入下一轮重试前，整体降频",
                )

            current_round = next_round
            round_id += 1

    if final_failed:
        write_failed_queue_file(
            final_failed,
            note="这些比赛在本次运行中达到最大尝试次数；建议过一段时间后重新运行脚本。",
        )
        print("\n🧊 以下比赛本次最终失败，已保存在失败队列文件中：")
        for item in final_failed:
            print(
                f"   - Contest {item['contest_id']}，"
                f"尝试 {item['attempt']}/{MAX_ATTEMPTS_PER_CONTEST}，原因：{item['reason']}"
            )
    else:
        clear_failed_queue_file()

    print(f"\n🏆 修复流程结束！本次成功更新 {total_ok} 个文件。")


if __name__ == "__main__":
    run_fixer()
