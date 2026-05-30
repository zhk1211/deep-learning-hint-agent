// Solution
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int t;
    cin >> t;
    while (t--) {
        int n, a, b;
        cin >> n >> a >> b;
        vector<vector<pair<int, int>>> adj(n + 1);
        for (int i = 0; i < n - 1; ++i) {
            int u, v, w;
            cin >> u >> v >> w;
            adj[u].emplace_back(v, w);
            adj[v].emplace_back(u, w);
        }

        vector<int> xor_a(n + 1, -1);
        queue<int> q;
        xor_a[a] = 0;
        q.push(a);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto &[v, w] : adj[u]) {
                if (v == b) continue;            // do not pass through b
                if (xor_a[v] == -1) {
                    xor_a[v] = xor_a[u] ^ w;
                    q.push(v);
                }
            }
        }

        vector<int> xor_b(n + 1, -1);
        xor_b[b] = 0;
        q.push(b);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto &[v, w] : adj[u]) {
                if (xor_b[v] == -1) {
                    xor_b[v] = xor_b[u] ^ w;
                    q.push(v);
                }
            }
        }

        set<int> vals_a;
        for (int i = 1; i <= n; ++i)
            if (xor_a[i] != -1)
                vals_a.insert(xor_a[i]);

        bool ok = false;
        for (int i = 1; i <= n; ++i) {
            if (i != b && xor_b[i] != -1 && vals_a.count(xor_b[i])) {
                ok = true;
                break;
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
