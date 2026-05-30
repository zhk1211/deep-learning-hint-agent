// Hint3
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

    // Check if the desired coloring is valid under the greedy algorithm
    for (int u = 1; u <= n; ++u) {
        set<int> neighbor_colors;
        for (int v : adj[u]) {
            if (t[v] == t[u]) {
                cout << -1 << '\n';
                return 0;
            }
            if (t[v] < t[u]) {
                neighbor_colors.insert(t[v]);
            }
        }
        // The greedy algorithm picks the smallest positive integer not in neighbor_colors
        // So the number of distinct colors less than t[u] must be exactly t[u] - 1
        if ((int)neighbor_colors.size() != t[u] - 1) {
            cout << -1 << '\n';
            return 0;
        }
    }

    // Construct order: sort by desired topic, then by index
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
