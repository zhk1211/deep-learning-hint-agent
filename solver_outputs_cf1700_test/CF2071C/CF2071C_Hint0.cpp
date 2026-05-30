// Hint0
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, st, en;
    cin >> n >> st >> en;
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    vector<int> parent(n + 1, -1);
    vector<int> depth(n + 1, 0);
    function<void(int, int)> dfs = [&](int u, int p) {
        parent[u] = p;
        for (int v : adj[u]) {
            if (v == p) continue;
            depth[v] = depth[u] + 1;
            dfs(v, u);
        }
    };
    dfs(en, -1);
    
    vector<int> path;
    int cur = st;
    while (cur != en) {
        path.push_back(cur);
        cur = parent[cur];
    }
    path.push_back(en);
    
    vector<int> order;
    vector<bool> on_path(n + 1, false);
    for (int v : path) on_path[v] = true;
    
    vector<int> subtree;
    function<void(int, int)> collect = [&](int u, int p) {
        if (!on_path[u]) subtree.push_back(u);
        for (int v : adj[u]) {
            if (v == p) continue;
            if (on_path[v]) continue;
            collect(v, u);
        }
    };
    
    for (int v : path) {
        collect(v, -1);
    }
    
    reverse(path.begin(), path.end());
    for (int v : path) order.push_back(v);
    for (int v : subtree) order.push_back(v);
    
    for (int i = 0; i < n; i++) {
        cout << order[i] << (i + 1 == n ? '\n' : ' ');
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
