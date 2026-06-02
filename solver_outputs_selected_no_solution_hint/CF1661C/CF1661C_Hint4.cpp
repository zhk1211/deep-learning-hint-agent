#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll solve_case(int n, vector<ll>& h) {
    ll mx = *max_element(h.begin(), h.end());
    ll ans = LLONG_MAX;
    for (ll target : {mx, mx + 1}) {
        ll odd_needed = 0, even_needed = 0;
        for (ll x : h) {
            ll diff = target - x;
            even_needed += diff / 2;
            odd_needed += diff % 2;
        }
        // We need to schedule odd_needed odd days and even_needed even days.
        // We can use extra even days to cover odd needs (by splitting a +2 into two +1s on two odd days).
        // Let's binary search on total days.
        ll lo = 0, hi = 2e14;
        while (lo < hi) {
            ll mid = (lo + hi) / 2;
            ll odd_days = (mid + 1) / 2;
            ll even_days = mid / 2;
            // We can convert one even day into two odd days (by not watering on that even day and watering on two odd days instead).
            // Actually, we can use even days to satisfy odd needs: each even day can be "split" into two odd days.
            // So total odd capacity = odd_days + 2 * even_days.
            // But we also need to satisfy even needs, which require even days.
            // We can also use odd days to satisfy even needs? No, odd days give +1, even days give +2.
            // We need at least even_needed even days. The remaining even days can be converted to odd days.
            if (even_days >= even_needed) {
                ll extra_even = even_days - even_needed;
                if (odd_days + 2 * extra_even >= odd_needed) {
                    hi = mid;
                } else {
                    lo = mid + 1;
                }
            } else {
                // Not enough even days, we need more days.
                lo = mid + 1;
            }
        }
        ans = min(ans, lo);
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
        cout << solve_case(n, h) << '\n';
    }
    return 0;
}
