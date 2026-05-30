// Hint1
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

        vector<long long> freq(MAX_VAL + 1, 0);
        for (int x : a) {
            freq[x]++;
        }

        vector<long long> pow2(n + 1, 1);
        for (int i = 1; i <= n; ++i) {
            pow2[i] = (pow2[i - 1] * 2) % MOD;
        }

        vector<long long> dp(MAX_VAL + 1, 0);
        for (int mask = 0; mask <= MAX_VAL; ++mask) {
            if (freq[mask] == 0) continue;
            vector<long long> new_dp = dp;
            for (int cur = 0; cur <= MAX_VAL; ++cur) {
                if (dp[cur] == 0) continue;
                int nxt = cur & mask;
                new_dp[nxt] = (new_dp[nxt] + dp[cur] * ((pow2[freq[mask]] - 1 + MOD) % MOD)) % MOD;
            }
            new_dp[mask] = (new_dp[mask] + (pow2[freq[mask]] - 1 + MOD)) % MOD;
            dp = move(new_dp);
        }

        long long ans = 0;
        for (int val = 0; val <= MAX_VAL; ++val) {
            if (__builtin_popcount(val) == k) {
                ans = (ans + dp[val]) % MOD;
            }
        }
        cout << ans << '\n';
    }
    return 0;
}
