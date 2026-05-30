// Hint2
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, a, b;
    cin >> n >> a >> b;
    a--; b--;
    vector<vector<int>> adj(n);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        u--; v--;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Find the path from a to b and its length
    vector<int> parent(n, -1);
    queue<int> q;
    q.push(a);
    parent[a] = a;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (u == b) break;
        for (int v : adj[u]) {
            if (parent[v] == -1) {
                parent[v] = u;
                q.push(v);
            }
        }
    }

    vector<int> path;
    int cur = b;
    while (cur != a) {
        path.push_back(cur);
        cur = parent[cur];
    }
    path.push_back(a);
    reverse(path.begin(), path.end());
    int dist_ab = path.size() - 1;

    // The meeting point is the middle vertex of the path
    int meet = path[dist_ab / 2];

    // BFS from meet to find the farthest distance
    vector<int> dist(n, -1);
    queue<int> q2;
    q2.push(meet);
    dist[meet] = 0;
    int maxd = 0;
    while (!q2.empty()) {
        int u = q2.front(); q2.pop();
        for (int v : adj[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                maxd = max(maxd, dist[v]);
                q2.push(v);
            }
        }
    }

    // Answer = 2*(n-1) - maxd + dist_ab/2
    // Actually steps = (dist_ab + 1) / 2 + 2*(n-1) - maxd
    int steps = (dist_ab + 1) / 2 + 2 * (n - 1) - maxd;
    cout << steps << '\n';
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
