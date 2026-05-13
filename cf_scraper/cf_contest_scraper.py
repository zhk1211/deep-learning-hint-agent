import json
import re
import os
import time
import random
from urllib.parse import urljoin, urlparse
from bs4 import BeautifulSoup
from playwright.sync_api import sync_playwright


BASE_URL = "https://codeforces.com"
SAVE_DIR = "cf_dataset"
WITH_HINT_DIR = os.path.join(SAVE_DIR, "with_hint")
WITHOUT_HINT_DIR = os.path.join(SAVE_DIR, "without_hint")
LUOGU_BASE_URL = "https://www.luogu.com.cn/problem"



def launch_system_chromium(p):
	"""
	使用本机已经安装的 Google Chrome / Chromium，而不是 Playwright 自带 Chromium。

	macOS 优先使用：
	  /Applications/Google Chrome.app/Contents/MacOS/Google Chrome

	Ubuntu / Linux 优先使用：
	  /snap/bin/chromium
	  /usr/bin/google-chrome

	注意：
	- 如果你在 macOS 终端运行脚本，可以调用 Mac 本机 Google Chrome；
	- 如果你在 Parallels Ubuntu 虚拟机里运行脚本，不能直接调用 Mac 宿主机的 Chrome，
	  只能调用 Ubuntu 虚拟机里安装的 Chromium / Chrome。
	"""
	browser_path_candidates = [
		# macOS: Google Chrome / Chromium
		"/Applications/Google Chrome.app/Contents/MacOS/Google Chrome",
		# "/Applications/Google Chrome for Testing.app/Contents/MacOS/Google Chrome for Testing",
		# "/Applications/Chromium.app/Contents/MacOS/Chromium",

		# Linux / Ubuntu
		"/snap/bin/chromium",
		"/usr/bin/chromium",
		"/usr/bin/chromium-browser",
		"/usr/bin/google-chrome",
		"/usr/bin/google-chrome-stable",
	]

	executable_path = None
	for path in browser_path_candidates:
		if os.path.exists(path):
			executable_path = path
			break

	launch_args = [
		"--no-sandbox",
		"--disable-dev-shm-usage",
		"--disable-features=CalculateNativeWinOcclusion",
	]

	if executable_path:
		print(f"🌐 使用本机浏览器: {executable_path}")
		return p.chromium.launch(
			headless=False,
			executable_path=executable_path,
			args=launch_args,
		)

	# 如果是 macOS 且装了 Chrome，但路径检测失败，尝试 Playwright 的 channel=chrome。
	# 这个也会调用系统 Chrome，不需要 playwright install chromium。
	try:
		print("🌐 未找到固定路径，尝试使用 channel='chrome' 调用系统 Google Chrome")
		return p.chromium.launch(
			headless=False,
			channel="chrome",
			args=launch_args,
		)
	except Exception as e:
		raise RuntimeError(
			"没有找到可用的系统 Google Chrome / Chromium。\n"
			"macOS 请确认已经安装 Google Chrome：\n"
			"  /Applications/Google Chrome.app/Contents/MacOS/Google Chrome\n"
			"Ubuntu 请安装 Chromium：\n"
			"  sudo snap install chromium\n"
			f"原始错误：{e}"
		)


def polite_sleep(min_seconds=6, max_seconds=15, reason=""):
	wait_time = random.uniform(min_seconds, max_seconds)

	if reason:
		print(f"⏳ {reason}，等待 {wait_time:.2f} 秒...")
	else:
		print(f"⏳ 等待 {wait_time:.2f} 秒...")

	time.sleep(wait_time)


def clean_text(text):
	if text is None:
		return ""

	text = text.replace("\xa0", " ")
	text = text.replace("\r\n", "\n").replace("\r", "\n")

	lines = []
	for line in text.split("\n"):
		line = re.sub(r"[ \t]+", " ", line).strip()
		lines.append(line)

	text = "\n".join(lines)
	text = re.sub(r"\n{3,}", "\n\n", text)

	return text.strip()


