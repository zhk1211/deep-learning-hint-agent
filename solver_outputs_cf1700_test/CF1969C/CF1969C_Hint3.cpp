// Hint3
#include <bits/stdc++.h>
using namespace std;

const long long INF = 1e18;

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
        
        // dp[i][j] = min sum for prefix up to i, with j operations used,
        // and we consider the last element as fixed (not being changed further)
        // We'll use a 2D DP with dimensions (n+1) x (k+1)
        vector<vector<long long>> dp(n + 1, vector<long long>(k + 1, INF));
        dp[0][0] = 0;
        
        for (int i = 0; i < n; ++i) {
            // For each position i, we consider it as the start of a segment
            // that will be replaced by the minimum value in that segment.
            // We can extend the segment up to k operations.
            long long min_val = a[i];
            for (int len = 1; len <= k + 1 && i + len <= n; ++len) {
                min_val = min(min_val, a[i + len - 1]);
                int ops = len - 1;
                for (int used = 0; used + ops <= k; ++used) {
                    if (dp[i][used] != INF) {
                        dp[i + len][used + ops] = min(dp[i + len][used + ops], dp[i][used] + min_val * len);
                    }
                }
            }
        }
        
        long long ans = INF;
        for (int j = 0; j <= k; ++j) {
            ans = min(ans, dp[n][j]);
        }
        cout << ans << '\n';
    }
    return 0;
}
