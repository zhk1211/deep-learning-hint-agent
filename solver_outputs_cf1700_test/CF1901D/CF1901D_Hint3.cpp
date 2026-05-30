// Hint3
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    if (n == 1) {
        cout << a[0] << "\n";
        return 0;
    }

    vector<long long> pref_max(n), suff_max(n);
    for (int i = 0; i < n; ++i) {
        pref_max[i] = a[i] + i;
        if (i > 0) pref_max[i] = max(pref_max[i], pref_max[i - 1]);
    }
    for (int i = n - 1; i >= 0; --i) {
        suff_max[i] = a[i] + (n - 1 - i);
        if (i + 1 < n) suff_max[i] = max(suff_max[i], suff_max[i + 1]);
    }

    long long ans = LLONG_MAX;
    for (int i = 0; i < n; ++i) {
        long long cand = a[i];
        if (i > 0) cand = max(cand, pref_max[i - 1]);
        if (i + 1 < n) cand = max(cand, suff_max[i + 1]);
        ans = min(ans, cand);
    }

    cout << ans << "\n";
    return 0;
}
