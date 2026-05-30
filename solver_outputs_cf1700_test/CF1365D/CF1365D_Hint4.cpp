// Hint4
#include <bits/stdc++.h>
using namespace std;

int n, m;
vector<string> grid;
vector<vector<bool>> visited;
int dx[] = {1, -1, 0, 0};
int dy[] = {0, 0, 1, -1};

bool valid(int x, int y) {
    return x >= 0 && x < n && y >= 0 && y < m && grid[x][y] != '#';
}

void dfs(int x, int y) {
    visited[x][y] = true;
    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i], ny = y + dy[i];
        if (valid(nx, ny) && !visited[nx][ny]) {
            dfs(nx, ny);
        }
    }
}

void solve() {
    cin >> n >> m;
    grid.resize(n);
    for (int i = 0; i < n; i++) {
        cin >> grid[i];
    }

    // Block cells adjacent to bad people
    bool possible = true;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == 'B') {
                for (int k = 0; k < 4; k++) {
                    int ni = i + dx[k], nj = j + dy[k];
                    if (ni >= 0 && ni < n && nj >= 0 && nj < m) {
                        if (grid[ni][nj] == 'G') {
                            possible = false;
                        } else if (grid[ni][nj] == '.') {
                            grid[ni][nj] = '#';
                        }
                    }
                }
            }
        }
    }

    if (!possible) {
        cout << "No\n";
        return;
    }

    // Check if all good people can reach (n-1, m-1)
    visited.assign(n, vector<bool>(m, false));
    if (grid[n-1][m-1] != '#') {
        dfs(n-1, m-1);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == 'G' && !visited[i][j]) {
                cout << "No\n";
                return;
            }
            if (grid[i][j] == 'B' && visited[i][j]) {
                cout << "No\n";
                return;
            }
        }
    }

    cout << "Yes\n";
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
