#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void solve() {
    int n, q;
    cin >> n >> q;
    vector<ll> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    
    vector<ll> pref(n + 1, 0);
    for (int i = 0; i < n; i++) pref[i + 1] = pref[i] + a[i];
    ll total = pref[n];
    
    auto sum_range = [&](ll l, ll r) -> ll {
        if (l > r) return 0;
        ll block_l = (l - 1) / n;
        ll block_r = (r - 1) / n;
        if (block_l == block_r) {
            ll start = (l - 1) % n;
            ll end = (r - 1) % n;
            return pref[end + 1] - pref[start];
        }
        ll res = 0;
        // first block
        ll start = (l - 1) % n;
        res += pref[n] - pref[start];
        // last block
        ll end = (r - 1) % n;
        res += pref[end + 1] - pref[0];
        // full blocks in between
        ll full_blocks = block_r - block_l - 1;
        res += full_blocks * total;
        return res;
    };
    
    while (q--) {
        ll l, r;
        cin >> l >> r;
        cout << sum_range(l, r) << '\n';
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