def sanitize_filename(name):
	name = re.sub(r'[\\/*?:"<>|]', "_", name)
	name = re.sub(r"\s+", " ", name)
	return name.strip()


def save_debug_html(filename, html):
	with open(filename, "w", encoding="utf-8") as f:
		f.write(html)

	print(f"🧪 已保存调试页面: {filename}")


def looks_like_cloudflare(html):
	"""
	判断页面是否真的是 Cloudflare 验证页。

	注意：
	正常 Codeforces 页面也可能包含 Cloudflare 资源，
	所以不能只靠 challenge-platform / cloudflare 这些字符串判断。
	"""
	if not html:
		return False

	html_lower = html.lower()
	soup = BeautifulSoup(html, "html.parser")

	title_tag = soup.find("title")
	title_text = title_tag.get_text(" ", strip=True).lower() if title_tag else ""
	body_text = soup.get_text(" ", strip=True).lower()

	normal_markers = [
		"contest materials",
		"complete problemset",
		"dashboard -",
		"problem-statement",
		'class="datatable"',
		'href="/contests"',
		'href="/problemset"',
		"/contest/",
		"/problem/",
		"/blog/entry/",
	]

	if any(marker in html_lower for marker in normal_markers):
		return False

	challenge_title_markers = [
		"just a moment",
		"请稍候",
		"attention required",
	]

	if any(marker in title_text for marker in challenge_title_markers):
		return True

	challenge_body_markers = [
		"正在进行安全验证",
		"请验证您是真人",
		"verify you are human",
		"checking your browser",
		"enable javascript and cookies to continue",
	]

	if any(marker in body_text for marker in challenge_body_markers):
		return True

	if "cf-turnstile-response" in html_lower:
		return True

	if "__cf_chl" in html_lower and not any(marker in html_lower for marker in normal_markers):
		return True

	return False


def parse_problem_url(url):
	if not url:
		return None

	full_url = urljoin(BASE_URL, url)
	parsed = urlparse(full_url)
	path = parsed.path

	patterns = [
		r"^/contest/(\d+)/problem/([A-Za-z0-9]+)$",
		r"^/problemset/problem/(\d+)/([A-Za-z0-9]+)$",
		r"^/gym/(\d+)/problem/([A-Za-z0-9]+)$",
	]

	for pattern in patterns:
		m = re.match(pattern, path)

		if m:
			contest_id = m.group(1)
			problem_index = m.group(2).upper()

			return {
				"contest_id": contest_id,
				"problem_index": problem_index,
				"problem_url": f"{BASE_URL}/contest/{contest_id}/problem/{problem_index}",
			}

	return None


def replace_problem_index(problem_url, new_index):
	info = parse_problem_url(problem_url)

	if not info:
		return problem_url

	return f"{BASE_URL}/contest/{info['contest_id']}/problem/{new_index.upper()}"


def find_problem_link_in_tag(tag):
	candidates = []

	if tag.name == "a" and tag.has_attr("href"):
		candidates.append(tag)

	candidates.extend(tag.find_all("a", href=True))

	for a_tag in candidates:
		href = a_tag.get("href", "")
		info = parse_problem_url(href)

		if info:
			return info

	return None


