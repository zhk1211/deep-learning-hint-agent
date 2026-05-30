// Hint1
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

void solve() {
    int n, m, q;
    cin >> n >> m >> q;
    vector<ll> a(n), b(m);
    for (int i = 0; i < n; i++) cin >> a[i];
    for (int i = 0; i < m; i++) cin >> b[i];
    sort(a.rbegin(), a.rend());
    sort(b.rbegin(), b.rend());
    vector<ll> prefA(n + 1, 0), prefB(m + 1, 0);
    for (int i = 0; i < n; i++) prefA[i + 1] = prefA[i] + a[i];
    for (int i = 0; i < m; i++) prefB[i + 1] = prefB[i] + b[i];
    
    while (q--) {
        int x, y, z;
        cin >> x >> y >> z;
        // We need to choose exactly z cards, at most x from A, at most y from B.
        // Let k be the number taken from A, then z-k from B.
        // Constraints: 0 <= k <= x, 0 <= z-k <= y => max(0, z-y) <= k <= min(x, z).
        int L = max(0, z - y);
        int R = min(x, z);
        if (L > R) {
            cout << 0 << "\n";
            continue;
        }
        // We want to maximize prefA[k] + prefB[z-k] over k in [L, R].
        // Since prefA is concave (differences decreasing) and prefB is also concave,
        // the sum is concave, so we can use ternary search or just check boundaries
        // because the function is unimodal. But careful: prefA and prefB are sums of
        // sorted descending arrays, so they are concave (second differences <= 0).
        // The sum of two concave functions is concave, so maximum is at one of the
        // boundaries? Actually concave function on integer interval attains maximum
        // at one of the endpoints if it's monotonic, but it could be in the middle.
        // However, since both sequences are sorted descending, the marginal gain
        // of taking one more from A is a[k] (decreasing), and marginal loss from B
        // is b[z-k] (increasing as k increases). So the difference f(k+1)-f(k) =
        // a[k] - b[z-k-1] (using 0-indexed). This difference is decreasing with k
        // because a[k] decreases and b[z-k-1] increases. So f is concave, and we
        // can find the maximum by binary searching the point where difference
        // changes sign, or just ternary search. Since R-L can be large, we do
        // ternary search on integers.
        auto get_val = [&](int k) -> ll {
            return prefA[k] + prefB[z - k];
        };
        int lo = L, hi = R;
        while (hi - lo > 2) {
            int m1 = lo + (hi - lo) / 3;
            int m2 = hi - (hi - lo) / 3;
            if (get_val(m1) < get_val(m2)) lo = m1;
            else hi = m2;
        }
        ll ans = 0;
        for (int k = lo; k <= hi; k++) {
            ans = max(ans, get_val(k));
        }
        cout << ans << "\n";
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
