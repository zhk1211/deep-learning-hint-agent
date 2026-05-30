// Hint5
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n, k, p, q;
        cin >> n >> k >> p >> q;
        vector<long long> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }

        // We can reduce each element to its minimum possible value
        // using mod p and mod q. Since we can apply operations on intervals
        // of length >= k, we can independently reduce elements if we can
        // cover them with intervals. The key is that we can always reduce
        // any element to min(a[i] % p, a[i] % q) if we can apply an interval
        // containing it. But we might need to apply mod p then mod q or vice versa.
        // Actually, we can achieve min(a[i] % p, a[i] % q, a[i] % p % q, a[i] % q % p).
        // Since p < q, a[i] % p % q = a[i] % p (because a[i] % p < p < q).
        // a[i] % q % p could be smaller. So the minimum possible value for a[i] is
        // min(a[i] % p, a[i] % q % p). But wait: we can also apply mod p after mod q,
        // so we can get a[i] % q % p. Also we can just apply mod p to get a[i] % p.
        // So the best we can do for a single element is min(a[i] % p, a[i] % q % p).
        // However, we have the constraint that operations must be on intervals of length >= k.
        // This means we cannot independently reduce every element if k > 1.
        // We need to choose a set of disjoint intervals of length >= k (or overlapping,
        // but overlapping doesn't help more because applying mod again on already reduced
        // values doesn't change them). Actually, we can apply operations sequentially,
        // and they can overlap. But the effect on an element is the result of a sequence
        // of mod operations. Since mod is idempotent and order matters only for different mods,
        // the final value of an element is determined by the last mod operation applied to it,
        // and possibly previous ones if the last one didn't cover it? No, each operation
        // applies to all elements in the interval. So if an element is covered by multiple
        // operations, the final value is the result of applying those mods in order.
        // But we can always reorder operations? Not exactly, because intervals have length constraint.
        // However, we can think of it as: we can choose for each element a sequence of mods,
        // but the sequences must be "compatible" with intervals of length >= k.
        // This is a classic problem: we have an array, we can apply operations on intervals
        // of length >= k. We want to minimize sum. Each operation reduces values in the interval
        // to their modulo. We can think of it as: we can "protect" some elements from being reduced,
        // or we can reduce them. But we want to reduce as much as possible.
        // Since we can apply operations multiple times, we can effectively set any element
        // to its minimum possible value if we can cover it with an interval of length >= k
        // that we use at the right time. But we might need to cover some elements with intervals
        // that also cover other elements we don't want to reduce further? Actually, if we apply
        // mod p on an interval, it reduces all elements in that interval to a[i] % p.
        // If we later apply mod q on a subinterval, those elements become (a[i] % p) % q = a[i] % p
        // (since a[i] % p < p < q). So applying mod q after mod p does nothing.
        // If we apply mod q first, then mod p, we get a[i] % q % p, which could be smaller.
        // So the optimal sequence for an element is either just mod p, or mod q then mod p.
        // We cannot get a[i] % q alone if we also apply mod p later, because mod p would further reduce it.
        // But we could apply mod q and never apply mod p on that element. So the possible final values
        // for an element are: original a[i], a[i] % p, a[i] % q, a[i] % q % p.
        // Since p < q, a[i] % q % p <= a[i] % p (because a[i] % q is already < q, and taking % p gives something < p).
        // Also a[i] % q could be larger than a[i] % p? Not necessarily. For example a=10, p=3, q=4: 10%3=1, 10%4=2, 10%4%3=2%3=2. So min is 1.
        // So the minimum possible value for a[i] is min({a[i], a[i] % p, a[i] % q, a[i] % q % p}).
        // But note that a[i] % q % p is always <= a[i] % p, so we can ignore a[i] % p if we can achieve a[i] % q % p.
        // However, achieving a[i] % q % p requires applying mod q then mod p on that element.
        // Achieving a[i] % q requires applying mod q and NOT mod p afterwards.
        // Achieving a[i] % p requires applying mod p (and possibly mod q before, but that would give a[i] % q % p, which is different).
        // So we have choices.

        // Now, the interval constraint: any operation must be on an interval of length >= k.
        // This means if we want to apply a certain mod to a set of elements, we must be able to cover them
        // with intervals of length >= k. We can apply multiple operations, possibly overlapping.
        // The problem reduces to: we can assign to each element a "state": 0 (original), 1 (mod p only), 2 (mod q only), 3 (mod q then mod p).
        // But we must be able to realize these states via a sequence of interval operations.
        // What sequences of operations are possible? We can think of it as: we can choose a set of intervals
        // for mod p and a set of intervals for mod q. The final value of an element depends on the last operation
        // that covered it. If the last operation covering it is mod p, it becomes something % p.
        // If the last is mod q, it becomes something % q. If it's covered by both, the order matters.
        // Actually, we can always arrange the order of operations arbitrarily. So we can decide for each element
        // which mod is applied last, and what was applied before. But note that if we apply mod p last,
        // any previous mod q is overridden because mod p will take the current value (which might have been reduced by mod q)
        // and take modulo p. So the final value is (value before last mod p) % p. If we never applied mod q, it's a[i] % p.
        // If we applied mod q before, it's (a[i] % q) % p = a[i] % q % p. So applying mod p last gives either a[i] % p
        // or a[i] % q % p depending on whether mod q was applied before. But wait: if we apply mod p last, we can choose
        // whether to have applied mod q earlier. However, applying mod q earlier requires covering the element with a mod q interval
        // at some point before the mod p interval. That is possible if we can cover it with a mod q interval of length >= k.
        // Similarly, if we apply mod q last, the final value is either a[i] % q (if no mod p before) or (a[i] % p) % q = a[i] % p
        // (since a[i] % p < p < q). So mod q last gives either a[i] % q or a[i] % p. But a[i] % p is already achievable
        // by mod p last without mod q before. So mod q last doesn't give anything new except a[i] % q.
        // Therefore, the possible final values for an element are:
        // - a[i] (no operation covers it, or operations cancelled out? Actually if covered by both, final is determined by last)
        // - a[i] % p (achieved by mod p last, no mod q before)
        // - a[i] % q (achieved by mod q last, no mod p before)
        // - a[i] % q % p (achieved by mod p last, with mod q before)
        // Note that a[i] % p % q = a[i] % p, so mod q last after mod p gives a[i] % p, which is already covered.
        // So we have four options per element.

        // Now, we need to assign each element to one of these four "methods", but the assignments must be consistent
        // with the ability to perform interval operations. Specifically, if we want to apply mod q before mod p
        // on some element, that element must be covered by a mod q interval, and later by a mod p interval.
        // The intervals for mod q and mod p can be chosen independently, but they must have length >= k.
        // This is equivalent to: we can choose a set of elements that receive mod q (at some point), and a set of elements
        // that receive mod p (at some point). The final value depends on the order. But we can always order operations
        // so that all mod q operations happen before all mod p operations, or vice versa, or interleaved.
        // Actually, we can just do all mod q operations first, then all mod p operations. Then:
        // - Elements covered by mod p (in the second phase) will end up as (value after mod q) % p.
        // - Elements not covered by mod p will end up as (value after mod q) % q = value after mod q (since mod q idempotent).
        // - Elements not covered by mod q but covered by mod p will end up as a[i] % p.
        // - Elements covered by neither remain a[i].
        // So if we do all mod q first, then all mod p, we can achieve:
        // - For elements in mod p set: if also in mod q set, final = a[i] % q % p; else final = a[i] % p.
        // - For elements not in mod p set: if in mod q set, final = a[i] % q; else final = a[i].
        // This covers all four cases! And we can choose the sets for mod q and mod p arbitrarily, as long as
        // each set can be exactly covered by a union of disjoint intervals of length >= k? Wait, the operations
        // are applied as intervals. We can apply multiple mod q operations on various intervals. The union of these intervals
        // is the set of elements that receive mod q. But can we achieve any arbitrary set? We can apply mod q on any interval
        // of length >= k. By applying multiple such intervals, we can cover any subset of indices, with the only restriction
        // that we cannot cover an element without also covering at least k consecutive elements? No, we can cover a single element
        // by choosing an interval of length >= k that includes it, but that would also cover other elements. So we cannot
        // independently choose which elements get mod q; the set of elements receiving mod q must be a union of intervals
        // of length >= k. Similarly for mod p. But we can apply operations in any order, and we can apply mod q on some intervals,
        // then mod p on some intervals. The final effect on each element is determined by the last operation that covered it.
        // If we do mod q then mod p, the set of elements that end up with mod p last is exactly the union of mod p intervals.
        // The set of elements that receive mod q (before mod p) is the union of mod q intervals. But note that if an element
        // is covered by a mod p interval, any mod q before is "overwritten" in the sense that the final value uses the mod p
        // on the value after mod q. So the set of elements that effectively get mod q before mod p is the intersection of
        // the mod q union and the mod p union. The set of elements that get only mod q (no mod p after) is the mod q union
        // minus the mod p union. The set that get only mod p is the mod p union minus the mod q union.
        // So we can choose two sets S_q and S_p, each being a union of intervals of length >= k.
        // Then the final value of element i is:
        // if i in S_p:
        //    if i in S_q: a[i] % q % p
        //    else: a[i] % p
        // else:
        //    if i in S_q: a[i] % q
        //    else: a[i]
        // We want to minimize sum of final values.

        // But wait: we can also do mod p first then mod q. That would give:
        // if i in S_q (last): if i in S_p: a[i] % p % q = a[i] % p; else: a[i] % q.
        // if i not in S_q: if i in S_p: a[i] % p; else a[i].
        // This is symmetric but with p and q swapped. Since p < q, a[i] % p is generally smaller than a[i] % q.
        // So doing mod p last seems better because it can reduce values more. But we have the option to do mod q last
        // for some elements to get a[i] % q, which might be smaller than a[i] % p? No, a[i] % q can be larger than a[i] % p
        // because q > p. For example a=10, p=3, q=4: 10%3=1, 10%4=2. So a[i] % p is smaller. But a[i] % q % p could be even smaller.
        // So the best is usually to apply mod p last. The only reason to apply mod q last is if we cannot apply mod p on that element
        // (because we cannot cover it with a mod p interval of length >= k without including other elements we don't want to reduce?
        // But reducing other elements is always beneficial or neutral? Actually, reducing an element never increases its value.
        // So applying mod p on any element is always beneficial or neutral. Therefore, we would want to apply mod p on as many elements
        // as possible. The only restriction is that we must apply it on intervals of length >= k. So we can always apply mod p
        // on the whole array if n >= k? But wait, if we apply mod p on the whole array, every element becomes a[i] % p.
        // That might not be optimal because for some elements, a[i] % q % p could be smaller. To achieve a[i] % q % p,
        // we need to apply mod q before mod p on that element. So we might want to apply mod q on some intervals, then mod p on some intervals.
        // Since mod p reduces everything to < p, and mod q reduces to < q, applying mod p last is generally better.
        // So the optimal strategy is: choose a set of intervals for mod q (which we apply first), then choose a set of intervals for mod p
        // (which we apply second). The final value for i is:
        // if i is covered by mod p: a[i] % q % p if also covered by mod q, else a[i] % p.
        // if i is not covered by mod p: a[i] % q if covered by mod q, else a[i].
        // Since a[i] % q % p <= a[i] % p and a[i] % q >= a[i] % q % p? Not necessarily, but usually a[i] % q % p is very small.
        // We want to minimize sum. Notice that if we don't cover i with mod p, we get either a[i] or a[i] % q, which are >= a[i] % p typically.
        // So it's almost always better to cover i with mod p. The only exception might be if a[i] % q is smaller than a[i] % p?
        // But as argued, since p < q, a[i] % p can be smaller or larger than a[i] % q. Example: a=7, p=3, q=5: 7%3=1, 7%5=2. So a[i]%p < a[i]%q.
        // Another: a=5, p=3, q=4: 5%3=2, 5%4=1. Here a[i]%q < a[i]%p. So it's possible that a[i] % q is smaller.
        // But if we cover i with mod p, we can still get a[i] % q % p. For a=5, p=3, q=4: a%q%p = 5%4%3 = 1%3 = 1. So we get 1, which is <= a[i]%q=1.
        // So covering with mod p (and possibly mod q before) gives value <= a[i] % q? Not always: if a[i] % q % p could be larger than a[i] % q?
        // Since a[i] % q < q, and we take % p, the result is < p. But a[i] % q could be < p as well. Example: a=2, p=3, q=4: a%q=2, a%q%p=2%3=2. Same.
        // What if a=3, p=3, q=4: a%q=3, a%q%p=3%3=0. So 0 < 3. So mod p last is better.
        // Is there a case where a[i] % q < a[i] % q % p? No, because a[i] % q % p is a modulo of a[i] % q, so it's <= a[i] % q.
        // So a[i] % q % p <= a[i] % q always. Therefore, if we can apply mod p on i, we can achieve a value <= a[i] % q.
        // So it's always at least as good to apply mod p on i as to apply only mod q. The only reason not to apply mod p on i
        // is if we are forced to not apply mod p on some interval due to the length constraint? But we can always apply mod p
        // on any interval of length >= k. If we want to apply mod p on i, we can just choose an interval of length k that contains i.
        // That will also apply mod p to k-1 other elements. But that only reduces their values further, which is good!
        // So why wouldn't we just apply mod p on the entire array? Because applying mod p on the entire array gives a[i] % p for all i.
        // But we might get even smaller values by applying mod q before mod p on some elements: a[i] % q % p.
        // To get that, we need to apply mod q on those elements before mod p. But applying mod q on an interval of length >= k
        // will also affect other elements. If we apply mod q on some interval, then later apply mod p on the whole array,
        // those elements in the intersection get a[i] % q % p, while others get a[i] % p. This is always <= a[i] % p.
        // So applying mod q before mod p can only improve things. But wait: if we apply mod q on an interval, it changes values
        // to a[i] % q. Then mod p on whole array changes them to (a[i] % q) % p = a[i] % q % p. For elements not in the mod q interval,
        // they become a[i] % p. So the sum is sum_{i in S_q} (a[i] % q % p) + sum_{i not in S_q} (a[i] % p).
        // This is <= sum a[i] % p. So we can only reduce the sum by applying mod q on some intervals before mod p.
        // But we must ensure that S_q is a union of intervals of length >= k. So the problem reduces to:
        // We will apply mod p on the entire array (or at least on a set that covers everything? Actually, we don't have to
        // apply mod p on everything; we could leave some elements without mod p. But as argued, applying mod p is always beneficial
        // because it reduces values. The only potential issue is if a[i] is already 0? But a[i] >= 1. Mod p makes it < p.
        // So it's always beneficial. So we should apply mod p on the whole array. But wait: is it always possible to apply mod p
        // on the whole array? We can apply it on the whole array in one operation if n >= k. If n < k, we cannot apply any operation
        // because interval length must be >= k. So if n < k, we can do nothing. So for n < k, answer is sum a[i].
        // But the problem says 1 <= k <= n. So n >= k always. So we can always apply mod p on the whole array.
        // Therefore, we can always achieve at most sum (a[i] % p). But we can do better by applying mod q on some intervals
        // before mod p. So the optimal strategy is: choose a set of disjoint? Not necessarily disjoint, but we can choose
        // a set of intervals for mod q (each length >= k), apply them, then apply mod p on the whole array.
        // The final value for i is: if i is covered by any mod q interval, then a[i] % q % p; else a[i] % p.
        // We want to minimize sum = sum_i (a[i] % p) - sum_{i covered by mod q} (a[i] % p - a[i] % q % p).
        // Let gain[i] = (a[i] % p) - (a[i] % q % p). Note that gain[i] >= 0 because a[i] % q % p <= a[i] % p.
        // We want to select a set of indices S (which must be a union of intervals of length >= k) to maximize sum_{i in S} gain[i].
        // Then answer = sum_i (a[i] % p) - max_gain.

        // But wait: is it always optimal to apply mod p on the whole array? What if applying mod p on some element
        // prevents us from getting a better value? No, because mod p last always gives value <= a[i] % p.
        // The only alternative is to not apply mod p on some element, leaving it as a[i] % q (if we apply mod q)
        // or a[i]. But a[i] % q >= a[i] % q % p, and a[i] >= a[i] % p. So applying mod p is always better or equal.
        // So yes, we should apply mod p on everything.

        // However, there is a catch: we can apply mod q after mod p? That would give a[i] % p % q = a[i] % p (since a[i] % p < p < q).
        // So it does nothing. So the only useful order is mod q then mod p.

        // But wait: what if we apply mod q on some interval, then mod p on some other interval that does not cover the whole array?
        // Then elements not covered by mod p remain as a[i] % q. But as argued, a[i] % q >= a[i] % q % p, so we could just extend
        // the mod p interval to cover them and get <= value. So we should cover everything with mod p.

        // Therefore, the problem reduces to: given array a, we can choose a set of intervals of length >= k to apply mod q.
        // Then we apply mod p to the whole array. The final value for i is a[i] % q % p if i is in any chosen interval,
        // else a[i] % p. We want to minimize the sum. This is equivalent to selecting a set of indices S that can be covered
        // by intervals of length >= k, to maximize sum_{i in S} gain[i], where gain[i] = (a[i] % p) - (a[i] % q % p).

        // But is it always possible to apply mod q on arbitrary intervals of length >= k? Yes, we can just apply mod q
        // on each chosen interval. They can overlap; overlapping doesn't hurt. The set of covered indices is the union.
        // So we need to find a subset of indices that can be expressed as a union of intervals of length >= k,
        // maximizing the sum of gains. This is a classic DP: we can choose any set of indices, but we cannot leave gaps
        // of length < k? Actually, if we choose an interval of length >= k, we cover all indices in it. We can choose multiple
        // intervals. The union can have gaps. But if we have a gap of length < k between two chosen intervals, we could
        // merge them into one interval? Not necessarily, because merging would include the gap indices, which might have
        // negative gain? But gain[i] >= 0 always! Because a[i] % q % p <= a[i] % p. So gain[i] >= 0.
        // Therefore, including any extra index never hurts; it only adds non-negative gain.
        // So if we have two intervals that are close (gap < k), we can just extend one to cover the gap, and the gain
        // from the gap indices is >= 0, so it's beneficial or neutral. Therefore, the optimal set S will be a union of
        // intervals of length >= k, but since gains are non-negative, we can always fill gaps of length < k without decreasing
        // the total gain. In fact, we can just take the whole array if all gains are non-negative? But wait, gain[i] could be zero.
        // So we can always include everything. But is there any restriction that we cannot apply mod q on the whole array?
        // We can apply mod q on the whole array if n >= k. Then every element gets a[i] % q % p. That gives sum a[i] % q % p.
        // But wait, is that always better than sum a[i] % p? Since gain[i] >= 0, sum a[i] % q % p <= sum a[i] % p.
        // So applying mod q on the whole array is always at least as good as not applying it. So the optimal is to apply mod q
        // on the whole array? Then answer would be sum a[i] % q % p. But the sample outputs don't match that.
        // Let's check sample 2: n=3, k=2, p=10, q=20. a=[31,41,59].
        // a%p = [1,1,9] sum=11.
        // a%q%p = [31%20%10=11%10=1, 41%20%10=1%10=1, 59%20%10=19%10=9] sum=11. Same.
        // Sample 3: n=4, k=3, p=3, q=4. a=[1,2,3,4].
        // a%p = [1,2,0,1] sum=4.
        // a%q%p = [1%4%3=1, 2%4%3=2, 3%4%3=3%3=0, 4%4%3=0] sum=3.
        // Sample output is 3. So applying mod q on whole array gives 3, which matches.
        // Sample 4: n=6, k=4, p=9, q=20. a=[18,27,180,9,45,99].
        // a%p = [0,0,0,0,0,0] sum=0. Output 0. So no need for mod q.
        // Sample 5: n=7, k=4, p=3, q=5. a=[6,7,14,12,100,78,4].
        // a%p = [0,1,2,0,1,0,1] sum=5.
        // a%q%p = [6%5%3=1%3=1, 7%5%3=2%3=2, 14%5%3=4%3=1, 12%5%3=2%3=2, 100%5%3=0, 78%5%3=3%3=0, 4%5%3=4%3=1] sum=7? 1+2+1+2+0+0+1=7. That's worse than 5.
        // But output is 4. So we don't apply mod q on everything. We apply mod q selectively.
        // Let's compute gains: gain[i] = a[i]%p - a[i]%q%p.
        // a[1]=6: 6%3=0, 6%5%3=1%3=1, gain=0-1=-1? Wait, gain can be negative? Let's check: a[i]%p = 0, a[i]%q%p = 1. So 0 - 1 = -1.
        // But earlier I said a[i]%q%p <= a[i]%p. Is that always true? 6%3=0, 6%5=1, 1%3=1. So 1 > 0. So a[i]%q%p can be greater than a[i]%p!
        // My assumption was wrong. Because a[i]%q%p is not necessarily <= a[i]%p. Example: a=6, p=3, q=5. a%p=0, a%q=1, 1%3=1. So 1 > 0.
        // So gain[i] can be negative! That changes everything.
        // So applying mod q before mod p can actually increase the value for some elements.
        // Therefore, we cannot just apply mod q everywhere. We need to carefully choose which elements get mod q.
        // So the problem is: we can choose a set S (union of intervals of length >= k) to apply mod q, then apply mod p on everything.
        // Final value for i in S: a[i] % q % p. For i not in S: a[i] % p.
        // We want to minimize sum = sum_{i not in S} (a[i] % p) + sum_{i in S} (a[i] % q % p).
        // = sum_i (a[i] % p) + sum_{i in S} (a[i] % q % p - a[i] % p).
        // Let cost[i] = a[i] % q % p - a[i] % p. This can be positive, negative, or zero.
        // We want to choose S (union of intervals of length >= k) to minimize sum_{i in S} cost[i].
        // Equivalently, maximize sum_{i in S} (-cost[i]).
        // Let profit[i] = a[i] % p - a[i] % q % p. Then we want to maximize sum_{i in S} profit[i].
        // But profit[i] can be negative (which means cost[i] positive). So we want to include indices with positive profit,
        // and avoid indices with negative profit. But we are forced to include all indices in any interval we choose.
        // So we need to select a set of intervals of length >= k such that the sum of profits in their union is maximized.
        // This is a classic maximum weight subarray problem with length constraint, but we can take multiple intervals.
        // Since intervals can overlap, the union is just the set of indices covered. We can think of it as selecting
        // a set of disjoint intervals? Actually, if intervals overlap, their union is just a larger interval.
        // So we can assume without loss of generality that the chosen set S is a union of disjoint intervals,
        // each of length >= k. Because if two intervals overlap, we can merge them into one interval that covers the union,
        // and the length of the merged interval is at least max(lengths) >= k. So S is a union of disjoint intervals of length >= k.
        // So the problem reduces to: partition the array into segments, where we can choose some segments to be "active"
        // (i.e., we apply mod q on them). Each active segment must have length >= k. We want to maximize the sum of profits
        // in the active segments. The inactive segments get profit 0 (i.e., we don't include them).
        // This is a classic DP: dp[i] = maximum profit we can get from prefix up to i.
        // dp[i] = max(dp[i-1], max_{j <= i-k+1} (dp[j-1] + sum_{t=j}^i profit[t]) ).
        // But we can also have multiple active segments. So dp[i] = max(dp[i-1], max_{j: i-j+1 >= k} (dp[j-1] + prefix_profit[i] - prefix_profit[j-1]) ).
        // This can be optimized with a segment tree or by maintaining the maximum of (dp[j-1] - prefix_profit[j-1]) for j <= i-k+1.
        // Let pref[i] = sum_{t=1}^i profit[t].
        // Then for a segment [j, i] with length >= k, the gain is pref[i] - pref[j-1].
        // So dp[i] = max(dp[i-1], pref[i] + max_{j <= i-k+1} (dp[j-1] - pref[j-1]) ).
        // We can maintain the maximum of (dp[j-1] - pref[j-1]) as we iterate i.
        // Base: dp[0] = 0. For i < k, we cannot have an active segment ending at i, so dp[i] = dp[i-1] (which is 0).
        // But wait: we can also choose to not apply mod p on everything? Earlier we assumed we always apply mod p on the whole array.
        // But what if we don't apply mod p on some elements? Could that be better?
        // If we don't apply mod p on an element, its final value is either a[i] (if no mod q) or a[i] % q (if mod q).
        // But we can always apply mod p on it and get a[i] % p or a[i] % q % p. Is it possible that a[i] % p > a[i]?
        // No, because a[i] % p < p <= a[i]? Not necessarily, a[i] could be less than p. If a[i] < p, then a[i] % p = a[i].
        // So applying mod p doesn't change it. So it's neutral. If a[i] >= p, a[i] % p < p <= a[i], so it reduces.
        // So applying mod p never increases the value. So we should always apply mod p on everything.
        // What about a[i] % q? If we apply mod q but not mod p, we get a[i] % q. If we then apply mod p, we get a[i] % q % p.
        // Could a[i] % q % p > a[i] % q? Yes, as in the example a=6, p=3, q=5: a%q=1, a%q%p=1%3=1. They are equal.
        // Another example: a=7, p=3, q=5: a%q=2, a%q%p=2%3=2. Equal.
        // Can a%q%p > a%q? Since a%q < q, and we take % p, the result is < p. But a%q could be < p. So it's possible that a%q%p = a%q.
        // It cannot be strictly greater because modulo operation always returns a value less than the divisor, but the original value
        // might be less than the divisor, so it stays the same. So a%q%p <= a%q? Actually, if a%q < p,
