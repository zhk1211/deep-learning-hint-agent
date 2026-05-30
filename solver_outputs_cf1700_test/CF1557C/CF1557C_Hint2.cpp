// Hint2
#include <bits/stdc++.h>
using namespace std;

const int MOD = 1'000'000'007;

long long modpow(long long a, long long e) {
    long long res = 1;
    while (e) {
        if (e & 1) res = res * a % MOD;
        a = a * a % MOD;
        e >>= 1;
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
        if (k == 0) {
            cout << 1 << '\n';
            continue;
        }
        long long pow2n = modpow(2, n);
        long long pow2n1 = modpow(2, n - 1);
        long long ways_eq = 0;
        if (n % 2 == 1) {
            ways_eq = (pow2n1 + 1) % MOD;
        } else {
            ways_eq = (pow2n1 - 1 + MOD) % MOD;
        }
        long long ways_gt = 0;
        if (n % 2 == 0) {
            ways_gt = 1;
        }
        long long dp_eq = 1;
        long long dp_gt = 0;
        for (int i = 0; i < k; i++) {
            long long new_eq = (dp_eq * ways_eq) % MOD;
            long long new_gt = (dp_gt * pow2n + dp_eq * ways_gt) % MOD;
            dp_eq = new_eq;
            dp_gt = new_gt;
        }
        long long ans = (dp_eq + dp_gt) % MOD;
        cout << ans << '\n';
    }
    return 0;
}
