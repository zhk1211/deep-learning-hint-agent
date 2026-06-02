// Solution
#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;

int modpow(long long a, long long e) {
    long long res = 1;
    a %= MOD;
    while (e) {
        if (e & 1) res = res * a % MOD;
        a = a * a % MOD;
        e >>= 1;
    }
    return (int)res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        int n, q;
        cin >> n >> q;
        vector<int> a(n), b(n);
        for (int i = 0; i < n; ++i) cin >> a[i];
        for (int i = 0; i < n; ++i) cin >> b[i];
        vector<int> sa = a, sb = b;
        sort(sa.begin(), sa.end());
        sort(sb.begin(), sb.end());
        long long prod = 1;
        for (int i = 0; i < n; ++i) {
            prod = prod * min(sa[i], sb[i]) % MOD;
        }
        cout << prod;
        while (q--) {
            int o, x;
            cin >> o >> x;
            --x;
            if (o == 1) {
                int val = a[x];
                a[x]++;
                int pos = upper_bound(sa.begin(), sa.end(), val) - sa.begin() - 1;
                prod = prod * modpow(min(sa[pos], sb[pos]), MOD - 2) % MOD;
                sa[pos]++;
                prod = prod * min(sa[pos], sb[pos]) % MOD;
            } else {
                int val = b[x];
                b[x]++;
                int pos = upper_bound(sb.begin(), sb.end(), val) - sb.begin() - 1;
                prod = prod * modpow(min(sa[pos], sb[pos]), MOD - 2) % MOD;
                sb[pos]++;
                prod = prod * min(sa[pos], sb[pos]) % MOD;
            }
            cout << ' ' << prod;
        }
        cout << '\n';
    }
    return 0;
}
