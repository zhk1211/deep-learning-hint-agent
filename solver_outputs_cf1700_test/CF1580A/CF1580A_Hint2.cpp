// Hint2
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
        vector<string> grid(n);
        for (int i = 0; i < n; i++) {
            cin >> grid[i];
        }
        
        vector<vector<int>> pref(n + 1, vector<int>(m + 1, 0));
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                pref[i][j] = pref[i-1][j] + pref[i][j-1] - pref[i-1][j-1] + (grid[i-1][j-1] == '1');
            }
        }
        
        auto get_sum = [&](int r1, int c1, int r2, int c2) {
            return pref[r2][c2] - pref[r1-1][c2] - pref[r2][c1-1] + pref[r1-1][c1-1];
        };
        
        int ans = INF;
        
        for (int r1 = 1; r1 <= n; r1++) {
            for (int r2 = r1 + 4; r2 <= n; r2++) {
                vector<int> best(m + 1, INF);
                for (int c = 4; c <= m; c++) {
                    int top_bottom = (c - 2) - get_sum(r1, 2, r1, c-1) + (c - 2) - get_sum(r2, 2, r2, c-1);
                    int middle = get_sum(r1+1, 2, r2-1, c-1);
                    int left = (r2 - r1 - 1) - get_sum(r1+1, 1, r2-1, 1);
                    int right = (r2 - r1 - 1) - get_sum(r1+1, c, r2-1, c);
                    int corners = (grid[r1-1][0] == '0') + (grid[r1-1][c-1] == '0') + (grid[r2-1][0] == '0') + (grid[r2-1][c-1] == '0');
                    
                    int current = top_bottom + middle + left + right + corners;
                    
                    if (c >= 5) {
                        int prev_left = (r2 - r1 - 1) - get_sum(r1+1, c-3, r2-1, c-3);
                        int prev_top_bottom = 2 - get_sum(r1, c-2, r1, c-2) + 2 - get_sum(r2, c-2, r2, c-2);
                        int prev_middle = get_sum(r1+1, c-2, r2-1, c-2);
                        int prev_corners = (grid[r1-1][c-3] == '0') + (grid[r1-1][c-2] == '0') + (grid[r2-1][c-3] == '0') + (grid[r2-1][c-2] == '0');
                        int prev_cost = prev_top_bottom + prev_middle + prev_left + prev_corners;
                        best[c-3] = min(best[c-3], prev_cost);
                    }
                    
                    if (c >= 4) {
                        ans = min(ans, current + best[c-3]);
                    }
                }
            }
        }
        
        cout << ans << "\n";
    }
    
    return 0;
}
