// Hint0
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<int> p(n + 1);
        for (int i = 2; i <= n; ++i) {
            cin >> p[i];
        }
        vector<ll> l(n + 1), r(n + 1);
        for (int i = 1; i <= n; ++i) {
            cin >> l[i] >> r[i];
        }

        vector<vector<int>> children(n + 1);
        for (int i = 2; i <= n; ++i) {
            children[p[i]].push_back(i);
        }

        int ans = 0;
        vector<ll> max_sum(n + 1, 0);

        function<void(int)> dfs = [&](int u) {
            ll sum = 0;
            for (int v : children[u]) {
                dfs(v);
                sum += max_sum[v];
            }
            if (sum < l[u]) {
                ++ans;
                max_sum[u] = r[u];
            } else {
                max_sum[u] = min(sum, r[u]);
            }
        };

        dfs(1);
        cout << ans << '\n';
    }
    return 0;
}
