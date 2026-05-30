// Hint1
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
        
        vector<int> min_vals(n);
        for (int i = 0; i < n; ++i) {
            min_vals[i] = min(sa[i], sb[i]);
        }
        
        long long prod = 1;
        for (int x : min_vals) {
            prod = prod * (x % MOD) % MOD;
        }
        
        vector<int> ans;
        ans.push_back(prod);
        
        while (q--) {
            int o, x;
            cin >> o >> x;
            --x;
            if (o == 1) {
                int old_val = a[x];
                ++a[x];
                int pos = upper_bound(sa.begin(), sa.end(), old_val) - sa.begin() - 1;
                ++sa[pos];
                int new_val = sa[pos];
                int old_min = min(old_val, sb[pos]);
                int new_min = min(new_val, sb[pos]);
                if (old_min != new_min) {
                    prod = prod * modpow(old_min % MOD, MOD - 2) % MOD;
                    prod = prod * (new_min % MOD) % MOD;
                }
            } else {
                int old_val = b[x];
                ++b[x];
                int pos = upper_bound(sb.begin(), sb.end(), old_val) - sb.begin() - 1;
                ++sb[pos];
                int new_val = sb[pos];
                int old_min = min(sa[pos], old_val);
                int new_min = min(sa[pos], new_val);
                if (old_min != new_min) {
                    prod = prod * modpow(old_min % MOD, MOD - 2) % MOD;
                    prod = prod * (new_min % MOD) % MOD;
                }
            }
            ans.push_back(prod);
        }
        
        for (int i = 0; i < (int)ans.size(); ++i) {
            if (i) cout << ' ';
            cout << ans[i];
        }
        cout << '\n';
    }
    return 0;
}
