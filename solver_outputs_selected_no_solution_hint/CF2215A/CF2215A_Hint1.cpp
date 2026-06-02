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
        
        // We can apply mod p and mod q operations on intervals of length >= k.
        // The key insight: applying mod m on an interval is equivalent to reducing each element
        // to its remainder modulo m. Since we can apply operations multiple times,
        // we can effectively reduce any element to min(a[i] % p, a[i] % q) if we can cover it
        // with intervals of length >= k. But we can also combine operations.
        // Actually, we can think of it as: we can replace a[i] with a[i] % p or a[i] % q
        // if we can include it in an interval of length >= k. Since intervals can overlap,
        // we can process elements independently if we can cover them.
        // The problem reduces to: for each position i, we can either keep a[i] or reduce it
        // to a[i] % p or a[i] % q, but only if we can "activate" that position via an interval.
        // Since we can choose any intervals of length >= k, we can cover any set of positions
        // as long as we can partition the array into segments where we apply operations.
        // Actually, we can apply operations sequentially. The final value at position i
        // will be the result of applying some sequence of mod operations.
        // Note that mod p then mod q is not necessarily the same as mod q then mod p.
        // But we can choose the order. The minimum possible value for a[i] is min(a[i] % p, a[i] % q, a[i] % p % q, a[i] % q % p).
        // However, since p < q, a[i] % p % q = a[i] % p (because a[i] % p < p < q).
        // And a[i] % q % p could be smaller than a[i] % p. So the minimum is min(a[i] % p, a[i] % q % p).
        // But we can also apply mod p multiple times? No, mod p once is enough.
        // So for each element, the best we can achieve individually is min(a[i] % p, a[i] % q % p).
        // However, we are constrained by intervals of length >= k.
        // We can think of it as: we can choose a set of disjoint intervals of length >= k,
        // and for each interval we can apply a sequence of operations (mod p and mod q) to all elements in it.
        // But since operations can be applied in any order and overlapping, it's equivalent to:
        // we can mark some positions as "modifiable", and for those positions we can achieve the individual minimum.
        // The constraint is that the set of modifiable positions must be coverable by intervals of length >= k.
        // That means we cannot have a gap of unmodifiable positions of length < k between modifiable ones?
        // Actually, we can apply an operation on an interval, then another on a different interval.
        // The final value at a position depends on the last operation that covered it.
        // So we can independently decide for each position whether to apply mod p or mod q last,
        // but we must ensure that the positions where we apply a particular operation form intervals of length >= k.
        // This is equivalent to: we can partition the array into segments, and for each segment we can choose
        // to apply a sequence of operations. But since we can do multiple passes, we can achieve:
        // For each position, we can either leave it as is, or reduce it to a[i] % p, or reduce it to a[i] % q % p.
        // But we can only do this if we can cover the positions we want to modify with intervals of length >= k.
        // Since we can apply operations on any intervals, we can modify any position as long as we can
        // include it in some interval of length >= k. But if we want to modify a set of positions,
        // we can just take the whole array and apply mod p everywhere, then apply mod q on some intervals.
        // Actually, we can always apply mod p on the whole array if n >= k. If n < k, we can't do any operation.
        // Wait, the operation requires interval length >= k. So if n < k, we cannot perform any operation.
        // So answer is sum a[i].
        // If n >= k, we can apply mod p on the whole array, reducing all elements to a[i] % p.
        // Then we can apply mod q on intervals of length >= k to further reduce some elements to (a[i] % p) % q = a[i] % p (since p < q).
        // That doesn't help. But we could apply mod q first on some intervals, then mod p on the whole array.
        // If we apply mod q on an interval, those elements become a[i] % q. Then mod p on the whole array makes them (a[i] % q) % p.
        // So we can achieve for any position: either a[i] % p (if we never apply mod q on it) or (a[i] % q) % p (if we apply mod q on it before the final mod p).
        // But we can only apply mod q on intervals of length >= k.
        // So the problem reduces to: we will apply mod p on the entire array (since n >= k, we can do this at the end).
        // Before that, we can choose some intervals of length >= k to apply mod q.
        // The final value at position i is:
        // - if it's covered by at least one mod q interval: (a[i] % q) % p
        // - else: a[i] % p
        // We want to minimize the sum.
        // So we want to choose a set of intervals of length >= k to apply mod q, to minimize sum of (a[i] % q) % p for covered positions + a[i] % p for uncovered.
        // This is equivalent to: we start with all positions uncovered, sum = sum (a[i] % p).
        // For each position, if we cover it, the change in sum is delta[i] = (a[i] % q) % p - (a[i] % p).
        // Since (a[i] % q) % p <= a[i] % p (because a[i] % q < q, and mod p can only reduce or keep same),
        // delta[i] <= 0. So covering a position never increases the sum.
        // We want to cover as many positions as possible with intervals of length >= k, but we can only cover positions if we can form intervals of length >= k.
        // However, we can cover any set of positions as long as the uncovered positions do not contain a gap of length < k?
        // Actually, we can apply multiple mod q intervals. They can overlap. The covered positions are the union of these intervals.
        // The union of intervals of length >= k can be any set that doesn't have a gap of length < k between covered segments?
        // Let's think: if we have a covered segment of length L, we can cover it with intervals of length >= k as long as L >= k.
        // If we have two covered segments separated by a gap of length d, we can cover them separately if each segment length >= k.
        // But if a segment length < k, we cannot cover it alone. However, we could extend an interval to include it, but that would also cover the gap.
        // So the constraint is: every maximal contiguous block of covered positions must have length >= k.
        // Because if we have a covered block of length < k, it must be part of a larger interval that also covers some uncovered positions, making them covered.
        // So we cannot have a covered block of length < k.
        // Therefore, we can choose any set of positions to cover, as long as every maximal contiguous covered segment has length >= k.
        // Equivalently, we cannot have a covered segment of length < k.
        // So we want to select a set of disjoint intervals of length >= k to cover, and we can also merge them if they are close, but merging might cover unwanted positions (which is fine since delta <= 0, covering more never hurts).
        // Actually, if delta[i] <= 0, covering extra positions is always beneficial or neutral. So we would want to cover as much as possible.
        // The only restriction is that we cannot cover a segment of length < k. So if we want to cover a position, we must cover a whole interval of length at least k containing it.
        // But we can cover the entire array if n >= k. So if n >= k, we can just cover everything by applying mod q on the whole array, then mod p on the whole array.
        // Then every position gets (a[i] % q) % p. Is that always optimal?
        // Since delta[i] <= 0, covering all positions gives sum = sum (a[i] % q) % p.
        // But wait, is it always true that (a[i] % q) % p <= a[i] % p? Yes, because x % p is non-increasing in x? Not necessarily monotonic, but (a % q) % p <= a % p?
        // Let's check: a % q is in [0, q-1]. Then (a % q) % p is in [0, p-1]. a % p is in [0, p-1]. Could (a % q) % p be greater than a % p?
        // Example: a=10, p=3, q=4. a%p=1, a%q=2, (a%q)%p=2. 2 > 1. So delta can be positive!
        // So covering a position might increase the sum. So we cannot just cover everything.
        // We need to carefully choose which positions to cover with mod q.
        // So the problem is: we can apply mod p on the whole array at the end. Before that, we can apply mod q on some intervals of length >= k.
        // The final value at position i is:
        // - if covered by mod q: (a[i] % q) % p
        // - else: a[i] % p
        // We want to minimize sum.
        // Let base = sum (a[i] % p). For each position, let gain[i] = (a[i] % p) - ((a[i] % q) % p). This is the amount we reduce the sum by covering i.
        // gain[i] can be positive, negative, or zero. We want to maximize total gain, subject to the constraint that the set of covered positions can be formed by intervals of length >= k.
        // Since we can apply multiple mod q intervals, the covered set is a union of intervals of length >= k. But note that if we have two intervals that are close, we could merge them into one larger interval, covering the gap. The gap positions would contribute their gain (which could be negative). So we only want to merge if the total gain of the gap is positive? Actually, we are not forced to merge. We can keep them separate. But if we keep them separate, the gap is uncovered. If we merge, we cover the gap. So we have the choice.
        // This is equivalent to: we can partition the array into segments, and for each segment we can either leave it uncovered, or cover it entirely with mod q, but the covered segments must have length >= k.
        // Wait, can we cover a segment of length >= k but leave some positions inside uncovered? No, because mod q is applied to the whole interval. So if we apply mod q on an interval, all positions in that interval become covered. So covered positions come in intervals of length >= k. But we can have multiple overlapping intervals, resulting in a covered set that is a union of intervals. However, any union of intervals of length >= k can be represented as a set of disjoint covered intervals, each of length >= k? Not necessarily: if we have [1,3] and [2,4] with k=3, the union is [1,4] length 4 >=3. So the union is a single interval of length >= k. In general, the union of intervals of length >= k will be a set of disjoint intervals, each of length >= k, except possibly if we have intervals that overlap but their union is still >= k. But we can always merge overlapping intervals into one. So the covered set is a collection of disjoint intervals, each of length >= k.
        // So we need to choose a set of disjoint intervals of length >= k to cover, maximizing the sum of gain[i] for i in these intervals.
        // This is a classic DP: dp[i] = maximum total gain considering prefix up to i.
        // dp[i] = max(dp[i-1], max_{j <= i-k+1} (dp[j-1] + sum_{t=j}^i gain[t]))
        // But we can also choose to cover an interval of length exactly L >= k. So we can compute prefix sums of gain.
        // Let pref[i] = sum_{t=1}^i gain[t].
        // Then for an interval [j, i] with length >= k, the gain is pref[i] - pref[j-1].
        // dp[i] = max(dp[i-1], max_{j: i-j+1 >= k} (dp[j-1] + pref[i] - pref[j-1]))
        // = max(dp[i-1], pref[i] + max_{j <= i-k+1} (dp[j-1] - pref[j-1]))
        // So we can maintain the maximum of (dp[j-1] - pref[j-1]) for j from 1 to i-k+1.
        // This is O(n) per test case.
        // But wait: we also have the option to not apply mod p at the end? The problem says we can perform operations an arbitrary number of times. We are not forced to apply mod p on the whole array. But we can always apply mod p on the whole array if n >= k. Is it always beneficial to do so? Applying mod p on a position changes a[i] to a[i] % p. Since a[i] % p <= a[i], it never increases the value. So we should always apply mod p on the whole array at the end if possible. If n < k, we cannot apply any operation, so we just take original a[i]. But wait, if n < k, we cannot even apply mod p on the whole array because interval length must be >= k. So we are stuck with original values.
        // So the algorithm:
        // If n < k: answer = sum a[i].
        // Else:
        // Compute base = sum (a[i] % p).
        // Compute gain[i] = (a[i] % p) - ((a[i] % q) % p).
        // Then find max total gain by selecting disjoint intervals of length >= k.
        // Answer = base - max_gain.
        // But is it always optimal to apply mod p on everything at the end? What if we apply mod q on some interval, then mod p on some other interval, but not on the whole array? Since we can apply mod p on any interval of length >= k, we could apply mod p on the whole array if n >= k. So we can always achieve the state where every position is either a[i] % p or (a[i] % q) % p. Could there be a better state where some positions are not modded by p? For example, a[i] % q might be smaller than both a[i] % p and (a[i] % q) % p? But a[i] % q is in [0, q-1]. Since p < q, a[i] % q could be smaller than a[i] % p? Actually, a[i] % q can be anything up to q-1, while a[i] % p is up to p-1. So a[i] % q could be larger than a[i] % p. But could it be smaller? If a[i] < p, then a[i] % p = a[i], and a[i] % q = a[i] (since a[i] < p < q). So they are equal. If a[i] >= p, a[i] % p < p, while a[i] % q could be anything. It's possible that a[i] % q < a[i] % p. For example, a=5, p=3, q=4. a%p=2, a%q=1. So 1 < 2. So leaving it as a[i] % q (without mod p) is better than mod p. But can we achieve that? We could apply mod q on that position and never apply mod p on it. But we can only apply mod p on intervals of length >= k. If we want to apply mod p on some positions but not others, we need to cover the ones we want to mod p with intervals of length >= k, and leave the others uncovered by mod p. But we can also apply mod p on the whole array, which would ruin the a[i] % q values. So maybe we don't apply mod p on the whole array. Instead, we apply mod q on some intervals, and mod p on some other intervals. The final value at a position depends on the last operation applied to it. So we can have three types of positions:
        // - Never modded: a[i]
        // - Last modded by q: a[i] % q
        // - Last modded by p: a[i] % p
        // - Last modded by q then p: (a[i] % q) % p
        // - Last modded by p then q: (a[i] % p) % q
        // But since we can choose the order, we can achieve any of these as long as we can cover the positions with appropriate intervals.
        // This seems more complex. However, note that we can always apply mod p on an interval, then mod q on a subinterval, etc.
        // Let's think about the optimal final value for a single element if we could choose independently: min(a[i], a[i]%p, a[i]%q, (a[i]%p)%q, (a[i]%q)%p).
        // Since p < q, (a[i]%p)%q = a[i]%p (because a[i]%p < p < q). And (a[i]%q)%p is as before.
        // So the candidates are: a[i], a[i]%p, a[i]%q, (a[i]%q)%p.
        // We want to minimize the sum. But we have the interval length constraint.
        // Is it ever beneficial to leave a[i] unmodded? Only if a[i] < a[i]%p and a[i] < a[i]%q, etc. But a[i]%p <= a[i] always, and a[i]%q <= a[i] always. So modding never increases the value. So we always want to mod every element at least once. So we should apply at least one mod operation to every element. Since we can apply mod p on the whole array (if n>=k), we can reduce all to a[i]%p. So base is a[i]%p. Then we can consider further reducing some by applying mod q before the mod p, or applying mod q after mod p? If we apply mod q after mod p, we get (a[i]%p)%q = a[i]%p (since p<q). So no change. So the only way to get something smaller than a[i]%p is to apply mod q first, then mod p, getting (a[i]%q)%p. Or apply mod q and never mod p, getting a[i]%q. But a[i]%q could be smaller than (a[i]%q)%p? Actually, (a[i]%q)%p <= a[i]%q, so mod p after q never hurts. So the best we can do for a position is min(a[i]%p, (a[i]%q)%p). But wait, what if we apply mod q and stop? Then we get a[i]%q. But we could also apply mod p on other positions. If we apply mod p on some interval that includes this position, it becomes (a[i]%q)%p. If we don't include it, it stays a[i]%q. So we have a choice: either we apply mod p on this position or not. If we apply mod p, we get (a[i]%q)%p. If we don't, we get a[i]%q. Which is smaller? It could be that a[i]%q < (a[i]%q)%p? No, because (x % p) <= x for any x. So (a[i]%q)%p <= a[i]%q. So applying mod p is always at least as good. So we should always apply mod p if possible. Therefore, the optimal final value for a position is either a[i]%p (if we never apply mod q to it) or (a[i]%q)%p (if we apply mod q before the final mod p). So our initial reduction is correct: we will apply mod p on the whole array at the end, and we can choose to apply mod q on some intervals before that.
        // But wait: what if n >= k, but we don't apply mod p on the whole array? Could we get a[i]%q on some positions and a[i]%p on others, without the final mod p on the q-modded ones? As argued, applying mod p on a q-modded position only reduces it further or keeps it same. So we might as well apply mod p on it. The only reason not to would be if we cannot apply mod p on that position without also applying it on others where it would increase the value? But mod p never increases the value. So we can safely apply mod p everywhere. So the final mod p on the whole array is always optimal.
        // Therefore, the problem is exactly as modeled: base = sum (a[i] % p), gain[i] = (a[i] % p) - ((a[i] % q) % p). Maximize sum of gain over covered intervals of length >= k.
        // But wait: is it always possible to apply mod p on the whole array? Yes, if n >= k, we can choose [1, n] and m = p. So we can always do that as the last operation.
        // So the DP is correct.
        
        if (n < k) {
            long long sum = 0;
            for (long long x : a) sum += x;
            cout << sum << '\n';
            continue;
        }
        
        vector<long long> base(n), gain(n);
        long long total_base = 0;
        for (int i = 0; i < n; ++i) {
            base[i] = a[i] % p;
            total_base += base[i];
            long long after_q = a[i] % q;
            long long after_q_p = after_q % p;
            gain[i] = base[i] - after_q_p; // reduction if we cover i with mod q before mod p
        }
        
        // dp[i] = max gain for prefix up to i (0-indexed)
        vector<long long> dp(n, 0);
        vector<long long> pref_gain(n + 1, 0);
        for (int i = 0; i < n; ++i) {
            pref_gain[i+1] = pref_gain[i] + gain[i];
        }
        
        // We need max_{j <= i-k+1} (dp[j-1] - pref_gain[j]) for 1-indexed j.
        // Let's use 1-indexed for DP to simplify.
        // dp[i] = max gain for first i elements.
        vector<long long> dp2(n + 1, 0);
        long long best = -1e18;
        // best will maintain max_{j <= i-k+1} (dp2[j-1] - pref_gain[j])
        // We need to handle j from 1 to i-k+1.
        // Initialize best for the first possible interval.
        // For i from 1 to n:
        // dp2[i] = dp2[i-1]
        // if i >= k:
        //   update best with j = i-k+1: candidate = dp2[(i-k+1)-1] - pref_gain[i-k+1] = dp2[i-k] - pref_gain[i-k+1]
        //   dp2[i] = max(dp2[i], pref_gain[i] + best)
        
        for (int i = 1; i <= n; ++i) {
            dp2[i] = dp2[i-1];
            if (i >= k) {
                int j = i - k + 1;
                long long cand = dp2[j-1] - pref_gain[j];
                if (cand > best) best = cand;
                dp2[i] = max(dp2[i], pref_gain[i] + best);
            }
        }
        
        long long max_gain = dp2[n];
        long long ans = total_base - max_gain;
        cout << ans << '\n';
    }
    
    return 0;
}
