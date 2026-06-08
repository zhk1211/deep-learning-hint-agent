#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void solve() {
    int n, q;
    cin >> n >> q;
    vector<ll> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    
    vector<ll> pref(n + 1, 0);
    for (int i = 0; i < n; ++i) pref[i + 1] = pref[i] + a[i];
    ll total = pref[n];
    
    auto get_sum = [&](ll idx) -> ll {
        // 1-indexed idx
        ll block = (idx - 1) / n;
        ll pos = (idx - 1) % n;
        ll ans = block * total;
        // sum of first (pos+1) elements of cyclic shift starting at block+1
        // shift start index (0-based) = block % n
        ll start = block % n;
        if (start + pos < n) {
            ans += pref[start + pos + 1] - pref[start];
        } else {
            ans += pref[n] - pref[start];
            ans += pref[pos - (n - start) + 1] - pref[0];
        }
        return ans;
    };
    
    while (q--) {
        ll l, r;
        cin >> l >> r;
        cout << get_sum(r) - get_sum(l - 1) << '\n';
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) solve();
    return 0;
}
