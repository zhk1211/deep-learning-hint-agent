// Hint3
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
        // Try all strings that differ from a[0] in at most one position
        for (int pos = 0; pos < m; ++pos) {
            string s = a[0];
            for (char c = 'a'; c <= 'z'; ++c) {
                s[pos] = c;
                bool ok = true;
                for (int i = 0; i < n; ++i) {
                    int diff = 0;
                    for (int j = 0; j < m; ++j) {
                        if (s[j] != a[i][j]) diff++;
                    }
                    if (diff > 1) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    ans = s;
                    break;
                }
            }
            if (ans != "-1") break;
        }
        // Also consider the original a[0] (already covered when c == a[0][pos] for all pos)
        cout << ans << '\n';
    }
    return 0;
}
