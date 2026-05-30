// Hint4
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
                pref[i + 1][j + 1] = pref[i][j + 1] + pref[i + 1][j] - pref[i][j] + (grid[i][j] == '1');
            }
        }
        
        auto ones = [&](int r1, int c1, int r2, int c2) {
            if (r1 > r2 || c1 > c2) return 0;
            return pref[r2 + 1][c2 + 1] - pref[r1][c2 + 1] - pref[r2 + 1][c1] + pref[r1][c1];
        };
        
        int ans = INF;
        
        // Iterate over top and bottom rows of the portal
        for (int top = 0; top < n; ++top) {
            for (int bot = top + 4; bot < n; ++bot) {
                // We'll maintain best value for left side up to current column
                // best[k] = min operations to make a portal ending at column k (as right column)
                // We'll compute on the fly using DP
                vector<int> best(m, INF);
                int cur_best = INF;
                
                // Precompute some column sums for fixed top and bot
                // For each column, cost to make vertical borders (excluding corners)
                vector<int> vert_cost(m, 0);
                for (int j = 0; j < m; ++j) {
                    // cells (top+1..bot-1, j) must be 1
                    int need = (bot - top - 1);
                    int have = ones(top + 1, j, bot - 1, j);
                    vert_cost[j] = need - have;
                }
                
                // For each right column, find best left column
                for (int right = 3; right < m; ++right) {
                    // Cost for right vertical border (excluding corners)
                    int cost_right = vert_cost[right];
                    
                    // Cost for top and bottom horizontal borders between left+1 and right-1
                    // We'll compute for left = right - 3, right - 4, ... and keep minimum
                    // We maintain running sum of horizontal costs
                    
                    // For left = right - 3:
                    int left = right - 3;
                    // Cost for left vertical border
                    int cost_left = vert_cost[left];
                    
                    // Horizontal borders: top row (top, left+1..right-1) and bottom row (bot, left+1..right-1)
                    int hor_top = ones(top, left + 1, top, right - 1);
                    int hor_bot = ones(bot, left + 1, bot, right - 1);
                    int need_hor = 2 * (right - left - 1);
                    int cost_hor = need_hor - (hor_top + hor_bot);
                    
                    // Interior must be 0
                    int interior = ones(top + 1, left + 1, bot - 1, right - 1);
                    
                    // Corners: we don't care, they can be anything, so cost 0 for them
                    int total = cost_left + cost_right + cost_hor + interior;
                    
                    cur_best = min(cur_best, total);
                    best[right] = min(best[right], cur_best);
                    
                    // Now update cur_best for next right by considering extending left?
                    // Actually we need to consider all left < right - 2.
                    // We can maintain running minimum of (cost_left + cost_hor_prefix + interior_prefix)
                    // But interior and horizontal costs depend on left.
                    // Let's do a separate inner loop for left? Since n,m <= 400 and sum n,m <= 400,
                    // O(n^2 * m^2) might be too slow (400^4 = 2.56e10). We need O(n^2 * m) or O(n * m^2).
                    // We can precompute prefix sums of columns to quickly get horizontal and interior costs.
                    // We'll iterate left from right-3 down to 0, maintaining cost.
                }
                
                // Actually the above is incomplete. Let's do a proper O(n^2 * m) approach.
                // For fixed top and bot, we can compute for each column j:
                // cost_top[j] = cost to make (top, j) a 1? No, corners are free.
                // We need to compute minimum operations for portal with right column = j.
                // We can maintain an array min_cost[j] = minimum cost for portal ending at column j.
                
                // Reset for this (top, bot) pair
                vector<int> dp(m, INF);
                
                // We'll compute prefix sums of ones in top and bottom rows between columns
                vector<int> top_ones(m + 1, 0), bot_ones(m + 1, 0);
                for (int j = 0; j < m; ++j) {
                    top_ones[j + 1] = top_ones[j] + (grid[top][j] == '1');
                    bot_ones[j + 1] = bot_ones[j] + (grid[bot][j] == '1');
                }
                
                // Also prefix sums of interior zeros? We need interior ones to subtract.
                // We have pref array for fast query.
                
                // For each right column, we try all left columns with distance at least 3
                // But we can optimize by maintaining best value of (cost_left + cost_hor_left_part + interior_left_part - something)
                // Let's derive formula for fixed left and right:
                // cost = (cost_left_vert) + (cost_right_vert) + (hor_cost between left+1 and right-1) + (interior ones)
                // cost_left_vert = (bot - top - 1) - ones(top+1..bot-1, left)
                // cost_right_vert = (bot - top - 1) - ones(top+1..bot-1, right)
                // hor_cost = 2*(right - left - 1) - (ones(top, left+1..right-1) + ones(bot, left+1..right-1))
                // interior = ones(top+1..bot-1, left+1..right-1)
                
                // Let's define for a column j:
                // A[j] = cost_vert[j] = (bot - top - 1) - ones(top+1..bot-1, j)
                // For a segment [l+1, r-1], we need:
                // B[l][r] = 2*(r-l-1) - (ones(top, l+1..r-1) + ones(bot, l+1..r-1)) + ones(top+1..bot-1, l+1..r-1)
                // Total = A[l] + A[r] + B[l][r]
                
                // We can rewrite B[l][r] using prefix sums:
                // Let P[j] = 2*j - (top_ones[j] + bot_ones[j]) + interior_ones_up_to_j?
                // interior_ones_up_to_j = ones(top+1..bot-1, 0..j-1)
                // Then B[l][r] = (P[r] - P[l+1])? Let's check.
                // For columns x in [l+1, r-1], contribution to B is:
                // 2 - (grid[top][x]=='1') - (grid[bot][x]=='1') + (ones in interior at column x)
                // Let val[x] = 2 - (grid[top][x]=='1') - (grid[bot][x]=='1') + ones(top+1..bot-1, x)
                // Then B[l][r] = sum_{x=l+1}^{r-1} val[x]
                // So if we precompute prefix sums of val, we can get B in O(1).
                
                vector<int> val(m);
                for (int j = 0; j < m; ++j) {
                    int top_bit = (grid[top][j] == '1');
                    int bot_bit = (grid[bot][j] == '1');
                    int interior_col = ones(top + 1, j, bot - 1, j);
                    val[j] = 2 - top_bit - bot_bit + interior_col;
                }
                vector<int> pref_val(m + 1, 0);
                for (int j = 0; j < m; ++j) {
                    pref_val[j + 1] = pref_val[j] + val[j];
                }
                
                // Now total cost for left = l, right = r:
                // cost = A[l] + A[r] + (pref_val[r] - pref_val[l+1])
                // We want minimum over l <= r-3.
                // For each r, we need min_{l <= r-3} (A[l] - pref_val[l+1]) + A[r] + pref_val[r]
                // So we can maintain running minimum of (A[l] - pref_val[l+1]) as we increase r.
                
                int min_left_part = INF;
                for (int r = 3; r < m; ++r) {
                    int l = r - 3;
                    // Update min_left_part with l
                    int left_val = vert_cost[l] - pref_val[l + 1];
                    min_left_part = min(min_left_part, left_val);
                    
                    int total = min_left_part + vert_cost[r] + pref_val[r];
                    ans = min(ans, total);
                }
            }
        }
        
        cout << ans << '\n';
    }
    
    return 0;
}
