// Hint4
#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;

int modpow(int a, int e) {
    int res = 1;
    while (e) {
        if (e & 1) res = 1LL * res * a % MOD;
        a = 1LL * a * a % MOD;
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
        int n, q;
        cin >> n >> q;
        vector<int> a(n), b(n);
        for (int i = 0; i < n; ++i) cin >> a[i];
        for (int i = 0; i < n; ++i) cin >> b[i];
        vector<int> sa = a, sb = b;
        sort(sa.begin(), sa.end());
        sort(sb.begin(), sb.end());
        vector<int> c(n);
        for (int i = 0; i < n; ++i) c[i] = min(sa[i], sb[i]);
        long long prod = 1;
        for (int x : c) prod = prod * (x % MOD) % MOD;
        cout << prod;
        while (q--) {
            int o, x;
            cin >> o >> x;
            --x;
            if (o == 1) {
                int val = a[x];
                ++a[x];
                int pos = upper_bound(sa.begin(), sa.end(), val) - sa.begin() - 1;
                int old_min = min(sa[pos], sb[pos]);
                ++sa[pos];
                int new_min = min(sa[pos], sb[pos]);
                if (new_min != old_min) {
                    prod = prod * modpow(old_min % MOD, MOD - 2) % MOD;
                    prod = prod * (new_min % MOD) % MOD;
                }
            } else {
                int val = b[x];
                ++b[x];
                int pos = upper_bound(sb.begin(), sb.end(), val) - sb.begin() - 1;
                int old_min = min(sa[pos], sb[pos]);
                ++sb[pos];
                int new_min = min(sa[pos], sb[pos]);
                if (new_min != old_min) {
                    prod = prod * modpow(old_min % MOD, MOD - 2) % MOD;
                    prod = prod * (new_min % MOD) % MOD;
                }
            }
            cout << ' ' << prod;
        }
        cout << '\n';
    }
    return 0;
}
