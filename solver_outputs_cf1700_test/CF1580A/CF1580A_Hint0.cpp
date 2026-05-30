// Hint0
#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n, m;
        cin >> n >> m;
        vector<string> a(n);
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }
        
        vector<vector<int>> pref(n + 1, vector<int>(m + 1, 0));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                pref[i + 1][j + 1] = pref[i][j + 1] + pref[i + 1][j] - pref[i][j] + (a[i][j] == '1');
            }
        }
        
        auto get_sum = [&](int r1, int c1, int r2, int c2) {
            return pref[r2 + 1][c2 + 1] - pref[r1][c2 + 1] - pref[r2 + 1][c1] + pref[r1][c1];
        };
        
        int ans = INF;
        
        for (int r1 = 0; r1 < n; r1++) {
            for (int r2 = r1 + 4; r2 < n; r2++) {
                vector<int> cost(m, 0);
                for (int c = 0; c < m; c++) {
                    int top_bottom = (a[r1][c] == '0') + (a[r2][c] == '0');
                    int middle = get_sum(r1 + 1, c, r2 - 1, c);
                    cost[c] = top_bottom + middle;
                }
                
                vector<int> pref_cost(m + 1, 0);
                for (int c = 0; c < m; c++) {
                    pref_cost[c + 1] = pref_cost[c] + cost[c];
                }
                
                int best = INF;
                for (int c2 = 3; c2 < m; c2++) {
                    int c1 = c2 - 3;
                    int cur = pref_cost[c2] - pref_cost[c1 + 1];
                    int left_col = (a[r1][c1] == '0') + (a[r2][c1] == '0') + get_sum(r1 + 1, c1, r2 - 1, c1);
                    int right_col = (a[r1][c2] == '0') + (a[r2][c2] == '0') + get_sum(r1 + 1, c2, r2 - 1, c2);
                    int middle_empty = (r2 - r1 - 1) * 2 - get_sum(r1 + 1, c1 + 1, r2 - 1, c2 - 1);
                    int total = cur + left_col + right_col + middle_empty;
                    best = min(best, total);
                    
                    if (c2 >= 4) {
                        int prev_c1 = c2 - 4;
                        int prev_cur = pref_cost[c2 - 1] - pref_cost[prev_c1 + 1];
                        int prev_left = (a[r1][prev_c1] == '0') + (a[r2][prev_c1] == '0') + get_sum(r1 + 1, prev_c1, r2 - 1, prev_c1);
                        int prev_right = (a[r1][c2 - 1] == '0') + (a[r2][c2 - 1] == '0') + get_sum(r1 + 1, c2 - 1, r2 - 1, c2 - 1);
                        int prev_mid_empty = (r2 - r1 - 1) * 2 - get_sum(r1 + 1, prev_c1 + 1, r2 - 1, c2 - 2);
                        int prev_total = prev_cur + prev_left + prev_right + prev_mid_empty;
                        best = min(best, prev_total);
                    }
                }
                ans = min(ans, best);
            }
        }
        
        cout << ans << '\n';
    }
    
    return 0;
}
