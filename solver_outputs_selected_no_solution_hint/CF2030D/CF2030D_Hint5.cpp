#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n, q;
        cin >> n >> q;
        vector<int> p(n + 1);
        for (int i = 1; i <= n; ++i) {
            cin >> p[i];
        }
        string s;
        cin >> s;
        s = " " + s; // 1-indexed

        // We need to check if the permutation is sortable.
        // Condition: For every i from 1 to n-1, if s[i] == 'L' and s[i+1] == 'R',
        // then the set of values in positions 1..i must be exactly {1..i}.
        // Equivalently, max(p[1..i]) == i.
        // We'll maintain a segment tree or Fenwick tree to track max prefix,
        // and count how many such "bad" positions exist.
        // A position i (1 <= i < n) is "bad" if s[i]=='L' && s[i+1]=='R' && max(p[1..i]) != i.
        // If there are zero bad positions, answer is YES, else NO.

        vector<int> pref_max(n + 1, 0);
        for (int i = 1; i <= n; ++i) {
            pref_max[i] = max(pref_max[i - 1], p[i]);
        }

        auto is_bad = [&](int i) -> bool {
            if (i < 1 || i >= n) return false;
            if (s[i] == 'L' && s[i + 1] == 'R') {
                return pref_max[i] != i;
            }
            return false;
        };

        int bad_cnt = 0;
        for (int i = 1; i < n; ++i) {
            if (is_bad(i)) ++bad_cnt;
        }

        while (q--) {
            int idx;
            cin >> idx;
            // flip s[idx]
            // positions that can change badness: idx-1 and idx
            for (int cand : {idx - 1, idx}) {
                if (cand >= 1 && cand < n) {
                    if (is_bad(cand)) --bad_cnt;
                }
            }
            s[idx] = (s[idx] == 'L' ? 'R' : 'L');
            for (int cand : {idx - 1, idx}) {
                if (cand >= 1 && cand < n) {
                    if (is_bad(cand)) ++bad_cnt;
                }
            }
            cout << (bad_cnt == 0 ? "YES" : "NO") << '\n';
        }
    }
    return 0;
}
