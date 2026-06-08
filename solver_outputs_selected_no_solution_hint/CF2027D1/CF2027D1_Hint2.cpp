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
        for (int i = 0; i < n; i++) cin >> a[i];
        vector<long long> b(m);
        for (int i = 0; i < m; i++) cin >> b[i];
        
        vector<long long> pref(n + 1, 0);
        for (int i = 0; i < n; i++) pref[i + 1] = pref[i] + a[i];
        
        vector<long long> dp(n + 1, INF);
        dp[0] = 0;
        
        for (int k = 0; k < m; k++) {
            long long limit = b[k];
            vector<long long> ndp = dp;
            int ptr = 0;
            for (int i = 0; i <= n; i++) {
                if (dp[i] >= INF) continue;
                while (ptr <= n && pref[ptr] - pref[i] <= limit) ptr++;
                if (ptr - 1 > i) {
                    ndp[ptr - 1] = min(ndp[ptr - 1], dp[i] + (m - 1 - k));
                }
            }
            for (int i = n - 1; i >= 0; i--) {
                ndp[i] = min(ndp[i], ndp[i + 1]);
            }
            dp = move(ndp);
        }
        
        if (dp[n] >= INF) cout << "-1\n";
        else cout << dp[n] << "\n";
    }
    return 0;
}
