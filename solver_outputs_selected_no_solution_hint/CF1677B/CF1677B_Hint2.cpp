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
        string s;
        cin >> s;
        int total = n * m;
        
        vector<int> col_good(m, 0);
        int good_cols = 0;
        vector<int> ans(total);
        vector<int> row_good(total, 0);
        
        int last_one = -1;
        for (int i = 0; i < total; ++i) {
            if (s[i] == '1') {
                last_one = i;
            }
            if (i - last_one < m) {
                row_good[i] = 1;
            }
        }
        
        for (int i = 0; i < total; ++i) {
            int col = i % m;
            if (s[i] == '1' && !col_good[col]) {
                col_good[col] = 1;
                ++good_cols;
            }
            int rows = 0;
            if (i >= m) {
                rows = row_good[i] + (i - m >= 0 ? ans[i - m] - good_cols : 0);
            } else {
                rows = row_good[i];
            }
            ans[i] = good_cols + rows;
        }
        
        for (int i = 0; i < total; ++i) {
            cout << ans[i] << " \n"[i == total - 1];
        }
    }
    return 0;
}
