import json
import time
import re
import os
from urllib.parse import urljoin
from bs4 import BeautifulSoup
from playwright.sync_api import sync_playwright


def get_tutorial_url(page, contest_url):
    """
    步骤 1: 访问比赛主页，寻找右侧 "Contest materials" 中的题解链接
    """
    print(f"🔍 正在访问比赛主页: {contest_url}")
    try:
        page.goto(contest_url, wait_until="domcontentloaded", timeout=60000)
        html_content = page.content()
        soup = BeautifulSoup(html_content, 'html.parser')

        for a_tag in soup.find_all('a', href=True):
            text = a_tag.get_text(strip=True).lower()
            href = a_tag['href']

            if ('tutorial' in text or 'editorial' in text) and '/blog/entry/' in href:
                full_url = urljoin("https://codeforces.com", href)
                print(f"🎯 成功找到题解链接: {full_url}")
                return full_url

        print("❌ 未能在页面找到题解(Tutorial/Editorial)的链接。这通常是因为官方还没发布。")
        return None

    except Exception as e:
        print(f"❌ 访问比赛页面失败: {e}")
        return None


def scrape_cf_dynamic(page, tutorial_url):
    """
    步骤 2: 访问题解页面，等待 JS 加载并提取 Hint & Solution
    """
    print(f"🚀 正在加载题解页面: {tutorial_url}")

    try:
        page.goto(tutorial_url, wait_until="domcontentloaded", timeout=60000)
        print("⏳ 等待 5 秒，让 JS 加载真正的题解内容...")
        page.wait_for_timeout(5000)
        html_content = page.content()

    except Exception as e:
        print(f"❌ 页面加载失败: {e}")
        return None

    soup = BeautifulSoup(html_content, 'html.parser')
    content_div = soup.find('div', class_='ttypography')

    if not content_div:
        print("❌ 未找到正文，可能是因为网络太慢。")
        return None

    problems_data = {}
    current_problem = "Problem_Unknown"

    # 匹配：
    # A. Problem Name
    # B1. Problem Name
    # 2211A - Problem Name
    # Problem A. Problem Name
    # Task A. Problem Name
    title_regex = re.compile(
        r'^('
        r'[A-Z]\d?\.?\s+.*?'
        r'|\d{3,6}[A-Z]\d?\s*[-–—.:]\s*.*'
        r'|Problem\s+[A-Z]\d?\.?\s*.*'
        r'|Task\s+[A-Z]\d?\.?\s*.*'
        r')$',
        re.IGNORECASE
    )

    for tag in content_div.find_all(True):
        text = tag.get_text(strip=True)
        is_new_problem = False

        if tag.name in ['h2', 'h3', 'h4'] and text:
            if title_regex.match(text) and len(text) < 100:
                is_new_problem = True

        elif tag.name in ['p', 'div'] and tag.find('strong'):
            if text and title_regex.match(text) and len(text) < 100:
                is_new_problem = True

        elif tag.name == 'a' and '/problem/' in tag.get('href', ''):
            if text and len(text) < 100:
                is_new_problem = True

        if is_new_problem:
            current_problem = text
            if current_problem not in problems_data:
                problems_data[current_problem] = {
                    "hints": [],
                    "solutions": []
                }

        elif tag.name == 'div' and 'spoiler' in tag.get('class', []):
            if current_problem not in problems_data:
                problems_data[current_problem] = {
                    "hints": [],
                    "solutions": []
                }

            title_tag = tag.find('b', class_='spoiler-title')
            content_tag = tag.find('div', class_='spoiler-content')

            if title_tag and content_tag:
                title_text = title_tag.get_text(strip=True)
                content_text = content_tag.get_text(separator='\n', strip=True)

                lower_title = title_text.lower()

                if 'hint' in lower_title:
                    problems_data[current_problem]["hints"].append({
                        "title": title_text,
                        "content": content_text
                    })

                elif 'solution' in lower_title or 'tutorial' in lower_title:
                    problems_data[current_problem]["solutions"].append({
                        "title": title_text,
                        "content": content_text
                    })

    return problems_data


def sanitize_filename(name):
    """
    去掉 Windows / macOS / Linux 文件名里的非法字符
    """
    name = re.sub(r'[\\/*?:"<>|]', "_", name)
    name = re.sub(r'\s+', ' ', name)
    return name.strip()


