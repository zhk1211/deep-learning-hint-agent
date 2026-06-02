#include <bits/stdc++.h>
using namespace std;

using ll = long long;

ll gcd(ll a, ll b) {
    while (b) {
        a %= b;
        swap(a, b);
    }
    return a;
}

ll lcm(ll a, ll b) {
    return a / gcd(a, b) * b;
}

void solve() {
    int n;
    cin >> n;
    vector<ll> a(n), b(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i] >> b[i];
    }
    
    ll g = 0;
    ll l = 1;
    int ans = 1;
    
    for (int i = 0; i < n; ++i) {
        g = gcd(g, a[i] * b[i]);
        l = lcm(l, b[i]);
        if (g % l != 0) {
            ++ans;
            g = a[i] * b[i];
            l = b[i];
        }
    }
    
    cout << ans << '\n';
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
