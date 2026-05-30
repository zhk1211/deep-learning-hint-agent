// Hint2
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

        // Directions for 8 neighbors
        const int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
        const int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

        for (int i = 0; i < n && ok; ++i) {
            for (int j = 0; j < m && ok; ++j) {
                if (grid[i][j] == '*' && !visited[i][j]) {
                    // Collect all connected '*' cells (4-directionally)
                    vector<pair<int, int>> comp;
                    queue<pair<int, int>> q;
                    q.push({i, j});
                    visited[i][j] = true;
                    while (!q.empty()) {
                        auto [x, y] = q.front(); q.pop();
                        comp.push_back({x, y});
                        // 4-directional connectivity
                        for (int d = 0; d < 4; ++d) {
                            int nx = x + dx[d * 2 + 1]; // dx[1], dx[3], dx[5], dx[7] are up, right, down, left
                            int ny = y + dy[d * 2 + 1];
                            if (nx >= 0 && nx < n && ny >= 0 && ny < m && grid[nx][ny] == '*' && !visited[nx][ny]) {
                                visited[nx][ny] = true;
                                q.push({nx, ny});
                            }
                        }
                    }

                    // An L-shape must have exactly 3 cells
                    if (comp.size() != 3) {
                        ok = false;
                        break;
                    }

                    // Check that the 3 cells form an L-shape (2x2 square with one missing)
                    // Find bounding box
                    int min_x = n, max_x = -1, min_y = m, max_y = -1;
                    for (auto [x, y] : comp) {
                        min_x = min(min_x, x);
                        max_x = max(max_x, x);
                        min_y = min(min_y, y);
                        max_y = max(max_y, y);
                    }
                    if (max_x - min_x != 1 || max_y - min_y != 1) {
                        ok = false;
                        break;
                    }

                    // Check that exactly 3 cells in the 2x2 are '*'
                    int cnt = 0;
                    for (int dxb = 0; dxb <= 1; ++dxb) {
                        for (int dyb = 0; dyb <= 1; ++dyb) {
                            if (grid[min_x + dxb][min_y + dyb] == '*') cnt++;
                        }
                    }
                    if (cnt != 3) {
                        ok = false;
                        break;
                    }

                    // Now check that no cell of this L-shape touches another '*' from a different component
                    // by edge or corner. Since we already isolated the component, we just need to check
                    // that no neighbor (8-directional) of any cell in comp is '*' and not in comp.
                    for (auto [x, y] : comp) {
                        for (int d = 0; d < 8; ++d) {
                            int nx = x + dx[d];
                            int ny = y + dy[d];
                            if (nx >= 0 && nx < n && ny >= 0 && ny < m && grid[nx][ny] == '*') {
                                // Check if (nx, ny) is in comp
                                bool in_comp = false;
                                for (auto [cx, cy] : comp) {
                                    if (cx == nx && cy == ny) {
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
                }
            }
        }

        cout << (ok ? "YES" : "NO") << '\n';
    }
    return 0;
}
