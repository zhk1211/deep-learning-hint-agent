// Hint1
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n, k;
        long long p, q;
        cin >> n >> k >> p >> q;
        vector<long long> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }
        
        long long g = gcd(p, q);
        long long ans = 0;
        for (int i = 0; i < n; ) {
            int j = i;
            while (j < n && a[j] % g == 0) {
                ++j;
            }
            int len = j - i;
            if (len >= k) {
                // all can be reduced to 0
            } else {
                for (int x = i; x < j; ++x) {
                    ans += a[x] % g;
                }
            }
            i = j;
            while (i < n && a[i] % g != 0) {
                ans += a[i] % g;
                ++i;
            }
        }
        cout << ans << '\n';
    }
    return 0;
}
