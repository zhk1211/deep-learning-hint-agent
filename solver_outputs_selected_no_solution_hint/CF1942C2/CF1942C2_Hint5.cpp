#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        long long n;
        int x, y;
        cin >> n >> x >> y;
        vector<long long> a(x);
        for (int i = 0; i < x; ++i) {
            cin >> a[i];
        }
        sort(a.begin(), a.end());
        
        long long ans = x - 2; // triangles from chosen vertices alone
        
        vector<long long> gaps;
        for (int i = 0; i < x; ++i) {
            long long diff;
            if (i + 1 < x) {
                diff = a[i + 1] - a[i];
            } else {
                diff = (a[0] + n) - a[i];
            }
            if (diff >= 2) {
                gaps.push_back(diff);
            }
        }
        
        sort(gaps.begin(), gaps.end(), [](long long d1, long long d2) {
            int p1 = (d1 % 2 == 0) ? 1 : 0;
            int p2 = (d2 % 2 == 0) ? 1 : 0;
            if (p1 != p2) return p1 < p2;
            return d1 < d2;
        });
        
        int rem = y;
        for (long long d : gaps) {
            if (rem <= 0) break;
            long long k = d / 2;
            if (d % 2 == 0) {
                // even gap
                long long use = min((long long)rem, k - 1);
                if (use > 0) {
                    ans += 2 * use;
                    rem -= use;
                    if (use == k - 1) {
                        ans += 1; // the extra triangle from closing the gap
                    }
                }
            } else {
                // odd gap
                long long use = min((long long)rem, k);
                ans += 2 * use;
                rem -= use;
            }
        }
        
        cout << ans << '\n';
    }
    return 0;
}
