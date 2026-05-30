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
        vector<string> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }
        
        string ans = "-1";
        string base = a[0];
        
        for (int pos = 0; pos < m; ++pos) {
            for (char c = 'a'; c <= 'z'; ++c) {
                string cand = base;
                cand[pos] = c;
                
                bool ok = true;
                for (int i = 0; i < n; ++i) {
                    int diff = 0;
                    for (int j = 0; j < m; ++j) {
                        if (cand[j] != a[i][j]) diff++;
                    }
                    if (diff > 1) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    ans = cand;
                    goto done;
                }
            }
        }
        done:
        cout << ans << '\n';
    }
    return 0;
}
