// Solution
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

    vector<long long> L(n), R(n);
    for (int i = 0; i < n; ++i) {
        L[i] = a[i] + n - i - 1;
        R[i] = a[i] + i;
    }

    vector<long long> pref_max(n);
    pref_max[0] = L[0];
    for (int i = 1; i < n; ++i) {
        pref_max[i] = max(pref_max[i - 1], L[i]);
    }

    vector<long long> suff_max(n);
    suff_max[n - 1] = R[n - 1];
    for (int i = n - 2; i >= 0; --i) {
        suff_max[i] = max(suff_max[i + 1], R[i]);
    }

    long long ans = LLONG_MAX;
    for (int i = 0; i < n; ++i) {
        long long cur = a[i];
        if (i > 0) cur = max(cur, pref_max[i - 1]);
        if (i < n - 1) cur = max(cur, suff_max[i + 1]);
        ans = min(ans, cur);
    }

    cout << ans << '\n';
    return 0;
}
