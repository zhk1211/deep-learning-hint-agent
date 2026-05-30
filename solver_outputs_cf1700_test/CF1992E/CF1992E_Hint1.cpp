// Hint1
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
        for (int a = 1; a <= 10000; ++a) {
            int total_len = len * a;
            int max_b = min(10000, n_int * a);
            for (int b = max(1, total_len - 7); b <= max_b; ++b) {
                if (b >= total_len) continue;
                int res_len = total_len - b;
                if (res_len <= 0 || res_len > 7) continue;
                int correct = n_int * a - b;
                string correct_str = to_string(correct);
                if ((int)correct_str.size() != res_len) continue;
                string formed;
                for (int i = 0; i < res_len; ++i) {
                    formed += n_str[i % len];
                }
                if (formed == correct_str) {
                    ans.emplace_back(a, b);
                }
            }
        }

        cout << ans.size() << '\n';
        for (auto [a, b] : ans) {
            cout << a << ' ' << b << '\n';
        }
    }
    return 0;
}
