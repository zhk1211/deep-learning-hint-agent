// Hint1
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<int> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }

        // We will binary search the answer
        int lo = 0, hi = n, ans = 0;
        while (lo <= hi) {
            int mid = (lo + hi) / 2;
            // Check if we can achieve rating >= mid
            // We simulate the process with target rating = mid
            // We want to see if there exists an interval to skip such that final rating >= mid
            // Equivalent: we start with x = 0, we want x >= mid after processing all, skipping one interval.
            // We can think of it as: we want to maximize final rating.
            // Let's compute prefix "rating" without skips, but we need to handle skips.
            // Better: we can compute the rating change if we skip an interval.
            // Let's define f(i) = rating after processing first i elements without skips.
            // But skipping changes the state.
            // Alternative: we can think of the process as: we start with x=0.
            // For each element, if we don't skip, x changes by +1 if a_i > x, 0 if a_i == x, -1 if a_i < x.
            // We want to skip a contiguous subarray to maximize final x.
            // This is similar to finding a subarray to remove to maximize final value.
            // We can binary search the answer: check if we can achieve >= K.
            // To check if we can achieve >= K, we can think of the process as: we want to end with x >= K.
            // We can define a DP or greedy: we process from left to right, maintaining the maximum possible x after processing prefix, given that we have already used the skip or not.
            // But we need to skip exactly one interval (could be empty? No, must skip at least one contest, so interval length >= 1).
            // So we can have states: 0 = haven't started skipping, 1 = currently skipping, 2 = finished skipping.
            // For a fixed target K, we want to know if we can end with x >= K.
            // However, the rating update depends on current x, which is not linear.
            // But we can simulate with DP for a given K? That would be O(n) per check, total O(n log n) which is acceptable (n sum 3e5, log n ~ 19, total ~ 6e6 operations).
            // Let's design check(K):
            // We want to know if there exists a skip interval such that final rating >= K.
            // We can compute the maximum possible final rating with one skip.
            // Let dp[i][state] = maximum possible rating after processing first i elements, with state 0/1/2.
            // state 0: haven't skipped yet.
            // state 1: currently skipping (we have started skipping, not yet finished).
            // state 2: finished skipping.
            // Transitions:
            // For each i from 1 to n:
            // For state 0: we must not skip this element. So new_x = update(dp[i-1][0], a[i]).
            // For state 1: we can either start skipping here (if we were in state 0 before), or continue skipping (from state 1). If we start skipping, we skip this element, so x remains same as dp[i-1][0]. If we continue skipping, x remains dp[i-1][1].
            // For state 2: we can either finish skipping (come from state 1, skipping this element? Actually if we finish skipping, we must have skipped some interval, and now we are processing this element normally. So we can come from state 1 by not skipping this element (i.e., we end the skip before this element), or come from state 2 by processing normally.
            // Wait: careful with the definition of state 1: "currently skipping" means we are inside the skip interval. The interval is [l, r]. When we are at i, if i < l, state 0; if l <= i <= r, state 1; if i > r, state 2.
            // So at step i, if we are in state 1, we skip a_i. If we are in state 0 or 2, we process a_i normally.
            // Transitions:
            // dp[i][0] = update(dp[i-1][0], a[i])  (only if we stay in state 0)
            // dp[i][1] = max(dp[i-1][0], dp[i-1][1])  // we can start skipping now (from state 0) or continue skipping (from state 1). In both cases, we skip a_i, so x unchanged.
            // dp[i][2] = max(update(dp[i-1][1], a[i]), update(dp[i-1][2], a[i]))  // we can end skip before this element (come from state 1 and process a_i) or continue after skip (from state 2 and process a_i).
            // But wait: if we come from state 1 and process a_i, that means the skip interval ended at i-1. So we process a_i normally. That's correct.
            // Initialization: dp[0][0] = 0, dp[0][1] = -inf, dp[0][2] = -inf.
            // At the end, we need to have skipped at least one contest, so we cannot end in state 0. Valid final states are state 1 (if we skip till the end) or state 2. So max(dp[n][1], dp[n][2]) is the maximum final rating.
            // Then we check if max >= K.
            // However, the update function depends on current x. It's simple: if a > x, x+1; if a == x, x; if a < x, x-1.
            // This DP is O(n) per check. With binary search, O(n log n). Sum n = 3e5, log n ~ 19, total ~ 5.7e6 operations, very fast.
            // But we need to be careful: the DP state values can be negative? Initially 0, and rating can go down to -n? Actually minimum possible rating is -n (if all a_i = 1 and we process all, x goes 0->1->0->-1...). So we can use -1e9 as -inf.
            // Let's implement check(K) returning true if max final rating >= K.

            auto check = [&](int K) -> bool {
                const int INF = 1e9;
                vector<int> dp0(n+1, -INF), dp1(n+1, -INF), dp2(n+1, -INF);
                dp0[0] = 0;
                for (int i = 1; i <= n; ++i) {
                    int val = a[i-1];
                    // state 0
                    if (dp0[i-1] != -INF) {
                        int x = dp0[i-1];
                        if (val > x) x++;
                        else if (val < x) x--;
                        dp0[i] = max(dp0[i], x);
                    }
                    // state 1
                    dp1[i] = max(dp0[i-1], dp1[i-1]); // skip a_i
                    // state 2
                    if (dp1[i-1] != -INF) {
                        int x = dp1[i-1];
                        if (val > x) x++;
                        else if (val < x) x--;
                        dp2[i] = max(dp2[i], x);
                    }
                    if (dp2[i-1] != -INF) {
                        int x = dp2[i-1];
                        if (val > x) x++;
                        else if (val < x) x--;
                        dp2[i] = max(dp2[i], x);
                    }
                }
                int best = max(dp1[n], dp2[n]);
                return best >= K;
            };

            if (check(mid)) {
                ans = mid;
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
        cout << ans << '\n';
    }
    return 0;
}
