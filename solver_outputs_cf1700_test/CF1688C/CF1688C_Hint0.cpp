// Hint0
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

        // Count occurrences of each character in all t_i and in s
        vector<int> cnt(26, 0);
        for (const string& str : t) {
            for (char c : str) {
                cnt[c - 'a']++;
            }
        }
        for (char c : s) {
            cnt[c - 'a']++;
        }

        // The initial string's characters are those with odd total count
        string ans;
        for (int i = 0; i < 26; ++i) {
            if (cnt[i] % 2 == 1) {
                ans += char('a' + i);
            }
        }
        cout << ans << '\n';
    }
    return 0;
}
