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
        
        long long ans = 0;
        for (long long x : a) ans += x;
        
        auto solve = [&](long long m) {
            vector<long long> b(n);
            for (int i = 0; i < n; ++i) {
                b[i] = a[i] % m;
            }
            vector<long long> pref(n + 1, 0);
            for (int i = 0; i < n; ++i) {
                pref[i + 1] = pref[i] + b[i];
            }
            vector<long long> dp(n + 1, 0);
            long long best = 0;
            for (int i = 1; i <= n; ++i) {
                dp[i] = dp[i - 1];
                if (i >= k) {
                    best = max(best, dp[i - k] - pref[i - k]);
                    dp[i] = max(dp[i], best + pref[i]);
                }
            }
            long long sum_b = pref[n];
            long long max_saved = dp[n];
            return sum_b - max_saved;
        };
        
        long long res_p = solve(p);
        long long res_q = solve(q);
        long long res_pq = LLONG_MAX;
        {
            vector<long long> b(n);
            for (int i = 0; i < n; ++i) {
                b[i] = a[i] % p % q;
            }
            vector<long long> pref(n + 1, 0);
            for (int i = 0; i < n; ++i) {
                pref[i + 1] = pref[i] + b[i];
            }
            vector<long long> dp(n + 1, 0);
            long long best = 0;
            for (int i = 1; i <= n; ++i) {
                dp[i] = dp[i - 1];
                if (i >= k) {
                    best = max(best, dp[i - k] - pref[i - k]);
                    dp[i] = max(dp[i], best + pref[i]);
                }
            }
            long long sum_b = pref[n];
            long long max_saved = dp[n];
            res_pq = sum_b - max_saved;
        }
        {
            vector<long long> b(n);
            for (int i = 0; i < n; ++i) {
                b[i] = a[i] % q % p;
            }
            vector<long long> pref(n + 1, 0);
            for (int i = 0; i < n; ++i) {
                pref[i + 1] = pref[i] + b[i];
            }
            vector<long long> dp(n + 1, 0);
            long long best = 0;
            for (int i = 1; i <= n; ++i) {
                dp[i] = dp[i - 1];
                if (i >= k) {
                    best = max(best, dp[i - k] - pref[i - k]);
                    dp[i] = max(dp[i], best + pref[i]);
                }
            }
            long long sum_b = pref[n];
            long long max_saved = dp[n];
            res_pq = min(res_pq, sum_b - max_saved);
        }
        
        ans = min({ans, res_p, res_q, res_pq});
        cout << ans << '\n';
    }
    return 0;
}
