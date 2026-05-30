#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, m;
    cin >> n >> m;
    vector<string> g(n);
    for (int i = 0; i < n; ++i) {
        cin >> g[i];
    }

    vector<vector<int>> ids(n, vector<int>(m, 0));
    int nextId = 1;

    // Identify elbows and mark L-shapes
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (g[i][j] != '*') continue;

            bool up = (i > 0 && g[i-1][j] == '*');
            bool down = (i < n-1 && g[i+1][j] == '*');
            bool left = (j > 0 && g[i][j-1] == '*');
            bool right = (j < m-1 && g[i][j+1] == '*');
            int cnt = up + down + left + right;

            if (cnt > 2) {
                cout << "NO\n";
                return;
            }
            if (cnt == 2) {
                // opposite neighbours = straight line, invalid L-shape
                if ((up && down) || (left && right)) {
                    cout << "NO\n";
                    return;
                }

                // collect the three cells of the L-shape
                vector<pair<int,int>> cells;
                cells.push_back({i, j});
                if (up) cells.push_back({i-1, j});
                if (down) cells.push_back({i+1, j});
                if (left) cells.push_back({i, j-1});
                if (right) cells.push_back({i, j+1});

                // check overlap with another L-shape
                for (auto &p : cells) {
                    if (ids[p.first][p.second] != 0) {
                        cout << "NO\n";
                        return;
                    }
                }

                // mark the L-shape
                for (auto &p : cells) {
                    ids[p.first][p.second] = nextId;
                }
                ++nextId;
            }
        }
    }

    // every shaded cell must belong to some L-shape
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (g[i][j] == '*' && ids[i][j] == 0) {
                cout << "NO\n";
                return;
            }
        }
    }

    // L-shapes must not touch by edge or corner
    const int di[] = {-1, -1, -1,  0, 0,  1, 1, 1};
    const int dj[] = {-1,  0,  1, -1, 1, -1, 0, 1};

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (g[i][j] != '*') continue;

            for (int k = 0; k < 8; ++k) {
                int ni = i + di[k];
                int nj = j + dj[k];
                if (ni >= 0 && ni < n && nj >= 0 && nj < m && g[ni][nj] == '*') {
                    if (ids[i][j] != ids[ni][nj]) {
                        cout << "NO\n";
                        return;
                    }
                }
            }
        }
    }

    cout << "YES\n";
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
