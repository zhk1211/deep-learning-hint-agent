#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll max_triangles(ll k) {
    if (k < 2) return 0;
    // maximum triangles with k lines: floor(k^2 / 4) * 2 + (k/2) * 2
    // Actually derived formula: 2 * floor(k^2/4) + 2 * floor(k/2)
    // which simplifies to:
    ll a = k / 2;
    ll b = k - a;
    return 2 * a * b + 2 * a;
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
        if (n == 2) {
            cout << 2 << "\n";
            continue;
        }
        if (n == 3) {
            cout << 3 << "\n";
            continue;
        }
        ll lo = 2, hi = 1;
        while (max_triangles(hi) < n) hi *= 2;
        while (lo < hi) {
            ll mid = lo + (hi - lo) / 2;
            if (max_triangles(mid) >= n) hi = mid;
            else lo = mid + 1;
        }
        cout << lo << "\n";
    }
    return 0;
}
