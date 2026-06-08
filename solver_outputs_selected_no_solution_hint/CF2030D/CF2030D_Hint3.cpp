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
        vector<int> p(n);
        for (int i = 0; i < n; ++i) {
            cin >> p[i];
        }
        string s;
        cin >> s;

        // We'll maintain a segment tree or Fenwick tree over "bad" positions.
        // A position i (1-indexed, 1 <= i <= n-1) is "bad" if:
        // s[i-1] == 'L' && s[i] == 'R' && max(p[0..i-1]) > i
        // Actually we need to check if the segment [1..i] can be sorted internally.
        // Condition: for any prefix that ends with 'L' followed by 'R' (or at boundaries),
        // the set of values in that prefix must be exactly {1..i}.
        // Equivalent: max(p[0..i-1]) == i for all i where s[i-1] == 'L' and s[i] == 'R'.
        // Also check boundaries: if s[0] == 'R', prefix 1 must have max = 1? Actually s[0] is always 'R'.
        // Similarly s[n-1] is always 'L'.
        // We'll maintain count of "bad" indices.

        vector<int> max_prefix(n);
        max_prefix[0] = p[0];
        for (int i = 1; i < n; ++i) {
            max_prefix[i] = max(max_prefix[i-1], p[i]);
        }

        auto is_bad = [&](int i) -> bool {
            // i is 1-indexed, 1 <= i <= n-1
            // condition: s[i-1] == 'L' && s[i] == 'R' && max_prefix[i-1] != i
            return (s[i-1] == 'L' && s[i] == 'R' && max_prefix[i-1] != i);
        };

        int bad_count = 0;
        vector<int> bad(n, 0); // 1-indexed for positions 1..n-1
        for (int i = 1; i < n; ++i) {
            if (is_bad(i)) {
                bad[i] = 1;
                ++bad_count;
            }
        }

        // Also need to check the whole array condition: max_prefix[n-1] must be n (always true for permutation)
        // and for the last segment: if s[n-1] == 'L', then the suffix condition is automatically satisfied
        // if all prefixes are good. Actually the condition for the whole array is that for every i where
        // s[i-1] == 'L' and s[i] == 'R', max_prefix[i-1] == i. If all such are good, then the array is sortable.
        // So answer is YES iff bad_count == 0.

        auto update = [&](int pos) {
            // pos is 1-indexed index in string (1..n)
            // changing s[pos-1] affects badness of pos and pos+1
            if (pos >= 1 && pos < n) {
                if (bad[pos]) {
                    bad[pos] = 0;
                    --bad_count;
                }
                if (is_bad(pos)) {
                    bad[pos] = 1;
                    ++bad_count;
                }
            }
            if (pos - 1 >= 1 && pos - 1 < n) {
                int i = pos - 1;
                if (bad[i]) {
                    bad[i] = 0;
                    --bad_count;
                }
                if (is_bad(i)) {
                    bad[i] = 1;
                    ++bad_count;
                }
            }
        };

        while (q--) {
            int idx;
            cin >> idx;
            // idx is 1-indexed, 2 <= idx <= n-1
            // flip s[idx-1]
            s[idx-1] = (s[idx-1] == 'L') ? 'R' : 'L';
            update(idx);
            cout << (bad_count == 0 ? "YES" : "NO") << "\n";
        }
    }
    return 0;
}
