// Hint5
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
        
        vector<long long> pref_max(n), suff_max(n);
        for (int i = 0; i < n; ++i) {
            pref_max[i] = b[i] + i;
            if (i > 0) pref_max[i] = max(pref_max[i], pref_max[i-1]);
        }
        for (int i = n-1; i >= 0; --i) {
            suff_max[i] = b[i] - i;
            if (i < n-1) suff_max[i] = max(suff_max[i], suff_max[i+1]);
        }
        
        long long ans = LLONG_MIN;
        for (int j = 1; j < n-1; ++j) {
            long long cur = pref_max[j-1] + b[j] + suff_max[j+1];
            ans = max(ans, cur);
        }
        cout << ans << '\n';
    }
    return 0;
}
