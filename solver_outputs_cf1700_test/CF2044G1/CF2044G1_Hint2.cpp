// Hint2
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
            if (indeg[i] == 0) {
                q.push(i);
            }
        }

        vector<int> dist(n, 0);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            int v = r[u];
            dist[v] = max(dist[v], dist[u] + 1);
            if (--indeg[v] == 0) {
                q.push(v);
            }
        }

        int ans = 2;
        for (int i = 0; i < n; ++i) {
            if (indeg[i] > 0) {
                ans = max(ans, dist[i] + 2);
            }
        }

        cout << ans << '\n';
    }

    return 0;
}
