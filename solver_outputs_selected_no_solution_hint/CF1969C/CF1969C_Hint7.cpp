#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n, k;
        cin >> n >> k;
        vector<int> a(n);
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }
        
        if (k >= n) {
            long long sum = 0;
            for (int x : a) sum += x;
            cout << sum << "\n";
            continue;
        }
        
        // dp[i][j][c] - minimum sum for prefix i, with j operations used,
        // c = 0: current element unchanged, c = 1: current element changed
        // We'll use 1-indexed for prefix, and we'll keep only previous row.
        // Actually we need to track the value of the last element? No, we can just
        // propagate the minimum possible value.
        // Let's think differently: we can change elements to neighbor values.
        // This is equivalent to: we can select up to k elements and set them to
        // the value of an adjacent element (which might also be changed).
        // The final array will have some elements equal to original, some equal to
        // neighbors. The sum is minimized by making elements as small as possible.
        // Since k <= 10, we can use DP over positions with state = number of operations used
        // and the value of the last element? But values are up to 1e9.
        // Better: we can think of the array after operations: each element is either
        // original a[i] or equal to some a[j] where j is adjacent to the position
        // after a chain of operations. Actually, if we change an element to a neighbor's value,
        // that neighbor might have been changed earlier. So the final value at position i
        // can be any a[j] such that the distance between i and j in terms of operations?
        // More precisely, if we perform operations sequentially, the value propagates.
        // The final value at position i is some original a[p] where p is within distance
        // equal to the number of operations used along the path.
        // Since k is small (<=10), we can consider that each position can take a value
        // from a small window around it.
        
        // Let's use DP: dp[i][j] = minimum sum for prefix up to i, having used j operations,
        // and the i-th element is unchanged (i.e., we haven't used an operation that changes
        // a[i] to something else? Actually we need to know the value at i to compute future
        // changes. But if we change a[i] to a neighbor's value, that neighbor could be i-1 or i+1.
        // This suggests we need to know the value at i-1.
        // Alternative: Since k is small, we can try all possibilities of which elements we change.
        // But n up to 3e5, so we need O(n * poly(k)).
        
        // Let's define dp[i][j][state] where state indicates the relationship between a[i] and a[i-1].
        // But values matter. However, note that if we change a[i] to a[i-1]'s value, then a[i] becomes
        // whatever a[i-1] is. So the value is determined by the original array and the pattern of changes.
        // We can think of the final array as being partitioned into segments where all elements in a segment
        // have the same value, which is the minimum original value in that segment? Not exactly, because
        // we can only propagate values, not create new minima. The operation replaces an element with a neighbor's value.
        // So the set of values in the final array is a subset of the original values.
        // To minimize sum, we want to replace large values with smaller neighboring values.
        // This is similar to: we can "spread" a small value to adjacent positions, each spread costs 1 operation.
        // So we can take a small value and expand it to cover up to k other positions.
        // But we can also chain: spread small value to neighbor, then that neighbor spreads further, etc.
        // So a single small value can cover a contiguous segment of length L using L-1 operations (if we start from that small value and expand outward).
        // However, we can also have multiple small values spreading.
        // The problem reduces to: choose a set of disjoint segments to "flatten" to some value within the segment,
        // such that the total number of operations (sum of (segment length - 1)) <= k, and we minimize the sum.
        // But wait: if we flatten a segment to its minimum value, we need to propagate that minimum to all positions.
        // The number of operations needed to make a segment of length L all equal to the minimum value m
        // is at most L-1 (if we start at the position of m and expand left/right). But if the minimum appears multiple times,
        // we might need fewer operations. Actually, to make a segment all equal to some value v that appears in the segment,
        // we can just expand v to cover the segment. The cost is the number of positions that are changed, which is L - (count of v in segment).
        // But we can also change v itself? No, we don't need to change the positions that already have v.
        // So if we pick a segment and a value v that appears in it, we can make the whole segment equal to v
        // with cost = (segment length) - (frequency of v in segment). We want to minimize sum, so we would pick v as the minimum value in the segment.
        // Then the sum reduction is sum of (a[i] - min) over the segment, and cost is number of elements not equal to min.
        // However, we can also have overlapping effects? If we do operations sequentially, we might change a value to something,
        // then later change it again. But that would be suboptimal because we could just directly change to the final value.
        // So we can assume each position is changed at most once, and the final value is some original value from the array.
        // Moreover, the set of positions that end up with a particular value v must form a contiguous segment? Not necessarily,
        // because we could have v at two separate places and expand from both, but they would meet. Actually, if we expand v from multiple sources,
        // the covered positions form a union of intervals that might be disconnected if there are other values in between that we don't change.
        // But we can always rearrange operations to make each value's coverage contiguous? Let's think:
        // Suppose we have array: 5 1 5 1 5. We want to change some 5s to 1. We can change the middle 5 to 1 using one operation (from left or right 1).
        // Then we have 5 1 1 1 5. The 1s are contiguous. If we change the first 5 to 1, we get 1 1 1 1 5. Still contiguous.
        // It seems that if we only propagate a single value, the set of positions that become that value will be a contiguous segment
        // containing all original occurrences of that value that we use as sources. But we could also have multiple different values spreading.
        // For example, we could have a segment flattened to 2 and another flattened to 3. They would be disjoint segments.
        // So the final array can be partitioned into contiguous segments, each segment having a constant value equal to some original element within that segment.
        // The cost for a segment is the number of positions in it that are not already equal to that value.
        // And total cost <= k.
        // This is a classic DP: dp[i][j] = min sum for prefix up to i, using j operations.
        // To compute dp[i][j], we consider the last segment ending at i, starting at some p <= i.
        // The segment [p, i] will be set to the minimum value in a[p..i]. Let min_val = min(a[p..i]).
        // The cost is (i - p + 1) - count of min_val in [p, i].
        // The sum for this segment is min_val * (i - p + 1).
        // Then dp[i][j] = min over p (dp[p-1][j - cost] + min_val * len).
        // This is O(n^2 * k) which is too slow.
        // But k is very small (<=10). Also n up to 3e5. We need a faster way.
        // Notice that the cost is at most the length of the segment. Since k <= 10, the segments we change must be short? Not necessarily:
        // we could have a segment of length 100 but with 99 occurrences of the minimum, so cost = 1. So length can be large.
        // However, the number of positions we actually change (cost) is <= k. So the total number of positions that are changed is at most k.
        // This means that in the final array, at most k positions differ from the original array.
        // Wait, is that true? If we change a position, it counts as one operation. But if we change a position multiple times, it still counts multiple operations.
        // But we can assume each position is changed at most once optimally. So the number of changed positions is exactly the number of operations used.
        // So at most k positions are changed. The rest remain as original.
        // Therefore, the final array differs from the original in at most k positions.
        // This is a crucial observation! Because each operation changes exactly one element. Even if we change an element to a neighbor's value,
        // and that neighbor might have been changed earlier, the element we change is still one position. So total changed positions <= k.
        // So we are selecting at most k positions to change, and we can change them to the value of an adjacent position (which might be original or already changed).
        // But since we only change at most k positions, we can think of it as: we can pick a set of positions to modify, and for each, we set it to the value of either left or right neighbor.
        // However, if we change a position, its neighbor might also be changed, so the value we copy could be a changed value.
        // But ultimately, the value we copy must originate from some original element within distance equal to the number of changes along the path.
        // Since total changes <= k, the distance from an original value to any changed position is at most k.
        // So each changed position takes the value of some original element within distance k.
        // This suggests we can just try all possibilities of which positions to change and to what, but that's exponential.
        // But we can use DP with state being the last few elements? Since k <= 10, the "window" of influence is small.
        // Actually, we can do DP over the array, keeping track of the last few values and how many operations used.
        // But values can be large, we can't keep all possible values.
        // However, note that the final value at position i can only be one of the original values in [i-k, i+k].
        // Because to affect position i, we must have a chain of changes starting from some original position j, and the number of changes is the distance |i-j|.
        // Since total changes <= k, the maximum distance is k. So a[i] can only become some a[j] with |i-j| <= k.
        // This is a key insight! Let's verify: Suppose we change position i to neighbor's value. That neighbor could be i-1 or i+1.
        // If we change i to i-1's value, and i-1 was already changed to i-2's value, then i effectively gets i-2's value with 2 operations.
        // So indeed, the value at i can be any a[j] such that the number of steps (operations) along the path from j to i is exactly the number of positions changed in between.
        // Since we only have at most k operations total, the distance from the original source to any changed position is at most k.
        // Therefore, for each position i, its possible final values are a subset of {a[j] : max(0, i-k) <= j <= min(n-1, i+k)}.
        // This window size is at most 2k+1 <= 21.
        // So we can compress values? Not exactly, but we can do DP with state being the value of the last element? Still too many.
        // But we can use the fact that we only care about the sum, and we can decide for each position whether to keep it or change it to one of the nearby values.
        // This looks like we can do DP with bitmask representing which of the last few positions were changed? But the value also matters.
        // Alternative perspective: Since we only change at most k positions, we can think of the array as mostly original, with some "mutations".
        // We can iterate over all possible sets of changed positions? No, too many.
        // Let's go back to the segment DP idea but with the constraint that cost <= k. The cost is the number of changed positions in the segment.
        // Since total cost <= k, the segments we change are limited. In fact, we can have at most k changed positions total.
        // So we can think of the original array as having some "bad" elements we want to reduce. We can reduce an element by changing it to a smaller neighbor.
        // But we can also change a neighbor to an even smaller value, creating a chain.
        // This is equivalent to: we can select a set of positions to change. For each changed position, its new value is the minimum of some original values within distance d, where d is the number of operations used to propagate that minimum to this position.
        // Actually, if we decide to change a set S of positions, the optimal way to assign values is to propagate the smallest possible original values to them.
        // This is like we have a graph where edges are between adjacent positions, and we can "activate" up to k edges to propagate values.
        // But maybe we can use the fact that k is tiny to do a DP that considers a sliding window of size O(k).
        // Let's try to design DP: dp[i][j][mask]? 
        // Another idea: Since k <= 10, we can consider that the only positions that might change are those that are within distance k of a "small" value.
        // But we don't know which are small.
        // Let's think about the problem as: we want to subtract as much as possible from the total sum by using at most k operations.
        // Each operation can be seen as: pick an element and set it to the value of an adjacent element. This can reduce the element's value if the neighbor is smaller.
        // But we can also chain reductions. The maximum reduction for a position i is if we can bring the global minimum to it, but that might cost distance operations.
        // So we want to pick up to k positions to reduce, and for each, we can reduce it to the minimum value within some distance L, paying L operations for that position? Not exactly, because operations can be shared: if we bring a small value to cover a contiguous block, the operations are shared.
        // Actually, if we bring a small value to cover a segment of length L, we pay L - (count of that value in segment) operations. The total reduction is sum of (original - small) over the segment.
        // This is a knapsack-like problem: we have segments with (cost, reduction). We want to choose disjoint segments with total cost <= k to maximize reduction.
        // Since k is small, the number of segments we can choose is at most k (each segment costs at least 1). But segments can be long.
        // However, the number of possible segments is O(n^2). We need to find the best segments efficiently.
        // Notice that for a fixed segment, the best value to set is the minimum in that segment. The cost is the number of elements not equal to that minimum.
        // So if we precompute for each position the next occurrence of a smaller value? Not sure.
        // Let's consider that we only care about segments where the minimum is at some position, and we expand left/right until we hit a smaller value? Because if we include a smaller value, that smaller value would be the new minimum, and we might as well expand from that smaller value instead.
        // So an optimal segment will have its minimum at one of its endpoints? Not necessarily, but the minimum will be somewhere inside.
        // Actually, if we have a segment and we set it all to its minimum m, the cost is the number of elements > m. The elements equal to m are free.
        // We can think of the array as having "peaks" and "valleys". We want to "lower" some peaks by spreading nearby valleys.
        // Since k is tiny, we can just try all possibilities of which valleys to spread and how far.
        // Another angle: The operation is exactly: a[i] = a[i-1] or a[i] = a[i+1]. This is like we can copy values left or right.
        // After at most k operations, the array will have at most k positions different from original.
        // So we can iterate over the number of operations used, and for each operation, we choose a position and a direction.
        // But the order matters because copying can propagate.
        // However, we can think of the final array as being obtained by taking the original array and applying a sequence of copy operations.
        // This is equivalent to: we can select a set of positions to be "sources" (original values) and for each other position, its value is the source value that can reach it via a path of copies, with the total number of copies (edges) <= k.
        // This is exactly the problem of finding a set of edges (copy operations) to activate such that the sum of values is minimized.
        // This is a minimum spanning tree-like problem on a line graph, but with node weights? Actually, we want to assign each node a value from the original values, with the constraint that if node i gets value from node j, then all nodes on the path between i and j must also get that same value? Not necessarily: if we copy from i to i+1, then i+1 gets i's value. Then we can copy from i+1 to i+2, so i+2 gets i's value. The intermediate nodes get the same value. So if a node gets a value from a source, all nodes on the path also get that value (since the value propagates step by step). So the set of nodes that get a particular source value forms a contiguous segment containing that source.
        // Therefore, the final array is a partition into contiguous segments, each segment having a value equal to some original element in that segment.
        // The cost is the number of nodes that are not the source node in their segment (i.e., nodes that were changed).
        // So we are back to the segment DP.
        // Now, how to do it efficiently with k <= 10?
        // Since total cost <= 10, the number of segments we actually change is at most 10 (each segment we change has at least 1 changed node). But we can also have segments of length 1 that we don't change (cost 0). So we can think of the array as being partitioned into segments, where some segments are "active" (we changed some nodes in them) and some are "inactive" (all nodes keep original values). The total number of changed nodes across all active segments is <= k.
        // So we can have at most k active segments, and each active segment can be long but with only a few changed nodes (the ones not equal to the segment's chosen value).
        // This suggests we can do DP over the array, and at each position we decide whether to start an active segment or not.
        // But the cost of an active segment depends on the chosen value and the positions of that value within the segment.
        // Since k is small, the number of changed nodes in an active segment is small. That means within an active segment, almost all nodes already equal the chosen value. So the chosen value must appear many times in that segment. In fact, the number of nodes not equal to the chosen value is at most k.
        // So an active segment is essentially a contiguous block where the chosen value is the majority, and we change a few outliers.
        // This means we can consider each position as a potential "center" of an active segment, and we expand left/right, but we can only afford to change up to k nodes total.
        // We can think of it as: we can select up to k nodes to change. For each changed node, we can set it to the value of any adjacent node (which might be original or already changed). But since we only change a few nodes, we can just try all possibilities of which nodes to change and to what, using the fact that the value we assign to a changed node must come from within distance k.
        // Let's try a DP with state: dp[i][j] = minimum sum for prefix up to i, with j operations used, and we know the value of a[i]? But a[i] can be many things.
        // However, note that if we don't change a[i], its value is original a[i]. If we change it, its value must be one of the values in [i-k, i+k] original.
        // Since k <= 10, for each i, there are at most 21 possible values. So we can do DP with state: dp[i][j][v] = min sum for prefix i, using j ops, and the value at i is v (where v is one of the possible values for position i). But v can be up to 1e9, we need to map it. The number of distinct possible values across all i is O(n * k), which is up to 3e5 * 21 ~ 6e6, too large for state.
        // But we can compress: for each i, the possible values are a subset of a[i-k..i+k]. We can just keep the index of the source position rather than the value. That is, the value at i is a[src] for some src in [i-k, i+k]. So state could be the offset src - i, which ranges from -k to k. That's at most 21 possibilities.
        // So dp[i][j][d] = minimum sum for prefix up to i, using j operations, and the value at i is a[i+d] (where d is in [-k, k] and i+d within [0, n-1]).
        // But wait: the value at i could also be the result of a chain of copies that doesn't directly come from an original source within distance k? Actually, as argued, any changed value must originate from some original source within distance equal to the number of operations used along the path. Since total operations <= k, the distance from the source to any changed node is at most k. So the source must be within [i-k, i+k]. So yes, the final value at i is some a[j] with |i-j| <= k.
        // But is it possible that the source is within [i-k, i+k] but the path uses more than k operations? No, because the path length is |i-j|, and each step is one operation. So if |i-j| <= k, we can achieve it with |i-j| operations (by copying along the path). But we might use fewer operations if some intermediate nodes already have that value? Actually, to make a[i] equal to a[j], we need to copy a[j] to j+1, then j+2, ..., up to i (or down). That requires |i-j| operations if none of the intermediate nodes already equal a[j]. If some already equal, we can skip them, but the number of operations is still at most |i-j|. So the cost to set a[i] to a[j] is at most |i-j|, but could be less if some intermediate nodes are already a[j]. However, in our DP, we are counting operations explicitly. So we need to know how many operations were used to achieve the current value at i.
        // This suggests a DP that tracks the "source" of the value at i, and the number of operations used so far. When we move from i-1 to i, we can either keep a[i] original (cost 0, source = i), or we can copy from i-1 (cost 1, source = source of i-1), or we can copy from i+1? But we process left to right, so copying from i+1 is tricky because we haven't processed i+1 yet. However, we can also copy from i+1 to i, which means we change a[i] to a[i+1]'s value. That operation would be performed when we are at i+1? Actually, the operation can be done in any order. If we want a[i] to get a[i+1]'s value, we can do it by first setting a[i+1] to something, then copying to a[i]. But that means the value at i comes from the right. In a left-to-right DP, we can handle this by allowing the value at i to be determined later? This is getting complicated.
        // Alternative: Since k is very small, we can use the fact that the total number of changed positions is at most k. So we can just try all combinations of which positions are changed? But n is up to 3e5, we can't try all subsets.
        // However, we can use the segment DP with the observation that the cost of a segment is the number of elements not equal to the minimum. Since total cost <= 10, the segments we choose must have very few elements not equal to the minimum. That means for any chosen segment, the minimum value must appear at almost all positions in the segment. So the segment is essentially a contiguous block where the minimum is the dominant value.
        // We can precompute for each position the nearest smaller element to the left and right. Then the "valleys" are local minima. The only values worth spreading are local minima (or maybe not, but if we spread a non-minimum, we could instead spread the minimum of the segment and get a better reduction for the same cost). So we only need to consider segments that are expanded from a local minimum until we hit a smaller value. Because if we include a smaller value, that smaller value becomes the new minimum and we should have expanded from it instead.
        // So the optimal segments are maximal intervals where a certain value is the minimum. That is, for each position i, consider the maximal interval [L, R] such that a[i] is the minimum in that interval. Then if we set this entire interval to a[i], the cost is (R-L+1) - (frequency of a[i] in [L,R]). The reduction is sum_{j=L}^R (a[j] - a[i]). We can choose a subsegment of this interval? If we take a subsegment, the minimum is still a[i] (if we don't include anything smaller), and the cost and reduction scale accordingly. But we might want to take a subsegment to save operations. Since we have a total budget k, we want to pick a set of disjoint subsegments from these maximal intervals to maximize total reduction.
        // This is now a problem: we have a set of intervals (each maximal interval for each position), and we can choose subsegments within them. But there are O(n) maximal intervals (each position defines one, but many are identical). Actually, the number of distinct maximal intervals where a[i] is the strict minimum? If there are equal values, it's trickier. But we can break ties by index to ensure strictness.
        // Even then, we have O(n) intervals, and we need to choose subsegments with total cost <= k. Since k <= 10, we can maybe do a DP that scans left to right and keeps track of the best reductions for each possible cost.
        // Let dp[i][j] = maximum reduction we can get in prefix up to i using exactly j operations.
        // To compute dp[i][j], we can either not change a[i] (dp[i][j] = dp[i-1][j]), or we can end a segment at i. The segment [p, i] will be set to the minimum value in that segment. Let m = min(a[p..i]). The cost = number of elements in [p, i] not equal to m. The reduction = sum_{t=p}^i (a[t] - m). Then dp[i][j] = max(dp[i][j], dp[p-1][j - cost] + reduction).
        // This is O(n^2 * k) which is too slow.
        // But note that cost <= k, so the segment [p, i] can have at most k elements not equal to m. That means in [p, i], almost all elements are equal to m. So m must be the majority value. This severely restricts the possible segments.
        // In fact, for a fixed i, as we move p leftwards, the minimum m can only decrease. The cost is the number of elements > m. We want cost <= k. So we can only extend p leftwards as long as the number of elements > current minimum is <= k. Since k <= 10, the number of elements > m in the segment is at most 10. So the segment can contain at most 10 elements that are not equal to the minimum. This means the segment is composed of a bunch of m's and at most 10 other elements.
        // Therefore, for a fixed i, there are only a few possible p's that give cost <= k. Specifically, as we go left from i, we can keep track of the minimum and the count of elements not equal to it. Once the count exceeds k, we stop. So for each i, we only need to consider O(k) possible left endpoints? Not exactly, because the minimum might change, resetting the count. But the total number of distinct (p, m) pairs with cost <= k is small.
        // Let's analyze: Starting from i and moving left, we maintain the current minimum m and the number of "bad" elements (those > m). When we encounter an element equal to m, cost doesn't increase. When we encounter an element > m, cost increases by 1. When we encounter an element < m, the minimum updates to this new smaller value, and all previously seen elements that were > new m become "bad", but some might now be equal? Actually, if we see a smaller value, the new minimum is that value. The elements we previously saw that were > old m are still > new m, so they remain bad. The elements that were equal to old m are now > new m, so they become bad! So the cost can jump by a lot. But since total cost must be <= k, the number of elements that are not equal to the final minimum must be <= k. So if we update the minimum to a smaller value, the number of elements that are not equal to this new minimum is at least the number of elements we've seen so far (since all previous elements are >= old m > new m? Actually, if old m > new m, then all previous elements are >= old m > new m, so they are all > new m, thus they all become bad). So the cost becomes at least the length of the segment. Since length can be large, cost would exceed k immediately unless the segment is very short. Therefore, the minimum can only decrease if the segment is very short (length <= k). Otherwise, the cost would exceed k.
        // This means that for segments with cost <= k, the minimum value must be the minimum of the entire segment, and it must appear at almost all positions. In particular, the segment can contain at most k elements that are not the minimum. So if we extend the segment and encounter a new minimum, the length of the segment at that point is at most k (because all previous elements become bad). So the segment length is at most k when the minimum updates. After that, we can only extend if we see elements equal to the new minimum (cost doesn't increase) or if we see elements > new minimum (cost increases by 1, up to k). So the segment can grow longer only by including elements equal to the minimum. Thus, the segment consists of a "core" where the minimum is established (length <= k) and then extensions of elements equal to the minimum.
        // This suggests that for each i, the possible left endpoints p that yield a valid segment with cost <= k are limited. We can just iterate p from i down to max(0, i - something) but we need to be careful.
        // Since k <= 10, we can simply for each i, try all p from i down to 0, but break early when cost exceeds k. However, cost can be 0 for a long stretch if all elements equal the minimum. So we might not break early. But we can break when the number of elements not equal to the current minimum exceeds k. The current minimum is the minimum in [p, i]. As we move p left, the minimum can only decrease. When it decreases, the cost (number of elements > new minimum) becomes at least the number of elements seen so far (since all previous were >= old min > new min). So if the length so far is > k, cost will exceed k. So we only need to consider p such that the distance from i to p is at most k, OR the segment [p, i] has the same minimum throughout and we only add elements equal to that minimum. But if we add elements equal to the minimum, cost doesn't increase, so we can extend arbitrarily far as long as we only see elements equal to the minimum. But wait: if we extend and see an element equal to the minimum, cost remains the same. So we could have a very long segment with cost 0 (if all elements are already equal to the minimum). In that case, we don't need to change anything, so reduction is 0. We only care about segments where cost > 0 (we actually change some elements). If cost = 0, reduction = 0, so it doesn't affect the DP. So we can ignore segments with cost 0.
        // Therefore, for segments with cost > 0 and <= k, the number of elements not equal to the minimum is between 1 and k. This means the segment can have at most k "bad" elements. The "good" elements (equal to the minimum) can be many. But as we extend the segment, if we only add good elements, the cost doesn't change, and the reduction increases by (a[t] - min) = 0. So adding good elements doesn't increase reduction! Because if a[t] == min, then a[t] - min = 0. So extending a segment by including elements that are already equal to the minimum gives no additional reduction. So we only get reduction from the "bad" elements. Therefore, the total reduction of a segment is the sum of (a[t] - min) over the bad elements. The good elements contribute 0 reduction. So the segment's reduction depends only on the bad elements and the minimum. The good elements just increase the length but don't affect cost or reduction.
        // This means we can think of the segment as just a set of bad elements that we want to reduce to some minimum value, and we can optionally include any number of good elements (equal to the minimum) in between without affecting cost or reduction. But including good elements might be necessary to connect the bad elements into a single segment? Actually, if we have two bad elements separated by good elements, we can choose to include the good elements in the segment to make it contiguous. But since good elements don't add cost or reduction, we can always include them for free. So the segment is essentially defined by a contiguous interval that contains some bad elements, and the minimum is the minimum of that interval. The cost is the number of bad elements. The reduction is the sum of (bad - min).
        // But wait: if we include good elements, they might be smaller than the bad elements but equal to the minimum. That's fine. If we include an element that is smaller than the current minimum, it becomes the new minimum, and all previous good elements become bad (since they are > new minimum). So that would increase cost. So we cannot include an element smaller than the minimum without increasing cost.
        // So the optimal segment for a given set of bad elements is to take the contiguous interval from the leftmost bad to the rightmost bad, and the minimum is the minimum in that interval. If there is an element smaller than the minimum of the bad elements inside that interval, it would become the new minimum and make some good elements bad, potentially increasing cost. But we can choose to exclude that smaller element? If we exclude it, the segment would be split. So we have to consider the trade-off.
        // Given the complexity, maybe we can use the fact that k <= 10 to do a DP that considers the exact set of changed positions. Since we only change at most 10 positions, we can think of the array as having at most 10 "mutations". We can iterate over the number of operations used, and for each operation, we choose a position and copy from left or right. But the order matters. However, we can use BFS/DP on the state of the array? No, array is too large.
        // Another perspective: The operation is equivalent to: we can take any element and set it to the value of its left or
