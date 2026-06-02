#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void solve() {
    int n;
    cin >> n;
    vector<int> p(n + 1);
    for (int i = 2; i <= n; ++i) {
        cin >> p[i];
    }
    vector<ll> l(n + 1), r(n + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> l[i] >> r[i];
    }

    vector<vector<int>> children(n + 1);
    for (int i = 2; i <= n; ++i) {
        children[p[i]].push_back(i);
    }

    int ans = 0;
    function<ll(int)> dfs = [&](int u) -> ll {
        ll sum = 0;
        for (int v : children[u]) {
            sum += dfs(v);
        }
        if (sum < l[u]) {
            ++ans;
            return r[u];
        } else {
            return min(sum, r[u]);
        }
    };

    dfs(1);
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
