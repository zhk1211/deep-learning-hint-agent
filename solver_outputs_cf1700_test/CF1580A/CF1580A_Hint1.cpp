// Hint1
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
        for (int i = 0; i < n; ++i) {
            cin >> grid[i];
        }
        
        // Precompute prefix sums for fast counting of 1s in any subrectangle
        vector<vector<int>> pref(n + 1, vector<int>(m + 1, 0));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                pref[i+1][j+1] = pref[i][j+1] + pref[i+1][j] - pref[i][j] + (grid[i][j] == '1');
            }
        }
        auto ones = [&](int r1, int c1, int r2, int c2) {
            return pref[r2+1][c2+1] - pref[r1][c2+1] - pref[r2+1][c1] + pref[r1][c1];
        };
        
        int ans = INF;
        
        // Iterate over top and bottom rows of the portal
        for (int top = 0; top < n; ++top) {
            for (int bot = top + 4; bot < n; ++bot) {
                // We'll maintain best value for left side up to current column
                // best = min over left columns of (cost to make left border + interior zeros needed)
                // We'll slide right column and keep track of best left column.
                int best = INF;
                for (int right = 3; right < m; ++right) {
                    int left = right - 3;
                    // Compute cost for left column as left border
                    // left border: rows (top+1 .. bot-1) must be 1
                    int left_border_cost = (bot - top - 1) - ones(top+1, left, bot-1, left);
                    // interior between left+1 and right-1: rows top+1..bot-1 must be 0
                    int interior_zeros = ones(top+1, left+1, bot-1, right-1);
                    // top and bottom borders between left+1 and right-1 must be 1
                    int top_bottom_cost = 0;
                    for (int c = left+1; c <= right-1; ++c) {
                        if (grid[top][c] == '0') top_bottom_cost++;
                        if (grid[bot][c] == '0') top_bottom_cost++;
                    }
                    // current cost for left part (excluding right border)
                    int cur = left_border_cost + interior_zeros + top_bottom_cost;
                    best = min(best, cur);
                    
                    // Now consider right as the right border of portal
                    // right border cost: rows top+1..bot-1 must be 1
                    int right_border_cost = (bot - top - 1) - ones(top+1, right, bot-1, right);
                    // total cost = best + right_border_cost
                    ans = min(ans, best + right_border_cost);
                }
            }
        }
        cout << ans << '\n';
    }
    return 0;
}
