// Hint5
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

        long long pow2_n = modpow(2, n);
        long long pow2_n_minus_1 = modpow(2, n - 1);

        // dp[i][0] : And > Xor in processed higher bits
        // dp[i][1] : And == Xor in processed higher bits
        vector<long long> dp0(k + 1), dp1(k + 1);
        dp0[0] = 0;
        dp1[0] = 1;

        for (int i = 1; i <= k; ++i) {
            // ways to make current bit equal and keep equality
            long long ways_equal;
            if (n % 2 == 0) {
                // even n: equal if even number of 1s in XOR (so XOR=0) and AND=0
                // number of subsets of n with even size = 2^(n-1)
                // but we need AND=0, so exclude the all-1 case (size n)
                ways_equal = (pow2_n_minus_1 - 1 + MOD) % MOD;
            } else {
                // odd n: equal if XOR=1 and AND=1 (all ones) or XOR=0 and AND=0 (even number of ones, not all)
                // all ones: 1 way
                // even number of ones, not all: 2^(n-1) - 1
                ways_equal = (pow2_n_minus_1) % MOD; // 2^(n-1) - 1 + 1 = 2^(n-1)
            }

            // ways to make And > Xor at this bit (so And=1, Xor=0)
            // And=1 means all bits are 1 -> 1 way
            // Xor=0 with all ones happens only if n is even
            long long ways_greater = (n % 2 == 0) ? 1 : 0;

            // transitions
            dp0[i] = (dp0[i-1] * pow2_n) % MOD; // already greater, anything goes
            dp0[i] = (dp0[i] + dp1[i-1] * ways_greater) % MOD; // become greater now

            dp1[i] = (dp1[i-1] * ways_equal) % MOD; // stay equal
        }

        long long ans = (dp0[k] + dp1[k]) % MOD;
        cout << ans << '\n';
    }
    return 0;
}
