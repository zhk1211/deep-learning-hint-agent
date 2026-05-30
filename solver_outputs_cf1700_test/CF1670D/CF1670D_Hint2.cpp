// Hint2
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

ll f(ll x) {
    if (x < 2) return 0;
    ll k = x / 3;
    ll r = x % 3;
    ll ans = 3 * k * k;
    if (r == 1) ans += 2 * k;
    else if (r == 2) ans += 4 * k + 1;
    return ans;
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
