#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        string n_str;
        cin >> n_str;
        int n = stoi(n_str);
        int len = (int)n_str.size();
        
        vector<pair<int, int>> ans;
        
        // We need to find all (a, b) such that:
        // 1 <= a <= 10000
        // 1 <= b <= min(10000, a * n)
        // The string s = n_str repeated a times
        // Remove last b characters -> resulting string must be non-empty
        // and its integer value equals n * a - b.
        
        // Let L = len * a (length of s)
        // After removing b chars, length = L - b > 0 => b < L
        // The resulting string is the first (L - b) characters of s.
        // This string represents an integer with (L - b) digits.
        // Its value must equal n * a - b.
        
        // Let the resulting string be T. T has length L - b.
        // Since s is periodic with period len, T is a prefix of s.
        // T must equal the decimal representation of n*a - b.
        
        // Key observation: n*a - b has at most len * a digits.
        // Actually, n*a - b < n*a, so it has at most the number of digits of n*a.
        // n*a has at most len * a digits (if n is like 99...9, it could be exactly len*a digits).
        // But T is exactly the first (L - b) characters of s.
        // So we need: the integer formed by first (L - b) chars of s equals n*a - b.
        
        // Let's denote d = L - b (number of digits of the answer).
        // Then b = L - d.
        // The answer value V = n*a - b = n*a - (L - d) = n*a - len*a + d = a*(n - len) + d.
        // Also V must equal the integer formed by first d characters of s.
        // Since s is n_str repeated, the first d characters are determined by d.
        // Let's write d = q * len + r, where 0 <= r < len.
        // Then the first d characters are: q full copies of n_str plus the first r characters of n_str.
        // So V = stoi(n_str repeated q times + n_str.substr(0, r)).
        
        // We have V = a*(n - len) + d.
        // Also V is a d-digit number (no leading zeros? n_str could start with '0'? n >= 1, so n_str[0] != '0').
        // Since n >= 1, n_str has no leading zeros, so V has exactly d digits.
        
        // We can iterate over possible d (number of digits of the answer).
        // d can range from 1 to len * 10000 (since a <= 10000).
        // But len <= 3 (n <= 100), so max d = 30000, which is too large.
        // However, we can iterate over a and compute possible b.
        
        // Alternative approach: iterate over a from 1 to 10000.
        // For each a, L = len * a.
        // We need b such that 1 <= b <= min(10000, a*n) and b < L.
        // The resulting string T = s.substr(0, L - b).
        // Its integer value must equal n*a - b.
        // Let d = L - b. Then b = L - d.
        // Condition: 1 <= L - d <= min(10000, a*n) and d > 0.
        // Also V = value of first d chars of s = n*a - (L - d) = n*a - len*a + d = a*(n - len) + d.
        
        // Notice that V is a d-digit number. Since n_str has no leading zeros, V's first digit is n_str[0].
        // Also V = a*(n - len) + d.
        // For fixed a, we can compute V for each possible d? But d can be up to L.
        // However, we can notice that V is determined by d: V = stoi(prefix of length d of s).
        // And we need V = a*(n - len) + d.
        // So a*(n - len) = V - d.
        // Since a is known, we can check if there exists d such that this holds.
        
        // But we can also iterate over possible lengths d of the answer.
        // d can be from 1 to len * 10000, but we can limit d because V = a*(n - len) + d must be a d-digit number.
        // Also a = (V - d) / (n - len) if n != len.
        // If n == len, then n is a single digit? Actually len is number of digits of n.
        // n <= 100, so len can be 1, 2, or 3.
        // If n == len, then n is 1? No, n=1 has len=1, so n=len=1. n=2 len=1, not equal. n=10 len=2, not equal.
        // So n == len only when n is a single-digit number? Actually len=1 for n=1..9, so n can equal len only if n=1? Wait, n is integer, len is its string length. For n=1, len=1, so n=len. For n=2..9, len=1, n != len. So only n=1 gives n=len.
        // We'll handle n=len separately.
        
        // For n != len:
        // a = (V - d) / (n - len). This must be integer between 1 and 10000.
        // Also b = len*a - d must satisfy 1 <= b <= min(10000, a*n) and b < len*a (i.e., d > 0).
        // And V must equal the prefix of length d of s, where s is n_str repeated a times.
        // But V is determined by d and a? Actually V is just the prefix of s of length d. But s depends on a.
        // However, the prefix of length d of s is simply the first d characters of the infinite repetition of n_str.
        // It does NOT depend on a, as long as a is large enough so that L >= d.
        // So for a given d, the value V_d = stoi(first d chars of infinite repetition of n_str) is fixed.
        // Then we need a = (V_d - d) / (n - len) to be an integer in [1, 10000].
        // And then we need to check if b = len*a - d satisfies the constraints.
        // Also we need L = len*a >= d (so that the prefix exists), which is equivalent to a >= ceil(d/len).
        // And we need b >= 1 => len*a - d >= 1 => a >= (d+1)/len.
        // And b <= a*n => len*a - d <= a*n => a*(n - len) >= -d, which is always true if n >= len? Actually n - len could be negative if n < len? But n <= 100, len up to 3. n can be less than len? For n=2, len=1, n > len. For n=99, len=2, n > len. For n=100, len=3, n > len. So n >= len always? n=1 len=1, n=2 len=1, n=10 len=2, n=99 len=2, n=100 len=3. So n >= len always, with equality only for n=1. So n - len >= 0.
        // So a*(n - len) >= 0, so b <= a*n is equivalent to len*a - d <= a*n => a*(n - len) + d >= 0, which is true.
        // Also b <= 10000.
        
        // So for each possible d (number of digits of the answer), we can compute V_d.
        // Then compute a = (V_d - d) / (n - len) if n != len.
        // Check if a is integer, 1 <= a <= 10000, and b = len*a - d satisfies 1 <= b <= min(10000, a*n) and b < len*a (d > 0).
        // Also we need the resulting string to be non-empty, which is d > 0.
        // And we need that the answer string T (first d chars of s) when converted to integer equals V_d, which it does by definition.
        // But wait: T is the first d chars of s, and s is n_str repeated a times. Since a >= ceil(d/len), the first d chars of s are exactly the first d chars of the infinite repetition. So V_d is correct.
        
        // However, we must ensure that the integer value of T is exactly V_d, and that T has no leading zeros. Since n_str has no leading zero, T won't have leading zeros.
        
        // What about n == len? That is n=1.
        // For n=1, len=1, n - len = 0.
        // Then the equation V = a*(0) + d = d.
        // So we need V_d = d.
        // V_d is the integer formed by first d characters of "1" repeated. That is a string of d ones, which is (10^d - 1)/9.
        // So we need (10^d - 1)/9 = d.
        // This only holds for d=1: 1 = 1. For d=2: 11 != 2. So only d=1 works.
        // Then a can be any? Wait, if n=1, len=1, then V = d = 1. So d=1.
        // Then b = len*a - d = a - 1.
        // Constraints: 1 <= b <= min(10000, a*n) = min(10000, a). So 1 <= a-1 <= min(10000, a) => a >= 2 and a-1 <= a (always) and a-1 <= 10000 => a <= 10001. Also a <= 10000 from problem. So a in [2, 10000].
        // Also b < len*a => a-1 < a, always true.
        // So for n=1, all a from 2 to 10000 give b = a-1, and the answer is "1" repeated a times minus last a-1 chars = "1", which equals 1 = 1*a - (a-1) = 1. Correct.
        // But wait, the sample for n=10 gives output with a=1262, b=2519. Let's check our logic for n=10.
        
        // Let's test our approach for n=10, len=2.
        // n - len = 8.
        // We need to find d such that a = (V_d - d) / 8 is integer in [1, 10000], and b = 2a - d in [1, min(10000, 10a)].
        // Sample output for n=10: one pair (1262, 2519). Let's check: a=1262, b=2519.
        // L = 2*1262 = 2524. d = L - b = 2524 - 2519 = 5.
        // V_d = first 5 chars of "10" repeated: "10101" = 10101.
        // Check: a*(n - len) + d = 1262*8 + 5 = 10096 + 5 = 10101. Yes.
        // So d=5 works.
        
        // So we can iterate over d. What is the maximum d?
        // d = L - b. Since b >= 1, d <= L - 1 = len*a - 1.
        // a <= 10000, len <= 3, so max d <= 30000 - 1 = 29999.
        // Also d is the number of digits of the answer. The answer n*a - b can be up to n*10000 - 1 <= 100*10000 = 1,000,000. So d <= 7? Actually 1,000,000 has 7 digits. But wait, n*a - b could be up to 1,000,000, which has 7 digits. But our d is the length of the string T, which is the first (L - b) characters of s. s has length up to 30000. So d can be up to 29999. But the integer value of T could be huge, up to 10^30000, which is way larger than n*a - b. However, the equation V = n*a - b forces V to be at most 1,000,000. So d cannot be large because V has at most 7 digits. Wait, V is the integer value of T. T is a string of length d. Its integer value is at least 10^{d-1} (since no leading zeros). So 10^{d-1} <= V = n*a - b <= 1,000,000. Thus d <= 7. Because 10^6 = 1,000,000 (7 digits), so d <= 7. Actually 10^6 has 7 digits, so d <= 7. Let's check: for n=2, sample has a=20, b=18, answer=22 (2 digits). a=219, b=216, answer=222? Wait, 2*219 - 216 = 438 - 216 = 222 (3 digits). a=2218, b=2214, answer=4436 - 2214 = 2222 (4 digits). So d is small.
        // So d is at most the number of digits of n*10000, which for n<=100 is at most 7 (since 100*10000=1,000,000 has 7 digits). So d <= 7.
        // That's a crucial observation! The answer n*a - b is at most 1,000,000, so it has at most 7 digits. Therefore, the string T must have length d <= 7.
        // But wait, is it possible that T has more digits? The problem says: "The solution gives the correct answer" means that it outputs a non-empty string, and this string, when converted to an integer, equals the correct answer, i.e., the value of n * a - b.
        // The correct answer is n*a - b, which is at most 1,000,000. So the string T, when converted to integer, must equal a number <= 1,000,000. Therefore, T cannot have more than 7 digits. If T had 8 digits, its integer value would be at least 10,000,000 > 1,000,000. So indeed d <= 7.
        // This drastically reduces the search space! We only need to consider d from 1 to 7.
        
        // Let's verify with the sample: n=2, d=2,3,4. n=3, d=3? Sample output for n=3: a=165, b=162. n*a - b = 495 - 162 = 333 (3 digits). So d=3. n=10: a=1262, b=2519, answer=12620 - 2519 = 10101 (5 digits). So d=5.
        // So d is indeed small.
        
        // Therefore, we can iterate d from 1 to 7 (or maybe up to 10 to be safe, but 7 is enough since max n=100, max a=10000, max product=1,000,000, which is 7 digits. Actually 1,000,000 is 7 digits, so d <= 7. But what if n=100, a=10000, b=1, then answer=999,999? That's 6 digits. So max digits is 7 for 1,000,000? 1,000,000 has 7 digits (1 followed by 6 zeros). So d <= 7.
        // However, we should also consider that n*a - b could be 1,000,000 exactly? b >= 1, so n*a - b <= n*a - 1 <= 1,000,000 - 1 = 999,999 (6 digits). So actually max is 6 digits? Wait, n=100, a=10000, n*a = 1,000,000. b >= 1, so max answer is 999,999 (6 digits). But if n=100, a=9999, n*a=999,900, b=1, answer=899,999? Actually 999,900 - 1 = 899,999? No, 999,900 - 1 = 899,999? That's wrong: 999,900 - 1 = 999,899 (6 digits). So max answer is 999,999 when n=100, a=10000, b=1? But b <= min(10000, a*n) = min(10000, 1,000,000) = 10000. So b can be 1. Then answer = 1,000,000 - 1 = 999,999 (6 digits). So max digits is 6. But wait, 1,000,000 is 7 digits, but we subtract at least 1, so it becomes 6 digits. However, if n=100, a=10000, b=0 is not allowed. So max answer is 999,999 (6 digits). But what about n=99, a=10000, n*a=990,000, b=1, answer=989,999 (6 digits). So d <= 6. But let's be safe and check up to 7. It won't hurt.
        
        // So we iterate d from 1 to 7.
        // For each d, we need to compute V_d = value of first d characters of the infinite repetition of n_str.
        // Since d <= 7, we can just build the string by repeating n_str until length >= d, then take substr.
        string repeated;
        while ((int)repeated.size() < d) repeated += n_str;
        string T = repeated.substr(0, d);
        long long V = stoll(T);
        
        // Now we need a = (V - d) / (n - len) if n != len.
        if (n != len) {
            if ((V - d) % (n - len) == 0) {
                long long a = (V - d) / (n - len);
                if (a >= 1 && a <= 10000) {
                    long long b = (long long)len * a - d;
                    if (b >= 1 && b <= min(10000LL, (long long)n * a) && b < (long long)len * a) {
                        ans.push_back({(int)a, (int)b});
                    }
                }
            }
        } else {
            // n == len, which means n=1, len=1.
            // We need V = d.
            if (V == d) {
                // Then a can be any? Actually from equation: V = a*(0) + d = d, so any a works as long as b constraints hold.
                // b = len*a - d = a - d.
                // For d=1, b = a - 1.
                // Constraints: 1 <= b <= min(10000, a*n) = min(10000, a) => a-1 >= 1 => a >= 2, and a-1 <= a (always), a-1 <= 10000 => a <= 10001. Also a <= 10000. So a in [2, 10000].
                // Also b < len*a => a-1 < a, always.
                // So for n=1, d=1, all a from 2 to 10000 work.
                // But wait, the problem says n is distinct across test cases, so n=1 will appear at most once.
                if (d == 1) {
                    for (int a = 2; a <= 10000; a++) {
                        int b = a - 1;
                        ans.push_back({a, b});
                    }
                }
                // For d > 1, V_d = 11...1 != d, so no solutions.
            }
        }
        
        // We might have duplicates? Could different d give the same (a,b)? Possibly, but we can use a set to avoid duplicates.
        // Since we iterate d from 1 to 7, and a is determined, we might get the same (a,b) from different d? Let's check: For a given (a,b), d = len*a - b is uniquely determined. So each (a,b) corresponds to exactly one d. So no duplicates.
        
        // Now we need to output the number of pairs and the pairs themselves.
        // The sample output for n=2 has 3 pairs, for n=3 has 1 pair, for n=10 has 1 pair.
        // Our logic for n=2: len=1, n-len=1.
        // d=2: V_d = first 2 chars of "2" repeated = "22" = 22. a = (22 - 2)/1 = 20. b = 1*20 - 2 = 18. Valid.
        // d=3: V_d = "222" = 222. a = (222 - 3)/1 = 219. b = 219 - 3 = 216. Valid.
        // d=4: V_d = "2222" = 2222. a = (2222 - 4)/1 = 2218. b = 2218 - 4 = 2214. Valid.
        // d=5: V_d = 22222. a = 22222 - 5 = 22217. But a <= 10000? 22217 > 10000, so invalid.
        // So we get exactly the 3 pairs.
        // For n=3: len=1, n-len=2.
        // d=3: V_d = "333" = 333. a = (333 - 3)/2 = 165. b = 165 - 3 = 162. Valid.
        // d=4: V_d = 3333. a = (3333 - 4)/2 = 3329/2 not integer.
        // d=5: V_d = 33333. a = (33333 - 5)/2 = 33328/2 = 16664 > 10000.
        // So only one.
        // For n=10: len=2, n-len=8.
        // d=5: V_d = first 5 of "10" repeated = "10101" = 10101. a = (10101 - 5)/8 = 10096/8 = 1262. b = 2*1262 - 5 = 2524 - 5 = 2519. Valid.
        // Other d? d=1: V=1, a=(1-1)/8=0 invalid. d=2: V=10, a=(10-2)/8=1, b=2*1-2=0 invalid. d=3: V=101, a=(101-3)/8=98/8 not int. d=4: V=1010, a=(1010-4)/8=1006/8 not int. d=6: V=101010, a=(101010-6)/8=101004/8=12625.5 no. d=7: V=1010101, a=(1010101-7)/8=1010094/8 not int.
        // So only one.
        
        // So our logic works!
        
        // We need to output the number of pairs and the pairs. The order? The sample output seems to be sorted by a? For n=2: 20 18, 219 216, 2218 2214. That's increasing a. So we should sort by a.
        sort(ans.begin(), ans.end());
        
        cout << ans.size() << "\n";
        for (auto &p : ans) {
            cout << p.first << " " << p.second << "\n";
        }
    }
    
    return 0;
}
