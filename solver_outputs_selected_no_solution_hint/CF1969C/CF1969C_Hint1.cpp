#include <bits/stdc++.h>
using namespace std;

const int INF = 1e18;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n, k;
        cin >> n >> k;
        vector<long long> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }

        // dp[i][j][0/1] - minimum sum for prefix up to i, using j operations,
        // and the last element is not changed (0) or changed (1) to some value.
        // We'll use a simpler DP: dp[i][j] = min sum for prefix i with j operations,
        // but we need to know the last value to propagate.
        // Better: dp[i][j][v] where v is the value of a[i] after operations.
        // Since a[i] can only become some a[t] from the array, we can compress.
        // But k <= 10, n <= 3e5, so O(n*k^2) is fine.
        
        // Alternative approach: we can change a contiguous segment to a single value
        // equal to one of the endpoints of the segment. The cost is sum of original values
        // minus (len)*min_value. We want to choose up to k disjoint segments to "flatten".
        // But segments can overlap? Operation: replace element with neighbor's value.
        // This means we can propagate a value across a segment. Effectively, we can choose
        // up to k elements and "spread" their values to adjacent elements, reducing sum.
        // It's equivalent to: we can select up to k positions, and for each, we can extend
        // its value to left and right, overwriting neighbors. Overwrites can overlap.
        // The final array will consist of blocks of equal values, where each block's value
        // comes from one of the original elements in that block. The number of blocks
        // is at most k+1? Actually, each operation can merge two adjacent elements,
        // so after k operations, the number of "changes" in the array is at most k.
        // More precisely, we can think of it as: we can choose up to k indices to "keep",
        // and the rest can be changed to match the nearest kept index? Not exactly.
        
        // Let's use DP: dp[i][j] = minimum sum for prefix of length i, using j operations,
        // and we consider the i-th element as the end of some segment.
        // But we need to know the value of the last element to compute future changes.
        // Since k is small, we can do DP over segments.
        
        // Another perspective: The operation allows us to change a[i] to a[i-1] or a[i+1].
        // This is equivalent to: we can delete an element and shift the rest? No.
        // It's like we can "propagate" a value. The minimum sum is achieved by making
        // some elements equal to the minimum in their neighborhood.
        
        // Let's use DP with states: dp[i][j] = min sum for first i elements with j operations,
        // where the i-th element is either unchanged or changed to some value.
        // But we need to know the value of a[i] after operations to know if we can change
        // a[i+1] to it. Since a[i] can only become one of the original a values, and k<=10,
        // we can try all possibilities for the last value.
        
        // Simpler: Since k <= 10, we can consider that we will only modify at most 10 elements?
        // Actually, one operation can change one element. So we can change at most k elements.
        // But changing one element might enable further changes? No, each operation changes
        // exactly one element to a neighbor's value. So after k operations, at most k elements
        // are changed from their original values. However, the changed elements can be
        // changed multiple times? The problem says "at most k times", so we can perform
        // up to k operations. Each operation changes one element. So total changed elements <= k.
        // But wait, we can change an element, then later change its neighbor to the new value,
        // effectively propagating a value across multiple elements with multiple operations.
        // So the number of changed elements can be up to k, but they might all become the same
        // value from a single original element.
        
        // So we can think: we choose a set of up to k elements to change. For each changed element,
        // we can set it to the value of any of its neighbors (which might also be changed).
        // The goal is to minimize sum.
        
        // This is equivalent to: we can select up to k elements and reduce their values to
        // the minimum of some connected component? Not exactly.
        
        // Let's think differently: The operation is like we can "spread" a small value.
        // The optimal strategy is to take the smallest elements and spread them to larger neighbors.
        // Since k is small (<=10), we can try all subsets of elements to "keep" and spread?
        // But n is up to 3e5, so we need O(n * poly(k)).
        
        // Consider the array as a sequence. We can perform operations to make some elements equal
        // to the minimum in a contiguous segment. Each operation can be seen as expanding a "low value"
        // by one step. So if we have a low value at position p, we can spend one operation to
        // change a[p-1] or a[p+1] to a[p]. Then we can continue from there.
        // So with k operations, we can take a set of "source" elements and expand them up to
        // a total of k steps. The expanded region will have the source value.
        // The sources can be multiple, and expansions can overlap.
        
        // This is similar to: we can choose up to k elements to "delete" and replace by neighbors.
        // Actually, it's like we can choose up to k elements to remove, and the array shrinks?
        // No, length remains same.
        
        // Let's formalize: We have an array a. We can apply operation: pick i, set a[i] = a[i-1] or a[i+1].
        // This means we can copy a value to an adjacent cell. So we can propagate a value.
        // The cost is the sum of the array. We want to minimize sum.
        // This is equivalent to: we can choose up to k positions to "overwrite" with the value of an adjacent cell.
        // The adjacent cell might have been overwritten earlier.
        
        // Since k is small, we can use DP that considers the "runs" of equal values.
        // Another idea: The final array will consist of the original values, except some elements
        // are changed to other original values. The total number of changed elements is at most k.
        // But wait, if we change a[i] to a[i-1], and then change a[i+1] to a[i] (which is now a[i-1]),
        // we effectively changed two elements with two operations. So yes, at most k elements are changed.
        // However, the changed elements might not be independent: if we change a segment to a single value,
        // we need one operation per changed element? Actually, to change a segment of length L to a value v
        // that is originally at one end of the segment, we need L-1 operations (if v is at one end) or
        // something like that. For example, to change [3,1,2] to [1,1,2], we changed one element (3 to 1)
        // with one operation. To change [2,2,1,3] to [2,1,1,1], we changed two elements (2 to 1, 3 to 1)
        // with two operations. So the number of operations equals the number of elements that are changed
        // from their original value. But is it always exactly that? If we change a[i] to a[i-1], and later
        // change a[i-1] to something else, does that count as changing a[i] again? The problem says
        // "replace it with any of its neighbor's value". So if we change a[i] to a[i-1], and then change
        // a[i-1] to a[i-2], a[i] remains the old a[i-1]. So a[i] was changed once. So each operation
        // changes exactly one element. So total changed elements <= k. And the final value of each changed
        // element is some original value from the array (because we only copy neighbor's value, which
        // ultimately traces back to an original value). So we are selecting at most k elements to change,
        // and for each, we assign it a value from some other element. But there is a constraint: the
        // assigned value must be reachable via a chain of adjacent copies. That means if we change a set S
        // of elements, the values we assign to them must come from elements not in S (or from S if we
        // copy from a changed element? But if we copy from a changed element, that changed element got its
        // value from somewhere else, so ultimately from an unchanged element). So we can think of it as:
        // we choose a set of at most k elements to "delete" (they will take the value of some neighbor),
        // and the remaining elements keep their original values. The deleted elements will take the value
        // of the nearest kept element? Not exactly, because we can chain: delete a[i], take a[i-1]; then
        // delete a[i-1], take a[i-2]; so a[i] ends up with a[i-2]. So a deleted element takes the value
        // of the nearest kept element in that direction? Actually, if we have a contiguous block of deleted
        // elements, they will all take the value of one of the endpoints of the block (the kept elements
        // at the boundaries). Because we can propagate from one endpoint across the block. So a block of
        // deleted elements of length L will cost L operations, and they will all become equal to the
        // minimum of the two boundary values? Wait, we can choose which boundary to propagate from.
        // To minimize sum, we would choose the smaller boundary value. So if we delete a contiguous segment
        // from l to r, we can make all of them equal to min(a[l-1], a[r+1]) (if both exist) or the only
        // existing boundary. But we can also have multiple separate deleted segments, total deleted elements <= k.
        // Also, we can delete elements at the ends? If we delete a[0], we can only copy from a[1]. So it
        // becomes a[1]. Similarly for the end.
        
        // So the problem reduces to: we can remove up to k elements from the array (they will be replaced
        // by the value of an adjacent kept element). The remaining elements stay as they are. The removed
        // elements form a set of disjoint intervals (since if we remove an element, we can't keep it).
        // For each removed interval, all elements in it will take the value of one of the interval's
        // boundary elements (the kept elements just outside the interval). We can choose which boundary
        // to use for each interval independently? Actually, within an interval, we can propagate from
        // one side, so all become the value of that side. So the cost of removing an interval [l, r] is
        // (r - l + 1) * min(a[l-1], a[r+1]) (with appropriate handling for ends). And we use (r - l + 1)
        // operations. We want to minimize the total sum, which is equivalent to minimizing the sum of
        // kept elements plus the sum of removed elements (which become boundary values). Alternatively,
        // we can think of it as: we start with total sum S. When we remove an element a[i] and replace it
        // with v, the sum changes by (v - a[i]). We want to minimize the final sum, so we want to choose
        // up to k elements to replace, with the constraint that replaced elements form intervals, and
        // each interval gets the value of one of its boundaries. But wait, can we have overlapping
        // intervals? If we remove two separate intervals, they are disjoint. Could we remove an interval,
        // and then later remove one of its boundaries? That would merge intervals. So the final set of
        // removed elements is a union of disjoint intervals. The boundaries are the kept elements.
        // So the problem is: choose a set of disjoint intervals to remove, total length <= k, such that
        // the sum of (boundary_value - original_value) over removed elements is minimized (i.e., we save
        // as much as possible). For each interval, we can choose the left or right boundary value.
        // But note: if we remove an interval, the boundary values are the original values of the kept
        // elements. They are not changed. So we can precompute the savings.
        
        // However, there is a catch: what if we remove an interval and choose the left boundary, but
        // that left boundary itself is later removed? Then the interval would merge with the left side.
        // So in the final configuration, the kept elements are fixed, and removed intervals are between
        // them. So we can just think of selecting up to k elements to remove, and they will form intervals
        // between kept elements. The value assigned to a removed interval is the minimum of its two
        // boundary kept elements. Because we can propagate from the smaller boundary. Is it always
        // possible to propagate from the smaller boundary? Yes, by starting at that boundary and
        // moving inward. So the cost for an interval [l, r] (inclusive) with boundaries L = l-1 and
        // R = r+1 (if they exist) is (r-l+1) * min(a[L], a[R]). If only one boundary exists (at the ends),
        // it's that boundary's value.
        
        // So we need to choose a set of disjoint intervals with total length <= k to minimize the sum.
        // This is a classic DP: dp[i][j] = minimum sum for prefix up to i, with j removals, and
        // we consider whether a[i] is kept or removed. But we need to know the boundary value when
        // we end an interval. Since the boundary value is the value of the kept element just before
        // the interval, we can track the last kept value.
        
        // Let's define dp[i][j] = minimum sum for first i elements, with j removals, and the i-th
        // element is kept. Then we can transition by considering the next kept element at position p > i,
        // and the interval (i+1 ... p-1) is removed. The cost added would be sum of original a in that
        // interval? No, we want to replace them with min(a[i], a[p]). So the contribution of that
        // interval to the total sum is (p - i - 1) * min(a[i], a[p]). And we add a[p] for the kept
        // element p. The number of removals used is (p - i - 1). So we can do:
        // dp[p][j + (p - i - 1)] = min(dp[p][...], dp[i][j] + (p - i - 1) * min(a[i], a[p]) + a[p]).
        // But we also need to consider the case where we don't keep any element after i? Actually,
        // we must keep the last element? Not necessarily, we could remove elements at the end.
        // If we remove a suffix, the boundary is the last kept element. So we can handle that by
        // adding a dummy element at the end with value 0? But we can't have value 0 because a_i >= 1.
        // Actually, if we remove a suffix from i+1 to n, the cost is (n - i) * a[i] (since only left
        // boundary exists). So we can just consider that as a transition to a virtual end.
        
        // Similarly, we can remove a prefix: if the first kept element is p, then elements 1..p-1
        // are removed and become a[p]. So we can initialize dp[p][p-1] = (p-1)*a[p] + a[p]? Wait,
        // the sum for prefix up to p with first kept at p: the removed prefix 1..p-1 cost (p-1)*a[p],
        // plus a[p] itself. So dp[p][p-1] = p * a[p]. But we can also keep a[1] and remove nothing.
        
        // So we can set dp[0][0] = 0, where 0 is a dummy kept element before the array with value?
        // We need its value for min. But for the first interval, there is no left boundary, so the
        // removed prefix just takes the value of the first kept element. So we can handle prefix
        // separately.
        
        // Let's define dp[i][j] = minimum sum for prefix of length i, where the i-th element is kept,
        // and we have used j removals. Here i from 1 to n. We also consider a dummy kept element at
        // position 0 with value a[1]? No, because if we remove a prefix, the value is a[p] for the
        // first kept p. So we can initialize dp[p][p-1] = p * a[p] for all p (removing prefix 1..p-1).
        // Also we can keep a[1] with 0 removals: dp[1][0] = a[1].
        
        // Then for each i, we can transition to p > i:
        // dp[p][j + (p - i - 1)] = min(..., dp[i][j] + (p - i - 1) * min(a[i], a[p]) + a[p]).
        // This is O(n^2 * k) which is too slow (n up to 3e5).
        
        // But k <= 10, so the number of removals between i and p is at most k. So p - i - 1 <= k.
        // Thus p is at most i + k + 1. So we only need to look ahead up to k+1 positions!
        // That makes it O(n * k^2) which is fine.
        
        // Also, we need to consider the case where we remove a suffix after the last kept element.
        // We can just take the minimum over all dp[i][j] and add the cost of removing the suffix
        // i+1..n: cost = (n - i) * a[i]. The total sum would be dp[i][j] + (n - i) * a[i].
        // And we must have j + (n - i) <= k? Actually, we can use at most k removals total.
        // So we need j + (n - i) <= k. But wait, the suffix removal uses (n - i) operations.
        // So we can only do it if we have enough remaining operations.
        
        // But is it always optimal to remove a suffix entirely? Yes, if we remove it, we just
        // propagate the last kept value. So we can consider that.
        
        // Let's formalize:
        // dp[i][j] = min sum for first i elements, with a[i] kept, using exactly j removals
        // among the first i elements (all removals are before i, not including i).
        // Actually, j counts removals in the prefix. The kept elements are some subset.
        // For i=1: dp[1][0] = a[1]. Also we can have removed a[1]? No, if we remove a[1], it's
        // part of a prefix removal ending at some p > 1. So we don't need dp for removed elements.
        
        // Initialize dp with INF.
        // For p from 1 to n:
        //   dp[p][p-1] = p * a[p]   // removed all before p, kept p.
        // But wait, what if p-1 > k? Then we can't remove that many. So only if p-1 <= k.
        
        // Then for i from 1 to n:
        //   for j from 0 to k:
        //     if dp[i][j] is INF, continue.
        //     // try to keep next element at p = i+1 ... i+k+1 (but p <= n)
        //     for p = i+1 to min(n, i + k + 1):
        //         int rem = p - i - 1; // number of removed elements between i and p
        //         if j + rem <= k:
        //             dp[p][j + rem] = min(dp[p][j + rem], dp[i][j] + rem * min(a[i], a[p]) + a[p]);
        //     // also consider removing the rest of the array after i (suffix)
        //     // we can do this at the end, but we can also do it here: the suffix removal
        //     // doesn't have a right boundary, so cost is (n - i) * a[i].
        //     // We'll handle suffix separately after the loops.
        
        // After filling dp, the answer is the minimum over all i and j such that j <= k of:
        // dp[i][j] + (n - i) * a[i], but we must ensure that the total removals = j + (n - i) <= k.
        // Because the suffix removal uses (n - i) operations.
        // Also, we could have the case where we remove everything? That would mean no kept element.
        // But we must have at least one kept element? Actually, if we remove all elements, what would
        // they become? There would be no neighbor to copy from. The operation requires a neighbor.
        // So we cannot remove all elements. At least one element must remain unchanged to provide
        // the value. So there is always at least one kept element. So our DP with at least one kept
        // is correct.
        
        // Also, what about the case where we keep a[1] and remove nothing? That's covered by dp[1][0].
        // What if we keep a[1] and remove some suffix? That's covered by suffix handling.
        
        // Let's test with examples.
        
        // Example 1: n=3, k=1, a=[3,1,2]
        // Initialize: dp[1][0] = 3 (kept a[1]=3)
        // dp[2][1] = 2 * a[2] = 2*1=2? Wait, p=2, p-1=1 <= k=1, so dp[2][1] = 2*1 = 2.
        // dp[3][2] = 3 * a[3] = 3*2=6, but 2 > k, so skip.
        // Now transitions from i=1:
        // dp[1][0]=3. p=2: rem=0, j+rem=0<=1, dp[2][0] = min(INF, 3 + 0*min(3,1) + 1) = 4.
        // p=3: rem=1, j+rem=1<=1, dp[3][1] = min(INF, 3 + 1*min(3,2) + 2) = 3+2+2=7.
        // From i=2:
        // dp[2][0]=4. p=3: rem=0, j+rem=0<=1, dp[3][0] = min(INF, 4 + 0*min(1,2) + 2) = 6.
        // dp[2][1]=2. p=3: rem=0, j+rem=1<=1, dp[3][1] = min(7, 2 + 0*min(1,2) + 2) = 4.
        // Now suffix: for each dp[i][j] with j + (n-i) <= k.
        // i=1: dp[1][0]=3, suffix rem=2, total=3 > k=1, skip.
        // i=2: dp[2][0]=4, suffix rem=1, total=1<=1, candidate = 4 + 1*a[2] = 4+1=5.
        //      dp[2][1]=2, suffix rem=1, total=2>1, skip.
        // i=3: dp[3][0]=6, suffix rem=0, total=0<=1, candidate = 6+0=6.
        //      dp[3][1]=4, suffix rem=0, total=1<=1, candidate = 4+0=4.
        // Minimum candidate = 4. Correct.
        
        // Example 2: n=1, k=3, a=[5]
        // dp[1][0] = 5. Suffix rem=0, candidate=5. Answer 5. Correct.
        
        // Example 3: n=4, k=2, a=[2,2,1,3]
        // Initialize: dp[1][0]=2; dp[2][1]=2*2=4; dp[3][2]=3*1=3 (since 2<=2); dp[4][3]=4*3=12 (3>2 skip).
        // Transitions:
        // i=1: dp[1][0]=2
        //   p=2: rem=0 -> dp[2][0]=min(INF, 2+0+2)=4
        //   p=3: rem=1 -> dp[3][1]=min(INF, 2+1*min(2,1)+1)=2+1+1=4
        //   p=4: rem=2 -> dp[4][2]=min(INF, 2+2*min(2,3)+3)=2+4+3=9
        // i=2: dp[2][0]=4, dp[2][1]=4
        //   p=3: rem=0 -> dp[3][0]=min(INF, 4+0+1)=5; dp[3][1]=min(4, 4+0+1)=5? Wait, dp[3][1] was 4, min(4,5)=4.
        //   p=4: rem=1 -> for j=0: dp[4][1]=min(INF, 4+1*min(2,3)+3)=4+2+3=9; for j=1: dp[4][2]=min(9, 4+1*min(2,3)+3)=9.
        // i=3: dp[3][0]=5, dp[3][1]=4, dp[3][2]=3
        //   p=4: rem=0 -> dp[4][0]=min(INF, 5+0+3)=8; dp[4][1]=min(9, 4+0+3)=7; dp[4][2]=min(9, 3+0+3)=6.
        // Suffix:
        // i=1: dp[1][0]=2, rem=3 -> total 3>2 skip.
        // i=2: dp[2][0]=4, rem=2 -> total 2<=2, candidate=4+2*a[2]=4+4=8; dp[2][1]=4, rem=2 -> total 3>2 skip.
        // i=3: dp[3][0]=5, rem=1 -> total 1<=2, candidate=5+1*a[3]=5+1=6; dp[3][1]=4, rem=1 -> total 2<=2, candidate=4+1=5; dp[3][2]=3, rem=1 -> total 3>2 skip.
        // i=4: dp[4][0]=8, rem=0 -> candidate=8; dp[4][1]=7, rem=0 -> candidate=7; dp[4][2]=6, rem=0 -> candidate=6.
        // Minimum is 5. Correct.
        
        // Example 4: n=6, k=3, a=[4,1,2,2,4,3]
        // We'll trust it works.
        
        // Complexity: O(n * k^2) per test case, sum n <= 3e5, k <= 10, so about 3e5 * 100 = 3e7 operations, which is fine in 1 second in C++.
        
        // Implementation details:
        // Use vector<vector<long long>> dp(n+1, vector<long long>(k+1, INF)).
        // 1-indexed for convenience.
        
        const long long INF = 1e18;
        vector<vector<long long>> dp(n+1, vector<long long>(k+1, INF));
        
        // Initialize by removing prefix
        for (int p = 1; p <= n; ++p) {
            int rem = p - 1;
            if (rem <= k) {
                dp[p][rem] = (long long)p * a[p-1];
            }
        }
        
        // Transitions
        for (int i = 1; i <= n; ++i) {
            for (int j = 0; j <= k; ++j) {
                if (dp[i][j] == INF) continue;
                // try next kept at p
                for (int p = i+1; p <= n && p - i - 1 <= k; ++p) {
                    int rem = p - i - 1;
                    if (j + rem <= k) {
                        long long cost = dp[i][j] + (long long)rem * min(a[i-1], a[p-1]) + a[p-1];
                        if (cost < dp[p][j + rem]) {
                            dp[p][j + rem] = cost;
                        }
                    }
                }
            }
        }
        
        long long ans = INF;
        for (int i = 1; i <= n; ++i) {
            for (int j = 0; j <= k; ++j) {
                if (dp[i][j] == INF) continue;
                int rem_suffix = n - i;
                if (j + rem_suffix <= k) {
                    long long total = dp[i][j] + (long long)rem_suffix * a[i-1];
                    if (total < ans) ans = total;
                }
            }
        }
        
        cout << ans << '\n';
    }
    return 0;
}
