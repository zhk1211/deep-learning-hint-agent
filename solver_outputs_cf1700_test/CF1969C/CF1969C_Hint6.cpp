// Hint6
#include <bits/stdc++.h>
using namespace std;

const long long INF = 1e18;

void solve() {
    int n, k;
    cin >> n >> k;
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }
    
    // dp[i][j] = min sum for prefix of length i with j operations used
    // i from 0 to n, j from 0 to k
    vector<vector<long long>> dp(n + 1, vector<long long>(k + 1, INF));
    dp[0][0] = 0;
    
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j <= k; ++j) {
            if (dp[i][j] == INF) continue;
            // we can extend a segment starting at i
            long long mn = a[i];
            // d is the number of operations used to merge d+1 elements
            for (int d = 0; d + j <= k && i + d < n; ++d) {
                if (d > 0) mn = min(mn, a[i + d]);
                int ni = i + d + 1;
                int nj = j + d;
                dp[ni][nj] = min(dp[ni][nj], dp[i][j] + mn * (d + 1));
            }
        }
    }
    
    long long ans = INF;
    for (int j = 0; j <= k; ++j) {
        ans = min(ans, dp[n][j]);
    }
    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}
