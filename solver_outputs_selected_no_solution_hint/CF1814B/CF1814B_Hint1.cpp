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
        // Try all possible final leg lengths m from 1 to about sqrt(b) + some range
        // Actually we can iterate m up to about 2*sqrt(b) or so.
        // But a tighter bound: m will not exceed roughly sqrt(a+b) + something.
        // Let's iterate m from 1 to 100000 (since a,b up to 1e9, sqrt is ~31623)
        // But we can also consider larger m by solving quadratic.
        
        long long limit = min(100000LL, b);
        for (long long m = 1; m <= limit; ++m) {
            // moves to increase legs to m: m-1
            long long moves = m - 1;
            // jumps needed in x and y directions
            long long jumps_x = (a + m - 1) / m;
            long long jumps_y = (b + m - 1) / m;
            moves += jumps_x + jumps_y;
            ans = min(ans, moves);
        }
        
        // For larger m, we can compute directly.
        // Let m be large, then jumps_x = ceil(a/m), jumps_y = ceil(b/m).
        // Total moves = m - 1 + ceil(a/m) + ceil(b/m).
        // We can find optimal m by checking around sqrt(a) and sqrt(b).
        // But since we already checked up to 1e5, and a,b up to 1e9, sqrt is ~31623,
        // so we already covered the optimal region. However, to be safe, we can also
        // check m = floor(sqrt(a)) and floor(sqrt(b)) and nearby values.
        
        vector<long long> candidates;
        for (long long k = 1; k * k <= a; ++k) {
            candidates.push_back(k);
            candidates.push_back(a / k);
            candidates.push_back(a / k + 1);
        }
        for (long long k = 1; k * k <= b; ++k) {
            candidates.push_back(k);
            candidates.push_back(b / k);
            candidates.push_back(b / k + 1);
        }
        candidates.push_back(1);
        candidates.push_back(max(a, b));
        
        for (long long m : candidates) {
            if (m < 1 || m > 1e9) continue;
            long long moves = m - 1;
            long long jumps_x = (a + m - 1) / m;
            long long jumps_y = (b + m - 1) / m;
            moves += jumps_x + jumps_y;
            ans = min(ans, moves);
        }
        
        cout << ans << '\n';
    }
    return 0;
}
