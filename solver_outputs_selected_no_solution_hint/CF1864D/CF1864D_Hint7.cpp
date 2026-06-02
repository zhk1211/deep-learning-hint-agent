#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<string> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }
    
    vector<vector<int>> diff(n + 2, vector<int>(n + 2, 0));
    vector<vector<int>> diag(n + 2, vector<int>(n + 2, 0));
    vector<vector<int>> anti(n + 2, vector<int>(n + 2, 0));
    
    int ans = 0;
    
    for (int i = 0; i < n; ++i) {
        vector<int> cur(n + 2, 0);
        for (int j = 0; j < n; ++j) {
            cur[j] += diff[i][j];
            if (j > 0) cur[j] += cur[j-1];
            
            int val = (a[i][j] - '0') ^ (cur[j] & 1);
            if (val == 1) {
                ++ans;
                // apply operation at (i, j)
                // update diff for rows below
                diff[i+1][j] ^= 1;
                // update diagonal (i+1, j-1) if j > 0
                if (j > 0) {
                    diag[i+1][j-1] ^= 1;
                }
                // update anti-diagonal (i+1, j+1)
                anti[i+1][j+1] ^= 1;
            }
            
            // propagate diagonal and anti-diagonal effects to cur
            if (i+1 < n) {
                // diagonal effect: from (i, j) to (i+1, j-1)
                if (j > 0) {
                    diff[i+1][j-1] ^= diag[i][j];
                    diag[i+1][j-1] ^= diag[i][j];
                }
                // anti-diagonal effect: from (i, j) to (i+1, j+1)
                if (j+1 < n) {
                    diff[i+1][j+1] ^= anti[i][j];
                    anti[i+1][j+1] ^= anti[i][j];
                }
            }
        }
    }
    
    cout << ans << '\n';
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
