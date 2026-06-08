#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n, k;
        cin >> n >> k;
        vector<long long> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }
        
        if (k >= n) {
            long long sum = accumulate(a.begin(), a.end(), 0LL);
            cout << sum << "\n";
            continue;
        }
        
        const long long INF = 1e18;
        vector<vector<long long>> dp(n + 1, vector<long long>(k + 1, INF));
        dp[0][0] = 0;
        
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j <= k; ++j) {
                if (dp[i][j] == INF) continue;
                long long min_val = a[i];
                for (int len = 1; len <= k - j + 1 && i + len <= n; ++len) {
                    min_val = min(min_val, a[i + len - 1]);
                    int nj = j + len - 1;
                    dp[i + len][nj] = min(dp[i + len][nj], dp[i][j] + min_val * len);
                }
            }
        }
        
        long long ans = INF;
        for (int j = 0; j <= k; ++j) {
            ans = min(ans, dp[n][j]);
        }
        cout << ans << "\n";
    }
    
    return 0;
}
