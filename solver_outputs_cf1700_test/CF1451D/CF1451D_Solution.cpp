// Solution
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        long long d, k;
        cin >> d >> k;
        
        long long z = 0;
        // Find maximum z such that (k*z, k*z) is within the circle
        // Condition: 2 * (k*z)^2 <= d^2
        // We can find z by binary search or directly
        long long low = 0, high = d / k + 1;
        while (low <= high) {
            long long mid = (low + high) / 2;
            if (2LL * mid * mid * k * k <= d * d) {
                z = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        
        // Check if (k*z, k*(z+1)) is within the circle
        if ((k * z) * (k * z) + (k * (z + 1)) * (k * (z + 1)) <= d * d) {
            cout << "Ashish\n";
        } else {
            cout << "Utkarsh\n";
        }
    }
    
    return 0;
}
