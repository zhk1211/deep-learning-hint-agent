#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

void solve() {
    int n, q;
    cin >> n >> q;
    vector<i64> a(2 * n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
        a[i + n] = a[i];
    }
    vector<i64> pref(2 * n + 1, 0);
    for (int i = 0; i < 2 * n; ++i) {
        pref[i + 1] = pref[i] + a[i];
    }
    auto sum_range = [&](int L, int R) -> i64 {
        if (L > R) return 0;
        return pref[R + 1] - pref[L];
    };
    i64 total = pref[n];
    while (q--) {
        i64 l, r;
        cin >> l >> r;
        --l; --r;
        i64 rl = l / n;
        i64 rr = r / n;
        i64 pl = l % n;
        i64 pr = r % n;
        if (rl == rr) {
            cout << sum_range(rl + pl, rl + pr) << '\n';
        } else {
            i64 ans = 0;
            ans += sum_range(rl + pl, rl + n - 1);
            ans += sum_range(rr, rr + pr);
            if (rr - rl > 1) {
                ans += total * (rr - rl - 1);
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
