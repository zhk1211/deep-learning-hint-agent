// Hint3
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
        
        // prefix max of p
        vector<int> pref_max(n);
        pref_max[0] = p[0];
        for (int i = 1; i < n; ++i) {
            pref_max[i] = max(pref_max[i-1], p[i]);
        }
        
        // suffix min of p
        vector<int> suff_min(n);
        suff_min[n-1] = p[n-1];
        for (int i = n-2; i >= 0; --i) {
            suff_min[i] = min(suff_min[i+1], p[i]);
        }
        
        // A position i (0-indexed) is "bad" if it is a boundary where
        // the prefix max up to i is > i+1 (meaning some element > i+1 is stuck on the left)
        // or suffix min from i+1 is < i+2 (meaning some element <= i+1 is stuck on the right)
        // but actually we need to check if the segment [0..i] can be separated from [i+1..n-1]
        // by a "LR" barrier. The condition for a barrier at i (between i and i+1) is:
        // s[i] == 'L' && s[i+1] == 'R'. If such a barrier exists, then elements cannot cross.
        // For the array to be sortable, for every i from 0 to n-2, if there is a barrier,
        // then the set of values in [0..i] must be exactly {1..i+1}.
        // This is equivalent to: pref_max[i] == i+1.
        // We'll maintain a set of indices i where there is a barrier and pref_max[i] != i+1.
        
        set<int> bad_barriers;
        for (int i = 0; i < n-1; ++i) {
            if (s[i] == 'L' && s[i+1] == 'R') {
                if (pref_max[i] != i+1) {
                    bad_barriers.insert(i);
                }
            }
        }
        
        auto update = [&](int idx) {
            // idx is the position changed (0-indexed)
            // barriers affected: idx-1 (between idx-1 and idx) and idx (between idx and idx+1)
            for (int i : {idx-1, idx}) {
                if (i >= 0 && i < n-1) {
                    if (s[i] == 'L' && s[i+1] == 'R') {
                        if (pref_max[i] != i+1) {
                            bad_barriers.insert(i);
                        } else {
                            bad_barriers.erase(i);
                        }
                    } else {
                        bad_barriers.erase(i);
                    }
                }
            }
        };
        
        while (q--) {
            int idx;
            cin >> idx;
            --idx; // to 0-indexed
            s[idx] = (s[idx] == 'L' ? 'R' : 'L');
            update(idx);
            if (bad_barriers.empty()) {
                cout << "YES\n";
            } else {
                cout << "NO\n";
            }
        }
    }
    return 0;
}
