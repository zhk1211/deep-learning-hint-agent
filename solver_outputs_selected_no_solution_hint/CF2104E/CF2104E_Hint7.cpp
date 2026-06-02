#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;
    string s;
    cin >> s;

    // next_pos[i][c] = first position >= i where character c occurs
    vector<array<int, 26>> next_pos(n + 2);
    for (int c = 0; c < 26; ++c) {
        next_pos[n + 1][c] = n + 1;
    }
    for (int i = n; i >= 0; --i) {
        next_pos[i] = next_pos[i + 1];
        if (i < n) {
            next_pos[i][s[i] - 'a'] = i + 1;
        }
    }

    int q;
    cin >> q;
    while (q--) {
        string t;
        cin >> t;
        int m = (int)t.size();

        // dp[i] = minimum length of prefix of s needed to match prefix of t of length i
        vector<int> dp(m + 1, n + 1);
        dp[0] = 0;
        for (int i = 0; i < m; ++i) {
            if (dp[i] <= n) {
                dp[i + 1] = next_pos[dp[i]][t[i] - 'a'];
            } else {
                dp[i + 1] = n + 1;
            }
        }

        // find the smallest i such that dp[i] > n, meaning prefix of length i is not a subsequence
        int ans = 0;
        for (int i = m; i >= 0; --i) {
            if (dp[i] <= n) {
                ans = m - i;
                break;
            }
        }
        cout << ans << '\n';
    }

    return 0;
}
