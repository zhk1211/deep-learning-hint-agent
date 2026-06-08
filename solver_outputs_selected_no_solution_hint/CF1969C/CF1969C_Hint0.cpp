#include <bits/stdc++.h>
using namespace std;

const int INF = 1e18;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n, k;
        cin >> n >> k;
        vector<long long> a(n);
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }
        
        // dp[i][j] = min sum for prefix up to i, with j operations used,
        // and the last element is a[i] (not changed) or some value from left.
        // We'll use a different approach: for each position, we can decide to
        // make a segment of equal values.
        
        // Let dp[i][j] = min sum for first i elements using j operations.
        // We'll consider segments ending at i.
        vector<vector<long long>> dp(n + 1, vector<long long>(k + 1, INF));
        dp[0][0] = 0;
        
        for (int i = 1; i <= n; i++) {
            // We can keep a[i-1] as is, no operation needed for this element
            // but we might have used operations before.
            for (int j = 0; j <= k; j++) {
                if (dp[i-1][j] != INF) {
                    dp[i][j] = min(dp[i][j], dp[i-1][j] + a[i-1]);
                }
            }
            
            // Try to extend a segment backwards: we make a segment [l, i] all equal to min value in that segment.
            // The number of operations needed is (i - l) because we change all except one (the min) to the min.
            // Actually, we can choose any value from the segment to be the final value, but to minimize sum we pick the minimum.
            long long min_val = a[i-1];
            for (int l = i-1; l >= 1; l--) {
                min_val = min(min_val, a[l-1]);
                int ops = i - l; // number of elements we change in this segment
                // We need to use ops operations for this segment.
                for (int j = ops; j <= k; j++) {
                    if (dp[l-1][j-ops] != INF) {
                        dp[i][j] = min(dp[i][j], dp[l-1][j-ops] + min_val * (i - l + 1));
                    }
                }
            }
        }
        
        long long ans = INF;
        for (int j = 0; j <= k; j++) {
            ans = min(ans, dp[n][j]);
        }
        cout << ans << '\n';
    }
    return 0;
}
