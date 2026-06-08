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

        // We need to find a, b such that:
        // 1 <= a <= 10000
        // 1 <= b <= min(10000, a * n_int)
        // The string s = n_str repeated a times
        // Removing last b characters gives a non-empty string
        // That string converted to integer equals n_int * a - b

        // Let L = len * a (length of s)
        // We remove b characters, so remaining length = L - b > 0 => b < L
        // The remaining string is the prefix of s of length L - b.
        // This prefix must equal the decimal representation of (n_int * a - b).

        // Let val = n_int * a - b.
        // The string representation of val must be exactly the prefix of s of length L - b.
        // Since s is just n_str repeated, the prefix of length L - b is formed by some number of full copies of n_str plus a prefix of n_str.

        // Let k = (L - b) / len (number of full copies of n_str in the prefix)
        // Let r = (L - b) % len (remaining characters from the next copy)
        // Then the prefix string is n_str repeated k times + first r characters of n_str.
        // This string must equal the string representation of val.

        // Also val = n_int * a - b.
        // We can iterate over possible a (1..10000) and possible lengths of val.
        // But val can be up to n_int * a <= 100 * 10000 = 1e6, so length up to 7.
        // However, L = len * a can be up to 3 * 10000 = 30000 (since n <= 100, len <= 3).
        // So we can just iterate a from 1 to 10000, compute L = len * a.
        // Then we need b such that:
        // 1 <= b <= min(10000, a * n_int)
        // b < L
        // The prefix of length L - b equals to_string(n_int * a - b)

        // Instead of iterating b, we can iterate over possible lengths of val.
        // Let val_len = number of digits of val.
        // val_len = L - b => b = L - val_len.
        // Conditions: b >= 1, b <= min(10000, a * n_int), b < L => val_len >= 1.
        // Also val = n_int * a - b = n_int * a - (L - val_len) = n_int * a - len * a + val_len = a * (n_int - len) + val_len.
        // But this doesn't directly give val.
        // Instead, we can just check for each a, and for each possible val_len from 1 to min(7, L-1) maybe?
        // But val can be up to 1e6, length up to 7. However, L can be up to 30000, so val_len can be up to 30000? No, val is at most 1e6, so val_len <= 7.
        // Wait: val = n_int * a - b. Since n_int <= 100, a <= 10000, n_int * a <= 1e6. b >= 1, so val <= 1e6 - 1. So val has at most 7 digits.
        // So val_len is small (1..7). But L - b = val_len, so b = L - val_len. Since b <= 10000, L - val_len <= 10000 => L <= 10000 + val_len <= 10007.
        // So len * a <= 10007 => a <= 10007 / len. Since len >= 1, a <= 10007. So a is at most 10007, which is fine.

        // So we can iterate a from 1 to 10000, compute L = len * a.
        // For each possible val_len from 1 to 7 (or until L-1), compute b = L - val_len.
        // Check if b is in [1, min(10000, a * n_int)].
        // Compute val = n_int * a - b.
        // Check if to_string(val) has length val_len (it should, but just in case).
        // Check if the prefix of s of length val_len equals to_string(val).
        // Since s is n_str repeated, we can construct the prefix by taking characters from n_str cyclically.

        // To avoid constructing large strings, we can just compare character by character.
        // But since a <= 10000 and len <= 3, L <= 30000, we can just build the string s for each a? That would be O(a * len) per a, total O(10000 * 3 * 10000) = 3e8, too slow.
        // We need a faster way.

        // Notice that val_len is small (<=7). So we only need to check the first val_len characters of s.
        // We can compute the prefix of s of length val_len without building the whole string.
        // The prefix is just the first val_len characters of n_str repeated.
        // We can generate it by taking n_str[(i % len)] for i=0..val_len-1.

        // Also, val = n_int * a - b. Since b = L - val_len = len * a - val_len,
        // val = n_int * a - (len * a - val_len) = a * (n_int - len) + val_len.
        // So val is determined by a and val_len.
        // Let d = n_int - len.
        // Then val = a * d + val_len.
        // We need val > 0 (since non-empty string and positive integer? Actually val could be 0? The problem says "outputs a non-empty string, and this string, when converted to an integer, equals the correct answer". The correct answer is n*a - b. If n*a - b = 0, the string "0" is non-empty, but is 0 allowed? The constraints: a >=1, b >=1, n>=1. n*a - b could be 0. But the problem says "non-empty string", and "converted to an integer". Usually "0" is allowed. But let's check: if val=0, then string is "0", length 1. That's fine. But we'll see if any test case has val=0. We'll allow val >= 0.
        // But val_len is the length of string representation of val. For val=0, length is 1.
        // So val_len = number of digits of val (with no leading zeros). Since val is computed as a*d + val_len, it might have leading zeros? No, val is an integer, its string representation has no leading zeros except "0".
        // So we need to ensure that the string we compare has no leading zeros, which it won't if we just convert val to string.

        // So algorithm:
        // For each test case:
        // n_int = stoi(n_str), len = n_str.size().
        // For a from 1 to 10000:
        //   L = len * a
        //   For val_len from 1 to 7 (or maybe up to 7, but also val_len < L):
        //     if val_len >= L: break (since b = L - val_len would be <=0)
        //     b = L - val_len
        //     if b < 1 or b > min(10000, a * n_int): continue
        //     val = a * (n_int - len) + val_len
        //     if val < 0: continue (though n_int >=1, len>=1, n_int - len could be negative if n_int < len, e.g., n=2, len=1, n_int - len = 1 >0; n=10, len=2, n_int - len = 8 >0; n=100, len=3, n_int - len = 97 >0. Actually n_int >= len? For n=2, len=1, yes. For n=99, len=2, 99>2. For n=100, len=3, 100>3. So n_int >= len always? n can be 1..100. len is number of digits. For n=1..9, len=1, n_int >=1. For n=10..99, len=2, n_int >=10 >2. For n=100, len=3, n_int=100 >3. So n_int - len >= 0 always. So val >= val_len >=1. So val >0 always.)
        //     // Now check if to_string(val) matches the prefix of s of length val_len.
        //     // Generate prefix of s of length val_len:
        //     bool ok = true;
        //     string val_str = to_string(val);
        //     if ((int)val_str.size() != val_len) continue; // just in case
        //     for (int i = 0; i < val_len; ++i) {
        //         if (val_str[i] != n_str[i % len]) {
        //             ok = false;
        //             break;
        //         }
        //     }
        //     if (ok) {
        //         ans.emplace_back(a, b);
        //     }

        // But wait: val_len can be up to 7, but what if val is larger? n_int * a <= 100 * 10000 = 1e6, so val <= 1e6, so max digits = 7. So val_len <= 7.
        // However, we also need to consider that val_len could be larger if a is small? No, val is bounded by 1e6.
        // So val_len is at most 7.
        // But is it possible that val has more digits? No, because n_int * a <= 1e6, b >=1, so val <= 999999, which is 6 digits. Actually 1e6 is 7 digits (1000000). But n_int=100, a=10000 gives 1e6, b>=1 so val <= 999999 (6 digits). So max digits = 6? Wait: 100 * 10000 = 1,000,000. If b=1, val=999,999 (6 digits). If b=0 not allowed. So val <= 999,999. So max digits = 6. But we can just loop up to 7 to be safe.

        // But wait: The problem says b <= min(10000, a * n). So b can be up to 10000. So a * n could be larger than 10000, but b is capped at 10000. So val = a*n - b could be larger than 1e6? No, a*n <= 10000 * 100 = 1e6. So val <= 1e6 - 1 = 999999. So 6 digits max.

        // So val_len from 1 to 6 (or 7) is enough.

        // However, there is a catch: The string s is n_str repeated a times. The prefix of length val_len is just the first val_len characters of that repetition. Since val_len is small, we can just check.

        // But wait: The problem statement says "when subtracting the integer b from the string s, the last b characters will be removed from it. If b is greater than or equal to the length of the string s, it will become empty." So we require non-empty, so b < L, which we already have (val_len >= 1).

        // Now, is it possible that val has leading zeros? No, because val is positive integer, to_string doesn't produce leading zeros.

        // So the algorithm seems correct.

        // Let's test with sample:
        // n=2: n_str="2", len=1, n_int=2.
        // a=20: L=20, val_len? We need to find val_len such that b = 20 - val_len, and val = 20*(2-1) + val_len = 20 + val_len.
        // For val_len=2: b=18, val=22. val_str="22". Prefix of s of length 2: s is "2"*20 = "2222...", first 2 chars "22". Matches. So (20,18) is found.
        // a=219: L=219, val_len=3? b=216, val=219*(1)+3=222? Wait: n_int - len = 2-1=1. val = 219*1 + 3 = 222. val_str="222", length 3. Prefix of s of length 3: "222". Matches. So (219,216).
        // a=2218: L=2218, val_len=4? b=2214, val=2218*1+4=2222. val_str="2222". Prefix of length 4: "2222". Matches.
        // So sample output matches.

        // n=3: n_str="3", len=1, n_int=3, d=2.
        // a=165: L=165, val_len=3? b=162, val=165*2+3=333. val_str="333", length 3. Prefix of s: "333". Matches.
        // So (165,162).

        // n=10: n_str="10", len=2, n_int=10, d=8.
        // a=1262: L=2524, val_len=5? b=2519, val=1262*8+5=10096+5=10101? Wait: 1262*8 = 10096, +5 = 10101. val_str="10101", length 5. Prefix of s: s = "10" repeated 1262 times = "101010...". First 5 chars: "10101". Matches.
        // So (1262,2519).

        // So the logic works.

        // Now, we need to output the number of pairs and the pairs themselves.
        // The order of pairs? The sample output seems to be in increasing order of a? 20, 219, 2218. So we can just collect and sort by a, or just output in the order we find them (which is increasing a since we loop a from 1 to 10000). But we also loop val_len from 1 to 6. For a given a, there might be multiple val_len that work? Let's check: For a=20, val_len=2 works. Could val_len=1 work? val_len=1: b=19, val=20+1=21, val_str="21", prefix of s length 1: "2". Not match. So only one per a? Possibly multiple? We should check all val_len. The sample output for n=2 has 3 pairs, each with different a. So it seems each a gives at most one valid pair? Not necessarily, but we can just collect all.

        // Complexity: For each test case, a up to 10000, val_len up to 6. So ~60,000 iterations per test case. t up to 100, total 6e6 iterations, very fast.

        // But wait: The constraints say n is distinct across test cases. So t <= 100, but n distinct. That doesn't affect complexity.

        // Let's implement.

        int d = n_int - len;
        for (int a = 1; a <= 10000; ++a) {
            int L = len * a;
            // val_len can be from 1 to 6 (or up to 7). But also val_len < L.
            int max_val_len = min(6, L - 1);
            for (int val_len = 1; val_len <= max_val_len; ++val_len) {
                int b = L - val_len;
                if (b < 1 || b > min(10000, a * n_int)) continue;
                int val = a * d + val_len;
                // val should be positive, but it is.
                string val_str = to_string(val);
                if ((int)val_str.size() != val_len) continue;
                // check prefix
                bool ok = true;
                for (int i = 0; i < val_len; ++i) {
                    if (val_str[i] != n_str[i % len]) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    ans.emplace_back(a, b);
                }
            }
        }

        // Output
        cout << ans.size() << '\n';
        for (auto &p : ans) {
            cout << p.first << ' ' << p.second << '\n';
        }
    }

    return 0;
}
