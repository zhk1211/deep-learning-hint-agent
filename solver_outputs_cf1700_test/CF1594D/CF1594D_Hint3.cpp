// Hint3
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, m;
    cin >> n >> m;
    vector<vector<pair<int, int>>> adj(n + 1);
    for (int k = 0; k < m; ++k) {
        int i, j;
        string c;
        cin >> i >> j >> c;
        int w = (c == "imposter") ? 1 : 0;
        adj[i].push_back({j, w});
        adj[j].push_back({i, w});
    }
    vector<int> color(n + 1, -1);
    int ans = 0;
    for (int i = 1; i <= n; ++i) {
        if (color[i] == -1) {
            vector<int> comp;
            queue<int> q;
            q.push(i);
            color[i] = 0;
            comp.push_back(i);
            bool ok = true;
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                for (auto &[v, w] : adj[u]) {
                    int expected = color[u] ^ w;
                    if (color[v] == -1) {
                        color[v] = expected;
                        comp.push_back(v);
                        q.push(v);
                    } else if (color[v] != expected) {
                        ok = false;
                    }
                }
            }
            if (!ok) {
                cout << -1 << '\n';
                return;
            }
            int cnt0 = 0, cnt1 = 0;
            for (int v : comp) {
                if (color[v] == 0) cnt0++;
                else cnt1++;
            }
            ans += max(cnt0, cnt1);
        }
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
