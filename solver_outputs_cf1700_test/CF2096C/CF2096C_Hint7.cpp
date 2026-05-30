// Hint7
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
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                cin >> h[i][j];
            }
        }
        vector<long long> a(n), b(n);
        for (int i = 0; i < n; ++i) cin >> a[i];
        for (int j = 0; j < n; ++j) cin >> b[j];
        
        // We need to assign each row and column a parity (0 or 1) of increments.
        // The condition: for adjacent cells (i,j) and (i,j+1):
        // h[i][j] + row[i] + col[j] != h[i][j+1] + row[i] + col[j+1]
        // => col[j] - col[j+1] != h[i][j+1] - h[i][j]
        // Similarly for vertical: row[i] - row[i+1] != h[i+1][j] - h[i][j]
        // Since we only care about parity (mod 2), we can reduce constraints mod 2.
        // Let r[i] = row[i] mod 2, c[j] = col[j] mod 2.
        // Then condition: c[j] xor c[j+1] != (h[i][j+1] - h[i][j]) mod 2
        // and r[i] xor r[i+1] != (h[i+1][j] - h[i][j]) mod 2.
        // This gives constraints on r and c.
        // We can model as 2-SAT or bipartite graph coloring.
        // Variables: r[0..n-1], c[0..n-1].
        // For each adjacent horizontal pair (i,j)-(i,j+1):
        //   diff = (h[i][j+1] - h[i][j]) & 1
        //   if diff == 0: c[j] != c[j+1]  => c[j] xor c[j+1] = 1
        //   else: c[j] == c[j+1]          => c[j] xor c[j+1] = 0
        // For each adjacent vertical pair (i,j)-(i+1,j):
        //   diff = (h[i+1][j] - h[i][j]) & 1
        //   if diff == 0: r[i] != r[i+1]  => r[i] xor r[i+1] = 1
        //   else: r[i] == r[i+1]          => r[i] xor r[i+1] = 0
        // Also, we can choose to flip all r and c simultaneously (since adding 1 to all rows and columns changes parity of all cells by 0 mod 2? Actually adding 1 to all rows and columns adds 2 to each cell, so parity unchanged. But we can also choose to add 1 to all rows? That would change parity of all cells? Wait, if we add 1 to all rows, each cell gets +1, parity flips. But we can also add 1 to all columns, which flips parity again. So overall parity of increments for a cell is r[i] xor c[j]. If we flip all r and c, r[i] xor c[j] remains same. So there is a global symmetry: we can fix one variable arbitrarily.)
        // We can solve with 2-coloring on a graph with nodes 0..2n-1 (first n for r, next n for c).
        // But constraints only within rows and within columns? Actually horizontal constraints involve only c's, vertical only r's. So r and c are independent!
        // So we can solve for r and c separately.
        // For rows: we have constraints between r[i] and r[i+1] based on vertical differences.
        // For columns: constraints between c[j] and c[j+1] based on horizontal differences.
        // However, there is a catch: the constraints must hold for ALL i (for horizontal) and ALL j (for vertical). 
        // For horizontal: for a fixed j, the condition c[j] xor c[j+1] != (h[i][j+1]-h[i][j]) mod 2 must hold for ALL i. 
        // If for some i the diff is 0 and for another i the diff is 1, then we have conflicting requirements: c[j] xor c[j+1] must be both 1 and 0. Impossible.
        // So we first check consistency: for each adjacent column pair (j, j+1), compute the required relation from each row i. If there is any conflict, impossible.
        // Similarly for rows.
        // If consistent, we have a set of equations: for each adjacent pair, either they must be equal or different.
        // This forms a graph where each connected component has 2 possible assignments (flip all). We need to choose assignment to minimize cost.
        // Cost: we need to assign actual integer increments to rows and columns, not just parity. But wait, the problem allows any non-negative integer increments (each worker can be hired at most once, so increment is 0 or 1). So we are choosing a subset of rows and columns to increment by 1. The parity constraints are necessary and sufficient? Let's check: if we satisfy parity constraints, does there exist a valid assignment of 0/1 increments? The condition is h[i][j] + r[i] + c[j] != h[i][j+1] + r[i] + c[j+1] => c[j] - c[j+1] != h[i][j+1] - h[i][j]. Since c[j], c[j+1] are 0 or 1, the difference is -1, 0, or 1. The RHS is some integer. If we only enforce mod 2, we might have a situation where c[j]=0, c[j+1]=1 gives difference -1, and h[i][j+1]-h[i][j] = -1, then they are equal, violating condition. But mod 2 they differ? -1 mod 2 = 1, 1 mod 2 = 1, so they are equal mod 2. Wait, condition is inequality of actual values, not mod 2. So parity is not sufficient. We need to consider actual differences.
        // Let's re-evaluate: we need h[i][j] + r[i] + c[j] != h[i][j+1] + r[i] + c[j+1] for all i,j.
        // => c[j] - c[j+1] != h[i][j+1] - h[i][j].
        // Since c[j], c[j+1] in {0,1}, the LHS is in {-1, 0, 1}.
        // So for a fixed j, the forbidden difference D_i = h[i][j+1] - h[i][j] must not be equal to c[j] - c[j+1].
        // This means we need to choose c[j], c[j+1] such that for all i, c[j] - c[j+1] != D_i.
        // Since there are only 3 possible values for LHS, we can check feasibility.
        // Similarly for rows: r[i] - r[i+1] != h[i+1][j] - h[i][j] for all j.
        // This is a 2-SAT-like problem but with more than binary variables? Actually each variable is binary (0 or 1). The constraint is on the difference of two variables. We can model as: for each adjacent pair, we have a set of forbidden differences. We need to assign 0/1 to all variables to avoid these forbidden differences.
        // This is equivalent to: we have a graph where each edge (u,v) has a set of allowed pairs (0,0), (0,1), (1,0), (1,1). We need to find an assignment satisfying all edges.
        // This is a constraint satisfaction problem on a path (for rows) and a path (for columns). Since the graph is a collection of paths (rows and columns are independent? Actually rows and columns are independent because constraints only involve either rows or columns, not both. There is no constraint mixing a row and a column variable. So we can solve rows and columns separately.)
        // For a path, we can do DP. For each connected component (which is a path), we can try both assignments for the first node and propagate. Since it's a path, we can just try both and take min cost.
        // But wait: the constraints for rows involve all columns j. For a fixed adjacent row pair (i,i+1), the forbidden difference is any value that appears as h[i+1][j] - h[i][j] for some j. So the set of forbidden differences for edge (i,i+1) is the set of all D_j = h[i+1][j] - h[i][j]. We need r[i] - r[i+1] not in that set.
        // Similarly for columns.
        // So we can compute for each adjacent row pair the set of forbidden differences. Since differences can be large, we only care if -1, 0, 1 are forbidden. Because r[i] - r[i+1] can only be -1, 0, 1. So we just check which of these three values appear in the set of D_j for that pair. If all three appear, impossible. Otherwise, we have allowed differences.
        // Then we need to assign 0/1 to rows to satisfy all adjacent constraints. This is exactly a 2-coloring problem with some edges requiring inequality (difference != 0) and some requiring specific differences? Actually difference != 0 means r[i] != r[i+1]. Difference != 1 means we cannot have r[i]=1, r[i+1]=0. Difference != -1 means we cannot have r[i]=0, r[i+1]=1.
        // So each edge gives a set of allowed (r[i], r[i+1]) pairs. We can represent this as a directed implication graph or just do DP on the path.
        // Since the graph is a path, we can do DP from left to right. For each node, we try both values 0 and 1, and keep min cost.
        // However, the path might have multiple connected components? The constraints are only between adjacent rows, so it's a single path of n nodes. So we can just do DP on the path.
        // Similarly for columns.
        // Total cost = min cost for rows + min cost for columns.
        // But wait: is there any interaction between rows and columns? The condition for cell (i,j) involves both r[i] and c[j]. However, the constraints we derived are only on adjacent cells. The condition for horizontal adjacency only involves c[j] and c[j+1], not r[i]. Similarly vertical only involves r[i] and r[i+1]. So indeed they are independent. We can choose row increments and column increments independently.
        // However, we must ensure that the final heights are valid. But we only enforced adjacent inequalities. There is no condition on non-adjacent cells. So independent solutions for rows and columns should work.
        // Let's double-check: Suppose we assign r and c satisfying all horizontal and vertical constraints. Then for any horizontal adjacent pair, h[i][j] + r[i] + c[j] != h[i][j+1] + r[i] + c[j+1] because c[j] - c[j+1] != h[i][j+1] - h[i][j]. This holds regardless of r[i]. So yes, independent.
        // So we can solve rows and columns separately.
        
        // For rows:
        // We have n nodes, edges between i and i+1 for i=0..n-2.
        // For each edge, compute forbidden differences set from all j.
        // Allowed pairs for (r[i], r[i+1]):
        // (0,0): diff 0
        // (0,1): diff -1
        // (1,0): diff 1
        // (1,1): diff 0
        // So if 0 is forbidden, then (0,0) and (1,1) are disallowed.
        // If 1 is forbidden, then (1,0) disallowed.
        // If -1 is forbidden, then (0,1) disallowed.
        // We can do DP: dp[i][val] = min cost to assign first i+1 rows with row i having value val.
        // Initialize dp[0][0] = 0, dp[0][1] = a[0].
        // For i from 0 to n-2:
        //   for each val in {0,1}:
        //     for each nxt in {0,1}:
        //       if (val, nxt) allowed by edge i:
        //         dp[i+1][nxt] = min(dp[i+1][nxt], dp[i][val] + (nxt ? a[i+1] : 0))
        // Answer for rows = min(dp[n-1][0], dp[n-1][1]).
        // If no valid assignment, return -1.
        
        auto solve_path = [&](const vector<long long>& cost) -> long long {
            // cost size n
            // build forbidden sets for each edge
            // but we need the grid differences. This lambda needs access to grid and direction.
            // We'll write separate code for rows and columns.
            return 0; // placeholder
        };
        
        // Actually we can write a generic function that takes a function to get difference for edge (i, i+1) and column j.
        // For rows: diff(i, j) = h[i+1][j] - h[i][j]
        // For columns: diff(j, i) = h[i][j+1] - h[i][j] (note swapped indices)
        
        auto solve = [&](bool is_row) -> long long {
            int m = is_row ? n : n; // number of variables
            const vector<long long>& cost = is_row ? a : b;
            // precompute forbidden sets for each edge
            // edge k between k and k+1 (0-indexed)
            vector<array<bool, 3>> forbid(m-1); // forbid[0]: diff -1, forbid[1]: diff 0, forbid[2]: diff 1
            for (int k = 0; k < m-1; ++k) {
                forbid[k] = {false, false, false};
                for (int j = 0; j < n; ++j) {
                    long long diff;
                    if (is_row) {
                        diff = h[k+1][j] - h[k][j];
                    } else {
                        diff = h[j][k+1] - h[j][k];
                    }
                    if (diff == -1) forbid[k][0] = true;
                    else if (diff == 0) forbid[k][1] = true;
                    else if (diff == 1) forbid[k][2] = true;
                    // other differences don't matter because LHS is only -1,0,1
                }
            }
            // DP
            vector<long long> dp0(m, INF), dp1(m, INF);
            dp0[0] = 0;
            dp1[0] = cost[0];
            for (int i = 0; i < m-1; ++i) {
                // from i to i+1
                // allowed transitions:
                // (0,0): diff 0 -> allowed if !forbid[i][1]
                // (0,1): diff -1 -> allowed if !forbid[i][0]
                // (1,0): diff 1 -> allowed if !forbid[i][2]
                // (1,1): diff 0 -> allowed if !forbid[i][1]
                if (dp0[i] < INF) {
                    if (!forbid[i][1]) { // (0,0)
                        dp0[i+1] = min(dp0[i+1], dp0[i]);
                    }
                    if (!forbid[i][0]) { // (0,1)
                        dp1[i+1] = min(dp1[i+1], dp0[i] + cost[i+1]);
                    }
                }
                if (dp1[i] < INF) {
                    if (!forbid[i][2]) { // (1,0)
                        dp0[i+1] = min(dp0[i+1], dp1[i]);
                    }
                    if (!forbid[i][1]) { // (1,1)
                        dp1[i+1] = min(dp1[i+1], dp1[i] + cost[i+1]);
                    }
                }
            }
            long long ans = min(dp0[m-1], dp1[m-1]);
            return ans;
        };
        
        long long row_cost = solve(true);
        long long col_cost = solve(false);
        
        if (row_cost >= INF || col_cost >= INF) {
            cout << -1 << '\n';
        } else {
            cout << row_cost + col_cost << '\n';
        }
    }
    return 0;
}
