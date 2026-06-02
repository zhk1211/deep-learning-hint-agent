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
        adj[a].push_back({b, d});
        adj[b].push_back({a, -d});
    }
    
    vector<long long> dist(n + 1, INF);
    vector<bool> visited(n + 1, false);
    
    for (int start = 1; start <= n; ++start) {
        if (visited[start]) continue;
        queue<int> q;
        q.push(start);
        dist[start] = 0;
        visited[start] = true;
        
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto &edge : adj[u]) {
                int v = edge.first;
                long long w = edge.second;
                if (!visited[v]) {
                    visited[v] = true;
                    dist[v] = dist[u] + w;
                    q.push(v);
                } else if (dist[v] != dist[u] + w) {
                    cout << "NO\n";
                    return;
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
