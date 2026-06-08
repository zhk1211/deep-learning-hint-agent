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
        for (int i = 0; i < 26; i++) {
            cin >> c[i];
            total += c[i];
        }
        int odd_positions = (total + 1) / 2;
        int even_positions = total / 2;
        vector<int> big, small;
        for (int i = 0; i < 26; i++) {
            if (c[i] > 0) {
                if (c[i] % 2 == 1) {
                    big.push_back(c[i]);
                } else {
                    small.push_back(c[i]);
                }
            }
        }
        if (big.size() > 1) {
            cout << 0 << "\n";
            continue;
        }
        long long ans = 1;
        if (!big.empty()) {
            int val = big[0];
            int half = val / 2;
            ans = ans * nCr(odd_positions, half + 1) % MOD;
            odd_positions -= (half + 1);
            even_positions -= half;
        }
        for (int x : small) {
            int half = x / 2;
            ans = ans * nCr(odd_positions, half) % MOD;
            odd_positions -= half;
            ans = ans * nCr(even_positions, half) % MOD;
            even_positions -= half;
        }
        cout << ans << "\n";
    }
    return 0;
}
