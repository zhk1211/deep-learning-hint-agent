// Hint7
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void solve() {
    int n;
    ll k, c;
    cin >> n >> k >> c;
    vector<vector<int>> adj(n);
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        --u; --v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    vector<ll> down(n, 0);
    function<void(int, int)> dfs1 = [&](int u, int p) {
        ll best = 0;
        for (int v : adj[u]) {
            if (v == p) continue;
            dfs1(v, u);
            best = max(best, down[v] + k);
        }
        down[u] = best;
    };
    dfs1(0, -1);

    vector<ll> up(n, 0);
    function<void(int, int)> dfs2 = [&](int u, int p) {
        ll mx1 = -1, mx2 = -1;
        int best_v = -1;
        for (int v : adj[u]) {
            if (v == p) continue;
            ll val = down[v] + k;
            if (val > mx1) {
                mx2 = mx1;
                mx1 = val;
                best_v = v;
            } else if (val > mx2) {
                mx2 = val;
            }
        }
        for (int v : adj[u]) {
            if (v == p) continue;
            ll use = (v == best_v) ? mx2 : mx1;
            up[v] = max(up[u] + k, max(use, 0LL));
            dfs2(v, u);
        }
    };
    dfs2(0, -1);

    ll ans = 0;
    for (int i = 0; i < n; ++i) {
        ll dist = max(down[i], up[i]);
        ll profit = dist - c * i; // distance from root 0 to i is i edges? Wait, cost depends on number of operations.
        // Actually, shifting root from 0 to i takes depth[i] operations, where depth is number of edges.
        // We need depth from original root 0.
    }

    // Compute depth from 0
    vector<int> depth(n, 0);
    function<void(int, int)> dfs_depth = [&](int u, int p) {
        for (int v : adj[u]) {
            if (v == p) continue;
            depth[v] = depth[u] + 1;
            dfs_depth(v, u);
        }
    };
    dfs_depth(0, -1);

    for (int i = 0; i < n; ++i) {
        ll dist = max(down[i], up[i]);
        ll profit = dist - c * depth[i];
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
