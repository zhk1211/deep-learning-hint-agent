#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    int r = -1;
    for (int i = 1; i <= n; i++) {
        if (adj[i].size() == 2) {
            r = i;
            break;
        }
    }
    
    if (r == -1) {
        cout << "NO\n";
        return;
    }
    
    cout << "YES\n";
    
    int u = adj[r][0];
    int v = adj[r][1];
    
    vector<pair<int, int>> ans;
    ans.push_back({u, r});
    ans.push_back({r, v});
    
    function<void(int, int, bool)> dfs = [&](int node, int parent, bool out) {
        for (int nb : adj[node]) {
            if (nb == parent) continue;
            if (out) {
                ans.push_back({node, nb});
            } else {
                ans.push_back({nb, node});
            }
            dfs(nb, node, out);
        }
    };
    
    dfs(u, r, true);
    dfs(v, r, false);
    
    for (auto [x, y] : ans) {
        cout << x << " " << y << "\n";
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
