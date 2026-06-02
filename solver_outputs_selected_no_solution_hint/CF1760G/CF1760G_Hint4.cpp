#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, a, b;
    cin >> n >> a >> b;
    vector<vector<pair<int, int>>> adj(n + 1);
    for (int i = 0; i < n - 1; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }
    
    set<int> reachable_from_a;
    queue<pair<int, int>> q;
    q.push({a, 0});
    vector<bool> visited(n + 1, false);
    visited[a] = true;
    while (!q.empty()) {
        auto [u, x] = q.front(); q.pop();
        reachable_from_a.insert(x);
        for (auto [v, w] : adj[u]) {
            if (v == b) continue;
            if (!visited[v]) {
                visited[v] = true;
                q.push({v, x ^ w});
            }
        }
    }
    
    fill(visited.begin(), visited.end(), false);
    queue<pair<int, int>> q2;
    q2.push({b, 0});
    visited[b] = true;
    bool found = false;
    while (!q2.empty()) {
        auto [u, x] = q2.front(); q2.pop();
        for (auto [v, w] : adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                int nx = x ^ w;
                if (reachable_from_a.count(nx)) {
                    found = true;
                    break;
                }
                q2.push({v, nx});
            }
        }
        if (found) break;
    }
    
    cout << (found ? "YES" : "NO") << '\n';
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
