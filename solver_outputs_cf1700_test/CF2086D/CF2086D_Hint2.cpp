// Hint2
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

long long nCr(int n, int r) {
    if (r < 0 || r > n) return 0;
    return fact[n] * invfact[r] % MOD * invfact[n-r] % MOD;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    precompute();
    int t;
    cin >> t;
    while (t--) {
        vector<int> c(26);
        int total = 0;
        int odd_count = 0, even_count = 0;
        for (int i = 0; i < 26; i++) {
            cin >> c[i];
            total += c[i];
            if (c[i] % 2 == 1) odd_count++;
            else if (c[i] > 0) even_count++;
        }
        if (odd_count > 1) {
            cout << 0 << '\n';
            continue;
        }
        int odd_positions = (total + 1) / 2;
        int even_positions = total / 2;
        long long ways = 1;
        for (int i = 0; i < 26; i++) {
            if (c[i] == 0) continue;
            int half = c[i] / 2;
            if (c[i] % 2 == 1) {
                ways = ways * nCr(odd_positions, half + 1) % MOD;
                odd_positions -= (half + 1);
                ways = ways * nCr(even_positions, half) % MOD;
                even_positions -= half;
            } else {
                ways = ways * nCr(odd_positions, half) % MOD;
                odd_positions -= half;
                ways = ways * nCr(even_positions, half) % MOD;
                even_positions -= half;
            }
        }
        cout << ways << '\n';
    }
    return 0;
}
