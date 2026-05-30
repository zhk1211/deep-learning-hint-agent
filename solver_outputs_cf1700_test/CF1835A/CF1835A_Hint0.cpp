// Hint0
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll pow10[7];

ll count_for_a(ll a, ll B, ll C) {
    ll min_b = pow10[B-1];
    ll max_b = pow10[B] - 1;
    ll min_c = pow10[C-1];
    ll max_c = pow10[C] - 1;
    ll min_sum = a + min_b;
    ll max_sum = a + max_b;
    if (min_sum > max_c || max_sum < min_c) return 0;
    ll L = max(min_b, min_c - a);
    ll R = min(max_b, max_c - a);
    if (L > R) return 0;
    return R - L + 1;
}

void solve() {
    int A, B, C;
    ll k;
    cin >> A >> B >> C >> k;
    ll min_a = pow10[A-1], max_a = pow10[A] - 1;
    for (ll a = min_a; a <= max_a; ++a) {
        ll cnt = count_for_a(a, B, C);
        if (k > cnt) {
            k -= cnt;
        } else {
            ll min_b = pow10[B-1];
            ll max_b = pow10[B] - 1;
            ll min_c = pow10[C-1];
            ll max_c = pow10[C] - 1;
            ll L = max(min_b, min_c - a);
            ll R = min(max_b, max_c - a);
            ll b = L + k - 1;
            ll c = a + b;
            cout << a << " + " << b << " = " << c << "\n";
            return;
        }
    }
    cout << "-1\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    pow10[0] = 1;
    for (int i = 1; i <= 6; ++i) pow10[i] = pow10[i-1] * 10;
    int t;
    cin >> t;
    while (t--) solve();
    return 0;
}
