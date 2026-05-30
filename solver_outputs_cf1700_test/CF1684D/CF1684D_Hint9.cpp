// Hint9
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
        
        long long total_base = 0;
        for (int i = 0; i < n; ++i) {
            total_base += a[i];
        }
        
        // We will choose up to k traps to jump over.
        // If we jump over trap i, we avoid a[i] damage but we also avoid the bonus damage
        // that would have been added to all subsequent traps.
        // The bonus damage avoided by jumping over trap i is (n - 1 - i).
        // So the total damage avoided by jumping over trap i is a[i] + (n - 1 - i).
        // We want to maximize the avoided damage.
        vector<long long> avoid(n);
        for (int i = 0; i < n; ++i) {
            avoid[i] = a[i] + (n - 1 - i);
        }
        sort(avoid.rbegin(), avoid.rend());
        
        long long max_avoid = 0;
        for (int i = 0; i < k; ++i) {
            max_avoid += avoid[i];
        }
        
        // Total damage if we jump over none: total_base + (0 + 1 + ... + n-1) = total_base + n*(n-1)/2
        // But we must subtract the avoided damage.
        long long total_bonus = (long long)n * (n - 1) / 2;
        long long total_damage = total_base + total_bonus - max_avoid;
        
        cout << total_damage << '\n';
    }
    
    return 0;
}
