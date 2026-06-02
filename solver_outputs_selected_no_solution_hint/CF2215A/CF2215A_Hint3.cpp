#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        int n, k;
        ll p, q;
        cin >> n >> k >> p >> q;
        vector<ll> a(n);
        for (int i = 0; i < n; ++i) cin >> a[i];
        if (q % p == 0) {
            // p divides q, so we can only use p effectively
            // we can reduce each element to a[i] % p
            // but we can only apply to intervals of length >= k
            // we want to minimize sum, so we want to reduce as many as possible
            // we can reduce an element if we can cover it with an interval of length >= k
            // we can always reduce all elements if n >= k, else none
            if (n >= k) {
                ll sum = 0;
                for (ll x : a) sum += x % p;
                cout << sum << '\n';
            } else {
                ll sum = 0;
                for (ll x : a) sum += x;
                cout << sum << '\n';
            }
            continue;
        }
        // general case: p and q, p < q, q % p != 0
        // we can reduce numbers using mod p or mod q
        // key insight: we can reduce any number to min(x % p, x % q) if we can cover it
        // but we can also chain operations: mod q then mod p can give x % p
        // actually, we can achieve x % p for any element if we can apply mod p on an interval containing it
        // similarly for mod q
        // but we can also do mod q then mod p on overlapping intervals to get x % p
        // the optimal value for a single element is min(x % p, x % q)
        // we want to achieve this for as many elements as possible
        // we can apply operations on intervals of length >= k
        // we can think of it as: we can "mark" some elements to be reduced to their optimal value
        // but we need to cover them with intervals of length >= k using the appropriate modulus
        // actually, we can always achieve the optimal value for an element if we can cover it with an interval of length >= k
        // because we can just apply the operation with the modulus that gives the minimum
        // but wait: if we want to use mod p on some elements and mod q on others, we might need separate intervals
        // however, we can apply multiple operations, so we can cover different groups with different intervals
        // the only restriction is that each operation requires an interval of length >= k
        // so we can reduce any subset of elements as long as we can partition them into groups, each group contained in some interval of length >= k
        // but we can also overlap intervals arbitrarily
        // the problem reduces to: we have n positions, we can choose to reduce each position to min(a[i]%p, a[i]%q) if we can "cover" it
        // but we can always cover any position by an interval of length >= k that includes it, as long as n >= k
        // if n < k, we cannot do any operation, so answer is sum of a[i]
        // if n >= k, we can reduce all elements? Not necessarily, because we might need to use different moduli
        // but we can just apply mod p on the whole array (if n >= k) to get all to a[i]%p
        // then we might want to further reduce some to a[i]%q if that is smaller
        // but mod q after mod p: (a[i]%p)%q = a[i]%p if a[i]%p < q, which is true since p < q
        // so mod q after mod p doesn't change anything
        // we can also do mod q first then mod p: (a[i]%q)%p = a[i]%p if a[i]%q >= p? Actually, (x % q) % p = x % p if q is multiple of p, but here q % p != 0
        // in general, (x % q) % p is not necessarily x % p
        // we need to think differently
        // Hint 1: answer doesn't change if we replace a with floor(a/m) for some fixed m? That seems for a different problem?
        // Actually, the hints might be for a different version. Let's think.
        // We have two moduli. We can apply them in any order on overlapping intervals.
        // Notice that if we apply mod p on an interval, all elements become < p.
        // Then if we apply mod q on an interval, since p < q, elements < p remain unchanged.
        // So mod q after mod p does nothing.
        // If we apply mod q first, elements become < q. Then mod p can reduce them further.
        // So the only useful sequence is mod q then mod p on some elements.
        // But we can also just apply mod p directly.
        // So for each element, the possible values we can achieve are: original, a[i]%p, a[i]%q, (a[i]%q)%p.
        // But (a[i]%q)%p is just a[i]%p if a[i]%q >= p? Not necessarily.
        // Actually, (x % q) % p = x % p if q is a multiple of p. Since q % p != 0, it's not.
        // However, we can achieve a[i]%p by just applying mod p on an interval containing i.
        // We can achieve a[i]%q by applying mod q.
        // Can we achieve something smaller than both? Maybe by applying mod q then mod p on the same element?
        // But mod p after mod q: if we apply mod q on interval A, then mod p on interval B that overlaps A, the elements in intersection get (a[i]%q)%p.
        // Is (a[i]%q)%p ever smaller than min(a[i]%p, a[i]%q)? 
        // Since a[i]%q < q, and p < q, (a[i]%q)%p is just a[i]%q if a[i]%q < p, else a[i]%q - p.
        // So it could be smaller than a[i]%p? For example, a[i]=10, p=3, q=4: a[i]%p=1, a[i]%q=2, (a[i]%q)%p=2%3=2. Not smaller.
        // a[i]=7, p=3, q=5: a[i]%p=1, a[i]%q=2, (2)%3=2.
        // a[i]=8, p=3, q=5: 8%3=2, 8%5=3, 3%3=0. So we can get 0, which is smaller than both 2 and 3!
        // So we can achieve values smaller than min(a[i]%p, a[i]%q) by doing mod q then mod p.
        // In fact, we can achieve a[i] % gcd(p, q)? Let's check: 8 % gcd(3,5)=8%1=0. Yes.
        // Actually, by Chinese Remainder Theorem, we can achieve a[i] % gcd(p, q) if we can apply both moduli.
        // More generally, we can achieve a[i] % g where g = gcd(p, q). Because we can reduce by p and q repeatedly.
        // But we are limited to intervals of length >= k.
        // So the problem is: we can apply mod p and mod q on intervals of length >= k. We want to minimize sum.
        // The optimal value we can achieve for an element is a[i] % g, where g = gcd(p, q).
        // Is that always achievable? We need to apply both moduli on the element, possibly in different operations.
        // We can apply mod q on an interval containing i, then mod p on an interval containing i.
        // Both intervals must have length >= k.
        // So if we can cover i with two intervals of length >= k (they can be the same or different), we can achieve a[i] % g.
        // If we can only cover it with one interval, we can only apply one modulus, so we get min(a[i]%p, a[i]%q).
        // If we cannot cover it with any interval of length >= k, we get a[i].
        // But wait: can we achieve a[i] % g with just one modulus if g = p? That happens when p divides q, which we handled separately.
        // In general case q % p != 0, g < p. So we need both moduli.
        // So the problem reduces to: we have n positions. We can choose to apply operations to intervals of length >= k.
        // Each operation applies a modulus (p or q) to all elements in the interval.
        // We want to assign to each position a final value: either a[i], a[i]%p, a[i]%q, or a[i]%g.
        // But we can also achieve a[i]%p by mod p, a[i]%q by mod q, and a[i]%g by both.
        // Actually, if we apply mod p, we get a[i]%p. If we then apply mod q, it doesn't change because a[i]%p < p < q.
        // So mod p then mod q gives a[i]%p.
        // If we apply mod q then mod p, we get (a[i]%q)%p. This is not necessarily a[i]%g.
        // But we can apply mod q, then mod p, then mod q, then mod p... eventually we can reach a[i] % g?
        // By Euclidean algorithm, we can reduce a number by repeatedly taking mod p and mod q. Since we can do operations in any order, we can simulate the Euclidean algorithm on each element if we can apply both moduli enough times.
        // However, we are limited by the number of operations? No, arbitrary number of times.
        // But each operation must be on an interval of length >= k.
        // If we can cover an element with at least one interval of length >= k for mod p and at least one for mod q (they can be the same interval if we apply both moduli on it? But one operation only applies one modulus. So we need two operations on that element, each with a different modulus, both on intervals of length >= k containing the element.
        // So an element can be reduced to a[i] % g if and only if there exist two intervals of length >= k containing it (they can overlap, even be the same interval used twice with different moduli).
        // Since we can apply operations arbitrarily many times, we can just use the whole array as the interval for mod q, then the whole array for mod p, provided n >= k.
        // If n >= k, we can apply mod q on [1, n] and mod p on [1, n]. Then every element gets both moduli, so we can achieve a[i] % g for all i.
        // Wait, is that true? Let's test: apply mod q on [1,n]: a[i] becomes a[i]%q. Then apply mod p on [1,n]: a[i] becomes (a[i]%q)%p. Is that equal to a[i]%g?
        // Not necessarily. For example, p=4, q=6, g=2. a[i]=7. 7%6=1, 1%4=1. But 7%2=1. Works.
        // a[i]=11: 11%6=5, 5%4=1. 11%2=1. Works.
        // a[i]=9: 9%6=3, 3%4=3. 9%2=1. Not equal! 3 != 1.
        // So (a[i]%q)%p is not always a[i]%g.
        // But we can do more operations: after mod p, we can do mod q again, etc.
        // Since we can do arbitrary number of operations, we can simulate the Euclidean algorithm on the whole array.
        // For a single element x, we can reduce it to x % g by repeatedly doing x = x % p, then x = x % q, etc.
        // But we are doing operations on intervals, so all elements in the interval undergo the same modulus operation.
        // If we apply mod p on the whole array, then mod q on the whole array, then mod p again, etc., we are effectively applying the Euclidean algorithm to each element individually, but the sequence of moduli is the same for all elements.
        // However, the Euclidean algorithm for two numbers p and q to find gcd doesn't depend on the starting value x; the sequence of moduli is fixed: it's the sequence of remainders in the Euclidean algorithm for p and q.
        // For example, to compute gcd(p,q), we do: p, q, p%q, q%(p%q), ... until 0.
        // If we apply mod p, then mod q, then mod p, then mod q... we are essentially doing: x -> x%p -> (x%p)%q -> ((x%p)%q)%p -> ...
        // This is not the same as the Euclidean algorithm on x and p,q.
        // Actually, we want to reduce x modulo g. We can do: x % p, then (x%p) % q, etc. But eventually we will reach a value < g? Not necessarily.
        // Let's think differently. The set of achievable values for an element is the set of numbers that can be obtained by starting from a[i] and repeatedly replacing the current value v by v % p or v % q, provided that each operation is applied to an interval containing the element.
        // Since we can choose intervals arbitrarily, we can apply different sequences to different elements? No, because an operation applies to a contiguous interval, so all elements in that interval get the same modulus. But we can cover different elements with different intervals. So we can apply different sequences to different elements, as long as for each operation, the interval has length >= k.
        // So for each element, we can independently choose a sequence of moduli (p or q) to apply, as long as we can cover that element with intervals of length >= k for each step.
        // But we can always cover an element with an interval of length >= k if n >= k, by just taking the whole array or a large enough subarray containing it.
        // So if n >= k, we can apply any sequence of moduli to any element independently? Not exactly, because operations are global in the sense that when we apply an operation on an interval, it affects all elements in that interval. But we can just apply operations on intervals that contain only the elements we want to affect? We can choose intervals that exactly cover the elements we want, but they must have length >= k. So if we want to apply a modulus to a specific element, we must also apply it to at least k-1 other elements in the same interval.
        // This means we cannot treat elements completely independently; there is a coupling.
        // However, we can always apply an operation to the whole array if n >= k. So we can apply the same sequence to all elements.
        // Can we achieve a[i] % g for all i by applying a fixed sequence of moduli to the whole array?
        // Let's test with p=4, q=6, g=2. Sequence: mod q, mod p, mod q, mod p...
        // Start with x.
        // After mod q: x%6
        // After mod p: (x%6)%4
        // After mod q: ((x%6)%4)%6. Since (x%6)%4 < 4 < 6, this is just (x%6)%4.
        // So it stabilizes at (x%6)%4.
        // Is (x%6)%4 always a multiple of g=2? Yes, because 4 and 6 are multiples of 2, so any combination of mods will preserve multiples of 2. But is it equal to x%2? Not necessarily, as we saw with 9: 9%6=3, 3%4=3, but 9%2=1. So we get 3 instead of 1.
        // Can we get 1? To get 1, we need to apply mod 2, but we don't have mod 2. We only have mod 4 and mod 6.
        // Actually, we can achieve x % 2 by doing mod 4 then mod 6? No.
        // Wait, the problem allows us to choose m from {p, q}. We don't have mod g.
        // So maybe we cannot always achieve x % g.
        // Let's re-evaluate. The hints say: "What happens if p divides q?" That suggests the case p|q is special.
        // In the general case, maybe we can only achieve min(x%p, x%q) for elements we can cover, and we can cover at most something?
        // Let's look at the sample 4: n=6, k=4, p=9, q=20. a = [18,27,180,9,45,99]. Output: 0.
        // Can we achieve 0 for all? Let's check: p=9, q=20, gcd=1. So x%1=0 for all. If we could achieve x%g, sum would be 0. Output is 0, so we achieved 0.
        // How? We can reduce each to 0? 18%9=0, 27%9=0? 27%9=0, 180%9=0, 9%9=0, 45%9=0, 99%9=0. So just apply mod 9 on the whole array (length 6 >= 4). That gives all zeros. So we didn't need mod 20.
        // Sample 5: n=7, k=4, p=3, q=5. a = [6,7,14,12,100,78,4]. Output: 4.
        // Let's compute min(x%3, x%5) for each:
        // 6: 0, 1 -> min 0
        // 7: 1, 2 -> 1
        // 14: 2, 4 -> 2
        // 12: 0, 2 -> 0
        // 100: 1, 0 -> 0
        // 78: 0, 3 -> 0
        // 4: 1, 4 -> 1
        // Sum of mins = 0+1+2+0+0+0+1 = 4. Output is 4. So we achieved the min for each element.
        // Can we achieve better? For 14, min is 2. Can we get 14%gcd(3,5)=14%1=0? If we could, sum would be smaller. But output is 4, so we cannot get 0 for 14.
        // So in this case, we cannot achieve x%g for all. We only achieved min(x%p, x%q).
        // Why? Because to get 0 for 14, we would need to apply mod 3 and mod 5 in some order. 14%5=4, 4%3=1. 14%3=2, 2%5=2. So we can't get 0. Actually, 14 mod 3 is 2, mod 5 is 4. No sequence of mod 3 and mod 5 can reduce 14 to 0 because 14 is not a multiple of 3 or 5, and the operations only take mod. You can never increase a number. The only way to get 0 is if at some point the number becomes a multiple of the modulus you apply. Starting from 14, applying mod 3 gives 2, mod 5 gives 4. From 2, mod 5 gives 2; from 4, mod 3 gives 1. From 1, mod 3 gives 1, mod 5 gives 1. So we can never reach 0. So the achievable values for 14 are {14, 2, 4, 1}. The minimum is 1? Wait, 1 is achievable? 14 -> mod 5 -> 4 -> mod 3 -> 1. Yes! So we can get 1 for 14. But the output sum is 4, and 14's contribution in the min sum is 2. Why didn't we get 1? Because to get 1 for 14, we need to apply mod 5 then mod 3 on 14. But we have to do it in an interval of length >= k=4. If we apply mod 5 on an interval containing 14, we also affect other elements. Then mod 3 on an interval containing 14 affects others. The other elements might not benefit from this sequence; they might end up larger than their min. So the coupling prevents us from achieving the absolute minimum for each element independently.
        // So the problem is about choosing a set of intervals to apply mod p and mod q to minimize the total sum, with the constraint that each interval has length >= k.
        // This looks like a DP or greedy problem.
        // Let's think about the effect of operations. Each operation on [l,r] with modulus m sets a_i = a_i % m for i in [l,r].
        // Since a_i % m < m, after a mod p operation, all elements in the interval become < p.
        // After a mod q operation, they become < q.
        // If we apply mod p after mod q, elements that were < q become < p (if they were >= p) or stay the same.
        // Notice that if we apply mod p on an interval, any previous mod q on that interval is essentially overwritten (unless the value was already < p, in which case mod p does nothing).
        // So the final value of an element depends on the last operation that actually changed it.
        // Actually, if we apply a sequence of operations, the final value is the result of applying the moduli in order. But since mod p makes everything < p, and p < q, any subsequent mod q will not change anything. So the only way mod q has an effect is if it is applied after mod p? No, if mod q is applied first, it makes things < q. Then mod p can reduce things further. If mod p is applied first, things become < p, then mod q does nothing. So the effective order is: we can have some mod q operations, then some mod p operations. Mod p operations after mod q can reduce values further. We can also have multiple mod q and mod p interleaved, but once mod p is applied, mod q becomes useless. So the final value of an element is determined by the last mod p operation that covers it, and possibly some mod q operations before that.
        // But we can also have mod q operations after the last mod p? They wouldn't change anything.
        // So we can think of the process as: we choose a set of disjoint intervals? Not necessarily disjoint.
        // Let's consider the effect of mod p. If we apply mod p on an interval, all elements in that interval become a_i % p. This is a very strong reduction. If we can apply mod p on the whole array, we get sum of (a_i % p). But we might do better by applying mod q on some parts before mod p, to reduce some a_i % p further? Actually, (a_i % q) % p can be smaller than a_i % p, as we saw with 8: 8%5=3, 3%3=0 < 8%3=2.
        // So we want to apply mod q on some elements before applying mod p, to make their mod p result smaller.
        // But applying mod q on an interval of length >= k might force us to also apply it on elements where it is not beneficial.
        // So we need to choose intervals for mod q (before the final mod p) to minimize the final sum.
        // Since we can do mod p at the very end on the whole array (if n >= k), we can assume we always do a final mod p on everything. Then the problem reduces to: we can apply mod q on some intervals of length >= k (before the final mod p) to change the initial values for the final mod p. The final value of an element is (a_i' % p), where a_i' is either a_i or a_i % q if it was covered by some mod q operation.
        // But wait, we can also apply mod q multiple times? Applying mod q twice is the same as once. So we just need to decide which elements get mod q before the final mod p.
        // However, we can also apply mod p on some intervals before the final mod p? That would just be redundant if we do a final mod p everywhere.
        // So the optimal strategy: choose a set of intervals of length >= k to apply mod q. Then apply mod p on the whole array (if n >= k). The final value for element i is:
        // if i is covered by at least one mod q interval: (a_i % q) % p
        // else: a_i % p
        // Is that always achievable? We need to ensure that the mod q intervals are applied before the final mod p. We can just do all mod q operations first, then one mod p on [1,n]. Since n >= k, we can do that.
        // But what if n < k? Then we can't do any operation, answer is sum a_i.
        // So for n >= k, the problem becomes: we have an array a. We can choose any number of intervals of length >= k. For each chosen interval, we apply mod q to its elements (this replaces a_i with a_i % q). Then we take the sum over i of (a_i % p). We want to minimize this sum.
        // But wait, is it always optimal to do a final mod p on the whole array? What if we don't do mod p on some elements? Could that be better? If we don't apply mod p on an element, its value remains whatever it was after mod q (or original). But since p < q, a_i % p <= a_i % q? Not necessarily: a_i % p could be larger than a_i % q? No, because a_i % p < p <= q, and a_i % q < q. But a_i % p could be larger than a_i % q? Example: p=3, q=5, a_i=4: a_i%3=1, a_i%5=4. So 1 < 4. a_i=7: 7%3=1, 7%5=2. 1 < 2. a_i=2: 2%3=2, 2%5=2. Equal. a_i=8: 8%3=2, 8%5=3. 2 < 3. So a_i % p is always <= a_i % q? Let's check: a_i % p < p <= q. a_i % q < q. But a_i % p could be greater than a_i % q if a_i % q < p? Example: p=5, q=7, a_i=6: 6%5=1, 6%7=6. 1 < 6. a_i=3: 3%5=3, 3%7=3. a_i=4: 4%5=4, 4%7=4. a_i=2: 2%5=2, 2%7=2. So a_i % p is always <= a_i % q? Actually, since p < q, the possible values of a_i % p are in [0, p-1], and a_i % q are in [0, q-1]. It's possible that a_i % q < a_i % p? For that, we need a_i % q < a_i % p. Since a_i % p < p, we need a_i % q < p. But a_i % q can be anything from 0 to q-1. So yes, it can be smaller than a_i % p. Example: p=4, q=5, a_i=6: 6%4=2, 6%5=1. So 1 < 2. So a_i % q can be smaller than a_i % p. So maybe we don't want to apply mod p on some elements? But if we apply mod p, we get a_i % p. If we don't apply mod p, we get either a_i or a_i % q. Could a_i % q be smaller than a_i % p? Yes, as above. So we might want to leave some elements with just mod q and not apply mod p.
        // But we can apply mod p on some intervals and not others. So the final value of an element is the result of the last operation applied to it. If the last operation is mod p, value is (previous value) % p. If the last operation is mod q, value is (previous value) % q. If no operation, value is a_i.
        // Since we can do operations in any order, we can decide for each element what its final operation is, subject to the constraint that operations are applied on intervals of length >= k.
        // This is more complex.
        // Let's reconsider the hints. Hint 1: "Can you show that the answer doesn't change if we replace a with floor(a/m) for some fixed m?" That suggests we can divide by something? Maybe we can replace a_i with a_i % something?
        // Hint 2: "How does the answer change when k is increased?" Probably the answer is non-decreasing with k.
        // Hint 3: "What happens if p divides q?" We already handled that: then we can only use p effectively, and we can reduce all if n >= k.
        // For the general case, maybe there is a known solution: we can always achieve a_i % p for all i if n >= k? But sample 5 output is 4, and sum of a_i % p = 6%3=0, 7%3=1, 14%3=2, 12%3=0, 100%3=1, 78%3=0, 4%3=1 -> sum = 5. But output is 4, which is less than 5. So we did better than just mod p on everything.
        // How did we get 4? By using mod q on some elements. In sample 5, we got min(x%p, x%q) for each? Sum of mins is 4. So we achieved the element-wise minimum of the two moduli.
        // Can we always achieve the element-wise minimum if n >= k? In sample 5, n=7, k=4. We achieved it. How? We need to apply mod q on elements where a_i % q < a_i % p, and mod p on the others. But we have to do it with intervals of length >= k.
        // Let's see which elements prefer mod q: 
        // 6: p=0, q=1 -> p better
        // 7: p=1, q=2 -> p better
        // 14: p=2, q=4 -> p better
        // 12: p=0, q=2 -> p better
        // 100: p=1, q=0 -> q better
        // 78: p=0, q=3 -> p better
        // 4: p=1, q=4 -> p better
        // So only 100 prefers mod q. We need to apply mod q on an interval of length >= 4 that contains index 5 (1-based). We can choose [2,5] or [3,6] or [4,7] etc. If we apply mod q on [4,7], we affect 12, 100, 78, 4. For 12: 12%5=2, which is worse than 12%3=0. For 78: 78%5=3, worse than 0. For 4: 4%5=4, worse than 1. So applying mod q on [4,7] would increase the sum for those elements. But we can then apply mod p on them to bring them back down? If we apply mod p after mod q on those, we get (a_i % q) % p. For 12: (12%5)%3 = 2%3 = 2, still worse than 0. For 78: 3%3=0, which is equal to 78%3=0. For 4: 4%3=1, equal to 4%3=1. So 12 becomes 2 instead of 0, which is bad. So we don't want to include 12 in the mod q interval.
        // To only affect 100, we need an interval of length >= 4 that contains 100 but not 12? The array is 0-indexed: [6,7,14,12,100,78,4]. 100 is at index 4. Intervals of length 4 containing index 4: [1,4], [2,5], [3,6], [4,7]. All of them include either 12 (index 3) or 78 (index 5) or both. So we cannot isolate 100. So how did the sample achieve sum 4? Maybe they used a different sequence.
        // Let's look at the note for sample 3: n=4, k=3, p=3, q=4. a=[1,2,3,4]. Output 3.
        // They did: [1,4] mod 4 -> [1,2,3,0]; then [2,4] mod 3 -> [1,2,0,0]. Sum=3.
        // Here, min(x%3, x%4): 1%3=1, 1%4=1 -> min 1; 2%3=2, 2%4=2 -> 2; 3%3=0, 3%4=3 -> 0; 4%3=1, 4%4=0 -> 0. Sum of mins = 1+2+0+0=3. They achieved it.
        // How? They applied mod 4 on whole array, then mod 3 on [2,4]. The final values: index 1: 1%4=1 (no mod 3) -> 1; index 2: 2%4=2, then 2%3=2 -> 2; index 3: 3%4=3, then 3%3=0 -> 0; index 4: 4%4=0, then 0%3=0 -> 0.
        // Notice that for index 3, min(3%3, 3%4)=0, achieved by mod 3 directly. But they did mod 4 first, then mod 3. (3%4)%3 = 3%3 = 0. So they got 0.
        // For index 4, min is 0, achieved by mod 4. They did mod 4, then mod 3 didn't change it.
        // So they used a combination: mod q on everything, then mod p on a suffix.
        // In general, we can apply mod q on some prefix/suffix/intervals, then mod p on some intervals.
        // This suggests a dynamic programming approach where we decide for each position whether it gets mod q, mod p, both, or none, with the constraint that the operations form intervals of length >= k.
        // But note that we can apply mod q and mod p in any order, and multiple times. However, as observed, once mod p is applied, mod q does nothing. So the final value is determined by the last mod p or mod q applied.
        // Actually, if we apply mod q then mod p, the mod p is the last operation. If we apply mod p then mod q, the mod q is last but does nothing because values are < p < q. So effectively, the last operation that matters is either a mod p (which reduces to something % p) or a mod q (if no mod p after it). But if we have a mod q after a mod p, it doesn't change anything, so we can ignore it.
        // Therefore, the final value of an element is:
        // - a_i % p if the last operation on it is mod p (or if it received mod p at some point and no mod q after that could change it, but mod q after mod p doesn't change, so it's still a_i % p)
        // - a_i % q if the last operation on it is mod q and it never received mod p after that.
        // -
