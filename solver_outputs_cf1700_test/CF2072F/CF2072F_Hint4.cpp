// Hint4
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n;
        unsigned int k;
        cin >> n >> k;
        
        vector<unsigned int> ans(n);
        for (int i = 0; i < n; ++i) {
            // Check if C(n-1, i) is odd
            if ((i & (n - 1 - i)) == 0) {
                ans[i] = k;
            } else {
                ans[i] = 0;
            }
        }
        
        for (int i = 0; i < n; ++i) {
            cout << ans[i] << (i + 1 == n ? '\n' : ' ');
        }
    }
    return 0;
}
