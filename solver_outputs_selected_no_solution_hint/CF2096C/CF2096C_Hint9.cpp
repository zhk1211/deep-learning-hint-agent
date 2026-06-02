#include <bits/stdc++.h>
using namespace std;

const long long INF = 1e18;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<vector<long long>> h(n, vector<long long>(n));
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                cin >> h[i][j];
        
        vector<long long> a(n), b(n);
        for (int i = 0; i < n; ++i) cin >> a[i];
        for (int j = 0; j < n; ++j) cin >> b[j];
        
        // Check if already beautiful
        bool beautiful = true;
        for (int i = 0; i < n && beautiful; ++i) {
            for (int j = 0; j < n - 1; ++j) {
                if (h[i][j] == h[i][j+1]) {
                    beautiful = false;
                    break;
                }
            }
        }
        for (int i = 0; i < n - 1 && beautiful; ++i) {
            for (int j = 0; j < n; ++j) {
                if (h[i][j] == h[i+1][j]) {
                    beautiful = false;
                    break;
                }
            }
        }
        if (beautiful) {
            cout << "0\n";
            continue;
        }
        
        // We need exactly n workers: some rows, some columns
        // Let x = number of row workers, y = number of column workers, x+y = n
        // For each cell (i,j), final height = h[i][j] + r_i + c_j
        // where r_i = 1 if row i chosen else 0, c_j = 1 if column j chosen else 0
        // Condition: for adjacent cells, heights differ.
        // Equivalent to: for any adjacent (i,j) and (i,j+1): h[i][j] + r_i + c_j != h[i][j+1] + r_i + c_{j+1}
        // => h[i][j] + c_j != h[i][j+1] + c_{j+1}
        // Similarly vertical: h[i][j] + r_i != h[i+1][j] + r_{i+1}
        // So row choices only affect vertical adjacencies, column choices only affect horizontal adjacencies.
        // We can choose row set R and column set C independently, with |R| + |C| = n.
        // For each possible k = |R| from 0 to n, we pick cheapest k rows and cheapest n-k columns.
        // But we must ensure that the chosen sets actually make the city beautiful.
        // Check: For horizontal: for each row i, for each j, h[i][j] + c_j != h[i][j+1] + c_{j+1}
        // This depends only on column choices C.
        // For vertical: for each column j, for each i, h[i][j] + r_i != h[i+1][j] + r_{i+1}
        // This depends only on row choices R.
        // So we need to find if there exists a set R of size k and a set C of size n-k such that
        // the row set fixes all vertical conflicts, and the column set fixes all horizontal conflicts.
        // But note: the condition must hold for ALL adjacent pairs, not just those involving chosen rows/columns.
        // Actually, if we choose a row, it adds 1 to that entire row, affecting vertical comparisons with both neighbors.
        // The condition for vertical (i,j)-(i+1,j): h[i][j] + r_i != h[i+1][j] + r_{i+1}
        // This is equivalent to: h[i][j] - h[i+1][j] != r_{i+1} - r_i.
        // Since r_i, r_{i+1} are in {0,1}, the difference can be -1, 0, 1.
        // So for each vertical edge, we have a constraint on (r_i, r_{i+1}).
        // Similarly for horizontal edges: h[i][j] - h[i][j+1] != c_{j+1} - c_j.
        // This is a 2-SAT like problem? But n <= 1000, sum n <= 1000, so we can try all k.
        // However, we need to assign exactly k ones to rows and n-k ones to columns.
        // Let's analyze the constraints more carefully.
        // For a vertical edge between row i and i+1 at column j:
        // Let d = h[i][j] - h[i+1][j].
        // We need d != r_{i+1} - r_i.
        // Possible (r_i, r_{i+1}) pairs: (0,0) diff 0; (0,1) diff 1; (1,0) diff -1; (1,1) diff 0.
        // So if d == 0, we cannot have r_i = r_{i+1}. Thus r_i != r_{i+1}.
        // If d == 1, we cannot have r_{i+1} - r_i = 1 => cannot have (r_i=0, r_{i+1}=1). So allowed: (0,0), (1,0), (1,1).
        // If d == -1, we cannot have r_{i+1} - r_i = -1 => cannot have (r_i=1, r_{i+1}=0). Allowed: (0,0), (0,1), (1,1).
        // If |d| > 1, then no restriction because difference can't be that.
        // So each vertical edge gives a constraint on adjacent row variables.
        // Similarly for horizontal edges on column variables.
        // We need to choose exactly k rows to be 1, and exactly n-k columns to be 1.
        // This is like: we have a graph on rows (a path) with constraints, and we need to select exactly k vertices to be 1.
        // Since it's a path, we can do DP to check if there exists an assignment with exactly k ones satisfying all edge constraints.
        // Same for columns.
        // Then we can compute min cost for each possible k by taking cheapest k rows and cheapest n-k columns that are valid.
        // But wait: the validity of row assignment depends only on the pattern of 0/1, not on which specific rows are chosen? 
        // Actually, the constraints are only between adjacent rows. So the validity depends on the sequence of bits.
        // We need to find if there exists a binary sequence of length n with exactly k ones that satisfies all vertical edge constraints.
        // And we want to minimize sum of a_i for rows with 1. So we need to find the minimum cost to choose exactly k rows such that the induced binary sequence is valid.
        // This is a DP on the path: dp[i][j][last] = min cost for first i rows, with j ones, and last row state = last (0 or 1).
        // But we also have to consider that the constraints are between row i and i+1, which depend on column j? Wait, the vertical constraint d = h[i][j] - h[i+1][j] depends on j!
        // For a fixed pair of rows (i,i+1), we have constraints for each column j. The condition must hold for ALL columns j simultaneously.
        // That means for a given pair (r_i, r_{i+1}), we need that for EVERY column j, h[i][j] - h[i+1][j] != r_{i+1} - r_i.
        // So if there exists any column j where the difference equals the forbidden value, then that (r_i, r_{i+1}) pair is invalid.
        // So we can precompute for each adjacent row pair which of the 4 possible state pairs are allowed.
        // Similarly for columns.
        // Then we have independent problems on rows and columns: find if there is a binary sequence with exactly k ones satisfying allowed transitions, and if so, min cost.
        // Since n <= 1000, we can do DP for each k from 0 to n, but that would be O(n^2) per test case, which is fine (sum n <= 1000, so n^2 <= 1e6 total).
        // However, we need to do this for both rows and columns, and then combine.
        // Let's define DP for rows:
        // allowed_row[i][s1][s2] = true if (r_i = s1, r_{i+1} = s2) is allowed.
        // We can compute this by checking all columns j: for each column, compute d = h[i][j] - h[i+1][j].
        // If d == 0 -> s1 != s2 required. So if s1 == s2, invalid.
        // If d == 1 -> cannot have s1=0, s2=1.
        // If d == -1 -> cannot have s1=1, s2=0.
        // If |d| > 1 -> always allowed.
        // If for any column the condition is violated for a given (s1,s2), then that transition is forbidden.
        // Then DP: dp[i][j][s] = min cost to assign first i rows (1-indexed), with j ones, ending with row i state s.
        // Initialize dp[1][1][1] = a[0], dp[1][0][0] = 0.
        // For i from 1 to n-1, update dp[i+1].
        // At the end, min cost for exactly k ones is min(dp[n][k][0], dp[n][k][1]).
        // If both INF, then impossible for that k.
        // Similarly for columns with costs b.
        // Then answer = min over k=0..n of (row_cost[k] + col_cost[n-k]).
        // If all INF, output -1.
        
        // Precompute allowed transitions for rows
        vector<vector<bool>> row_ok(n-1, vector<bool>(4, true)); // 0:00, 1:01, 2:10, 3:11
        for (int i = 0; i < n-1; ++i) {
            for (int j = 0; j < n; ++j) {
                long long d = h[i][j] - h[i+1][j];
                if (d == 0) {
                    row_ok[i][0] = false; // 00
                    row_ok[i][3] = false; // 11
                } else if (d == 1) {
                    row_ok[i][1] = false; // 01
                } else if (d == -1) {
                    row_ok[i][2] = false; // 10
                }
                // else |d|>1: all allowed
            }
        }
        
        // DP for rows
        vector<vector<long long>> dp_row(n+1, vector<long long>(n+1, INF));
        // dp[i][j][s] -> we can compress: dp[i][j][0] and dp[i][j][1]
        // We'll use two 2D arrays: dp0[i][j], dp1[i][j]
        vector<vector<long long>> dp0(n+1, vector<long long>(n+1, INF));
        vector<vector<long long>> dp1(n+1, vector<long long>(n+1, INF));
        dp0[1][0] = 0;
        dp1[1][1] = a[0];
        for (int i = 1; i < n; ++i) {
            for (int j = 0; j <= i; ++j) {
                if (dp0[i][j] < INF) {
                    // transition to 0
                    if (row_ok[i-1][0]) { // 00
                        dp0[i+1][j] = min(dp0[i+1][j], dp0[i][j]);
                    }
                    // transition to 1
                    if (row_ok[i-1][1]) { // 01
                        dp1[i+1][j+1] = min(dp1[i+1][j+1], dp0[i][j] + a[i]);
                    }
                }
                if (dp1[i][j] < INF) {
                    // transition to 0
                    if (row_ok[i-1][2]) { // 10
                        dp0[i+1][j] = min(dp0[i+1][j], dp1[i][j]);
                    }
                    // transition to 1
                    if (row_ok[i-1][3]) { // 11
                        dp1[i+1][j+1] = min(dp1[i+1][j+1], dp1[i][j] + a[i]);
                    }
                }
            }
        }
        vector<long long> row_cost(n+1, INF);
        for (int k = 0; k <= n; ++k) {
            row_cost[k] = min(dp0[n][k], dp1[n][k]);
        }
        
        // Precompute allowed transitions for columns
        vector<vector<bool>> col_ok(n-1, vector<bool>(4, true));
        for (int j = 0; j < n-1; ++j) {
            for (int i = 0; i < n; ++i) {
                long long d = h[i][j] - h[i][j+1];
                if (d == 0) {
                    col_ok[j][0] = false;
                    col_ok[j][3] = false;
                } else if (d == 1) {
                    col_ok[j][1] = false;
                } else if (d == -1) {
                    col_ok[j][2] = false;
                }
            }
        }
        
        // DP for columns
        vector<vector<long long>> dp0_col(n+1, vector<long long>(n+1, INF));
        vector<vector<long long>> dp1_col(n+1, vector<long long>(n+1, INF));
        dp0_col[1][0] = 0;
        dp1_col[1][1] = b[0];
        for (int j = 1; j < n; ++j) {
            for (int k = 0; k <= j; ++k) {
                if (dp0_col[j][k] < INF) {
                    if (col_ok[j-1][0]) {
                        dp0_col[j+1][k] = min(dp0_col[j+1][k], dp0_col[j][k]);
                    }
                    if (col_ok[j-1][1]) {
                        dp1_col[j+1][k+1] = min(dp1_col[j+1][k+1], dp0_col[j][k] + b[j]);
                    }
                }
                if (dp1_col[j][k] < INF) {
                    if (col_ok[j-1][2]) {
                        dp0_col[j+1][k] = min(dp0_col[j+1][k], dp1_col[j][k]);
                    }
                    if (col_ok[j-1][3]) {
                        dp1_col[j+1][k+1] = min(dp1_col[j+1][k+1], dp1_col[j][k] + b[j]);
                    }
                }
            }
        }
        vector<long long> col_cost(n+1, INF);
        for (int k = 0; k <= n; ++k) {
            col_cost[k] = min(dp0_col[n][k], dp1_col[n][k]);
        }
        
        long long ans = INF;
        for (int k = 0; k <= n; ++k) {
            if (row_cost[k] < INF && col_cost[n-k] < INF) {
                ans = min(ans, row_cost[k] + col_cost[n-k]);
            }
        }
        if (ans == INF) cout << "-1\n";
        else cout << ans << "\n";
    }
    return 0;
}
