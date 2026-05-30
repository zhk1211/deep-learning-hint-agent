// Hint4
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

    // Check feasibility
    for (int u = 1; u <= n; ++u) {
        int c = t[u];
        // Check that all colors < c appear among neighbors
        vector<bool> seen(c, false);
        for (int v : adj[u]) {
            if (t[v] < c) seen[t[v]] = true;
            if (t[v] == c) {
                // Two adjacent vertices cannot have the same color
                cout << -1 << '\n';
                return 0;
            }
        }
        for (int col = 1; col < c; ++col) {
            if (!seen[col]) {
                cout << -1 << '\n';
                return 0;
            }
        }
    }

    // Build order: sort by desired topic, then by index
    vector<int> order(n);
    iota(order.begin(), order.end(), 1);
    sort(order.begin(), order.end(), [&](int a, int b) {
        if (t[a] != t[b]) return t[a] < t[b];
        return a < b;
    });

    // Simulate to verify
    vector<int> assigned(n + 1, 0);
    for (int u : order) {
        int c = t[u];
        // Find mex among assigned neighbors
        vector<bool> used(c + 1, false);
        for (int v : adj[u]) {
            if (assigned[v] != 0 && assigned[v] <= c) {
                used[assigned[v]] = true;
            }
        }
        int mex = 1;
        while (mex <= c && used[mex]) ++mex;
        if (mex != c) {
            cout << -1 << '\n';
            return 0;
        }
        assigned[u] = c;
    }

    for (int i = 0; i < n; ++i) {
        cout << order[i] << " \n"[i == n - 1];
    }

    return 0;
}
