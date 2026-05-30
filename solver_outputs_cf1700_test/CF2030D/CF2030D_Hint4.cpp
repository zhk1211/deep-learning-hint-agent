// Hint4
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

        // diff array for intervals [l, r] where l is min, r is max of values in a segment
        // Actually we need to check if each position i is "good": 
        // i is good if there is no k in [i, i+1) such that s[k]=='L' and s[k+1]=='R' 
        // that blocks movement from left to right across the boundary.
        // But the problem reduces to: we can sort if for every i from 1 to n-1,
        // the set of elements that need to cross the boundary between i and i+1 can do so.
        // This is possible iff for every i, the segment [min_val, max_val] of the prefix of length i
        // is exactly [1, i] (i.e., the prefix is a permutation of 1..i). 
        // Wait, the condition from hints: we can swap an element from i to j (i<j) if there is no k in [i, j-1] 
        // with s[k]=='L' and s[k+1]=='R'. So the array is sortable iff for every i, 
        // the maximum value in the prefix of length i is <= i, OR the boundary between i and i+1 is "passable" 
        // in both directions? Actually, if the prefix has all numbers 1..i, then no element needs to cross 
        // the boundary i|i+1. If it doesn't, some element > i is in the prefix and must move right, 
        // and some element <= i is in the suffix and must move left. They need to cross the boundary.
        // They can cross if there is no "LR" block at that boundary. 
        // So we need: for every i from 1 to n-1, if the prefix max > i, then the boundary i must not be blocked.
        // A boundary i is blocked if s[i-1]=='L' and s[i]=='R' (using 0-indexed: s[i-1]=='L' && s[i]=='R').
        // So we maintain the set of "bad" indices i (1-indexed boundaries) where prefix max > i and boundary is blocked.
        // After each update, we toggle s at index idx (1-indexed). This can affect boundaries idx-1 and idx.
        // We recompute their blocked status and check if they are bad.
        // If the set of bad indices is empty, answer YES, else NO.

        vector<int> pref_max(n);
        pref_max[0] = p[0];
        for (int i = 1; i < n; ++i) {
            pref_max[i] = max(pref_max[i-1], p[i]);
        }

        auto is_blocked = [&](int i) -> bool {
            // boundary between i and i+1 (1-indexed i from 1 to n-1)
            // 0-indexed: between i-1 and i
            return (s[i-1] == 'L' && s[i] == 'R');
        };

        set<int> bad; // stores boundaries i (1-indexed) that are bad
        for (int i = 1; i <= n-1; ++i) {
            if (pref_max[i-1] > i && is_blocked(i)) {
                bad.insert(i);
            }
        }

        while (q--) {
            int idx;
            cin >> idx;
            // idx is 1-indexed, 2 <= idx <= n-1
            // flip s[idx-1]
            s[idx-1] = (s[idx-1] == 'L' ? 'R' : 'L');

            // boundaries affected: idx-1 and idx (if they exist)
            for (int b : {idx-1, idx}) {
                if (b >= 1 && b <= n-1) {
                    // remove from bad if present
                    bad.erase(b);
                    // re-evaluate
                    if (pref_max[b-1] > b && is_blocked(b)) {
                        bad.insert(b);
                    }
                }
            }

            if (bad.empty()) {
                cout << "YES\n";
            } else {
                cout << "NO\n";
            }
        }
    }
    return 0;
}
