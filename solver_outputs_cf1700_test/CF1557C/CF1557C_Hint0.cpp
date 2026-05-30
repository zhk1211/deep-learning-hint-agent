// Hint0
#include <bits/stdc++.h>
using namespace std;

const int MOD = 1'000'000'007;

long long mod_pow(long long a, long long b) {
    long long res = 1;
    a %= MOD;
    while (b) {
        if (b & 1) res = res * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n, k;
        cin >> n >> k;
        long long ans;
        if (k == 0) {
            ans = 1;
        } else {
            long long total = mod_pow(2, n);
            long long eq, diff;
            if (n % 2 == 0) {
                eq = (total - 2 + MOD) % MOD * mod_pow(2, MOD - 2) % MOD;
                diff = total;
            } else {
                eq = (total + 2) % MOD * mod_pow(2, MOD - 2) % MOD;
                diff = (total - 2 + MOD) % MOD * mod_pow(2, MOD - 2) % MOD;
            }
            long long dp_eq = 1, dp_gt = 0;
            for (int i = 0; i < k; ++i) {
                long long new_eq = dp_eq * eq % MOD;
                long long new_gt = (dp_eq * diff % MOD + dp_gt * total % MOD) % MOD;
                dp_eq = new_eq;
                dp_gt = new_gt;
            }
            ans = (dp_eq + dp_gt) % MOD;
        }
        cout << ans << '\n';
    }
    return 0;
}
