// Hint10
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n, k;
        cin >> n >> k;
        vector<long long> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }
        
        long long total_damage = 0;
        for (int i = 0; i < n; ++i) {
            total_damage += a[i];
        }
        
        vector<long long> penalty(n);
        for (int i = 0; i < n; ++i) {
            penalty[i] = a[i] + i;
        }
        
        sort(penalty.begin(), penalty.end(), greater<long long>());
        
        long long dodge = 0;
        for (int i = 0; i < k; ++i) {
            dodge += penalty[i];
        }
        
        long long extra = (long long)k * (k - 1) / 2;
        long long ans = total_damage + (long long)n * k - dodge - extra;
        cout << ans << '\n';
    }
    
    return 0;
}
