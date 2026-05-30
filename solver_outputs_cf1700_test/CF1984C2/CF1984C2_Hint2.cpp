// Hint2
#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<long long> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }
        
        long long min_sum = 0, max_sum = 0;
        for (int i = 0; i < n; ++i) {
            long long new_min = min_sum + a[i];
            long long new_max = max_sum + a[i];
            long long abs_min = abs(min_sum + a[i]);
            long long abs_max = abs(max_sum + a[i]);
            min_sum = min({new_min, new_max, abs_min, abs_max});
            max_sum = max({new_min, new_max, abs_min, abs_max});
        }
        long long k = max_sum;
        
        vector<long long> suf_max(n + 1, LLONG_MIN);
        suf_max[n] = 0;
        long long cur = 0;
        for (int i = n - 1; i >= 0; --i) {
            cur += a[i];
            suf_max[i] = max(suf_max[i + 1], cur);
        }
        
        vector<long long> dp_min(n + 1), dp_max(n + 1);
        dp_min[0] = dp_max[0] = 0;
        for (int i = 0; i < n; ++i) {
            dp_min[i + 1] = min(dp_min[i] + a[i], abs(dp_min[i] + a[i]));
            dp_max[i + 1] = max(dp_max[i] + a[i], abs(dp_max[i] + a[i]));
        }
        
        vector<long long> pow2(n + 1);
        pow2[0] = 1;
        for (int i = 1; i <= n; ++i) {
            pow2[i] = (pow2[i - 1] * 2) % MOD;
        }
        
        long long ans = 0;
        long long pref = 0;
        for (int i = 0; i < n; ++i) {
            pref += a[i];
            long long min_reach = dp_min[i];
            long long max_reach = dp_max[i];
            long long min_abs = abs(pref);
            long long max_abs = abs(pref);
            long long min_after = min_reach + a[i];
            long long max_after = max_reach + a[i];
            long long min_abs_after = abs(min_reach + a[i]);
            long long max_abs_after = abs(max_reach + a[i]);
            long long cur_min = min({min_after, max_after, min_abs_after, max_abs_after});
            long long cur_max = max({min_after, max_after, min_abs_after, max_abs_after});
            long long need_min = k - suf_max[i + 1];
            long long need_max = k - suf_max[i + 1];
            if (cur_min <= need_max && cur_max >= need_min) {
                long long ways = pow2[n - 1 - i];
                ans = (ans + ways) % MOD;
            }
        }
        cout << ans << '\n';
    }
    return 0;
}
