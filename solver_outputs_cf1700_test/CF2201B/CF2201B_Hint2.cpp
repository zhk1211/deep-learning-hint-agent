// Hint2
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, k;
    cin >> n >> k;
    
    if (k < n || k > 2 * n - 1) {
        cout << "NO\n";
        return;
    }
    
    cout << "YES\n";
    vector<int> a(2 * n);
    
    if (k == 2 * n - 1) {
        // Special case: maximum operations
        for (int i = 0; i < n; i++) {
            a[i] = i + 1;
            a[i + n] = i + 1;
        }
    } else {
        int pairs_to_shift = k - n;
        // First n positions: 1..n
        for (int i = 0; i < n; i++) {
            a[i] = i + 1;
        }
        // Place second copies
        // For pairs that need 2 operations, place them after position n
        // For pairs that need 1 operation, place them at the end
        int pos = n;
        for (int i = 0; i < pairs_to_shift; i++) {
            a[pos++] = i + 1;
        }
        for (int i = pairs_to_shift; i < n; i++) {
            a[pos++] = i + 1;
        }
    }
    
    for (int i = 0; i < 2 * n; i++) {
        cout << a[i] << " \n"[i == 2 * n - 1];
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
