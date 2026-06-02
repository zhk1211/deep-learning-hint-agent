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
        
        // We need to assign parity to rows and columns.
        // Let x_i = 1 if we hire row i, else 0.
        // Let y_j = 1 if we hire column j, else 0.
        // New height: h[i][j] + x_i + y_j.
        // Condition: for adjacent cells, new heights must differ.
        // h[i][j] + x_i + y_j != h[i][j+1] + x_i + y_{j+1}  => h[i][j] - h[i][j+1] != y_{j+1} - y_j
        // Similarly for vertical: h[i][j] - h[i+1][j] != x_{i+1} - x_i
        
        // Since x_i, y_j are 0/1, differences are in {-1,0,1}.
        // So we need to avoid certain differences.
        
        // Let's define for each adjacent pair a forbidden difference.
        // For horizontal: if h[i][j] == h[i][j+1], then we must have y_j != y_{j+1}.
        // If h[i][j] - h[i][j+1] == 1, then we cannot have y_{j+1} - y_j = -1 (i.e., y_j=1, y_{j+1}=0).
        // If h[i][j] - h[i][j+1] == -1, then we cannot have y_{j+1} - y_j = 1 (i.e., y_j=0, y_{j+1}=1).
        // If |diff| >= 2, no restriction.
        
        // Similarly for vertical with x.
        
        // Also, we have cross restrictions? No, x and y are independent except through cost.
        // Wait: the condition only involves x_i and x_{i+1} for vertical, and y_j and y_{j+1} for horizontal.
        // There is no condition linking x and y directly! Because each cell's new height is h + x_i + y_j,
        // and adjacent cells share either same row or same column, so the other variable cancels.
        // So the problem decomposes into independent problems on rows and columns!
        // We just need to choose x and y to satisfy the respective adjacency constraints,
        // and minimize sum a_i x_i + sum b_j y_j.
        
        // For rows: we have a chain of n variables x_0..x_{n-1} with constraints between x_i and x_{i+1}
        // derived from all columns j.
        // For a fixed i and i+1, we look at all j. For each j, we have a constraint based on h[i][j] - h[i+1][j].
        // Let d = h[i][j] - h[i+1][j].
        // We need x_i - x_{i+1} != d. (since new difference = d + x_i - x_{i+1} != 0)
        // So x_i - x_{i+1} cannot equal d.
        // Since x_i, x_{i+1} in {0,1}, x_i - x_{i+1} is in {-1,0,1}.
        // So if d == 0, we cannot have x_i == x_{i+1} (difference 0).
        // If d == 1, we cannot have x_i=1, x_{i+1}=0 (difference 1).
        // If d == -1, we cannot have x_i=0, x_{i+1}=1 (difference -1).
        // If |d| >= 2, no restriction.
        
        // But wait: there might be multiple j giving different d for the same pair (i,i+1).
        // If any j gives a restriction, it applies. If multiple restrictions conflict, it might be impossible.
        // For a given pair (i,i+1), we can have multiple forbidden assignments for (x_i, x_{i+1}).
        // We need to find if there exists an assignment to all x_i satisfying all pairwise constraints.
        // This is a 2-SAT problem on a path graph? Actually variables are boolean, constraints are on adjacent pairs.
        // We can solve by dynamic programming on the chain.
        
        // Similarly for columns.
        
        // Then total cost = min cost for rows + min cost for columns.
        // If either is impossible, overall impossible.
        
        // Let's implement DP for rows.
        // dp[i][v] = min cost for first i+1 variables with x_i = v.
        // Initialize dp[0][0] = 0, dp[0][1] = a[0].
        // For i from 0 to n-2:
        //   for each v in {0,1}:
        //     for each u in {0,1}:
        //       if (v,u) is allowed by constraints between i and i+1:
        //          dp[i+1][u] = min(dp[i+1][u], dp[i][v] + (u ? a[i+1] : 0))
        
        // Constraints between i and i+1: we need to check all j.
        // We can precompute a 2x2 bool array allowed[i][v][u] for each adjacent pair.
        
        auto solve_chain = [&](const vector<vector<long long>>& grid, const vector<long long>& cost, bool is_row) -> long long {
            // grid is n x n. If is_row, we compare row i and i+1.
            // If !is_row, we compare column j and j+1 (transpose).
            int N = n;
            vector<vector<bool>> allowed(N-1, vector<bool>(4, true)); // 4 combinations: v*2+u
            for (int i = 0; i < N-1; ++i) {
                for (int j = 0; j < N; ++j) {
                    long long d;
                    if (is_row) {
                        d = grid[i][j] - grid[i+1][j];
                    } else {
                        d = grid[j][i] - grid[j][i+1];
                    }
                    if (d == 0) {
                        // forbid v == u
                        allowed[i][0] = false; // 0,0
                        allowed[i][3] = false; // 1,1
                    } else if (d == 1) {
                        // forbid v=1, u=0
                        allowed[i][2] = false; // 1,0
                    } else if (d == -1) {
                        // forbid v=0, u=1
                        allowed[i][1] = false; // 0,1
                    }
                    // else no restriction
                }
            }
            vector<long long> dp(2, INF);
            dp[0] = 0;
            dp[1] = cost[0];
            for (int i = 0; i < N-1; ++i) {
                vector<long long> ndp(2, INF);
                for (int v = 0; v < 2; ++v) {
                    if (dp[v] == INF) continue;
                    for (int u = 0; u < 2; ++u) {
                        if (allowed[i][v*2 + u]) {
                            long long add = (u ? cost[i+1] : 0);
                            ndp[u] = min(ndp[u], dp[v] + add);
                        }
                    }
                }
                dp = move(ndp);
            }
            long long ans = min(dp[0], dp[1]);
            return ans;
        };
        
        long long row_cost = solve_chain(h, a, true);
        long long col_cost = solve_chain(h, b, false);
        
        if (row_cost >= INF/2 || col_cost >= INF/2) {
            cout << "-1\n";
        } else {
            cout << row_cost + col_cost << "\n";
        }
    }
    return 0;
}
