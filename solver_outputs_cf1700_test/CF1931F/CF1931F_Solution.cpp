#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, k;
    cin >> n >> k;
    vector<vector<int>> a(k, vector<int>(n));
    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> a[i][j];
        }
    }
    if (k == 1 || n <= 2) {
        cout << "YES\n";
        return;
    }
    vector<vector<int>> g(n + 1);
    vector<int> indeg(n + 1, 0);
    for (int i = 0; i < k; ++i) {
        for (int j = 1; j < n - 1; ++j) {
            int u = a[i][j];
            int v = a[i][j + 1];
            g[u].push_back(v);
            ++indeg[v];
        }
    }
    queue<int> q;
    for (int i = 1; i <= n; ++i) {
        if (indeg[i] == 0) {
            q.push(i);
        }
    }
    int cnt = 0;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        ++cnt;
        for (int v : g[u]) {
            if (--indeg[v] == 0) {
                q.push(v);
            }
        }
    }
    cout << (cnt == n ? "YES" : "NO") << "\n";
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
