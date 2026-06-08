#include <bits/stdc++.h>
using namespace std;

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

        if (k >= n) {
            long long sum = 0;
            for (long long x : a) sum += x;
            cout << sum << '\n';
            continue;
        }

        const long long INF = 1e18;
        // dp[i][j] = min sum for prefix up to i, with j operations used,
        // and the current segment's value is a[i] (we keep a[i] as the value of the last segment)
        // We'll use a different DP: dp[i][j] = min sum for first i elements, using j operations,
        // where the last element is a[i] (unchanged) or we have a segment of equal values ending at i.
        // Better: dp[i][j] = min sum for prefix i (1-indexed) with j operations, and the last element
        // is forced to be a[i] (we don't change a[i] itself, but we can change previous ones to a[i]).
        // Actually, we can think of segments of equal values. We choose some positions to be "anchors"
        // that keep their original values, and the elements between anchors are changed to the minimum
        // of the two neighboring anchors? Wait, operation: replace an element with any of its neighbor's value.
        // This means we can propagate values. The final array will consist of blocks of equal values.
        // Each block's value must be one of the original values in that block (since we only copy neighbors).
        // To minimize sum, for a block we would choose the minimum value in that block.
        // The number of operations needed to form a block of length L with a chosen value is L-1 (if the value
        // is already present in the block, we can propagate it). But we can also have multiple blocks.
        // The total operations = sum over blocks (length of block - 1) = n - number of blocks.
        // So using at most k operations means we can have at least n - k blocks.
        // We want to partition the array into at least n - k contiguous blocks, and for each block we take
        // the minimum value in that block, and sum them up. We want to minimize the total sum.
        // Since k <= 10, n - k is large, but we can have up to n blocks (k=0). Actually we want to minimize
        // sum of block minimums. This is equivalent to: we can keep some elements as "anchors" (they remain
        // unchanged), and the elements between anchors are changed to the minimum of the two surrounding anchors?
        // No, if we have a block, we can choose any value from that block to be the block's value. The minimum
        // in the block is optimal. So we want to partition into blocks, each block contributes its minimum.
        // Number of blocks = B. Operations = n - B <= k => B >= n - k.
        // We want to minimize sum of block minimums with at most k operations, i.e., at least n - k blocks.
        // Since k is small (<=10), we can think of merging adjacent elements. Initially B = n, sum = sum a_i.
        // Each merge of two adjacent blocks reduces B by 1, and the new block's minimum is min(min1, min2).
        // The cost reduction = min1 + min2 - min(min1, min2) = max(min1, min2). Wait, sum of minimums decreases
        // by (min1 + min2) - min(min1, min2) = max(min1, min2). So merging two blocks reduces the sum by the
        // maximum of their minimums. We want to maximize the total reduction by doing at most k merges.
        // But merges can only be done on adjacent blocks. This is like: we have an array of values (initially a_i).
        // We can merge adjacent elements, the new element takes the minimum of the two. We want to perform at most k
        // merges to minimize the sum. This is equivalent to: we can select up to k elements to "remove" by merging
        // them with a neighbor, and the cost we save is the value of the removed element (but careful: if we merge
        // a block with value v into a block with value u < v, we save v; if u > v, we save u? Actually, if we merge
        // two blocks with minimums m1 and m2, the new minimum is min(m1,m2). The sum decreases by m1+m2 - min(m1,m2)
        // = max(m1,m2). So we save the larger of the two minimums. So we want to choose up to k merges to maximize
        // the sum of max(m1,m2) over merges. But merges are sequential and change the blocks.
        // This is a known problem: we can think of it as selecting a set of elements to be "deleted" by merging
        // them into a neighbor. Actually, if we merge two blocks, the larger minimum disappears. So we are effectively
        // deleting the larger minimum from the sum. We can delete up to k elements from the sum, but we can only
        // delete an element if it is the larger of two adjacent blocks at the time of merge. However, we can always
        // achieve deleting any set of up to k elements? Not exactly, because the order matters. But note that we can
        // always merge an element with a smaller neighbor to delete the larger element. If we want to delete a set of
        // elements, we can just merge each of them into a smaller adjacent element. The only issue is if two elements
        // we want to delete are adjacent and both larger than their other neighbors? We can still do it by merging
        // one into the other? Actually, if we have a sequence, we can always delete any k elements as long as we
        // don't delete all elements? Let's think: we want to minimize sum, so we want to delete the largest possible
        // values. Can we always delete the k largest elements? Not necessarily, because if the largest element is
        // surrounded by even larger elements? But all elements are positive. If we have a large element, we can merge
        // it with a smaller neighbor to delete it. The only problem is if all neighbors are larger, then merging
        // would delete the neighbor instead. But we can choose which neighbor to merge with. If we want to delete a
        // specific element, we can merge it with a neighbor that is smaller. If no neighbor is smaller, then the
        // element is a local minimum. We cannot delete a local minimum by merging it with a larger neighbor because
        // that would delete the larger neighbor. So we can only delete elements that are not local minima? Actually,
        // if we merge a local minimum with a larger neighbor, the new block's minimum is the local minimum, so the
        // larger neighbor is deleted. So we can delete the larger neighbor. So we can delete any element that is not
        // the minimum of the whole array? But we can do multiple merges. The optimal strategy is to select a set of
        // elements to keep as the final block minimums. The final blocks will have minimums that are some subset of
        // the original elements. The sum is the sum of these minimums. The number of operations is n - number of blocks.
        // So we want to choose a subset of indices to be the "block representatives" (the minimums of the blocks),
        // such that the blocks form a partition of the array. The condition for a partition: if we choose indices
        // i1 < i2 < ... < iB as the positions of the minimums of the blocks, then for each block, the minimum is
        // the minimum value in that segment. But we can choose the minimum of the segment to be any element in the
        // segment. To minimize the sum, for a given partition, we would take the minimum in each segment. So the
        // problem reduces to: partition the array into B >= n - k segments, minimize sum of segment minimums.
        // This is a classic DP: dp[i][j] = min sum for prefix i using j segments.
        // n up to 3e5, k up to 10, so B can be n, n-1, ..., n-k. That's only k+1 possible values for number of segments.
        // But DP with O(n * k) states and O(n) transition is O(n^2 * k) which is too slow.
        // We need a faster way. Since k is very small (<=10), we can think of it as: we start with all elements as
        // segments (sum = total sum). We can merge adjacent segments up to k times. Each merge combines two adjacent
        // segments and the new segment's value is the minimum of the two. The reduction in sum is max(min1, min2).
        // So we want to choose up to k merges to maximize the sum of max(min1, min2). This is equivalent to: we have
        // an array of values. We can perform up to k operations: pick an element and merge it with left or right
        // neighbor, the element disappears and the neighbor becomes min(neighbor, element). The reduction is the
        // value of the disappeared element if it was larger than the neighbor, otherwise the neighbor's value.
        // Actually, if we merge a into b, new value = min(a,b). Reduction = a+b - min(a,b) = max(a,b). So we remove
        // the larger of the two. So we can think of it as: we can delete up to k elements, but when we delete an
        // element, we must merge it with an adjacent element, and the cost reduction is the maximum of the two.
        // But if we delete an element that is smaller than its neighbor, the reduction is the neighbor's value,
        // meaning we effectively delete the neighbor instead? That's confusing.
        // Alternative perspective: The final array consists of blocks of equal values. Each block's value is the
        // minimum of the original elements in that block. The operations used = sum (block length - 1) = n - number of blocks.
        // So we want to choose a set of block boundaries. There are n-1 possible boundaries between adjacent elements.
        // If we don't put a boundary between i and i+1, they are in the same block, which costs 1 operation.
        // So we can omit up to k boundaries. Initially we have all boundaries (n blocks). Omitting a boundary merges
        // two blocks. The new block's minimum is the minimum of the two blocks. The reduction in sum is the sum of
        // the two minimums minus the new minimum = max(min1, min2). So each omitted boundary gives a reduction equal
        // to the maximum of the minimums of the two blocks it merges. But the minimums of blocks change as we merge.
        // This is like: we have an array a. We want to remove up to k elements from the sum, but we can only remove
        // an element if it is "covered" by a merge? Actually, consider the process backwards: start with the final
        // blocks. Each block has a minimum value which is some a_i. The sum is sum of these minimums. The blocks are
        // formed by merging adjacent elements. The minimum of a block is the minimum of its elements. So if we have
        // a block with minimum m, and we split it into two blocks, the sum increases by the minimums of the two parts.
        // This is like: we can think of the array as a sequence of values. We want to select a set of elements to be
        // the "surviving" minimums. The other elements are "deleted" by merging them into a surviving element.
        // An element can only be deleted if it is merged into a surviving element that is smaller or equal? Actually,
        // if we have a surviving element with value v, we can merge adjacent larger elements into it, and they disappear.
        // But if an adjacent element is smaller, merging would make the smaller element survive and v disappear.
        // So a surviving element must be a local minimum of the block? Not necessarily, because we can merge a larger
        // element into a smaller one, so the smaller one survives. So the surviving elements are exactly the elements
        // that are never merged into a smaller element. In other words, the final minimums are a subset of the original
        // elements such that between any two consecutive chosen minimums, all elements are >= both? No, if we have
        // a block, its minimum is the smallest element in that block. That smallest element will be the one that
        // survives. All other elements in the block are >= it and are merged into it (or into other elements that
        // eventually merge into it). So the final minimums are exactly the elements that are the minimums of their
        // respective blocks. They must be the minimum in their block. So if we choose a set of indices to be the
        // block minimums, they must be such that each block's minimum is indeed the minimum in that block. That means
        // for a block from L to R with minimum at index m, a_m <= a_i for all i in [L,R]. And the blocks partition
        // the array. So we are choosing a set of indices that are "record lows" in some sense? Actually, if we just
        // take the whole array, the minimum is the global minimum. If we split, we get more minimums.
        // This is equivalent to: we can choose up to k positions to "cut" the array, and for each segment, we take
        // its minimum. We want to minimize the sum of segment minimums.
        // Since k is small (<=10), we can think of it as: we can change up to k elements to the value of some neighbor.
        // The operation: replace an element with any of its neighbor's value. This means we can propagate values.
        // After up to k operations, the array will have at most k elements changed? Actually, one operation changes
        // one element. So at most k elements are changed. The sum is sum of original array minus the reductions from
        // the changes. When we change an element a_i to a neighbor's value, the new value is either a_{i-1} or a_{i+1}.
        // The reduction is a_i - new_value (if new_value < a_i), otherwise negative reduction (sum increases), but we
        // would never do that. So we only change an element if the neighbor is smaller. So we can reduce the sum by
        // changing up to k elements to smaller adjacent values. But note that after changing one element, the neighbor
        // might change, allowing further changes. However, the operation is "replace it with any of its neighbor's value"
        // at the time of operation. So we can do a sequence of operations. This is exactly the same as merging blocks.
        // The minimum sum is achieved by taking the original array and for each element, we can either keep it or
        // change it to the value of some element that can be reached by a chain of operations of length at most k?
        // Actually, each operation changes one element. So after k operations, at most k elements have been changed.
        // The final array has at most k elements different from original. But they can be changed to values that were
        // originally elsewhere. The total sum is sum of final array. We want to minimize this sum.
        // This is equivalent to: we can select up to k elements and decrease them to the value of some adjacent element
        // (which might also have been decreased). But the final value of any element is the minimum of some contiguous
        // segment of the original array? Because we can only copy values from neighbors. So the final value at position i
        // must be equal to some original a_j where j is in the same connected component of the graph where edges are
        // between adjacent elements if we performed operations between them? Actually, if we never change an element,
        // it keeps its value. If we change it, it takes a neighbor's value. That neighbor might have been changed
        // earlier. So the value propagates. The set of elements that end up with the same value form a contiguous
        // segment, and that value is the minimum of the original values in that segment? Not necessarily the minimum,
        // because we could choose to propagate a larger value? But we want to minimize sum, so we would propagate the
        // smallest value in the segment. So indeed, the final array is partitioned into blocks of equal values, and
        // each block's value is the minimum of the original elements in that block. The number of operations used
        // is exactly the number of elements that were changed, which is sum (block length - 1) = n - number of blocks.
        // So we are back to the same formulation: partition into B blocks, B >= n - k, minimize sum of block minimums.
        // Now, how to compute this efficiently?
        // Since k <= 10, we can use DP with state: dp[i][j] = minimum sum for prefix i using exactly j blocks.
        // But we need to compute the minimum in a segment quickly. The transition: dp[i][j] = min_{0 <= p < i} (dp[p][j-1] + min(p+1..i)).
        // This is O(n^2 * k) which is too slow for n=3e5.
        // However, k is very small. We can think of it differently: we want to remove up to k elements from the sum
        // by merging. Each merge of two adjacent blocks reduces the sum by the maximum of their minimums.
        // This is similar to: we have an array, we can perform up to k operations, each operation picks an element
        // and merges it with a neighbor, the cost reduction is the maximum of the two. We want to maximize reduction.
        // Notice that if we merge two blocks, the new block's minimum is the minimum of the two. So the larger minimum
        // is "deleted" from the sum. So we are effectively deleting up to k elements from the sum, but we can only
        // delete an element if it is the larger of two adjacent blocks at the time of deletion. However, we can always
        // delete any element that is not the global minimum? Let's test: array [3,1,2]. k=1. We can delete 3 by merging
        // with 1 (reduction 3), or delete 2 by merging with 1 (reduction 2). We cannot delete 1 because it's the
        // smallest. So we can delete any of the larger elements. What if array [2,3,1]? k=1. We can delete 3 by merging
        // with 2 (reduction 3) or with 1 (reduction 3). We can delete 2 by merging with 3? Merging 2 and 3: max(2,3)=3,
        // so reduction 3, but the new block is min(2,3)=2, so 3 is deleted, not 2. So we cannot delete 2 because it's
        // smaller than 3. But we could merge 2 with 1: max(2,1)=2, reduction 2, new block min=1, so 2 is deleted.
        // So we can delete 2 by merging with 1. So we can delete any element as long as it has a smaller neighbor.
        // If an element is a local minimum (smaller than both neighbors), we cannot delete it directly, but we could
        // delete its neighbors first, then it might become adjacent to something else? But if we delete a neighbor,
        // the local minimum's neighbor becomes the next element. If that next element is larger, we still can't delete
        // the local minimum. Actually, a local minimum can never be deleted because to delete it, we need to merge it
        // with a smaller element, but there is none. So local minima (including global minimum) can never be deleted.
        // They will always remain as block minimums. So the elements that can be deleted are exactly those that are
        // not local minima? But after some deletions, the array changes. The elements that survive are exactly the
        // elements that are never merged into a smaller element. This is equivalent to: the final block minimums are
        // a subset of the original elements that form a "valley" sequence? Actually, if we look at the original array,
        // the elements that can be deleted are those that are not "record lows" from some direction? 
        // Let's think about the optimal strategy. We want to maximize the sum of deleted elements. We can delete up to k
        // elements. Each deletion removes one element from the sum. But we can only delete an element if we merge it
        // into a smaller adjacent element. So we can delete any element that has a smaller neighbor. If we delete an
        // element, its neighbor might become adjacent to the next element, potentially allowing further deletions.
        // This is exactly the process of: we can delete any element that is not a left-to-right minimum or right-to-left
        // minimum? Actually, consider the array. The elements that can never be deleted are those that are smaller than
        // all elements to their left and right? No, the global minimum can never be deleted. Also, if there are multiple
        // local minima, they can't be deleted. But if we delete the elements between them, they might become adjacent
        // and then one could be deleted? If we have two local minima with a larger element between them, we can delete
        // the larger element. Then the two minima become adjacent. If they are equal, merging them doesn't change sum.
        // If one is smaller, the larger one can be deleted by merging into the smaller one. So eventually, only the
        // global minimum remains if we do enough operations? But we are limited to k operations.
        // Actually, the problem is equivalent to: we can choose up to k elements to "remove" from the sum, but we can
        // only remove an element if we also "cover" it by a smaller element that remains. The total reduction is the
        // sum of the removed elements. We want to maximize the sum of removed elements. The removed elements must be
        // such that each removed element is merged into some smaller element that is not removed (or is removed later?
        // but if the smaller element is also removed, then the reduction would be the smaller element? No, if we merge
        // a into b, and later merge b into c, the reduction from the first merge is max(a,b). If b > a, reduction is b.
        // Then b is gone, and we merge the new block (min=a) into c, reduction max(a,c). So total reduction = b + max(a,c).
        // If we instead merged b into c first, then a into the result, reduction = max(b,c) + max(a, min(b,c)). 
        // This is getting complicated.
        // Let's go back to the partition DP. Since k <= 10, we can use the fact that we only need to consider up to k
        // merges. That means we start with n blocks, and we merge at most k times. So the number of blocks is at least
        // n - k. Since k is small, the number of blocks is large. This means most blocks are of length 1 (i.e., we keep
        // the original elements). Only at most k merges happen, so at most 2k elements are involved in merges? Actually,
        // each merge reduces the number of blocks by 1. So after k merges, we have n - k blocks. The total number of
        // elements in blocks of length > 1 is at most 2k? Because each merge combines two blocks into one, so the total
        // number of elements in merged blocks is at most the number of original blocks that were merged. Initially all
        // blocks have length 1. Each merge takes two blocks and makes one block. The number of elements in the resulting
        // block is the sum of their lengths. If we do k merges, the total number of elements that are in blocks of length
        // > 1 is at most 2k? Actually, if we merge disjoint pairs, we can have up to 2k elements in merged blocks.
        // But merges can be cascaded: merge 1 and 2, then merge the result with 3, etc. Then a block of length 3 uses
        // 2 merges and involves 3 elements. In general, a block of length L uses L-1 merges. So the sum of (L-1) over
        // all blocks of length > 1 is exactly the number of merges, which is <= k. So the total number of elements
        // that are in blocks of length > 1 is sum L = sum (L-1) + number of such blocks <= k + (number of merged blocks).
        // But the number of merged blocks is at most k (since each merge reduces block count by 1, starting from n,
        // we end with n - k blocks, so the number of original blocks that were merged into others is exactly k? Wait:
        // initially n blocks. After k merges, we have n - k blocks. The number of original blocks that are "absorbed"
        // into other blocks is exactly k. Because each merge makes one block disappear. So exactly k original elements
        // are "deleted" (their values are removed from the sum, but they might be replaced by smaller values? Actually,
        // when a block is absorbed, its minimum is either kept or discarded. The sum reduction is the maximum of the
        // two minimums. So the deleted value is the larger of the two minimums. So we are deleting exactly k values
        // from the multiset of block minimums? Not exactly, because the surviving block's minimum might be smaller than
        // both? No, the new minimum is the minimum of the two, so one of the two minimums is discarded. So each merge
        // discards exactly one block minimum. So after k merges, we discard exactly k block minimums. The final sum
        // is the sum of the original array minus the sum of the discarded block minimums? Wait, initially sum = sum of
        // all elements (each is its own block minimum). When we merge two blocks with minimums m1 and m2, the new
        // minimum is min(m1,m2). The sum changes from m1+m2 to min(m1,m2). So the reduction is max(m1,m2). So we
        // subtract max(m1,m2). So we are effectively removing the larger of the two minimums from the sum. So we are
        // removing exactly one value per merge, and that value is the larger of the two block minimums being merged.
        // So after k merges, we have removed k values from the initial multiset of block minimums (which is just the
        // original array). But the removed values are not necessarily the original elements; they are the block minimums
        // at the time of merge. However, the block minimums are always some original elements. So we are selecting
        // k original elements to remove from the sum, but with the constraint that we can only remove an element if
        // at the time of removal, it is the larger of two adjacent block minimums. This is equivalent to: we can remove
        // an element if we merge it with a smaller adjacent block minimum. So we can remove any element that has a
        // smaller adjacent original element? Not exactly, because after some removals, the adjacent block minimum might
        // be from a merged block, which is the minimum of several original elements. That minimum is at most the
        // original elements in that block. So it's smaller or equal to the original adjacent element. So if we want to
        // remove a large element, we can do it by merging it with a smaller element. The smaller element could be an
        // original element or a block minimum from previous merges. But the block minimum is always the minimum of its
        // constituent original elements. So it's <= any of them. So if we have a large element, we can always merge it
        // with a smaller neighbor (if one exists) and remove the large element. If both neighbors are larger, then the
        // element is a local minimum, and we cannot remove it unless we first remove the larger neighbors? But if we
        // remove a larger neighbor, we merge it with something else, and the local minimum might become adjacent to
        // something even larger? Actually, if we have a local minimum, we cannot remove it directly. But we could
        // remove one of its neighbors by merging it with the other neighbor? That would make the local minimum adjacent
        // to the other neighbor. If that other neighbor is also larger, we still can't remove the local minimum.
        // So local minima can never be removed. They are "stuck" as block minimums forever. So the elements that can
        // be removed are exactly those that are not local minima? But wait, if we have [5, 3, 4], 3 is a local minimum.
        // We can remove 5 by merging with 3 (reduction 5). Then array becomes [3, 4]. Now 3 is adjacent to 4. 3 is still
        // a local minimum (only one neighbor, but smaller). We cannot remove 3. We can remove 4 by merging with 3
        // (reduction 4). So we removed 5 and 4, total reduction 9. The original sum was 12, final sum 3. The removed
        // elements are 5 and 4. 3 was never removed. So the local minimum 3 survived. In general, the elements that
        // survive are those that are never the larger in any merge. They are the elements that are smaller than all
        // elements that get merged into them. This is exactly the set of elements that are left-to-right minima or
        // right-to-left minima? Actually, if we consider the process of repeatedly merging adjacent elements where
        // the larger is absorbed into the smaller, the final surviving elements are exactly the elements that are
        // smaller than all elements between them and the next surviving element. This is like: we can choose a set of
        // surviving elements such that between any two consecutive surviving elements, all elements are >= both?
        // Not necessarily >= both, but >= the smaller of the two? Let's think: if we have surviving elements at i and j
        // with i < j, and we merge everything between them into either i or j. The merges will eventually make the
        // whole segment between i and j have the value min(a_i, a_j) if we merge appropriately? Actually, we can choose
        // the direction of merges. We can merge all elements between i and j into the smaller of a_i and a_j. So the
        // sum reduction from that segment would be the sum of all elements between i and j minus (j - i - 1) * min(a_i, a_j)?
        // No, if we merge them all into the smaller one, the final value of the whole block is min(a_i, a_j). The
        // reduction is the sum of the original values in the segment minus the final value times the length? Wait,
        // the initial sum of the segment from i to j is a_i + ... + a_j. After merging into one block with value
        // m = min(a_i, a_j), the sum becomes m * (j - i + 1). The reduction is sum - m * length. But we are only
        // allowed k merges. The number of merges to combine a segment of length L into one block is L-1. So if we
        // combine a segment of length L, we use L-1 operations. The reduction is sum of elements in segment minus
        // L * min(segment). But we can also choose to not merge the whole segment, but only parts.
        // This suggests we can think of it as: we can choose up to k operations. Each operation can be seen as
        // "deleting" one element by merging it into a smaller neighbor. The reduction is the value of the deleted
        // element. But we can only delete an element if it has a smaller neighbor at the time of deletion. This is
        // equivalent to: we can delete any element that is not a "record minimum" from some direction? Actually,
        // consider the array. We can always delete the largest element that has a smaller neighbor. We can repeat this.
        // This is exactly the process of: we can delete any element that is not a local minimum. But after deletions,
        // new local minima may appear? No, if we delete a neighbor, a local minimum remains a local minimum or becomes
        // a boundary. It never becomes larger than its new neighbor because the new neighbor was originally separated
        // by the deleted element, and the deleted element was larger than the local minimum? Actually, if we have
        // [5, 3, 4], 3 is local min. Delete 5 -> [3,4]. 3 is still smaller than 4. Delete 4 -> [3]. So 3 survives.
        // If we have [4, 5, 3], 3 is local min. Delete 5 by merging with 3 -> [4,3]. Now 4 and 3. 3 is smaller, so
        // we can delete 4 by merging with 3 -> [3]. So 3 survives. So local minima always survive. What about elements
        // that are not local minima? They can always be deleted by merging with a smaller neighbor. But what if an
        // element is not a local minimum, but both neighbors are also not local minima and we want to delete all of
        // them? We can delete them one by one. The only restriction is that we cannot delete an element if it has no
        // smaller neighbor. But if it's not a local minimum, it has at least one smaller neighbor. So it can be deleted.
        // However, if we delete the smaller neighbor first, then the element might lose its smaller neighbor. So the
        // order matters. To maximize the number of deletions (or the sum of deletions), we should always delete larger
        // elements first, or delete elements that have a smaller neighbor that we don't plan to delete. Since we want
        // to maximize the sum of deleted elements, we should delete the largest possible elements. The only elements
        // we cannot delete are those that are local minima and we choose not to delete their neighbors? Actually, we
        // can delete any element that is not a local minimum, provided we don't delete its smaller neighbor first.
        // But if we want to delete both a local minimum and its neighbor? We can't delete the local minimum because
        // it has no smaller neighbor. So local minima are undeletable. What about elements that are not local minima
        // but become local minima after deletions? For example, [2, 5, 3]. 2 is local min? 2 < 5, so 2 is a local min
        // (left boundary). 5 is not local min (2 < 5). 3 is local min? 3 < 5, and right boundary, so 3 is local min.
        // We can delete 5 by merging with 2 (reduction 5) -> [2,3]. Now 2 and 3 are both local minima? 2 < 3, so 2 is
        // local min, 3 is not? 3 > 2, so 3 has a smaller neighbor (2), so 3 is not a local min. We can delete 3 by
        // merging with 2 -> [2]. So we deleted 5 and 3. Both were not local minima initially? 3 was a local min
        // initially? Initially, 3 had left neighbor 5 and right boundary. Since 3 < 5, it was a local min. But after
        // deleting 5, 3's left neighbor became 2, which is smaller, so 3 was no longer a local min and could be deleted.
        // So an element that is initially a local minimum can become deletable if we delete a larger neighbor and expose
        // a smaller neighbor. So the only elements that can never be deleted are those that are smaller than all elements
        // to their left and right? That is, the global minimum? Actually, in [2,5,3], the global minimum is 2. 3 was
        // deleted. So only the global minimum survived? But wait, if we have [3,1,2], global min is 1. We can delete 3
        // and 2, leaving 1. So only the global minimum survives if k >= 2? But k=1 in sample, we deleted
