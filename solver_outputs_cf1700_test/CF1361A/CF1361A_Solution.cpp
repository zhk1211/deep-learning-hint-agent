// Solution
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; ++i) {
        int a, b;
        cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    vector<int> t(n + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> t[i];
    }

    vector<pair<int, int>> order;
    order.reserve(n);
    for (int i = 1; i <= n; ++i) {
        order.emplace_back(t[i], i);
    }
    sort(order.begin(), order.end());

    vector<int> color(n + 1, 0);
    vector<int> seen(n + 2, 0);
    int timer = 0;

    vector<int> ans;
    ans.reserve(n);

    for (auto [topic, u] : order) {
        ++timer;
        for (int v : adj[u]) {
            if (color[v] != 0) {
                seen[color[v]] = timer;
            }
        }
        int mex = 1;
        while (seen[mex] == timer) {
            ++mex;
        }
        if (mex != topic) {
            cout << -1 << '\n';
            return 0;
        }
        color[u] = mex;
        ans.push_back(u);
    }

    for (int i = 0; i < n; ++i) {
        if (i) cout << ' ';
        cout << ans[i];
    }
    cout << '\n';

    return 0;
}
