#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int t;
    cin >> t;
    while (t--) {
        int n, m;
        cin >> n >> m;
        vector<vector<int>> a(n, vector<int>(m));
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < m; ++j)
                cin >> a[i][j];
        // If path length (n + m - 1) is odd, sum cannot be zero
        if ((n + m) % 2 == 0) {
            cout << "NO\n";
            continue;
        }
        vector<vector<int>> dp_min(n, vector<int>(m));
        vector<vector<int>> dp_max(n, vector<int>(m));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                if (i == 0 && j == 0) {
                    dp_min[i][j] = dp_max[i][j] = a[i][j];
                } else if (i == 0) {
                    dp_min[i][j] = dp_min[i][j-1] + a[i][j];
                    dp_max[i][j] = dp_max[i][j-1] + a[i][j];
                } else if (j == 0) {
                    dp_min[i][j] = dp_min[i-1][j] + a[i][j];
                    dp_max[i][j] = dp_max[i-1][j] + a[i][j];
                } else {
                    dp_min[i][j] = min(dp_min[i-1][j], dp_min[i][j-1]) + a[i][j];
                    dp_max[i][j] = max(dp_max[i-1][j], dp_max[i][j-1]) + a[i][j];
                }
            }
        }
        if (dp_min[n-1][m-1] <= 0 && dp_max[n-1][m-1] >= 0)
            cout << "YES\n";
        else
            cout << "NO\n";
    }
    return 0;
}
