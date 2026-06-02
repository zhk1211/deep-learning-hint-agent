#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n, m, k, q;
        cin >> n >> m >> k >> q;
        vector<pair<int, int>> ops(q);
        for (int i = 0; i < q; ++i) {
            cin >> ops[i].first >> ops[i].second;
        }

        vector<bool> row_used(n + 1, false), col_used(m + 1, false);
        int row_count = 0, col_count = 0;
        long long ans = 1;

        for (int i = q - 1; i >= 0; --i) {
            int x = ops[i].first, y = ops[i].second;
            bool useful = false;
            if (!row_used[x] && col_count < m) {
                row_used[x] = true;
                row_count++;
                useful = true;
            }
            if (!col_used[y] && row_count < n) {
                col_used[y] = true;
                col_count++;
                useful = true;
            }
            if (useful) {
                ans = (ans * k) % MOD;
            }
        }
        cout << ans << '\n';
    }
    return 0;
}
