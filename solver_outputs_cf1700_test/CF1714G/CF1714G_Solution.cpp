#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<vector<tuple<int, long long, long long>>> g(n + 1);
    for (int j = 2; j <= n; ++j) {
        int p;
        long long a, b;
        cin >> p >> a >> b;
        g[p].emplace_back(j, a, b);
    }
    vector<int> ans(n + 1);
    vector<long long> pref_b;
    pref_b.reserve(n);
    function<void(int, long long)> dfs = [&](int u, long long sum_a) {
        for (auto [v, a, b] : g[u]) {
            long long new_sum_a = sum_a + a;
            pref_b.push_back(pref_b.empty() ? b : pref_b.back() + b);
            int lo = 0, hi = (int)pref_b.size() - 1;
            int best = -1;
            while (lo <= hi) {
                int mid = (lo + hi) / 2;
                if (pref_b[mid] <= new_sum_a) {
                    best = mid;
                    lo = mid + 1;
                } else {
                    hi = mid - 1;
                }
            }
            ans[v] = best + 1;
            dfs(v, new_sum_a);
            pref_b.pop_back();
        }
    };
    dfs(1, 0);
    for (int i = 2; i <= n; ++i) {
        cout << ans[i] << " \n"[i == n];
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
