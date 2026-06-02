#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n;
        long long m;
        cin >> n >> m;
        vector<long long> a(n), c(n);
        for (int i = 0; i < n; ++i) cin >> a[i];
        for (int i = 0; i < n; ++i) cin >> c[i];
        
        unordered_map<long long, long long> cnt;
        for (int i = 0; i < n; ++i) {
            cnt[a[i]] = c[i];
        }
        
        long long ans = 0;
        for (auto& [x, cx] : cnt) {
            long long k1 = min(cx, m / x);
            long long coins = m - k1 * x;
            
            auto it = cnt.find(x + 1);
            if (it != cnt.end()) {
                long long cx1 = it->second;
                long long k2 = min(cx1, coins / (x + 1));
                coins -= k2 * (x + 1);
                long long r = min({k1, cx1 - k2, coins});
                long long petals = (k1 - r) * x + (k2 + r) * (x + 1);
                ans = max(ans, petals);
            } else {
                ans = max(ans, k1 * x);
            }
        }
        cout << ans << '\n';
    }
    return 0;
}
