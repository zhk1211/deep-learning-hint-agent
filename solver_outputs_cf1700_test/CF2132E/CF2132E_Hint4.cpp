// Hint4
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void solve() {
    int n, m, q;
    cin >> n >> m >> q;
    vector<ll> a(n), b(m);
    for (int i = 0; i < n; ++i) cin >> a[i];
    for (int i = 0; i < m; ++i) cin >> b[i];
    sort(a.rbegin(), a.rend());
    sort(b.rbegin(), b.rend());
    vector<ll> pref_a(n + 1, 0), pref_b(m + 1, 0);
    for (int i = 0; i < n; ++i) pref_a[i + 1] = pref_a[i] + a[i];
    for (int i = 0; i < m; ++i) pref_b[i + 1] = pref_b[i] + b[i];
    
    while (q--) {
        int x, y, z;
        cin >> x >> y >> z;
        int L = max(0, z - y);
        int R = min(x, z);
        ll ans = 0;
        // We need to maximize pref_a[i] + pref_b[z - i] for i in [L, R]
        // Since pref_a is concave (differences decreasing) and pref_b is also concave,
        // the sum is concave, so we can use ternary search or just check boundaries
        // because the function is unimodal. But constraints are small enough to just
        // check all? No, q up to 1e5, n,m up to 2e5, but sum of q is 1e5, and R-L can be large.
        // However, note that the optimal i is either L, R, or where the derivative changes sign.
        // Derivative: a[i] - b[z - i - 1] (if we consider discrete). We can binary search for the point
        // where a[i] <= b[z - i - 1] (since arrays are sorted descending).
        // Actually, we want to maximize sum of top i from a and top (z-i) from b.
        // The function f(i) = pref_a[i] + pref_b[z-i] is concave because differences are decreasing.
        // So we can ternary search, but careful with discrete and flat regions.
        // Simpler: find the unconstrained optimal i0 (without x,y limits) by binary search on derivative.
        // Then the constrained optimal is the projection of i0 onto [L, R].
        auto get_val = [&](int i) -> ll {
            return pref_a[i] + pref_b[z - i];
        };
        // Find unconstrained optimal i in [0, z] (but bounded by n,m)
        int low_un = max(0, z - m);
        int high_un = min(n, z);
        int i0 = low_un;
        // binary search for largest i such that a[i-1] >= b[z-i] (if i>0 and z-i < m)
        // Actually, we want to find where derivative crosses zero.
        // f(i+1) - f(i) = a[i] - b[z - i - 1] (0-indexed).
        // We want the last i where this difference >= 0.
        int lo = low_un, hi = high_un;
        while (lo < hi) {
            int mid = (lo + hi + 1) / 2;
            // check if moving from mid-1 to mid increases sum
            // f(mid) - f(mid-1) = a[mid-1] - b[z - mid]
            if (mid > low_un && (z - mid) < m && a[mid - 1] >= b[z - mid]) {
                lo = mid;
            } else if (mid == low_un) {
                lo = mid;
                break;
            } else {
                hi = mid - 1;
            }
        }
        i0 = lo;
        // Now clamp to [L, R]
        int best_i = clamp(i0, L, R);
        ans = get_val(best_i);
        // Also check boundaries just in case (ternary might have flat regions)
        ans = max(ans, get_val(L));
        ans = max(ans, get_val(R));
        cout << ans << '\n';
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
