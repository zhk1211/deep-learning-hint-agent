#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, x;
    cin >> n >> x;
    vector<int> p(n + 1);
    int pos_x = -1;
    for (int i = 1; i <= n; ++i) {
        cin >> p[i];
        if (p[i] == x) pos_x = i;
    }

    int l = 1, r = n + 1;
    while (r - l > 1) {
        int m = (l + r) / 2;
        if (p[m] <= x) l = m;
        else r = m;
    }

    if (p[l] == x) {
        cout << "0\n";
        return;
    }

    // We need at most 2 swaps.
    // First, swap x into the position that binary search will check.
    // The binary search ends at l, so we can swap pos_x with l.
    // But we must ensure that after swap, the binary search still ends at the same l.
    // Actually, we can just swap pos_x with l, then run binary search again to see if it works.
    // If not, we can do one more swap to fix.
    
    cout << "1\n";
    cout << pos_x << " " << l << "\n";
    swap(p[pos_x], p[l]);
    
    // Run binary search again
    int l2 = 1, r2 = n + 1;
    while (r2 - l2 > 1) {
        int m = (l2 + r2) / 2;
        if (p[m] <= x) l2 = m;
        else r2 = m;
    }
    
    if (p[l2] == x) {
        // Already correct
        return;
    }
    
    // Need second swap: swap x (now at l) with l2
    cout << "1\n";
    cout << l << " " << l2 << "\n";
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
