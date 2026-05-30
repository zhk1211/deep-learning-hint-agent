// Hint1
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, k;
    cin >> n >> k;
    vector<int> marked(k);
    vector<bool> is_marked(n + 1, false);
    for (int i = 0; i < k; ++i) {
        cin >> marked[i];
        is_marked[marked[i]] = true;
    }
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    if (k == 0) {
        cout << 0 << '\n';
        return;
    }
    // First BFS to find farthest marked node from an arbitrary marked node
    int start = marked[0];
    vector<int> dist(n + 1, -1);
    queue<int> q;
    q.push(start);
    dist[start] = 0;
    int far1 = start;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (is_marked[u]) far1 = u;
        for (int v : adj[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
    // Second BFS from far1 to find farthest marked node and distances
    fill(dist.begin(), dist.end(), -1);
    q.push(far1);
    dist[far1] = 0;
    int far2 = far1;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (is_marked[u]) far2 = u;
        for (int v : adj[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
    // Distances from far2
    vector<int> dist2(n + 1, -1);
    q.push(far2);
    dist2[far2] = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (dist2[v] == -1) {
                dist2[v] = dist2[u] + 1;
                q.push(v);
            }
        }
    }
    int ans = INT_MAX;
    for (int i = 1; i <= n; ++i) {
        ans = min(ans, max(dist[i], dist2[i]));
    }
    cout << ans << '\n';
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
