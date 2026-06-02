#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int T;
    cin >> T;
    while (T--) {
        int n;
        cin >> n;
        vector<int> cnt(26, 0);
        for (int i = 0; i < 2 * n; ++i) {
            string t;
            cin >> t;
            for (char c : t) {
                cnt[c - 'a'] ^= 1;
            }
        }
        string s;
        cin >> s;
        for (char c : s) {
            cnt[c - 'a'] ^= 1;
        }
        char ans = 'a';
        for (int i = 0; i < 26; ++i) {
            if (cnt[i]) {
                ans = 'a' + i;
                break;
            }
        }
        cout << ans << '\n';
    }
    return 0;
}
