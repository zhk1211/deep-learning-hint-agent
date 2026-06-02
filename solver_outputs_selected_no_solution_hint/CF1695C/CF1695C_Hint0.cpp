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
        vector<vector<int>> a(n, vector<int>(m));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                cin >> a[i][j];
            }
        }

        // If the path length (n + m - 1) is odd, sum can't be 0
        if ((n + m - 1) % 2 != 0) {
            cout << "NO\n";
            continue;
        }

        // dp_min[i][j] = minimum possible sum to reach (i,j)
        // dp_max[i][j] = maximum possible sum to reach (i,j)
        vector<vector<int>> dp_min(n, vector<int>(m));
        vector<vector<int>> dp_max(n, vector<int>(m));

        dp_min[0][0] = dp_max[0][0] = a[0][0];

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                if (i == 0 && j == 0) continue;
                int mn = INT_MAX, mx = INT_MIN;
                if (i > 0) {
                    mn = min(mn, dp_min[i-1][j]);
                    mx = max(mx, dp_max[i-1][j]);
                }
                if (j > 0) {
                    mn = min(mn, dp_min[i][j-1]);
                    mx = max(mx, dp_max[i][j-1]);
                }
                dp_min[i][j] = mn + a[i][j];
                dp_max[i][j] = mx + a[i][j];
            }
        }

        if (dp_min[n-1][m-1] <= 0 && 0 <= dp_max[n-1][m-1]) {
            cout << "YES\n";
        } else {
            cout << "NO\n";
        }
    }

    return 0;
}
