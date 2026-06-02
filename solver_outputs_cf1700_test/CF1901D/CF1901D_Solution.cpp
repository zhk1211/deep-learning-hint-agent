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

    vector<long long> pref_max(n), suff_max(n);
    pref_max[0] = a[0] + (n - 1);
    for (int i = 1; i < n; ++i) {
        pref_max[i] = max(pref_max[i - 1], a[i] + (n - 1 - i));
    }
    suff_max[n - 1] = a[n - 1] + (n - 1);
    for (int i = n - 2; i >= 0; --i) {
        suff_max[i] = max(suff_max[i + 1], a[i] + i);
    }

    long long ans = LLONG_MAX;
    for (int i = 0; i < n; ++i) {
        long long cur = a[i];
        if (i > 0) {
            cur = max(cur, pref_max[i - 1]);
        }
        if (i < n - 1) {
            cur = max(cur, suff_max[i + 1]);
        }
        ans = min(ans, cur);
    }

    cout << ans << '\n';
    return 0;
}
