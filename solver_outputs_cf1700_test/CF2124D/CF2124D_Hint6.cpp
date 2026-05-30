// Hint6
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

        if (k == n) {
            bool pal = true;
            for (int i = 0; i < n / 2; ++i) {
                if (a[i] != a[n - 1 - i]) {
                    pal = false;
                    break;
                }
            }
            cout << (pal ? "YES" : "NO") << "\n";
            continue;
        }

        // For k >= 2, we can delete any element as long as there are at least k elements.
        // We can always reduce the array to a palindrome if we can match pairs from the ends.
        // The only restriction: we cannot delete elements if the remaining array size is less than k.
        // We can simulate matching from both ends using two pointers.
        int l = 0, r = n - 1;
        while (l < r && a[l] == a[r]) {
            ++l;
            --r;
        }
        // Now a[l] != a[r] (or l >= r)
        if (l >= r) {
            cout << "YES\n";
            continue;
        }
        // We need to delete some elements to make the remaining array a palindrome.
        // We can delete elements from the middle part.
        // The middle part is a[l..r]. We can delete any element from it as long as the total remaining size >= k.
        // But we can also delete elements from the ends if needed.
        // Actually, we can always delete elements from the middle to match the ends.
        // We can try to match a[l] with some a[j] in the middle, or a[r] with some a[i] in the middle.
        // Since we can delete any element (with the condition that we have at least k elements to perform an operation),
        // we can always delete the mismatched elements as long as we have enough elements.
        // The only case when it's impossible is when we cannot perform any operation because the array size is less than k,
        // and the array is not a palindrome.
        // But we can also delete elements from the ends if we have enough elements.
        // Let's think: we can always reduce the array to a palindrome if we can delete all mismatched elements.
        // The only obstacle is if we get stuck with an array of size < k that is not a palindrome.
        // We can simulate the process greedily: try to match from the ends, and if we can't, we need to delete one of the mismatched elements.
        // We can delete either the left mismatched element or the right one, but we need at least k elements in the current array to do so.
        // So we can try both possibilities: delete left or delete right, and see if we can eventually match.
        // Since n is up to 2e5, we can do a linear scan with two pointers and a counter of available elements.
        // Actually, we can think of it as: we can delete any element as long as the current array size >= k.
        // So we can always delete elements until the array size becomes k-1, but then we can't delete anymore.
        // So the final array must be a palindrome of size <= k-1, or we can reduce it to empty (size 0).
        // But we can also stop earlier if we get a palindrome.
        // So the question reduces to: can we obtain a palindrome by deleting some elements, with the constraint that we never need to delete when size < k?
        // We can always delete elements from the middle to match the ends, as long as we have >= k elements.
        // The only problematic case is when we have a mismatch and the current array size is < k, so we can't delete.
        // So we need to check if we can avoid getting into a state where size < k and not palindrome.
        // We can simulate matching from both ends. When we encounter a mismatch, we have two choices: delete the left element or the right element.
        // We can try both choices recursively, but that would be O(2^n). However, we can observe that we only need to delete one of them, and then continue.
        // Actually, we can think of it as: we can delete any prefix or suffix of the middle part as long as we have enough elements.
        // Let's formalize: we have two pointers l and r. Initially l=0, r=n-1.
        // While l < r and a[l] == a[r], we move l++, r--.
        // If l >= r, palindrome achieved.
        // Otherwise, we have a mismatch. We need to delete either a[l] or a[r] (or some elements in between) to try to match.
        // But we can only delete if the current subarray length (r - l + 1) >= k.
        // If it's < k, we cannot delete anything, so it's impossible.
        // If it's >= k, we can delete a[l] and then try to match the new l+1 with r, or delete a[r] and try to match l with r-1.
        // We can try both possibilities. Since we only go forward, we can do this in O(n) by trying both paths.
        // We can write a function that simulates the process with a given starting l, r, and returns if we can achieve palindrome.
        // But we can also observe that we can always delete the left element if we have >= k elements, and then continue. If that fails, we try deleting the right element.
        // However, we might need to delete multiple elements from one side. So we can just try to match by skipping elements on one side.
        // Actually, we can think of it as: we can delete any number of elements from the left or right as long as the total deleted elements don't reduce the array below k at the moment of deletion.
        // But we can delete one by one. So we can just try to match a[l] with some element on the right, or a[r] with some element on the left.
        // We can do a two-pointer approach where we try to find a matching pair by moving one pointer.
        // Let's try to match a[l] with some a[j] where j > l. We can delete all elements between l and j? Not exactly, we delete elements one by one.
        // But we can delete a[l] itself if we want to skip it. So we can just move l forward until we find a match for a[r], or move r backward until we find a match for a[l].
        // However, we must ensure that at each deletion step, the current array size is >= k.
        // The current array size is the number of elements remaining. Initially n. When we delete an element, size decreases by 1.
        // So we can delete as long as size >= k.
        // So we can simulate: we have l and r. We want to make a[l] == a[r]. If they are equal, we move both.
        // If not, we can try to delete a[l] (if size >= k) and then l++, or delete a[r] and r--.
        // We can try both paths. Since the total number of deletions is at most n, and we only branch once (we can try one path, if it fails try the other), we can do it in O(n).
        // Let's implement a function can(l, r, size) but we don't need size explicitly because size = r - l + 1.
        // We'll try to match greedily: first try to delete left until we find a match for right, or delete right until we find a match for left.
        // But we must check the size condition at each deletion.
        // Actually, we can just try to match by moving one pointer and checking if the remaining size is >= k for each deletion.
        // Since we only need to check if it's possible, we can do a linear scan with two pointers and a flag for which side we are deleting.
        // Let's think simpler: we can always delete any element as long as the current length >= k.
        // So we can delete elements from the middle to make the ends match.
        // The only thing that can stop us is if we have a mismatch and the current length is < k.
        // So we can just try to match from the ends, and if we hit a mismatch, we need to delete one of the mismatched elements.
        // We can try both possibilities: delete left or delete right. We can do this recursively with memoization? But n is 2e5, so we need O(n).
        // Notice that the decision is only needed when we have a mismatch. We can try both paths, but they are independent and we can just run two simulations.
        // We can write a function that takes initial l, r and simulates the process, returning true if palindrome is achieved.
        // In the simulation, while l < r:
        //   if a[l] == a[r]: l++, r--
        //   else:
        //       if (r - l + 1 < k) return false;
        //       // try deleting left
        //       if (simulate(l+1, r)) return true;
        //       // try deleting right
        //       if (simulate(l, r-1)) return true;
        //       return false;
        // This would be exponential if we branch multiple times. But we only branch at the first mismatch? Actually, we might branch multiple times.
        // However, we can observe that we only need to try deleting from one side until we find a match, and we don't need to branch again because after we start deleting from one side, we can continue deleting from that side until we match or fail.
        // Actually, we can think of it as: we can delete a prefix of the left side or a suffix of the right side.
        // We can try to find the first index i > l such that a[i] == a[r], and we can delete all elements from l to i-1, provided that at each step the size was >= k.
        // But we delete one by one, so the size condition is just that the initial size before any deletion is >= k, and then we can delete as many as we want as long as after each deletion the size is still >= k? Wait, the operation requires that the subarray we select has length >= k. We can select the whole current array as the subarray, and then delete the k-th smallest. So we can delete any element as long as the current array length >= k. So we can delete multiple elements, but each deletion requires the current array length >= k. So if we start with length L >= k, we can delete one element, length becomes L-1. We can continue deleting as long as length >= k. So we can delete down to length k-1. So if we want to delete a block of elements, we just need that the length before the first deletion is >= k, and we can delete until length becomes k-1. But we might not need to delete that many.
        // So in our simulation, when we decide to delete left elements to match a[r], we can delete a[l], then check if new l matches r. If not, we can delete again, as long as current length >= k.
        // So we can just loop: while l < r and a[l] != a[r] and (r - l + 1) >= k: we can delete a[l] (l++). Then if a[l] == a[r], we continue matching. If we exit because length < k, then it's impossible.
        // Similarly for deleting right.
        // So we can try both strategies: try to delete left until match or fail, and try to delete right until match or fail.
        // If either succeeds, return true.
        // But wait, what if we need to delete some from left and some from right? That would be covered by trying both? Actually, if we delete from left and then later we have another mismatch, we might need to delete from right. But our simulation of "delete left until match" only deletes left at the first mismatch. After we match, we continue and might encounter another mismatch. At that point, we can again choose to delete left or right. So we need to allow branching at each mismatch.
        // However, we can observe that we can always choose to delete from the side that has the mismatched element. But we might need to delete from both sides. But the order doesn't matter because deletions are independent. We can think of it as: we want to find a subsequence (not necessarily contiguous) that is a palindrome, and we delete the rest. The condition is that we can delete the rest as long as at the moment of each deletion, the current array size is >= k. This is equivalent to: we can delete any set of elements as long as we never delete when the number of remaining elements is < k. That means we can delete elements as long as the number of remaining elements is at least k. So we can delete elements until only k-1 elements remain. So the final palindrome can be of any size <= k-1, or we can stop earlier if we get a palindrome of size >= k. But if we get a palindrome of size >= k, we can also continue deleting to make it smaller? No, we want to achieve a palindrome, we can stop as soon as we have a palindrome.
        // So the problem reduces to: can we select a palindrome subsequence (by deleting elements) such that we never need to delete when the current array size is < k?
        // This is equivalent to: can we delete elements to form a palindrome, with the constraint that we can only delete when the current size >= k.
        // We can always delete from the ends first. The only issue is if we have a mismatch and the current size is < k, we are stuck.
        // So we can simulate the process with two pointers, and when we hit a mismatch, we have two choices: delete left or delete right. We can try both paths. Since the total number of deletions is at most n, and we only branch when we have a mismatch, the number of branches might be large in worst case? Consider alternating mismatches: we might branch at each step, leading to 2^(n/k) which is too much.
        // But we can use the fact that we only need to try deleting from one side continuously until we match or fail. Actually, we can think of it as: we want to find the longest matching prefix and suffix. The elements that are not matched must be deleted. We can delete them from the left or right. The only constraint is that we cannot delete if the current size is < k. So we can just try to match greedily from the outside in, and if we hit a mismatch, we can try to skip the left element (if size >= k) or skip the right element (if size >= k). We can try both possibilities using a recursive function with memoization on (l, r). But n is 2e5, so memoization would be O(n^2) in worst case? No, because l and r only move inward, the number of states is O(n). Actually, if we use memoization, the number of distinct (l, r) pairs we visit is at most O(n) because l increases and r decreases. But we might visit the same (l, r) from different paths? Since we only move inward, the total number of states is O(n^2) in worst case if we branch a lot? Wait, l and r are indices. If we branch at each mismatch, we could have many different combinations of l and r. For example, we could delete some from left, then some from right, etc. The number of possible (l, r) pairs is O(n^2). But we only care about pairs that are reachable. In the worst case, we might have a sequence like 1 2 1 2 1 2... and k=2. We might have many ways to delete. But we can observe that we only need to try two possibilities at the first mismatch, and then we can continue greedily? Let's think deeper.

        // Alternative approach: We can always delete any element as long as size >= k. So we can delete elements until size = k-1. If the remaining array of size k-1 is a palindrome, then YES. If not, can we get a palindrome of size >= k? We can also stop earlier if we get a palindrome of size >= k. So the question is: can we delete elements to get a palindrome, with the only restriction that we can't delete when size < k. This means we can delete any elements as long as we don't reduce the size below k during the deletion process. But we can reduce the size below k only if we stop deleting. So we can delete down to size k-1, and then we must have a palindrome. So if there exists a palindrome of size <= k-1 that can be obtained by deleting elements from the original array (preserving order), and we can delete the other elements while always having size >= k during deletions, then YES. But wait, the order of deletions matters: we must be able to delete the other elements without ever having size < k when we perform a deletion. If we want to end up with a palindrome of size m <= k-1, we need to delete n - m elements. We can delete them in any order, but each deletion requires the current size >= k. So we can delete elements as long as the current size >= k. So we can delete until size = k-1. So if m <= k-1, we can just delete elements until size = k-1, and if that final array is a palindrome, we win. But we might not be able to choose which elements remain if we are forced to delete in a certain order? Actually, we can choose the order of deletions. We can always delete from the ends first. So we can delete any elements we want as long as we never need to delete when size < k. So we can just delete elements from the ends until we get a palindrome of size >= k, or until size = k-1 and check if it's a palindrome. But we can also delete from the middle? The operation allows selecting any subarray of length >= k and deleting the k-th smallest. That means we can delete any element as long as there are at least k elements in the whole array. So we can delete any element, not just from the ends. So we have full control over which elements to delete, as long as we have >= k elements at the moment of deletion. So we can delete any set of elements, provided we delete them one by one, and we never attempt to delete when size < k. So we can delete down to size k-1, and we can choose which k-1 elements remain, as long as we can delete the others in some order. But can we always delete any element we want? The operation requires selecting a subarray of length >= k and deleting the k-th smallest. If we want to delete a specific element, we can always find a subarray of length >= k containing it, and then we need it to be the k-th smallest. Is it always possible to make any element the k-th smallest? Not necessarily. For example, if k=2 and we have [1, 100], we can't delete 1 because in any subarray of length >=2 containing 1, 1 will be the 1st smallest, not the 2nd. So we can only delete the k-th smallest. So we cannot delete arbitrary elements! This is the key. The hints say: "Can we erase any element we want?" The answer is NO. We can only erase the k-th smallest element of a subarray of length >= k. So we have restrictions on which elements we can delete.

        // So we need to analyze which elements can be deleted. The operation: choose l, r with r-l+1 >= k, find the k-th smallest in that subarray, delete it. So we can delete an element if there exists a subarray of length >= k containing it, where it is the k-th smallest. That means there are at least k-1 elements in that subarray that are <= it? Actually, k-th smallest means there are exactly k-1 elements strictly smaller? Or smaller or equal? The problem says: "a_i is the k-th smallest number out of the subarray". If there are duplicates, the k-th smallest is defined by order. For example, [1,2,2,1,3] with k=3, the 3rd smallest is 2 (the first 2 or second 2). So if we have duplicates, we can choose any of the duplicates that are the k-th smallest. So to delete an element x, we need a subarray of length >= k where x is the k-th smallest. That means there are at least k-1 elements in that subarray that are <= x, and at most (length - k) elements that are >= x? Actually, if x is the k-th smallest, then there are exactly k-1 elements that are strictly less than x, or if there are duplicates, the k-th smallest could be one of the duplicates. More precisely, if we sort the subarray, the element at index k (1-based) is the k-th smallest. So x must be >= the k-th smallest? Actually, x is exactly the element at position k in sorted order. So there are k-1 elements <= x? Not exactly, because if there are duplicates, the k-th smallest could be x, and there could be more than k-1 elements <= x. But the definition: "a_i is the k-th smallest number" means if you sort the subarray, the element at the k-th position is a_i. If there are multiple equal to a_i, any of them could be the k-th position depending on the sorting. So we can choose i such that a_i is one of the elements that can be the k-th smallest. So we can delete an element if we can find a subarray of length >= k where that element can be considered the k-th smallest. This is possible if there are at least k-1 elements in the subarray that are <= it, and at least (length - k) elements that are >= it? Actually, if we want a specific element to be the k-th smallest, we need that in the subarray, the number of elements strictly less than it is < k, and the number of elements less than or equal to it is >= k. So we need at least k elements <= it, and at most k-1 elements strictly less than it. So if we have an element x, we can delete it if we can find a subarray of length >= k containing x, such that the number of elements in the subarray that are < x is <= k-1, and the number of elements <= x is >= k. This is always possible if we can include enough elements <= x and not too many < x. But we can also include elements > x to increase the length.

        // This seems complicated. Let's look at the hints. Hint 1: "Can we erase any element we want?" Hint 2: "How can we erase elements?" Hint 3: "Consider a two-pointer approach." Hint 4: "If we want to erase an element, how can we find a valid interval where we can erase the element?" Hint 5: "Maintain two pointers, l and r which represent the left and right boundary of the interval." Hint 6: "What is the complexity of finding the k-th smallest element in an array?"

        // The hints suggest that we can erase elements by using a two-pointer approach to find a valid interval. Maybe we can always erase elements from the ends? Or maybe we can only erase elements that are not the minimum or maximum? Let's think about the operation: we select a subarray of length >= k, and delete the k-th smallest. If k=1, we delete the smallest element. So we can delete the minimum element of any subarray of length >=1. That means we can delete any element that is the minimum of some subarray? Actually, if k=1, we can delete the 1st smallest, i.e., the minimum. So we can delete any element that is the minimum of a subarray of length >=1. But we can choose the subarray to be just that element? No, length must be >= k = 1, so we can choose l=r=i, then the subarray is [a_i], the 1st smallest is a_i, so we can delete any element! So for k=1, we can delete any element. That matches the sample: second test case k=1, we can delete anything. So for k=1, we can always reduce to a palindrome (e.g., empty array). So YES.

        // For k>1, we cannot delete arbitrary elements. We can only delete the k-th smallest of a subarray of length >= k. This means we can delete an element if we can surround it with enough smaller or equal elements? Let's analyze. Suppose we want to delete a_i. We need to choose l <= i <= r with r-l+1 >= k, such that a_i is the k-th smallest in a[l..r]. This means that in a[l..r], there are exactly k-1 elements that are <= a_i? Not exactly, because of duplicates. But essentially, we need that a_i is not too small and not too large relative to the subarray. Specifically, we need that the number of elements in the subarray that are strictly less than a_i is <= k-1, and the number of elements that are <= a_i is >= k. So we need at least k elements in the subarray that are <= a_i, and at most k-1 elements that are strictly less. So if we can find a subarray of length >= k containing a_i, where a_i is the k-th smallest, we can delete it. This is possible if there are at least k-1 elements in the array that are <= a_i? Not necessarily, because we can only use elements in the subarray. But we can extend the subarray to include more elements. If we include elements > a_i, they don't affect the count of elements <= a_i. So we can always increase the length by adding larger elements. So the condition to delete a_i is: there exists a subarray containing a_i with at least k elements <= a_i, and at most k-1 elements strictly less than a_i. That means we need at least k elements in the array that are <= a_i, and we can choose a subarray that includes a_i and enough of those <= a_i elements, while avoiding including too many strictly smaller elements. But we can also include elements equal to a_i. So if there are at least k elements in the whole array that are <= a_i, we can potentially delete a_i? Not exactly, because we need to form a contiguous subarray containing a_i that has at least k elements <= a_i. We can always extend left and right to include more elements <= a_i until we have k of them, as long as we don't include too many strictly smaller elements? Actually, if we include strictly smaller elements, they count towards the k-1 limit. We need the number of strictly smaller elements to be <= k-1. So if there are many strictly smaller elements, we might exceed k-1 before we get k elements <= a_i. But we can also include elements equal to a_i, which count as <= a_i but not strictly smaller. So if we have enough copies of a_i, we can use them. In general, an element a_i can be deleted if there exists a subarray containing a_i where the number of elements < a_i is < k, and the number of elements <= a_i is >= k. This is equivalent to: the k-th smallest element in some subarray containing a_i is a_i. This is possible if and only if there are at least k elements in the array that are <= a_i, and we can find a subarray where a_i is not too small. Actually, we can always choose the subarray to be the whole array if n >= k. Then the k-th smallest of the whole array is some value. We can delete that value. So we can always delete the k-th smallest of the entire array. Then the array shrinks, and we can again delete the k-th smallest of the new array, etc. So we can repeatedly delete the k-th smallest element of the current array. This is a valid sequence of operations. So we can simulate the process by always deleting the k-th smallest element of the current array. But we can also choose other subarrays. So the set of deletable elements is exactly those that can become the k-th smallest of some subarray of length >= k. But maybe we can only delete elements that are "not too small". 

        // Let's look at the sample cases to understand.
        // Test 3: n=6, k=6, a=[2,3,4,5,3,2]. k=n. We can only select the whole array. The 6th smallest is the maximum? Actually, 6th smallest in 6 elements is the largest. The largest is 5. So we delete 5, get [2,3,4,3,2] which is palindrome. So YES.
        // Test 4: n=5, k=4, a=[5,2,4,3,1]. Can we? Let's see. We need to delete elements to get palindrome. The array is not palindrome. We can try to delete the 4th smallest of some subarray of length >=4. The whole array length 5, 4th smallest: sorted: 1,2,3,4,5 -> 4th is 4. Delete 4 -> [5,2,3,1]. Not palindrome. Now length 4, k=4, we must select whole array. 4th smallest is max: 5. Delete 5 -> [2,3,1]. Length 3 < k, cannot delete. Not palindrome. So NO. What if we chose a different subarray initially? We could choose subarray of length 4: e.g., [5,2,4,3] (indices 1-4). Sorted: 2,3,4,5 -> 4th smallest is 5. Delete 5 -> [2,4,3,1]. Not palindrome. Then length 4, must delete 4th smallest of whole array: sorted 1,2,3,4 -> 4th is 4. Delete 4 -> [2,3,1]. Not palindrome. Or choose subarray [2,4,3,1] (indices 2-5): sorted 1,2,3,4 -> 4th is 4. Delete 4 -> [5,2,3,1] same. So NO.
        // Test 5: n=8, k=5, a=[4,7,1,2,3,1,3,4]. Output NO.
        // Test 6: n=5, k=4, a=[1,2,1,2,2]. Output YES. How? Let's see. We can delete the 4th smallest of whole array: sorted 1,1,2,2,2 -> 4th is 2. There are three 2's. We can delete one of the 2's. Suppose we delete the last 2: [1,2,1,2]. Length 4, k=4, must delete 4th smallest: sorted 1,1,2,2 -> 4th is 2. Delete a 2 -> [1,2,1] which is palindrome. So YES.
        // Test 7: n=3, k=3, a=[1,2,2]. Output NO. k=n=3, must delete 3rd smallest = max = 2. Delete 2 -> [1,2] length 2 < k, cannot delete, not palindrome. If we delete the other 2, same. So NO.
        // Test 8: n=4, k=4, a=[2,1,2,2]. Output YES. k=n=4, delete 4th smallest = max = 2. Delete one 2 -> [2,1,2] which is palindrome. So YES.

        // Observations: When k=n, we can only delete the maximum element (the n-th smallest). So we can only delete the largest element. We can do this repeatedly until length < k. So we can only delete the largest elements one by one. So the final array will be the original array without some of the largest elements. So we can get a palindrome if by removing some largest elements we get a palindrome. In test 3, removing 5 gives palindrome. In test 7, removing 2 gives [1,2] not palindrome, and we can't remove more because length <3. So NO. In test 8, removing one 2 gives [2,1,2] palindrome. So YES.

        // For general k, we can delete the k-th smallest of any subarray of length >= k. This is more flexible. But maybe we can characterize which elements are deletable. An element x is deletable if there are at least k elements <= x in the array? Not exactly, because we need a subarray containing x with at least k elements <= x and at most k-1 elements < x. If we take the whole array, the k-th smallest is some value v. We can delete any occurrence of v? Actually, if there are multiple v's, we can choose which one to delete? The problem says: "select an index i such that l<=i<=r where a_i is the k-th smallest number". If there are multiple, we can select any. So if the k-th smallest is v, we can delete any element with value v that is in the subarray? Wait, the k-th smallest is a specific value, but there might be multiple elements with that value. The k-th smallest number is the number that appears at position k when sorted. If there are duplicates, the k-th smallest could be, say, the second 2. But any element with value 2 that is in the subarray could be considered the k-th smallest if we order them appropriately? The problem says: "a_i is the k-th smallest number out of the subarray". If there are multiple elements with the same value, they are all the k-th smallest if the k-th position falls within that value. For example, subarray [2,2,2] with k=2: sorted is [2,2,2], the 2nd smallest is 2. Any of the three 2's can be chosen as the 2nd smallest? The problem says: "If there are multiple possible i, you may select any." So yes, if the k-th smallest value is v, and there are multiple v's, we can delete any of them. So we can delete any occurrence of the k-th smallest value in the chosen subarray.

        // So the operation is: pick a subarray of length >= k, find its k-th smallest value (call it v), then delete any one element with value v from that subarray.

        // Now, how to determine if we can get a palindrome? This seems like we can delete elements that are "too small" or "too large"? Let's think about the two-pointer approach from the hints. Hint 3: "Consider a two-pointer approach." Hint 5: "Maintain two pointers, l and r which represent the left and right boundary of the interval." Maybe we can simulate the process by trying to match the ends, and when we have a mismatch, we can delete the mismatched element if it is the k-th smallest of some subarray? But we can choose the subarray to be the whole current array? If the current array length >= k, we can always delete the k-th smallest of the whole array. So we can always delete the k-th smallest element of the current array. So we can repeatedly delete the k-th smallest element until the array length < k. Then we check if the remaining array is a palindrome. But is this always optimal? Can we get a palindrome by deleting other elements? Suppose we want to keep some small elements and delete large ones. If we always delete the k-th smallest, we might delete small elements that we want to keep. But we can choose subarrays that don't include those small elements? We can choose a subarray that excludes the small elements we want to keep, and then delete the k-th smallest of that subarray. So we have more control.

        // Let's think differently. The operation allows us to delete an element if we can find a subarray of length >= k where that element is the k-th smallest. This is equivalent to: we can delete an element x if there exists a subarray containing x with at least k elements <= x, and at most k-1 elements < x. In other words, x is not among the (k-1) smallest elements of that subarray, but it is among the elements <= the k-th smallest. Actually, x must be exactly the k-th smallest. So if we sort the subarray, x is at position k. So there are exactly k-1 elements <= x that are strictly smaller? Not exactly, because of duplicates. But the condition is: number of elements < x is <= k-1, and number of elements <= x is >= k. So x must have at least k elements <= it in the subarray, and at most k-1 elements strictly smaller. This means that in the subarray, x is not too small: there are at most k-1 elements strictly smaller than x. So x cannot be one of the (k-1) smallest elements
