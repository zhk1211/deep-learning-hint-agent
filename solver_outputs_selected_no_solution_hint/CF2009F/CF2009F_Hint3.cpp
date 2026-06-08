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
    
    auto sum_cyclic = [&](ll l, ll r) -> ll {
        if (l > r) return 0;
        ll res = 0;
        ll block_l = (l - 1) / n;
        ll block_r = (r - 1) / n;
        if (block_l == block_r) {
            ll start = (l - 1) % n;
            ll end = (r - 1) % n;
            if (start <= end) {
                res = pref[end + 1] - pref[start];
            } else {
                res = total - (pref[start] - pref[end + 1]);
            }
        } else {
            ll start = (l - 1) % n;
            res += total - (pref[start] - pref[0]);
            ll end = (r - 1) % n;
            res += pref[end + 1] - pref[0];
            res += (block_r - block_l - 1) * total;
        }
        return res;
    };
    
    auto query = [&](ll l, ll r) -> ll {
        ll block_l = (l - 1) / n;
        ll block_r = (r - 1) / n;
        if (block_l == block_r) {
            return sum_cyclic(l, r);
        }
        ll ans = 0;
        ans += sum_cyclic(l, (block_l + 1) * n);
        ans += sum_cyclic(block_r * n + 1, r);
        if (block_l + 1 <= block_r - 1) {
            ll full_blocks = block_r - block_l - 1;
            ans += full_blocks * total;
        }
        return ans;
    };
    
    while (q--) {
        ll l, r;
        cin >> l >> r;
        cout << query(l, r) << '\n';
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}
