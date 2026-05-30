// Solution
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
        vector<vector<int>> adj(n + 1);
        vector<int> indeg(n + 1, 0);
        for (int i = 0; i < k; ++i) {
            vector<int> a(n);
            for (int j = 0; j < n; ++j) cin >> a[j];
            for (int j = 1; j < n - 1; ++j) {
                int u = a[j], v = a[j + 1];
                adj[u].push_back(v);
                indeg[v]++;
            }
        }
        queue<int> q;
        for (int i = 1; i <= n; ++i)
            if (indeg[i] == 0) q.push(i);
        int visited = 0;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            visited++;
            for (int v : adj[u])
                if (--indeg[v] == 0) q.push(v);
        }
        cout << (visited == n ? "YES" : "NO") << '\n';
    }
    return 0;
}
