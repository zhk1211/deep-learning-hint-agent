#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<vector<tuple<int, long long, long long>>> adj(n + 1);
    for (int i = 2; i <= n; ++i) {
        int p;
        long long a, b;
        cin >> p >> a >> b;
        adj[p].emplace_back(i, a, b);
    }
    
    vector<int> ans(n + 1, 0);
    vector<long long> pref_b;
    pref_b.reserve(n);
    
    function<void(int, long long)> dfs = [&](int u, long long sum_a) {
        for (auto &[v, a, b] : adj[u]) {
            long long new_sum_a = sum_a + a;
            pref_b.push_back(pref_b.empty() ? b : pref_b.back() + b);
            
            // binary search on pref_b to find max length with sum <= new_sum_a
            int lo = 0, hi = (int)pref_b.size() - 1, best = -1;
            while (lo <= hi) {
                int mid = (lo + hi) / 2;
                if (pref_b[mid] <= new_sum_a) {
                    best = mid;
                    lo = mid + 1;
                } else {
                    hi = mid - 1;
                }
            }
            ans[v] = best + 1; // length = index + 1
            
            dfs(v, new_sum_a);
            pref_b.pop_back();
        }
    };
    
    dfs(1, 0);
    
    for (int i = 2; i <= n; ++i) {
        cout << ans[i] << (i == n ? '\n' : ' ');
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
