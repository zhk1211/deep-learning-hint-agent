// Hint1
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

ll f(ll x) {
    if (x <= 0) return 0;
    return 3LL * x * (x - 1) + 1;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        ll n;
        cin >> n;
        if (n == 1) {
            cout << 2 << "\n";
            continue;
        }
        ll lo = 1, hi = 1;
        while (f(hi) < n) hi *= 2;
        while (lo < hi) {
            ll mid = (lo + hi) / 2;
            if (f(mid) >= n) hi = mid;
            else lo = mid + 1;
        }
        cout << 2 * lo << "\n";
    }
    return 0;
}
