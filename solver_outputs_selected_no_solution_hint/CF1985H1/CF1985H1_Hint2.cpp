#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, m;
    cin >> n >> m;
    vector<string> grid(n);
    for (int i = 0; i < n; ++i) {
        cin >> grid[i];
    }

    vector<vector<int>> comp_id(n, vector<int>(m, -1));
    vector<int> comp_size;
    int cur_id = 0;

    const int dx[] = {1, -1, 0, 0};
    const int dy[] = {0, 0, 1, -1};

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] == '#' && comp_id[i][j] == -1) {
                int sz = 0;
                queue<pair<int,int>> q;
                q.push({i, j});
                comp_id[i][j] = cur_id;
                while (!q.empty()) {
                    auto [x, y] = q.front(); q.pop();
                    sz++;
                    for (int d = 0; d < 4; ++d) {
                        int nx = x + dx[d];
                        int ny = y + dy[d];
                        if (nx >= 0 && nx < n && ny >= 0 && ny < m &&
                            grid[nx][ny] == '#' && comp_id[nx][ny] == -1) {
                            comp_id[nx][ny] = cur_id;
                            q.push({nx, ny});
                        }
                    }
                }
                comp_size.push_back(sz);
                cur_id++;
            }
        }
    }

    int ans = 0;

    // Try rows
    for (int i = 0; i < n; ++i) {
        int total = 0;
        unordered_set<int> seen;
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] == '#') {
                int id = comp_id[i][j];
                if (!seen.count(id)) {
                    seen.insert(id);
                    total += comp_size[id];
                }
            } else {
                total++; // will become '#'
            }
            // check above
            if (i > 0 && grid[i-1][j] == '#') {
                int id = comp_id[i-1][j];
                if (!seen.count(id)) {
                    seen.insert(id);
                    total += comp_size[id];
                }
            }
            // check below
            if (i+1 < n && grid[i+1][j] == '#') {
                int id = comp_id[i+1][j];
                if (!seen.count(id)) {
                    seen.insert(id);
                    total += comp_size[id];
                }
            }
        }
        ans = max(ans, total);
    }

    // Try columns
    for (int j = 0; j < m; ++j) {
        int total = 0;
        unordered_set<int> seen;
        for (int i = 0; i < n; ++i) {
            if (grid[i][j] == '#') {
                int id = comp_id[i][j];
                if (!seen.count(id)) {
                    seen.insert(id);
                    total += comp_size[id];
                }
            } else {
                total++;
            }
            // check left
            if (j > 0 && grid[i][j-1] == '#') {
                int id = comp_id[i][j-1];
                if (!seen.count(id)) {
                    seen.insert(id);
                    total += comp_size[id];
                }
            }
            // check right
            if (j+1 < m && grid[i][j+1] == '#') {
                int id = comp_id[i][j+1];
                if (!seen.count(id)) {
                    seen.insert(id);
                    total += comp_size[id];
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
