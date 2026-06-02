#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n, k;
        long long p, q;
        cin >> n >> k >> p >> q;
        vector<long long> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }
        
        // If p divides q, we can only use p effectively
        if (q % p == 0) {
            long long ans = 0;
            for (long long x : a) {
                ans += x % p;
            }
            cout << ans << '\n';
            continue;
        }
        
        // We can always reduce each element to < p using q, then possibly further with p
        // Strategy: use q on intervals of length >= k to reduce numbers to < q
        // Then use p on intervals to reduce further
        
        // First, compute minimum sum if we only use q
        // We can reduce any element to a[i] % q by applying q on a segment containing it
        // But we need segments of length >= k
        // If n >= k, we can cover all elements with one segment, so all become a[i] % q
        // If n < k, we cannot use any operation, so sum is original sum
        
        long long sum_only_q = 0;
        if (n >= k) {
            for (long long x : a) {
                sum_only_q += x % q;
            }
        } else {
            for (long long x : a) {
                sum_only_q += x;
            }
        }
        
        // Now consider using p after q
        // After using q, each element is in [0, q-1]
        // We can use p on segments of length >= k to reduce elements to < p
        // But p and q are coprime? Not necessarily, but q % p != 0 since we handled that case
        // Actually, we can reduce any element to min(x % p, x) using p
        // The key insight: we can reduce each element independently to its value modulo p,
        // provided we can cover it with a segment of length >= k where we apply p.
        // But we can also apply p on the whole array if n >= k.
        // However, we might not be able to reduce all elements if n < k.
        
        // Let's think: after applying q, we have array b[i] = a[i] % q (if n >= k)
        // Then we want to apply p on some segments to minimize sum.
        // Since p < q and q % p != 0, we can reduce b[i] to b[i] % p.
        // But we can only apply p on segments of length >= k.
        // If n >= k, we can apply p on the whole array, so all become b[i] % p.
        // So answer would be sum of (a[i] % q) % p.
        // But wait, is that always optimal? Could we skip q for some elements?
        // If we apply p directly on original a[i], we get a[i] % p.
        // Since p < q, a[i] % p might be smaller than (a[i] % q) % p?
        // Actually, (a[i] % q) % p = a[i] % p if q is a multiple of p, but we handled that.
        // In general, (x % q) % p is not necessarily x % p.
        // Example: p=3, q=5, x=7: x%p=1, (x%q)%p = 2%3=2. So different.
        // So we might want to apply p directly on some elements without first applying q.
        
        // The operation allows choosing m from {p,q}.
        // We can apply operations in any order.
        // Since p < q, applying p after q might not give the same as applying p directly.
        // But we can always apply p first? No, order matters because modulo is not commutative.
        // Actually, (x mod q) mod p != x mod p in general.
        // So we have two choices for each element: reduce mod p or mod q (or both in some order).
        // But we can only apply operations on intervals of length >= k.
        
        // Let's analyze the effect of operations.
        // Applying m on an interval replaces each a_i with a_i mod m.
        // This is idempotent: applying same m again does nothing.
        // Applying a smaller modulus after a larger one: (x mod q) mod p.
        // Applying larger after smaller: (x mod p) mod q = x mod p (since x mod p < p < q).
        // So order matters: if we apply p then q, the q has no effect because numbers are already < p < q.
        // So effectively, we can either:
        // - Apply only p on some elements
        // - Apply only q on some elements
        // - Apply q then p on some elements (which gives (x mod q) mod p)
        // We cannot apply p then q to get a different result.
        
        // So for each element, the possible final values are:
        // x (no op), x mod p, x mod q, (x mod q) mod p.
        // We want to choose for each element one of these, subject to the constraint that
        // the operations can be realized by interval operations of length >= k.
        
        // When can we achieve a certain combination?
        // We can think of it as: we can apply p on some set of intervals, and q on some set of intervals.
        // Since applying p after q is equivalent to applying p on the result of q,
        // we can just consider the final modulus applied to each element.
        // Actually, if we apply q then p on an element, the last operation applied is p.
        // If we apply only q, last is q.
        // If we apply only p, last is p.
        // So the final value is either x mod p or x mod q or (x mod q) mod p.
        // But note: (x mod q) mod p is just the result of applying p after q.
        // So we can think of each element as having a "last operation" which is either p or q or none.
        // If last is p, the value is either x mod p (if no q before) or (x mod q) mod p (if q before).
        // But wait, if we apply p directly, we get x mod p. If we apply q then p, we get (x mod q) mod p.
        // These are different. So we have two possible outcomes from "last operation p".
        // However, we can always achieve x mod p by just applying p on the original array.
        // To achieve (x mod q) mod p, we must apply q first, then p.
        // So the set of possible final values for an element is:
        // original, x mod p, x mod q, (x mod q) mod p.
        
        // But we can also apply operations on intervals that cover multiple elements.
        // The constraint is that each operation must be on an interval of length >= k.
        // We can do multiple operations.
        
        // Let's think about the structure of operations.
        // Suppose we want to apply p on some set of elements, and q on some set.
        // We can always apply p on the whole array if n >= k, turning all to x mod p.
        // But if we want some elements to be x mod q, we need to apply q on them.
        // If we apply q on an interval, it overwrites previous p on that interval.
        // So the final value on an element is determined by the last operation covering it.
        
        // This is similar to: we have a sequence, we can cover it with intervals of length >= k,
        // each interval assigned either p or q. The value at position i is a_i mod (assigned modulus of the last interval covering i).
        // We want to minimize sum.
        
        // Since we can do operations in any order, we can think of building the final array from right to left or something.
        // Actually, we can always achieve any assignment where the set of elements with a certain modulus can be covered by intervals of length >= k?
        // Not exactly, because intervals can overlap.
        
        // Let's consider the case n >= k. Then we can apply p on the whole array initially, getting all a_i mod p.
        // Then we can apply q on some intervals of length >= k to change those elements to (a_i mod p) mod q = a_i mod p (since a_i mod p < p < q).
        // So applying q after p does nothing! Because numbers are already < q.
        // So if we apply p first, q becomes useless.
        // Therefore, to get the benefit of q, we must apply q before p, or not apply p at all on those elements.
        
        // So the optimal strategy: we can partition the array into segments.
        // On some segments, we apply q (and possibly p afterwards, but p after q gives (x mod q) mod p).
        // On other segments, we apply p directly (or nothing, but p is better or equal to nothing since p >= 1, but x mod p <= x).
        // Actually, applying p is always at least as good as doing nothing, because x mod p <= x.
        // So we can assume we always apply p on elements where we don't apply q? Not necessarily, because applying p might prevent us from applying q later? But we can order operations: first apply q on some intervals, then apply p on some intervals. The p intervals will overwrite q intervals where they overlap. So elements covered by p in the second phase will end up with (previous value) mod p. If previous value was original, they get x mod p. If previous value was x mod q, they get (x mod q) mod p.
        // Elements not covered by p in the second phase will keep their value from the first phase (either original or x mod q).
        
        // So we can think of it as: we choose a set of intervals for q (first phase), then a set of intervals for p (second phase).
        // The final value at i is:
        // - if i is covered by some p-interval: (value after q-phase) mod p
        // - else: value after q-phase (original if not covered by q, or x mod q if covered by q)
        
        // Since p < q, applying p on an element that was not covered by q gives x mod p.
        // Applying p on an element that was covered by q gives (x mod q) mod p.
        // Note that x mod p <= (x mod q) mod p? Not necessarily. Example: p=3, q=5, x=7: x mod p = 1, (x mod q) mod p = 2. So x mod p is smaller.
        // So we might prefer to apply p directly on some elements rather than q then p.
        // But if we apply p directly, we cannot have q applied there (since p overwrites).
        // So for each element, we have three reasonable options:
        // 1. Apply p directly: value = x mod p
        // 2. Apply q only: value = x mod q
        // 3. Apply q then p: value = (x mod q) mod p
        // Option 4: do nothing: value = x (but this is always >= x mod p, so dominated by option 1 if we can apply p)
        
        // Now, can we achieve any combination of these options? The constraints are:
        // - q-intervals must have length >= k.
        // - p-intervals must have length >= k.
        // - p-intervals can overlap with q-intervals; where they overlap, option 3 happens.
        // - Where only q-interval covers, option 2.
        // - Where only p-interval covers, option 1.
        // - Where neither covers, option 4 (but we can always cover with p if n >= k, so option 1 dominates).
        
        // If n < k, we cannot do any operation, so answer is sum of a_i.
        // So assume n >= k.
        
        // We can always cover the whole array with p-intervals (one interval of length n >= k).
        // So we can set all elements to option 1: sum = sum (a_i mod p).
        // Can we do better by using q on some segments?
        // For an element, option 2 gives x mod q. Is x mod q < x mod p? Sometimes yes, sometimes no.
        // Option 3 gives (x mod q) mod p. This is always <= x mod q, and could be < x mod p.
        // So we might want to use q on some segments, and possibly p on top of them.
        
        // But note: if we apply p on top of q, we get option 3. If we don't apply p, we get option 2.
        // Since p-intervals can be anywhere, we can selectively apply p on some parts of a q-interval.
        // However, p-intervals must have length >= k. So we cannot apply p on isolated small regions.
        
        // Let's think differently. Since we can do operations in any order, we can think of the final array as being built by:
        // For each element, we can choose to reduce it modulo p or modulo q, but with the restriction that
        // the set of elements reduced modulo q must be a union of intervals of length >= k?
        // Actually, we can apply q on multiple intervals. The union of q-intervals is the set of elements that get q applied (at least once).
        // Then we can apply p on some intervals. The p-intervals will reduce those elements modulo p.
        // If a p-interval covers an element that had q applied, it becomes (x mod q) mod p.
        // If it covers an element without q, it becomes x mod p.
        // So the final value depends on whether the element is in the q-set and whether it is in the p-set.
        
        // Let S_q be the set of indices covered by at least one q-interval.
        // Let S_p be the set of indices covered by at least one p-interval.
        // Then final value:
        // i in S_p and i in S_q: (a_i mod q) mod p
        // i in S_p and i not in S_q: a_i mod p
        // i not in S_p and i in S_q: a_i mod q
        // i not in S_p and i not in S_q: a_i
        
        // We can assume S_p is the whole array if n >= k, because we can always apply p on [1,n] and it only reduces values (since a_i mod p <= a_i, and (a_i mod q) mod p <= a_i mod q). So setting S_p = all indices is always optimal or equal.
        // Wait, is it always optimal to apply p everywhere? Applying p on an element that had q applied changes it from a_i mod q to (a_i mod q) mod p. Since (x mod q) mod p <= x mod q, this is always an improvement or equal. Applying p on an element without q changes it from a_i to a_i mod p, which is also <=. So applying p on the whole array at the end can only decrease the sum. Therefore, we can assume without loss of generality that we always apply p on the entire array at the very end.
        // But wait: if we apply p on the whole array at the end, then any q applied earlier will be "overwritten" by p, resulting in (a_i mod q) mod p. If we didn't apply q at all, we get a_i mod p. So the choice is: for each element, we can either have a_i mod p (if we don't apply q to it) or (a_i mod q) mod p (if we do apply q to it before the final p).
        // But we can also choose to not apply the final p on some elements? No, we are applying p on the whole array. That covers everything.
        // So the problem reduces to: we can choose a set of intervals of length >= k to apply q. Then we apply p on the whole array. The final value at index i is (a_i mod q) mod p if i is covered by some q-interval, else a_i mod p.
        // Is this always optimal? What if we don't apply p on the whole array, but only on parts? That would leave some elements as a_i mod q (if they had q) or a_i (if no q). But a_i mod q >= (a_i mod q) mod p, and a_i >= a_i mod p. So applying p everywhere is at least as good. So yes, we can assume final p on everything.
        
        // Therefore, the problem simplifies:
        // We have array a. We can choose a set of disjoint? No, overlapping intervals of length >= k to apply q.
        // Then we apply p on everything.
        // We want to minimize sum over i of:
        //   if i is covered by any q-interval: (a_i mod q) mod p
        //   else: a_i mod p
        
        // Let cost_if_q(i) = (a_i mod q) mod p
        // Let cost_if_no_q(i) = a_i mod p
        // We want to choose a union of intervals of length >= k (the q-intervals) to minimize total cost.
        // Note that intervals can overlap; the union is just the set of indices that get q.
        // The constraint is that this set must be a union of intervals each of length >= k.
        // But any union of intervals of length >= k is just a set that can be formed by such intervals.
        // Actually, if we have a set S that is a union of intervals of length >= k, then S is just a set where every maximal contiguous block in S has length >= k? Not necessarily, because we could cover a block of length < k by overlapping larger intervals? But if we have an interval of length >= k, it covers a contiguous block of length >= k. The union of such intervals can have contiguous blocks of any length? For example, two intervals of length k that overlap can form a block of length > k. But can we have a block of length < k in the union? Suppose we have intervals [1, k] and [2, k+1]. Their union is [1, k+1] which has length k+1 >= k. If we try to have a block of length < k, say we want to cover only indices 1..k-1. We cannot, because any interval we use must have length >= k, so it would cover at least k consecutive indices. Therefore, any index covered by some q-interval must be part of a contiguous covered segment of length at least k? Actually, if an index is covered, it belongs to some interval of length >= k. That interval covers a contiguous block of length >= k containing that index. So the entire block is covered. Thus, the set of covered indices is a union of disjoint contiguous blocks, each of length at least k. Because if two intervals overlap, their union is a larger contiguous block, which will have length >= k. So the covered set is exactly a set of disjoint intervals, each of length >= k.
        // Wait, is it possible to have a covered set that is a single interval of length >= k, but with "holes"? No, intervals are contiguous. So the covered set is a union of disjoint intervals, each of length >= k.
        // Therefore, we can choose any set of disjoint intervals of length >= k to apply q. (They don't even need to be disjoint? If they overlap, we can just take their union as a single interval, which will have length >= k. So we can assume we choose a set of disjoint intervals of length >= k.)
        
        // So the problem: given array, choose a set of disjoint intervals of length >= k to "apply q". For indices in these intervals, cost = (a_i mod q) mod p. For others, cost = a_i mod p. Minimize total cost.
        
        // This is a classic DP problem.
        // Let dp[i] = minimum cost for prefix up to i.
        // For each i, we can either not cover i with q: dp[i] = dp[i-1] + cost_no_q(i)
        // Or we can end a q-interval at i: we choose a start j <= i - k + 1, and cover [j, i] with q.
        // Then dp[i] = min_{j <= i - k + 1} ( dp[j-1] + sum_{t=j}^i cost_q(t) )
        // We can precompute prefix sums of cost_q to get sum in O(1).
        // Then dp[i] = min( dp[i-1] + cost_no_q(i), min_{j <= i - k + 1} ( dp[j-1] - pref_q[j-1] ) + pref_q[i] )
        // We can maintain the minimum of (dp[j-1] - pref_q[j-1]) for j from 1 to i - k + 1.
        // This is O(n) per test case.
        
        // But wait: is it always optimal to apply p on the whole array at the end? What if we don't apply p on some elements that had q, leaving them as a_i mod q? That would be cost = a_i mod q. But we already have the option to not apply p on those elements by simply not including them in the final p-interval. However, we assumed we apply p on everything. But if a_i mod q < (a_i mod q) mod p? That's impossible because (x mod q) mod p <= x mod q. So applying p always reduces or keeps same. So it's never worse.
        // What about elements without q? a_i mod p <= a_i. So applying p is always better.
        // So final p on everything is indeed optimal.
        
        // But wait! There is a catch: the operation requires the interval length to be at least k. If we apply p on the whole array, that's one interval of length n >= k, which is allowed. So yes.
        
        // However, is it possible that we want to apply q on some interval, but then NOT apply p on that interval, because applying p would force us to also apply p on adjacent elements where it might be worse? No, because applying p on an element never increases its value. So applying p on more elements is always better or neutral. There's no negative effect of applying p on an element. So we can always apply p on everything.
        
        // Therefore, the DP above should give the correct answer.
        
        // Let's test with the sample.
        // Sample 1: n=1, k=1, p=3, q=4, a=[2026]
        // n>=k, cost_no_q = 2026%3 = 1, cost_q = (2026%4)%3 = 2%3=2.
        // We can choose interval [1,1] for q or not. Min is 1. Output 1. Matches.
        
        // Sample 2: n=3, k=2, p=10, q=20, a=[31,41,59]
        // cost_no_q: 31%10=1, 41%10=1, 59%10=9 -> sum=11
        // cost_q: (31%20)%10 = 11%10=1, (41%20)%10=1, (59%20)%10=19%10=9 -> same.
        // But we can only use intervals of length >=2. We can cover [1,2] or [2,3] or [1,3]. All give same cost. Output 11. Matches.
        
        // Sample 3: n=4, k=3, p=3, q=4, a=[1,2,3,4]
        // cost_no_q: 1%3=1, 2%3=2, 3%3=0, 4%3=1 -> sum=4
        // cost_q: (1%4)%3=1, (2%4)%3=2, (3%4)%3=0, (4%4)%3=0 -> sum=3
        // We can cover [1,3] or [2,4] or [1,4] with q. All give sum 3. Output 3. Matches.
        
        // Sample 4: n=6, k=4, p=9, q=20, a=[18,27,180,9,45,99]
        // Let's compute:
        // cost_no_q: 18%9=0, 27%9=0, 180%9=0, 9%9=0, 45%9=0, 99%9=0 -> sum=0
        // So answer 0. Matches.
        
        // Sample 5: n=7, k=4, p=3, q=5, a=[6,7,14,12,100,78,4]
        // cost_no_q: 6%3=0, 7%3=1, 14%3=2, 12%3=0, 100%3=1, 78%3=0, 4%3=1 -> sum=5
        // cost_q: (6%5)%3=1%3=1, (7%5)%3=2%3=2, (14%5)%3=4%3=1, (12%5)%3=2%3=2, (100%5)%3=0, (78%5)%3=3%3=0, (4%5)%3=4%3=1 -> sum=7
        // So cost_q is worse. But we can choose to apply q on some intervals? Actually, if we apply q on some interval, we get cost_q there, which is higher. So we should not apply q at all. Then sum=5. But sample output is 4. So my reasoning is missing something.
        // Let's re-examine sample 5.
        // Input: 7 4 3 5
        // 6 7 14 12 100 78 4
        // Output: 4
        // How to get 4?
        // Let's try to find operations.
        // We can apply q on some interval, then p on some interval.
        // Maybe we don't apply p on everything? Or we apply p on some parts without q?
        // Let's compute all possibilities manually.
        // a = [6,7,14,12,100,78,4]
        // Option: apply q on [1,4] (len 4): becomes [1,2,4,2,100,78,4] (since 6%5=1, 7%5=2, 14%5=4, 12%5=2)
        // Then apply p on [4,7] (len 4): indices 4..7: 2%3=2, 100%3=1, 78%3=0, 4%3=1 -> array [1,2,4,2,1,0,1] sum=11.
        // Not 4.
        // Try apply p on [1,4] first: [0,1,2,0,100,78,4]
        // Then apply q on [4,7]: [0,1,2,0,0,3,4] (100%5=0, 78%5=3, 4%5=4) sum=10.
        // Try apply q on [1,5] (len 5): [1,2,4,2,0,78,4]
        // Then p on [3,6] (len 4): [1,2,1,2,0,0,4] sum=10.
        // Try apply q on [2,5]: [6,2,4,2,0,78,4]
        // Then p on [1,4]: [0,2,1,2,0,78,4] sum=87? No, 0+2+1+2+0+78+4=87.
        // Try apply p on [1,7] (whole): [0,1,2,0,1,0,1] sum=5.
        // How to get 4?
        // Maybe apply q on [1,4] and p on [1,4]? That gives [1,2,1,2,100,78,4] sum=188.
        // Let's think: we can apply q on some interval, then p on some interval, but p interval can be inside q interval? Yes.
        // What if we apply q on [1,5] -> [1,2,4,2,0,78,4]
        // Then apply p on [1,4] -> [1,2,1,2,0,78,4] sum=88.
        // Not 4.
        // Let's try to get 4: maybe we don't apply p on everything. Suppose we apply q on [1,4] -> [1,2,4,2,100,78,4]
        // Then apply p on [5,7]? len 3 < 4, not allowed.
        // Apply p on [4,7] -> [1,2,4,2,1,0,1] sum=11.
        // Apply p on [1,3]? len 3 < 4.
        // Apply p on [1,4] -> [1,2,1,2,100,78,4] sum=188.
        // Apply p on [2,5] -> [1,2,1,2,0,78,4] sum=88.
        // Apply p on [3,6] -> [1,2,1,2,0,0,4] sum=10.
        // Apply p on [4,7] -> [1,2,4,2,1,0,1] sum=11.
        // What if we apply p first on some interval, then q on some interval?
        // p on [1,4] -> [0,1,2,0,100,78,4]
        // q on [1,4] -> [0,1,2,0,100,78,4] (since <5)
        // q on [4,7] -> [0,1,2,0,0,3,4] sum=10.
        // q on [1,5] -> [0,1,2,0,0,78,4] sum=85.
        // q on [2,5] -> [0,1,2,0,0,78,4] sum=85.
        // q on [3,6] -> [0,1,2,0,0,3,4] sum=10.
        // q on [1,7] -> [0,1,2,0,0,3,4] sum=10.
        // Not 4.
        // Maybe we can apply multiple operations? The problem says arbitrary number of times.
        // Let's try: apply q on [1,4] -> [1,2,4,2,100,78,4]
        // apply q on [4,7] -> [1,2,4,2,0,3,4] (100%5=0, 78%5=3, 4%5=4)
        // apply p on [1,4] -> [1,2,1,2,0,3,4] sum=13.
        // apply p on [2,5] -> [1,2,1,2,0,3,4] sum=13.
        // apply p on [3,6] -> [1,2,1,2,0,0,4] sum=10.
        // apply p on [4,7] -> [1,2,4,2,0,0,1] sum=10.
        // Not 4.
        // What if we apply p on [1,7] first: [0,1,2,0,1,0,1] sum=5.
        // Then apply q on [1,4]? No effect because <5.
        // Then apply q on [4,7]? [0,1,2,0,1,0,1] -> 1%5=1, 0%5=0, 1%5=1 -> [0,1,2,0,1,0,1] sum=5.
        // So 5.
        // How to get 4? Let's look at the note for sample 3, but sample 5 has no note.
        // Maybe we can apply q on [1,5] and p on [1,5]? That gives [1,2,1,2,0,78,4] sum=88.
        // Wait, maybe we can apply p on [1,7] and then q on [1,7]? No effect.
        // Let's compute all possible final values for each element if we can choose any combination of operations, ignoring interval constraints, and see the minimum possible sum.
        // For each element, possible values:
        // 6: 6, 6%3=0, 6%5=1, (6%5)%3=1 -> min 0
        // 7: 7, 1, 2, 2 -> min 1
        // 14: 14, 2, 4, 1 -> min 1
        // 12: 12, 0, 2, 2 -> min 0
        // 100: 100, 1, 0, 0 -> min 0
        // 78: 78, 0, 3, 0 -> min 0
        // 4: 4, 1, 4, 1 -> min 1
        // Sum of mins = 0+1+1+0+0+0+1 = 3. But output is 4. So we cannot achieve all mins simultaneously due to interval constraints.
        // So we need to choose intervals wisely.
        // Let's try to achieve sum 4.
        // Maybe: apply q on [1,4] -> [1,2,4,2,100,78,4]
        // apply p on [3,6] -> [1,2,1,2,0,0,4] sum=10.
        // apply p on [1,4]? No.
        // What if we apply p on [1,4] first: [0,1,2,0,100,78,4]
        // then apply q on [3,6] -> [0,1,2,0,0,3,4] sum=10.
        // What if we apply q on [2,5] -> [6,2,4,2,0,78,4]
        // then p on [1,4] -> [0,2,1,2,0,78,4] sum=87.
        // What if we apply q on [1,5] -> [1,2,4,2,0,78,4]
        // then p on [1,5]? len 5: [1,2,1,2,0,78,4] sum=88.
        // What if we apply p on [1,5] first: [0,1,2,0,1,78,4]
        // then q on [1,5] -> [0,1,2,0,1,78,4] (no change)
        // then q on [4,7]? len 4: [0,1,2,0,1,3,4] sum=11.
        // Not 4.
        // Let's try: apply q on [1,4] -> [1,2,4,2,100,78,4]
        // apply q on [5,7]? len 3 < 4, not allowed.
        // apply q on [4,7] -> [1,2,4,2,0,3,4]
        // apply p on [1,3]? len 3 < 4.
        // apply p on [2,5] -> [1,2,1,2,0,3,4] sum=13.
        // apply p on [1,4] -> [1,2,1,2,0,3,4] sum=13.
        // apply p on [3,6] -> [1,2,1,2,0,0,4] sum=10.
        // apply p on [4,7] -> [1,2,4,2,0,0,1] sum=10.
        // What if we apply p on [1,7] at
