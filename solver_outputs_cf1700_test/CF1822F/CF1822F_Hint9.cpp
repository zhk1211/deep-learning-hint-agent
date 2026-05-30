// Hint9
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void solve() {
    int n;
    ll k, c;
    cin >> n >> k >> c;
    vector<vector<int>> g(n);
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        --u; --v;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    // dp1[v] = max distance from v to a leaf in its subtree (when rooted at 0)
    vector<ll> dp1(n, 0);
    function<void(int, int)> dfs1 = [&](int u, int p) {
        for (int v : g[u]) {
            if (v == p) continue;
            dfs1(v, u);
            dp1[u] = max(dp1[u], dp1[v] + k);
        }
    };
    dfs1(0, -1);

    // dp2[v] = max distance from v to any leaf in the whole tree (rerooted)
    vector<ll> dp2(n, 0);
    function<void(int, int, ll)> dfs2 = [&](int u, int p, ll up_dist) {
        // up_dist is the max distance from u going through parent
        dp2[u] = max(dp1[u], up_dist);
        
        // find two largest dp1[v] + k among children
        ll mx1 = -1, mx2 = -1;
        int mx1_v = -1;
        for (int v : g[u]) {
            if (v == p) continue;
            ll val = dp1[v] + k;
            if (val > mx1) {
                mx2 = mx1;
                mx1 = val;
                mx1_v = v;
            } else if (val > mx2) {
                mx2 = val;
            }
        }
        
        for (int v : g[u]) {
            if (v == p) continue;
            ll best_other = (v == mx1_v) ? mx2 : mx1;
            ll new_up = max(up_dist + k, best_other + k);
            // also consider if up_dist is 0 (root case) but up_dist is passed correctly
            if (new_up < 0) new_up = 0; // no other branches
            dfs2(v, u, new_up);
        }
    };
    dfs2(0, -1, 0);

    ll ans = 0;
    // For each possible root v, profit = dp2[v] - c * depth[v]
    // depth from original root 0
    vector<int> depth(n, 0);
    function<void(int, int)> dfs_depth = [&](int u, int p) {
        for (int v : g[u]) {
            if (v == p) continue;
            depth[v] = depth[u] + 1;
            dfs_depth(v, u);
        }
    };
    dfs_depth(0, -1);
    
    for (int v = 0; v < n; ++v) {
        ans = max(ans, dp2[v] - c * depth[v]);
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
