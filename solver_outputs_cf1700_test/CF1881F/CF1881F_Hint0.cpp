// Hint0
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
    // First BFS to find one endpoint of the diameter of marked nodes
    vector<int> dist(n + 1, -1);
    queue<int> q;
    q.push(marked[0]);
    dist[marked[0]] = 0;
    int far = marked[0];
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
                if (is_marked[v]) far = v;
            }
        }
    }
    // Second BFS from far to find the other endpoint and distances
    fill(dist.begin(), dist.end(), -1);
    q.push(far);
    dist[far] = 0;
    int other = far;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
                if (is_marked[v]) other = v;
            }
        }
    }
    // Third BFS from other to get distances to all nodes
    vector<int> dist2(n + 1, -1);
    q.push(other);
    dist2[other] = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (dist2[v] == -1) {
                dist2[v] = dist2[u] + 1;
                q.push(v);
            }
        }
    }
    // The answer is the minimum over all vertices of max(dist from far, dist from other)
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
