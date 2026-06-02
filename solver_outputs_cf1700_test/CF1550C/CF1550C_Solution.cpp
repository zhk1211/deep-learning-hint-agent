#include <bits/stdc++.h>
using namespace std;

bool is_good(const vector<int>& a, int l, int r) {
    int len = r - l + 1;
    if (len <= 2) return true;
    for (int i = l; i <= r; ++i) {
        for (int j = i + 1; j <= r; ++j) {
            for (int k = j + 1; k <= r; ++k) {
                if ((a[i] <= a[j] && a[j] <= a[k]) ||
                    (a[i] >= a[j] && a[j] >= a[k])) {
                    return false;
                }
            }
        }
    }
    return true;
}

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    long long ans = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n && j - i + 1 <= 4; ++j) {
            if (is_good(a, i, j)) {
                ++ans;
            }
        }
    }
    cout << ans << '\n';
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
