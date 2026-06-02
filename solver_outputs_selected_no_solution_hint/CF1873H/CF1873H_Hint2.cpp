#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, a, b;
    cin >> n >> a >> b;
    --a; --b;
    vector<vector<int>> g(n);
    for (int i = 0; i < n; ++i) {
        int u, v;
        cin >> u >> v;
        --u; --v;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    // find cycle
    vector<int> deg(n);
    for (int i = 0; i < n; ++i) deg[i] = g[i].size();
    queue<int> q;
    for (int i = 0; i < n; ++i) if (deg[i] == 1) q.push(i);
    vector<bool> in_cycle(n, true);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        in_cycle[u] = false;
        for (int v : g[u]) {
            if (in_cycle[v] && --deg[v] == 1) {
                q.push(v);
            }
        }
    }

    // if a == b, Marcel already caught
    if (a == b) {
        cout << "NO\n";
        return;
    }

    // if b is in cycle, Valeriu can escape forever
    if (in_cycle[b]) {
        cout << "YES\n";
        return;
    }

    // find entry point of Valeriu to cycle
    vector<int> dist_b(n, -1);
    queue<int> qb;
    dist_b[b] = 0;
    qb.push(b);
    int entry_b = -1;
    while (!qb.empty()) {
        int u = qb.front(); qb.pop();
        if (in_cycle[u]) {
            entry_b = u;
            break;
        }
        for (int v : g[u]) {
            if (dist_b[v] == -1) {
                dist_b[v] = dist_b[u] + 1;
                qb.push(v);
            }
        }
    }

    // distance from a to entry_b
    vector<int> dist_a(n, -1);
    queue<int> qa;
    dist_a[a] = 0;
    qa.push(a);
    while (!qa.empty()) {
        int u = qa.front(); qa.pop();
        if (u == entry_b) break;
        for (int v : g[u]) {
            if (dist_a[v] == -1) {
                dist_a[v] = dist_a[u] + 1;
                qa.push(v);
            }
        }
    }

    int da = dist_a[entry_b];
    int db = dist_b[entry_b];

    if (db < da) {
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
