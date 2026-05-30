// Hint2
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

    // Find cycle nodes using topological sort (remove leaves)
    vector<int> deg(n);
    for (int i = 0; i < n; ++i) deg[i] = adj[i].size();
    queue<int> q;
    for (int i = 0; i < n; ++i) if (deg[i] == 1) q.push(i);
    vector<bool> in_cycle(n, true);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        in_cycle[u] = false;
        for (int v : adj[u]) {
            if (in_cycle[v]) {
                --deg[v];
                if (deg[v] == 1) q.push(v);
            }
        }
    }

    // If Valeriu starts on cycle, he can escape forever
    if (in_cycle[b]) {
        cout << "YES\n";
        return;
    }

    // BFS to find distances from a and b
    auto bfs = [&](int s) {
        vector<int> dist(n, -1);
        queue<int> q;
        dist[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    q.push(v);
                }
            }
        }
        return dist;
    };

    vector<int> da = bfs(a), db = bfs(b);

    // Find entry node for Valeriu: closest cycle node from b
    int entry = -1;
    int min_dist = n + 1;
    for (int i = 0; i < n; ++i) {
        if (in_cycle[i] && db[i] < min_dist) {
            min_dist = db[i];
            entry = i;
        }
    }

    // Valeriu can escape if he reaches entry strictly before Marcel
    if (db[entry] < da[entry]) {
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
    while (t--) solve();
    return 0;
}
