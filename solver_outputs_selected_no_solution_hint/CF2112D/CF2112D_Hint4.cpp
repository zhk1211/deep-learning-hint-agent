#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<vector<int>> adj(n);
    vector<pair<int,int>> edges;
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        u--; v--;
        adj[u].push_back(v);
        adj[v].push_back(u);
        edges.emplace_back(u, v);
    }
    
    // Check if n is odd: impossible to have exactly n good pairs
    if (n % 2 == 1) {
        cout << "NO\n";
        return;
    }
    
    // Find a centroid-like decomposition: we need a perfect matching
    // We'll root the tree at 0 and compute subtree sizes
    vector<int> sz(n, 0);
    vector<int> parent(n, -1);
    vector<int> order;
    function<void(int,int)> dfs = [&](int u, int p) {
        parent[u] = p;
        order.push_back(u);
        sz[u] = 1;
        for (int v : adj[u]) {
            if (v != p) {
                dfs(v, u);
                sz[u] += sz[v];
            }
        }
    };
    dfs(0, -1);
    
    // Check if there is a vertex with odd subtree size that is not the root
    // Actually, we need to pair vertices such that each edge direction is from parent to child or vice versa
    // The condition for exactly n good pairs is that the tree can be partitioned into n/2 directed paths of length 1 (edges)
    // This is equivalent to existence of a perfect matching in the tree.
    // A tree has a perfect matching iff for every vertex, when removed, the number of odd components is at most 1.
    // Alternatively, we can greedily match leaves.
    
    vector<int> deg(n);
    for (int i = 0; i < n; i++) deg[i] = adj[i].size();
    vector<bool> removed(n, false);
    vector<pair<int,int>> directed_edges;
    queue<int> q;
    for (int i = 0; i < n; i++) if (deg[i] == 1) q.push(i);
    
    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (removed[u]) continue;
        // find a neighbor not removed
        int p = -1;
        for (int v : adj[u]) if (!removed[v]) { p = v; break; }
        if (p == -1) continue;
        // match u and p
        removed[u] = true;
        removed[p] = true;
        directed_edges.emplace_back(u, p); // u -> p
        // update degrees of neighbors of p
        for (int v : adj[p]) {
            if (!removed[v]) {
                deg[v]--;
                if (deg[v] == 1) q.push(v);
            }
        }
    }
    
    if (directed_edges.size() != n / 2) {
        cout << "NO\n";
        return;
    }
    
    cout << "YES\n";
    for (auto &e : directed_edges) {
        cout << e.first + 1 << " " << e.second + 1 << "\n";
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
