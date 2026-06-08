#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void solve() {
    int n, q;
    cin >> n >> q;
    vector<ll> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }
    ll total = accumulate(a.begin(), a.end(), 0LL);
    vector<ll> pref(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        pref[i + 1] = pref[i] + a[i];
    }
    auto range_sum = [&](int l, int r) -> ll {
        if (l > r) return 0;
        return pref[r + 1] - pref[l];
    };
    auto query = [&](ll idx) -> ll {
        ll block = (idx - 1) / n;
        ll pos = (idx - 1) % n;
        ll ans = block * total;
        ll start = block % n;
        ll end = (start + pos) % n;
        if (start + pos < n) {
            ans += range_sum(start, start + pos);
        } else {
            ans += range_sum(start, n - 1);
            ans += range_sum(0, end);
        }
        return ans;
    };
    while (q--) {
        ll l, r;
        cin >> l >> r;
        cout << query(r) - query(l - 1) << '\n';
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
