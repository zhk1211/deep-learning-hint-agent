#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, k;
    cin >> n >> k;
    vector<int> friends(k);
    for (int i = 0; i < k; ++i) {
        cin >> friends[i];
    }
    vector<vector<int>> g(n + 1);
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    vector<int> dist_v(n + 1, -1);
    queue<int> q;
    dist_v[1] = 0;
    q.push(1);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : g[u]) {
            if (dist_v[v] == -1) {
                dist_v[v] = dist_v[u] + 1;
                q.push(v);
            }
        }
    }
    vector<int> dist_f(n + 1, -1);
    for (int x : friends) {
        dist_f[x] = 0;
        q.push(x);
    }
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : g[u]) {
            if (dist_f[v] == -1) {
                dist_f[v] = dist_f[u] + 1;
                q.push(v);
            }
        }
    }
    bool can_win = false;
    for (int u = 2; u <= n; ++u) {
        if (g[u].size() == 1) {
            if (dist_v[u] < dist_f[u]) {
                can_win = true;
                break;
            }
        }
    }
    cout << (can_win ? "YES" : "NO") << '\n';
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
