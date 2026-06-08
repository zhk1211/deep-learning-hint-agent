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
        int n = (int)s.size();
        int ans = 0;
        
        for (int len = n / 2 * 2; len >= 2; len -= 2) {
            bool found = false;
            for (int i = 0; i + len <= n; ++i) {
                bool ok = true;
                int half = len / 2;
                for (int j = 0; j < half; ++j) {
                    if (s[i + j] != '?' && s[i + half + j] != '?' && s[i + j] != s[i + half + j]) {
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
