// Hint3
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
        vector<string> t(2 * n);
        for (int i = 0; i < 2 * n; ++i) {
            cin >> t[i];
        }
        string s;
        cin >> s;

        array<int, 26> freq = {};
        for (char c : s) {
            freq[c - 'a']++;
        }
        for (const string& str : t) {
            for (char c : str) {
                freq[c - 'a']++;
            }
        }

        string ans;
        for (int i = 0; i < 26; ++i) {
            if (freq[i] % 2 == 1) {
                ans += char('a' + i);
            }
        }
        cout << ans << '\n';
    }
    return 0;
}
