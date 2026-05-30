// Hint3
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];

    // dp0[i] = rating after processing first i elements without any skip
    // dp1[i] = max rating after processing first i elements with a skip interval that has started but not necessarily ended
    // dp2[i] = max rating after processing first i elements with a skip interval that has ended
    // We'll use 1-indexed for convenience
    vector<int> dp0(n + 1, 0), dp1(n + 1, 0), dp2(n + 1, 0);
    
    // Initial rating is 0
    dp0[0] = 0;
    dp1[0] = -1e9; // impossible to have started skip before any element
    dp2[0] = -1e9; // impossible to have ended skip before any element

    for (int i = 1; i <= n; ++i) {
        int val = a[i - 1];
        
        // dp0: no skip, normal update
        if (val > dp0[i - 1]) dp0[i] = dp0[i - 1] + 1;
        else if (val == dp0[i - 1]) dp0[i] = dp0[i - 1];
        else dp0[i] = dp0[i - 1] - 1;
        
        // dp1: skip interval has started (we are currently skipping or have skipped some)
        // Option 1: start skipping at this element (transition from dp0[i-1])
        // Option 2: continue skipping (transition from dp1[i-1])
        dp1[i] = max(dp0[i - 1], dp1[i - 1]);
        
        // dp2: skip interval has ended (we are now processing normally after a skip)
        // Option 1: end skip just before this element (transition from dp1[i-1])
        // Option 2: continue after skip (transition from dp2[i-1])
        int from_skip_end = dp1[i - 1];
        int from_continue = dp2[i - 1];
        int best_prev = max(from_skip_end, from_continue);
        if (best_prev < -1e8) {
            dp2[i] = -1e9;
        } else {
            if (val > best_prev) dp2[i] = best_prev + 1;
            else if (val == best_prev) dp2[i] = best_prev;
            else dp2[i] = best_prev - 1;
        }
    }
    
    // Answer is max of dp1[n] (skip ended at or before n) and dp2[n] (skip ended and then processed)
    // dp0[n] is not allowed because we must skip at least one contest.
    int ans = max(dp1[n], dp2[n]);
    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}
