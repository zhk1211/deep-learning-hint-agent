#include <bits/stdc++.h>
using namespace std;

const long long INF = 1e18;

void solve() {
    int n, m;
    cin >> n >> m;
    vector<vector<pair<int, long long>>> adj(n + 1);
    for (int i = 0; i < m; ++i) {
        int a, b;
        long long d;
        cin >> a >> b >> d;
        // a is d meters in front of b => pos[a] - pos[b] = d
        // pos[a] <= pos[b] + d  and  pos[b] <= pos[a] - d
        adj[b].push_back({a, d});
        adj[a].push_back({b, -d});
    }
    
    vector<long long> dist(n + 1, INF);
    // We need to check all components, so we run Bellman-Ford from each unvisited node.
    // Since graph may be disconnected, we can just initialize dist to 0 for all nodes
    // and run Bellman-Ford n times; if any update on n-th iteration, negative cycle exists.
    // But we must ensure we don't get false positive from INF.
    // Better: run from each unvisited node with dist = 0.
    vector<bool> visited(n + 1, false);
    for (int start = 1; start <= n; ++start) {
        if (visited[start]) continue;
        dist[start] = 0;
        // Bellman-Ford for this component
        bool updated;
        for (int iter = 0; iter < n; ++iter) {
            updated = false;
            for (int u = 1; u <= n; ++u) {
                if (dist[u] == INF) continue;
                for (auto &[v, w] : adj[u]) {
                    if (dist[v] > dist[u] + w) {
                        dist[v] = dist[u] + w;
                        updated = true;
                        visited[v] = true;
                    }
                }
            }
            if (!updated) break;
        }
        if (updated) {
            cout << "NO\n";
            return;
        }
        // Mark all reachable nodes as visited
        queue<int> q;
        q.push(start);
        visited[start] = true;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto &[v, w] : adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
    }
    cout << "YES\n";
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
