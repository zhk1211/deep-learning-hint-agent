// Hint4
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

    // First DFS: compute max depth from each node in its subtree (downwards)
    vector<ll> down1(n, 0), down2(n, 0);
    vector<int> best_child(n, -1);
    
    function<void(int, int)> dfs1 = [&](int u, int p) {
        for (int v : adj[u]) {
            if (v == p) continue;
            dfs1(v, u);
            ll val = down1[v] + k;
            if (val > down1[u]) {
                down2[u] = down1[u];
                down1[u] = val;
                best_child[u] = v;
            } else if (val > down2[u]) {
                down2[u] = val;
            }
        }
    };
    dfs1(0, -1);

    // Second DFS: compute max distance from each node to any leaf (global)
    vector<ll> max_dist(n, 0);
    function<void(int, int, ll)> dfs2 = [&](int u, int p, ll up_dist) {
        max_dist[u] = max(down1[u], up_dist);
        for (int v : adj[u]) {
            if (v == p) continue;
            ll best_down = (best_child[u] == v) ? down2[u] : down1[u];
            ll new_up = max(up_dist, best_down) + k;
            dfs2(v, u, new_up);
        }
    };
    dfs2(0, -1, 0);

    // Now we need to find max over all possible roots of (max_dist[root] - c * depth_from_original_root)
    // depth_from_original_root is just distance in edges from node 0 to node u.
    // Since each edge length is k, distance = depth * k, but cost is c per operation (edge move).
    // So profit if root is u: max_dist[u] - c * (dist_edges(0, u))
    // We can compute distances from 0 using BFS/DFS.
    vector<int> depth_edges(n, 0);
    function<void(int, int)> dfs_depth = [&](int u, int p) {
        for (int v : adj[u]) {
            if (v == p) continue;
            depth_edges[v] = depth_edges[u] + 1;
            dfs_depth(v, u);
        }
    };
    dfs_depth(0, -1);

    ll ans = 0;
    for (int i = 0; i < n; ++i) {
        ll profit = max_dist[i] - c * depth_edges[i];
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
