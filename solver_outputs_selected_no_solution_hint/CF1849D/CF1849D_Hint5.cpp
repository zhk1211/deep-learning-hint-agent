#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    // dp[i][state] = min coins to paint first i elements (0-indexed up to i-1)
    // state 0: no red element available to the right of the processed prefix
    // state 1: there is a red element with value 1 available
    // state 2: there is a red element with value 2 available
    // state 3: there is a red element with value >=1 available (general, but we track exact values)
    // Actually we track: 0 = no active red, 1 = active red with value 1, 2 = active red with value 2
    const int INF = 1e9;
    vector<array<int, 3>> dp(n + 1, {INF, INF, INF});
    dp[0][0] = 0;

    for (int i = 0; i < n; ++i) {
        for (int state = 0; state < 3; ++state) {
            if (dp[i][state] == INF) continue;
            int cur = dp[i][state];

            // Option 1: pay coin to paint a[i] red
            // After painting, the new active red is a[i] (if a[i] > 0)
            int new_state = (a[i] > 0 ? a[i] : 0);
            dp[i + 1][new_state] = min(dp[i + 1][new_state], cur + 1);

            // Option 2: use active red from left (state > 0) to paint a[i]
            if (state > 0) {
                // We can paint a[i] for free, consuming 1 from active red
                int remaining = state - 1;
                // The new active red could be the remaining value, or a[i] if it's >0 and we want to keep it
                // But we can also choose to keep the remaining active red if it's >0, or switch to a[i] if better
                // Actually after painting a[i], the active red can be either the leftover from previous,
                // or the newly painted a[i] if it's >0. We take the maximum possible value to maximize future use.
                int best = max(remaining, a[i]);
                if (best > 2) best = 2; // values only up to 2
                dp[i + 1][best] = min(dp[i + 1][best], cur);
            }
        }
    }

    int ans = INF;
    for (int s = 0; s < 3; ++s) {
        ans = min(ans, dp[n][s]);
    }
    cout << ans << '\n';

    return 0;
}
