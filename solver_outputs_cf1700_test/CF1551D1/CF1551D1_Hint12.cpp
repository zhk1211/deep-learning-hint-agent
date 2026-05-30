// Hint12
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
            // Both even
            if (k % 2 == 0 && k <= (m / 2) * n) {
                possible = true;
            }
        } else if (n % 2 == 1) {
            // n odd, m even
            // We must place m/2 horizontal dominoes in the last row
            int min_h = m / 2;
            if (k >= min_h && (k - min_h) % 2 == 0 && k <= (m / 2) * n) {
                possible = true;
            }
        } else {
            // m odd, n even
            // We can place vertical dominoes in the last column
            // No forced horizontal dominoes
            if (k % 2 == 0 && k <= (m / 2) * n) {
                possible = true;
            }
        }
        
        cout << (possible ? "YES" : "NO") << '\n';
    }
    
    return 0;
}
