// Hint1
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
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }

        // dp[i][j] = min sum for prefix up to i, having used j operations,
        // and the last element is a[i] (not changed after its final value is set)
        // We'll use a 2D DP with dimensions (n) x (k+1), initialized to INF.
        vector<vector<long long>> dp(n, vector<long long>(k + 1, INF));
        
        // Base case: first element
        for (int j = 0; j <= k; ++j) {
            dp[0][j] = a[0];
        }

        // Precompute prefix minimums to quickly get min value in a range
        // Actually we need to consider making a segment equal to the minimum in that segment.
        // For each i, we try to extend a segment backwards.
        for (int i = 1; i < n; ++i) {
            for (int j = 0; j <= k; ++j) {
                // Option 1: don't change a[i] relative to previous segment
                // This means a[i] stays as is, and we just add it to dp[i-1][j]
                if (dp[i-1][j] != INF) {
                    dp[i][j] = min(dp[i][j], dp[i-1][j] + a[i]);
                }

                // Option 2: a[i] is part of a segment that got changed to some minimum
                // We try all possible lengths L of the segment ending at i that we "flatten"
                // to the minimum value in that segment.
                // The number of operations needed is L-1 (we change L-1 elements to the min).
                long long min_val = a[i];
                for (int len = 1; len <= i + 1 && len - 1 <= j; ++len) {
                    int start = i - len + 1;
                    min_val = min(min_val, a[start]);
                    int ops = len - 1;
                    int prev_j = j - ops;
                    long long prev_sum = (start == 0) ? 0 : dp[start - 1][prev_j];
                    if (prev_sum != INF) {
                        dp[i][j] = min(dp[i][j], prev_sum + min_val * len);
                    }
                }
            }
        }

        long long ans = INF;
        for (int j = 0; j <= k; ++j) {
            ans = min(ans, dp[n-1][j]);
        }
        cout << ans << '\n';
    }
    return 0;
}
