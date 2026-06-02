#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll solve_case(int n, vector<ll>& h) {
    ll mx = *max_element(h.begin(), h.end());
    ll lo = 0, hi = 2e14;
    ll ans = hi;
    while (lo <= hi) {
        ll days = (lo + hi) / 2;
        ll odd_days = (days + 1) / 2;
        ll even_days = days / 2;
        ll need_odd = 0, need_even = 0;
        bool ok = true;
        for (ll x : h) {
            ll diff = mx - x;
            ll take_even = min(even_days, diff / 2);
            diff -= take_even * 2;
            even_days -= take_even;
            need_odd += diff;
            if (need_odd > odd_days) {
                ok = false;
                break;
            }
        }
        if (ok && need_odd <= odd_days) {
            ans = days;
            hi = days - 1;
        } else {
            lo = days + 1;
        }
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<ll> h(n);
        for (int i = 0; i < n; ++i) cin >> h[i];
        ll mx = *max_element(h.begin(), h.end());
        ll ans = solve_case(n, h);
        // also try mx+1
        for (ll x : h) {
            if (x < mx) {
                vector<ll> h2 = h;
                for (auto& v : h2) v = mx + 1 - (mx - v);
                ans = min(ans, solve_case(n, h2));
                break;
            }
        }
        cout << ans << '\n';
    }
    return 0;
}
