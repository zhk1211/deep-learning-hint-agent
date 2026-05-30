// Hint1
#include <bits/stdc++.h>
using namespace std;

void solve() {
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
        if (x == y) return;
        if (sz[x] < sz[y]) swap(x, y);
        parent[y] = x;
        sz[x] += sz[y];
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
    vector<int> comp_id(n * m, -1);
    vector<int> comp_size;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] == '#') {
                int id = find(i * m + j);
                if (comp_id[id] == -1) {
                    comp_id[id] = comp_size.size();
                    comp_size.push_back(sz[id]);
                }
            }
        }
    }

    int ans = 0;

    // Check rows
    for (int i = 0; i < n; ++i) {
        int total = 0;
        unordered_set<int> seen_comps;
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] == '#') {
                int id = find(i * m + j);
                int cid = comp_id[id];
                if (seen_comps.insert(cid).second) {
                    total += comp_size[cid];
                }
            } else {
                total++; // will be filled
            }
        }
        // Add adjacent rows (i-1 and i+1) components that touch this row
        for (int di = -1; di <= 1; di += 2) {
            int ni = i + di;
            if (ni >= 0 && ni < n) {
                for (int j = 0; j < m; ++j) {
                    if (grid[ni][j] == '#') {
                        int id = find(ni * m + j);
                        int cid = comp_id[id];
                        if (seen_comps.insert(cid).second) {
                            total += comp_size[cid];
                        }
                    }
                }
            }
        }
        ans = max(ans, total);
    }

    // Check columns
    for (int j = 0; j < m; ++j) {
        int total = 0;
        unordered_set<int> seen_comps;
        for (int i = 0; i < n; ++i) {
            if (grid[i][j] == '#') {
                int id = find(i * m + j);
                int cid = comp_id[id];
                if (seen_comps.insert(cid).second) {
                    total += comp_size[cid];
                }
            } else {
                total++; // will be filled
            }
        }
        // Add adjacent columns (j-1 and j+1) components that touch this column
        for (int dj = -1; dj <= 1; dj += 2) {
            int nj = j + dj;
            if (nj >= 0 && nj < m) {
                for (int i = 0; i < n; ++i) {
                    if (grid[i][nj] == '#') {
                        int id = find(i * m + nj);
                        int cid = comp_id[id];
                        if (seen_comps.insert(cid).second) {
                            total += comp_size[cid];
                        }
                    }
                }
            }
        }
        ans = max(ans, total);
    }

    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}
