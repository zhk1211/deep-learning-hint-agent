// Hint7
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
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }
        
        // We can reduce each element to min(a[i] % p, a[i] % q) if we can apply operation on it.
        // But we can only apply operations on intervals of length >= k.
        // The goal is to minimize sum.
        // We can think of it as: we can choose some set of indices to reduce to their best value,
        // but the chosen indices must be coverable by intervals of length >= k.
        // Actually, we can apply multiple operations, and they can overlap.
        // The key observation: we can reduce any element to its best value if we can cover it with some interval.
        // Since we can apply operations arbitrarily many times, we can reduce any element that belongs to at least one interval of length >= k that we choose.
        // But we can also apply operations sequentially, so we can cover any set of indices that can be partitioned into intervals of length >= k.
        // However, we can also use overlapping intervals to cover isolated indices? Let's think.
        // If we have an interval of length >= k, we can reduce all its elements to their best values.
        // If we have two intervals that overlap, their union is also an interval of length >= k? Not necessarily, but we can just apply operation on the union if it's length >= k.
        // Actually, we can apply operation on any interval of length >= k. So we can cover any set of indices that can be expressed as a union of intervals of length >= k.
        // This is equivalent to: we can reduce any index i if there exists an interval of length >= k containing i that we decide to use.
        // But we can use multiple intervals. The optimal strategy: we want to reduce as many elements as possible to their best values.
        // However, we might not be able to reduce all elements if some are isolated (cannot be part of any interval of length >= k without including other elements we don't want to reduce? But reducing other elements is always beneficial or neutral, because best value <= original value. So we never lose by reducing more elements. Therefore, we should reduce every element that can be covered by some interval of length >= k. But wait: if we reduce an element, we might be forced to reduce others in the same interval, but that's fine because it only decreases sum. So the problem reduces to: we can reduce any element that belongs to at least one interval of length >= k that we choose. Since we can choose any intervals, we can reduce any element that is not "too isolated". Specifically, an element at position i can be reduced if there exists an interval of length >= k that contains i and we choose it. But we can choose multiple intervals. So we can reduce all elements except possibly some that cannot be covered by any interval of length >= k without covering other elements? But covering other elements is fine. So we can reduce all elements if n >= k? Not exactly: if n < k, we cannot perform any operation because no interval of length >= k exists. So if n < k, answer is sum of original a_i. If n >= k, we can reduce all elements? Wait, we can choose the whole array [1, n] which has length n >= k, so we can reduce all elements to their best values in one operation. So answer would be sum of min(a_i % p, a_i % q). But sample 2: n=3, k=2, p=10, q=20. n>=k, so we can reduce all? Best values: 31%10=1, 41%10=1, 59%10=9 -> sum=11. 31%20=11, 41%20=1, 59%20=19 -> min for each: 1,1,9 sum=11. Output is 11. Sample 3: n=4, k=3, p=3, q=4. Best values: 1%3=1, 2%3=2, 3%3=0, 4%3=1 -> sum=4; with 4: 1%4=1, 2%4=2, 3%4=3, 4%4=0 -> min: 1,2,0,0 sum=3. Output 3. So indeed, if n>=k, we can reduce all? But sample 4: n=6, k=4, p=9, q=20. n>=k, best values: 18%9=0, 27%9=0, 180%9=0, 9%9=0, 45%9=0, 99%9=0 -> sum=0. Output 0. Sample 5: n=7, k=4, p=3, q=5. n>=k, best values: 6%3=0, 7%3=1, 14%3=2, 12%3=0, 100%3=1, 78%3=0, 4%3=1 -> sum=5; with 5: 6%5=1, 7%5=2, 14%5=4, 12%5=2, 100%5=0, 78%5=3, 4%5=4 -> min: 0,1,2,0,0,0,1 sum=4. Output 4. Sample 6: n=9, k=4, p=244, q=353. n>=k, best values: compute min(a_i%p, a_i%q). Let's check: 9982%244 = 9982 - 244*40 = 9982-9760=222; %353 = 9982 - 353*28 = 9982-9884=98 -> min 98. 4435%244 = 4435-244*18=4435-4392=43; %353=4435-353*12=4435-4236=199 -> min 43. 3998%244=3998-244*16=3998-3904=94; %353=3998-353*11=3998-3883=115 -> min 94. 2443%244=2443-244*10=2443-2440=3; %353=2443-353*6=2443-2118=325 -> min 3. 5399%244=5399-244*22=5399-5368=31; %353=5399-353*15=5399-5295=104 -> min 31. 8244%244=8244-244*33=8244-8052=192; %353=8244-353*23=8244-8119=125 -> min 125. 3539%244=3539-244*14=3539-3416=123; %353=3539-353*10=3539-3530=9 -> min 9. 9824%244=9824-244*40=9824-9760=64; %353=9824-353*27=9824-9531=293 -> min 64. 4353%244=4353-244*17=4353-4148=205; %353=4353-353*12=4353-4236=117 -> min 117. Sum = 98+43+94+3+31+125+9+64+117 = 584? Wait output is 569. So not all elements reduced to min? Let's recalc: 98+43=141, +94=235, +3=238, +31=269, +125=394, +9=403, +64=467, +117=584. But output is 569. So we cannot reduce all elements to their best? Why? Because n=9, k=4, so we can choose interval of length >=4. But maybe we cannot reduce all elements to their best simultaneously? Wait, we can apply multiple operations. But the operation sets a_i = a_i % m. If we apply multiple operations, the final value is not necessarily min(a_i%p, a_i%q) because applying modulo sequentially might yield different results. For example, if we apply %p then %q, the result is (a_i % p) % q. Since p < q, (a_i % p) % q = a_i % p (because a_i % p < p < q). So applying %p after %q might not be the same as min. Actually, we can choose m in {p,q} for each operation. The final value of a_i after a sequence of operations is the result of applying some sequence of modulo p and q. Since modulo is idempotent and order matters? Let's analyze: For any integer x, the set of possible values we can reach by repeatedly applying %p and %q is exactly the set of values we can get by applying at most one %p and at most one %q, but the order matters. Since p < q, we have:
