// Hint4
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n, m;
        cin >> n >> m;
        string s;
        cin >> s;
        
        vector<int> pref(n + 1, 0);
        vector<int> pref_min(n + 1, 0), pref_max(n + 1, 0);
        for (int i = 0; i < n; i++) {
            pref[i + 1] = pref[i] + (s[i] == '+' ? 1 : -1);
            pref_min[i + 1] = min(pref_min[i], pref[i + 1]);
            pref_max[i + 1] = max(pref_max[i], pref[i + 1]);
        }
        
        vector<int> suff_min(n + 2, 0), suff_max(n + 2, 0);
        suff_min[n + 1] = 0;
        suff_max[n + 1] = 0;
        for (int i = n; i >= 1; i--) {
            int delta = (s[i - 1] == '+' ? 1 : -1);
            suff_min[i] = min(0, delta + suff_min[i + 1]);
            suff_max[i] = max(0, delta + suff_max[i + 1]);
        }
        
        while (m--) {
            int l, r;
            cin >> l >> r;
            int low = min(pref_min[l - 1], pref[l - 1] + suff_min[r + 1]);
            int high = max(pref_max[l - 1], pref[l - 1] + suff_max[r + 1]);
            cout << high - low + 1 << '\n';
        }
    }
    return 0;
}
