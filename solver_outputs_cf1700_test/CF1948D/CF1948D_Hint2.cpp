// Hint2
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        string s;
        cin >> s;
        int n = s.size();
        int ans = 0;
        
        for (int len = n / 2 * 2; len >= 2; len -= 2) {
            bool found = false;
            for (int l = 0; l + len <= n; ++l) {
                int half = len / 2;
                bool ok = true;
                for (int i = 0; i < half; ++i) {
                    char c1 = s[l + i];
                    char c2 = s[l + half + i];
                    if (c1 != '?' && c2 != '?' && c1 != c2) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    ans = len;
                    found = true;
                    break;
                }
            }
            if (found) break;
        }
        cout << ans << '\n';
    }
    return 0;
}
