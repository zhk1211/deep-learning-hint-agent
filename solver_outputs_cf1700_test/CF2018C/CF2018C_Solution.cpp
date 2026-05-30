// Solution
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    vector<int> depth(n + 1), max_depth(n + 1);
    function<void(int, int, int)> dfs = [&](int u, int p, int d) {
        depth[u] = d;
        max_depth[u] = d;
        for (int v : adj[u]) {
            if (v == p) continue;
            dfs(v, u, d + 1);
            max_depth[u] = max(max_depth[u], max_depth[v]);
        }
    };
    dfs(1, 0, 0);

    int max_d = *max_element(depth.begin() + 1, depth.end());
    vector<int> diff(max_d + 2, 0);
    for (int i = 1; i <= n; i++) {
        int l = depth[i];
        int r = max_depth[i];
        diff[l]++;
        diff[r + 1]--;
    }

    int cur = 0, best = 0;
    for (int d = 0; d <= max_d; d++) {
        cur += diff[d];
        best = max(best, cur);
    }

    cout << n - best << '\n';
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