def get_tutorial_url(page, contest_url):
	"""
	保持旧代码风格：
	page.goto -> page.content -> BeautifulSoup 遍历 a[href]
	"""
	print(f"🔍 正在访问比赛主页: {contest_url}")

	try:
		page.goto(contest_url, wait_until="domcontentloaded", timeout=60000)
		html_content = page.content()

		if looks_like_cloudflare(html_content):
			print("❌ 比赛主页返回的是 Cloudflare 验证页，跳过本场。")
			save_debug_html("debug_contest_cloudflare.html", html_content)
			return None

		soup = BeautifulSoup(html_content, "html.parser")

		backup_candidates = []

		for a_tag in soup.find_all("a", href=True):
			text = a_tag.get_text(strip=True).lower()
			href = a_tag["href"]

			if "/blog/entry/" not in href:
				continue

			full_url = urljoin(BASE_URL, href)

			if ("tutorial" in text or "editorial" in text) and "/blog/entry/" in href:
				print(f"🎯 成功找到题解链接: {full_url}")
				return full_url

			if "solution" in text or "sketch" in text or "analysis" in text:
				backup_candidates.append((text, full_url))

		if backup_candidates:
			print("⚠️ 没找到 Tutorial / Editorial，但找到备用题解链接：")
			for text, url in backup_candidates[:5]:
				print(f"   {text} -> {url}")

			print(f"🎯 使用备用题解链接: {backup_candidates[0][1]}")
			return backup_candidates[0][1]

		print("❌ 未能在页面找到题解 Tutorial / Editorial 链接。")
		return None

	except Exception as e:
		print(f"❌ 访问比赛页面失败: {e}")
		return None


def ensure_problem_entry(problems_data, key, problem_info=None, editorial_heading=""):
	if key not in problems_data:
		problems_data[key] = {
			"problem_info": problem_info,
			"editorial_heading": editorial_heading,
			"hints": [],
			"solutions": [],
		}

	if problem_info and not problems_data[key].get("problem_info"):
		problems_data[key]["problem_info"] = problem_info

	if editorial_heading and not problems_data[key].get("editorial_heading"):
		problems_data[key]["editorial_heading"] = editorial_heading

	return problems_data[key]


def extract_target_index_from_spoiler_title(title_text):
	title_text = clean_text(title_text)

	patterns = [
		r"\b(?:solution|tutorial|editorial|hint)\s+([A-Z]\d?)\b",
		r"^([A-Z]\d?)\s+(?:solution|tutorial|editorial|hint)\b",
	]

	for pattern in patterns:
		m = re.search(pattern, title_text, re.IGNORECASE)

		if m:
			return m.group(1).upper()

	return None


def parse_problem_heading_fallback(problem_heading, contest_id, fallback_index):
	s = re.sub(r"\s+", " ", problem_heading).strip()

	patterns = [
		r"^(?:CF)?\s*(\d{3,6})([A-Z]\d?)\s*[-–—.:]\s*(.+)$",
		r"^(?:Problem|Task)\s+([A-Z]\d?)\s*[\.\-–—:]?\s*(.+)$",
		r"^([A-Z]\d?)\s*[\.\-–—:]\s*(.+)$",
		r"^([A-Z]\d?)\s+(.+)$",
	]

	for pattern in patterns:
		m = re.match(pattern, s, re.IGNORECASE)

		if not m:
			continue

		if len(m.groups()) == 3:
			problem_index = m.group(2).upper()
			problem_title = m.group(3).strip()
			return f"CF{contest_id}{problem_index}", problem_title

		if len(m.groups()) == 2:
			problem_index = m.group(1).upper()
			problem_title = m.group(2).strip()
			return f"CF{contest_id}{problem_index}", problem_title

	return f"CF{contest_id}_Unknown{fallback_index}", (
		s if s != "Problem_Unknown" else f"Problem {fallback_index}"
	)


