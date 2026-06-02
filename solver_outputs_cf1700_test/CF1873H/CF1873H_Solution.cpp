#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, a, b;
    cin >> n >> a >> b;
    --a; --b;
    vector<vector<int>> adj(n);
    for (int i = 0; i < n; ++i) {
        int u, v;
        cin >> u >> v;
        --u; --v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    if (a == b) {
        cout << "NO\n";
        return;
    }
    vector<int> parent(n, -1);
    vector<bool> visited(n, false);
    int entry = -1;
    function<void(int, int)> dfs = [&](int u, int p) {
        visited[u] = true;
        parent[u] = p;
        for (int v : adj[u]) {
            if (v == p) continue;
            if (visited[v]) {
                if (entry == -1) entry = v;
            } else {
                dfs(v, u);
            }
        }
    };
    dfs(b, -1);
    vector<int> dist_b(n, -1);
    queue<int> q;
    dist_b[b] = 0;
    q.push(b);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (dist_b[v] == -1) {
                dist_b[v] = dist_b[u] + 1;
                q.push(v);
            }
        }
    }
    vector<int> dist_a(n, -1);
    dist_a[a] = 0;
    q.push(a);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (dist_a[v] == -1) {
                dist_a[v] = dist_a[u] + 1;
                q.push(v);
            }
        }
    }
    if (dist_b[entry] < dist_a[entry]) {
        cout << "YES\n";
    } else {
        cout << "NO\n";
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
