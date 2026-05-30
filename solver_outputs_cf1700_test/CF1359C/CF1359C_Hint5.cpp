// Hint5
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
        
        // We need k hot and k-1 cold (odd total cups: 2k-1)
        // Temperature = (k*h + (k-1)*c) / (2k-1)
        // We want this close to t
        // Solve: (k*h + (k-1)*c) / (2k-1) = t
        // k*h + k*c - c = 2k*t - t
        // k*(h + c - 2t) = c - t
        // k = (t - c) / (2*t - h - c)
        
        long long num = t - c;
        long long den = 2 * t - h - c;
        long long k = num / den;
        
        long long best_k = k;
        double best_diff = abs((double)(k * h + (k - 1) * c) / (2 * k - 1) - t);
        
        for (long long cand : {k, k + 1}) {
            if (cand <= 0) continue;
            double val = (double)(cand * h + (cand - 1) * c) / (2 * cand - 1);
            double diff = abs(val - t);
            if (diff < best_diff) {
                best_diff = diff;
                best_k = cand;
            } else if (diff == best_diff && 2 * cand - 1 < 2 * best_k - 1) {
                best_k = cand;
            }
        }
        
        // Compare with 2 cups
        double two_cups = (h + c) / 2.0;
        double diff2 = abs(two_cups - t);
        double diff_best = abs((double)(best_k * h + (best_k - 1) * c) / (2 * best_k - 1) - t);
        
        if (diff2 < diff_best) {
            cout << 2 << "\n";
        } else if (diff2 == diff_best) {
            cout << min(2, 2 * (int)best_k - 1) << "\n";
        } else {
            cout << 2 * best_k - 1 << "\n";
        }
    }
    return 0;
}
