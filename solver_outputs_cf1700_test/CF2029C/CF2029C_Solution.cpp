#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<int> a(n + 1);
        for (int i = 1; i <= n; ++i) {
            cin >> a[i];
        }
        
        // dp[i][0] : before skip interval
        // dp[i][1] : inside skip interval
        // dp[i][2] : after skip interval
        vector<array<int, 3>> dp(n + 1);
        dp[0][0] = 0;
        dp[0][1] = dp[0][2] = -1e9;
        
        auto f = [&](int x, int ai) -> int {
            if (ai > x) return x + 1;
            if (ai == x) return x;
            return x - 1;
        };
        
        for (int i = 1; i <= n; ++i) {
            dp[i][0] = f(dp[i-1][0], a[i]);
            dp[i][1] = max(dp[i-1][1], dp[i-1][0]);
            dp[i][2] = max(f(dp[i-1][1], a[i]), f(dp[i-1][2], a[i]));
        }
        
        cout << max(dp[n][1], dp[n][2]) << '\n';
    }
    
    return 0;
}
