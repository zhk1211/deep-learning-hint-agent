#include <bits/stdc++.h>
using namespace std;

using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<ll> h(n);
        ll mx = 0;
        for (int i = 0; i < n; ++i) {
            cin >> h[i];
            mx = max(mx, h[i]);
        }

        auto check = [&](ll target, ll days) -> bool {
            ll cnt1 = (days + 1) / 2; // number of +1 operations
            ll cnt2 = days / 2;       // number of +2 operations
            ll need1 = 0;
            for (int i = 0; i < n; ++i) {
                ll diff = target - h[i];
                // use as many +2 as possible
                ll take2 = min(cnt2, diff / 2);
                cnt2 -= take2;
                diff -= take2 * 2;
                need1 += diff; // remaining must be covered by +1
            }
            return need1 <= cnt1;
        };

        ll ans = LLONG_MAX;
        for (ll target : {mx, mx + 1}) {
            ll lo = 0, hi = 1e18;
            while (lo < hi) {
                ll mid = lo + (hi - lo) / 2;
                if (check(target, mid)) {
                    hi = mid;
                } else {
                    lo = mid + 1;
                }
            }
            ans = min(ans, lo);
        }
        cout << ans << '\n';
    }
    return 0;
}
