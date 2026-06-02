#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;
    string s;
    cin >> s;

    vector<int> next_pos(n + 2, n);
    vector<int> first_after(n + 2, n);
    for (int i = n - 1; i >= 0; --i) {
        next_pos[i] = first_after[s[i] - 'a'];
        first_after[s[i] - 'a'] = i;
    }

    vector<int> dp(n + 1, 0);
    for (int i = n - 1; i >= 0; --i) {
        int max_val = 0;
        for (int c = 0; c < k; ++c) {
            int pos = first_after[c];
            if (pos < i) continue;
            if (pos == n) {
                max_val = max(max_val, 1);
            } else {
                max_val = max(max_val, dp[pos] + 1);
            }
        }
        dp[i] = max_val;
    }

    int q;
    cin >> q;
    while (q--) {
        string t;
        cin >> t;
        int m = (int)t.size();
        int cur = 0;
        bool pleasant = true;
        for (char ch : t) {
            int c = ch - 'a';
            int pos = first_after[c];
            while (pos < cur) {
                pos = next_pos[pos];
            }
            if (pos == n) {
                pleasant = false;
                break;
            }
            cur = pos + 1;
        }
        if (!pleasant) {
            cout << 0 << '\n';
        } else {
            cout << dp[cur] << '\n';
        }
    }

    return 0;
}
