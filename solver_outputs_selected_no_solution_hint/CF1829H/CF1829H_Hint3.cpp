#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n, k;
        cin >> n >> k;
        vector<int> a(n);
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }
        
        vector<long long> dp(64, 0);
        dp[63] = 1; // empty subsequence AND is all ones (63 in 6 bits)
        
        for (int x : a) {
            vector<long long> ndp = dp;
            for (int mask = 0; mask < 64; mask++) {
                int new_mask = mask & x;
                ndp[new_mask] = (ndp[new_mask] + dp[mask]) % MOD;
            }
            dp = ndp;
        }
        
        long long ans = 0;
        for (int mask = 0; mask < 64; mask++) {
            if (__builtin_popcount(mask) == k) {
                ans = (ans + dp[mask]) % MOD;
            }
        }
        // subtract the empty subsequence if k == 6 (since empty AND is 63, popcount 6)
        if (k == 6) {
            ans = (ans - 1 + MOD) % MOD;
        }
        cout << ans << '\n';
    }
    return 0;
}
