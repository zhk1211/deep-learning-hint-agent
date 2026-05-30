// Hint5
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

    // First DFS: compute max depth in subtree (downward)
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

    // Second DFS: compute max distance to any leaf from each node
    vector<ll> max_dist(n, 0);
    function<void(int, int, ll)> dfs2 = [&](int u, int p, ll up_dist) {
        max_dist[u] = max(down1[u], up_dist);
        for (int v : adj[u]) {
            if (v == p) continue;
            ll best_down = (v == best_child[u]) ? down2[u] : down1[u];
            ll new_up = max(up_dist, best_down) + k;
            dfs2(v, u, new_up);
        }
    };
    dfs2(0, -1, 0);

    // Third DFS: compute answer by trying each node as root
    ll ans = 0;
    function<void(int, int, int)> dfs3 = [&](int u, int p, int depth) {
        ll profit = max_dist[u] - depth * c;
        ans = max(ans, profit);
        for (int v : adj[u]) {
            if (v == p) continue;
            dfs3(v, u, depth + 1);
        }
    };
    dfs3(0, -1, 0);

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
