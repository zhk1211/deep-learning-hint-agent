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
    // First, swap x to its correct position (pos_x) with the element at l.
    // Then run binary search again to see if it works.
    // If not, we need one more swap to fix the binary search path.
    cout << "1\n" << pos_x << " " << l << "\n";
    swap(p[pos_x], p[l]);
    pos_x = l;

    l = 1, r = n + 1;
    while (r - l > 1) {
        int m = (l + r) / 2;
        if (p[m] <= x) l = m;
        else r = m;
    }

    if (p[l] == x) return;

    // Now we need a second swap.
    // The binary search ended at l, but p[l] != x.
    // We need to swap the element at l with the element at pos_x (which is x).
    // But pos_x might have changed? Actually pos_x is where x is now.
    // Wait, we already swapped x to l, so pos_x == l.
    // But after the second binary search, l might be different.
    // So we need to find where x is now.
    int new_pos_x = -1;
    for (int i = 1; i <= n; ++i) {
        if (p[i] == x) {
            new_pos_x = i;
            break;
        }
    }
    cout << "1\n" << new_pos_x << " " << l << "\n";
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
