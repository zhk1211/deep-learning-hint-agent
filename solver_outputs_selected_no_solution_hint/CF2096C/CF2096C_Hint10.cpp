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

        // Check if already beautiful
        bool beautiful = true;
        for (int i = 0; i < n && beautiful; ++i) {
            for (int j = 0; j < n - 1; ++j) {
                if (h[i][j] == h[i][j + 1]) {
                    beautiful = false;
                    break;
                }
            }
        }
        for (int i = 0; i < n - 1 && beautiful; ++i) {
            for (int j = 0; j < n; ++j) {
                if (h[i][j] == h[i + 1][j]) {
                    beautiful = false;
                    break;
                }
            }
        }
        if (beautiful) {
            cout << "0\n";
            continue;
        }

        // We need to hire exactly n workers (one per row or one per column) or n+something?
        // According to hints: if less than n workers impossible, and more than n not needed.
        // Actually we can hire up to n workers from A and up to n from B, but total hired <= n.
        // The key: we must hire exactly n workers, each either from A or B, covering all rows/columns.
        // Because if we miss a row and a column, the intersection can't be fixed.
        // So we choose a subset of rows R and columns C with |R|+|C| = n.
        // Then we add 1 to chosen rows and columns.
        // Condition: for all i,j: h[i][j] + (i in R) + (j in C) != adjacent ones.
        // This is equivalent to: for each adjacent pair, the parity of (row/col selection) must differ if heights equal.
        // Let x_i = 1 if row i hired, 0 else. y_j = 1 if col j hired, 0 else.
        // Sum x_i + sum y_j = n.
        // Adjacent horizontally: h[i][j] + x_i + y_j != h[i][j+1] + x_i + y_{j+1} => y_j != y_{j+1} if h[i][j] == h[i][j+1].
        // Adjacent vertically: h[i][j] + x_i + y_j != h[i+1][j] + x_{i+1} + y_j => x_i != x_{i+1} if h[i][j] == h[i+1][j].
        // So constraints only involve x_i differences and y_j differences based on equal adjacent heights.
        // This forms a bipartite graph of constraints? Actually x_i and y_j are independent except total sum = n.
        // We can think of x and y as binary variables. The constraints are: for each row i, if there is a vertical equal pair, x_i must be opposite to x_{i+1}. Similarly for columns.
        // This forces the x array to be determined up to flipping all bits, same for y.
        // But total sum = n links them: sum x + sum y = n.
        // So we can find all possible (sum x, sum y) pairs that satisfy constraints, then minimize cost.

        // Build constraints for rows:
        vector<vector<int>> row_adj(n);
        for (int i = 0; i < n - 1; ++i) {
            for (int j = 0; j < n; ++j) {
                if (h[i][j] == h[i + 1][j]) {
                    row_adj[i].push_back(i + 1);
                    row_adj[i + 1].push_back(i);
                    break; // only need one equal to force difference
                }
            }
        }
        // Build constraints for columns:
        vector<vector<int>> col_adj(n);
        for (int j = 0; j < n - 1; ++j) {
            for (int i = 0; i < n; ++i) {
                if (h[i][j] == h[i][j + 1]) {
                    col_adj[j].push_back(j + 1);
                    col_adj[j + 1].push_back(j);
                    break;
                }
            }
        }

        // Check bipartiteness and find components for rows
        vector<int> row_color(n, -1);
        vector<vector<int>> row_comps; // each component: list of nodes
        bool possible = true;
        for (int i = 0; i < n; ++i) {
            if (row_color[i] == -1) {
                row_color[i] = 0;
                queue<int> q;
                q.push(i);
                vector<int> comp;
                while (!q.empty()) {
                    int u = q.front(); q.pop();
                    comp.push_back(u);
                    for (int v : row_adj[u]) {
                        if (row_color[v] == -1) {
                            row_color[v] = row_color[u] ^ 1;
                            q.push(v);
                        } else if (row_color[v] == row_color[u]) {
                            possible = false;
                        }
                    }
                }
                row_comps.push_back(comp);
            }
        }
        if (!possible) {
            cout << "-1\n";
            continue;
        }

        // Same for columns
        vector<int> col_color(n, -1);
        vector<vector<int>> col_comps;
        for (int j = 0; j < n; ++j) {
            if (col_color[j] == -1) {
                col_color[j] = 0;
                queue<int> q;
                q.push(j);
                vector<int> comp;
                while (!q.empty()) {
                    int u = q.front(); q.pop();
                    comp.push_back(u);
                    for (int v : col_adj[u]) {
                        if (col_color[v] == -1) {
                            col_color[v] = col_color[u] ^ 1;
                            q.push(v);
                        } else if (col_color[v] == col_color[u]) {
                            possible = false;
                        }
                    }
                }
                col_comps.push_back(comp);
            }
        }
        if (!possible) {
            cout << "-1\n";
            continue;
        }

        // For each row component, we can choose to flip all colors (i.e., assign x_i = color or 1-color).
        // This gives two possible sums of x in that component: cnt0 and cnt1.
        // Similarly for columns.
        // We need total sum x + total sum y = n.
        // We can compute all possible total sums for rows and columns separately, then combine.
        // Since n <= 1000, number of components <= n. We can use DP (bitset) to find reachable sums.
        // But total sum of n over test cases <= 1000, so O(n^2/64) is fine.

        bitset<1001> row_dp, col_dp;
        row_dp[0] = 1;
        for (auto& comp : row_comps) {
            int cnt0 = 0, cnt1 = 0;
            for (int u : comp) {
                if (row_color[u] == 0) cnt0++;
                else cnt1++;
            }
            bitset<1001> next;
            next |= (row_dp << cnt0);
            next |= (row_dp << cnt1);
            row_dp = next;
        }

        col_dp[0] = 1;
        for (auto& comp : col_comps) {
            int cnt0 = 0, cnt1 = 0;
            for (int u : comp) {
                if (col_color[u] == 0) cnt0++;
                else cnt1++;
            }
            bitset<1001> next;
            next |= (col_dp << cnt0);
            next |= (col_dp << cnt1);
            col_dp = next;
        }

        // Now we need to choose sum_x and sum_y such that sum_x + sum_y = n.
        // Also we need to know the exact assignment to compute cost? We need minimum cost.
        // Cost = sum_{i in hired rows} a_i + sum_{j in hired cols} b_j.
        // Since we can flip each component independently, we need to find for each possible sum_x, the minimum cost to achieve that sum_x, and similarly for sum_y.
        // Then answer = min_{sx+sy=n} (min_cost_row[sx] + min_cost_col[sy]).

        vector<long long> min_cost_row(n + 1, INF);
        // Initialize with first component
        // We'll do DP with costs
        vector<long long> dp_row(n + 1, INF);
        dp_row[0] = 0;
        for (auto& comp : row_comps) {
            vector<long long> new_dp(n + 1, INF);
            // compute cost for two assignments
            long long cost0 = 0, cost1 = 0;
            int cnt0 = 0, cnt1 = 0;
            for (int u : comp) {
                if (row_color[u] == 0) {
                    cost0 += a[u];
                    cnt0++;
                } else {
                    cost1 += a[u];
                    cnt1++;
                }
            }
            // Actually careful: if we choose assignment 0 (x_i = row_color), then hired rows are those with row_color=1? Wait.
            // x_i = 1 means hired. We can set x_i = row_color or x_i = 1 - row_color.
            // If we set x_i = row_color, then hired rows are those with row_color=1. So cost = sum_{u: row_color[u]=1} a[u].
            // If we flip, hired rows are those with row_color=0. So cost = sum_{u: row_color[u]=0} a[u].
            // So for each component, we have two choices: (cnt_hired = number of 1s in chosen assignment, cost).
            // Let's define for a component:
            // choice A: x_i = row_color -> hired = {u | row_color[u]=1}, cnt = cnt1, cost = cost1.
            // choice B: x_i = 1 - row_color -> hired = {u | row_color[u]=0}, cnt = cnt0, cost = cost0.
            // So we have pairs (cnt1, cost1) and (cnt0, cost0).
            for (int s = 0; s <= n; ++s) {
                if (dp_row[s] != INF) {
                    if (s + cnt1 <= n) new_dp[s + cnt1] = min(new_dp[s + cnt1], dp_row[s] + cost1);
                    if (s + cnt0 <= n) new_dp[s + cnt0] = min(new_dp[s + cnt0], dp_row[s] + cost0);
                }
            }
            dp_row = move(new_dp);
        }
        min_cost_row = dp_row;

        vector<long long> dp_col(n + 1, INF);
        dp_col[0] = 0;
        for (auto& comp : col_comps) {
            vector<long long> new_dp(n + 1, INF);
            long long cost0 = 0, cost1 = 0;
            int cnt0 = 0, cnt1 = 0;
            for (int u : comp) {
                if (col_color[u] == 0) {
                    cost0 += b[u];
                    cnt0++;
                } else {
                    cost1 += b[u];
                    cnt1++;
                }
            }
            for (int s = 0; s <= n; ++s) {
                if (dp_col[s] != INF) {
                    if (s + cnt1 <= n) new_dp[s + cnt1] = min(new_dp[s + cnt1], dp_col[s] + cost1);
                    if (s + cnt0 <= n) new_dp[s + cnt0] = min(new_dp[s + cnt0], dp_col[s] + cost0);
                }
            }
            dp_col = move(new_dp);
        }
        vector<long long> min_cost_col = dp_col;

        long long ans = INF;
        for (int sx = 0; sx <= n; ++sx) {
            int sy = n - sx;
            if (min_cost_row[sx] != INF && min_cost_col[sy] != INF) {
                ans = min(ans, min_cost_row[sx] + min_cost_col[sy]);
            }
        }

        if (ans == INF) cout << "-1\n";
        else cout << ans << "\n";
    }
    return 0;
}
