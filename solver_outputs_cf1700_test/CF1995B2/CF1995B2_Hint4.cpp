// Hint4
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void solve() {
    int n;
    ll m;
    cin >> n >> m;
    vector<pair<ll, ll>> flowers(n);
    for (int i = 0; i < n; ++i) cin >> flowers[i].first;
    for (int i = 0; i < n; ++i) cin >> flowers[i].second;
    sort(flowers.begin(), flowers.end());
    
    ll ans = 0;
    for (int i = 0; i < n; ++i) {
        ll a = flowers[i].first;
        ll c = flowers[i].second;
        ll take = min(c, m / a);
        ans = max(ans, take * a);
    }
    
    for (int i = 0; i + 1 < n; ++i) {
        if (flowers[i + 1].first != flowers[i].first + 1) continue;
        ll a1 = flowers[i].first, c1 = flowers[i].second;
        ll a2 = flowers[i + 1].first, c2 = flowers[i + 1].second;
        
        ll take1 = min(c1, m / a1);
        ll rem = m - take1 * a1;
        ll take2 = min(c2, rem / a2);
        ll cur = take1 * a1 + take2 * a2;
        ans = max(ans, cur);
        
        if (take1 > 0) {
            ll extra = min({take1, c2 - take2, rem % a2});
            cur += extra;
            ans = max(ans, cur);
        }
    }
    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) solve();
    return 0;
}
