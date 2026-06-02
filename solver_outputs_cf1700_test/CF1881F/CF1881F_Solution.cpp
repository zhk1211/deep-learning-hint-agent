#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, k;
    cin >> n >> k;
    vector<int> marked(k);
    for (int i = 0; i < k; ++i) {
        cin >> marked[i];
        --marked[i];
    }
    vector<vector<int>> adj(n);
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        --u; --v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    if (k == 1) {
        cout << 0 << '\n';
        return;
    }
    vector<bool> is_marked(n, false);
    for (int v : marked) is_marked[v] = true;
    
    auto bfs = [&](int start) -> pair<int, int> {
        vector<int> dist(n, -1);
        queue<int> q;
        dist[start] = 0;
        q.push(start);
        int farthest = start;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            if (is_marked[u] && dist[u] > dist[farthest]) {
                farthest = u;
            }
            for (int v : adj[u]) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    q.push(v);
                }
            }
        }
        return {farthest, dist[farthest]};
    };
    
    int v1 = marked[0];
    auto [v2, d1] = bfs(v1);
    auto [v3, d2] = bfs(v2);
    cout << (d2 + 1) / 2 << '\n';
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
