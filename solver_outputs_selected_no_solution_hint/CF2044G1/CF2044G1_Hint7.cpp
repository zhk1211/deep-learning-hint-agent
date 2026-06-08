#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<int> r(n);
        for (int i = 0; i < n; ++i) {
            cin >> r[i];
            --r[i];
        }

        vector<int> indeg(n, 0);
        for (int i = 0; i < n; ++i) {
            indeg[r[i]]++;
        }

        queue<int> q;
        for (int i = 0; i < n; ++i) {
            if (indeg[i] == 0) q.push(i);
        }

        vector<int> topo;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            topo.push_back(u);
            int v = r[u];
            if (--indeg[v] == 0) q.push(v);
        }

        vector<bool> in_cycle(n, false);
        for (int i = 0; i < n; ++i) {
            if (indeg[i] > 0) in_cycle[i] = true;
        }

        vector<int> depth(n, 0);
        for (int i = (int)topo.size() - 1; i >= 0; --i) {
            int u = topo[i];
            int v = r[u];
            depth[u] = depth[v] + 1;
        }

        int ans = 2;
        for (int i = 0; i < n; ++i) {
            if (!in_cycle[i]) {
                ans = max(ans, depth[i] + 2);
            }
        }

        vector<bool> visited(n, false);
        for (int i = 0; i < n; ++i) {
            if (in_cycle[i] && !visited[i]) {
                int cur = i;
                int len = 0;
                while (!visited[cur]) {
                    visited[cur] = true;
                    cur = r[cur];
                    len++;
                }
                ans = max(ans, len + 1);
            }
        }

        cout << ans << '\n';
    }
    return 0;
}
