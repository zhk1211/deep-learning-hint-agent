#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<long long> b(n);
        for (int i = 0; i < n; ++i) {
            cin >> b[i];
        }

        vector<long long> pref_max(n);
        pref_max[0] = b[0] + 0;
        for (int i = 1; i < n; ++i) {
            pref_max[i] = max(pref_max[i - 1], b[i] + i);
        }

        vector<long long> suff_max(n);
        suff_max[n - 1] = b[n - 1] - (n - 1);
        for (int i = n - 2; i >= 0; --i) {
            suff_max[i] = max(suff_max[i + 1], b[i] - i);
        }

        long long ans = LLONG_MIN;
        for (int m = 1; m < n - 1; ++m) {
            long long cur = b[m] + pref_max[m - 1] + suff_max[m + 1];
            if (cur > ans) ans = cur;
        }

        cout << ans << '\n';
    }

    return 0;
}
