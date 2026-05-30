// Solution
#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;
const int MAXN = 500005;

long long fact[MAXN], invfact[MAXN];

long long modpow(long long a, long long e) {
    long long res = 1;
    while (e) {
        if (e & 1) res = res * a % MOD;
        a = a * a % MOD;
        e >>= 1;
    }
    return res;
}

void precompute() {
    fact[0] = 1;
    for (int i = 1; i < MAXN; i++) {
        fact[i] = fact[i-1] * i % MOD;
    }
    invfact[MAXN-1] = modpow(fact[MAXN-1], MOD-2);
    for (int i = MAXN-2; i >= 0; i--) {
        invfact[i] = invfact[i+1] * (i+1) % MOD;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    precompute();
    int t;
    cin >> t;
    while (t--) {
        int c[26];
        int sum = 0;
        for (int i = 0; i < 26; i++) {
            cin >> c[i];
            sum += c[i];
        }
        int odd = (sum + 1) / 2;
        int even = sum / 2;
        vector<long long> dp(sum + 1, 0);
        dp[0] = 1;
        for (int i = 0; i < 26; i++) {
            if (c[i] == 0) continue;
            for (int j = sum; j >= c[i]; j--) {
                dp[j] = (dp[j] + dp[j - c[i]]) % MOD;
            }
        }
        long long ans = dp[odd] * fact[odd] % MOD;
        ans = ans * fact[even] % MOD;
        for (int i = 0; i < 26; i++) {
            ans = ans * invfact[c[i]] % MOD;
        }
        cout << ans << '\n';
    }
    return 0;
}
