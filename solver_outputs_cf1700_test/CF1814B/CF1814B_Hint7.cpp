// Hint7
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        long long a, b;
        cin >> a >> b;
        if (a > b) swap(a, b);
        
        long long ans = LLONG_MAX;
        // Try all possible final leg lengths m from 1 to about sqrt(b) + some margin
        // Actually we can limit m up to around 2*sqrt(b) or just up to 1e5 for safety
        // But a,b up to 1e9, so sqrt is ~31623, we can go a bit higher
        for (long long m = 1; m * m <= 2 * b + 10; ++m) {
            long long moves = (m - 1); // increases to reach length m
            // jumps needed in x and y directions
            long long jumps_x = (a + m - 1) / m;
            long long jumps_y = (b + m - 1) / m;
            moves += jumps_x + jumps_y;
            ans = min(ans, moves);
        }
        cout << ans << '\n';
    }
    return 0;
}
