#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n;
        string s;
        cin >> n >> s;
        
        int ans = 0;
        for (int i = 0; i < n; ++i) {
            int bal = 0, mn = 0;
            for (int j = i; j < n; ++j) {
                if (s[j] == '+') {
                    bal++;
                } else {
                    bal--;
                }
                mn = min(mn, bal);
                // Check if substring s[i..j] is promising
                // Condition: bal % 3 == 0 and bal >= 3 * mn
                if (bal <= 0 && bal % 3 == 0 && bal >= 3 * mn) {
                    ans++;
                }
            }
        }
        cout << ans << '\n';
    }
    return 0;
}
