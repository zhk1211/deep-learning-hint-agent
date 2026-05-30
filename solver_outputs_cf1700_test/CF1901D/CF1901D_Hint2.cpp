// Hint2
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    if (n == 1) {
        cout << a[0] << "\n";
        return 0;
    }

    vector<int> b = a;
    for (int i = 0; i < n; ++i) {
        b[i] += i;
    }
    vector<int> pref_max(n), suff_max(n);
    pref_max[0] = b[0];
    for (int i = 1; i < n; ++i) {
        pref_max[i] = max(pref_max[i - 1], b[i]);
    }
    suff_max[n - 1] = b[n - 1];
    for (int i = n - 2; i >= 0; --i) {
        suff_max[i] = max(suff_max[i + 1], b[i]);
    }

    vector<int> c = a;
    for (int i = 0; i < n; ++i) {
        c[i] += (n - 1 - i);
    }
    vector<int> pref_max_c(n), suff_max_c(n);
    pref_max_c[0] = c[0];
    for (int i = 1; i < n; ++i) {
        pref_max_c[i] = max(pref_max_c[i - 1], c[i]);
    }
    suff_max_c[n - 1] = c[n - 1];
    for (int i = n - 2; i >= 0; --i) {
        suff_max_c[i] = max(suff_max_c[i + 1], c[i]);
    }

    int ans = INT_MAX;
    for (int i = 0; i < n; ++i) {
        int cur = a[i];
        if (i > 0) {
            cur = max(cur, pref_max[i - 1] - i + 1);
        }
        if (i < n - 1) {
            cur = max(cur, suff_max_c[i + 1] - (n - 1 - i) + 1);
        }
        ans = min(ans, cur);
    }

    cout << ans << "\n";
    return 0;
}
