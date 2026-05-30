// Hint5
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
        
        set<int> rows, cols;
        long long ans = 1;
        for (int i = q - 1; i >= 0; i--) {
            int x = ops[i].first, y = ops[i].second;
            bool useful = false;
            if (rows.find(x) == rows.end() && cols.size() < m) useful = true;
            if (cols.find(y) == cols.end() && rows.size() < n) useful = true;
            if (useful) ans = (ans * k) % MOD;
            rows.insert(x);
            cols.insert(y);
        }
        cout << ans << '\n';
    }
    return 0;
}
