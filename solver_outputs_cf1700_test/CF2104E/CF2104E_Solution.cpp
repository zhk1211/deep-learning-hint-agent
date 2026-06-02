#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;
    string s;
    cin >> s;

    vector<array<int, 26>> nxt(n + 2);
    array<int, 26> last;
    fill(last.begin(), last.end(), n);
    for (int i = n; i >= 0; --i) {
        nxt[i] = last;
        if (i > 0) {
            last[s[i - 1] - 'a'] = i - 1;
        }
    }

    vector<int> d(n + 2);
    d[n] = 0;
    for (int i = n - 1; i >= 0; --i) {
        int mx = 0;
        for (int c = 0; c < k; ++c) {
            mx = max(mx, nxt[i + 1][c]);
        }
        d[i] = 1 + d[mx];
    }

    int q;
    cin >> q;
    while (q--) {
        string t;
        cin >> t;
        int p = 0;
        for (char ch : t) {
            p = nxt[p][ch - 'a'] + 1;
            if (p > n) break;
        }
        if (p > n) {
            cout << 0 << '\n';
        } else {
            cout << d[p] << '\n';
        }
    }

    return 0;
}
