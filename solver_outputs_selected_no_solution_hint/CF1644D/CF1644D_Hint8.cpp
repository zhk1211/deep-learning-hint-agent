#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n, m, k, q;
        cin >> n >> m >> k >> q;
        
        vector<pair<int,int>> ops(q);
        for (int i = 0; i < q; i++) {
            cin >> ops[i].first >> ops[i].second;
        }
        
        vector<bool> row_used(n + 1, false);
        vector<bool> col_used(m + 1, false);
        int rows_covered = 0, cols_covered = 0;
        long long ans = 1;
        
        for (int i = q - 1; i >= 0; i--) {
            int x = ops[i].first;
            int y = ops[i].second;
            
            bool row_new = !row_used[x];
            bool col_new = !col_used[y];
            
            if (row_new || col_new) {
                if (rows_covered < n && cols_covered < m) {
                    ans = (ans * k) % MOD;
                }
            }
            
            if (row_new) {
                row_used[x] = true;
                rows_covered++;
            }
            if (col_new) {
                col_used[y] = true;
                cols_covered++;
            }
        }
        
        cout << ans << '\n';
    }
    
    return 0;
}
