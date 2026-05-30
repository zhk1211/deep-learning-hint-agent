// Hint3
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
        long long ans = LLONG_MAX;
        for (long long m = 1; m <= 100000; m++) {
            long long moves = (m - 1) + (a + m - 1) / m + (b + m - 1) / m;
            ans = min(ans, moves);
        }
        cout << ans << '\n';
    }
    return 0;
}
