#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll INF = 1e18;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<vector<ll>> h(n, vector<ll>(n));
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                cin >> h[i][j];
        vector<ll> a(n), b(n);
        for (int i = 0; i < n; ++i) cin >> a[i];
        for (int j = 0; j < n; ++j) cin >> b[j];

        // We will try all 4 possibilities for the parity pattern of (row+col) mod 2
        // For each parity, we decide which parity gets +1 from row/col operations.
        // Actually, the condition is that adjacent cells have different heights.
        // After adding row[i] and col[j] to h[i][j], we need h[i][j] + row[i] + col[j] != h[i'][j'] + row[i'] + col[j'] for adjacent.
        // This is equivalent to: for each adjacent pair, the difference in original heights must not equal the difference in (row+col) adjustments.
        // Since row and col adjustments are independent per row/col, we can think of it as assigning each row and col a parity (0 or 1) of whether we add 1.
        // But we can add multiple times? No, each worker can be hired at most once, so row[i] is either 0 or 1, col[j] is either 0 or 1.
        // So we are choosing a subset of rows and columns to increment by 1.
        // The condition becomes: for all adjacent (i,j) and (i,j+1): h[i][j] + r[i] + c[j] != h[i][j+1] + r[i] + c[j+1] => h[i][j] - h[i][j+1] != c[j+1] - c[j].
        // Similarly for vertical: h[i][j] - h[i+1][j] != r[i+1] - r[i].
        // Since r[i], c[j] are in {0,1}, the differences are in {-1,0,1}.
        // This suggests a 2-coloring of the grid based on parity of (i+j). Let's set x[i][j] = r[i] + c[j] mod 2? Actually, we can consider the parity of the total increment.
        // Let's define for each cell a target parity p[i][j] = (r[i] + c[j]) % 2. Then the condition is that adjacent cells have different total increments mod 2? Not exactly.
        // The condition h[i][j] + r[i] + c[j] != h[i][j+1] + r[i] + c[j+1] can be rewritten as (h[i][j] - h[i][j+1]) % 2 != (c[j+1] - c[j]) % 2? Wait, we are dealing with actual values, not mod 2.
        // But since r and c are 0/1, the difference in total increment is in {-1,0,1}. So the condition is that the original difference is not equal to that small difference.
        // This is a constraint on the original heights modulo something? Actually, if original difference is 0, then we must have c[j+1] != c[j] (i.e., they differ). If original difference is 1 or -1, we must avoid the case where the adjustment difference equals it.
        // This looks like we can decide for each row and column whether to pick it, and we need to satisfy constraints that depend on differences of original heights.
        // Since n <= 1000, sum n <= 1000, we can perhaps model as 2-SAT or bipartite matching? Let's analyze further.

        // Consider the parity of (i+j). Let's color the grid like a chessboard. For adjacent cells, they have different parity.
        // Let's denote for each cell the value v[i][j] = h[i][j] + r[i] + c[j].
        // The condition is v[i][j] != v[i'][j'] for adjacent.
        // This is equivalent to: for each edge, the two endpoints have different values.
        // Since r[i] and c[j] are 0/1, the possible values for a cell are h[i][j] + 0, +1, or +2.
        // Notice that if we consider the parity of v[i][j], it is (h[i][j] + r[i] + c[j]) % 2.
        // For adjacent cells, if they have the same parity, they could still be different if their values differ by at least 2. But since increments are at most 2, if original difference is 0, they can become equal only if increments are same. If original difference is 1, they can become equal if increments differ by 1 in the opposite direction.
        // This seems complicated. Let's think differently.

        // Observe that the operation of adding 1 to a row or column is equivalent to toggling the parity of all cells in that row/column.
        // The condition "no two adjacent cells have the same height" is a local constraint.
        // Let's define for each cell a target value? Actually, we can think of assigning each cell a "color" based on its final height mod 2? Not enough.

        // Another perspective: Since we can only add 0 or 1 to each row and column, the total increment of cell (i,j) is r[i] + c[j] ∈ {0,1,2}.
        // The final height is h[i][j] + r[i] + c[j].
        // For adjacent cells (i,j) and (i,j+1), we need h[i][j] + r[i] + c[j] != h[i][j+1] + r[i] + c[j+1] => h[i][j] - h[i][j+1] != c[j+1] - c[j].
        // Since c[j+1] - c[j] ∈ {-1,0,1}, this means that if h[i][j] == h[i][j+1], we must have c[j] != c[j+1].
        // If h[i][j] - h[i][j+1] == 1, we must have c[j+1] - c[j] != 1 => c[j+1] <= c[j] (i.e., not (c[j]=0, c[j+1]=1)).
        // If h[i][j] - h[i][j+1] == -1, we must have c[j+1] - c[j] != -1 => c[j+1] >= c[j] (i.e., not (c[j]=1, c[j+1]=0)).
        // If the difference is anything else (absolute difference >=2), then no matter what c[j], c[j+1] are, the final heights cannot be equal because the adjustment difference is at most 1 in magnitude, so the final difference will be at least 1. So no constraint.

        // Similarly for vertical: h[i][j] - h[i+1][j] != r[i+1] - r[i].
        // So constraints only arise when original adjacent heights are equal or differ by exactly 1.
        // This gives constraints between c[j] and c[j+1] (for each row i) and between r[i] and r[i+1] (for each column j).
        // But note that the horizontal constraint involves c[j] and c[j+1] and depends on h[i][j] - h[i][j+1] for each row i. So for a fixed column pair (j, j+1), we may have multiple constraints from different rows. They must all be satisfied simultaneously.
        // Similarly for vertical.

        // Let's formalize:
        // For each adjacent column pair (j, j+1), we have a set of constraints from rows i=1..n:
        // Let diff = h[i][j] - h[i][j+1].
        // If diff == 0: then c[j] != c[j+1].
        // If diff == 1: then not (c[j]=0 and c[j+1]=1) => (c[j], c[j+1]) != (0,1).
        // If diff == -1: then not (c[j]=1 and c[j+1]=0) => (c[j], c[j+1]) != (1,0).
        // If |diff| >= 2: no constraint.
        // For a given column pair, we need to choose (c[j], c[j+1]) ∈ {0,1}^2 that satisfies all these constraints from all rows.
        // Similarly for row pairs.

        // Additionally, there are no constraints linking r and c directly? Wait, the horizontal constraints only involve c, vertical only involve r. So the problem decouples into independent choices for rows and columns? But we have costs a_i and b_j, and we want to minimize sum of a_i for chosen rows + sum of b_j for chosen columns. Since constraints are separate, we can solve for rows and columns independently? Let's check if there is any cross constraint. The condition for adjacent cells only involves either two cells in the same row (so only c's) or two cells in the same column (only r's). There is no constraint that involves both a row and a column variable simultaneously. So indeed, the row choices and column choices are independent! The total cost is sum of costs of chosen rows plus sum of costs of chosen columns. We can minimize them separately.

        // Wait, is that true? Let's verify: The condition for horizontal adjacency: h[i][j] + r[i] + c[j] != h[i][j+1] + r[i] + c[j+1]. The r[i] cancels out! So it only depends on c[j] and c[j+1]. Similarly vertical: r[i] and r[i+1] appear, c[j] cancels. So yes, the constraints are completely decoupled. The problem reduces to two independent problems: choose a subset of rows (r[i] ∈ {0,1}) to satisfy vertical constraints with minimum cost, and choose a subset of columns (c[j] ∈ {0,1}) to satisfy horizontal constraints with minimum cost. The total cost is the sum.

        // So we just need to solve for a 1D array of variables x[0..n-1] ∈ {0,1} with constraints between adjacent variables, minimizing sum of costs for variables set to 1.
        // This is a classic problem solvable by DP or by considering each connected component (which is the whole line) and trying both possibilities for the first variable, then propagating.

        // Let's detail the constraints for columns (horizontal):
        // For each j from 0 to n-2, we have a set of allowed pairs for (c[j], c[j+1]) based on all rows i.
        // We can precompute for each j, the allowed transitions.
        // Initially, all 4 pairs are allowed. Then for each row i, we look at diff = h[i][j] - h[i][j+1].
        // If diff == 0: forbid (0,0) and (1,1).
        // If diff == 1: forbid (0,1).
        // If diff == -1: forbid (1,0).
        // If |diff| >= 2: nothing forbidden.
        // After processing all rows, we have a set of allowed (c[j], c[j+1]).
        // If for some j, no pair is allowed, then it's impossible for columns -> overall impossible.

        // Similarly for rows (vertical):
        // For each i from 0 to n-2, for each column j, diff = h[i][j] - h[i+1][j].
        // Constraints on (r[i], r[i+1]).
        // If diff == 0: forbid (0,0) and (1,1).
        // If diff == 1: forbid (0,1).
        // If diff == -1: forbid (1,0).
        // If |diff| >= 2: nothing.

        // Then we need to find assignment of r[0..n-1] ∈ {0,1} minimizing sum of a_i for i where r[i]=1, subject to adjacent constraints.
        // This is a shortest path problem on a graph with 2 nodes per position? Actually, it's a DP on a chain.
        // For each position i, we have two states: 0 or 1. Transition from i to i+1 allowed if (state_i, state_{i+1}) is allowed.
        // Cost of state 1 at position i is a_i, cost of state 0 is 0.
        // We want minimum total cost.
        // Since n <= 1000, we can just do DP: dp[i][s] = min cost for first i+1 variables ending with state s at i.
        // dp[0][0] = 0, dp[0][1] = a[0].
        // For i from 0 to n-2:
        //   dp[i+1][0] = min over s in {0,1} such that (s,0) allowed: dp[i][s] + 0
        //   dp[i+1][1] = min over s in {0,1} such that (s,1) allowed: dp[i][s] + a[i+1]
        // If at some point both are INF, impossible.
        // Answer for rows is min(dp[n-1][0], dp[n-1][1]).
        // Same for columns with costs b.

        // But wait: Is there any global constraint linking rows and columns? The problem statement says "no two adjacent by side buildings have the same height". We satisfied horizontal and vertical separately. But what about the case where a cell's final height might equal a diagonal neighbor? The problem only forbids adjacent by side (sharing an edge), not diagonal. So we are good.

        // Let's double-check with sample 2:
        // n=4. We'll test later.

        // Edge case: n=1? But constraints say n>=2, so fine.

        // Complexity: O(n^2) per test case, sum n <= 1000, so O(sum n^2) <= 1e6, easily fits.

        auto solve_1d = [&](const vector<vector<bool>>& allowed) -> ll {
            // allowed[i][s1][s2] is true if transition from s1 to s2 is allowed at edge i (between i and i+1)
            // Actually we can just compute allowed transitions per edge.
            // We'll do DP.
            int m = allowed.size() + 1; // number of variables
            // allowed has size m-1, each is a 2x2 bool array.
            vector<ll> dp0(m, INF), dp1(m, INF);
            dp0[0] = 0;
            dp1[0] = a[0]; // wait, for rows we use a, for columns we use b. We'll pass cost array.
            // We'll make the lambda take cost array.
        };

        // Let's write a function:
        auto min_cost = [&](const vector<ll>& cost, const vector<array<array<bool,2>,2>>& trans) -> ll {
            int m = cost.size();
            vector<ll> dp0(m, INF), dp1(m, INF);
            dp0[0] = 0;
            dp1[0] = cost[0];
            for (int i = 0; i < m-1; ++i) {
                const auto& t = trans[i];
                if (t[0][0]) dp0[i+1] = min(dp0[i+1], dp0[i]);
                if (t[1][0]) dp0[i+1] = min(dp0[i+1], dp1[i]);
                if (t[0][1]) dp1[i+1] = min(dp1[i+1], dp0[i] + cost[i+1]);
                if (t[1][1]) dp1[i+1] = min(dp1[i+1], dp1[i] + cost[i+1]);
            }
            ll ans = min(dp0[m-1], dp1[m-1]);
            return ans;
        };

        // Build column constraints (horizontal)
        vector<array<array<bool,2>,2>> col_trans(n-1);
        for (int j = 0; j < n-1; ++j) {
            auto& t = col_trans[j];
            for (int s1 = 0; s1 < 2; ++s1)
                for (int s2 = 0; s2 < 2; ++s2)
                    t[s1][s2] = true;
            for (int i = 0; i < n; ++i) {
                ll diff = h[i][j] - h[i][j+1];
                if (diff == 0) {
                    t[0][0] = false;
                    t[1][1] = false;
                } else if (diff == 1) {
                    t[0][1] = false;
                } else if (diff == -1) {
                    t[1][0] = false;
                }
                // else |diff|>=2: no restriction
            }
        }

        // Build row constraints (vertical)
        vector<array<array<bool,2>,2>> row_trans(n-1);
        for (int i = 0; i < n-1; ++i) {
            auto& t = row_trans[i];
            for (int s1 = 0; s1 < 2; ++s1)
                for (int s2 = 0; s2 < 2; ++s2)
                    t[s1][s2] = true;
            for (int j = 0; j < n; ++j) {
                ll diff = h[i][j] - h[i+1][j];
                if (diff == 0) {
                    t[0][0] = false;
                    t[1][1] = false;
                } else if (diff == 1) {
                    t[0][1] = false;
                } else if (diff == -1) {
                    t[1][0] = false;
                }
            }
        }

        ll cost_rows = min_cost(a, row_trans);
        ll cost_cols = min_cost(b, col_trans);

        if (cost_rows >= INF/2 || cost_cols >= INF/2) {
            cout << -1 << '\n';
        } else {
            cout << cost_rows + cost_cols << '\n';
        }
    }
    return 0;
}
