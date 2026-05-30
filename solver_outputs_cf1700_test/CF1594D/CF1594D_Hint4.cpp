// Hint4
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, m;
    cin >> n >> m;
    vector<vector<pair<int, int>>> adj(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v;
        string s;
        cin >> u >> v >> s;
        int w = (s == "imposter" ? 1 : 0);
        adj[u].emplace_back(v, w);
        adj[v].emplace_back(u, w);
    }
    vector<int> col(n + 1, -1);
    int ans = 0;
    for (int i = 1; i <= n; ++i) {
        if (col[i] != -1) continue;
        queue<int> q;
        q.push(i);
        col[i] = 0;
        int cnt[2] = {0, 0};
        cnt[0]++;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto [v, w] : adj[u]) {
                int nc = col[u] ^ w;
                if (col[v] == -1) {
                    col[v] = nc;
                    cnt[nc]++;
                    q.push(v);
                } else if (col[v] != nc) {
                    cout << -1 << '\n';
                    return;
                }
            }
        }
        ans += max(cnt[0], cnt[1]);
    }
    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) solve();
    return 0;
}
