#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int t;
    cin >> t;
    while (t--) {
        int n, m;
        cin >> n >> m;
        string s;
        cin >> s;

        vector<int> pref(n + 1), pre_min(n + 1), pre_max(n + 1);
        pref[0] = 0;
        pre_min[0] = 0;
        pre_max[0] = 0;
        for (int i = 1; i <= n; ++i) {
            int delta = (s[i - 1] == '+') ? 1 : -1;
            pref[i] = pref[i - 1] + delta;
            pre_min[i] = min(pre_min[i - 1], pref[i]);
            pre_max[i] = max(pre_max[i - 1], pref[i]);
        }

        vector<int> suf_min(n + 2), suf_max(n + 2);
        suf_min[n + 1] = 0;
        suf_max[n + 1] = 0;
        for (int i = n; i >= 1; --i) {
            int delta = (s[i - 1] == '+') ? 1 : -1;
            suf_min[i] = min(0, delta + suf_min[i + 1]);
            suf_max[i] = max(0, delta + suf_max[i + 1]);
        }

        while (m--) {
            int l, r;
            cin >> l >> r;
            int overall_min = min(pre_min[l - 1], pref[l - 1] + suf_min[r + 1]);
            int overall_max = max(pre_max[l - 1], pref[l - 1] + suf_max[r + 1]);
            int ans = overall_max - overall_min + 1;
            cout << ans << '\n';
        }
    }
    return 0;
}
