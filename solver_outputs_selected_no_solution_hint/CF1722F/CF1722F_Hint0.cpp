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
        for (int i = 0; i < n; i++) {
            cin >> grid[i];
        }
        
        vector<vector<bool>> visited(n, vector<bool>(m, false));
        bool ok = true;
        
        for (int i = 0; i < n && ok; i++) {
            for (int j = 0; j < m && ok; j++) {
                if (grid[i][j] == '*' && !visited[i][j]) {
                    // collect all cells of this component
                    vector<pair<int,int>> comp;
                    queue<pair<int,int>> q;
                    q.push({i, j});
                    visited[i][j] = true;
                    
                    while (!q.empty()) {
                        auto [x, y] = q.front(); q.pop();
                        comp.push_back({x, y});
                        // 8-directional connectivity to find the whole shape
                        for (int dx = -1; dx <= 1; dx++) {
                            for (int dy = -1; dy <= 1; dy++) {
                                if (dx == 0 && dy == 0) continue;
                                int nx = x + dx, ny = y + dy;
                                if (nx >= 0 && nx < n && ny >= 0 && ny < m && 
                                    grid[nx][ny] == '*' && !visited[nx][ny]) {
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
                    
                    // Check if the 3 cells form an L-shape (2x2 block with one missing)
                    // Find bounding box
                    int minx = n, maxx = -1, miny = m, maxy = -1;
                    for (auto [x, y] : comp) {
                        minx = min(minx, x);
                        maxx = max(maxx, x);
                        miny = min(miny, y);
                        maxy = max(maxy, y);
                    }
                    
                    if (maxx - minx != 1 || maxy - miny != 1) {
                        ok = false;
                        break;
                    }
                    
                    // Check that exactly 3 cells in the 2x2 are '*'
                    int cnt = 0;
                    for (int dx = 0; dx <= 1; dx++) {
                        for (int dy = 0; dy <= 1; dy++) {
                            if (grid[minx+dx][miny+dy] == '*') cnt++;
                        }
                    }
                    if (cnt != 3) {
                        ok = false;
                        break;
                    }
                    
                    // Now check that no other L-shape touches this one by edge or corner
                    // We already know the 2x2 block. Check all 12 surrounding cells.
                    for (int dx = -1; dx <= 2; dx++) {
                        for (int dy = -1; dy <= 2; dy++) {
                            int nx = minx + dx, ny = miny + dy;
                            if (nx >= 0 && nx < n && ny >= 0 && ny < m) {
                                // if it's inside the 2x2 block, skip
                                if (dx >= 0 && dx <= 1 && dy >= 0 && dy <= 1) continue;
                                if (grid[nx][ny] == '*') {
                                    ok = false;
                                    break;
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
