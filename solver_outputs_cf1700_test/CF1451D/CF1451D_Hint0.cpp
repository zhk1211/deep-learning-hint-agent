// Hint0
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
        
        long long max_moves = 0;
        long long low = 0, high = d / k;
        while (low <= high) {
            long long mid = (low + high) / 2;
            long long x = mid * k;
            long long y = mid * k;
            if (x * x + y * y <= d * d) {
                max_moves = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        
        long long x = max_moves * k;
        long long y = (max_moves + 1) * k;
        if (x * x + y * y <= d * d) {
            cout << "Ashish\n";
        } else {
            cout << "Utkarsh\n";
        }
    }
    
    return 0;
}
