// Hint4
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
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }
        
        // dp[i][j] = minimum sum for prefix of length i with j operations used
        // i from 0 to n, j from 0 to k
        vector<vector<long long>> dp(n + 1, vector<long long>(k + 1, INF));
        dp[0][0] = 0;
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j <= k; j++) {
                if (dp[i][j] == INF) continue;
                // Option 1: no operation on a[i]
                dp[i + 1][j] = min(dp[i + 1][j], dp[i][j] + a[i]);
                
                // Option 2: perform operations to extend a segment
                // We can change a[i] to some value from a[i+1..i+len]
                // and also change those elements to a[i] or something else
                // Actually, we can think of making a segment of equal elements
                // by taking the minimum in that segment
                long long min_val = a[i];
                for (int len = 1; len <= k - j && i + len <= n; len++) {
                    min_val = min(min_val, a[i + len - 1]);
                    // We use (len) operations? Wait, to make a segment of length len equal,
                    // we need len-1 operations (changing len-1 elements to the min)
                    int ops = len - 1;
                    if (j + ops <= k) {
                        dp[i + len][j + ops] = min(dp[i + len][j + ops], dp[i][j] + min_val * len);
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
