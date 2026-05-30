// Hint4
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

        // Check parity condition: for each adjacent pair, the difference in heights
        // must have opposite parity to the difference in row/col increments.
        // More precisely, we need to assign row parity r[i] in {0,1} and col parity c[j] in {0,1}
        // such that for every adjacent pair (i,j)-(i,j+1): (h[i][j] + r[i] + c[j]) != (h[i][j+1] + r[i] + c[j+1])
        // => c[j] - c[j+1] != h[i][j+1] - h[i][j] (mod 2)
        // Similarly for vertical: r[i] - r[i+1] != h[i+1][j] - h[i][j] (mod 2)
        // This is a 2-coloring problem on a bipartite graph of rows and columns.
        // We can solve it by checking consistency and then DP for min cost.

        // Build graph for parity constraints
        vector<vector<pair<int,int>>> g(n + n);
        bool possible = true;
        auto add_edge = [&](int u, int v, int w) {
            g[u].emplace_back(v, w);
            g[v].emplace_back(u, w);
        };
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n - 1; ++j) {
                ll diff = h[i][j+1] - h[i][j];
                int w = (diff % 2 + 2) % 2;
                // c[j] and c[j+1] must have different parity if w == 0? Wait:
                // We need c[j] - c[j+1] != diff (mod 2)
                // => c[j] + c[j+1] != diff (mod 2) because -c[j+1] ≡ c[j+1] mod 2? No, -1 ≡ 1 mod 2, so -c ≡ c mod 2.
                // Actually mod 2, subtraction is same as addition. So c[j] - c[j+1] ≡ c[j] + c[j+1] (mod 2).
                // So condition: c[j] + c[j+1] != diff (mod 2)
                // => c[j] + c[j+1] ≡ 1 - diff (mod 2)
                // Let w = (1 - diff) mod 2.
                w = (1 - w) % 2;
                if (w < 0) w += 2;
                add_edge(j, n + j + 1, w); // column j (index j) and column j+1 (index n+j+1)
            }
        }
        for (int j = 0; j < n; ++j) {
            for (int i = 0; i < n - 1; ++i) {
                ll diff = h[i+1][j] - h[i][j];
                int w = (diff % 2 + 2) % 2;
                w = (1 - w) % 2;
                if (w < 0) w += 2;
                add_edge(n + n + i, n + n + i + 1, w); // row i (index n+n+i) and row i+1
            }
        }
        // Also constraints between row i and column j? Not directly, because they don't need to be different.
        // But we need to assign parities consistently. The graph has components. For each component, we can choose the parity of one node, then all others are determined.
        // We'll compute for each component the cost of setting its "root" to 0 or 1.
        vector<int> color(n + n, -1);
        vector<ll> cost0(n + n, 0), cost1(n + n, 0);
        int comp_id = 0;
        vector<int> comp_of(n + n, -1);
        vector<vector<int>> comp_nodes;
        function<void(int,int,int)> dfs = [&](int u, int c, int cur_comp) {
            color[u] = c;
            comp_of[u] = cur_comp;
            comp_nodes[cur_comp].push_back(u);
            for (auto &[v, w] : g[u]) {
                int nc = c ^ w;
                if (color[v] == -1) {
                    dfs(v, nc, cur_comp);
                } else if (color[v] != nc) {
                    possible = false;
                }
            }
        };
        for (int i = 0; i < n + n; ++i) {
            if (color[i] == -1) {
                comp_nodes.push_back({});
                dfs(i, 0, comp_id++);
            }
        }
        if (!possible) {
            cout << "-1\n";
            continue;
        }
        // Now we have components. For each component, we can choose to flip all parities (cost changes).
        // For each node, we know its parity assignment relative to component root.
        // We need to compute cost for rows and columns separately.
        // For a row i (index n+n+i), if its final parity is p (0 or 1), cost is (p == 0 ? 0 : a[i]).
        // For a column j (index j), if its final parity is p, cost is (p == 0 ? 0 : b[j]).
        // But wait: parity p means we add p to that row/col? Actually, we can add 0 or 1 mod 2? No, we can add any number of times, but adding 2 is same as adding 0 mod 2 for parity condition.
        // However, we can also add 2, 4, ... which doesn't change parity but costs more. So optimal is to add at most 1 per row/col? Not necessarily, because we might need to fix differences >1.
        // The parity condition only ensures adjacent cells have different parity. But they could still be equal if difference is even and we add same parity.
        // Actually, if we ensure all adjacent cells have different parity, then they can't be equal because equal numbers have same parity. So parity condition is sufficient to avoid equality? Wait: if two numbers have different parity, they cannot be equal. So yes, ensuring adjacent cells have different parity is necessary and sufficient to avoid equality.
        // So we only care about parity of increments. We can add 0 or 1 to each row/col (mod 2). Adding more than 1 is suboptimal because it costs more and doesn't help parity.
        // Thus we just need to choose parity for each row and column to satisfy constraints, minimizing sum of costs where cost is a_i if row parity=1, else 0; similarly b_j if col parity=1.
        // This is exactly a minimum weight assignment on the bipartite graph of components? Wait, rows and columns are in the same components now because we added edges between columns and between rows, but not between rows and columns. So components are either all rows, all columns, or mixed? Actually we only added edges among columns and among rows separately. So components are either a set of columns or a set of rows. They are independent.
        // So we can solve for rows and columns independently.
        // For a component of columns, we have a set of columns with relative parities. We can choose the parity of the root (0 or 1). The cost for the component is sum over columns j in component of (parity(j) ? b[j] : 0). We can compute cost if root=0 and if root=1, take min.
        // Same for rows.
        // Total cost is sum over components of min cost.
        ll total_cost = 0;
        for (int cid = 0; cid < comp_id; ++cid) {
            ll cost0_comp = 0, cost1_comp = 0;
            for (int u : comp_nodes[cid]) {
                // determine if it's row or column
                if (u < n) { // column
                    // parity of u if root=0 is color[u] (since root color=0)
                    // if root=1, parity is color[u]^1
                    if (color[u] == 0) {
                        cost0_comp += 0;
                        cost1_comp += b[u];
                    } else {
                        cost0_comp += b[u];
                        cost1_comp += 0;
                    }
                } else if (u < n + n) { // also column? Wait indices: 0..n-1 are columns, n..n+n-1? Actually we used n+j+1 for column j+1? Let's check:
                    // In add_edge for columns: j and n+j+1. So column j is j, column j+1 is n+j+1. That means columns are 0..n-1, and also n..n+n-1? That's overlapping. Mistake.
                    // We should re-index properly.
                }
            }
        }
        // Let's redo indexing cleanly.
        // We'll have nodes 0..n-1 for columns, n..2n-1 for rows.
        // So total 2n nodes.
        // Rebuild graph.
        int V = 2 * n;
        vector<vector<pair<int,int>>> g2(V);
        auto add_edge2 = [&](int u, int v, int w) {
            g2[u].emplace_back(v, w);
            g2[v].emplace_back(u, w);
        };
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n - 1; ++j) {
                ll diff = h[i][j+1] - h[i][j];
                int w = (diff % 2 + 2) % 2;
                w = (1 - w) % 2;
                if (w < 0) w += 2;
                add_edge2(j, j+1, w); // columns j and j+1
            }
        }
        for (int j = 0; j < n; ++j) {
            for (int i = 0; i < n - 1; ++i) {
                ll diff = h[i+1][j] - h[i][j];
                int w = (diff % 2 + 2) % 2;
                w = (1 - w) % 2;
                if (w < 0) w += 2;
                add_edge2(n + i, n + i + 1, w); // rows i and i+1
            }
        }
        vector<int> color2(V, -1);
        vector<int> comp2(V, -1);
        int comps = 0;
        vector<vector<int>> comp_nodes2;
        possible = true;
        function<void(int,int,int)> dfs2 = [&](int u, int c, int cur) {
            color2[u] = c;
            comp2[u] = cur;
            comp_nodes2[cur].push_back(u);
            for (auto &[v, w] : g2[u]) {
                int nc = c ^ w;
                if (color2[v] == -1) {
                    dfs2(v, nc, cur);
                } else if (color2[v] != nc) {
                    possible = false;
                }
            }
        };
        for (int i = 0; i < V; ++i) {
            if (color2[i] == -1) {
                comp_nodes2.push_back({});
                dfs2(i, 0, comps++);
            }
        }
        if (!possible) {
            cout << "-1\n";
            continue;
        }
        total_cost = 0;
        for (int cid = 0; cid < comps; ++cid) {
            ll cost0 = 0, cost1 = 0;
            for (int u : comp_nodes2[cid]) {
                if (u < n) { // column
                    if (color2[u] == 0) {
                        cost0 += 0;
                        cost1 += b[u];
                    } else {
                        cost0 += b[u];
                        cost1 += 0;
                    }
                } else { // row
                    int r = u - n;
                    if (color2[u] == 0) {
                        cost0 += 0;
                        cost1 += a[r];
                    } else {
                        cost0 += a[r];
                        cost1 += 0;
                    }
                }
            }
            total_cost += min(cost0, cost1);
        }
        cout << total_cost << "\n";
    }
    return 0;
}
