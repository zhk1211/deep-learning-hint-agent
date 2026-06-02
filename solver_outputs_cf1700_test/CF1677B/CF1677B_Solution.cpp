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
        
        vector<int> ans(total, 0);
        vector<int> col_good(m, 0);
        int col_ans = 0;
        int last_one = -1e9;
        vector<int> row_ans(total, 0);
        
        for (int i = 0; i < total; ++i) {
            int col_idx = i % m;
            if (s[i] == '1') {
                if (!col_good[col_idx]) {
                    col_good[col_idx] = 1;
                    ++col_ans;
                }
                last_one = i;
            }
            ans[i] += col_ans;
            
            if (i >= m) {
                row_ans[i] = row_ans[i - m];
            }
            if (i - last_one < m) {
                row_ans[i]++;
            }
            ans[i] += row_ans[i];
        }
        
        for (int i = 0; i < total; ++i) {
            cout << ans[i] << " \n"[i == total - 1];
        }
    }
    return 0;
}
