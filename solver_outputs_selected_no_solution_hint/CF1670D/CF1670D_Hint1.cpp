#include <bits/stdc++.h>
using namespace std;

using ll = long long;

ll f(ll k) {
    if (k < 2) return 0;
    ll m = k / 3;
    ll r = k % 3;
    ll base = 3 * m * (m + 1);
    if (r == 1) base += 2 * m;
    else if (r == 2) base += 4 * m + 2;
    return base;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        ll n;
        cin >> n;
        ll lo = 0, hi = 1;
        while (f(hi) < n) hi *= 2;
        while (lo < hi) {
            ll mid = lo + (hi - lo) / 2;
            if (f(mid) >= n) hi = mid;
            else lo = mid + 1;
        }
        cout << lo << '\n';
    }
    return 0;
}
