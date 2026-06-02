#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void solve() {
    int n;
    ll k, c;
    cin >> n >> k >> c;
    vector<vector<int>> adj(n);
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        --u; --v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // First BFS/DFS to find farthest node from 0 (root 1)
    vector<ll> dist0(n, -1);
    queue<int> q;
    dist0[0] = 0;
    q.push(0);
    int far1 = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (dist0[v] == -1) {
                dist0[v] = dist0[u] + 1;
                q.push(v);
                if (dist0[v] > dist0[far1]) far1 = v;
            }
        }
    }

    // Second BFS from far1 to get distances from far1
    vector<ll> dist1(n, -1);
    dist1[far1] = 0;
    q.push(far1);
    int far2 = far1;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (dist1[v] == -1) {
                dist1[v] = dist1[u] + 1;
                q.push(v);
                if (dist1[v] > dist1[far2]) far2 = v;
            }
        }
    }

    // Third BFS from far2 to get distances from far2
    vector<ll> dist2(n, -1);
    dist2[far2] = 0;
    q.push(far2);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (dist2[v] == -1) {
                dist2[v] = dist2[u] + 1;
                q.push(v);
            }
        }
    }

    ll ans = 0;
    for (int i = 0; i < n; ++i) {
        ll max_dist = max(dist1[i], dist2[i]);
        ll profit = max_dist * k - dist0[i] * c;
        ans = max(ans, profit);
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
