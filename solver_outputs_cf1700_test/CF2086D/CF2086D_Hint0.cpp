// Hint0
#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;
const int MAXN = 500000;

vector<long long> fact, invfact;

long long modpow(long long a, long long e) {
    long long res = 1;
    while (e) {
        if (e & 1) res = res * a % MOD;
        a = a * a % MOD;
        e >>= 1;
    }
    return res;
}

void precompute(int n) {
    fact.resize(n + 1);
    invfact.resize(n + 1);
    fact[0] = 1;
    for (int i = 1; i <= n; i++) {
        fact[i] = fact[i - 1] * i % MOD;
    }
    invfact[n] = modpow(fact[n], MOD - 2);
    for (int i = n - 1; i >= 0; i--) {
        invfact[i] = invfact[i + 1] * (i + 1) % MOD;
    }
}

long long nCr(int n, int r) {
    if (r < 0 || r > n) return 0;
    return fact[n] * invfact[r] % MOD * invfact[n - r] % MOD;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    vector<vector<int>> tests(t, vector<int>(26));
    int max_total = 0;
    for (int i = 0; i < t; i++) {
        int sum = 0;
        for (int j = 0; j < 26; j++) {
            cin >> tests[i][j];
            sum += tests[i][j];
        }
        max_total = max(max_total, sum);
    }

    precompute(max_total);

    for (auto &c : tests) {
        int total = accumulate(c.begin(), c.end(), 0);
        int odd_positions = (total + 1) / 2;
        int even_positions = total / 2;

        vector<int> non_zero;
        int odd_sum = 0;
        bool possible = true;
        for (int i = 0; i < 26; i++) {
            if (c[i] > 0) {
                non_zero.push_back(c[i]);
                if (c[i] % 2 == 1) {
                    odd_sum++;
                }
            }
        }

        if (odd_sum > 1) {
            cout << 0 << '\n';
            continue;
        }

        // We need to distribute half of each even count to odd positions and half to even positions.
        // For the single odd count (if any), one goes to odd, rest split equally.
        long long ways = 1;
        int odd_remaining = odd_positions;
        int even_remaining = even_positions;

        for (int cnt : non_zero) {
            if (cnt % 2 == 1) {
                // one goes to odd
                odd_remaining--;
                cnt--;
            }
            int half = cnt / 2;
            if (half > odd_remaining || half > even_remaining) {
                possible = false;
                break;
            }
            ways = ways * nCr(odd_remaining, half) % MOD;
            odd_remaining -= half;
            ways = ways * nCr(even_remaining, half) % MOD;
            even_remaining -= half;
        }

        if (!possible) {
            cout << 0 << '\n';
        } else {
            cout << ways % MOD << '\n';
        }
    }

    return 0;
}
