#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<string> grid(n);
        for (int i = 0; i < n; ++i) {
            cin >> grid[i];
        }

        vector<vector<int>> diff(n + 2, vector<int>(n + 2, 0));
        vector<vector<int>> diag(n + 2, vector<int>(n + 2, 0));
        vector<vector<int>> rdiag(n + 2, vector<int>(n + 2, 0));

        int ans = 0;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                diff[i][j] += (i > 0 ? diff[i - 1][j] : 0);
                diag[i][j] += (i > 0 && j > 0 ? diag[i - 1][j - 1] : 0);
                rdiag[i][j] += (i > 0 && j + 1 < n ? rdiag[i - 1][j + 1] : 0);

                int cur = (grid[i][j] - '0') ^ ((diff[i][j] + diag[i][j] + rdiag[i][j]) & 1);
                if (cur) {
                    ++ans;
                    diff[i][j] ^= 1;
                    diag[i][j] ^= 1;
                    rdiag[i][j] ^= 1;
                }

                if (i + 1 < n) {
                    diff[i + 1][j] += diff[i][j];
                    if (j > 0) diag[i + 1][j - 1] += diag[i][j];
                    if (j + 1 < n) rdiag[i + 1][j + 1] += rdiag[i][j];
                }
            }
        }
        cout << ans << '\n';
    }
    return 0;
}
