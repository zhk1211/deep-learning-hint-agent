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

        vector<vector<int>> comp_id(n, vector<int>(m, -1));
        vector<int> comp_size;
        int comp_cnt = 0;

        const int dx[] = {1, -1, 0, 0};
        const int dy[] = {0, 0, 1, -1};

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                if (grid[i][j] == '#' && comp_id[i][j] == -1) {
                    int sz = 0;
                    queue<pair<int, int>> q;
                    q.push({i, j});
                    comp_id[i][j] = comp_cnt;
                    while (!q.empty()) {
                        auto [x, y] = q.front();
                        q.pop();
                        ++sz;
                        for (int k = 0; k < 4; ++k) {
                            int nx = x + dx[k];
                            int ny = y + dy[k];
                            if (nx >= 0 && nx < n && ny >= 0 && ny < m &&
                                grid[nx][ny] == '#' && comp_id[nx][ny] == -1) {
                                comp_id[nx][ny] = comp_cnt;
                                q.push({nx, ny});
                            }
                        }
                    }
                    comp_size.push_back(sz);
                    ++comp_cnt;
                }
            }
        }

        int ans = 0;

        // Try rows
        for (int i = 0; i < n; ++i) {
            int total = 0;
            unordered_set<int> seen_comps;
            for (int j = 0; j < m; ++j) {
                if (grid[i][j] == '#') {
                    int cid = comp_id[i][j];
                    if (seen_comps.insert(cid).second) {
                        total += comp_size[cid];
                    }
                } else {
                    ++total; // this cell becomes '#'
                }
                // check neighbors above and below
                if (i > 0 && grid[i-1][j] == '#') {
                    int cid = comp_id[i-1][j];
                    if (seen_comps.insert(cid).second) {
                        total += comp_size[cid];
                    }
                }
                if (i + 1 < n && grid[i+1][j] == '#') {
                    int cid = comp_id[i+1][j];
                    if (seen_comps.insert(cid).second) {
                        total += comp_size[cid];
                    }
                }
            }
            ans = max(ans, total);
        }

        // Try columns
        for (int j = 0; j < m; ++j) {
            int total = 0;
            unordered_set<int> seen_comps;
            for (int i = 0; i < n; ++i) {
                if (grid[i][j] == '#') {
                    int cid = comp_id[i][j];
                    if (seen_comps.insert(cid).second) {
                        total += comp_size[cid];
                    }
                } else {
                    ++total;
                }
                // check neighbors left and right
                if (j > 0 && grid[i][j-1] == '#') {
                    int cid = comp_id[i][j-1];
                    if (seen_comps.insert(cid).second) {
                        total += comp_size[cid];
                    }
                }
                if (j + 1 < m && grid[i][j+1] == '#') {
                    int cid = comp_id[i][j+1];
                    if (seen_comps.insert(cid).second) {
                        total += comp_size[cid];
                    }
                }
            }
            ans = max(ans, total);
        }

        cout << ans << '\n';
    }

    return 0;
}
