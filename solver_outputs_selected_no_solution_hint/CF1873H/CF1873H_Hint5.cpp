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

    // Find the cycle in the graph (since n edges, n nodes, exactly one cycle)
    vector<int> deg(n);
    for (int i = 0; i < n; ++i) deg[i] = adj[i].size();
    queue<int> q;
    for (int i = 0; i < n; ++i) if (deg[i] == 1) q.push(i);
    vector<bool> in_cycle(n, true);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        in_cycle[u] = false;
        for (int v : adj[u]) {
            if (--deg[v] == 1) q.push(v);
        }
    }

    // If Valeriu starts on the cycle, he can escape forever if a != b
    if (in_cycle[b]) {
        if (a == b) cout << "NO\n";
        else cout << "YES\n";
        return;
    }

    // Find the entry point to the cycle from Valeriu's start
    vector<int> dist_b(n, -1);
    queue<int> qb;
    dist_b[b] = 0;
    qb.push(b);
    int entry = -1;
    while (!qb.empty()) {
        int u = qb.front(); qb.pop();
        if (in_cycle[u]) {
            entry = u;
            break;
        }
        for (int v : adj[u]) {
            if (dist_b[v] == -1) {
                dist_b[v] = dist_b[u] + 1;
                qb.push(v);
            }
        }
    }

    // Distance from Marcel to entry
    vector<int> dist_a(n, -1);
    queue<int> qa;
    dist_a[a] = 0;
    qa.push(a);
    while (!qa.empty()) {
        int u = qa.front(); qa.pop();
        for (int v : adj[u]) {
            if (dist_a[v] == -1) {
                dist_a[v] = dist_a[u] + 1;
                qa.push(v);
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
