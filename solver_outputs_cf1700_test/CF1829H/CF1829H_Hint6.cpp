// Hint6
#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;
const int MAX_MASK = 64;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n, k;
        cin >> n >> k;
        vector<int> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }

        vector<long long> dp(MAX_MASK, 0);
        dp[63] = 1; // 63 is all ones in 6 bits, acts as identity for AND

        for (int x : a) {
            vector<long long> new_dp = dp;
            for (int mask = 0; mask < MAX_MASK; ++mask) {
                int new_mask = mask & x;
                new_dp[new_mask] = (new_dp[new_mask] + dp[mask]) % MOD;
            }
            dp = move(new_dp);
        }

        long long ans = 0;
        for (int mask = 0; mask < MAX_MASK; ++mask) {
            if (__builtin_popcount(mask) == k) {
                ans = (ans + dp[mask]) % MOD;
            }
        }
        // subtract the empty subsequence if k == 0? Wait, dp[63] initially is 1 for empty.
        // But we only want non-empty subsequences.
        // The empty subsequence AND is not defined, but our DP includes it.
        // We need to subtract the empty subsequence if it contributes to the answer.
        // Empty subsequence corresponds to mask 63 (since we start with dp[63]=1 and never AND it with anything).
        // It has popcount(63)=6. So if k==6, we must subtract 1.
        if (k == 6) {
            ans = (ans - 1 + MOD) % MOD;
        }
        cout << ans << '\n';
    }
    return 0;
}