- x % p is in [0, p-1]
- x % q is in [0, q-1]
- (x % p) % q = x % p (since x%p < p < q)
- (x % q) % p: this could be smaller than both x%p and x%q? For example, x=10, p=3, q=4: x%3=1, x%4=2, (x%4)%3 = 2%3=2, not smaller. Another example: x=7, p=3, q=5: 7%3=1, 7%5=2, (7%5)%3=2%3=2. Actually, (x % q) % p is always <= x % p? Not necessarily: x=8, p=3, q=5: 8%3=2, 8%5=3, 3%3=0. So we can get 0, which is smaller than both 2 and 3. So the minimum possible value for a single element is min( x%p, x%q, (x%q)%p ). But wait, we can also do (%p then %q) which gives x%p. So the set of reachable values is { x%p, x%q, (x%q)%p } (since (x%p)%q = x%p). Could we get (x%p)%q? That's x%p. Could we get something by alternating more? No, because after two operations, the value is less than p, and any further modulo operation leaves it unchanged (since p<q, value < p < q, so %p or %q does nothing). So the minimum possible value for a single element is min( a_i % p, a_i % q, (a_i % q) % p ). But note that (a_i % q) % p is always <= a_i % p? Actually, (a_i % q) is some number in [0, q-1], and taking %p gives a number in [0, p-1]. It could be smaller than a_i % p. So the true minimum for an element is min( a_i % p, (a_i % q) % p ). Because a_i % q might be larger than a_i % p, but (a_i % q) % p could be smaller. So the best we can do for an element is min( a_i % p, (a_i % q) % p ). Let's check sample 6 with this formula.

