// Hint1
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        long long x, y;
        cin >> x >> y;
        long long ans = 0;
        // k = floor(a/b) = a mod b, a = k*b + k, b > k
        // a <= x, b <= y, k >= 1
        // k*b + k <= x => b <= (x - k)/k = x/k - 1
        // also b > k and b <= y
        // so for each k, number of b is max(0, min(y, x/k - 1) - k)
        for (long long k = 1; k * k <= x; ++k) {
            long long max_b = min(y, x / k - 1);
            if (max_b > k) {
                ans += max_b - k;
            }
        }
        cout << ans << '\n';
    }
    return 0;
}
