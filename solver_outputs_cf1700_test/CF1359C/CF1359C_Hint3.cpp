// Hint3
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
        
        // If target is exactly hot or cold
        if (t == h) {
            cout << 1 << "\n";
            continue;
        }
        if (t * 2 <= h + c) {
            cout << 2 << "\n";
            continue;
        }
        
        // For odd number of cups: k hot, k-1 cold (total 2k-1)
        // Temperature = (k*h + (k-1)*c) / (2k-1)
        // We want this as close to t as possible.
        // Solve for k: (k*h + (k-1)*c) / (2k-1) = t
        // k*h + k*c - c = t*(2k-1)
        // k*(h+c) - c = 2k*t - t
        // k*(h+c - 2t) = c - t
        // k = (t - c) / (2*t - h - c)
        
        long long num = t - c;
        long long den = 2 * t - h - c;
        
        // k is number of hot cups
        long long k = num / den;
        
        // Check k and k+1
        long long best_k = -1;
        double best_diff = 1e18;
        
        for (long long cand : {k, k + 1}) {
            if (cand <= 0) continue;
            long long hot = cand;
            long long cold = cand - 1;
            double temp = (double)(hot * h + cold * c) / (hot + cold);
            double diff = fabs(temp - t);
            if (diff < best_diff) {
                best_diff = diff;
                best_k = cand;
            } else if (fabs(diff - best_diff) < 1e-12 && cand < best_k) {
                best_k = cand;
            }
        }
        
        // Compare with 2 cups (even case)
        double even_temp = (h + c) / 2.0;
        double even_diff = fabs(even_temp - t);
        
        if (even_diff < best_diff) {
            cout << 2 << "\n";
        } else if (fabs(even_diff - best_diff) < 1e-12) {
            cout << min(2LL, 2 * best_k - 1) << "\n";
        } else {
            cout << 2 * best_k - 1 << "\n";
        }
    }
    
    return 0;
}
