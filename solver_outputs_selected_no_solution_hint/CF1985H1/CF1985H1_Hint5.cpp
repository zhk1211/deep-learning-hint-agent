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

    const int dx[] = {1, -1, 0, 0};
    const int dy[] = {0, 0, 1, -1};

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] == '#') {
                for (int d = 0; d < 4; ++d) {
                    int ni = i + dx[d];
                    int nj = j + dy[d];
                    if (ni >= 0 && ni < n && nj >= 0 && nj < m && grid[ni][nj] == '#') {
                        unite(i * m + j, ni * m + nj);
                    }
                }
            }
        }
    }

    // Precompute row and column counts of '.'
    vector<int> row_empty(n, 0);
    vector<int> col_empty(m, 0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] == '.') {
                row_empty[i]++;
                col_empty[j]++;
            }
        }
    }

    int ans = 0;

    // Try each row
    for (int r = 0; r < n; ++r) {
        unordered_set<int> comps;
        int total = row_empty[r]; // new cells from this row
        // also consider adjacent rows to connect components
        for (int dr = -1; dr <= 1; ++dr) {
            int nr = r + dr;
            if (nr < 0 || nr >= n) continue;
            for (int c = 0; c < m; ++c) {
                if (grid[nr][c] == '#') {
                    int id = find(nr * m + c);
                    comps.insert(id);
                }
            }
        }
        for (int id : comps) {
            total += sz[id];
        }
        ans = max(ans, total);
    }

    // Try each column
    for (int c = 0; c < m; ++c) {
        unordered_set<int> comps;
        int total = col_empty[c];
        for (int dc = -1; dc <= 1; ++dc) {
            int nc = c + dc;
            if (nc < 0 || nc >= m) continue;
            for (int r = 0; r < n; ++r) {
                if (grid[r][nc] == '#') {
                    int id = find(r * m + nc);
                    comps.insert(id);
                }
            }
        }
        for (int id : comps) {
            total += sz[id];
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