def parse_problem_heading(problem_heading, contest_id, fallback_index):
    """
    将题解中的题目标题解析成：

    A. Antimedian Deletion
    -> CF2211A, Antimedian Deletion

    2211A - Antimedian Deletion
    -> CF2211A, Antimedian Deletion

    Problem A. Antimedian Deletion
    -> CF2211A, Antimedian Deletion
    """
    s = re.sub(r'\s+', ' ', problem_heading).strip()

    patterns = [
        # 2211A - Antimedian Deletion
        r'^(?:CF)?\s*(\d{3,6})([A-Z]\d?)\s*[-–—.:]\s*(.+)$',

        # Problem A. Antimedian Deletion
        # Task A. Antimedian Deletion
        r'^(?:Problem|Task)\s+([A-Z]\d?)\s*[\.\-–—:]?\s*(.+)$',

        # A. Antimedian Deletion
        # B1. Something
        r'^([A-Z]\d?)\s*[\.\-–—:]\s*(.+)$',

        # A Antimedian Deletion
        # B1 Something
        r'^([A-Z]\d?)\s+(.+)$',
    ]

    for pattern in patterns:
        m = re.match(pattern, s, re.IGNORECASE)

        if not m:
            continue

        # 第一种情况：带 contest id，例如 2211A - xxx
        if len(m.groups()) == 3:
            real_contest_id = m.group(1)
            problem_index = m.group(2).upper()
            problem_title = m.group(3).strip()

            # 一般还是用 contest_url 里的 contest_id，更稳
            problem_code = f"CF{contest_id}{problem_index}"
            return problem_code, problem_title

        # 其他情况：只有 A / B / C1
        elif len(m.groups()) == 2:
            problem_index = m.group(1).upper()
            problem_title = m.group(2).strip()

            problem_code = f"CF{contest_id}{problem_index}"
            return problem_code, problem_title

    # 兜底：如果实在解析不出来
    fallback_code = f"CF{contest_id}_Unknown{fallback_index}"
    fallback_title = s if s != "Problem_Unknown" else f"Problem {fallback_index}"

    return fallback_code, fallback_title


def make_unique_filename(save_dir, base_name):
    """
    防止文件重名。
    如果 CF2211A xxx.json 已经存在，则生成：
    CF2211A xxx_2.json
    CF2211A xxx_3.json
    """
    path = os.path.join(save_dir, base_name)

    if not os.path.exists(path):
        return path

    name, ext = os.path.splitext(base_name)
    idx = 2

    while True:
        new_base_name = f"{name}_{idx}{ext}"
        new_path = os.path.join(save_dir, new_base_name)

        if not os.path.exists(new_path):
            return new_path

        idx += 1


def process_contest(contest_url):
    """
    启动 Playwright 并串联整个流程
    """
    with sync_playwright() as p:
        browser = p.chromium.launch(headless=False)
        context = browser.new_context()

        # 全局拦截 MathJax，保留原始 $$$ 公式文本，避免乱码
        context.route("**/*mathjax*", lambda route: route.abort())
        context.route("**/*MathJax*", lambda route: route.abort())

        page = context.new_page()

        tutorial_url = get_tutorial_url(page, contest_url)

        if tutorial_url:
            data = scrape_cf_dynamic(page, tutorial_url)

            if data:
                contest_id = contest_url.rstrip('/').split('/')[-1]

                save_dir = "cf_dataset"
                os.makedirs(save_dir, exist_ok=True)

                saved_count = 0

                for idx, (problem_name, content) in enumerate(data.items(), start=1):
                    if content["hints"] or content["solutions"]:
                        problem_code, problem_title = parse_problem_heading(
                            problem_name,
                            contest_id,
                            idx
                        )

                        safe_title = sanitize_filename(problem_title)

                        # 最终文件名格式：
                        # CF2211A Antimedian Deletion.json
                        filename_base = f"{problem_code} {safe_title}.json"
                        filename_base = sanitize_filename(filename_base)

                        filename = make_unique_filename(save_dir, filename_base)

                        with open(filename, 'w', encoding='utf-8') as f:
                            json.dump(content, f, ensure_ascii=False, indent=4)

                        print(f"✅ 生成文件: {filename}")
                        saved_count += 1

                print(f"🎉 比赛 {contest_id} 爬取完毕，共保存了 {saved_count} 道有价值的题目！\n")

            else:
                print("❌ 题解抓取或解析失败。")

        browser.close()


if __name__ == "__main__":
    contests_to_scrape = [
        f"https://codeforces.com/contest/{i}"
        for i in range(2222, 1999, -1)
    ]

    for contest in contests_to_scrape:
        process_contest(contest)