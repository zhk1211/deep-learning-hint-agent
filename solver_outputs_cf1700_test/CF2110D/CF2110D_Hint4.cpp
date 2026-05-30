// Hint4
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

struct Edge {
    int to;
    ll w;
};

void solve() {
    int n, m;
    cin >> n >> m;
    vector<ll> b(n);
    for (int i = 0; i < n; ++i) cin >> b[i];
    vector<vector<Edge>> adj(n);
    for (int i = 0; i < m; ++i) {
        int s, t; ll w;
        cin >> s >> t >> w;
        --s; --t;
        adj[s].push_back({t, w});
    }
    
    // Check reachability first
    vector<bool> reachable(n, false);
    reachable[0] = true;
    for (int u = 0; u < n; ++u) {
        if (!reachable[u]) continue;
        for (auto &e : adj[u]) {
            reachable[e.to] = true;
        }
    }
    if (!reachable[n-1]) {
        cout << "-1\n";
        return;
    }
    
    // Binary search on answer
    ll low = 0, high = 1e18, ans = -1;
    while (low <= high) {
        ll mid = (low + high) / 2;
        
        // DP: max batteries we can have at each node
        vector<ll> dp(n, -1);
        dp[0] = mid;
        bool possible = false;
        
        for (int u = 0; u < n; ++u) {
            if (dp[u] < 0) continue;
            // We can take batteries at u
            ll cur = dp[u] + b[u];
            // Try to traverse edges
            for (auto &e : adj[u]) {
                if (cur >= e.w) {
                    dp[e.to] = max(dp[e.to], cur);
                }
            }
            if (u == n-1 && dp[u] >= 0) {
                possible = true;
                break;
            }
        }
        
        if (possible) {
            ans = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    
    cout << ans << "\n";
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
