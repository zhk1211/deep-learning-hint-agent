// Hint3
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        long long n;
        cin >> n;
        
        long long lo = 0, hi = 1e6;
        long long ans = hi;
        while (lo <= hi) {
            long long mid = (lo + hi) / 2;
            long long k = mid / 3;
            long long r = mid % 3;
            long long triangles = 3 * k * (k + 1);
            if (r == 1) triangles += 2 * (k + 1);
            else if (r == 2) triangles += 2 * (k + 1) + (2 * k + 1);
            
            if (triangles >= n) {
                ans = mid;
                hi = mid - 1;
            } else {
                lo = mid + 1;
            }
        }
        cout << ans << '\n';
    }
    return 0;
}