def scrape_cf_dynamic(page, tutorial_url):
	"""
	保持旧代码的 Hint / Solution 解析方式；
	新增：
	1. 用题目超链接识别真实题号；
	2. 相同题目重复出现时不重复打印。
	"""
	print(f"🚀 正在加载题解页面: {tutorial_url}")

	try:
		page.goto(tutorial_url, wait_until="domcontentloaded", timeout=60000)
		print("⏳ 等待 20 秒，让 JS 加载真正的题解内容...")
		page.wait_for_timeout(20000)
		html_content = page.content()

	except Exception as e:
		print(f"❌ 页面加载失败: {e}")
		return None

	if looks_like_cloudflare(html_content):
		print("❌ 题解页面返回的是 Cloudflare 验证页。")
		save_debug_html("debug_editorial_cloudflare.html", html_content)
		return None

	soup = BeautifulSoup(html_content, "html.parser")
	content_div = soup.find("div", class_="ttypography")

	if not content_div:
		print("❌ 未找到正文 div.ttypography，可能是网络太慢或页面不是题解正文。")
		save_debug_html("debug_no_ttypography.html", html_content)
		return None

	problems_data = {}
	current_key = "Problem_Unknown"
	current_problem_info = None
	current_heading = "Problem_Unknown"
	last_announced_key = None

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

	ensure_problem_entry(
		problems_data,
		current_key,
		problem_info=None,
		editorial_heading=current_heading,
	)

	for tag in content_div.find_all(True):
		text = tag.get_text(strip=True)
		is_new_problem = False
		problem_info = None

		if tag.name in ["h2", "h3", "h4"] and text:
			problem_info = find_problem_link_in_tag(tag)

			if problem_info:
				is_new_problem = True
			elif title_regex.match(text) and len(text) < 120:
				is_new_problem = True

		elif tag.name in ["p", "div"] and tag.find("strong"):
			problem_info = find_problem_link_in_tag(tag)

			if problem_info and len(text) < 160:
				is_new_problem = True
			elif text and title_regex.match(text) and len(text) < 120:
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

				if current_key != last_announced_key:
					print(f"🔗 识别题目: {current_heading} -> {current_key}")
					last_announced_key = current_key

			else:
				current_problem_info = None
				current_key = f"UNKNOWN::{current_heading}"

				if current_key != last_announced_key:
					print(f"⚠️ 识别到无链接题目: {current_heading}")
					last_announced_key = current_key

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

			title_tag = tag.find("b", class_="spoiler-title")
			if not title_tag:
				title_tag = tag.find(class_="spoiler-title")

			content_tag = tag.find("div", class_="spoiler-content")
			if not content_tag:
				content_tag = tag.find(class_="spoiler-content")

			if title_tag and content_tag:
				title_text = title_tag.get_text(strip=True)
				content_text = content_tag.get_text(separator="\n", strip=True)

				lower_title = title_text.lower()

				target_key = current_key
				target_problem_info = current_problem_info

				target_index = extract_target_index_from_spoiler_title(title_text)

				if target_index and current_problem_info:
					if target_index != current_problem_info["problem_index"]:
						new_url = replace_problem_index(
							current_problem_info["problem_url"],
							target_index,
						)

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
					problems_data[target_key]["hints"].append(
						{
							"title": title_text,
							"content": content_text,
						}
					)

				elif (
					"solution" in lower_title
					or "tutorial" in lower_title
					or "editorial" in lower_title
					or "implementation" in lower_title
				):
					problems_data[target_key]["solutions"].append(
						{
							"title": title_text,
							"content": content_text,
						}
					)

	return problems_data
def build_luogu_statement_text(content, samples):
	parts = []

	description = clean_text(content.get("description", ""))
	format_i = clean_text(content.get("formatI", ""))
	format_o = clean_text(content.get("formatO", ""))
	hint = clean_text(content.get("hint", ""))

	if description:
		parts.append("Description\n" + description)

	if format_i:
		parts.append("Input Format\n" + format_i)

	if format_o:
		parts.append("Output Format\n" + format_o)

	if samples:
		sample_parts = []
		for i, sample in enumerate(samples, start=1):
			if not isinstance(sample, list) or len(sample) < 2:
				continue

			sample_input = clean_text(sample[0])
			sample_output = clean_text(sample[1])

			sample_parts.append(
				f"Sample {i}\nInput\n{sample_input}\nOutput\n{sample_output}"
			)

		if sample_parts:
			parts.append("Examples\n" + "\n\n".join(sample_parts))

	if hint:
		parts.append("Note\n" + hint)

	return clean_text("\n\n".join(parts))


