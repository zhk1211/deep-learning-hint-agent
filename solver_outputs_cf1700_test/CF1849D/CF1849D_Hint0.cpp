// Hint0
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

    // dp[i][state] = min coins to cover prefix up to i (inclusive)
    // state 0: i is red and has 0 remaining value (cannot extend right)
    // state 1: i is red and has 1 remaining value (can extend right once)
    // state 2: i is red and has 2 remaining value (can extend right twice)
    // We process left to right, considering how to make i red.
    const int INF = 1e9;
    vector<array<int, 3>> dp(n, {INF, INF, INF});

    // Base: first element
    // To make first red, we can either pay coin (then it has a[0] value)
    // or if a[0] == 0, we must pay coin and it has 0.
    dp[0][0] = 1; // pay coin, value becomes a[0], but we store remaining value
    if (a[0] >= 1) dp[0][1] = 1;
    if (a[0] >= 2) dp[0][2] = 1;

    // Also, if a[0] == 0, only state 0 possible with 1 coin.
    // Actually, if we pay coin, the element gets its original value a[i].
    // So dp[i][v] = min coins to make prefix 0..i red, and element i has v remaining value (v <= a[i]).
    // But we can also get value from left neighbor.

    for (int i = 1; i < n; ++i) {
        // Option 1: pay coin for i
        for (int v = 0; v <= a[i]; ++v) {
            // i becomes red with value v, we need min over previous states
            int best_prev = *min_element(dp[i-1].begin(), dp[i-1].end());
            if (best_prev != INF) {
                dp[i][v] = min(dp[i][v], best_prev + 1);
            }
        }

        // Option 2: use left neighbor's remaining value
        // left neighbor must have at least 1 remaining, and we consume 1 to paint i.
        // After painting, i gets its original value a[i] (but we can also think it gets a[i] remaining).
        // Actually, when we paint i using left, left's remaining decreases by 1.
        // So if left had state r (1 or 2), after painting i, left becomes r-1.
        // i becomes red with its full value a[i] (since it's newly painted).
        // But we must track remaining value of i for future extensions.
        // So transition: from dp[i-1][r] with r>=1, we can go to dp[i][a[i]] (but a[i] could be 0,1,2).
        // However, we also need to consider that left's state after this operation is r-1.
        // But dp[i-1][r] already accounts for left being red with r remaining.
        // After using 1, left's remaining becomes r-1, but we don't need to track left anymore because we move to i.
        // So dp[i][a[i]] = min(dp[i][a[i]], dp[i-1][r]) for r>=1.
        // Wait, is that correct? The cost is already paid in dp[i-1][r]. We don't pay extra coin.
        // So yes.
        if (a[i] <= 2) {
            for (int r = 1; r <= 2; ++r) {
                if (dp[i-1][r] != INF) {
                    dp[i][a[i]] = min(dp[i][a[i]], dp[i-1][r]);
                }
            }
        }

        // Option 3: use left neighbor's remaining value, but left had 2 and we use 1, left becomes 1.
        // That's covered above because we transition to dp[i][a[i]] from dp[i-1][2].
        // But what if a[i] is 0? Then i gets 0 remaining, and left's remaining decreases by 1.
        // That's covered: dp[i][0] can be reached from dp[i-1][r] with r>=1.
        // So above loop handles all a[i].

        // However, there is another possibility: we could use left neighbor's value to paint i, but i might have been already painted? No, we only paint blue.
        // Also, we might use multiple steps? No, one operation paints one blue.
    }

    int ans = *min_element(dp[n-1].begin(), dp[n-1].end());
    cout << ans << '\n';

    return 0;
}
