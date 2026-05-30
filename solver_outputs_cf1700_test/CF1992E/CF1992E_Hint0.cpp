// Hint0
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
            // total length of repeated string
            int total_len = len * a;
            // b must be at most min(10000, a * n_int) and also less than total_len to keep non-empty
            int max_b = min(10000, a * n_int);
            // We need to find b such that:
            // 1) 1 <= b <= max_b
            // 2) b < total_len (non-empty result)
            // 3) The resulting string after removing last b chars equals the integer n_int * a - b
            // The resulting string length is total_len - b.
            // Let the correct answer be C = n_int * a - b.
            // The string representation of C must exactly match the prefix of length (total_len - b) of the repeated n_str.
            // Since C can have at most 7 digits (since a<=10000, n<=100, so n*a <= 1e6, minus b <= 1e4, still <= 1e6, so up to 7 digits),
            // we can check possible lengths of C.
            // Let L = number of digits of C. Then total_len - b = L, so b = total_len - L.
            // Also C = n_int * a - b = n_int * a - (total_len - L) = n_int * a - total_len + L.
            // So we can iterate over possible L (from 1 to 7) and compute b, then check constraints.
            for (int L = 1; L <= 7; ++L) {
                int b = total_len - L;
                if (b < 1 || b > max_b || b >= total_len) continue;
                int C = n_int * a - b;
                if (C <= 0) continue;
                // Check if C has exactly L digits
                string C_str = to_string(C);
                if ((int)C_str.size() != L) continue;
                // Build the prefix of repeated n_str of length L
                string prefix;
                // We can build by taking characters from repeated n_str
                // Since L <= 7, we can just build a long enough string
                string repeated;
                int need = (L + len - 1) / len;
                for (int i = 0; i < need; ++i) repeated += n_str;
                prefix = repeated.substr(0, L);
                if (prefix == C_str) {
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
