#include <bits/stdc++.h>
using namespace std;

using ll = long long;

ll solve_case(int n, vector<int>& h) {
    int max_h = *max_element(h.begin(), h.end());
    ll lo = 0, hi = 1e18;
    ll ans = hi;
    while (lo <= hi) {
        ll mid = (lo + hi) / 2;
        ll odd_days = (mid + 1) / 2;
        ll even_days = mid / 2;
        ll need_odd = 0;
        ll need_even = 0;
        bool ok = true;
        for (int i = 0; i < n; ++i) {
            ll diff = max_h - h[i];
            ll take_even = min(even_days, diff / 2);
            even_days -= take_even;
            diff -= take_even * 2;
            need_odd += diff;
            if (need_odd > odd_days) {
                ok = false;
                break;
            }
        }
        if (ok && need_odd <= odd_days) {
            ans = mid;
            hi = mid - 1;
        } else {
            lo = mid + 1;
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
        vector<int> h(n);
        for (int i = 0; i < n; ++i) cin >> h[i];
        ll ans = solve_case(n, h);
        // also check max_h+1
        int max_h = *max_element(h.begin(), h.end());
        {
            ll lo = 0, hi = 1e18;
            ll best = hi;
            int target = max_h + 1;
            while (lo <= hi) {
                ll mid = (lo + hi) / 2;
                ll odd_days = (mid + 1) / 2;
                ll even_days = mid / 2;
                ll need_odd = 0;
                bool ok = true;
                ll rem_even = even_days;
                for (int i = 0; i < n; ++i) {
                    ll diff = target - h[i];
                    ll take_even = min(rem_even, diff / 2);
                    rem_even -= take_even;
                    diff -= take_even * 2;
                    need_odd += diff;
                    if (need_odd > odd_days) {
                        ok = false;
                        break;
                    }
                }
                if (ok && need_odd <= odd_days) {
                    best = mid;
                    hi = mid - 1;
                } else {
                    lo = mid + 1;
                }
            }
            ans = min(ans, best);
        }
        cout << ans << '\n';
    }
    return 0;
}
