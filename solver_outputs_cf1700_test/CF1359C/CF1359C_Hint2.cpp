// Hint2
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int T;
    cin >> T;
    while (T--) {
        long long h, c, t;
        cin >> h >> c >> t;
        
        if (t == h) {
            cout << 1 << "\n";
            continue;
        }
        if (2 * t <= h + c) {
            cout << 2 << "\n";
            continue;
        }
        
        // We need odd number of cups: 2k+1 cups, k+1 hot, k cold
        // Temperature = ((k+1)*h + k*c) / (2k+1)
        // We want this close to t.
        // Solve for k: ((k+1)*h + k*c) = t*(2k+1)
        // => k*(h + c - 2t) = t - h
        // => k = (t - h) / (h + c - 2t)
        // Since h + c - 2t < 0 (because 2t > h+c), and t - h < 0, k is positive.
        long long num = t - h;
        long long den = h + c - 2 * t;
        long long k = num / den;
        
        // Check k and k+1
        long long best_k = -1;
        long double best_diff = 1e18;
        for (long long cand : {k, k + 1}) {
            if (cand < 0) continue;
            long long hot = cand + 1;
            long long cold = cand;
            long double temp = (long double)(hot * h + cold * c) / (hot + cold);
            long double diff = fabsl(temp - t);
            if (diff < best_diff) {
                best_diff = diff;
                best_k = cand;
            } else if (diff == best_diff && cand < best_k) {
                best_k = cand;
            }
        }
        
        // Also consider 2 cups (even case)
        long double even_temp = (h + c) / 2.0;
        long double even_diff = fabsl(even_temp - t);
        if (even_diff < best_diff) {
            cout << 2 << "\n";
        } else if (even_diff == best_diff) {
            cout << min(2LL, 2 * best_k + 1) << "\n";
        } else {
            cout << 2 * best_k + 1 << "\n";
        }
    }
    return 0;
}
