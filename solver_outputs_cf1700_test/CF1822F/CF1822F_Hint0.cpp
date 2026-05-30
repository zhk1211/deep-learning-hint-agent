// Hint0
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void solve() {
    int n;
    ll k, c;
    cin >> n >> k >> c;
    vector<vector<int>> g(n);
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        --u; --v;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    vector<ll> depth(n);
    vector<int> parent(n, -1);
    function<void(int, int)> dfs1 = [&](int u, int p) {
        parent[u] = p;
        for (int v : g[u]) {
            if (v == p) continue;
            depth[v] = depth[u] + 1;
            dfs1(v, u);
        }
    };
    dfs1(0, -1);

    int far = max_element(depth.begin(), depth.end()) - depth.begin();
    depth.assign(n, 0);
    parent.assign(n, -1);
    dfs1(far, -1);
    int far2 = max_element(depth.begin(), depth.end()) - depth.begin();
    vector<ll> dist1 = depth;

    depth.assign(n, 0);
    parent.assign(n, -1);
    dfs1(far2, -1);
    vector<ll> dist2 = depth;

    ll ans = 0;
    for (int i = 0; i < n; ++i) {
        ll max_dist = max(dist1[i], dist2[i]);
        ll profit = max_dist * k - c * (dist1[i] + dist2[i] - max_dist) / 2;
        // Actually, the distance from root 1 to i is not directly needed.
        // We need to consider moving root from 1 to i.
        // The cost to move from 1 to i is c * distance(1, i).
        // The profit if root is i: max distance from i to any node * k - c * dist(1, i).
        // max distance from i is max(dist1[i], dist2[i]).
        // So we need dist(1, i).
    }

    // Recompute properly:
    // We need distance from original root 1 to every node.
    depth.assign(n, 0);
    parent.assign(n, -1);
    dfs1(0, -1);
    vector<ll> dist0 = depth;

    ans = 0;
    for (int i = 0; i < n; ++i) {
        ll max_dist = max(dist1[i], dist2[i]);
        ll profit = max_dist * k - dist0[i] * c;
        ans = max(ans, profit);
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
