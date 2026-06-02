#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;
    string s;
    cin >> s;
    int q;
    cin >> q;

    vector<int> nxt(n + 2, n);
    vector<int> pos(k, n);
    for (int i = n - 1; i >= 0; --i) {
        pos[s[i] - 'a'] = i;
        int mn = n;
        for (int c = 0; c < k; ++c) {
            mn = min(mn, pos[c]);
        }
        nxt[i] = mn;
    }

    vector<int> jump(n + 2);
    jump[n] = n;
    for (int i = n - 1; i >= 0; --i) {
        if (nxt[i] == n) {
            jump[i] = n;
        } else {
            jump[i] = jump[nxt[i] + 1];
        }
    }

    while (q--) {
        string t;
        cin >> t;
        int cur = 0;
        int ans = 0;
        for (char ch : t) {
            if (cur >= n) {
                break;
            }
            int c = ch - 'a';
            int p = n;
            for (int i = cur; i < n; ++i) {
                if (s[i] - 'a' == c) {
                    p = i;
                    break;
                }
            }
            if (p == n) {
                cur = n;
                break;
            }
            cur = p + 1;
        }
        if (cur >= n) {
            cout << 0 << '\n';
            continue;
        }
        int steps = 0;
        while (cur < n) {
            cur = jump[cur];
            ++steps;
        }
        cout << steps << '\n';
    }

    return 0;
}
