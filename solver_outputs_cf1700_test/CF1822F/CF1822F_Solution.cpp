// Solution
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        int n;
        ll k, c;
        cin >> n >> k >> c;
        vector<vector<int>> adj(n + 1);
        for (int i = 0; i < n - 1; ++i) {
            int u, v;
            cin >> u >> v;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
        vector<int> parent(n + 1), depth(n + 1);
        vector<int> order;
        order.reserve(n);
        queue<int> q;
        q.push(1);
        parent[1] = 0;
        depth[1] = 0;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            order.push_back(u);
            for (int v : adj[u]) {
                if (v != parent[u]) {
                    parent[v] = u;
                    depth[v] = depth[u] + 1;
                    q.push(v);
                }
            }
        }
        vector<ll> down1(n + 1, 0), down2(n + 1, 0);
        vector<int> best(n + 1, 0);
        // bottom-up
        for (int i = n - 1; i >= 0; --i) {
            int u = order[i];
            for (int v : adj[u]) {
                if (v != parent[u]) {
                    ll cand = down1[v] + 1;
                    if (cand > down1[u]) {
                        down2[u] = down1[u];
                        down1[u] = cand;
                        best[u] = v;
                    } else if (cand > down2[u]) {
                        down2[u] = cand;
                    }
                }
            }
        }
        vector<ll> up(n + 1, 0);
        // top-down
        for (int i = 0; i < n; ++i) {
            int u = order[i];
            for (int v : adj[u]) {
                if (v != parent[u]) {
                    ll from_up = up[u] + 1;
                    ll from_sib = (best[u] == v) ? down2[u] + 1 : down1[u] + 1;
                    up[v] = max(from_up, from_sib);
                }
            }
        }
        ll ans = 0;
        for (int v = 1; v <= n; ++v) {
            ll dist = max(down1[v], up[v]);
            ll profit = k * dist - c * depth[v];
            if (profit > ans) ans = profit;
        }
        cout << ans << '\n';
    }
    return 0;
}
