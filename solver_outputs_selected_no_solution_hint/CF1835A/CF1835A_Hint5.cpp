#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll pow10[7];

void pre() {
    pow10[0] = 1;
    for (int i = 1; i <= 6; i++) pow10[i] = pow10[i-1] * 10;
}

// count numbers with exactly d digits
ll cnt_digits(int d) {
    return pow10[d] - pow10[d-1];
}

void solve() {
    int A, B, C;
    ll k;
    cin >> A >> B >> C >> k;

    ll min_a = pow10[A-1], max_a = pow10[A] - 1;
    ll min_b = pow10[B-1], max_b = pow10[B] - 1;
    ll min_c = pow10[C-1], max_c = pow10[C] - 1;

    // iterate over a
    for (ll a = min_a; a <= max_a; a++) {
        // c must be in [min_c, max_c]
        // b = c - a, so b in [min_c - a, max_c - a]
        ll low_b = max(min_b, min_c - a);
        ll high_b = min(max_b, max_c - a);
        if (low_b > high_b) continue;
        ll cnt = high_b - low_b + 1;
        if (k > cnt) {
            k -= cnt;
            continue;
        }
        // answer is a + (low_b + k - 1) = (low_b + k - 1 + a)
        ll b = low_b + k - 1;
        ll c = a + b;
        cout << a << " + " << b << " = " << c << "\n";
        return;
    }
    cout << "-1\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    pre();
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}
