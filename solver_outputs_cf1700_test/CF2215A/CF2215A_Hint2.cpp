// Hint2
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

        // dp[i][0] = min sum for prefix i when last operation was mod p
        // dp[i][1] = min sum for prefix i when last operation was mod q
        // dp[i][2] = min sum for prefix i when no operation applied yet (or original)
        const long long INF = 1e18;
        vector<array<long long, 3>> dp(n + 1, {INF, INF, INF});
        dp[0][0] = dp[0][1] = dp[0][2] = 0;

        for (int i = 1; i <= n; ++i) {
            long long val = a[i - 1];
            long long modp = val % p;
            long long modq = val % q;
            long long best_mod = min(modp, modq);

            // Option 1: start a new segment at i (length 1, but need k length to apply)
            // Actually we can only apply operation if segment length >= k.
            // We'll handle transitions with segment logic.

            // For each state, we can either not apply operation on i, or apply operation
            // as part of a segment that started earlier.

            // We'll maintain best values for segments ending at i.
            // Let's compute dp directly with segment consideration.

            // dp[i][state] = min over:
            // - dp[i-1][state] + val (no operation on i)
            // - if we can end a segment at i of length >= k:
            //   min over start j <= i-k+1 of dp[j-1][any] + (i-j+1)*mod_value
            // But we can optimize using prefix minima.

            // We'll keep running minima for each mod choice.
        }

        // We'll use a different approach: since operations can overlap, the optimal
        // is to reduce each element to either a[i]%p or a[i]%q, but we must ensure
        // that we can cover all chosen reductions with segments of length >= k.
        // This is equivalent to: we can reduce a[i] if it's part of some segment of length >= k
        // where all elements in that segment are reduced using the same modulus.
        // So we need to partition the array into segments of length >= k (except possibly a suffix
        // of unmodified elements) where each segment uses one modulus.

        // Let's compute DP: dp[i] = min sum for prefix i.
        // dp[i] = min(dp[i-1] + a[i], min over j <= i-k+1 of dp[j-1] + cost(j, i, mod))
        // where cost(j, i, mod) = sum_{t=j}^i (a[t] % mod)
        // We can precompute prefix sums of a, a%p, a%q.

        vector<long long> pref_a(n + 1, 0), pref_p(n + 1, 0), pref_q(n + 1, 0);
        for (int i = 0; i < n; ++i) {
            pref_a[i + 1] = pref_a[i] + a[i];
            pref_p[i + 1] = pref_p[i] + (a[i] % p);
            pref_q[i + 1] = pref_q[i] + (a[i] % q);
        }

        vector<long long> dp2(n + 1, INF);
        dp2[0] = 0;
        // We'll maintain the best dp[j-1] - pref_p[j-1] and similar for q.
        // For each i, we consider segments ending at i.
        // We need j <= i - k + 1.
        // We'll update the best values as we move i.

        long long best_p = INF, best_q = INF;
        // best_p = min_{j <= i-k+1} (dp2[j-1] - pref_p[j-1])
        // Then dp2[i] = min(dp2[i], best_p + pref_p[i])

        for (int i = 1; i <= n; ++i) {
            // Option: no operation on i
            dp2[i] = dp2[i - 1] + a[i - 1];

            // If we can start a segment ending at i of length >= k
            if (i >= k) {
                int j = i - k + 1;
                // Update best with j
                best_p = min(best_p, dp2[j - 1] - pref_p[j - 1]);
                best_q = min(best_q, dp2[j - 1] - pref_q[j - 1]);
            }

            // Apply segment using p
            if (best_p != INF) {
                dp2[i] = min(dp2[i], best_p + pref_p[i]);
            }
            // Apply segment using q
            if (best_q != INF) {
                dp2[i] = min(dp2[i], best_q + pref_q[i]);
            }
        }

        cout << dp2[n] << '\n';
    }

    return 0;
}
