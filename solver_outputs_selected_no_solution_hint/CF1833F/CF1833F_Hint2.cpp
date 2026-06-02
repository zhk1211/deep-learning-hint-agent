#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

long long modpow(long long a, long long b) {
    long long res = 1;
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
        int n, m;
        cin >> n >> m;
        vector<int> a(n);
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }
        sort(a.begin(), a.end());

        vector<pair<int, int>> freq;
        for (int x : a) {
            if (freq.empty() || freq.back().first != x) {
                freq.push_back({x, 1});
            } else {
                freq.back().second++;
            }
        }

        int k = freq.size();
        vector<long long> inv(n + 1), fact(n + 1), invfact(n + 1);
        fact[0] = 1;
        for (int i = 1; i <= n; i++) {
            fact[i] = fact[i - 1] * i % MOD;
        }
        invfact[n] = modpow(fact[n], MOD - 2);
        for (int i = n - 1; i >= 0; i--) {
            invfact[i] = invfact[i + 1] * (i + 1) % MOD;
        }

        auto nCr = [&](int N, int R) -> long long {
            if (R < 0 || R > N) return 0;
            return fact[N] * invfact[R] % MOD * invfact[N - R] % MOD;
        };

        long long ans = 0;
        int l = 0;
        long long prod = 1;
        for (int r = 0; r < k; r++) {
            prod = prod * freq[r].second % MOD;
            while (freq[r].first - freq[l].first >= m) {
                prod = prod * modpow(freq[l].second, MOD - 2) % MOD;
                l++;
            }
            if (r - l + 1 >= m) {
                ans = (ans + nCr(r - l + 1, m) * prod) % MOD;
            }
        }
        cout << ans << '\n';
    }
    return 0;
}
