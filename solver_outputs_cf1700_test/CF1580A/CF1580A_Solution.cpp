#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

void solve() {
    int n, m;
    cin >> n >> m;
    vector<string> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }
    
    vector<vector<int>> pref(n + 1, vector<int>(m + 1, 0));
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            pref[i][j] = pref[i-1][j] + pref[i][j-1] - pref[i-1][j-1] + (a[i-1][j-1] == '1');
        }
    }
    
    auto get_sum = [&](int r1, int c1, int r2, int c2) {
        return pref[r2][c2] - pref[r1-1][c2] - pref[r2][c1-1] + pref[r1-1][c1-1];
    };
    
    int ans = INF;
    
    for (int r1 = 1; r1 <= n; ++r1) {
        for (int r2 = r1 + 4; r2 <= n; ++r2) {
            vector<int> best(m + 1, INF);
            for (int c = 4; c <= m; ++c) {
                int inner_ones = get_sum(r1 + 1, 2, r2 - 1, c - 1);
                int top_bottom = (c - 2) * 2 - get_sum(r1, 2, r1, c - 1) - get_sum(r2, 2, r2, c - 1);
                int left = (r2 - r1 - 1) - get_sum(r1 + 1, 1, r2 - 1, 1);
                int right = (r2 - r1 - 1) - get_sum(r1 + 1, c, r2 - 1, c);
                int corners = (a[r1-1][0] == '0') + (a[r2-1][0] == '0') + (a[r1-1][c-1] == '0') + (a[r2-1][c-1] == '0');
                
                int cur = inner_ones + top_bottom + left + right + corners;
                
                if (c >= 5) {
                    int prev_best = best[c - 1];
                    int prev_inner = get_sum(r1 + 1, 2, r2 - 1, c - 2);
                    int prev_top_bottom = (c - 3) * 2 - get_sum(r1, 2, r1, c - 2) - get_sum(r2, 2, r2, c - 2);
                    int prev_left = (r2 - r1 - 1) - get_sum(r1 + 1, 1, r2 - 1, 1);
                    int prev_right = (r2 - r1 - 1) - get_sum(r1 + 1, c - 1, r2 - 1, c - 1);
                    int prev_corners = (a[r1-1][0] == '0') + (a[r2-1][0] == '0') + (a[r1-1][c-2] == '0') + (a[r2-1][c-2] == '0');
                    int prev_val = prev_inner + prev_top_bottom + prev_left + prev_right + prev_corners;
                    best[c - 1] = min(best[c - 1], prev_val);
                }
                
                if (c >= 5) {
                    int add_right = (r2 - r1 - 1) - get_sum(r1 + 1, c, r2 - 1, c);
                    int add_top_bottom = 2 - get_sum(r1, c - 1, r1, c - 1) - get_sum(r2, c - 1, r2, c - 1);
                    int add_corners = (a[r1-1][c-1] == '0') + (a[r2-1][c-1] == '0');
                    int add_inner = get_sum(r1 + 1, c - 1, r2 - 1, c - 1);
                    int prev_total = best[c - 1] - add_inner - add_top_bottom - add_right - add_corners;
                    ans = min(ans, prev_total + cur);
                }
            }
        }
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
