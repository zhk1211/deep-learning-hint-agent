// Hint3
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
        vector<long long> b(n);
        for (int i = 0; i < n; ++i) {
            cin >> b[i];
        }
        
        long long ans = LLONG_MIN;
        for (int i = 0; i < n; ++i) {
            ans = max(ans, b[i] - i);
        }
        long long best = LLONG_MIN;
        for (int i = 0; i < n; ++i) {
            if (i > 0) {
                ans = max(ans, best + b[i] - i);
            }
            best = max(best, b[i] + i);
        }
        best = LLONG_MIN;
        for (int i = 0; i < n; ++i) {
            if (i > 1) {
                ans = max(ans, best + b[i] - i);
            }
            if (i > 0) {
                best = max(best, b[i] + i + b[i-1] - (i-1));
            }
        }
        
        cout << ans << '\n';
    }
    
    return 0;
}
