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

        // We will decide parity of row increments and column increments.
        // Let x_i in {0,1} be parity of row i increment, y_j in {0,1} be parity of column j increment.
        // Condition: for adjacent cells horizontally: h[i][j] + x_i + y_j != h[i][j+1] + x_i + y_{j+1} mod 2
        // => y_j != y_{j+1} mod 2 if h[i][j] % 2 == h[i][j+1] % 2
        //    y_j == y_{j+1} mod 2 if h[i][j] % 2 != h[i][j+1] % 2
        // Similarly for vertical: x_i != x_{i+1} if h[i][j] % 2 == h[i+1][j] % 2
        //                         x_i == x_{i+1} if h[i][j] % 2 != h[i+1][j] % 2

        // Build constraints for columns (y) from horizontal adjacencies.
        vector<vector<pair<int,int>>> col_adj(n);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n-1; ++j) {
                bool same = (h[i][j] % 2) == (h[i][j+1] % 2);
                if (same) {
                    col_adj[j].push_back({j+1, 1});
                    col_adj[j+1].push_back({j, 1});
                } else {
                    col_adj[j].push_back({j+1, 0});
                    col_adj[j+1].push_back({j, 0});
                }
            }
        }
        // Build constraints for rows (x) from vertical adjacencies.
        vector<vector<pair<int,int>>> row_adj(n);
        for (int j = 0; j < n; ++j) {
            for (int i = 0; i < n-1; ++i) {
                bool same = (h[i][j] % 2) == (h[i+1][j] % 2);
                if (same) {
                    row_adj[i].push_back({i+1, 1});
                    row_adj[i+1].push_back({i, 1});
                } else {
                    row_adj[i].push_back({i+1, 0});
                    row_adj[i+1].push_back({i, 0});
                }
            }
        }

        // Bipartite coloring check for columns
        vector<int> col_color(n, -1);
        bool ok_col = true;
        function<void(int,int)> dfs_col = [&](int u, int c) {
            col_color[u] = c;
            for (auto &[v, w] : col_adj[u]) {
                int nc = c ^ w;
                if (col_color[v] == -1) dfs_col(v, nc);
                else if (col_color[v] != nc) ok_col = false;
            }
        };
        for (int i = 0; i < n && ok_col; ++i)
            if (col_color[i] == -1) dfs_col(i, 0);

        // Bipartite coloring check for rows
        vector<int> row_color(n, -1);
        bool ok_row = true;
        function<void(int,int)> dfs_row = [&](int u, int c) {
            row_color[u] = c;
            for (auto &[v, w] : row_adj[u]) {
                int nc = c ^ w;
                if (row_color[v] == -1) dfs_row(v, nc);
                else if (row_color[v] != nc) ok_row = false;
            }
        };
        for (int i = 0; i < n && ok_row; ++i)
            if (row_color[i] == -1) dfs_row(i, 0);

        if (!ok_col || !ok_row) {
            cout << "-1\n";
            continue;
        }

        // Now we have two possible assignments for columns (color 0 or 1) and rows.
        // But we can flip all x and all y simultaneously without changing differences mod 2.
        // However, costs are not symmetric: we need to choose actual increments (not just parity).
        // The actual increments are x_i + 2*k_i, y_j + 2*l_j. The parity condition only fixes x_i mod 2, y_j mod 2.
        // To minimize cost, we should pick the cheapest parity assignment for rows and columns independently,
        // because row and column choices are independent in cost and the parity constraints are separate.
        // For each connected component of rows, we can choose which parity gets 0 and which gets 1.
        // The cost for a component is min( sum_{i in comp, color=0} a_i , sum_{i in comp, color=1} a_i ).
        // Similarly for columns.

        // Compute components for rows
        vector<bool> vis_row(n, false);
        ll row_cost = 0;
        for (int i = 0; i < n; ++i) {
            if (!vis_row[i]) {
                vector<int> comp;
                function<void(int)> dfs_comp = [&](int u) {
                    vis_row[u] = true;
                    comp.push_back(u);
                    for (auto &[v, w] : row_adj[u])
                        if (!vis_row[v]) dfs_comp(v);
                };
                dfs_comp(i);
                ll cost0 = 0, cost1 = 0;
                for (int u : comp) {
                    if (row_color[u] == 0) cost0 += a[u];
                    else cost1 += a[u];
                }
                // We can flip the whole component: cost0 becomes cost of color 1, cost1 becomes cost of color 0.
                // Actually we have two choices: assign parity 0 to color 0 and parity 1 to color 1, or flip.
                // So min cost for this component is min(cost0, cost1).
                row_cost += min(cost0, cost1);
            }
        }

        // Compute components for columns
        vector<bool> vis_col(n, false);
        ll col_cost = 0;
        for (int i = 0; i < n; ++i) {
            if (!vis_col[i]) {
                vector<int> comp;
                function<void(int)> dfs_comp = [&](int u) {
                    vis_col[u] = true;
                    comp.push_back(u);
                    for (auto &[v, w] : col_adj[u])
                        if (!vis_col[v]) dfs_comp(v);
                };
                dfs_comp(i);
                ll cost0 = 0, cost1 = 0;
                for (int u : comp) {
                    if (col_color[u] == 0) cost0 += b[u];
                    else cost1 += b[u];
                }
                col_cost += min(cost0, cost1);
            }
        }

        cout << row_cost + col_cost << "\n";
    }
    return 0;
}
