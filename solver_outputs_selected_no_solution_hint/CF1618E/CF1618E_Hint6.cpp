#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void solve() {
    int n;
    cin >> n;
    vector<ll> b(n);
    for (int i = 0; i < n; ++i) cin >> b[i];

    if (n == 1) {
        cout << "YES\n" << b[0] << "\n";
        return;
    }

    ll sum_b = accumulate(b.begin(), b.end(), 0LL);
    ll total_mult = (ll)n * (n + 1) / 2;
    if (sum_b % total_mult != 0) {
        cout << "NO\n";
        return;
    }
    ll sum_a = sum_b / total_mult;

    vector<ll> a(n);
    for (int i = 0; i < n; ++i) {
        int prev = (i - 1 + n) % n;
        ll diff = b[i] - b[prev];
        ll val = sum_a - diff;
        if (val <= 0 || val % n != 0) {
            cout << "NO\n";
            return;
        }
        a[i] = val / n;
        if (a[i] < 1 || a[i] > 1000000000) {
            cout << "NO\n";
            return;
        }
    }

    cout << "YES\n";
    for (int i = 0; i < n; ++i) {
        cout << a[i] << " \n"[i == n - 1];
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) solve();
    return 0;
}
