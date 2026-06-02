#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll INF = 1e18;

void solve() {
    int n, m;
    cin >> n >> m;
    vector<ll> b(n);
    for (int i = 0; i < n; ++i) cin >> b[i];
    
    vector<vector<pair<int, ll>>> adj(n);
    for (int i = 0; i < m; ++i) {
        int u, v; ll w;
        cin >> u >> v >> w;
        --u; --v;
        adj[u].emplace_back(v, w);
    }
    
    vector<ll> dp(n, INF);
    dp[0] = 0;
    
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<>> pq;
    pq.emplace(0, 0);
    
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d != dp[u]) continue;
        
        for (auto &[v, w] : adj[u]) {
            ll need = max(w, d);
            ll take = max(0LL, need - d);
            if (take > b[u]) continue;
            ll nd = d + take;
            if (nd < dp[v]) {
                dp[v] = nd;
                pq.emplace(nd, v);
            }
        }
    }
    
    if (dp[n-1] == INF) {
        cout << "-1\n";
    } else {
        cout << dp[n-1] << "\n";
    }
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
