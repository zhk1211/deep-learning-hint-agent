#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
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
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                cin >> h[i][j];
        vector<ll> a(n), b(n);
        for (int i = 0; i < n; i++) cin >> a[i];
        for (int j = 0; j < n; j++) cin >> b[j];
        
        // Check if already beautiful
        bool beautiful = true;
        for (int i = 0; i < n && beautiful; i++) {
            for (int j = 0; j < n-1; j++) {
                if (h[i][j] == h[i][j+1]) { beautiful = false; break; }
            }
        }
        for (int i = 0; i < n-1 && beautiful; i++) {
            for (int j = 0; j < n; j++) {
                if (h[i][j] == h[i+1][j]) { beautiful = false; break; }
            }
        }
        if (beautiful) {
            cout << "0\n";
            continue;
        }
        
        // We need to hire exactly n workers: some rows, some columns.
        // Let x = number of rows hired, y = number of columns hired, x+y = n.
        // For each cell (i,j), final height = h[i][j] + r_i + c_j, where r_i in {0,1}, c_j in {0,1}.
        // Condition: for adjacent cells, heights must differ.
        // This is equivalent to: for each edge, the parity of (r_i + c_j) must be opposite to the parity of the difference of original heights?
        // Actually, condition: h[i][j] + r_i + c_j != h[i][j+1] + r_i + c_{j+1}  =>  (c_j - c_{j+1}) != h[i][j+1] - h[i][j].
        // Similarly for vertical: (r_i - r_{i+1}) != h[i+1][j] - h[i][j].
        // Since r_i, c_j are 0/1, differences are in {-1,0,1}.
        // This is a 2-SAT like problem but with exactly n ones total.
        // Observation: If we fix the set of rows and columns to hire (size n), then the assignment of which rows/columns is forced by the parity constraints?
        // Actually, we can think of it as: we want to assign each row a bit r_i, each column a bit c_j, sum of bits = n.
        // For each horizontal adjacency (i,j)-(i,j+1): r_i + c_j != r_i + c_{j+1} + (h[i][j+1]-h[i][j]) mod something? No, it's exact inequality.
        // Since r_i, c_j are 0/1, the difference r_i + c_j - (r_i + c_{j+1}) = c_j - c_{j+1} is in {-1,0,1}.
        // So we need c_j - c_{j+1} != D where D = h[i][j+1] - h[i][j].
        // If |D| > 1, then it's automatically satisfied regardless of c_j, c_{j+1} because max difference is 1.
        // If D == 0, then we need c_j != c_{j+1}.
        // If D == 1, then we need c_j - c_{j+1} != 1 => c_j != 1 or c_{j+1} != 0 => not (c_j=1 and c_{j+1}=0) => c_j <= c_{j+1} (since bits). So c_j=0 or c_{j+1}=1.
        // If D == -1, then c_j - c_{j+1} != -1 => not (c_j=0 and c_{j+1}=1) => c_j >= c_{j+1}.
        // Similarly for vertical: r_i - r_{i+1} != D_v.
        // So we have constraints on rows and columns separately! They are independent except for the total sum = n.
        // Wait, horizontal constraints only involve columns, vertical only involve rows. So rows and columns are independent except the total count.
        // So we can find all valid assignments of rows (with some number of ones k) and columns (with n-k ones) that satisfy constraints, and minimize cost.
        // Since n <= 1000, we can do DP for rows and columns separately to find minimum cost to achieve exactly k ones while satisfying constraints.
        // Then combine.
        
        // For rows: we have constraints between adjacent rows i and i+1 for each column j.
        // For a fixed j, constraint: r_i - r_{i+1} != D where D = h[i+1][j] - h[i][j].
        // This must hold for all j. So for each adjacent row pair (i,i+1), we have multiple constraints (one per column). They must all be satisfied.
        // If any column gives |D|>1, no constraint. If D==0, need r_i != r_{i+1}. If D==1, need not (r_i=1, r_{i+1}=0) => r_i <= r_{i+1}. If D==-1, need r_i >= r_{i+1}.
        // So for each pair (i,i+1), we can deduce allowed pairs of (r_i, r_{i+1}):
        // - If there exists a column with D==0 and another with D==1? Then we might have conflicting requirements.
        // Actually, we need to satisfy all columns simultaneously. So for a given pair (i,i+1), we collect the set of constraints from all columns.
        // Let S be the set of D values that appear (only -1,0,1 matter). If S contains both 0 and 1? Then we need r_i != r_{i+1} AND r_i <= r_{i+1}. The only possibility is r_i=0, r_{i+1}=1.
        // If S contains both 0 and -1: need r_i != r_{i+1} AND r_i >= r_{i+1} => r_i=1, r_{i+1}=0.
        // If S contains 1 and -1: need r_i <= r_{i+1} AND r_i >= r_{i+1} => r_i = r_{i+1}. But if also 0 is present, then impossible.
        // If S contains only 0: need r_i != r_{i+1}.
        // If S contains only 1: need r_i <= r_{i+1} (so 00,01,11 allowed).
        // If S contains only -1: need r_i >= r_{i+1} (00,10,11 allowed).
        // If S contains none of {-1,0,1}: no constraint, all pairs allowed.
        // So we can determine for each adjacent pair the allowed transitions.
        // Then we can do DP over rows: dp[i][k][last] = min cost to assign first i rows, with k ones, ending with last (0/1).
        // Similarly for columns.
        // Then answer = min over k=0..n of (dp_rows[n][k] + dp_cols[n][n-k]).
        
        // But careful: The constraints for rows come from vertical adjacencies. For each column j, we look at h[i+1][j] - h[i][j].
        // So for a fixed row pair (i,i+1), we need to check all columns j.
        
        auto solve_dim = [&](const vector<vector<ll>>& grid, const vector<ll>& cost, bool transpose) -> vector<vector<ll>> {
            // grid is n x n. If transpose, we are solving for columns (so we swap indices).
            // We'll build allowed transitions for adjacent indices.
            int N = n;
            vector<vector<int>> allowed(N-1, vector<int>(4, 0)); // allowed[i][state] where state = (last<<1)|cur
            for (int i = 0; i < N-1; i++) {
                set<int> diffs;
                for (int j = 0; j < N; j++) {
                    ll diff;
                    if (!transpose) {
                        diff = grid[i+1][j] - grid[i][j];
                    } else {
                        diff = grid[j][i+1] - grid[j][i];
                    }
                    if (diff == 0) diffs.insert(0);
                    else if (diff == 1) diffs.insert(1);
                    else if (diff == -1) diffs.insert(-1);
                }
                bool has0 = diffs.count(0);
                bool has1 = diffs.count(1);
                bool hasm1 = diffs.count(-1);
                // Determine allowed (last, cur)
                if (has0 && has1 && hasm1) {
                    // impossible
                    allowed[i][0] = allowed[i][1] = allowed[i][2] = allowed[i][3] = 0;
                } else if (has0 && has1) {
                    // need r_i != r_{i+1} and r_i <= r_{i+1} => only 0->1
                    allowed[i][1] = 1; // 0->1
                } else if (has0 && hasm1) {
                    // need r_i != r_{i+1} and r_i >= r_{i+1} => only 1->0
                    allowed[i][2] = 1; // 1->0
                } else if (has1 && hasm1) {
                    // need r_i <= r_{i+1} and r_i >= r_{i+1} => r_i == r_{i+1}
                    allowed[i][0] = 1; // 0->0
                    allowed[i][3] = 1; // 1->1
                    if (has0) {
                        // also has0? impossible, but we already handled has0 with others.
                    }
                } else if (has0) {
                    // only need r_i != r_{i+1}
                    allowed[i][1] = 1; // 0->1
                    allowed[i][2] = 1; // 1->0
                } else if (has1) {
                    // only need r_i <= r_{i+1}
                    allowed[i][0] = 1; // 0->0
                    allowed[i][1] = 1; // 0->1
                    allowed[i][3] = 1; // 1->1
                } else if (hasm1) {
                    // only need r_i >= r_{i+1}
                    allowed[i][0] = 1; // 0->0
                    allowed[i][2] = 1; // 1->0
                    allowed[i][3] = 1; // 1->1
                } else {
                    // no constraints
                    allowed[i][0] = allowed[i][1] = allowed[i][2] = allowed[i][3] = 1;
                }
            }
            // DP
            vector<vector<ll>> dp(N+1, vector<ll>(N+1, INF));
            // base: first element
            dp[1][0] = 0; // not hired
            dp[1][1] = cost[0]; // hired
            for (int i = 1; i < N; i++) {
                for (int k = 0; k <= i; k++) {
                    if (dp[i][k] >= INF) continue;
                    for (int last = 0; last < 2; last++) {
                        ll val = dp[i][k];
                        // try cur = 0
                        if (allowed[i-1][last*2 + 0]) {
                            dp[i+1][k] = min(dp[i+1][k], val);
                        }
                        // try cur = 1
                        if (allowed[i-1][last*2 + 1]) {
                            dp[i+1][k+1] = min(dp[i+1][k+1], val + cost[i]);
                        }
                    }
                }
            }
            // dp[N][k] for last=0 and last=1
            vector<ll> res(N+1, INF);
            for (int k = 0; k <= N; k++) {
                res[k] = min(dp[N][k], dp[N][k]); // wait, dp[N][k] already min over last? Actually dp[i][k] is min over last? No, we didn't separate by last in dp state. We need to.
            }
            // Let's redo DP with state (i, k, last)
            vector<vector<vector<ll>>> dp2(N+1, vector<vector<ll>>(N+1, vector<ll>(2, INF)));
            dp2[1][0][0] = 0;
            dp2[1][1][1] = cost[0];
            for (int i = 1; i < N; i++) {
                for (int k = 0; k <= i; k++) {
                    for (int last = 0; last < 2; last++) {
                        if (dp2[i][k][last] >= INF) continue;
                        for (int cur = 0; cur < 2; cur++) {
                            if (allowed[i-1][last*2 + cur]) {
                                ll add = (cur == 1) ? cost[i] : 0;
                                dp2[i+1][k+cur][cur] = min(dp2[i+1][k+cur][cur], dp2[i][k][last] + add);
                            }
                        }
                    }
                }
            }
            vector<ll> ans(N+1, INF);
            for (int k = 0; k <= N; k++) {
                ans[k] = min(dp2[N][k][0], dp2[N][k][1]);
            }
            return ans;
        };
        
        vector<ll> row_cost = solve_dim(h, a, false);
        vector<ll> col_cost = solve_dim(h, b, true);
        
        ll ans = INF;
        for (int k = 0; k <= n; k++) {
            if (row_cost[k] < INF && col_cost[n-k] < INF) {
                ans = min(ans, row_cost[k] + col_cost[n-k]);
            }
        }
        if (ans >= INF) cout << "-1\n";
        else cout << ans << "\n";
    }
    return 0;
}
