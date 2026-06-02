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
        long long ans = a + b;
        for (int m = 1; m <= 100000; ++m) {
            long long cur = (a + m - 1) / m + (b + m - 1) / m + (m - 1);
            if (cur < ans) ans = cur;
        }
        cout << ans << '\n';
    }
    return 0;
}
