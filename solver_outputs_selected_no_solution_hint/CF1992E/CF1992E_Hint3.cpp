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
            // The string s = n_str repeated a times
            // Its length is a * len
            // b must be less than a * len to have non-empty result
            // Also b <= min(10000, a * n_int)
            int max_b = min(10000, a * n_int);
            // The result string length is a*len - b
            // The correct numerical answer is a * n_int - b
            // The result string must represent exactly that number
            // The number of digits of correct answer is either len(ans) or something else
            // Let L = a*len - b (length of resulting string)
            // The resulting string is the prefix of s of length L
            // We need that this prefix, interpreted as integer, equals a*n_int - b
            // Since n_str is small (len <= 3 for n<=100), we can try possible lengths
            // The correct answer has at most 7 digits (a*n_int <= 10000*100 = 1e6, minus b)
            // So L is small (<=7). We can iterate over possible L?
            // But a*len can be up to 10000*3 = 30000, so L can be large, but the number
            // represented by the prefix is at most 1e6, so L cannot be larger than 7.
            // Actually if L > 7, the prefix would be a number > 1e6, but correct answer <= 1e6,
            // so they can't match. So we only care about L <= 7.
            // Thus b = a*len - L, with 1 <= L <= min(7, a*len)
            // Also b >= 1 and b <= min(10000, a*n_int)
            for (int L = 1; L <= min(7, a * len); ++L) {
                int b = a * len - L;
                if (b < 1 || b > min(10000, a * n_int)) continue;
                // Build the prefix of s of length L
                string prefix;
                // s is n_str repeated a times, we need first L characters
                for (int i = 0; i < L; ++i) {
                    prefix += n_str[i % len];
                }
                // Convert to integer
                long long val = stoll(prefix);
                if (val == (long long)a * n_int - b) {
                    ans.emplace_back(a, b);
                }
            }
        }

        cout << ans.size() << '\n';
        for (auto &p : ans) {
            cout << p.first << ' ' << p.second << '\n';
        }
    }
    return 0;
}
