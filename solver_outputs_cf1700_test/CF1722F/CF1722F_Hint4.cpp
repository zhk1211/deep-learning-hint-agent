// Hint4
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

        vector<vector<int>> comp(n, vector<int>(m, -1));
        int comp_id = 0;
        bool ok = true;

        // Directions for 8 neighbors
        const int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
        const int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

        for (int i = 0; i < n && ok; ++i) {
            for (int j = 0; j < m && ok; ++j) {
                if (grid[i][j] != '*' || comp[i][j] != -1) continue;

                // Find all '*' cells in the 3x3 neighborhood that are not yet assigned
                vector<pair<int,int>> star_cells;
                for (int k = 0; k < 8; ++k) {
                    int ni = i + dx[k];
                    int nj = j + dy[k];
                    if (ni >= 0 && ni < n && nj >= 0 && nj < m && grid[ni][nj] == '*' && comp[ni][nj] == -1) {
                        star_cells.emplace_back(ni, nj);
                    }
                }

                // The current cell itself is '*'
                star_cells.emplace_back(i, j);

                // Remove duplicates (current cell might be added twice if it's in the 8-neighbor list? No, because we only added unassigned neighbors)
                // But current cell is unassigned, so it's not in the list from the loop.
                // So star_cells has current cell + its unassigned '*' neighbors.

                // We need exactly 3 cells to form an L-shape
                if (star_cells.size() != 3) {
                    ok = false;
                    break;
                }

                // Check if these 3 cells form an L-shape (2x2 block with one missing)
                // They must be within a 2x2 area.
                int min_r = n, max_r = -1, min_c = m, max_c = -1;
                for (auto [r, c] : star_cells) {
                    min_r = min(min_r, r);
                    max_r = max(max_r, r);
                    min_c = min(min_c, c);
                    max_c = max(max_c, c);
                }
                if (max_r - min_r > 1 || max_c - min_c > 1) {
                    ok = false;
                    break;
                }

                // Check that no two L-shapes touch by edge or corner:
                // For each of the 3 cells, check its 8 neighbors. If any neighbor is '*' and belongs to a different component (already assigned), it's invalid.
                for (auto [r, c] : star_cells) {
                    for (int k = 0; k < 8; ++k) {
                        int nr = r + dx[k];
                        int nc = c + dy[k];
                        if (nr >= 0 && nr < n && nc >= 0 && nc < m && grid[nr][nc] == '*') {
                            if (comp[nr][nc] != -1 && comp[nr][nc] != comp_id) {
                                ok = false;
                                break;
                            }
                        }
                    }
                    if (!ok) break;
                }
                if (!ok) break;

                // Assign component id
                for (auto [r, c] : star_cells) {
                    comp[r][c] = comp_id;
                }
                ++comp_id;
            }
        }

        // Final check: all '*' cells must be assigned
        for (int i = 0; i < n && ok; ++i) {
            for (int j = 0; j < m && ok; ++j) {
                if (grid[i][j] == '*' && comp[i][j] == -1) {
                    ok = false;
                }
            }
        }

        cout << (ok ? "YES" : "NO") << '\n';
    }

    return 0;
}
