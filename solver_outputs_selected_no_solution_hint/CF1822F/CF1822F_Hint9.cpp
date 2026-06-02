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

    // First BFS from node 0 to find farthest node A
    vector<ll> dist0(n, -1);
    queue<int> q;
    dist0[0] = 0;
    q.push(0);
    int A = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (dist0[u] > dist0[A]) A = u;
        for (int v : adj[u]) {
            if (dist0[v] == -1) {
                dist0[v] = dist0[u] + k;
                q.push(v);
            }
        }
    }

    // Second BFS from A to find farthest node B and distances from A
    vector<ll> distA(n, -1);
    distA[A] = 0;
    q.push(A);
    int B = A;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (distA[u] > distA[B]) B = u;
        for (int v : adj[u]) {
            if (distA[v] == -1) {
                distA[v] = distA[u] + k;
                q.push(v);
            }
        }
    }

    // Third BFS from B to get distances from B
    vector<ll> distB(n, -1);
    distB[B] = 0;
    q.push(B);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (distB[v] == -1) {
                distB[v] = distB[u] + k;
                q.push(v);
            }
        }
    }

    ll ans = 0;
    for (int i = 0; i < n; ++i) {
        ll maxDist = max(distA[i], distB[i]);
        ll profit = maxDist - c * (dist0[i] / k);
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
