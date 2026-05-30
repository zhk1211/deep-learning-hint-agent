// Hint3
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
        
        const int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
        const int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};
        
        for (int i = 0; i < n && ok; ++i) {
            for (int j = 0; j < m && ok; ++j) {
                if (grid[i][j] == '*' && !visited[i][j]) {
                    // Find all cells in this component
                    vector<pair<int,int>> comp;
                    queue<pair<int,int>> q;
                    q.push({i, j});
                    visited[i][j] = true;
                    
                    while (!q.empty()) {
                        auto [x, y] = q.front(); q.pop();
                        comp.push_back({x, y});
                        for (int d = 0; d < 8; ++d) {
                            int nx = x + dx[d];
                            int ny = y + dy[d];
                            if (nx >= 0 && nx < n && ny >= 0 && ny < m && grid[nx][ny] == '*' && !visited[nx][ny]) {
                                visited[nx][ny] = true;
                                q.push({nx, ny});
                            }
                        }
                    }
                    
                    if (comp.size() != 3) {
                        ok = false;
                        break;
                    }
                    
                    // Check if these 3 cells form an L-shape
                    // An L-shape has exactly one cell with 2 neighbors (the elbow)
                    // and two cells with 1 neighbor (the ends)
                    // Also, the cells must be in a 2x2 bounding box
                    int min_x = n, max_x = -1, min_y = m, max_y = -1;
                    for (auto [x, y] : comp) {
                        min_x = min(min_x, x);
                        max_x = max(max_x, x);
                        min_y = min(min_y, y);
                        max_y = max(max_y, y);
                    }
                    if (max_x - min_x > 1 || max_y - min_y > 1) {
                        ok = false;
                        break;
                    }
                    
                    // Count neighbors within the component
                    int count2 = 0, count1 = 0;
                    for (auto [x, y] : comp) {
                        int neigh = 0;
                        for (int d = 0; d < 4; ++d) { // only 4-directional adjacency
                            int nx = x + dx[2*d+1]; // using dx[1], dx[3], dx[5], dx[7] which are up, right, down, left
                            int ny = y + dy[2*d+1];
                            if (nx >= 0 && nx < n && ny >= 0 && ny < m && grid[nx][ny] == '*') {
                                // check if this neighbor is in the component
                                for (auto [cx, cy] : comp) {
                                    if (cx == nx && cy == ny) {
                                        neigh++;
                                        break;
                                    }
                                }
                            }
                        }
                        if (neigh == 2) count2++;
                        else if (neigh == 1) count1++;
                    }
                    if (count2 != 1 || count1 != 2) {
                        ok = false;
                        break;
                    }
                    
                    // Check that no other star touches this component by edge or corner
                    // Actually, the BFS already grouped all 8-connected stars, so if there were
                    // any touching stars, they would be in the same component.
                    // Since we already checked size == 3, this is automatically satisfied.
                }
            }
        }
        
        cout << (ok ? "YES" : "NO") << '\n';
    }
    
    return 0;
}
