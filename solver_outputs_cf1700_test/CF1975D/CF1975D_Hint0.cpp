// Hint0
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, a, b;
    cin >> n >> a >> b;
    --a; --b;
    vector<vector<int>> g(n);
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        --u; --v;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    auto bfs = [&](int s) {
        vector<int> dist(n, -1);
        queue<int> q;
        dist[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : g[u]) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    q.push(v);
                }
            }
        }
        return dist;
    };

    auto da = bfs(a);
    auto db = bfs(b);

    // find the vertex on the path from a to b that is closest to a
    // we need the distance from a to b
    int dab = da[b];
    // the meeting point is the vertex at distance ceil(dab/2) from a along the path
    // we can find it by checking all vertices
    int meet = a;
    for (int i = 0; i < n; ++i) {
        if (da[i] + db[i] == dab) { // on the path
            if (da[i] == (dab + 1) / 2) {
                meet = i;
                break;
            }
        }
    }

    // after meeting, we need to traverse the whole tree starting from meet
    // the number of steps is: time to meet + 2*(n-1) - max_depth_from_meet
    auto dmeet = bfs(meet);
    int maxd = *max_element(dmeet.begin(), dmeet.end());
    int steps_to_meet = (dab + 1) / 2;
    long long ans = steps_to_meet + 2LL * (n - 1) - maxd;
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