def parse_luogu_problem_from_html(html_content, expected_code):
	"""
	从洛谷页面的 script#lentille-context 里解析结构化题面。
	比 inner_text() 稳定很多。
	"""
	if not html_content:
		return None

	soup = BeautifulSoup(html_content, "html.parser")
	script = soup.find("script", id="lentille-context")

	if not script:
		return None

	try:
		data = json.loads(script.string or script.get_text())
	except Exception as e:
		print(f"⚠️ 解析洛谷 lentille-context JSON 失败: {e}")
		return None

	problem = data.get("data", {}).get("problem", {})

	pid = problem.get("pid", "")
	if pid.upper() != expected_code.upper():
		print(f"⚠️ 洛谷页面 pid 不匹配: expected={expected_code}, actual={pid}")
		return None

	content = problem.get("content") or {}
	samples = problem.get("samples") or []

	m = re.match(r"^CF(\d+)([A-Za-z][A-Za-z0-9]*)$", expected_code, re.IGNORECASE)
	if not m:
		return None

	contest_id = m.group(1)
	problem_index = m.group(2).upper()

	problem_title = problem.get("title") or content.get("name") or "Unknown Problem"
	statement_text = build_luogu_statement_text(content, samples)

	return {
		"problem_index": problem_index,
		"problem_code": f"CF{contest_id}{problem_index}",
		"problem_title": clean_text(problem_title),
		"statement": statement_text,
	}


def fetch_luogu_problem_statement(page, problem_info):
	"""
	从洛谷抓取题面：
	https://www.luogu.com.cn/problem/CF2222A

	关键修正：
	不再用 inner_text() 硬切页面文本；
	直接解析 script#lentille-context 里的 JSON。
	"""
	contest_id = problem_info["contest_id"]
	problem_index = problem_info["problem_index"]
	problem_code = f"CF{contest_id}{problem_index}"
	luogu_url = f"{LUOGU_BASE_URL}/{problem_code}"

	# polite_sleep(4, 10, f"准备从洛谷抓取题面 {problem_code} 前")
	print(f"📘 正在从洛谷抓取题面: {luogu_url}")

	try:
		page.goto(luogu_url, wait_until="domcontentloaded", timeout=60000)

		wait_ms = random.randint(5000, 6000)
		print(f"⏳ 等待洛谷题面加载 {wait_ms / 1000:.2f} 秒...")
		page.wait_for_timeout(wait_ms)

		html_content = page.content()

		info = parse_luogu_problem_from_html(html_content, problem_code)

		if info:
			print(f"✅ 洛谷题面解析成功: {info['problem_code']} {info['problem_title']}")
			return info

		print(f"❌ 洛谷题面解析失败: {luogu_url}")
		save_debug_html(f"debug_luogu_{problem_code}.html", html_content)
		return None

	except Exception as e:
		print(f"❌ 洛谷题面加载失败: {luogu_url}, error = {e}")
		return None



def build_output_json(statement_info, editorial_content):
	return {
		"statement": [
			{
				"title": "Statement",
				"content": statement_info.get("statement", ""),
			}
		],
		"hints": editorial_content.get("hints", []),
		"solutions": editorial_content.get("solutions", []),
	}


def make_unique_filename(save_dir, base_name):
	os.makedirs(save_dir, exist_ok=True)
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


def make_fallback_statement_info(problem_info, editorial_heading, fallback_idx):
	if problem_info:
		contest_id = problem_info["contest_id"]
		problem_index = problem_info["problem_index"]

		_, problem_title = parse_problem_heading_fallback(
			editorial_heading,
			contest_id,
			fallback_idx,
		)

		return {
			"problem_index": problem_index,
			"problem_code": f"CF{contest_id}{problem_index}",
			"problem_title": problem_title,
			"statement": "",
		}

	return {
		"problem_index": "Unknown",
		"problem_code": f"CF_Unknown{fallback_idx}",
		"problem_title": "Unknown Problem",
		"statement": "",
	}


