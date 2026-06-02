#include <bits/stdc++.h>
using namespace std;

using ll = long long;

ll pow10[7];

void precompute() {
    pow10[0] = 1;
    for (int i = 1; i <= 6; i++) {
        pow10[i] = pow10[i - 1] * 10;
    }
}

void solve() {
    int A, B, C;
    ll k;
    cin >> A >> B >> C >> k;

    ll minA = pow10[A - 1];
    ll maxA = pow10[A] - 1;
    ll minB = pow10[B - 1];
    ll maxB = pow10[B] - 1;
    ll minC = pow10[C - 1];
    ll maxC = pow10[C] - 1;

    for (ll a = minA; a <= maxA; a++) {
        ll lowB = max(minC - a, minB);
        ll highB = min(maxC - a, maxB);

        if (lowB > highB) continue;

        ll cnt = highB - lowB + 1;
        if (k > cnt) {
            k -= cnt;
            continue;
        }

        ll b = lowB + k - 1;
        ll c = a + b;
        cout << a << " + " << b << " = " << c << "\n";
        return;
    }

    cout << "-1\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    precompute();

    int t;
    cin >> t;
    while (t--) {
        solve();
    }

    return 0;
}
