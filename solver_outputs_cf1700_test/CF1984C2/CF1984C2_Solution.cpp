#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;

int mod_pow(int base, int exp) {
    int res = 1;
    while (exp) {
        if (exp & 1) res = 1LL * res * base % MOD;
        base = 1LL * base * base % MOD;
        exp >>= 1;
    }
    return res;
}

void solve() {
    int n;
    cin >> n;
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    
    vector<long long> pref(n);
    pref[0] = a[0];
    for (int i = 1; i < n; ++i) pref[i] = pref[i-1] + a[i];
    
    long long min_pref = *min_element(pref.begin(), pref.end());
    
    if (min_pref >= 0) {
        cout << mod_pow(2, n) << '\n';
        return;
    }
    
    int ans = 0;
    int cnt_nonneg = 0;
    for (int i = 0; i < n; ++i) {
        if (pref[i] == min_pref) {
            int ways = 1LL * mod_pow(2, cnt_nonneg) * mod_pow(2, n - 1 - i) % MOD;
            ans = (ans + ways) % MOD;
        }
        if (pref[i] >= 0) cnt_nonneg++;
    }
    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) solve();
    return 0;
}
