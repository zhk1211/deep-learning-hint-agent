#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, a, b;
    cin >> n >> a >> b;
    vector<vector<pair<int,int>>> g(n+1);
    for (int i = 0; i < n-1; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }
    
    set<int> reachable_from_a;
    queue<pair<int,int>> q;
    q.push({a, 0});
    vector<bool> vis(n+1, false);
    vis[a] = true;
    while (!q.empty()) {
        auto [u, x] = q.front(); q.pop();
        reachable_from_a.insert(x);
        for (auto [v, w] : g[u]) {
            if (v == b) continue;
            if (!vis[v]) {
                vis[v] = true;
                q.push({v, x ^ w});
            }
        }
    }
    
    bool ok = false;
    queue<pair<int,int>> q2;
    q2.push({b, 0});
    fill(vis.begin(), vis.end(), false);
    vis[b] = true;
    while (!q2.empty()) {
        auto [u, x] = q2.front(); q2.pop();
        for (auto [v, w] : g[u]) {
            if (!vis[v]) {
                vis[v] = true;
                int nx = x ^ w;
                if (reachable_from_a.count(nx)) {
                    ok = true;
                }
                q2.push({v, nx});
            }
        }
    }
    
    cout << (ok ? "YES" : "NO") << '\n';
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
