// Hint7
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
        
        // We need k hot and k-1 cold, total 2k-1 cups
        // Temperature = (k*h + (k-1)*c) / (2k-1)
        // We want this close to t
        // Solve: k*h + (k-1)*c = t*(2k-1)
        // k*h + k*c - c = 2k*t - t
        // k*(h + c - 2t) = c - t
        // k = (t - c) / (2*t - h - c)
        
        long long num = t - c;
        long long den = 2 * t - h - c;
        
        // k is the integer part
        long long k = num / den;
        
        // Check k and k+1
        long long best_k = k;
        double best_diff = abs((double)(k * h + (k - 1) * c) / (2 * k - 1) - t);
        
        for (long long cand : {k, k + 1}) {
            if (cand <= 0) continue;
            double val = (double)(cand * h + (cand - 1) * c) / (2 * cand - 1);
            double diff = abs(val - t);
            if (diff < best_diff - 1e-12) {
                best_diff = diff;
                best_k = cand;
            } else if (abs(diff - best_diff) < 1e-12 && 2 * cand - 1 < 2 * best_k - 1) {
                best_k = cand;
            }
        }
        
        // Also compare with 2 cups
        double two_cups = (h + c) / 2.0;
        double diff2 = abs(two_cups - t);
        if (diff2 < best_diff - 1e-12) {
            cout << 2 << "\n";
        } else if (abs(diff2 - best_diff) < 1e-12 && 2 < 2 * best_k - 1) {
            cout << 2 << "\n";
        } else {
            cout << 2 * best_k - 1 << "\n";
        }
    }
    return 0;
}
