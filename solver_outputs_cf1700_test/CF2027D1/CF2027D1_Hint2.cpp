// Hint2
#include <bits/stdc++.h>
using namespace std;

const long long INF = 1e18;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n, m;
        cin >> n >> m;
        vector<long long> a(n);
        for (int i = 0; i < n; i++) cin >> a[i];
        vector<long long> b(m);
        for (int i = 0; i < m; i++) cin >> b[i];
        
        // dp[i] = min cost to remove prefix of length i
        // We'll use a 1D DP over positions in a
        vector<long long> dp(n + 1, INF);
        dp[0] = 0;
        
        // For each k from 1 to m, we can use b_{k-1} as the limit
        // We process k from m down to 1? Actually we can process k from 1 to m
        // But we need to consider that we can increase k at any time without cost.
        // The cost of using b_{k-1} is m - k.
        // Since we can increase k freely, we can use any b_{k-1} at any point,
        // paying cost m - k for that operation.
        // So we can just consider all possible (k, segment) choices.
        
        // We can precompute prefix sums of a
        vector<long long> pref(n + 1, 0);
        for (int i = 0; i < n; i++) pref[i+1] = pref[i] + a[i];
        
        // For each position i, we want to transition to j > i where sum(i..j-1) <= b_{k-1}
        // cost = dp[i] + (m - k)
        // Since n*m <= 3e5, we can iterate over k and use two pointers?
        // Actually n*m <= 3e5 means total n*m over test cases is small.
        // We can do DP with states (i, k) but that would be O(n*m) which is acceptable.
        // But we need to be careful: n*m <= 3e5, so O(n*m) is fine.
        
        // Let's define dp2[i][k] = min cost to remove prefix of length i, with current k value = k?
        // But k can increase, so we can just think: we are at some position, we can choose any k (1..m)
        // and remove a segment with sum <= b_{k-1}, paying m-k.
        // Since we can increase k freely, the order of k choices doesn't matter except we can't decrease k.
        // But we can always increase k before an operation, so effectively we can use any b_{k-1} at any time,
        // paying cost m - k. So we can just consider all possible (k, segment) transitions.
        
        // However, we can also do multiple operations with the same k? Yes, if we don't increase k.
        // But increasing k is free, so we can always use the smallest possible cost for a given segment length?
        // Actually, for a fixed segment sum S, we need b_{k-1} >= S. The cost is m - k.
        // Since b is strictly decreasing, larger k means smaller b_{k-1}, but cost m - k is smaller.
        // So for a given S, we want the largest k (smallest cost) such that b_{k-1} >= S.
        // That is, we can precompute for each possible sum S, the minimum cost to remove a segment of sum <= S.
        // But sums can be up to 3e14, too large.
        
        // Alternative: Since n*m <= 3e5, we can do DP over positions and k.
        // Let dp[i][k] = min cost to remove first i elements, ending with current k = k (i.e., we have increased k to this value and haven't increased further yet).
        // But we can increase k at any time, so dp[i][k] can transition to dp[i][k'] for k' > k with no cost.
        // So we can maintain for each i, the minimum over k of dp[i][k].
        // Actually, we can define dp[i] = min cost to remove first i elements. Then we consider all possible next segments.
        // For a fixed k, we can remove a segment starting at i+1 with sum <= b_{k-1}. The cost added is m - k.
        // So dp[j] = min(dp[j], dp[i] + m - k) for all i < j, sum(i+1..j) <= b_{k-1}.
        // This is O(n^2 * m) naive, but we can optimize.
        
        // Since n*m <= 3e5, we can iterate over k and for each k, use two pointers to update dp.
        // For a fixed k, we can find for each i, the furthest j such that sum(i+1..j) <= b_{k-1}.
        // Then we can update dp[j] = min(dp[j], dp[i] + m - k).
        // But doing this for all i,j pairs could be O(n^2) per k, too much.
        // However, we can use the fact that dp is non-decreasing? Not necessarily.
        // We can use a segment tree or monotonic queue? But n*m is small, maybe we can do something else.
        
        // Let's think differently: Since n*m <= 3e5, we can consider DP with states (i, k) meaning we are at position i (0..n) and current k value is k (1..m).
        // dp[i][k] = min cost to remove first i elements, and we are currently at k (meaning we have increased to k and can use b_{k-1} for next operation).
        // From state (i, k), we can:
        // 1. Increase k to k+1 (if k < m) with cost 0: dp[i][k+1] = min(dp[i][k+1], dp[i][k]).
        // 2. Remove a segment starting at i+1 with sum <= b_{k-1}: for j > i with sum(i+1..j) <= b_{k-1}, dp[j][k] = min(dp[j][k], dp[i][k] + (m - k)).
        // But wait, the cost m - k is incurred when we perform the operation. If we do multiple operations with the same k, we pay m - k each time.
        // So the transition cost is m - k per segment removed.
        
        // The number of states is (n+1)*m <= 3e5 + m, which is acceptable.
        // Transitions: increasing k is O(1) per state. Removing segments: for a fixed k and i, we need to find all j such that sum(i+1..j) <= b_{k-1}.
        // This could be many j's. But we can optimize by noting that we only need to consider the furthest j for each i? Not exactly, because dp[i][k] might be smaller for larger i, but we can use a running minimum.
        // Actually, for a fixed k, as i increases, the condition sum(i+1..j) <= b_{k-1} means j is non-decreasing. We can use two pointers.
        // Let's process k from 1 to m. For each k, we want to compute the best way to use b_{k-1} for segments.
        // We can maintain an array best[j] = min cost to reach j using some operations ending with k (or less).
        // But we also have the ability to increase k, which we can handle by propagating dp values from smaller k.
        
        // Let's define dp[i] = min cost to remove first i elements (using any sequence of operations). We'll compute this iteratively.
        // For each k from 1 to m, we can consider using b_{k-1} for some segments. But we can use b_{k-1} multiple times.
        // Since we can increase k freely, we can think of processing k in increasing order, and for each k, we allow operations with cost m - k using limit b_{k-1}.
        // We can do a DP where we update dp[j] from dp[i] using limit b_{k-1} with cost m - k.
        // To do this efficiently for all k, we can use the fact that n*m is small, so we can just do a 2D DP.
        
        // Let's implement the 2D DP with states (i, k). We'll use a vector<vector<long long>> dp(n+1, vector<long long>(m+1, INF)).
        // dp[0][1] = 0.
        // For i from 0 to n:
        //   For k from 1 to m:
        //     if dp[i][k] == INF continue;
        //     // Increase k
        //     if (k < m) dp[i][k+1] = min(dp[i][k+1], dp[i][k]);
        //     // Remove segment
        //     long long sum = 0;
        //     for (int j = i+1; j <= n; j++) {
        //         sum += a[j-1];
        //         if (sum > b[k-1]) break;
        //         dp[j][k] = min(dp[j][k], dp[i][k] + (m - k));
        //     }
        // Then answer is min_{k} dp[n][k].
        // Complexity: For each i, k, we iterate j until sum exceeds b[k-1]. In worst case, sum could be large, but a_i >= 1, so the inner loop runs at most b[k-1] times? No, a_i can be up to 1e9, but n is up to 3e5. However, n*m <= 3e5, so total states (i,k) is O(n*m). The inner loop over j could be O(n) per state, leading to O(n^2 * m) which is too much.
        // But we can optimize: For a fixed k, we can precompute the furthest j for each i using two pointers because b[k-1] is fixed and a_i are positive.
        // Let R[i] = max j such that sum(i+1..j) <= b[k-1]. Then we can update dp[j][k] for all j in (i, R[i]].
        // This is still O(n^2) per k if we do it naively. But we can use the fact that dp[i][k] is the minimum cost to reach i with current k. We can maintain a running minimum.
        // Actually, we can process i in increasing order, and for each i, we update a range [i+1, R[i]] with value dp[i][k] + (m-k). We need range min updates and point queries? Or we can do DP forward: when we are at i, we already have the minimum cost to reach i with any k. But we need to separate by k because cost depends on k.
        // However, note that for a fixed k, the cost added is constant (m-k). So if we define f[i] = min cost to reach i using any operations, we might be able to incorporate k.
        // Let's think differently: Since we can increase k at any time for free, the only thing that matters is which b we use for each segment. The cost of using b_{k-1} is m-k. Since b is decreasing, using a smaller b (larger k) costs less. So we want to use the smallest possible b (largest k) that can cover the segment sum.
        // This suggests we can precompute for each possible segment sum the minimum cost. But sums are large.
        
        // Another angle: n*m <= 3e5 means either n or m is small. We can choose the dimension to iterate over.
        // If m is small, we can do DP over k. If n is small, we can do DP over positions.
        // Let's design an algorithm that is O(n * m) or O(n * m * log n) using the fact that total n*m is small.
        // We can do DP with states (i, k) but we need to avoid the O(n) inner loop.
        // Notice that for a fixed k, the transitions are: from i we can go to any j in (i, R[i]] with cost m-k.
        // This is equivalent to: dp[j][k] = min(dp[j][k], min_{i < j, sum(i+1..j) <= b[k-1]} dp[i][k] + (m-k)).
        // Since m-k is constant for fixed k, we can define a new array: best[j] = min_{i < j, sum(i+1..j) <= b[k-1]} dp[i][k].
        // Then dp[j][k] = min(dp[j][k], best[j] + (m-k)).
        // We can compute best[j] efficiently using a monotonic queue or two pointers because the condition sum(i+1..j) <= b[k-1] is equivalent to pref[j] - pref[i] <= b[k-1] => pref[i] >= pref[j] - b[k-1].
        // As j increases, the required pref[i] increases. So we can maintain a pointer i and a data structure for minimum dp[i][k] among valid i.
        // Specifically, for a fixed k, we can iterate j from 1 to n. We maintain a pointer i (starting at 0) such that pref[j] - pref[i] <= b[k-1] is false for i too small. Actually, we want i such that pref[j] - pref[i] <= b[k-1], i.e., pref[i] >= pref[j] - b[k-1]. Since pref is increasing, as j increases, the threshold pref[j] - b[k-1] increases. So we can move i forward as long as pref[i] < pref[j] - b[k-1]. The valid i's are those from some start index to j-1. We need the minimum dp[i][k] among these valid i.
        // We can maintain a deque or a multiset of dp[i][k] for valid i. When we move i forward, we remove dp[i][k] from the structure. When we advance j, we add dp[j-1][k] to the structure (since i can be j-1). Then best[j] = minimum in structure.
        // This gives O(n) per k, so total O(n*m) which is acceptable since n*m <= 3e5.
        // Also we need to handle the increase of k: dp[i][k+1] = min(dp[i][k+1], dp[i][k]). This can be done by just taking min over k for each i at the end, or propagating during the DP.
        // Actually, we can process k from 1 to m. For each k, we first propagate dp[i][k] from dp[i][k-1] (since we can increase k from k-1 to k for free). Then we run the two-pointer DP for this k to compute new dp[j][k] using segments with b_{k-1}.
        // But wait: we can also increase k after some operations. Our DP state dp[i][k] means we are at position i and current k is k. We can increase k at any time, so dp[i][k] should be at most dp[i][k-1]. So when we start processing k, we should set dp[i][k] = min(dp[i][k], dp[i][k-1]) for all i.
        // Then we run the segment removal transitions for this k, which may update dp[j][k] for j > i.
        // Finally, the answer is min_{k} dp[n][k].
        
        // Let's implement this.
        // dp[i][k] for i=0..n, k=1..m. Initialize with INF.
        // dp[0][1] = 0.
        // For k = 1 to m:
        //   if k > 1: for i=0..n: dp[i][k] = min(dp[i][k], dp[i][k-1])
        //   Now run two-pointer for this k:
        //   We need to compute dp[j][k] = min(dp[j][k], min_{valid i} dp[i][k] + (m - k))
        //   valid i: i < j and pref[j] - pref[i] <= b[k-1]
        //   We'll maintain a data structure for minimum dp[i][k] among valid i.
        //   Since we only need to query minimum and remove elements when they become invalid, we can use a multiset or a monotonic queue with a pointer.
        //   Actually, we can use a deque that maintains increasing values of dp[i][k] (like sliding window minimum). But the window of valid i is not contiguous in index? It is contiguous in index because the condition pref[i] >= pref[j] - b[k-1] defines a suffix of i's up to j-1. Since pref is increasing, the valid i's are those with index >= some L, where L is the smallest index such that pref[L] >= pref[j] - b[k-1]. As j increases, L is non-decreasing. So the valid i's form a sliding window [L, j-1]. We can maintain a monotonic queue for minimum in this window.
        //   However, we also need to add new i's as j increases: when we move to j, we add i = j-1 to the window. We also need to remove i's that are < L.
        //   So we can do:
        //   deque<int> dq; // stores indices i with increasing dp[i][k]
        //   int L = 0; // left bound of valid i's
        //   for j = 1 to n:
        //       // add i = j-1 to window
        //       while (!dq.empty() && dp[dq.back()][k] >= dp[j-1][k]) dq.pop_back();
        //       dq.push_back(j-1);
        //       // update L: smallest i such that pref[i] >= pref[j] - b[k-1]
        //       while (L < j && pref[L] < pref[j] - b[k-1]) L++;
        //       // remove indices < L from dq
        //       while (!dq.empty() && dq.front() < L) dq.pop_front();
        //       // now min valid dp[i][k] is dp[dq.front()][k] if dq not empty
        //       if (!dq.empty()) {
        //           dp[j][k] = min(dp[j][k], dp[dq.front()][k] + (m - k));
        //       }
        //   }
        // This works because we only need to consider i < j, and we add i = j-1 before computing for j. Also we need to ensure that we don't use i = j (which is not allowed). The window is [L, j-1]. L is computed based on pref[j] - b[k-1]. Since pref is 0-indexed, pref[0]=0.
        // One detail: dp[i][k] might be INF. We should handle that. The monotonic queue will still work if we just push INF, but we can skip pushing if INF to save time, but it's fine.
        // After processing all k, answer is min_{k} dp[n][k]. If INF, output -1.
        
        // Let's test with sample.
        // Complexity: O(n*m) per test case, total O(sum n*m) <= 3e5, very fast.
        
        // We need to allocate dp as vector<vector<long long>> (n+1, vector<long long>(m+1, INF)).
        // But n*m <= 3e5, so total elements across all k is (n+1)*m <= 3e5 + m, which is fine.
        // We can also flatten to 1D array of size (n+1)*m, but vector of vectors is okay.
        
        vector<vector<long long>> dp(n + 1, vector<long long>(m + 1, INF));
        dp[0][1] = 0;
        
        for (int k = 1; k <= m; k++) {
            if (k > 1) {
                for (int i = 0; i <= n; i++) {
                    dp[i][k] = min(dp[i][k], dp[i][k-1]);
                }
            }
            // two-pointer for this k
            deque<int> dq;
            int L = 0;
            long long limit = b[k-1];
            for (int j = 1; j <= n; j++) {
                // add i = j-1
                long long val = dp[j-1][k];
                if (val < INF) {
                    while (!dq.empty() && dp[dq.back()][k] >= val) dq.pop_back();
                    dq.push_back(j-1);
                }
                // update L
                long long need = pref[j] - limit;
                while (L < j && pref[L] < need) L++;
                // remove out-of-window
                while (!dq.empty() && dq.front() < L) dq.pop_front();
                // update dp[j][k]
                if (!dq.empty()) {
                    dp[j][k] = min(dp[j][k], dp[dq.front()][k] + (m - k));
                }
            }
        }
        
        long long ans = INF;
        for (int k = 1; k <= m; k++) {
            ans = min(ans, dp[n][k]);
        }
        if (ans == INF) cout << -1 << "\n";
        else cout << ans << "\n";
    }
    return 0;
}
