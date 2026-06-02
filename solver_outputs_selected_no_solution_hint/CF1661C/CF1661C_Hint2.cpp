#include <bits/stdc++.h>
using namespace std;

using ll = long long;

ll solve_case(int n, vector<int>& h) {
    int mx = *max_element(h.begin(), h.end());
    ll ans = LLONG_MAX;
    for (int target : {mx, mx + 1}) {
        ll odd_needed = 0, even_needed = 0;
        for (int x : h) {
            ll diff = target - x;
            even_needed += diff / 2;
            odd_needed += diff % 2;
        }
        // We can convert 2 odd days into 1 even day (by using two +1 instead of one +2)
        // Actually we want to balance odd and even days.
        // Let total operations = odd_needed + even_needed.
        // We can replace 2 odd with 1 even, but we need to minimize days.
        // Days = max(2*odd_needed - 1, 2*even_needed) ? No, standard approach:
        // We want to minimize max(2*O - 1, 2*E) where O and E are final counts after possible conversions.
        // Better: binary search on days.
        auto can = [&](ll days) -> bool {
            ll odd_days = (days + 1) / 2;
            ll even_days = days / 2;
            ll need_odd = odd_needed;
            ll need_even = even_needed;
            // Use odd days first for odd increments, then remaining odd days can be used as two +1 for even increments.
            if (odd_days < need_odd) return false;
            ll extra_odd = odd_days - need_odd;
            // Each extra odd day can provide 2 height (by using two +1 days? No, one odd day gives +1, but we can use two odd days to simulate one even day.
            // Actually extra odd days can be used to cover even needs: each extra odd day gives +1, but we need +2 increments.
            // We can combine two odd days to get +2, but that consumes two odd days.
            // So total even capacity from odd days = extra_odd / 2.
            ll even_from_odd = extra_odd / 2;
            ll total_even_capacity = even_days + even_from_odd;
            return total_even_capacity >= need_even;
        };
        ll low = 0, high = 2e14;
        while (low < high) {
            ll mid = (low + high) / 2;
            if (can(mid)) high = mid;
            else low = mid + 1;
        }
        ans = min(ans, low);
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
        vector<int> h(n);
        for (int i = 0; i < n; ++i) cin >> h[i];
        cout << solve_case(n, h) << '\n';
    }
    return 0;
}
