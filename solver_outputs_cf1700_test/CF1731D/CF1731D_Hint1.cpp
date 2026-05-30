// Hint1
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
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < m; ++j)
                cin >> a[i][j];
        
        int low = 1, high = min(n, m), ans = 1;
        while (low <= high) {
            int mid = (low + high) / 2;
            vector<vector<int>> dp(n, vector<int>(m, 0));
            bool ok = false;
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < m; ++j) {
                    if (a[i][j] >= mid) {
                        dp[i][j] = 1;
                        if (i > 0 && j > 0)
                            dp[i][j] = min({dp[i-1][j], dp[i][j-1], dp[i-1][j-1]}) + 1;
                        if (dp[i][j] >= mid) {
                            ok = true;
                            break;
                        }
                    }
                }
                if (ok) break;
            }
            if (ok) {
                ans = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        cout << ans << '\n';
    }
    return 0;
}