def process_contest(contest_url, processed_tutorial_urls):
	"""
	保持旧代码结构：
	每个 contest 单独启动 browser/new_context。
	"""
	with sync_playwright() as p:
		browser = launch_system_chromium(p)
		context = browser.new_context()

		context.route("**/*mathjax*", lambda route: route.abort())
		context.route("**/*MathJax*", lambda route: route.abort())

		page = context.new_page()

		tutorial_url = get_tutorial_url(page, contest_url)

		if tutorial_url:
			if tutorial_url in processed_tutorial_urls:
				print(f"⚠️ 题解已经处理过，跳过重复 editorial: {tutorial_url}")
				browser.close()
				return

			processed_tutorial_urls.add(tutorial_url)

			data = scrape_cf_dynamic(page, tutorial_url)

			if data:
				os.makedirs(SAVE_DIR, exist_ok=True)
				os.makedirs(WITH_HINT_DIR, exist_ok=True)
				os.makedirs(WITHOUT_HINT_DIR, exist_ok=True)

				luogu_statement_cache = {}

				saved_count = 0
				with_hint_count = 0
				without_hint_count = 0

				for idx, (problem_key, content) in enumerate(data.items(), start=1):
					if not content["hints"] and not content["solutions"]:
						continue

					problem_info = content.get("problem_info")
					editorial_heading = content.get("editorial_heading", problem_key)

					if problem_info:
						cid = problem_info["contest_id"]
						problem_index = problem_info["problem_index"]
						problem_code = f"CF{cid}{problem_index}"

						if problem_code not in luogu_statement_cache:
							luogu_statement_cache[problem_code] = fetch_luogu_problem_statement(
								page,
								problem_info,
							)

						statement_info = luogu_statement_cache.get(problem_code)

						if not statement_info:
							statement_info = make_fallback_statement_info(
								problem_info,
								editorial_heading,
								idx,
							)
					else:
						contest_id = contest_url.rstrip("/").split("/")[-1]
						problem_code, problem_title = parse_problem_heading_fallback(
							editorial_heading,
							contest_id,
							idx,
						)

						statement_info = {
							"problem_index": "Unknown",
							"problem_code": problem_code,
							"problem_title": problem_title,
							"statement": "",
						}

					filename_base = (
						f"{statement_info['problem_code']} "
						f"{statement_info['problem_title']}.json"
					)
					filename_base = sanitize_filename(filename_base)

					# 分类规则：
					# 1. 有至少一个 Hint -> cf_dataset/with_hint/
					# 2. 没有 Hint 但有 Solution -> cf_dataset/without_hint/
					has_hint = bool(content.get("hints"))
					if has_hint:
						target_dir = WITH_HINT_DIR
						category_name = "with_hint"
					else:
						target_dir = WITHOUT_HINT_DIR
						category_name = "without_hint"

					filename = make_unique_filename(target_dir, filename_base)

					output_json = build_output_json(statement_info, content)

					with open(filename, "w", encoding="utf-8") as f:
						json.dump(output_json, f, ensure_ascii=False, indent=4)

					print(f"✅ 生成文件 [{category_name}]: {filename}")
					saved_count += 1
					if has_hint:
						with_hint_count += 1
					else:
						without_hint_count += 1

				print(
					f"🎉 题解 {tutorial_url} 爬取完毕，共保存了 {saved_count} 道有价值的题目！"
					f"其中有 Hint: {with_hint_count} 道，无 Hint: {without_hint_count} 道。\n"
				)

			else:
				print("❌ 题解抓取或解析失败。")

		browser.close()


if __name__ == "__main__":
	contests_to_scrape = [
		f"https://codeforces.com/contest/{i}"
		for i in range(1721, 999, -1)
	]

	processed_tutorial_urls = set()

	for contest in contests_to_scrape:
		process_contest(contest, processed_tutorial_urls)

		polite_sleep(3, 5, "本场比赛处理结束，进入下一场前")