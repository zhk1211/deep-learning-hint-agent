// Hint0
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
        // For b > x, floor(a/b) = 0, so we need a mod b = 0 => a = 0, impossible.
        // So b <= x.
        // Let k = floor(a/b) = a mod b.
        // Then a = k*b + k = k*(b+1), with 0 <= k < b.
        // Also a <= x, b <= y.
        // So k*(b+1) <= x, k < b, b <= y.
        // For each b, k ranges from 1 to min(b-1, x/(b+1)).
        // We can iterate over k or b. Since x,y up to 1e9, we need O(sqrt(x)) or similar.
        // Let's iterate over k.
        // For a fixed k, we need b > k, b <= y, and k*(b+1) <= x => b+1 <= x/k => b <= x/k - 1.
        // So b_max = min(y, x/k - 1).
        // Valid b are from k+1 to b_max.
        // So for each k, add max(0LL, b_max - k).
        // k can go up to sqrt(x) roughly, because k*(k+1) <= k*(b+1) <= x => k^2 < x.
        for (long long k = 1; k * (k + 1) <= x && k < y; ++k) {
            long long b_max = min(y, x / k - 1);
            if (b_max > k) {
                ans += b_max - k;
            }
        }
        cout << ans << '\n';
    }
    return 0;
}
