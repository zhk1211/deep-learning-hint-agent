// Hint6
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

        // We will try all 4 possibilities for the parity pattern of (row + col) mod 2
        // For each pattern, we decide which parity gets +1 and which gets 0 (relative)
        // Actually we need to assign each row and column a shift (0 or 1) such that
        // for any adjacent cells, the total shift differs.
        // This is equivalent to: for each row i, let r_i in {0,1}, for each col j, let c_j in {0,1}.
        // The condition: for adjacent horizontally: (r_i + c_j) != (r_i + c_{j+1}) mod 2 => c_j != c_{j+1}
        // So all columns must alternate: c_1, c_2, ... must be alternating 0,1,0,1... or 1,0,1,0...
        // Similarly, rows must alternate: r_i != r_{i+1}.
        // So there are exactly 2 choices for rows (r_1=0 or 1) and independently 2 for columns.
        // Total 4 combinations.

        // For a fixed (r, c) assignment, the required total addition to cell (i,j) is some target T_ij.
        // We want to achieve T_ij = h_ij + r_i + c_j (mod 2? No, we need actual values to avoid equality).
        // Actually the condition is that adjacent cells have different final heights.
        // If we add r_i to row i and c_j to col j, final height = h_ij + r_i + c_j.
        // We need: h_ij + r_i + c_j != h_i(j+1) + r_i + c_{j+1}  => h_ij + c_j != h_i(j+1) + c_{j+1}
        // and similarly for vertical: h_ij + r_i != h_{(i+1)j} + r_{i+1}.
        // Since c_j and c_{j+1} are different (alternating), the condition becomes:
        // h_ij - h_i(j+1) != c_{j+1} - c_j.
        // Since c_j, c_{j+1} are 0/1 and different, c_{j+1} - c_j is either +1 or -1.
        // So we need: h_ij - h_i(j+1) != d_j where d_j = c_{j+1} - c_j (which is +1 if c_j=0,c_{j+1}=1 else -1).
        // Similarly vertical: h_ij - h_{(i+1)j} != r_{i+1} - r_i (which is +1 or -1).
        // So for a fixed alternating pattern of rows and columns, we can check if it's possible without any additions?
        // Wait, we are allowed to add r_i and c_j which are 0 or 1? But the problem says we can hire each worker at most once.
        // Hiring worker i of company A adds 1 to entire row i. So r_i is either 0 or 1.
        // Similarly c_j is 0 or 1.
        // So indeed r_i, c_j in {0,1}.
        // But wait: the costs are given, we want to minimize sum of a_i for rows where r_i=1 plus sum of b_j for cols where c_j=1.
        // However, we also have the condition that the final heights must not be equal on adjacent cells.
        // The condition we derived: for each horizontal edge, h_ij - h_i(j+1) != d_j, where d_j = c_{j+1} - c_j.
        // Since c alternates, d_j is either +1 or -1 depending on the starting column parity.
        // Similarly vertical: h_ij - h_{(i+1)j} != e_i, where e_i = r_{i+1} - r_i (either +1 or -1).
        // So if we fix the alternating patterns (i.e., fix r_1 in {0,1} and c_1 in {0,1}), then all r_i and c_j are determined.
        // Then we can check if the condition holds. If it does, cost is sum of a_i for r_i=1 + sum of b_j for c_j=1.
        // But wait: is that sufficient? What if the original heights already violate the condition for that pattern?
        // Then we cannot fix it by adding 1s because we are forced to add exactly r_i and c_j. We cannot add more.
        // So if for some edge the difference equals d_j or e_i, then this pattern is invalid.
        // However, we might be able to choose a different pattern. There are only 4 patterns.
        // But is it possible that no pattern works, yet we can still make the city beautiful by adding more than 1 to some row/column?
        // The problem says each worker can be hired at most once, and adds exactly 1. So we can only add 0 or 1 to each row/column.
        // So the total addition to cell (i,j) is r_i + c_j with r_i, c_j in {0,1}.
        // So the above analysis is exact. There are only 4 possible assignments of (r_i, c_j) up to the alternating pattern.
        // But wait: Are we forced to have alternating rows and columns? Let's re-derive carefully.

        // We need: for all i,j: h_ij + r_i + c_j != h_i(j+1) + r_i + c_{j+1}
        // => h_ij + c_j != h_i(j+1) + c_{j+1}.
        // This must hold for all i and all j < n-1.
        // For a fixed i, consider the sequence of columns. The condition involves only c_j and c_{j+1}.
        // It does not force c_j to alternate globally? Let's see: If c_j = c_{j+1}, then condition becomes h_ij != h_i(j+1).
        // If they are different, condition becomes h_ij - h_i(j+1) != c_{j+1} - c_j.
        // So we are not forced to have c_j alternate. We could have some adjacent columns with same c, as long as the original heights differ.
        // But wait: The condition must hold for ALL rows i. For a given pair of adjacent columns (j, j+1), the condition involves h_ij and h_i(j+1) for all i.
        // If we set c_j = c_{j+1}, then we need h_ij != h_i(j+1) for all i. If there exists any row where h_ij == h_i(j+1), then we cannot set c_j = c_{j+1}.
        // If we set c_j != c_{j+1}, we need h_ij - h_i(j+1) != d (where d = +-1) for all i. If for some row the difference equals d, then we cannot set them different.
        // So for each adjacent column pair, we have constraints depending on the choice of c_j, c_{j+1}.
        // Similarly for rows.

        // This looks like a 2-SAT or bipartite graph problem. But n <= 1000, sum n <= 1000, so we can maybe do something else.
        // Let's think differently: The final height of (i,j) is H_ij = h_ij + r_i + c_j.
        // We need H_ij != H_i(j+1) and H_ij != H_(i+1)j.
        // This is equivalent to: For each adjacent pair, the difference in final heights is not zero.
        // Difference horizontally: H_ij - H_i(j+1) = (h_ij - h_i(j+1)) + (c_j - c_{j+1}).
        // We need this != 0.
        // So c_j - c_{j+1} != h_i(j+1) - h_ij.
        // Since c_j, c_{j+1} in {0,1}, c_j - c_{j+1} is in {-1, 0, 1}.
        // So for each horizontal edge (i,j)-(i,j+1), we have a forbidden value for c_j - c_{j+1}: it cannot be equal to h_i(j+1) - h_ij.
        // But h_i(j+1) - h_ij can be any integer (possibly large). If it is not in {-1,0,1}, then the forbidden value is not in {-1,0,1}, so no restriction on c_j - c_{j+1} from this edge.
        // If it is in {-1,0,1}, then that particular difference is forbidden.
        // Similarly for vertical edges: r_i - r_{i+1} != h_(i+1)j - h_ij.

        // So we have variables r_i, c_j in {0,1}. We want to assign them to satisfy:
        // For each horizontal edge (i,j)-(i,j+1): c_j - c_{j+1} != D_h(i,j) where D_h = h_i(j+1) - h_ij, but only if D_h in {-1,0,1}.
        // For each vertical edge (i,j)-(i+1,j): r_i - r_{i+1} != D_v(i,j) where D_v = h_(i+1)j - h_ij, only if D_v in {-1,0,1}.

        // Notice that the constraints on c_j are independent per column pair? Actually for a fixed j, the constraint involves c_j and c_{j+1} and must hold for all i. So for a given column pair (j, j+1), we have a set of forbidden differences (from each row i). The actual difference c_j - c_{j+1} must not be equal to any of these forbidden values. Since c_j - c_{j+1} can only be -1, 0, 1, if the set of forbidden values contains all three, then no assignment for this pair is possible -> impossible.
        // If it contains two values, then only one possible difference is allowed.
        // If it contains one value, two differences allowed.
        // If it contains none, all three allowed.

        // Similarly for rows.

        // Moreover, the constraints on columns are independent of rows, and vice versa? The variables r_i and c_j are separate. The constraints only involve either r's or c's, not both. So the problem decouples into two independent problems: one for rows and one for columns! Wait, is that true? Let's check: Horizontal edges only involve c_j and c_{j+1}. Vertical edges only involve r_i and r_{i+1}. There is no constraint mixing r and c. So indeed, the row assignments and column assignments are completely independent. The total cost is sum of a_i for r_i=1 plus sum of b_j for c_j=1. So we can minimize the cost for rows and columns separately and sum them up.

        // But wait: Is there any global constraint? The final heights must be beautiful, which is exactly those pairwise inequalities. There is no constraint linking a row and a column except through a cell, but the inequalities are only between adjacent cells, which separate into row and column differences. So yes, independent.

        // So we need to solve: Given n variables r_i in {0,1}, and for each adjacent pair (i,i+1), a set of forbidden differences F_i (subset of {-1,0,1}) such that r_i - r_{i+1} not in F_i. Minimize sum of a_i * r_i.
        // Similarly for columns with costs b_j.

        // This is a classic problem: we have a chain of binary variables with constraints on differences. We can solve it with DP.

        // For rows: We want to assign r_1..r_n in {0,1} to minimize sum a_i r_i subject to r_i - r_{i+1} != d for each forbidden d in F_i.
        // Since r_i in {0,1}, the difference can be -1 (0-1), 0 (0-0 or 1-1), 1 (1-0).
        // So for each edge i, we have a set of allowed transitions between r_i and r_{i+1}.
        // We can do DP: dp[i][0] = min cost for first i rows with r_i = 0; dp[i][1] similarly.
        // Initialize dp[1][0] = 0, dp[1][1] = a_1.
        // For i from 1 to n-1, for each state of r_i, we can transition to r_{i+1} if the difference is allowed.
        // Allowed if (r_i - r_{i+1}) is not in F_i.
        // F_i is the set of forbidden differences for edge (i,i+1). How to compute F_i?
        // For each row pair (i,i+1), we look at all columns j. For each j, D_v = h_(i+1)j - h_ij. If D_v in {-1,0,1}, we add D_v to F_i.
        // Actually the constraint is r_i - r_{i+1} != D_v. So if D_v is in {-1,0,1}, we forbid that difference.
        // So F_i = { D_v | j=1..n, D_v in {-1,0,1} }.
        // Similarly for columns: For each column pair (j,j+1), F'_j = { D_h | i=1..n, D_h in {-1,0,1} } where D_h = h_i(j+1) - h_ij. Forbid c_j - c_{j+1} in F'_j.

        // Then we run DP for rows and columns separately. If either DP returns INF, impossible -> -1. Else sum the mins.

        // Let's test with sample 2:
        // n=4. h:
        // 1 2 1 2
        // 3 2 1 2
        // 1 2 1 1
        // 1 3 1 2
        // a: 1 2 3 4
        // b: 5 6 7 8
        // Compute row constraints:
        // Edge 1-2: differences for each col:
        // col1: 3-1=2 (ignore)
        // col2: 2-2=0 -> forbid 0
        // col3: 1-1=0 -> forbid 0
        // col4: 2-2=0 -> forbid 0
        // So F_1 = {0}. Allowed differences: -1, 1.
        // Edge 2-3:
        // col1: 1-3=-2 ignore
        // col2: 2-2=0 forbid 0
        // col3: 1-1=0 forbid 0
        // col4: 1-2=-1 forbid -1
        // F_2 = {0, -1}. Allowed: 1.
        // Edge 3-4:
        // col1: 1-1=0 forbid 0
        // col2: 3-2=1 forbid 1
        // col3: 1-1=0 forbid 0
        // col4: 2-1=1 forbid 1
        // F_3 = {0,1}. Allowed: -1.
        // DP rows:
        // dp[1][0]=0, dp[1][1]=1
        // i=1->2: allowed diff -1,1.
        // From 0: diff to 0 is 0 (forbidden), to 1 is -1 (allowed). So dp[2][1] = dp[1][0] + a_2 = 0+2=2.
        // From 1: diff to 0 is 1 (allowed), to 1 is 0 (forbidden). dp[2][0] = dp[1][1] + 0 = 1.
        // dp[2][0]=1, dp[2][1]=2.
        // i=2->3: allowed diff 1 only.
        // From 0: diff to 0 is 0 (forbidden), to 1 is -1 (forbidden). So no transition from 0.
        // From 1: diff to 0 is 1 (allowed), to 1 is 0 (forbidden). dp[3][0] = dp[2][1] + 0 = 2.
        // dp[3][0]=2, dp[3][1]=INF.
        // i=3->4: allowed diff -1 only.
        // From 0: diff to 0 is 0 (forbidden), to 1 is -1 (allowed). dp[4][1] = dp[3][0] + a_4 = 2+4=6.
        // From 1: INF.
        // So min row cost = min(dp[4][0]=INF, dp[4][1]=6) = 6.
        // But sample answer says hire row 2 and 4: cost 2+4=6. Yes.
        // Columns:
        // Edge 1-2: differences for each row:
        // row1: 2-1=1 forbid 1
        // row2: 2-3=-1 forbid -1
        // row3: 2-1=1 forbid 1
        // row4: 3-1=2 ignore
        // F'_1 = {1, -1}. Allowed: 0.
        // Edge 2-3:
        // row1: 1-2=-1 forbid -1
        // row2: 1-2=-1 forbid -1
        // row3: 1-2=-1 forbid -1
        // row4: 1-3=-2 ignore
        // F'_2 = {-1}. Allowed: 0,1.
        // Edge 3-4:
        // row1: 2-1=1 forbid 1
        // row2: 2-1=1 forbid 1
        // row3: 1-1=0 forbid 0
        // row4: 2-1=1 forbid 1
        // F'_3 = {1,0}. Allowed: -1.
        // DP cols:
        // dp[1][0]=0, dp[1][1]=5
        // 1->2: allowed 0 only.
        // From 0: to 0 diff 0 allowed -> dp[2][0] = 0+0=0; to 1 diff -1 forbidden.
        // From 1: to 0 diff 1 forbidden; to 1 diff 0 allowed -> dp[2][1] = 5+6=11.
        // dp[2][0]=0, dp[2][1]=11.
        // 2->3: allowed 0,1.
        // From 0: to 0 (0) allowed -> dp[3][0]=0+0=0; to 1 (-1) forbidden.
        // From 1: to 0 (1) allowed -> dp[3][0] = min(0, 11+0)=0; to 1 (0) allowed -> dp[3][1] = 11+7=18.
        // dp[3][0]=0, dp[3][1]=18.
        // 3->4: allowed -1 only.
        // From 0: to 0 (0) forbidden; to 1 (-1) allowed -> dp[4][1] = 0+8=8.
        // From 1: to 0 (1) forbidden; to 1 (0) forbidden.
        // dp[4][0]=INF, dp[4][1]=8.
        // Min col cost = 8. Total = 6+8=14. Matches sample.

        // Sample 3:
        // 3
        // 1 2 2
        // 2 2 1
        // 2 1 1
        // a: 100 100 100
        // b: 100 100 100
        // Row edges:
        // 1-2: col1: 2-1=1 forbid 1; col2: 2-2=0 forbid 0; col3: 1-2=-1 forbid -1. F_1 = {-1,0,1} -> all forbidden. Impossible. So -1. Correct.

        // So the algorithm is correct.

        // Complexity: O(n^2) per test case, sum n <= 1000, so O(sum n^2) <= 1e6, very fast.

        // Implementation details:
        // For each test case:
        // Read n, h (n x n), a (n), b (n).
        // Compute row constraints: vector<set<int>> row_forbid(n-1); or just a bitmask of forbidden differences.
        // For i in 0..n-2:
        //   forbidden mask = 0
        //   for j in 0..n-1:
        //     diff = h[i+1][j] - h[i][j];
        //     if diff == -1: mask |= 1; // we can map -1 to 0, 0 to 1, 1 to 2? Better: use array of bools for -1,0,1.
        //     else if diff == 0: mask |= 2;
        //     else if diff == 1: mask |= 4;
        //   store mask.
        // Similarly for columns:
        // For j in 0..n-2:
        //   mask = 0
        //   for i in 0..n-1:
        //     diff = h[i][j+1] - h[i][j];
        //     if diff == -1: mask |= 1;
        //     else if diff == 0: mask |= 2;
        //     else if diff == 1: mask |= 4;
        //   store mask.

        // DP for rows:
        // dp0 = 0, dp1 = a[0]
        // for i from 0 to n-2:
        //   mask = row_forbid[i]
        //   new_dp0 = INF, new_dp1 = INF
        //   // from previous 0
        //   if dp0 < INF:
        //     // to 0: diff = 0, check if 0 forbidden: mask & 2
        //     if !(mask & 2) new_dp0 = min(new_dp0, dp0)
        //     // to 1: diff = -1, check if -1 forbidden: mask & 1
        //     if !(mask & 1) new_dp1 = min(new_dp1, dp0 + a[i+1])
        //   // from previous 1
        //   if dp1 < INF:
        //     // to 0: diff = 1, check if 1 forbidden: mask & 4
        //     if !(mask & 4) new_dp0 = min(new_dp0, dp1)
        //     // to 1: diff = 0, check if 0 forbidden: mask & 2
        //     if !(mask & 2) new_dp1 = min(new_dp1, dp1 + a[i+1])
        //   dp0 = new_dp0, dp1 = new_dp1
        // row_cost = min(dp0, dp1)
        // if row_cost == INF -> impossible.

        // Similarly for columns with b.

        // Total cost = row_cost + col_cost.

        // Edge case: n=2, works fine.

        // Note: a_i and b_j up to 1e9, sum can be up to 2e9 * 1000? Actually n<=1000, sum of a_i up to 1e12, fits in long long.

        auto solve_dp = [&](const vector<long long>& cost, const vector<int>& forbid_masks) -> long long {
            int m = cost.size();
            if (m == 0) return 0;
            long long dp0 = 0, dp1 = cost[0];
            for (int i = 0; i < m-1; ++i) {
                int mask = forbid_masks[i];
                long long ndp0 = INF, ndp1 = INF;
                if (dp0 < INF) {
                    if (!(mask & 2)) ndp0 = min(ndp0, dp0);
                    if (!(mask & 1)) ndp1 = min(ndp1, dp0 + cost[i+1]);
                }
                if (dp1 < INF) {
                    if (!(mask & 4)) ndp0 = min(ndp0, dp1);
                    if (!(mask & 2)) ndp1 = min(ndp1, dp1 + cost[i+1]);
                }
                dp0 = ndp0;
                dp1 = ndp1;
            }
            return min(dp0, dp1);
        };

        vector<int> row_masks(n-1, 0);
        for (int i = 0; i < n-1; ++i) {
            int mask = 0;
            for (int j = 0; j < n; ++j) {
                long long diff = h[i+1][j] - h[i][j];
                if (diff == -1) mask |= 1;
                else if (diff == 0) mask |= 2;
                else if (diff == 1) mask |= 4;
            }
            row_masks[i] = mask;
        }

        vector<int> col_masks(n-1, 0);
        for (int j = 0; j < n-1; ++j) {
            int mask = 0;
            for (int i = 0; i < n; ++i) {
                long long diff = h[i][j+1] - h[i][j];
                if (diff == -1) mask |= 1;
                else if (diff == 0) mask |= 2;
                else if (diff == 1) mask |= 4;
            }
            col_masks[j] = mask;
        }

        long long row_cost = solve_dp(a, row_masks);
        long long col_cost = solve_dp(b, col_masks);

        if (row_cost >= INF/2 || col_cost >= INF/2) {
            cout << -1 << '\n';
        } else {
            cout << row_cost + col_cost << '\n';
        }
    }
    return 0;
}
