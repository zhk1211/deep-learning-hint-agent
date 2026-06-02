#include <bits/stdc++.h>
using namespace std;

const int dx[] = {1, -1, 0, 0};
const int dy[] = {0, 0, 1, -1};

void solve() {
    int n, m;
    cin >> n >> m;
    vector<string> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    // If there are no good persons, we can just block everything around bad persons
    // but the problem says "all good people are able to escape" - if no good, vacuously true.
    // However, we must ensure no bad person can escape.
    // We can just block all empty cells around bad persons and check if any bad can reach exit.
    // But we also need to ensure good persons can reach exit after blocking.
    // Standard solution: block all empty cells adjacent to bad persons, then check if any good person
    // can reach exit and no bad person can reach exit.

    // Step 1: Block cells adjacent to bad persons
    bool possible = true;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (a[i][j] == 'B') {
                for (int k = 0; k < 4; ++k) {
                    int ni = i + dx[k];
                    int nj = j + dy[k];
                    if (ni >= 0 && ni < n && nj >= 0 && nj < m) {
                        if (a[ni][nj] == '.') {
                            a[ni][nj] = '#';
                        } else if (a[ni][nj] == 'G') {
                            // Good person adjacent to bad person -> impossible to separate
                            possible = false;
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

    // Step 2: Check if any good person can reach exit (n-1, m-1)
    // Use BFS from exit, but only if exit is not blocked
    if (a[n-1][m-1] == '#') {
        // If exit is blocked, no one can escape
        // Check if there are any good persons
        bool hasGood = false;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                if (a[i][j] == 'G') {
                    hasGood = true;
                    break;
                }
            }
            if (hasGood) break;
        }
        if (hasGood) {
            cout << "No\n";
        } else {
            cout << "Yes\n";
        }
        return;
    }

    vector<vector<bool>> vis(n, vector<bool>(m, false));
    queue<pair<int,int>> q;
    q.push({n-1, m-1});
    vis[n-1][m-1] = true;

    while (!q.empty()) {
        auto [x, y] = q.front(); q.pop();
        for (int k = 0; k < 4; ++k) {
            int nx = x + dx[k];
            int ny = y + dy[k];
            if (nx >= 0 && nx < n && ny >= 0 && ny < m && !vis[nx][ny] && a[nx][ny] != '#') {
                vis[nx][ny] = true;
                q.push({nx, ny});
            }
        }
    }

    // Check all good persons are reachable
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (a[i][j] == 'G' && !vis[i][j]) {
                cout << "No\n";
                return;
            }
        }
    }

    // Check no bad person is reachable
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (a[i][j] == 'B' && vis[i][j]) {
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
