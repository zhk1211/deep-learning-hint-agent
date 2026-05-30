// Hint5
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, a, b;
    cin >> n >> a >> b;
    vector<vector<pair<int, int>>> adj(n + 1);
    for (int i = 0; i < n - 1; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    set<int> reachable_from_a;
    function<void(int, int, int)> dfs_a = [&](int u, int p, int x) {
        reachable_from_a.insert(x);
        for (auto [v, w] : adj[u]) {
            if (v == p || v == b) continue;
            dfs_a(v, u, x ^ w);
        }
    };
    dfs_a(a, 0, 0);

    bool found = false;
    function<void(int, int, int)> dfs_b = [&](int u, int p, int x) {
        if (u != b && reachable_from_a.count(x)) {
            found = true;
            return;
        }
        for (auto [v, w] : adj[u]) {
            if (v == p) continue;
            dfs_b(v, u, x ^ w);
            if (found) return;
        }
    };
    dfs_b(b, 0, 0);

    cout << (found ? "YES" : "NO") << '\n';
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
