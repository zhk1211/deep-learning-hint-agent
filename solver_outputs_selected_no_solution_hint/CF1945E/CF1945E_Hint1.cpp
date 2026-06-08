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
    
    // We need p[l] == x after at most 2 swaps.
    // First, swap x into position l.
    cout << "1\n";
    cout << pos_x << " " << l << "\n";
    swap(p[pos_x], p[l]);
    pos_x = l;
    
    // Now run binary search again to see if it's correct.
    l = 1, r = n + 1;
    while (r - l > 1) {
        int m = (l + r) / 2;
        if (p[m] <= x) l = m;
        else r = m;
    }
    
    if (p[l] == x) return;
    
    // If still not correct, we need one more swap.
    // The binary search path is determined by comparisons with x.
    // We need to place x at the final l. Currently x is at pos_x.
    // The final l is where binary search ends. We can just swap x with that position.
    cout << "1\n";
    cout << pos_x << " " << l << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) solve();
    return 0;
}
