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
        
        // DSU for connected components
        vector<int> parent(n * m);
        vector<int> sz(n * m, 1);
        iota(parent.begin(), parent.end(), 0);
        
        function<int(int)> find = [&](int x) {
            return parent[x] == x ? x : parent[x] = find(parent[x]);
        };
        
        auto unite = [&](int x, int y) {
            x = find(x);
            y = find(y);
            if (x != y) {
                if (sz[x] < sz[y]) swap(x, y);
                parent[y] = x;
                sz[x] += sz[y];
            }
        };
        
        const int dx[] = {0, 0, 1, -1};
        const int dy[] = {1, -1, 0, 0};
        
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                if (grid[i][j] == '#') {
                    for (int k = 0; k < 4; ++k) {
                        int ni = i + dx[k];
                        int nj = j + dy[k];
                        if (ni >= 0 && ni < n && nj >= 0 && nj < m && grid[ni][nj] == '#') {
                            unite(i * m + j, ni * m + nj);
                        }
                    }
                }
            }
        }
        
        // Precompute component sizes for each cell
        vector<int> comp_size(n * m, 0);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                if (grid[i][j] == '#') {
                    comp_size[i * m + j] = sz[find(i * m + j)];
                }
            }
        }
        
        int ans = 0;
        
        // Try each row
        for (int i = 0; i < n; ++i) {
            int total = 0;
            unordered_set<int> seen;
            for (int j = 0; j < m; ++j) {
                if (grid[i][j] == '#') {
                    int root = find(i * m + j);
                    if (seen.insert(root).second) {
                        total += sz[root];
                    }
                } else {
                    total += 1; // this cell becomes '#'
                }
                // check adjacent rows
                if (i > 0 && grid[i-1][j] == '#') {
                    int root = find((i-1) * m + j);
                    if (seen.insert(root).second) {
                        total += sz[root];
                    }
                }
                if (i + 1 < n && grid[i+1][j] == '#') {
                    int root = find((i+1) * m + j);
                    if (seen.insert(root).second) {
                        total += sz[root];
                    }
                }
            }
            ans = max(ans, total);
        }
        
        // Try each column
        for (int j = 0; j < m; ++j) {
            int total = 0;
            unordered_set<int> seen;
            for (int i = 0; i < n; ++i) {
                if (grid[i][j] == '#') {
                    int root = find(i * m + j);
                    if (seen.insert(root).second) {
                        total += sz[root];
                    }
                } else {
                    total += 1; // this cell becomes '#'
                }
                // check adjacent columns
                if (j > 0 && grid[i][j-1] == '#') {
                    int root = find(i * m + (j-1));
                    if (seen.insert(root).second) {
                        total += sz[root];
                    }
                }
                if (j + 1 < m && grid[i][j+1] == '#') {
                    int root = find(i * m + (j+1));
                    if (seen.insert(root).second) {
                        total += sz[root];
                    }
                }
            }
            ans = max(ans, total);
        }
        
        cout << ans << '\n';
    }
    
    return 0;
}
