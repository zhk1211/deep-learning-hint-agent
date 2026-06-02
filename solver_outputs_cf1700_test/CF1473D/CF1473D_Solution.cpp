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
        vector<int> pref_min(n + 1, 0);
        vector<int> pref_max(n + 1, 0);
        for (int i = 0; i < n; ++i) {
            pref[i + 1] = pref[i] + (s[i] == '+' ? 1 : -1);
            pref_min[i + 1] = min(pref_min[i], pref[i + 1]);
            pref_max[i + 1] = max(pref_max[i], pref[i + 1]);
        }

        vector<int> suff_min(n + 2, 0);
        vector<int> suff_max(n + 2, 0);
        for (int i = n; i >= 1; --i) {
            int delta = (s[i - 1] == '+' ? 1 : -1);
            suff_min[i] = min(0, delta + suff_min[i + 1]);
            suff_max[i] = max(0, delta + suff_max[i + 1]);
        }

        while (m--) {
            int l, r;
            cin >> l >> r;
            int min_pref = pref_min[l - 1];
            int max_pref = pref_max[l - 1];
            int val = pref[l - 1];
            int min_suff = val + suff_min[r + 1];
            int max_suff = val + suff_max[r + 1];
            int overall_min = min(min_pref, min_suff);
            int overall_max = max(max_pref, max_suff);
            cout << overall_max - overall_min + 1 << '\n';
        }
    }
    return 0;
}
