#include <bits/stdc++.h>
using namespace std;

const int MOD = 1'000'000'007;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n, k;
        cin >> n >> k;
        vector<int> dp(64, 0);
        for (int i = 0; i < n; ++i) {
            int a;
            cin >> a;
            vector<int> new_dp = dp; // case: don't take a
            new_dp[a] = (new_dp[a] + 1) % MOD; // new subsequence with only a
            for (int j = 0; j < 64; ++j) {
                if (dp[j]) {
                    int val = j & a;
                    new_dp[val] = (new_dp[val] + dp[j]) % MOD; // take a after some subsequence
                }
            }
            dp = move(new_dp);
        }
        int ans = 0;
        for (int x = 0; x < 64; ++x) {
            if (__builtin_popcount(x) == k) {
                ans = (ans + dp[x]) % MOD;
            }
        }
        cout << ans << '\n';
    }
    return 0;
}
