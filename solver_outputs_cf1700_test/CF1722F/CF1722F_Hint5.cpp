// Hint5
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
        
        vector<vector<int>> comp(n, vector<int>(m, -1));
        int comp_id = 0;
        bool ok = true;
        
        const int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
        const int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};
        
        for (int i = 0; i < n && ok; i++) {
            for (int j = 0; j < m && ok; j++) {
                if (grid[i][j] != '*') continue;
                
                // Count shaded neighbors (8-directional)
                int cnt = 0;
                for (int d = 0; d < 8; d++) {
                    int ni = i + dx[d];
                    int nj = j + dy[d];
                    if (ni >= 0 && ni < n && nj >= 0 && nj < m && grid[ni][nj] == '*') {
                        cnt++;
                    }
                }
                
                if (cnt > 2) {
                    ok = false;
                    break;
                }
                
                if (cnt == 2) {
                    // Find the two neighbors
                    vector<pair<int,int>> neigh;
                    for (int d = 0; d < 8; d++) {
                        int ni = i + dx[d];
                        int nj = j + dy[d];
                        if (ni >= 0 && ni < n && nj >= 0 && nj < m && grid[ni][nj] == '*') {
                            neigh.emplace_back(ni, nj);
                        }
                    }
                    
                    // Check if the three cells form an L-shape
                    // They must be exactly 3 cells, and the two neighbors must be adjacent to each other
                    // Also check that the two neighbors are not collinear with the center
                    int r1 = neigh[0].first, c1 = neigh[0].second;
                    int r2 = neigh[1].first, c2 = neigh[1].second;
                    
                    // Check if they are adjacent (8-directional)
                    bool adj = false;
                    for (int d = 0; d < 8; d++) {
                        if (r1 + dx[d] == r2 && c1 + dy[d] == c2) {
                            adj = true;
                            break;
                        }
                    }
                    
                    if (!adj) {
                        ok = false;
                        break;
                    }
                    
                    // Check that they are not in a straight line (i.e., not collinear)
                    if ((i == r1 && i == r2) || (j == c1 && j == c2)) {
                        ok = false;
                        break;
                    }
                    
                    // Mark the three cells with a unique component id
                    if (comp[i][j] != -1 || comp[r1][c1] != -1 || comp[r2][c2] != -1) {
                        ok = false;
                        break;
                    }
                    comp[i][j] = comp_id;
                    comp[r1][c1] = comp_id;
                    comp[r2][c2] = comp_id;
                    comp_id++;
                }
            }
        }
        
        // Check for isolated cells (cnt < 2) that are not part of any L-shape
        for (int i = 0; i < n && ok; i++) {
            for (int j = 0; j < m && ok; j++) {
                if (grid[i][j] == '*' && comp[i][j] == -1) {
                    ok = false;
                }
            }
        }
        
        // Check that no two different L-shapes are adjacent by edge or corner
        for (int i = 0; i < n && ok; i++) {
            for (int j = 0; j < m && ok; j++) {
                if (grid[i][j] != '*') continue;
                int cid = comp[i][j];
                for (int d = 0; d < 8; d++) {
                    int ni = i + dx[d];
                    int nj = j + dy[d];
                    if (ni >= 0 && ni < n && nj >= 0 && nj < m && grid[ni][nj] == '*') {
                        if (comp[ni][nj] != cid) {
                            ok = false;
                            break;
                        }
                    }
                }
            }
        }
        
        cout << (ok ? "YES" : "NO") << '\n';
    }
    
    return 0;
}
