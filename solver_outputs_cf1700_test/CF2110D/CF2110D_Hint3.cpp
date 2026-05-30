// Hint3
#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const ll INF = 1e18;

void solve() {
    int n, m;
    cin >> n >> m;
    vector<ll> b(n);
    for (int i = 0; i < n; ++i) cin >> b[i];
    
    vector<vector<pair<int,ll>>> adj(n);
    for (int i = 0; i < m; ++i) {
        int u, v; ll w;
        cin >> u >> v >> w;
        --u; --v;
        adj[u].emplace_back(v, w);
    }
    
    // Check if n is reachable from 0
    vector<bool> vis(n, false);
    queue<int> q;
    q.push(0);
    vis[0] = true;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (auto &[v, w] : adj[u]) {
            if (!vis[v]) {
                vis[v] = true;
                q.push(v);
            }
        }
    }
    if (!vis[n-1]) {
        cout << "-1\n";
        return;
    }
    
    // Binary search on answer
    ll lo = 0, hi = 0;
    for (int i = 0; i < n; ++i) hi += b[i];
    ll ans = hi;
    
    auto can = [&](ll start) -> bool {
        // dp[v] = max batteries when arriving at v (before taking b[v])
        vector<ll> dp(n, -INF);
        dp[0] = start;
        // Process vertices in increasing order (since edges go from smaller to larger)
        for (int u = 0; u < n; ++u) {
            if (dp[u] < 0) continue;
            // Take batteries at u
            ll cur = dp[u] + b[u];
            for (auto &[v, w] : adj[u]) {
                if (cur >= w) {
                    dp[v] = max(dp[v], cur);
                }
            }
        }
        return dp[n-1] >= 0;
    };
    
    while (lo <= hi) {
        ll mid = (lo + hi) / 2;
        if (can(mid)) {
            ans = mid;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) solve();
    return 0;
}
