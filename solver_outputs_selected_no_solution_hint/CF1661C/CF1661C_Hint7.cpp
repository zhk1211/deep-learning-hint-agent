#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll solve_case(const vector<ll>& h) {
    ll mx = *max_element(h.begin(), h.end());
    ll need_odd = 0, need_even = 0;
    for (ll x : h) {
        ll diff = mx - x;
        need_even += diff / 2;
        need_odd += diff % 2;
    }
    auto days_for = [&](ll target) -> ll {
        ll odd = 0, even = 0;
        for (ll x : h) {
            ll diff = target - x;
            even += diff / 2;
            odd += diff % 2;
        }
        if (odd > even) {
            return 2 * odd - 1;
        } else {
            ll total = odd + even;
            ll days = total / 3 * 2 + total % 3;
            return max(2 * odd - 1, days);
        }
    };
    ll ans = days_for(mx);
    ans = min(ans, days_for(mx + 1));
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
        cout << solve_case(h) << '\n';
    }
    return 0;
}
