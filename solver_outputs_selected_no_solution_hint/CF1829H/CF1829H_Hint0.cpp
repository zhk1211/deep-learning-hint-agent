#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;
const int MAX_VAL = 63;

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

        vector<long long> dp(MAX_VAL + 1, 0);
        dp[MAX_VAL] = 1; // empty subsequence AND = 63 (all ones)

        for (int x : a) {
            vector<long long> ndp = dp;
            for (int mask = 0; mask <= MAX_VAL; ++mask) {
                if (dp[mask] > 0) {
                    int new_mask = mask & x;
                    ndp[new_mask] = (ndp[new_mask] + dp[mask]) % MOD;
                }
            }
            dp = move(ndp);
        }

        long long ans = 0;
        for (int mask = 0; mask <= MAX_VAL; ++mask) {
            if (__builtin_popcount(mask) == k) {
                ans = (ans + dp[mask]) % MOD;
            }
        }
        // subtract the empty subsequence if its AND (63) has k set bits
        if (__builtin_popcount(MAX_VAL) == k) {
            ans = (ans - 1 + MOD) % MOD;
        }
        cout << ans << '\n';
    }
    return 0;
}
