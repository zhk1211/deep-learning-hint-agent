// Hint1
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<int> a(n + 2);
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
    }
    a[0] = a[n + 1] = 0;

    ll total = (ll)n * (n + 1) / 2;
    ll ans = total;
    for (int i = 1; i <= n; ++i) {
        if (a[i] != a[i - 1]) {
            ans += (ll)(i - 1) * (n - i + 1);
        }
    }

    while (m--) {
        int i, x;
        cin >> i >> x;
        if (a[i] != x) {
            if (a[i] != a[i - 1]) {
                ans -= (ll)(i - 1) * (n - i + 1);
            }
            if (a[i] != a[i + 1]) {
                ans -= (ll)i * (n - i);
            }
            a[i] = x;
            if (a[i] != a[i - 1]) {
                ans += (ll)(i - 1) * (n - i + 1);
            }
            if (a[i] != a[i + 1]) {
                ans += (ll)i * (n - i);
            }
        }
        cout << ans << '\n';
    }

    return 0;
}
