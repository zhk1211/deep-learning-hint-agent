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
                        
                        // 4-directional connectivity
                        int dr[] = {-1, 1, 0, 0};
                        int dc[] = {0, 0, -1, 1};
                        for (int d = 0; d < 4; ++d) {
                            int nr = r + dr[d];
                            int nc = c + dc[d];
                            if (nr >= 0 && nr < n && nc >= 0 && nc < m && 
                                grid[nr][nc] == '*' && !visited[nr][nc]) {
                                visited[nr][nc] = true;
                                q.push({nr, nc});
                            }
                        }
                    }
                    
                    // An L-shape must have exactly 3 cells
                    if (comp.size() != 3) {
                        ok = false;
                        break;
                    }
                    
                    // Check that the 3 cells form an L-shape (2x2 square missing one cell)
                    // Find bounding box
                    int min_r = n, max_r = -1, min_c = m, max_c = -1;
                    for (auto [r, c] : comp) {
                        min_r = min(min_r, r);
                        max_r = max(max_r, r);
                        min_c = min(min_c, c);
                        max_c = max(max_c, c);
                    }
                    
                    // Must fit in a 2x2 area
                    if (max_r - min_r > 1 || max_c - min_c > 1) {
                        ok = false;
                        break;
                    }
                    
                    // Check that no two L-shapes touch by corner or edge
                    // For each cell in this L-shape, check all 8 neighbors
                    // If any neighbor is '*' and not in this component, it's invalid
                    for (auto [r, c] : comp) {
                        for (int dr = -1; dr <= 1; ++dr) {
                            for (int dc = -1; dc <= 1; ++dc) {
                                if (dr == 0 && dc == 0) continue;
                                int nr = r + dr;
                                int nc = c + dc;
                                if (nr >= 0 && nr < n && nc >= 0 && nc < m && 
                                    grid[nr][nc] == '*') {
                                    // Check if (nr, nc) is in the current component
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
                            if (!ok) break;
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
