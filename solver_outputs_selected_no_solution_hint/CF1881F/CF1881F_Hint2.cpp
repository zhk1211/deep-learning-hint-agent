#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, k;
    cin >> n >> k;
    vector<int> marked(k);
    vector<bool> is_marked(n + 1, false);
    for (int i = 0; i < k; ++i) {
        cin >> marked[i];
        is_marked[marked[i]] = true;
    }
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    if (k == 1) {
        cout << 0 << '\n';
        return;
    }
    vector<int> dist(n + 1, -1);
    function<void(int, int, int)> dfs = [&](int u, int p, int d) {
        dist[u] = d;
        for (int v : adj[u]) {
            if (v != p) {
                dfs(v, u, d + 1);
            }
        }
    };
    dfs(marked[0], 0, 0);
    int far = marked[0];
    for (int i = 1; i <= n; ++i) {
        if (is_marked[i] && dist[i] > dist[far]) {
            far = i;
        }
    }
    dfs(far, 0, 0);
    int far2 = far;
    for (int i = 1; i <= n; ++i) {
        if (is_marked[i] && dist[i] > dist[far2]) {
            far2 = i;
        }
    }
    int diameter = dist[far2];
    vector<int> dist1 = dist;
    dfs(far2, 0, 0);
    vector<int> dist2 = dist;
    int ans = n;
    for (int i = 1; i <= n; ++i) {
        ans = min(ans, max(dist1[i], dist2[i]));
    }
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
