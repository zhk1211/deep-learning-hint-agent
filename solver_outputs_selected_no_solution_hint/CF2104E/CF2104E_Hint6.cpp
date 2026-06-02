#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;
    string s;
    cin >> s;

    vector<int> nxt(n + 1, n);
    vector<int> last(k, n);
    for (int i = n - 1; i >= 0; --i) {
        last[s[i] - 'a'] = i;
        int mn = n;
        for (int c = 0; c < k; ++c) {
            mn = min(mn, last[c]);
        }
        nxt[i] = mn;
    }

    int q;
    cin >> q;
    while (q--) {
        string t;
        cin >> t;
        int pos = -1;
        int steps = 0;
        for (char ch : t) {
            if (pos + 1 >= n) {
                steps++;
                pos = -1;
            }
            int c = ch - 'a';
            int npos = n;
            if (pos + 1 < n) {
                int start = pos + 1;
                if (s[start] - 'a' == c) {
                    npos = start;
                } else {
                    int candidate = nxt[start];
                    if (candidate < n && s[candidate] - 'a' == c) {
                        npos = candidate;
                    } else {
                        npos = n;
                    }
                }
            }
            if (npos == n) {
                steps++;
                pos = -1;
                if (s[0] - 'a' == c) {
                    pos = 0;
                } else {
                    int candidate = nxt[0];
                    if (candidate < n && s[candidate] - 'a' == c) {
                        pos = candidate;
                    } else {
                        pos = -1;
                    }
                }
            } else {
                pos = npos;
            }
        }
        cout << steps << '\n';
    }

    return 0;
}
