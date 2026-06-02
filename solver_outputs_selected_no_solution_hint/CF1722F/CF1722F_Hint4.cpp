#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n, m;
        cin >> n >> m;
        vector<string> grid(n);
        for (int i = 0; i < n; ++i) {
            cin >> grid[i];
        }
        
        vector<vector<bool>> visited(n, vector<bool>(m, false));
        bool ok = true;
        
        for (int i = 0; i < n && ok; ++i) {
            for (int j = 0; j < m && ok; ++j) {
                if (grid[i][j] == '*' && !visited[i][j]) {
                    // Find all cells of this connected component (by edges)
                    vector<pair<int,int>> comp;
                    queue<pair<int,int>> q;
                    q.push({i, j});
                    visited[i][j] = true;
                    
                    while (!q.empty()) {
                        auto [r, c] = q.front(); q.pop();
                        comp.push_back({r, c});
                        
                        for (int dr = -1; dr <= 1; ++dr) {
                            for (int dc = -1; dc <= 1; ++dc) {
                                if (dr == 0 && dc == 0) continue;
                                int nr = r + dr, nc = c + dc;
                                if (nr >= 0 && nr < n && nc >= 0 && nc < m && grid[nr][nc] == '*' && !visited[nr][nc]) {
                                    // Only connect by edges, not corners
                                    if (abs(dr) + abs(dc) == 1) {
                                        visited[nr][nc] = true;
                                        q.push({nr, nc});
                                    }
                                }
                            }
                        }
                    }
                    
                    // Check component size
                    if (comp.size() != 3) {
                        ok = false;
                        break;
                    }
                    
                    // Check if it forms an L-shape (2x2 bounding box with exactly 3 cells)
                    int min_r = n, max_r = -1, min_c = m, max_c = -1;
                    for (auto [r, c] : comp) {
                        min_r = min(min_r, r);
                        max_r = max(max_r, r);
                        min_c = min(min_c, c);
                        max_c = max(max_c, c);
                    }
                    
                    if (max_r - min_r != 1 || max_c - min_c != 1) {
                        ok = false;
                        break;
                    }
                    
                    // Check that no two L-shapes touch by corner or edge
                    // We already know this component is isolated by edges (BFS only edge-connected)
                    // But we need to check corners: any * in the 8-neighborhood of this component
                    // that is not part of the component would be a violation.
                    // Since we only visited edge-connected cells, we can check all 8 neighbors of each cell in comp.
                    for (auto [r, c] : comp) {
                        for (int dr = -1; dr <= 1; ++dr) {
                            for (int dc = -1; dc <= 1; ++dc) {
                                if (dr == 0 && dc == 0) continue;
                                int nr = r + dr, nc = c + dc;
                                if (nr >= 0 && nr < n && nc >= 0 && nc < m && grid[nr][nc] == '*') {
                                    // If this neighbor is not in our component, it's a violation
                                    bool in_comp = false;
                                    for (auto [cr, cc] : comp) {
                                        if (cr == nr && cc == nc) {
                                            in_comp = true;
                                            break;
                                        }
                                    }
                                    if (!in_comp) {
                                        ok = false;
                                        break;
                                    }
                                }
                            }
                        }
                        if (!ok) break;
                    }
                }
            }
        }
        
        cout << (ok ? "YES" : "NO") << '\n';
    }
    
    return 0;
}
