// Hint11
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n, m, k;
        cin >> n >> m >> k;
        
        int total = n * m / 2;
        int vertical = total - k;
        
        bool possible = false;
        
        // Check if k horizontal dominoes are possible
        if (n % 2 == 0 && m % 2 == 0) {
            // Both even: k must be even
            if (k % 2 == 0) possible = true;
        } else if (n % 2 == 1) {
            // n odd, m even
            // We must place m/2 horizontal dominoes in the last row
            int min_horiz = m / 2;
            if (k >= min_horiz && (k - min_horiz) % 2 == 0) possible = true;
        } else {
            // m odd, n even
            // We can place at most n*(m-1)/2 horizontal dominoes
            // and k must be even because vertical dominoes come in pairs horizontally
            if (k % 2 == 0 && k <= n * (m - 1) / 2) possible = true;
        }
        
        cout << (possible ? "YES" : "NO") << '\n';
    }
    
    return 0;
}
