#include <bits/stdc++.h>
using namespace std;

void solve() {
    int t;
    cin >> t;
    while (t--) {
        int n, m;
        cin >> n >> m;
        vector<vector<pair<int, long long>>> adj(n + 1);
        for (int i = 0; i < m; ++i) {
            int a, b; long long d;
            cin >> a >> b >> d;
            adj[a].emplace_back(b, d);
            adj[b].emplace_back(a, -d);
        }

        vector<long long> pos(n + 1, 0);
        vector<bool> vis(n + 1, false);
        bool ok = true;

        for (int i = 1; i <= n && ok; ++i) {
            if (!vis[i]) {
                queue<int> q;
                q.push(i);
                vis[i] = true;
                pos[i] = 0;
                while (!q.empty() && ok) {
                    int u = q.front(); q.pop();
                    for (auto &[v, w] : adj[u]) {
                        if (!vis[v]) {
                            vis[v] = true;
                            pos[v] = pos[u] + w;
                            q.push(v);
                        } else {
                            if (pos[v] != pos[u] + w) {
                                ok = false;
                                break;
                            }
                        }
                    }
                }
            }
        }

        cout << (ok ? "YES" : "NO") << '\n';
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}
