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
        
        if (t * 2 <= h + c) {
            cout << 2 << "\n";
            continue;
        }
        
        long long k = (h - t) / (2 * t - h - c);
        
        auto val = [&](long long k) -> long double {
            return (long double)(k * (h + c) + h) / (2 * k + 1);
        };
        
        long double diff1 = fabsl(val(k) - t);
        long double diff2 = fabsl(val(k + 1) - t);
        
        if (diff1 <= diff2) {
            cout << 2 * k + 1 << "\n";
        } else {
            cout << 2 * (k + 1) + 1 << "\n";
        }
    }
    
    return 0;
}
