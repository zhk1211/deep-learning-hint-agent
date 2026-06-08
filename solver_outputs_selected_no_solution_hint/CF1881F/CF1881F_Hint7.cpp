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
    if (k == 1) {
        cout << 0 << '\n';
        return;
    }
    // First BFS to find one endpoint of the diameter of marked vertices
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
                if (is_marked[v] && dist[v] > dist[far]) far = v;
            }
        }
    }
    // Second BFS from far to find the other endpoint and distances
    fill(dist.begin(), dist.end(), -1);
    q.push(far);
    dist[far] = 0;
    int far2 = far;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
                if (is_marked[v] && dist[v] > dist[far2]) far2 = v;
            }
        }
    }
    vector<int> d1 = dist;
    // Third BFS from far2
    fill(dist.begin(), dist.end(), -1);
    q.push(far2);
    dist[far2] = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
    vector<int> d2 = dist;
    int ans = n;
    for (int i = 1; i <= n; ++i) {
        ans = min(ans, max(d1[i], d2[i]));
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
