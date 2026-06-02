#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }
    vector<int> pref_max(n), suff_min(n);
    pref_max[0] = a[0];
    for (int i = 1; i < n; ++i) {
        pref_max[i] = max(pref_max[i - 1], a[i]);
    }
    suff_min[n - 1] = a[n - 1];
    for (int i = n - 2; i >= 0; --i) {
        suff_min[i] = min(suff_min[i + 1], a[i]);
    }
    vector<int> ans(n);
    ans[n - 1] = pref_max[n - 1];
    for (int i = n - 2; i >= 0; --i) {
        if (pref_max[i] > suff_min[i + 1]) {
            ans[i] = ans[i + 1];
        } else {
            ans[i] = pref_max[i];
        }
    }
    for (int i = 0; i < n; ++i) {
        cout << ans[i] << " \n"[i == n - 1];
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}
