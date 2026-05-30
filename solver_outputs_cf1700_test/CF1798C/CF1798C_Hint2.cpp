// Hint2
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

ll gcd(ll a, ll b) {
    return b == 0 ? a : gcd(b, a % b);
}

ll lcm(ll a, ll b) {
    return a / gcd(a, b) * b;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<ll> a(n), b(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i] >> b[i];
        }

        int ans = 0;
        int i = 0;
        while (i < n) {
            ++ans;
            ll g = a[i] * b[i];
            ll l = b[i];
            int j = i;
            while (j < n) {
                g = gcd(g, a[j] * b[j]);
                l = lcm(l, b[j]);
                if (g % l != 0) {
                    break;
                }
                ++j;
            }
            i = j;
        }
        cout << ans << '\n';
    }
    return 0;
}
