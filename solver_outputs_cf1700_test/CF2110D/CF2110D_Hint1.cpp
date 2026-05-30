// Hint1
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll INF = 1e18;

void solve() {
    int n, m;
    cin >> n >> m;
    vector<ll> b(n);
    for (int i = 0; i < n; ++i) cin >> b[i];
    vector<vector<pair<int,ll>>> g(n);
    for (int i = 0; i < m; ++i) {
        int u, v; ll w;
        cin >> u >> v >> w;
        --u; --v;
        g[u].push_back({v, w});
    }
    
    vector<ll> dp(n, INF);
    dp[0] = 0;
    priority_queue<pair<ll,int>, vector<pair<ll,int>>, greater<>> pq;
    pq.push({0, 0});
    
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d != dp[u]) continue;
        for (auto &[v, w] : g[u]) {
            ll need = max(w, dp[u]);
            if (need <= b[u] + dp[u]) {
                ll nd = need;
                if (nd < dp[v]) {
                    dp[v] = nd;
                    pq.push({nd, v});
                }
            }
        }
    }
    
    if (dp[n-1] == INF) {
        cout << "-1\n";
        return;
    }
    
    vector<int> order(n);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int i, int j) { return dp[i] < dp[j]; });
    
    vector<ll> best = dp;
    for (int i : order) {
        if (dp[i] == INF) continue;
        for (auto &[v, w] : g[i]) {
            ll need = max(w, best[i]);
            if (need <= b[i] + best[i]) {
                best[v] = min(best[v], need);
            }
        }
    }
    
    cout << best[n-1] << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) solve();
    return 0;
}
