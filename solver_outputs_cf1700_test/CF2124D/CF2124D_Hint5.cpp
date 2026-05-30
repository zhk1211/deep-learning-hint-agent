// Hint5
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
        vector<int> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }

        if (k == 1) {
            cout << "YES\n";
            continue;
        }

        // We can always delete any element if k <= n, but we need to check
        // if we can form a palindrome. The key observation: we can delete
        // any element as long as there exists a subarray of length >= k
        // containing it where it is the k-th smallest. Actually, we can
        // delete any element we want if we can find a suitable interval.
        // The only restriction is that we cannot delete elements if n < k.
        // But we can always delete elements from the ends? Let's think.
        // The problem reduces to: can we delete elements to make the array
        // a palindrome? Equivalent to: can we match elements from left and
        // right, deleting mismatched ones, as long as we can delete them.
        // Since we can delete any element (proved by hints), we just need
        // to check if we can match all remaining elements into a palindrome.
        // But wait, we can only delete elements if there is a subarray of
        // length >= k where it is the k-th smallest. Is it always possible
        // to delete any element? Not exactly. Consider k=2, array [1,2].
        // We cannot delete 1 because any subarray of length >=2 containing 1
        // is the whole array, and 1 is the 1st smallest, not 2nd.
        // So we need to be careful.

        // From hints: maintain two pointers l and r. We want to make a[l..r]
        // a palindrome by deleting elements. We can delete a[l] if there exists
        // a subarray of length >= k containing a[l] where it is the k-th smallest.
        // Similarly for a[r]. Actually, we can delete any element if we can find
        // a subarray of length >= k where it is the k-th smallest. When can we
        // not delete an element? If all subarrays of length >= k containing it
        // have it as not the k-th smallest. This happens if the element is too
        // small or too large relative to its neighbors? Let's analyze.

        // Consider we want to delete a[i]. We need a subarray of length >= k
        // containing i, where a[i] is the k-th smallest. This means there are
        // exactly k-1 elements in that subarray that are <= a[i] (with ties
        // broken arbitrarily, but we can choose the subarray to make it work).
        // If we can find such a subarray, we can delete a[i]. The hints suggest
        // a two-pointer approach: we can always delete elements from the ends
        // if we have enough elements? Let's think differently.

        // Actually, the problem is known: we can delete any element if and only if
        // there exists an element equal to it on the other side? No.

        // Let's re-read hints: "Can we erase any element we want?" "How can we erase elements?"
        // "Consider a two-pointer approach." "If we want to erase an element, how can we find a valid interval?"
        // "Maintain two pointers, l and r which represent the left and right boundary of the interval."

        // This suggests we can simulate the process by trying to match from both ends.
        // We can delete a[l] if we can find a valid interval that includes l and has length >= k
        // where a[l] is the k-th smallest. But maybe we can always delete a[l] if there is
        // another element equal to a[l] somewhere? Not necessarily.

        // Let's think about the condition for being able to delete an element x at position i.
        // We need a subarray of length L >= k containing i, such that x is the k-th smallest.
        // This means there are at least k-1 elements in that subarray that are <= x.
        // If we take a subarray that includes i and extends to the right or left, we can
        // include enough elements <= x. The only way we cannot is if there are not enough
        // elements <= x in the whole array? But we can choose the subarray. Actually, if we
        // take a subarray that includes i and has length exactly k, we need exactly k-1 elements
        // <= x in that subarray (excluding x itself? Wait, the k-th smallest: if there are duplicates,
        // we can choose which one is considered the k-th smallest. The problem says: "select an index i
        // such that a_i is the k-th smallest number out of the subarray. If there are multiple possible i,
        // you may select any." So if there are duplicates, we can choose any of them as the k-th smallest.
        // This means if we have a subarray with at least k elements <= x, we can make x the k-th smallest
        // by including enough elements <= x and choosing the appropriate duplicate. Actually, if we have
        // a subarray where the number of elements <= x is >= k, and x is among them, we can always
        // choose x as the k-th smallest by ordering the duplicates appropriately? The k-th smallest
        // is defined by value, and if there are ties, we can pick any index with that value. So if we
        // have at least k elements in the subarray with value <= x, and x is one of them, we can pick
        // x as the k-th smallest if we ensure that there are exactly k-1 elements <= x that are not x?
        // Actually, if there are multiple elements equal to x, we can order them arbitrarily. So if we
        // have a subarray with at least k elements <= x, we can always pick x as the k-th smallest
        // by considering the sorted order: we need the k-th element to be x. This is possible if the
        // number of elements < x is < k, and the number of elements <= x is >= k. So we need:
        // count(< x) < k <= count(<= x). So we need at least k elements <= x, and strictly less than k
        // elements < x. This means x must be the k-th smallest value in the subarray, and there must be
        // enough copies of x or smaller values to reach k. So if we can find a subarray of length >= k
        // containing i where the number of elements < a[i] is < k and number of elements <= a[i] is >= k,
        // we can delete a[i].

        // This seems complicated. Let's look for a simpler characterization. The hints say "two-pointer".
        // Maybe we can always delete elements from the ends if the array is long enough? Consider k=2.
        // We can delete an element if there is a subarray of length >=2 where it is the 2nd smallest.
        // For the first element a[1], we can take subarray [1,2]. a[1] is the 2nd smallest if a[1] >= a[2].
        // If a[1] < a[2], then a[1] is the 1st smallest, so we cannot delete it using [1,2]. But we could
        // use a longer subarray? If we take [1,3], we need a[1] to be the 2nd smallest. That means there
        // must be exactly one element < a[1] in that subarray. If a[1] is the minimum, we cannot delete it
        // unless there are duplicates? If a[1] is the minimum and there is another element equal to it,
        // then we can have count(< a[1]) = 0, count(<= a[1]) >= 2, so we need k=2, so count(<=) >=2, which
        // is true if there is another element equal to a[1]. So we can delete the minimum if there is a duplicate.
        // If a[1] is strictly the unique minimum, we cannot delete it because any subarray containing it
        // will have count(< a[1]) = 0, and we need count(<= a[1]) >= k. But count(<= a[1]) = 1 (only itself),
        // so we need k=1, but k>=2. So unique minimum cannot be deleted if k>=2.

        // This suggests that an element can be deleted if there exists another element equal to it,
        // or if it is not the minimum? Let's test: k=2, array [2,1]. Can we delete 2? Subarray [1,2]:
        // 2 is the 2nd smallest (since 1<2). So yes. Can we delete 1? Subarray [1,2]: 1 is the 1st smallest,
        // so no. But we could take a longer subarray? Only two elements. So 1 cannot be deleted. So unique
        // minimum cannot be deleted. What about [1,2,1]? k=2. Can we delete the first 1? Subarray [1,2]:
        // 1 is 1st smallest. Subarray [1,2,1]: elements are 1,1,2. Sorted: 1,1,2. The 2nd smallest is 1.
        // We can choose the first 1 as the 2nd smallest? Yes, because there are two 1s, we can pick either
        // as the 2nd smallest. So we can delete the first 1. So if there is a duplicate, we can delete it.
        // What about [1,3,2]? k=2. Delete 1? Subarray [1,3]: 1 is 1st. [1,3,2]: sorted 1,2,3. 2nd smallest is 2.
        // 1 is 1st. So cannot delete 1. So unique minimum cannot be deleted.

        // In general, for any k, an element x can be deleted if there exists a subarray of length >= k
        // containing x, where the number of elements < x is < k, and number of elements <= x is >= k.
        // This is equivalent to: there are at least k elements <= x in the subarray, and at most k-1 elements < x.
        // So we need to find a subarray containing x that has at least k elements <= x, but not too many smaller ones.
        // This is always possible if there are at least k elements <= x in the whole array? Not necessarily,
        // because the subarray must contain x and have length >= k. But we can always take a subarray that
        // includes x and enough elements <= x from the rest of the array, as long as the total number of
        // elements <= x is >= k. However, we also need the number of elements < x to be < k. If there are
        // >= k elements strictly smaller than x, then any subarray containing x and enough elements <= x
        // will also contain those smaller elements, making count(< x) >= k, so x cannot be the k-th smallest.
        // So x can be deleted iff the total number of elements < x in the whole array is < k, and the total
        // number of elements <= x is >= k? But we can choose the subarray to exclude some smaller elements.
        // We don't have to include all smaller elements. We just need a subarray of length >= k containing x
        // where count(< x) < k and count(<= x) >= k. We can choose a subarray that avoids some smaller elements
        // if they are far away. So the condition is local: there exists a subarray of length >= k containing x
        // with at most k-1 elements < x and at least k elements <= x. This is equivalent to: there exists a
        // subarray of length >= k containing x where the number of elements > x is at most (length - k).
        // Not sure.

        // Let's think about the two-pointer approach from the hints. We want to make the array a palindrome
        // by deleting elements. We can use two pointers l=1, r=n. While l < r:
        // If a[l] == a[r], we can keep them and move l++, r--.
        // If a[l] != a[r], we need to delete one of them. We can delete a[l] if possible, or delete a[r] if possible.
        // If we can delete either, we can proceed. If we cannot delete either, then it's impossible.
        // So the problem reduces to: given the current array, can we delete the current leftmost or rightmost element?
        // And we can repeat this. But the array changes as we delete. However, the hints say "maintain two pointers
        // l and r which represent the left and right boundary of the interval." This suggests we don't actually
        // delete, but we just move pointers and check if we can "skip" elements.

        // Maybe we can always delete a[l] if there is another element equal to a[l] somewhere to the right?
        // Let's test: k=2, a=[1,2,3]. l=1, r=3. a[1]=1, a[3]=3. Can we delete 1? We need a subarray of length >=2
        // containing index 1 where 1 is the 2nd smallest. The only subarrays containing index 1 are [1,2] and [1,2,3].
        // In [1,2], 1 is 1st smallest. In [1,2,3], 1 is 1st smallest. So cannot delete 1. Can we delete 3?
        // Subarrays containing index 3: [2,3] and [1,2,3]. In [2,3], 3 is 2nd smallest (2<3). So we can delete 3!
        // So we can delete the rightmost element if it is not the minimum? Actually, 3 is the maximum, so it's easy.
        // What about [3,2,1]? k=2. l=1, r=3. a[1]=3, a[3]=1. Can we delete 3? Subarray [3,2]: 3 is 2nd smallest (2<3),
        // so yes. Can we delete 1? Subarray [2,1]: 1 is 1st smallest. [3,2,1]: 1 is 1st. So cannot delete 1.
        // So we can delete the leftmost if it is not the unique minimum? In [3,2,1], 3 is not the minimum, so we can delete it.
        // What if both are the unique minimum? [1,2,1]? But 1 is not unique. [1,2,3]? left is 1 (unique min), right is 3 (not min).
        // So it seems we can always delete an element that is not the unique minimum of the current array?
        // Let's test: k=3, a=[1,2,3,4]. Can we delete 1? Need subarray of length >=3 where 1 is 3rd smallest.
        // Subarray [1,2,3]: sorted 1,2,3. 3rd smallest is 3. 1 is 1st. [1,2,3,4]: sorted 1,2,3,4. 3rd is 3. So no.
        // Can we delete 4? Subarray [2,3,4]: sorted 2,3,4. 3rd is 4. Yes! So we can delete the maximum if there are
        // at least k elements? Actually, to delete an element x, we need a subarray where x is the k-th smallest.
        // If x is the maximum, we can take a subarray ending at x with length k, then x will be the k-th smallest
        // (since all others are smaller). So any element that is the maximum in some subarray of length k can be deleted.
        // In particular, if the array length >= k, the rightmost element can be deleted if we take the subarray
        // of the last k elements, and the rightmost is the maximum among them? Not necessarily the maximum, but
        // if it is the k-th smallest, which means there are exactly k-1 elements <= it in that subarray. If we take
        // the last k elements, the rightmost element is the k-th smallest if and only if there are exactly k-1 elements
        // <= it in that subarray. That is true if it is the maximum of those k elements? Actually, if it is the maximum,
        // then all other k-1 elements are <= it, so it is the k-th smallest. If it is not the maximum, say there is an
        // element larger than it in the last k, then that larger element would be the k-th smallest? Wait, sorted order:
        // the k-th smallest is the largest among the k smallest. If the rightmost is not the maximum, then there is some
        // element larger than it, so the rightmost would be at most the (k-1)-th smallest. So to be the k-th smallest,
        // it must be the maximum of the chosen subarray. So we can delete an element if we can find a subarray of length
        // >= k where it is the maximum. Similarly, we can delete an element if we can find a subarray where it is the minimum?
        // No, to be the k-th smallest, it cannot be the minimum unless k=1. So we need it to be the maximum of some subarray
        // of length >= k? Actually, if it is the maximum of a subarray of length L >= k, then in that subarray, there are
        // L-1 elements <= it. We need it to be the k-th smallest, so we need exactly k-1 elements <= it. That means we need
        // L-1 = k-1 => L = k. So we need a subarray of length exactly k where it is the maximum? Not exactly: if there are
        // duplicates, we can have more than k-1 elements <= it, but we can still choose it as the k-th smallest if we
        // order duplicates appropriately. The condition is: there exists a subarray of length >= k containing x, such that
        // the number of elements < x is < k, and the number of elements <= x is >= k. If x is the maximum of the subarray,
        // then all other elements are <= x, so count(<= x) = length >= k. We also need count(< x) < k. If there are at least
        // k elements strictly less than x, then count(< x) >= k, so x cannot be the k-th smallest. So x can be deleted if
        // there exists a subarray of length >= k containing x, where x is the maximum, and there are at most k-1 elements
        // strictly less than x in that subarray. But if x is the maximum, then all other elements are <= x, so count(< x)
        // is the number of elements strictly less than x. We need that number to be < k. Since the subarray has length >= k,
        // if x is the maximum, then there are at least k-1 other elements. If all of them are strictly less than x, then
        // count(< x) >= k-1. It could be exactly k-1 if length = k and all others are strictly less. Then count(< x) = k-1 < k,
        // and count(<= x) = k >= k. So x can be deleted. If length > k, then count(< x) >= k, so x cannot be deleted.
        // Therefore, x can be deleted if there exists a subarray of length exactly k where x is the strict maximum?
        // What if there are duplicates of x? If there are duplicates, then some elements are equal to x. Then count(< x)
        // could be less than k-1, and count(<= x) >= k. For example, k=2, subarray [x, x]. x is the maximum (not strict).
        // count(< x) = 0 < 2, count(<= x) = 2 >= 2. So x can be deleted. So we need a subarray of length >= k containing x,
        // where the number of elements strictly less than x is <= k-1, and the number of elements <= x is >= k.
        // This is equivalent to: there exists a subarray of length >= k containing x, where x is not the unique minimum
        // of a subarray with too many smaller elements? Actually, it's easier: we can always delete an element if there
        // exists a subarray of length k where it is the maximum (allowing ties)? Let's check: if we have a subarray of
        // length k where x is the maximum (i.e., no element is strictly greater than x), then all elements are <= x.
        // So count(<= x) = k >= k. count(< x) is the number of elements strictly less than x. Since there are k elements
        // total, count(< x) <= k-1 (if x is the maximum, there can be at most k-1 elements strictly less, because at least
        // one element is x itself). So count(< x) <= k-1 < k. So the condition is satisfied! Therefore, if there exists
        // a subarray of length exactly k containing x, where x is the maximum (i.e., no element strictly greater than x),
        // then x can be deleted. Is the converse true? If x can be deleted, does there necessarily exist a subarray of
        // length k where x is the maximum? Suppose there is a subarray of length > k where x is the k-th smallest.
        // Then count(< x) < k and count(<= x) >= k. We can possibly shrink the subarray to length k by removing some
        // elements that are > x? But if we remove elements > x, x remains the k-th smallest? Actually, if we remove
        // elements > x, the total number of elements decreases, and x's rank might change. But we can always find a
        // subarray of length exactly k where x is the maximum? Not necessarily. Consider k=3, array [1,2,4,3], x=3.
        // Can we delete 3? Subarray [1,2,4,3]: sorted 1,2,3,4. 3rd smallest is 3. So yes. Is there a subarray of length 3
        // where 3 is the maximum? Subarrays containing 3 of length 3: [2,4,3] -> max is 4, so 3 is not max. [1,2,4,3] length 4.
        // What about [1,2,3]? Not contiguous. So no subarray of length 3 where 3 is max. But we deleted 3 using length 4.
        // So the condition "subarray of length k where x is max" is sufficient but not necessary.
        // However, maybe we can always reduce to that? Let's see if we can still delete 3 in [1,2,4,3] with k=3.
        // We used the whole array. But after deleting 3, we get [1,2,4]. That's not a palindrome. But the question is
        // whether we can eventually get a palindrome. So we don't need to delete 3 necessarily; we might delete other things.

        // Let's think about the two-pointer simulation. We want to match a[l] and a[r]. If they are equal, we keep them.
        // If not, we must delete one of them. We can delete a[l] if there exists a subarray of length >= k containing l
        // where a[l] is the k-th smallest. Since l is the leftmost, any subarray containing l will start at l or earlier.
        // But earlier elements have been deleted? In our two-pointer simulation, we are considering the current array
        // after some deletions. The pointers l and r represent the current boundaries. So the current array is a[l..r].
        // We can delete a[l] if there exists a subarray within a[l..r] of length >= k containing l, where a[l] is the
        // k-th smallest. Since l is the left boundary, the subarray must start at l. So we need a subarray a[l..i]
        // with i-l+1 >= k, such that a[l] is the k-th smallest in a[l..i]. Similarly for a[r], we need a subarray
        // a[j..r] with r-j+1 >= k, where a[r] is the k-th smallest.

        // This is exactly the two-pointer approach: we can delete a[l] if there exists an index i >= l+k-1 such that
        // a[l] is the k-th smallest in a[l..i]. Since a[l] is the first element, being the k-th smallest means there
        // are exactly k-1 elements in a[l..i] that are <= a[l] (and we can choose a[l] as the k-th if there are ties).
        // Actually, we need count(<= a[l]) >= k and count(< a[l]) < k in that prefix. Since a[l] is the first element,
        // we can just scan to the right until we have seen k elements <= a[l]. If we can reach such an index i where
        // the number of elements <= a[l] is exactly k (or more, but we can stop at exactly k), and the number of elements
        // strictly less than a[l] is < k, then we can delete a[l]. But since we are including a[l] itself, we need to
        // find k-1 other elements <= a[l]. If we find them, then we have at least k elements <= a[l]. What about
        // elements strictly less? If among those k elements, some are strictly less, that's fine as long as the total
        // number of strictly less elements in the subarray is < k. Since the subarray has length >= k, if we take exactly
        // the prefix up to the point where we have seen k elements <= a[l], the length might be > k. In that prefix,
        // there could be many elements strictly less than a[l]. But we only care about the condition: we need a subarray
        // where count(< a[l]) < k. If we take the minimal prefix that contains k elements <= a[l], then the number of
        // elements strictly less than a[l] could be anything. If it's >= k, then a[l] cannot be the k-th smallest in that
        // prefix. But we could take a longer prefix? That would only increase count(< a[l]). So if the minimal prefix
        // already has >= k elements strictly less than a[l], then no prefix can work, because any prefix containing l
        // and having >= k elements <= a[l] will also contain those strictly less elements. So a[l] can be deleted if and
        // only if in the minimal prefix that contains k elements <= a[l], the number of elements strictly less than a[l]
        // is < k. But wait, we could also choose a subarray that doesn't start at l? No, l is the left boundary, so any
        // subarray containing l must start at l or earlier. Since we have deleted everything left of l, it must start at l.
        // So the condition for deleting a[l] is: there exists a prefix of the current array (starting at l) of length >= k,
        // such that a[l] is the k-th smallest. This is equivalent to: let c be the number of elements in the current array
        // that are <= a[l]. If c < k, we can never have k elements <= a[l], so cannot delete. If c >= k, consider the
        // position of the k-th element <= a[l] (including a[l] itself). Let that position be p. Then in the prefix l..p,
        // we have exactly k elements <= a[l]. The number of elements strictly less than a[l] in this prefix is some number s.
        // If s < k, then a[l] can be the k-th smallest (since we have k elements <= a[l], and we can choose a[l] as the
        // k-th if there are ties; actually we need count(< a[l]) < k, which is s < k, and count(<= a[l]) = k >= k, so yes).
        // If s >= k, then even in this minimal prefix, we already have >= k elements strictly less than a[l], so a[l]
        // cannot be the k-th smallest. Could a longer prefix work? No, because s would only increase. So a[l] can be deleted
        // iff s < k, where s is the number of elements strictly less than a[l] in the prefix up to the k-th element <= a[l].
        // But note: if there are duplicates of a[l], we can choose which one is the k-th smallest. The k-th element <= a[l]
        // might be a duplicate of a[l]. In that case, s is the number of elements strictly less than a[l] in that prefix.
        // If s < k, we can choose a[l] as the k-th smallest. So condition: let pos be the index of the k-th element in the
        // current array that is <= a[l] (1-indexed from l). If such pos exists and the number of elements < a[l] in a[l..pos]
        // is < k, then we can delete a[l]. Otherwise, we cannot.

        // Similarly for a[r]: we need a suffix ending at r of length >= k, where a[r] is the k-th smallest. By symmetry,
        // we can consider the reversed array and apply the same condition.

        // So the two-pointer algorithm: 
        // l = 0, r = n-1 (0-indexed).
        // While l < r:
        //   if a[l] == a[r]: l++, r--
        //   else:
        //     check if we can delete a[l] from the current subarray a[l..r].
        //     check if we can delete a[r] from the current subarray a[l..r].
        //     if can delete a[l]: l++
        //     else if can delete a[r]: r--
        //     else: return NO
        // If loop finishes, return YES.

        // But wait, we might need to delete multiple elements from one side? The loop handles that by repeatedly checking.
        // However, the condition for deleting a[l] depends on the current subarray. As we move l and r, the subarray shrinks.
        // We need to efficiently check the condition for the current boundaries.

        // Let's formalize the condition for a[l] in a[l..r]:
        // We need to find the k-th element in a[l..r] that is <= a[l]. Let's count how many elements <= a[l] are in a[l..r].
        // If count < k, cannot delete.
        // Otherwise, find the index p (l <= p <= r) such that the number of elements <= a[l] in a[l..p] is exactly k.
        // Then let s = number of elements < a[l] in a[l..p].
        // If s < k, we can delete a[l]; else cannot.

        // Similarly for a[r]: we need to find the k-th element from the right that is <= a[r].
        // Let count = number of elements <= a[r] in a[l..r]. If < k, cannot.
        // Find index p (l <= p <= r) such that number of elements <= a[r] in a[p..r] is exactly k.
        // Let s = number of elements < a[r] in a[p..r].
        // If s < k, can delete a[r]; else cannot.

        // We need to do this efficiently as l and r move. Since n sum <= 2e5, we can afford O(n) per test case if we are careful.
        // But we might need to recompute counts frequently. However, we can just simulate with two pointers and use data structures?
        // Actually, we can precompute something? The array elements are up to n. We can use a Fenwick tree or segment tree to
        // query counts of <= x in a range, and also find the k-th element <= x. But we need to do this while l and r move.
        // Since we only move l and r inwards, we are effectively removing elements from the left and right. We can maintain
        // a data structure that supports: remove leftmost, remove rightmost, query number of elements <= x in current range,
        // and find the position of the k-th element <= x from left or right. This is doable with a balanced BST or Fenwick tree
        // with coordinate compression? But a_i <= n, so we can use a Fenwick tree over values? No, we need positions.
        // We need to find the k-th element <= x in the current subarray. That's a range query on a dynamic array.
        // Since we only remove from ends, we can use a deque and just scan? But scanning could be O(n) per step, leading to O(n^2).
        // We need a faster way.

        // Alternative insight: The condition for deleting a[l] might be equivalent to: there exists an element equal to a[l]
        // to the right within distance such that...? Let's think simpler.
        // From the hints: "Can we erase any element we want?" "How can we erase elements?" "Consider a two-pointer approach."
        // "If we want to erase an element, how can we find a valid interval?" "Maintain two pointers, l and r which represent
        // the left and right boundary of the interval."
        // This suggests that we don't need complex data structures; there is a greedy observation.

        // Let's re-read the problem note: In the second test case: [1,1,2,1] with k=1. They deleted the first two 1s? Actually k=1,
        // so any element can be deleted because any subarray of length >=1 works, and the 1st smallest is the element itself.
        // So k=1 is always YES.
        // Third test case: [2,3,4,5,3,2] k=6. They deleted the whole array? No, they did one operation: selected l=1,r=6,
        // and deleted the 6th smallest? Wait, k=6, subarray length 6, the 6th smallest is the maximum, which is 5. They deleted 5,
        // getting [2,3,4,3,2] which is a palindrome. So they deleted the maximum.
        // Fourth test case: [5,2,4,3,1] k=4. Output NO.
        // Fifth: [4,7,1,2,3,1,3,4] k=5. Output NO.
        // Sixth: [1,2,1,2,2] k=4. Output YES.
        // Seventh: [1,2,2] k=3. Output NO.
        // Eighth: [2,1,2,2] k=4. Output YES.

        // Let's analyze the sixth: n=5, k=4, a=[1,2,1,2,2]. Output YES. How? We need to form a palindrome.
        // Maybe we can delete one element to get [1,2,1,2]? Not palindrome. [1,2,2,2]? Not. [2,1,2,2]? Not.
        // Maybe delete two elements? Let's see: we can delete an element if there's a subarray of length >=4 where it is the 4th smallest.
        // The 4th smallest in a subarray of length 4 is the maximum of that subarray. So we can delete an element if it is the maximum
        // of some subarray of length 4. In [1,2,1,2,2], the subarray of length 4: [1,2,1,2] max=2, so we can delete a 2.
        // [2,1,2,2] max=2, delete a 2. So we can delete any 2 that is in a window of length 4 where it is the max.
        // The first 2 is at index 2 (1-indexed). Window [1,2,1,2] includes it, max is 2, so we can delete it. After deleting,
        // we get [1,1
