#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<vector<int>> adj(n + 1);
    vector<pair<int, int>> edges;
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
        edges.emplace_back(u, v);
    }
    
    // Check if n is odd
    if (n % 2 == 0) {
        cout << "NO\n";
        return;
    }
    
    // Find a centroid-like vertex: we need a vertex whose removal splits tree into components of even sizes
    // Actually, we need to root the tree and direct edges from parent to child if subtree size is odd, else child to parent
    // But we need exactly n good pairs. The condition is n odd.
    // We'll root at 1 and compute subtree sizes.
    vector<int> sz(n + 1, 0);
    vector<int> parent(n + 1, 0);
    vector<vector<int>> tree(n + 1);
    
    function<void(int, int)> dfs = [&](int u, int p) {
        sz[u] = 1;
        parent[u] = p;
        for (int v : adj[u]) {
            if (v == p) continue;
            tree[u].push_back(v);
            dfs(v, u);
            sz[u] += sz[v];
        }
    };
    dfs(1, 0);
    
    // Now direct edges: for each edge (u, v) where u is parent of v,
    // if sz[v] is odd, direct u -> v, else v -> u.
    // This yields exactly n good pairs.
    cout << "YES\n";
    for (auto [u, v] : edges) {
        if (parent[u] == v) swap(u, v); // now u is parent of v
        if (sz[v] % 2 == 1) {
            cout << u << " " << v << "\n";
        } else {
            cout << v << " " << u << "\n";
        }
    }
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
