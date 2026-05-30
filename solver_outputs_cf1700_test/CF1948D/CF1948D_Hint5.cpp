// Hint5
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
        
        for (int len = 2; len <= n; len += 2) {
            int half = len / 2;
            int good = 0;
            for (int i = 0; i < half; ++i) {
                if (s[i] == '?' || s[i + half] == '?' || s[i] == s[i + half]) {
                    ++good;
                }
            }
            if (good == half) ans = len;
            for (int l = 1; l + len <= n; ++l) {
                int r = l + len - 1;
                int prev_l = l - 1;
                int prev_r = r - len;
                if (s[prev_l] == '?' || s[prev_r] == '?' || s[prev_l] == s[prev_r]) {
                    --good;
                }
                if (s[l + half - 1] == '?' || s[r] == '?' || s[l + half - 1] == s[r]) {
                    ++good;
                }
                if (good == half) {
                    ans = len;
                    break;
                }
            }
        }
        cout << ans << '\n';
    }
    return 0;
}
