#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const ll INF = 1e18;

void solve() {
    int n, m;
    cin >> n >> m;
    vector<ll> b(n);
    for (int i = 0; i < n; ++i) cin >> b[i];
    
    vector<vector<pair<int, ll>>> g(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        ll w;
        cin >> u >> v >> w;
        --u; --v;
        g[u].emplace_back(v, w);
    }
    
    vector<ll> dp(n, INF);
    dp[0] = 0;
    
    using State = pair<ll, int>;
    priority_queue<State, vector<State>, greater<State>> pq;
    pq.emplace(0, 0);
    
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d != dp[u]) continue;
        
        for (auto [v, w] : g[u]) {
            ll need = max(w, dp[u]);
            ll extra = max(0LL, b[u] - need);
            ll new_dp = need + extra;
            if (new_dp < dp[v]) {
                dp[v] = new_dp;
                pq.emplace(dp[v], v);
            }
        }
    }
    
    if (dp[n-1] == INF) cout << "-1\n";
    else cout << dp[n-1] << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) solve();
    
    return 0;
}
