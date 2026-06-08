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
        for (int i = 0; i < x; i++) {
            cin >> a[i];
        }
        sort(a.begin(), a.end());
        
        long long ans = x - 2;
        vector<long long> gaps;
        for (int i = 0; i < x; i++) {
            long long diff;
            if (i + 1 < x) {
                diff = a[i+1] - a[i];
            } else {
                diff = a[0] + n - a[i];
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
        
        for (long long d : gaps) {
            if (y == 0) break;
            long long add = d / 2;
            if (d % 2 == 0) {
                long long need = add - 1;
                if (need <= 0) {
                    ans += add;
                    continue;
                }
                long long take = min((long long)y, need);
                y -= take;
                ans += take;
                if (take == need) {
                    ans += 1;
                } else {
                    ans += take;
                }
            } else {
                long long take = min((long long)y, add);
                y -= take;
                ans += take;
            }
        }
        
        cout << ans << "\n";
    }
    return 0;
}
