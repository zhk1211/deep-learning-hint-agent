// Solution
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int t;
    cin >> t;
    while (t--) {
        int n, m;
        cin >> n >> m;
        vector<vector<int>> a(n, vector<int>(m + 1));
        for (int i = 0; i < n; ++i) {
            for (int j = 1; j <= m; ++j) {
                cin >> a[i][j];
            }
        }

        unordered_set<long long> prefixes;
        for (int i = 0; i < n; ++i) {
            vector<int> inv(m + 1);
            for (int j = 1; j <= m; ++j) {
                inv[a[i][j]] = j;
            }
            long long code = 0;
            for (int k = 1; k <= m; ++k) {
                code = code * 11 + inv[k];
                prefixes.insert(code);
            }
        }

        vector<int> ans(n);
        for (int i = 0; i < n; ++i) {
            long long code = 0;
            int best = 0;
            for (int k = 1; k <= m; ++k) {
                code = code * 11 + a[i][k];
                if (prefixes.find(code) != prefixes.end()) {
                    best = k;
                } else {
                    break;
                }
            }
            ans[i] = best;
        }

        for (int i = 0; i < n; ++i) {
            cout << ans[i] << " \n"[i == n - 1];
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}
