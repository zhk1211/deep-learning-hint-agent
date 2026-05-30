// Hint5
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; ++i) {
        int a, b;
        cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    vector<int> t(n + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> t[i];
    }

    // Check condition 1: no edge between same color
    for (int u = 1; u <= n; ++u) {
        for (int v : adj[u]) {
            if (t[u] == t[v]) {
                cout << -1 << '\n';
                return 0;
            }
        }
    }

    // Check condition 2: for each vertex u with color c,
    // all colors 1..c-1 must appear among its neighbors
    for (int u = 1; u <= n; ++u) {
        int c = t[u];
        vector<bool> seen(c, false);
        int cnt = 0;
        for (int v : adj[u]) {
            int cv = t[v];
            if (cv < c && !seen[cv]) {
                seen[cv] = true;
                ++cnt;
            }
        }
        if (cnt != c - 1) {
            cout << -1 << '\n';
            return 0;
        }
    }

    // Build order: sort by color, then by index
    vector<int> order(n);
    iota(order.begin(), order.end(), 1);
    sort(order.begin(), order.end(), [&](int a, int b) {
        if (t[a] != t[b]) return t[a] < t[b];
        return a < b;
    });

    for (int i = 0; i < n; ++i) {
        cout << order[i] << " \n"[i == n - 1];
    }

    return 0;
}
