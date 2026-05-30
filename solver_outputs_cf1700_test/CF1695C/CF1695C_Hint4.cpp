// Hint4
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
        
        // If the path length is odd, sum cannot be 0 because sum of odd number of ±1 is odd
        if ((n + m - 1) % 2 != 0) {
            cout << "NO\n";
            continue;
        }
        
        // dp_min[i][j] = minimum sum to reach (i,j)
        // dp_max[i][j] = maximum sum to reach (i,j)
        vector<vector<int>> dp_min(n, vector<int>(m));
        vector<vector<int>> dp_max(n, vector<int>(m));
        
        dp_min[0][0] = dp_max[0][0] = a[0][0];
        
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                if (i == 0 && j == 0) continue;
                int min_val = INT_MAX, max_val = INT_MIN;
                if (i > 0) {
                    min_val = min(min_val, dp_min[i-1][j]);
                    max_val = max(max_val, dp_max[i-1][j]);
                }
                if (j > 0) {
                    min_val = min(min_val, dp_min[i][j-1]);
                    max_val = max(max_val, dp_max[i][j-1]);
                }
                dp_min[i][j] = min_val + a[i][j];
                dp_max[i][j] = max_val + a[i][j];
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
