// Hint0
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
        for (int i = 0; i < n; ++i) {
            ans = max(ans, b[i] + i);
        }
        
        vector<long long> pref_max(n), suff_max(n);
        pref_max[0] = b[0] - 0;
        for (int i = 1; i < n; ++i) {
            pref_max[i] = max(pref_max[i-1], b[i] - i);
        }
        suff_max[n-1] = b[n-1] + (n-1);
        for (int i = n-2; i >= 0; --i) {
            suff_max[i] = max(suff_max[i+1], b[i] + i);
        }
        
        long long best = LLONG_MIN;
        for (int j = 1; j < n-1; ++j) {
            best = max(best, pref_max[j-1] + b[j] + suff_max[j+1]);
        }
        
        cout << best << '\n';
    }
    
    return 0;
}
