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

        int lo = 1, hi = min(n, m), ans = 1;
        while (lo <= hi) {
            int mid = (lo + hi) / 2;
            vector<vector<int>> b(n, vector<int>(m, 0));
            for (int i = 0; i < n; ++i)
                for (int j = 0; j < m; ++j)
                    if (a[i][j] >= mid) b[i][j] = 1;

            vector<vector<int>> pref(n + 1, vector<int>(m + 1, 0));
            for (int i = 1; i <= n; ++i)
                for (int j = 1; j <= m; ++j)
                    pref[i][j] = b[i-1][j-1] + pref[i-1][j] + pref[i][j-1] - pref[i-1][j-1];

            bool ok = false;
            for (int i = 0; i + mid <= n && !ok; ++i) {
                for (int j = 0; j + mid <= m && !ok; ++j) {
                    int sum = pref[i+mid][j+mid] - pref[i][j+mid] - pref[i+mid][j] + pref[i][j];
                    if (sum == mid * mid) ok = true;
                }
            }

            if (ok) {
                ans = mid;
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
        cout << ans << '\n';
    }
    return 0;
}
