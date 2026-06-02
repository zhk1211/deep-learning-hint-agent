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
                    // Find all cells of this component
                    vector<pair<int,int>> comp;
                    queue<pair<int,int>> q;
                    q.push({i, j});
                    visited[i][j] = true;
                    while (!q.empty()) {
                        auto [x, y] = q.front(); q.pop();
                        comp.push_back({x, y});
                        for (int dx = -1; dx <= 1; ++dx) {
                            for (int dy = -1; dy <= 1; ++dy) {
                                if (dx == 0 && dy == 0) continue;
                                int nx = x + dx, ny = y + dy;
                                if (nx >= 0 && nx < n && ny >= 0 && ny < m && grid[nx][ny] == '*' && !visited[nx][ny]) {
                                    visited[nx][ny] = true;
                                    q.push({nx, ny});
                                }
                            }
                        }
                    }
                    
                    if (comp.size() != 3) {
                        ok = false;
                        break;
                    }
                    
                    // Check if the 3 cells form an L-shape (not a line)
                    sort(comp.begin(), comp.end());
                    bool is_L = false;
                    // Check all 4 rotations of L-shape
                    // L-shape occupies cells (0,0), (1,0), (0,1) in a 2x2 block
                    // So min row and min col among the 3 cells
                    int min_r = comp[0].first;
                    int min_c = min({comp[0].second, comp[1].second, comp[2].second});
                    // Check if they fit in a 2x2 block starting at (min_r, min_c)
                    if (min_r + 1 < n && min_c + 1 < m) {
                        vector<pair<int,int>> block = {{min_r, min_c}, {min_r+1, min_c}, {min_r, min_c+1}, {min_r+1, min_c+1}};
                        for (int mask = 0; mask < 4; ++mask) {
                            // skip the cell that is not part of L-shape
                            vector<pair<int,int>> shape;
                            for (int k = 0; k < 4; ++k) {
                                if (k != mask) shape.push_back(block[k]);
                            }
                            sort(shape.begin(), shape.end());
                            if (shape == comp) {
                                is_L = true;
                                break;
                            }
                        }
                    }
                    if (!is_L) {
                        ok = false;
                        break;
                    }
                    
                    // Check that no other * touches this L-shape by edge or corner
                    // We already collected the component, so if there were adjacent * they would be in the same component.
                    // Since component size is exactly 3, no other * touches it.
                    // So this check is automatically satisfied.
                }
            }
        }
        
        cout << (ok ? "YES" : "NO") << '\n';
    }
    
    return 0;
}
