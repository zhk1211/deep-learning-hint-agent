#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<int> c(n);
        for (int i = 0; i < n; ++i) {
            cin >> c[i];
        }
        
        bool ok = true;
        for (int i = 0; i < n - 1; ++i) {
            if (c[i + 1] - c[i] > 1) {
                ok = false;
                break;
            }
        }
        
        // Also need to check that c contains exactly one 1
        int cnt1 = 0;
        for (int x : c) {
            if (x == 1) cnt1++;
        }
        if (cnt1 != 1) ok = false;
        
        cout << (ok ? "YES" : "NO") << '\n';
    }
    
    return 0;
}
