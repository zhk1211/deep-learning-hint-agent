// Solution
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
    vector<int> comp_min_row, comp_max_row;
    vector<int> comp_min_col, comp_max_col;

    const int dx[] = {1, -1, 0, 0};
    const int dy[] = {0, 0, 1, -1};

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] == '#' && comp_id[i][j] == -1) {
                int id = comp_size.size();
                comp_size.push_back(0);
                comp_min_row.push_back(i);
                comp_max_row.push_back(i);
                comp_min_col.push_back(j);
                comp_max_col.push_back(j);

                queue<pair<int,int>> q;
                q.push({i, j});
                comp_id[i][j] = id;
                comp_size[id]++;

                while (!q.empty()) {
                    auto [x, y] = q.front(); q.pop();
                    for (int d = 0; d < 4; ++d) {
                        int nx = x + dx[d];
                        int ny = y + dy[d];
                        if (nx >= 0 && nx < n && ny >= 0 && ny < m &&
                            grid[nx][ny] == '#' && comp_id[nx][ny] == -1) {
                            comp_id[nx][ny] = id;
                            comp_size[id]++;
                            comp_min_row[id] = min(comp_min_row[id], nx);
                            comp_max_row[id] = max(comp_max_row[id], nx);
                            comp_min_col[id] = min(comp_min_col[id], ny);
                            comp_max_col[id] = max(comp_max_col[id], ny);
                            q.push({nx, ny});
                        }
                    }
                }
            }
        }
    }

    int ans = 0;

    // Rows
    vector<int> F_row(n, 0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] == '.') F_row[i]++;
        }
    }

    vector<long long> R_row(n + 2, 0);
    for (int id = 0; id < (int)comp_size.size(); ++id) {
        int rmin = comp_min_row[id];
        int rmax = comp_max_row[id];
        int L = max(0, rmin - 1);
        int R = min(n - 1, rmax + 1);
        R_row[L] += comp_size[id];
        R_row[R + 1] -= comp_size[id];
    }
    for (int i = 1; i < n; ++i) {
        R_row[i] += R_row[i - 1];
    }

    for (int i = 0; i < n; ++i) {
        long long cur = F_row[i] + R_row[i];
        ans = max(ans, (int)cur);
    }

    // Columns
    vector<int> F_col(m, 0);
    for (int j = 0; j < m; ++j) {
        for (int i = 0; i < n; ++i) {
            if (grid[i][j] == '.') F_col[j]++;
        }
    }

    vector<long long> R_col(m + 2, 0);
    for (int id = 0; id < (int)comp_size.size(); ++id) {
        int cmin = comp_min_col[id];
        int cmax = comp_max_col[id];
        int L = max(0, cmin - 1);
        int R = min(m - 1, cmax + 1);
        R_col[L] += comp_size[id];
        R_col[R + 1] -= comp_size[id];
    }
    for (int j = 1; j < m; ++j) {
        R_col[j] += R_col[j - 1];
    }

    for (int j = 0; j < m; ++j) {
        long long cur = F_col[j] + R_col[j];
        ans = max(ans, (int)cur);
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
