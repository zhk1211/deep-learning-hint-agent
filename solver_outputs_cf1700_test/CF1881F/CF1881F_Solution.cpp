#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n, k;
        cin >> n >> k;
        vector<bool> marked(n + 1, false);
        vector<int> mark_list(k);
        for (int i = 0; i < k; ++i) {
            cin >> mark_list[i];
            marked[mark_list[i]] = true;
        }

        vector<vector<int>> adj(n + 1);
        for (int i = 0; i < n - 1; ++i) {
            int u, v;
            cin >> u >> v;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        if (k == 0) {
            cout << 0 << '\n';
            continue;
        }

        auto bfs = [&](int start) -> pair<int, int> {
            vector<int> dist(n + 1, -1);
            queue<int> q;
            dist[start] = 0;
            q.push(start);
            int far = start, max_d = 0;
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                if (marked[u] && dist[u] > max_d) {
                    max_d = dist[u];
                    far = u;
                }
                for (int v : adj[u]) {
                    if (dist[v] == -1) {
                        dist[v] = dist[u] + 1;
                        q.push(v);
                    }
                }
            }
            return {far, max_d};
        };

        int v1 = mark_list[0];
        auto [v2, _] = bfs(v1);
        auto [v3, d] = bfs(v2);
        int ans = (d + 1) / 2; // ceil(d/2)
        cout << ans << '\n';
    }

    return 0;
}
