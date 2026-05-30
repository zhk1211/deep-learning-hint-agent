// Hint1
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, a, b;
    cin >> n >> a >> b;
    --a; --b;
    vector<vector<int>> g(n);
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        --u; --v;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    // Find the path from a to b
    vector<int> parent(n, -1);
    queue<int> q;
    q.push(a);
    parent[a] = a;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (u == b) break;
        for (int v : g[u]) {
            if (parent[v] == -1) {
                parent[v] = u;
                q.push(v);
            }
        }
    }

    vector<int> path;
    for (int v = b; v != a; v = parent[v]) {
        path.push_back(v);
    }
    path.push_back(a);
    reverse(path.begin(), path.end());
    int len = path.size();
    int meet = path[(len - 1) / 2]; // meeting point

    // BFS to find farthest distance from meet
    vector<int> dist(n, -1);
    queue<int> q2;
    q2.push(meet);
    dist[meet] = 0;
    int maxd = 0;
    while (!q2.empty()) {
        int u = q2.front(); q2.pop();
        for (int v : g[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                maxd = max(maxd, dist[v]);
                q2.push(v);
            }
        }
    }

    long long ans = (len - 1) + 2LL * (n - 1) - maxd;
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
