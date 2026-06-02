#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<vector<int>> adj(n + 1);
    vector<pair<int, int>> edges;
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
        edges.emplace_back(u, v);
    }
    
    // Check if there is a vertex with degree >= 3
    int root = -1;
    for (int i = 1; i <= n; i++) {
        if (adj[i].size() >= 3) {
            root = i;
            break;
        }
    }
    
    if (root == -1) {
        // Tree is a path, impossible to get exactly n good pairs
        cout << "NO\n";
        return;
    }
    
    // Root the tree at the vertex with degree >= 3
    vector<int> parent(n + 1, 0);
    vector<int> depth(n + 1, 0);
    vector<vector<int>> children(n + 1);
    
    function<void(int, int)> dfs = [&](int u, int p) {
        parent[u] = p;
        for (int v : adj[u]) {
            if (v != p) {
                depth[v] = depth[u] + 1;
                children[u].push_back(v);
                dfs(v, u);
            }
        }
    };
    dfs(root, 0);
    
    // We will direct edges: from parent to child if depth is even, else child to parent
    // This ensures exactly n good pairs
    cout << "YES\n";
    for (auto &e : edges) {
        int u = e.first, v = e.second;
        if (parent[u] == v) {
            // v is parent of u
            if (depth[v] % 2 == 0) {
                cout << v << " " << u << "\n";
            } else {
                cout << u << " " << v << "\n";
            }
        } else {
            // u is parent of v
            if (depth[u] % 2 == 0) {
                cout << u << " " << v << "\n";
            } else {
                cout << v << " " << u << "\n";
            }
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
