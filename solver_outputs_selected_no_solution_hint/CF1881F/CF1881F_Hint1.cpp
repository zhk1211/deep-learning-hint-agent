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
    // First BFS to find one endpoint of the diameter of marked vertices
    int start = marked[0];
    vector<int> dist(n + 1, -1);
    queue<int> q;
    q.push(start);
    dist[start] = 0;
    int far = start;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (is_marked[u] && dist[u] > dist[far]) far = u;
        for (int v : adj[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
    // Second BFS from far to find distances from far
    int u = far;
    vector<int> dist_u(n + 1, -1);
    q.push(u);
    dist_u[u] = 0;
    int v_far = u;
    while (!q.empty()) {
        int x = q.front(); q.pop();
        if (is_marked[x] && dist_u[x] > dist_u[v_far]) v_far = x;
        for (int y : adj[x]) {
            if (dist_u[y] == -1) {
                dist_u[y] = dist_u[x] + 1;
                q.push(y);
            }
        }
    }
    // Third BFS from v_far
    int v = v_far;
    vector<int> dist_v(n + 1, -1);
    q.push(v);
    dist_v[v] = 0;
    while (!q.empty()) {
        int x = q.front(); q.pop();
        for (int y : adj[x]) {
            if (dist_v[y] == -1) {
                dist_v[y] = dist_v[x] + 1;
                q.push(y);
            }
        }
    }
    int ans = INT_MAX;
    for (int i = 1; i <= n; ++i) {
        ans = min(ans, max(dist_u[i], dist_v[i]));
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
