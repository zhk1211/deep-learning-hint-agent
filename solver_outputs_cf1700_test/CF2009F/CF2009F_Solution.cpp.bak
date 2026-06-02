// Solution
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void solve() {
    int n, q;
    cin >> n >> q;
    vector<ll> a(2 * n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
        a[i + n] = a[i];
    }
    vector<ll> pref(2 * n + 1, 0);
    for (int i = 0; i < 2 * n; ++i) {
        pref[i + 1] = pref[i] + a[i];
    }
    auto sum_range = [&](int L, int R) -> ll {
        if (L > R) return 0;
        return pref[R + 1] - pref[L];
    };
    ll total = pref[n];
    while (q--) {
        ll l, r;
        cin >> l >> r;
        --l; --r;
        ll rl = l / n, rr = r / n;
        ll pl = l % n, pr = r % n;
        if (rl == rr) {
            cout << sum_range(rl + pl, rl + pr) << '\n';
        } else {
            ll ans = 0;
            ans += sum_range(rl + pl, rl + n - 1);
            ans += sum_range(rr, rr + pr);
            if (rr - rl > 1) {
                ans += (rr - rl - 1) * total;
            }
            cout << ans << '\n';
        }
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