Compute for each a_i: best = min( a_i % p, (a_i % q) % p ).
p=244, q=353.
a1=9982: 9982%244=222; 9982%353=98; 98%244=98; min=98.
a2=4435: 4435%244=43; 4435%353=199; 199%244=199; min=43.
a3=3998: 3998%244=94; 3998%353=115; 115%244=115; min=94.
a4=2443: 2443%244=3; 2443%353=325; 325%244=81; min=3? Wait 325%244=81, min(3,81)=3.
a5=5399: 5399%244=31; 5399%353=104; 104%244=104; min=31.
a6=8244: 8244%244=192; 8244%353=125; 125%244=125; min=125.
a7=3539: 3539%244=123; 3539%353=9; 9%244=9; min=9.
a8=9824: 9824%244=64; 9824%353=293; 293%244=49; min=49? Wait 293-244=49, so min(64,49)=49.
a9=4353: 4353%244=205; 4353%353=117; 117%244=117; min=117.
Sum = 98+43+94+3+31+125+9+49+117 = 569. Yes! Output 569. So the true minimum value for an element is min( a_i % p, (a_i % q) % p ). But wait, can we achieve this for each element independently? The operations are applied on intervals. We need to assign to each element a sequence of operations (which m to apply) such that the final value is that minimum. But we can only apply operations on intervals of length >= k. So we need to cover the array with intervals where we apply certain mod operations. However, we can apply multiple operations on overlapping intervals. The final value of an element is determined by the sequence of mod operations applied to it, in the order they are applied. Since we can choose the order of intervals, we can effectively apply any sequence of mod operations to each element, as long as the set of elements that receive a particular operation can be covered by intervals of length >= k. But we can apply operations one after another. So we can think of it as: we want to assign to each element a final value from the set of reachable values, and we need to ensure that for each operation we use, the set of indices where we apply that operation is a union of intervals of length >= k? Actually, we apply an operation by choosing an interval [l,r] and an m, and we set a_i = a_i % m for all i in [l,r]. We can do this multiple times. So the sequence of operations is a sequence of (interval, m). The effect on each index i is the composition of the mod operations for intervals that contain i, in the order they are performed. Since modulo operations are not commutative, the order matters. But we can choose the order arbitrarily. So we can achieve for each index i any value that can be obtained by some sequence of %p and %q, provided that we can schedule the operations such that the intervals cover the required indices. However, we can always just apply a single operation on the whole array if n>=k, but that would apply the same m to all elements. To get different values for different elements, we need to use multiple intervals with different m. But we can apply an operation with m=p on some interval, and later m=q on another interval, etc. The final value for an element is the result of applying the operations in order. So if we want an element to end up as (a_i % q) % p, we need to apply %q first, then %p. If we want a_i % p, we can just apply %p (and maybe %q later, but that doesn't change it). If we want (a_i % p) % q = a_i % p, same. So the two interesting targets are: target A = a_i % p, target B = (a_i % q) % p. Note that target B is always <= target A? Not necessarily: e.g., a_i=5, p=3, q=4: A=2, B=(5%4)%3=1%3=1, so B < A. Another: a_i=4, p=3, q=5: A=1, B=(4%5)%3=4%3=1, equal. a_i=7, p=3, q=5: A=1, B=(7%5)%3=2%3=2, so B > A. So we need to choose for each element whether to go for A or B, but we might be forced to apply operations in a certain order globally. Since we can apply operations in any order, we can first apply %q on some intervals, then apply %p on some intervals. The final value for an element i will be:
- If we never apply %q to i, only %p: value = a_i % p.
- If we apply %q to i, then later %p: value = (a_i % q) % p.
- If we apply %q to i but no %p afterwards: value = a_i % q. But a_i % q could be larger than a_i % p, so we would never want that unless we also apply %p later. So we can assume we always apply %p after %q if we want the B target. But we could also apply %p before %q? That would give a_i % p (since %p then %q leaves it unchanged). So the only way to get B is to have %q applied, and then %p applied later. So the sequence of operations must have some %q operations, followed by some %p operations. We can interleave them? If we do %p, then %q, then %p, the final is still (a_i % p) % q % p = a_i % p. So no benefit. So the optimal strategy is to choose a set of indices S where we want to achieve B = (a_i % q) % p, and the rest achieve A = a_i % p. To achieve B on S, we need to apply %q on some intervals covering S, and then apply %p on some intervals covering S (and possibly others). But we can also apply %p on the whole array at the end. So the condition is: we need to be able to cover S with intervals of length >= k where we apply %q. Then we can apply %p on the whole array (or on intervals covering S) afterwards. But wait, if we apply %p on the whole array, it also affects elements not in S, reducing them to A. That's fine. So the problem reduces to: we can choose a set S of indices to "upgrade" to B (which might be worse than A? Actually we only choose S if B < A, otherwise we keep A). But we must be able to cover S with intervals of length >= k where we apply %q. However, we can apply %q on multiple intervals. So S can be any union of intervals of length >= k. But we can also apply %q on intervals that include elements not in S; that would set those elements to a_i % q, but then we apply %p on them later, so they become (a_i % q) % p = B. So if we apply %q on an interval, all elements in that interval will end up as B if we later apply %p on them. So we cannot selectively apply %q to only some elements in an interval; the whole interval gets %q. Therefore, if we decide to use a %q interval, all elements in that interval will become B (assuming we later apply %p). So the choice is: we can partition the array into segments where we either apply %q (and then %p) or just %p. But we can apply %q on multiple intervals, and they can overlap. However, if two %q intervals overlap, their union is also covered by %q. So effectively, the set of indices that receive %q is a union of intervals of length >= k. But since we can choose any intervals, the set of indices that receive %q can be any set that can be covered by intervals of length >= k. What sets can be covered? Any set that does not contain a gap of length < k? Actually, if we have a set of indices, we can cover it with intervals of length >= k if and only if every maximal contiguous segment of the set has length >= k? Not exactly: we can cover a set that has isolated elements by using overlapping intervals that extend beyond the set. But if we extend beyond the set, we include elements we didn't intend to include, which would then also receive %q. So if we want to apply %q to exactly a set S, we need S to be a union of intervals of length >= k. But we can also include extra elements; that would just force them to B as well. Since B might be larger than A for some elements, we might not want to include them. So we need to choose a set S (indices where we apply %q) such that S is a union of intervals of length >= k, and for each i in S, we get value B_i, for i not in S, we get A_i. We want to minimize sum. Since we can choose S arbitrarily as long as it's a union of intervals of length >= k, we need to decide which intervals to take. This is a classic DP or greedy problem: we have an array, each element has two possible values: A_i = a_i % p, B_i = (a_i % q) % p. We can choose to "activate" some intervals of length >= k, and all elements in activated intervals take value B_i, others take A_i. We want to minimize total sum. Since B_i can be greater or less than A_i, we want to include intervals where B_i < A_i, but we might be forced to include some elements where B_i > A_i if they are inside a chosen interval. So we need to select a set of disjoint? Not necessarily disjoint; overlapping intervals just mean the union is covered. So the covered set is a union of intervals of length >= k. This is equivalent to: we can cover any set that can be formed by taking some intervals of length >= k. The optimal covered set will be a union of disjoint intervals of length >= k? Actually, if two intervals overlap, their union is an interval of length >= k (since the union of two overlapping intervals is an interval, and its length is at least max(length1, length2) >= k). So we can merge overlapping intervals into one larger interval. Therefore, the covered set can be assumed to be a set of disjoint intervals, each of length >= k. So the problem: partition the array into segments, each segment is either "covered" (length >= k) or "uncovered". In covered segments, all elements take B_i; in uncovered, take A_i. We want to minimize sum. But wait, can a covered segment be longer than necessary? Yes, we can choose any interval of length >= k. So we can choose a set of disjoint intervals, each length >= k, and the elements in those intervals get B, others get A. This is exactly the problem: we have an array of costs A_i and B_i. We can select some disjoint subarrays of length at least k, and for selected subarrays we pay B_i instead of A_i. We want to minimize total cost. This is a standard DP: dp[i] = minimum cost for prefix up to i. dp[i] = min(dp[i-1] + A_i, min_{j <= i-k+1} (dp[j-1] + sum_{t=j}^i B_t)). We can optimize with prefix sums. But wait, is it always optimal to take disjoint intervals? What if we take an interval that covers some elements where B_i > A_i, but it allows us to cover other elements where B_i is much smaller? Yes, that's handled by the DP: we consider intervals of length >= k, and we can choose to include them even if some B_i > A_i, as long as the net benefit is positive. So the DP is correct.

But wait, there's a catch: the operation sequence. We assumed we can achieve B_i by applying %q then %p. But to do that, we need to apply %q on the chosen intervals, and then apply %p on those same intervals (or on the whole array). We can always apply %p on the whole array at the end. So if we choose a set of disjoint intervals for %q, we can just apply %q on each of those intervals (they are disjoint, so we can do them in any order), and finally apply %p on the whole array. That yields B_i for elements in those intervals, and A_i for others. So the DP model is valid.

But is there any other possibility? Could we apply %q on an interval, then %p on a smaller interval inside it, to get some elements as B and some as A? For example, apply %q on [1,5], then %p on [2,4]. Then element 1 gets %q only -> value a_1 % q. Element 2 gets %q then %p -> B_2. Element 5 gets %q only -> a_5 % q. But a_i % q might be larger than A_i. So we wouldn't want that. We could then apply %p on the whole array to fix 1 and 5, making them B_1 and B_5. So that just expands the covered set. So we can't get a mix of A and B within a %q interval unless we leave some elements with only %q, which is suboptimal. So the DP approach seems correct.

Let's test on sample 5: n=7, k=4, p=3, q=5.
A_i = a_i % 3: 
a: 6,7,14,12,100,78,4
A: 0,1,2,0,1,0,1
B_i = (a_i % 5) % 3:
a%5: 1,2,4,2,0,3,4
B: 1%3=1, 2%3=2, 4%3=1, 2%3=2, 0%3=0, 3%3=0, 4%3=1
So B: 1,2,1,2,0,0,1
Compare A and B:
i=1: A=0, B=1 -> A better
i=2: A=1, B=2 -> A better
i=3: A=2, B=1 -> B better
i=4: A=0, B=2 -> A better
i=5: A=1, B=0 -> B better
i=6: A=0, B=0 -> equal
i=7: A=1, B=1 -> equal
We need intervals of length >=4. We can choose an interval covering indices 3-6 (length 4): B values: 1,2,0,0 sum=3; A values: 2,0,1,0 sum=3. Net change: 0. Not beneficial. What about interval 2-5? length 4: B:2,1,2,0 sum=5; A:1,2,0,1 sum=4. Worse. Interval 1-4: B:1,2,1,2 sum=6; A:0,1,2,0 sum=3. Worse. Interval 4-7: B:2,0,0,1 sum=3; A:0,1,0,1 sum=2. Worse. So no interval of length >=4 gives net benefit. But sample output is 4. Sum of A_i = 0+1+2+0+1+0+1 = 5. But output is 4. So we can achieve 4. How? The note says: "In the fifth test case, a possible way to obtain sum=4 is to apply the following operations: ..." Wait, the note doesn't give the operations for sample 5. It only gives for 2 and 3. But output is 4. So my DP would give 5 if no interval is beneficial. But we can achieve 4. So my model is missing something.

Let's re-examine sample 5 manually. n=7, k=4, p=3, q=5.
a = [6,7,14,12,100,78,4]
We can apply operations. Let's try to find a sequence that yields sum 4.
We want to minimize sum. The absolute minimum possible if we could choose per element independently: min(A_i, B_i) = [0,1,1,0,0,0,1] sum=3. But we can't achieve that because we need intervals of length >=4. Output is 4, so we can get sum 4.
How to get 4? Let's try:
Option: apply %q on [3,6] (length 4): indices 3,4,5,6. Then apply %p on [1,7] (whole array). Let's simulate:
Initial: [6,7,14,12,100,78,4]
After %q on [3,6]: a3=14%5=4, a4=12%5=2, a5=100%5=0, a6=78%5=3. Array: [6,7,4,2,0,3,4]
After %p on all: a1=6%3=0, a2=7%3=1, a3=4%3=1, a4=2%3=2, a5=0%3=0, a6=3%3=0, a7=4%3=1. Sum = 0+1+1+2+0+0+1 = 5. Not 4.
Try %q on [4,7]: a4=12%5=2, a5=0, a6=3, a7=4. Then %p all: a1=0, a2=1, a3=14%3=2, a4=2%3=2, a5=0, a6=0, a7=1 -> sum=0+1+2+2+0+0+1=6.
Try %q on [2,5]: a2=7%5=2, a3=4, a4=2, a5=0. Then %p all: a1=0, a2=2%3=2, a3=1, a4=2, a5=0, a6=78%3=0, a7=1 -> sum=0+2+1+2+0+0+1=6.
Try %q on [1,4]: a1=6%5=1, a2=2, a3=4, a4=2. %p all: a1=1%3=1, a2=2, a3=1, a4=2, a5=100%3=1, a6=0, a7=1 -> sum=1+2+1+2+1+0+1=8.
Try %q on [1,5] (length 5): a1=1, a2=2, a3=4, a4=2, a5=0. %p all: a1=1, a2=2, a3=1, a4=2, a5=0, a6=0, a7=1 -> sum=7.
Try %q on [3,7] (length 5): a3=4, a4=2, a5=0, a6=3, a7=4. %p all: a1=0, a2=1, a3=1, a4=2, a5=0, a6=0, a7=1 -> sum=5.
Try %q on [2,6] (length 5): a2=2, a3=4, a4=2, a5=0, a6=3. %p all: a1=0, a2=2, a3=1, a4=2, a5=0, a6=0, a7=1 -> sum=6.
Try %q on [1,6] (length 6): a1=1, a2=2, a3=4, a4=2, a5=0, a6=3. %p all: 1,2,1,2,0,0,1 -> sum=7.
Try %q on [2,7] (length 6): a2=2, a3=4, a4=2, a5=0, a6=3, a7=4. %p all: 0,2,1,2,0,0,1 -> sum=6.
Try %q on whole array: a%5: 1,2,4,2,0,3,4. %p all: 1,2,1,2,0,0,1 -> sum=7.
None gives 4. What about applying %p first, then %q? That would just give A_i for those with %p, and %q on top doesn't change. So no.
What about applying %q on some interval, then %p on a different interval, not the whole array? For example, apply %q on [1,4], then %p on [3,6]. Let's simulate:
Start: [6,7,14,12,100,78,4]
%q on [1,4]: a1=1, a2=2, a3=4, a4=2. Array: [1,2,4,2,100,78,4]
%p on [3,6]: a3=4%3=1, a4=2%3=2, a5=100%3=1, a6=78%3=0. Array: [1,2,1,2,1,0,4]
Sum = 1+2+1+2+1+0+4 = 11.
Not good.
Try %q on [3,6], then %p on [1,4]:
%q on [3,6]: a3=4, a4=2, a5=0, a6=3. Array: [6,7,4,2,0,3,4]
%p on [1,4]: a1=0, a2=1, a3=1, a4=2. Array: [0,1,1,2,0,3,4] sum=11.
Try %q on [1,5], then %p on [3,7]:
%q: [1,2,4,2,0,78,4]
%p on [3,7]: a3=1, a4=2, a5=0, a6=0, a7=1 -> [1,2,1,2,0,0,1] sum=7.
Try %q on [2,5], then %p on [4,7]:
%q: [6,2,4,2,0,78,4]
%p on [4,7]: a4=2, a5=0, a6=0, a7=1 -> [6,2,4,2,0,0,1] sum=15.
Try %q on [1,4] and %q on [5,7]? But we can do multiple %q operations. Let's try %q on [1,4] and %q on [4,7] (overlap). Then %p on all.
%q on [1,4]: [1,2,4,2,100,78,4]
%q on [4,7]: a4=2%5=2, a5=100%5=0, a6=78%5=3, a7=4%5=4 -> [1,2,4,2,0,3,4]
%p all: [1,2,1,2,0,0,1] sum=7.
Try %q on [1,5] and %q on [3,7]:
%q on [1,5]: [1,2,4,2,0,78,4]
%q on [3,7]: a3=4%5=4, a4=2%5=2, a5=0, a6=78%5=3, a7=4%5=4 -> [1,2,4,2,0,3,4]
%p all: [1,2,1,2,0,0,1] sum=7.
Try %p first on some interval, then %q on another? %p then %q: if we do %
