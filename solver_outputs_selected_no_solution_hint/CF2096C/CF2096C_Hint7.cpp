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
        
        // We need to decide for each row and column whether to increment it.
        // Let x_i in {0,1} for row i, y_j in {0,1} for column j.
        // Condition: for all adjacent cells, h[i][j] + x_i + y_j != h[i'][j'] + x_i' + y_j'.
        // This is equivalent to: (x_i + y_j) - (x_i' + y_j') != h[i'][j'] - h[i][j].
        // For adjacent cells, the difference in (x+y) must not equal the negative height difference.
        
        // Key observation: if we fix the parity of (x_i + y_j) for all cells, we can check feasibility.
        // Actually, the condition only involves differences of (x_i + y_j) across edges.
        // This is a 2-coloring problem on the bipartite graph of rows and columns.
        // Let d_row[i] = x_i, d_col[j] = y_j. For each edge (i,j)-(i,j+1): (x_i+y_j) - (x_i+y_{j+1}) = y_j - y_{j+1} != h[i][j+1] - h[i][j].
        // So y_j - y_{j+1} != diff_horiz[i][j].
        // Similarly for vertical: x_i - x_{i+1} != diff_vert[i][j].
        // Since x_i, y_j are 0/1, differences are in {-1,0,1}.
        // So if any |diff| > 1, it's impossible to violate? Actually we need to avoid equality.
        // If diff is 0, we must have y_j != y_{j+1} (difference must be non-zero, so ±1). So y_j and y_{j+1} must be different.
        // If diff is 1, we must have y_j - y_{j+1} != 1 => not (y_j=1, y_{j+1}=0). So we cannot have y_j=1 and y_{j+1}=0.
        // If diff is -1, we must have y_j - y_{j+1} != -1 => not (y_j=0, y_{j+1}=1).
        // Similarly for rows.
        
        // This gives constraints between variables. We can model as 2-SAT? But n up to 1000, sum n 1000, so we can do something simpler.
        // Notice that the constraints only involve differences. The whole system is basically: we need to assign 0/1 to rows and columns.
        // We can try all 4 possibilities for (x_0, y_0) maybe? But constraints might force everything.
        // Actually, if we fix one row's value, does it determine everything? Not necessarily, because constraints are only between adjacent rows/columns.
        // But we can think of it as: we have a graph where each edge gives a relation between the two endpoints (must be equal or different).
        // This is a bipartite graph coloring problem with two colors, but with some edges forcing inequality and some forcing equality? Wait:
        // For horizontal edge: if diff == 0, then y_j != y_{j+1}. If diff == 1, then not (y_j=1, y_{j+1}=0) => y_j <= y_{j+1} (since 0/1). Actually it means if y_j=1 then y_{j+1}=1. So y_j=1 => y_{j+1}=1. If diff == -1, then not (y_j=0, y_{j+1}=1) => y_j=0 => y_{j+1}=0? Wait: not (y_j=0, y_{j+1}=1) means we cannot have y_j=0 and y_{j+1}=1. So if y_j=0 then y_{j+1} must be 0. So y_j=0 => y_{j+1}=0.
        // So horizontal edges give constraints between columns only. Vertical edges give constraints between rows only.
        // There are no constraints mixing rows and columns! That's crucial.
        // So the problem decomposes into independent problems on rows and on columns.
        // For rows: we have constraints between x_i and x_{i+1} based on vertical differences.
        // For columns: constraints between y_j and y_{j+1} based on horizontal differences.
        // And we can choose any combination of valid row assignments and column assignments independently.
        // Then the cost is sum of a_i for rows with x_i=1 plus sum of b_j for columns with y_j=1.
        // We want to minimize cost.
        
        // So we need to find valid 0/1 assignments for rows and columns separately, minimizing cost.
        // For rows: we have a chain of n variables x_0..x_{n-1}. For each i from 0 to n-2, we have a constraint based on vertical differences in column j? Wait, vertical edge between (i,j) and (i+1,j) gives constraint: x_i - x_{i+1} != h[i+1][j] - h[i][j].
        // But there are n columns, so for a given adjacent row pair (i,i+1), we have n constraints (one per column). They must all be satisfied simultaneously.
        // Let diff = h[i+1][j] - h[i][j]. The constraint is: x_i - x_{i+1} != diff.
        // Since x_i, x_{i+1} in {0,1}, x_i - x_{i+1} can be -1, 0, 1.
        // So if for some column, diff is not in {-1,0,1}, then the constraint is automatically satisfied (since x_i - x_{i+1} can never equal that diff). So we only care about columns where diff is -1, 0, or 1.
        // For a fixed row pair (i,i+1), we collect the set of diffs that appear in any column. The constraint is that x_i - x_{i+1} must not be equal to any of these diffs.
        // So the allowed values for (x_i, x_{i+1}) are those where the difference is not in the forbidden set.
        // Since there are only 4 possible pairs, we can just check which pairs are allowed.
        // Then we need to choose x_0..x_{n-1} to minimize sum a_i * x_i, subject to these adjacent constraints.
        // This is a shortest path / DP on a chain of 2 states per position.
        
        // Similarly for columns.
        
        // If either rows or columns have no valid assignment, answer -1.
        // Otherwise, min cost = min_row_cost + min_col_cost.
        
        // Let's implement.
        
        auto solve_chain = [&](const vector<long long>& cost, const vector<vector<int>>& forbidden_diff) -> long long {
            // forbidden_diff[i][d] where d is -1,0,1 mapped to 0,1,2? Actually we just need to know for each adjacent pair (i,i+1), which differences are forbidden.
            // We'll compute allowed pairs.
            int n = cost.size();
            // dp[i][val] = min cost for first i+1 variables ending with x_i = val
            vector<array<long long, 2>> dp(n, {INF, INF});
            dp[0][0] = 0;
            dp[0][1] = cost[0];
            for (int i = 0; i < n-1; ++i) {
                for (int cur = 0; cur < 2; ++cur) {
                    if (dp[i][cur] == INF) continue;
                    for (int nxt = 0; nxt < 2; ++nxt) {
                        int diff = cur - nxt; // -1, 0, 1
                        bool ok = true;
                        for (int d : forbidden_diff[i]) {
                            if (diff == d) { ok = false; break; }
                        }
                        if (ok) {
                            long long add = (nxt == 1) ? cost[i+1] : 0;
                            dp[i+1][nxt] = min(dp[i+1][nxt], dp[i][cur] + add);
                        }
                    }
                }
            }
            return min(dp[n-1][0], dp[n-1][1]);
        };
        
        // Build forbidden differences for rows
        vector<vector<int>> row_forbidden(n-1);
        bool row_possible = true;
        for (int i = 0; i < n-1; ++i) {
            set<int> forbidden;
            for (int j = 0; j < n; ++j) {
                long long diff = h[i+1][j] - h[i][j];
                if (diff >= -1 && diff <= 1) {
                    forbidden.insert((int)diff);
                }
            }
            row_forbidden[i] = vector<int>(forbidden.begin(), forbidden.end());
            // If all three differences are forbidden, then no assignment possible
            if (forbidden.size() == 3) {
                row_possible = false;
            }
        }
        
        // Build forbidden differences for columns
        vector<vector<int>> col_forbidden(n-1);
        bool col_possible = true;
        for (int j = 0; j < n-1; ++j) {
            set<int> forbidden;
            for (int i = 0; i < n; ++i) {
                long long diff = h[i][j+1] - h[i][j];
                if (diff >= -1 && diff <= 1) {
                    forbidden.insert((int)diff);
                }
            }
            col_forbidden[j] = vector<int>(forbidden.begin(), forbidden.end());
            if (forbidden.size() == 3) {
                col_possible = false;
            }
        }
        
        if (!row_possible || !col_possible) {
            cout << -1 << '\n';
            continue;
        }
        
        long long row_cost = solve_chain(a, row_forbidden);
        long long col_cost = solve_chain(b, col_forbidden);
        
        if (row_cost >= INF/2 || col_cost >= INF/2) {
            cout << -1 << '\n';
        } else {
            cout << row_cost + col_cost << '\n';
        }
    }
    return 0;
}
