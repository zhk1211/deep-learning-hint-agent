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
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }
        vector<long long> b(m);
        for (int i = 0; i < m; i++) {
            cin >> b[i];
        }
        
        // dp[i] = min cost to remove prefix of length i
        vector<long long> dp(n + 1, INF);
        dp[0] = 0;
        
        for (int k = 0; k < m; k++) {
            long long limit = b[k];
            int r = 0;
            long long sum = 0;
            // two pointers to find reachable states
            for (int l = 0; l < n; l++) {
                if (dp[l] >= INF) continue;
                while (r < n && sum + a[r] <= limit) {
                    sum += a[r];
                    r++;
                }
                // from l we can remove up to r elements
                for (int j = l + 1; j <= r; j++) {
                    dp[j] = min(dp[j], dp[l] + (m - 1 - k));
                }
                sum -= a[l];
            }
        }
        
        if (dp[n] >= INF) {
            cout << -1 << "\n";
        } else {
            cout << dp[n] << "\n";
        }
    }
    return 0;
}
