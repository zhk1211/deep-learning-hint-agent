// Solution
#include <bits/stdc++.h>
using namespace std;

const long long INF = 1e18;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n, m;
        cin >> n >> m;
        vector<long long> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }
        vector<long long> b(m);
        for (int i = 0; i < m; ++i) {
            cin >> b[i];
        }
        
        vector<long long> pref(n + 1, 0);
        for (int i = 0; i < n; ++i) {
            pref[i + 1] = pref[i] + a[i];
        }
        
        vector<vector<long long>> dp(n + 1, vector<long long>(m + 1, INF));
        dp[0][1] = 0;
        
        for (int j = 1; j <= m; ++j) {
            int r = 0;
            for (int i = 0; i <= n; ++i) {
                if (dp[i][j] >= INF) continue;
                if (j < m) {
                    dp[i][j + 1] = min(dp[i][j + 1], dp[i][j]);
                }
                r = max(r, i);
                while (r < n && pref[r + 1] - pref[i] <= b[j - 1]) {
                    ++r;
                }
                if (r > i) {
                    dp[r][j] = min(dp[r][j], dp[i][j] + (m - j));
                }
            }
        }
        
        long long ans = INF;
        for (int j = 1; j <= m; ++j) {
            ans = min(ans, dp[n][j]);
        }
        if (ans >= INF) {
            cout << -1 << '\n';
        } else {
            cout << ans << '\n';
        }
    }
    return 0;
}
