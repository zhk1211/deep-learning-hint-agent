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
        int n_int = stoi(n_str);
        int len = (int)n_str.size();

        vector<pair<int, int>> ans;
        // a ranges from 1 to 10000
        for (int a = 1; a <= 10000; ++a) {
            // The correct answer is n_int * a - b
            // The string s = n_str repeated a times, length = len * a
            // After removing last b chars, length = len * a - b
            // This remaining string must equal the string representation of n_int * a - b
            // Let correct = n_int * a - b, which must be > 0
            // The remaining string length must equal the number of digits of correct
            // Also the remaining string must be a prefix of s, so it's n_str repeated some times possibly plus a prefix of n_str
            // Let k = number of full copies of n_str in the remaining string, and maybe a partial prefix
            // The remaining string length = len * a - b
            // The correct value = n_int * a - b
            // We can iterate over possible lengths of the remaining string, which is the number of digits of correct
            // Since a <= 10000, n_int <= 100, correct <= 1e6, so at most 7 digits
            // Let d = number of digits of correct = len * a - b
            // Then b = len * a - d
            // Also correct = n_int * a - b = n_int * a - (len * a - d) = a * (n_int - len) + d
            // So correct is determined by a and d
            // We need 1 <= b <= min(10000, a * n_int)
            // b = len * a - d, so 1 <= len * a - d <= min(10000, a * n_int)
            // Also correct > 0
            // And the remaining string (first d characters of s) must equal the string representation of correct
            // Since s is periodic with period n_str, the first d characters are just n_str repeated floor(d/len) times plus first d%len chars of n_str
            // So we can check for each a and each possible d (from 1 to len*a, but d is small because correct <= 1e6)
            // Actually correct = a*(n_int - len) + d, and correct <= 1e6 roughly, so d is bounded
            // We can just iterate d from 1 to min(len*a, 7) maybe? But correct can be up to 1e6, so d up to 7.
            // However, n_int - len can be negative if n_int < len (e.g., n=2, len=1, n_int=2, n_int - len = 1 >0; n=10, len=2, n_int=10, n_int-len=8 >0; n=100, len=3, n_int=100, n_int-len=97 >0)
            // Actually n_int >= len? For n=1..100, len=1..3, n_int >= len always? 1>=1, 2>=1, 10>=2, 100>=3. Yes.
            // So n_int - len >= 0.
            // correct = a*(n_int - len) + d. Since a up to 10000, n_int-len up to 99, product up to 990000, plus d up to 7, so correct <= 1e6, d <= 7.
            // So we can iterate d from 1 to 7 (or up to len*a, but len*a can be 30000, but d is small because correct has at most 7 digits)
            // Wait: correct could be larger if n_int=100, len=1? No, n=100 has len=3. n_int is the integer value, len is string length.
            // For n=2, len=1, n_int=2, n_int-len=1. a=10000 => correct = 10000*1 + d = 10000+d, which is 5 digits. So d up to 5.
            // For n=99, len=2, n_int=99, n_int-len=97. a=10000 => correct = 970000+d, 6 digits. d up to 6.
            // For n=100, len=3, n_int=100, n_int-len=97. a=10000 => correct = 970000+d, 6 digits. d up to 6.
            // So max digits is 6. Let's set max_d = 7 to be safe.
            // But wait: b = len*a - d must be >=1 and <= min(10000, a*n_int).
            // Also correct must be >0.
            // And the string condition: the first d characters of s (which is n_str repeated) must equal the string of correct.
            // We can precompute the infinite periodic string pattern from n_str.
            // For each a, we can try d from 1 to min(len*a, 7) (since correct's string length is d).
            // But correct's string length is d, and correct = a*(n_int - len) + d.
            // So we can just compute correct and check if its string length is indeed d, and then check if the prefix matches.
            // However, we must also ensure that b = len*a - d is within [1, min(10000, a*n_int)].
            // Since a*n_int can be up to 1e6, min(10000, a*n_int) is at most 10000.
            // So b <= 10000.
            // len*a - d <= 10000 => a <= (10000 + d)/len. Since len>=1, a <= 10000+7, so a up to 10000 is fine.
            // Also b >= 1 => len*a - d >= 1 => a >= (1+d)/len.
            // So we can just loop a from 1 to 10000, and for each a, loop d from 1 to min(len*a, 7) (or better, compute correct and its length).
            // Actually, we can compute correct = n_int * a - b, but b is unknown. We are iterating d = length of correct.
            // correct = a*(n_int - len) + d.
            // We need to check if the string of correct has length d and matches the prefix of s.
            // Since d is small, we can just construct the prefix of s of length d: it's just n_str repeated and truncated.
            // Then convert correct to string and compare.
            // Also need b = len*a - d to satisfy constraints.
            // But wait: Is it possible that correct has length d, but b = len*a - d is not the only way? We derived b from d = len*a - b, so b is determined by d.
            // So we just check all a and d.
            // However, a up to 10000, d up to 7, total operations 70000 per test case, t up to 100, total 7e6, easily fits in 1 second.
            // But we must output all valid pairs. The sample output for n=2 has 3 pairs, for n=3 has 1 pair, for n=10 has 1 pair.
            // Let's test with n=2: len=1, n_int=2, n_int-len=1.
            // a=20: d? correct = 20*1 + d = 20+d. b = 1*20 - d = 20-d. b=18 => d=2. correct=22, length 2. Prefix of s (20 times "2") of length 2 is "22". Matches. So valid.
            // a=219: d? b=216 => d = 219*1 - 216 = 3. correct = 219*1 + 3 = 222, length 3. Prefix of 219 times "2" length 3 is "222". Matches.
            // a=2218: b=2214 => d=4. correct=2218+4=2222, length 4. Prefix matches.
            // So pattern: a = something, d = something.
            // Our loop will find these.
            // But wait: The problem says "The solution gives the correct answer" means it outputs a non-empty string, and this string, when converted to an integer, equals the correct answer.
            // So the remaining string must be non-empty, which means b < len*a, so d >= 1. We already have d>=1.
            // Also the remaining string converted to integer equals n_int*a - b. That's exactly our condition.
            // So algorithm:
            // For each test case:
            //   read n as string n_str, compute n_int = stoi(n_str), len = n_str.size().
            //   ans.clear()
            //   for a in 1..10000:
            //       max_d = min(len * a, 7)  // because correct's length is at most 7
            //       for d in 1..max_d:
            //           b = len * a - d
            //           if b < 1 or b > min(10000, a * n_int): continue
            //           correct = n_int * a - b
            //           if correct <= 0: continue
            //           // build prefix of s of length d
            //           string prefix = "";
            //           for i in 0..d-1:
            //               prefix += n_str[i % len]
            //           if prefix == to_string(correct):
            //               ans.push_back({a, b})
            //   output ans.size(), then each pair
            // But wait: Is it possible that correct has length > 7? For n=100, a=10000, n_int=100, len=3, n_int-len=97, correct = 970000 + d. d up to 6, so correct up to 970006, length 6. So 7 is safe.
            // However, what if n_int - len is negative? As argued, n_int >= len always for n>=1? n=1: len=1, n_int=1, n_int-len=0. correct = 0*a + d = d. d up to 1? Actually correct = d, length of d is 1. So d=1. correct=1. b = 1*a - 1 = a-1. Constraints: b <= min(10000, a*1) = min(10000, a). So a-1 <= a always true, and a-1 >=1 => a>=2. Also b<=10000 => a-1<=10000 => a<=10001. So a from 2 to 10000. Let's test n=1: n_str="1", len=1, n_int=1. a=2, d=1: b=1, correct=2*1-1=1, prefix of "1"*2 length 1 is "1", matches. So (2,1) is valid? But sample doesn't have n=1. We'll see.
            // But wait: The problem says n is distinct across test cases, but we don't need to worry.
            // Let's check n=10 from sample: n_str="10", len=2, n_int=10, n_int-len=8.
            // Sample output: 1 pair: 1262 2519.
            // a=1262, b=2519. d = len*a - b = 2*1262 - 2519 = 2524 - 2519 = 5.
            // correct = n_int*a - b = 10*1262 - 2519 = 12620 - 2519 = 10101. Length 5.
            // Prefix of s (1262 times "10") length 5: "10101" (since "10" repeated: 1,0,1,0,1 -> "10101"). Matches.
            // So our algorithm will find it.
            // Complexity: For each a up to 10000, d up to 7, building prefix of length up to 7, to_string of up to 7 digits. Very fast.
            // But wait: max_d = min(len*a, 7) is not entirely correct because correct's length could be larger than 7 if n_int - len is large? But n_int <= 100, len <= 3, n_int - len <= 99. a <= 10000, product <= 990000. Plus d <= 7, so correct <= 990007, which is 6 digits. So 7 is safe. Actually 6 digits max. So we can set max_d = min(len*a, 6) but 7 is fine.
            // However, what if n_int - len is negative? Not possible.
            // So we can just loop d from 1 to min(len*a, 7).
            // But wait: Could there be cases where correct has length > 7? No, because n_int*a <= 100*10000 = 1,000,000. So correct <= 1,000,000. Length at most 7. So d <= 7.
            // So we can just loop d from 1 to min(len*a, 7).
            // But we also need to ensure that the prefix of s of length d equals to_string(correct). We can precompute the infinite string pattern? Not necessary.
            // Let's implement.
        }

        // Actually, we can optimize: For each a, we can compute the correct value directly from d, but we need to check if the string matches.
        // Let's code.
    }
    return 0;
}
