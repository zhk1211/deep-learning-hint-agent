// Hint2
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
        
        // We will try all 4 possible parity assignments for the first row and first column
        // parity: 0 means even number of increments, 1 means odd number of increments
        long long ans = INF;
        for (int row0_parity = 0; row0_parity <= 1; ++row0_parity) {
            for (int col0_parity = 0; col0_parity <= 1; ++col0_parity) {
                // Determine required parity for each row and column based on constraints
                vector<int> row_parity(n, -1);
                vector<int> col_parity(n, -1);
                row_parity[0] = row0_parity;
                col_parity[0] = col0_parity;
                
                bool possible = true;
                // For each cell, the condition is: h[i][j] + row_parity[i] + col_parity[j] must differ from neighbors
                // We can derive constraints: for adjacent cells (i,j) and (i,j+1):
                // h[i][j] + row_parity[i] + col_parity[j] != h[i][j+1] + row_parity[i] + col_parity[j+1]
                // => col_parity[j] - col_parity[j+1] != h[i][j+1] - h[i][j] (mod 2)
                // Similarly for vertical: row_parity[i] - row_parity[i+1] != h[i+1][j] - h[i][j] (mod 2)
                
                // We can propagate constraints using BFS/DFS
                // Build graph: nodes 0..n-1 for rows, n..2n-1 for columns
                // Edge between row i and col j with weight = (h[i][j] % 2) ? Actually we need difference constraints
                // Let's use 2-SAT style: each variable can be 0 or 1, constraints are inequalities
                
                // Actually we can determine parity by checking consistency of a bipartite graph
                // For each adjacent pair, we get a constraint on the difference of parities
                
                // We'll use a graph where nodes are rows and columns, edges represent required difference mod 2
                // If we assign a parity to one node, all others are determined
                
                // Let's build graph
                vector<vector<pair<int, int>>> adj(2 * n);
                // For horizontal adjacency
                for (int i = 0; i < n; ++i) {
                    for (int j = 0; j < n - 1; ++j) {
                        // col j and col j+1 must have different parity if h[i][j] and h[i][j+1] have same parity
                        // because h[i][j] + row_parity[i] + col_parity[j] != h[i][j+1] + row_parity[i] + col_parity[j+1]
                        // => col_parity[j] - col_parity[j+1] != h[i][j+1] - h[i][j] (mod 2)
                        // If h[i][j+1] - h[i][j] is even, then col_parity[j] and col_parity[j+1] must be different
                        // If odd, they must be same
                        long long diff = h[i][j+1] - h[i][j];
                        int req_diff = (diff % 2 == 0) ? 1 : 0; // 1 means different, 0 means same
                        int u = n + j;
                        int v = n + j + 1;
                        adj[u].push_back({v, req_diff});
                        adj[v].push_back({u, req_diff});
                    }
                }
                // For vertical adjacency
                for (int i = 0; i < n - 1; ++i) {
                    for (int j = 0; j < n; ++j) {
                        long long diff = h[i+1][j] - h[i][j];
                        int req_diff = (diff % 2 == 0) ? 1 : 0;
                        int u = i;
                        int v = i + 1;
                        adj[u].push_back({v, req_diff});
                        adj[v].push_back({u, req_diff});
                    }
                }
                
                // Now propagate from initial assignments
                vector<int> val(2 * n, -1);
                val[0] = row0_parity;
                val[n] = col0_parity;
                
                queue<int> q;
                q.push(0);
                q.push(n);
                
                while (!q.empty()) {
                    int u = q.front(); q.pop();
                    for (auto [v, req_diff] : adj[u]) {
                        int expected = val[u] ^ req_diff;
                        if (val[v] == -1) {
                            val[v] = expected;
                            q.push(v);
                        } else if (val[v] != expected) {
                            possible = false;
                            break;
                        }
                    }
                    if (!possible) break;
                }
                
                if (!possible) continue;
                
                // For any unassigned nodes, we can choose parity freely to minimize cost
                // For each connected component, we can flip all parities, so we try both
                // But we already fixed some, so we need to consider components separately
                // Actually we can compute min cost for each component given fixed assignments
                
                // Let's find connected components of the graph (ignoring weights for connectivity)
                vector<bool> visited(2 * n, false);
                long long total_cost = 0;
                
                for (int start = 0; start < 2 * n; ++start) {
                    if (visited[start]) continue;
                    // BFS to find component
                    vector<int> comp;
                    queue<int> qc;
                    qc.push(start);
                    visited[start] = true;
                    while (!qc.empty()) {
                        int u = qc.front(); qc.pop();
                        comp.push_back(u);
                        for (auto [v, _] : adj[u]) {
                            if (!visited[v]) {
                                visited[v] = true;
                                qc.push(v);
                            }
                        }
                    }
                    
                    // Check if any node in component has fixed parity
                    int fixed_node = -1;
                    for (int u : comp) {
                        if (val[u] != -1) {
                            fixed_node = u;
                            break;
                        }
                    }
                    
                    if (fixed_node != -1) {
                        // Propagate fixed value within component
                        queue<int> qp;
                        qp.push(fixed_node);
                        while (!qp.empty()) {
                            int u = qp.front(); qp.pop();
                            for (auto [v, req_diff] : adj[u]) {
                                int expected = val[u] ^ req_diff;
                                if (val[v] == -1) {
                                    val[v] = expected;
                                    qp.push(v);
                                }
                            }
                        }
                        // Compute cost for this component
                        long long cost0 = 0, cost1 = 0;
                        for (int u : comp) {
                            if (u < n) {
                                // row
                                if (val[u] == 0) cost0 += 0; else cost0 += a[u];
                                if (val[u] == 1) cost1 += a[u]; else cost1 += 0;
                            } else {
                                // column
                                int idx = u - n;
                                if (val[u] == 0) cost0 += 0; else cost0 += b[idx];
                                if (val[u] == 1) cost1 += b[idx]; else cost1 += 0;
                            }
                        }
                        // Since val is fixed, we just take the cost corresponding to current assignment
                        // But wait, we might have both 0 and 1 in the component? No, val is determined.
                        // Actually cost0 and cost1 are just costs if all were 0 or all were 1, but we have mixed.
                        // We need to compute cost based on actual val[u].
                        long long comp_cost = 0;
                        for (int u : comp) {
                            if (u < n) {
                                if (val[u] == 1) comp_cost += a[u];
                            } else {
                                if (val[u] == 1) comp_cost += b[u - n];
                            }
                        }
                        total_cost += comp_cost;
                    } else {
                        // No fixed node, we can choose parity for entire component to minimize cost
                        long long cost0 = 0, cost1 = 0;
                        for (int u : comp) {
                            if (u < n) {
                                cost0 += 0; // parity 0 costs 0
                                cost1 += a[u];
                            } else {
                                cost0 += 0;
                                cost1 += b[u - n];
                            }
                        }
                        total_cost += min(cost0, cost1);
                        // We don't need to assign val, but we can just leave it
                    }
                }
                
                ans = min(ans, total_cost);
            }
        }
        
        if (ans == INF) cout << -1 << '\n';
        else cout << ans << '\n';
    }
    return 0;
}
