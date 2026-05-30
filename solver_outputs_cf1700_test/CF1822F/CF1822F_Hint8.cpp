// Hint8
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
    
    // dp1[v] = max distance from v to a leaf in its subtree (when rooted at 0)
    vector<ll> dp1(n, 0);
    function<void(int, int)> dfs1 = [&](int u, int p) {
        for (int v : adj[u]) {
            if (v == p) continue;
            dfs1(v, u);
            dp1[u] = max(dp1[u], dp1[v] + k);
        }
    };
    dfs1(0, -1);
    
    // dp2[v] = max distance from v to any leaf in the whole tree
    vector<ll> dp2(n, 0);
    function<void(int, int, ll)> dfs2 = [&](int u, int p, ll up_max) {
        dp2[u] = max(dp1[u], up_max);
        // find two largest dp1[child] + k
        ll mx1 = -1, mx2 = -1;
        int best = -1;
        for (int v : adj[u]) {
            if (v == p) continue;
            ll val = dp1[v] + k;
            if (val > mx1) {
                mx2 = mx1;
                mx1 = val;
                best = v;
            } else if (val > mx2) {
                mx2 = val;
            }
        }
        for (int v : adj[u]) {
            if (v == p) continue;
            ll use = (v == best) ? mx2 : mx1;
            ll new_up = max(up_max + k, use + k);
            if (new_up < 0) new_up = 0;
            dfs2(v, u, new_up);
        }
    };
    dfs2(0, -1, 0);
    
    ll ans = 0;
    // distance from original root (0) to node v is depth * k
    vector<ll> depth(n, 0);
    function<void(int, int)> dfs_depth = [&](int u, int p) {
        for (int v : adj[u]) {
            if (v == p) continue;
            depth[v] = depth[u] + 1;
            dfs_depth(v, u);
        }
    };
    dfs_depth(0, -1);
    
    for (int v = 0; v < n; ++v) {
        ll profit = dp2[v] - depth[v] * c;
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
