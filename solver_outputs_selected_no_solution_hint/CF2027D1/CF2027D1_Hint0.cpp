#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n, m;
        cin >> n >> m;
        vector<long long> a(n);
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }
        vector<long long> b(m);
        for (int i = 0; i < m; i++) {
            cin >> b[i];
        }

        // dp[i] = min cost to remove prefix of length i
        // dp[0] = 0
        // We'll use a deque for sliding window minimum
        const long long INF = 1e18;
        vector<long long> dp(n + 1, INF);
        dp[0] = 0;

        // For each k from 1 to m, we can use b_{k-1} as the limit
        // We process k from 1 to m, but note that we can skip k's (type 1 operations)
        // The cost of using b_{k-1} is (m - k)
        // We want to compute dp[i] = min over j < i, sum(j+1..i) <= b_{k-1} of dp[j] + (m - k)
        // But we can also use multiple k's in increasing order.
        // We can process k from 1 to m, and for each k, we update dp using a sliding window.
        // However, we must ensure we only use each k at most once? Actually we can use the same k multiple times,
        // but since b is decreasing, using a larger k (smaller b) later is allowed only if we increased k.
        // The sequence of operations: we start at k=1, we can do type 2 (remove prefix with sum <= b_k) any number of times,
        // then type 1 to increase k, then type 2 with new k, etc.
        // So we can think of it as: we choose a sequence of k's (non-decreasing, strictly increasing when we do type 1)
        // and for each k we remove some prefixes.
        // This is equivalent to: we partition the array into segments, each segment sum <= b_{k_i} for some k_i,
        // and the k_i sequence is non-decreasing, and the cost is sum (m - k_i) for each segment.
        // Since we can always increase k without cost, we can assume we use the smallest possible k for each segment
        // that can cover its sum. But we must respect the order: k cannot decrease.
        // So we can process k from 1 to m, and for each k, we can extend dp to cover as many elements as possible
        // with sum <= b_{k-1}. But we can also skip k's.
        // A better way: For each position i, we want the minimum cost to reach i.
        // We can use a segment [j+1, i] with sum <= b_{k-1} for some k, and cost = dp[j] + (m - k).
        // Since b is decreasing, for a fixed i, as k increases, b_{k-1} decreases, so the allowed j range shrinks.
        // We can precompute for each i the smallest k such that there exists j with sum(j+1..i) <= b_{k-1}.
        // But we need to consider all possible j.
        // Alternative: DP with monotonic queue for each k.
        // Since n*m <= 3e5, we can afford O(n*m) if we are careful? No, n*m sum is 3e5, so O(n*m) total is fine!
        // Wait: sum of n*m over test cases <= 3e5. So we can do an O(n*m) algorithm per test case.
        // But n and m individually can be up to 3e5, but product is small. So we can iterate over k and for each k,
        // we can update dp using a two-pointer or sliding window. That would be O(n*m) if we do a full pass over n for each k.
        // However, if n=3e5 and m=1, product is 3e5, O(n*m) is 3e5, fine.
        // If n=1 and m=3e5, product is 3e5, O(n*m) is 3e5, fine.
        // So we can do: for each k from 1 to m, we consider using b_{k-1} as the limit.
        // We maintain a sliding window of valid j for current i, and we want min dp[j] in that window.
        // Since we process k in increasing order, we can update dp in place.
        // But careful: using a later k (larger index) means we have already increased k, so we can use it.
        // The order of operations: we can use k=1, then increase to k=2, then use k=2, etc.
        // So when we process k=2, we can use dp values that were already computed using k=1 or k=2.
        // So we can just do: for each k from 1 to m:
        //   cost = m - k;
        //   We want to update dp[i] = min(dp[i], min_{j in window} dp[j] + cost) for all i where window is valid.
        //   We can do this with a deque for each k, but that would be O(n) per k, total O(n*m).
        //   Since n*m <= 3e5, O(n*m) is acceptable.
        // Let's implement:
        // For each k from 1 to m:
        //   long long limit = b[k-1];
        //   int cost = m - k;
        //   We'll use two pointers: left = 0, right = 0, current_sum = 0.
        //   We'll maintain a deque of indices j with increasing dp[j].
        //   For i from 1 to n:
        //       current_sum += a[i-1];
        //       while (current_sum > limit) {
        //           current_sum -= a[left];
        //           left++;
        //       }
        //       // now all j from left to i-1 are valid (since sum(j+1..i) <= limit)
        //       // we need to add j = i-1 to deque? Actually we add j to deque as we move i.
        //       // We'll maintain deque for j in [left-1? Wait, j can be from left-1? 
        //       // The prefix removed is from j+1 to i, so j can be from 0 to i-1.
        //       // The condition sum(j+1..i) <= limit means j >= some lower bound L_i.
        //       // As i increases, L_i is non-decreasing.
        //       // So we can maintain a sliding window of j.
        //       // We'll add j = i-1 to deque after processing i? Actually we need j up to i-1.
        //       // Let's do: before processing i, we add j = i-1 to deque? But we need to add j when it becomes available.
        //       // Better: iterate i from 1 to n, maintain left pointer such that sum(left+1..i) <= limit.
        //       // Then valid j are from left to i-1.
        //       // We'll maintain a deque of indices j in [left, i-1] with increasing dp[j].
        //       // When we move i to i+1, we add j = i to deque, and we might increase left.
        //       // So we can do:
        //       // deque<int> dq; // stores indices j
        //       // int left = 0;
        //       // long long cur_sum = 0;
        //       // for (int i = 1; i <= n; i++) {
        //       //     cur_sum += a[i-1];
        //       //     while (cur_sum > limit) {
        //       //         cur_sum -= a[left];
        //       //         left++;
        //       //     }
        //       //     // add j = i-1 to deque
        //       //     int new_j = i-1;
        //       //     while (!dq.empty() && dp[dq.back()] >= dp[new_j]) dq.pop_back();
        //       //     dq.push_back(new_j);
        //       //     // remove indices < left from deque
        //       //     while (!dq.empty() && dq.front() < left) dq.pop_front();
        //       //     // now min dp in window is dp[dq.front()]
        //       //     if (!dq.empty()) {
        //       //         dp[i] = min(dp[i], dp[dq.front()] + cost);
        //       //     }
        //       // }
        // But careful: The dp values we use for j might have been computed using a larger k (smaller cost) or same k.
        // Since we process k in increasing order, when we are at k, dp[j] already contains the best cost using k' <= k.
        // That's correct because we can use k' <= k before increasing to k.
        // However, we must ensure we don't use a j that was reached using a k' > k, but we process k increasing, so dp[j] only has k' <= k.
        // So this works.
        // But we need to initialize dp[0] = 0, others INF.
        // Then for each k, we run the sliding window.
        // Total time O(n*m) which is fine.

        for (int k = 1; k <= m; k++) {
            long long limit = b[k-1];
            int cost = m - k;
            deque<int> dq;
            int left = 0;
            long long cur_sum = 0;
            // We need to add j=0 initially? For i=1, valid j are from left to 0.
            // We'll add j=0 before loop? Actually we add j = i-1 at each step.
            // Let's do: for i=1, we add j=0 first.
            // We can start with dq containing j=0? But we need to ensure dp[0] is not INF.
            // dp[0] = 0, so it's fine.
            // We'll add j=0 before processing i=1.
            dq.push_back(0);
            for (int i = 1; i <= n; i++) {
                cur_sum += a[i-1];
                while (cur_sum > limit) {
                    cur_sum -= a[left];
                    left++;
                }
                // remove indices < left
                while (!dq.empty() && dq.front() < left) dq.pop_front();
                // now update dp[i]
                if (!dq.empty()) {
                    dp[i] = min(dp[i], dp[dq.front()] + cost);
                }
                // add j = i to deque for next iterations
                int new_j = i;
                if (dp[new_j] < INF) {
                    while (!dq.empty() && dp[dq.back()] >= dp[new_j]) dq.pop_back();
                    dq.push_back(new_j);
                }
            }
        }

        if (dp[n] >= INF) {
            cout << -1 << '\n';
        } else {
            cout << dp[n] << '\n';
        }
    }
    return 0;
}
