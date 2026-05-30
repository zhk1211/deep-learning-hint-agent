// Hint2
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
        
        vector<int> ans(total);
        vector<int> col_good(m, 0);
        int good_cols = 0;
        int last_one = -1;
        vector<int> row_ans(total, 0);
        
        for (int i = 0; i < total; ++i) {
            // columns
            int col_idx = i % m;
            if (s[i] == '1') {
                if (!col_good[col_idx]) {
                    col_good[col_idx] = 1;
                    ++good_cols;
                }
            }
            
            // rows
            if (s[i] == '1') {
                last_one = i;
            }
            if (i >= m) {
                row_ans[i] = row_ans[i - m];
            }
            if (last_one != -1 && i - last_one < m) {
                row_ans[i]++;
            }
            
            ans[i] = good_cols + row_ans[i];
        }
        
        for (int i = 0; i < total; ++i) {
            cout << ans[i] << (i + 1 == total ? '\n' : ' ');
        }
    }
    return 0;
}
