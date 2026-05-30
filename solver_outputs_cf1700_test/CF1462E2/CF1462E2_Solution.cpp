// Solution
#include <bits/stdc++.h>
using namespace std;

const int MOD = 1'000'000'007;
const int MAXN = 200'005;

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
    for (int i = 1; i < MAXN; ++i) {
        fact[i] = fact[i-1] * i % MOD;
    }
    invfact[MAXN-1] = modpow(fact[MAXN-1], MOD-2);
    for (int i = MAXN-2; i >= 0; --i) {
        invfact[i] = invfact[i+1] * (i+1) % MOD;
    }
}

long long nCr(int n, int r) {
    if (r < 0 || r > n) return 0;
    return fact[n] * invfact[r] % MOD * invfact[n-r] % MOD;
}

void solve() {
    int t;
    cin >> t;
    while (t--) {
        int n, m, k;
        cin >> n >> m >> k;
        vector<int> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }
        sort(a.begin(), a.end());
        
        long long ans = 0;
        int j = 0;
        for (int i = 0; i < n; ++i) {
            while (j + 1 < n && a[j+1] - a[i] <= k) {
                ++j;
            }
            int cnt = j - i + 1;
            if (cnt >= m) {
                ans = (ans + nCr(cnt-1, m-1)) % MOD;
            }
        }
        cout << ans << '\n';
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    precompute();
    solve();
    return 0;
}
