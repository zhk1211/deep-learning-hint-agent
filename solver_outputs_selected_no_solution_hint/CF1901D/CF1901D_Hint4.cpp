#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }
    
    if (n == 1) {
        cout << a[0] << "\n";
        return 0;
    }
    
    // We need to find min x such that for any valid order starting at some i,
    // all monsters die. The worst-case order for a fixed i is that the damage
    // sequence x, x-1, ..., x-n+1 is assigned to monsters in some order
    // determined by the chain lightning rule. The rule: first hit i, then
    // each subsequent hit must be adjacent to some previously hit monster.
    // This is equivalent to: the order of hits is a permutation that is a
    // linear extension of the tree order rooted at i, where the tree is just
    // the path graph. Actually, the possible orders are exactly those where
    // the set of hit monsters forms a contiguous segment at all times.
    // So the order is: start at i, then expand left or right one by one.
    // The sequence of damages is decreasing: d_k = x - (k-1) for k-th hit.
    // The worst-case assignment of these damages to monsters, given the
    // expansion order, is that the monster that gets hit later gets less damage.
    // To guarantee all die, we need that for every monster j, its damage >= a_j.
    // The damage a monster receives depends on when it is hit.
    // For a fixed start i, the hit times are determined by the expansion.
    // The worst-case expansion order (from Vasya's perspective) is the one
    // that minimizes the damage to the monster with the highest requirement,
    // i.e., the adversary chooses the order of left/right expansions.
    // We need to find x such that for the optimal i, even in the worst
    // expansion order, all constraints hold.
    
    // Let's analyze: For a fixed i, the hit times: monster i at time 1.
    // Then we have two segments: left of i and right of i.
    // The expansion will interleave left and right steps.
    // The time a monster at distance d from i is hit depends on how many
    // steps were taken in the opposite direction before it.
    // The worst-case for a monster at distance d is that it is hit as late
    // as possible. The latest possible time for a monster at distance d is
    // if we first fully expand in the other direction, then come back.
    // Specifically, for monster j, let L = i - j if j < i, or R = j - i if j > i.
    // The latest time it can be hit is: 1 + (total monsters on the other side) + (distance - 1)?
    // Actually, the set of hit monsters is always a contiguous segment [l, r] containing i.
    // Initially l = r = i. At each step, we either decrease l or increase r.
    // The time a monster at position p is hit is exactly the step when the segment expands to include p.
    // The worst-case for p is that we delay expanding to its side as much as possible.
    // If p < i, the latest time is when we first expand r all the way to n, then expand l to p.
    // So time = 1 + (n - i) + (i - p). Similarly for p > i: time = 1 + (i - 1) + (p - i).
    // But wait, is that always achievable? Yes, the adversary can choose to always expand right
    // until reaching n, then expand left. That gives the latest possible time for left-side monsters.
    // So for a fixed i, the worst-case damage for monster j is:
    // damage = x - (time - 1) = x - time + 1.
    // We need damage >= a_j  =>  x >= a_j + time - 1.
    // So for each j, we have a lower bound on x.
    // The required x for start i is max over j of (a_j + time(j) - 1).
    // We want to choose i to minimize this maximum.
    
    // Let's compute time(j) for worst-case:
    // If j <= i: time = 1 + (n - i) + (i - j) = n - j + 1.
    // If j >= i: time = 1 + (i - 1) + (j - i) = j.
    // Check: For j = i, both formulas give? Left: n - i + 1, Right: i. They are different!
    // But for j = i, the time is always 1 (first hit). The worst-case time for i is 1, not those.
    // The formulas above assume we delay hitting j as much as possible, but for j = i, we can't delay.
    // So we need to treat j = i separately: time = 1.
    // For j < i, the latest time is indeed when we go right first: time = 1 + (n - i) + (i - j) = n - j + 1.
    // For j > i, the latest time is when we go left first: time = 1 + (i - 1) + (j - i) = j.
    // So the bound for j < i: x >= a_j + (n - j + 1) - 1 = a_j + n - j.
    // For j > i: x >= a_j + j - 1.
    // For j = i: x >= a_i + 1 - 1 = a_i.
    
    // But wait, is it always possible for the adversary to achieve these times simultaneously?
    // The adversary chooses one order. The order that maximizes time for left monsters is
    // going right first, then left. In that order, the times for left monsters are exactly n - j + 1.
    // The times for right monsters in that order are: for j > i, time = 1 + (j - i) (since we go right first).
    // That is not the worst-case for right monsters. The worst-case for right monsters is going left first.
    // The adversary cannot simultaneously maximize times for both sides.
    // So the worst-case order for a fixed i is not simply taking the max of individual worst-cases.
    // We need to consider the actual worst-case order that maximizes the required x.
    // The required x for a given order is max_j (a_j + time(j) - 1).
    // The adversary will choose the order (sequence of left/right expansions) to maximize this max.
    // We need to find the order that gives the highest lower bound on x, and then we choose i to minimize that.
    
    // Let's formalize: For a fixed i, an order is a sequence of moves: L (expand left) or R (expand right).
    // There are (i-1) L moves and (n-i) R moves, in some order.
    // The time a monster at position j is hit:
    // If j < i: time = 1 + (number of R moves before the (i-j)-th L move) + (i-j).
    // If j > i: time = 1 + (number of L moves before the (j-i)-th R move) + (j-i).
    // The adversary wants to maximize max_j (a_j + time(j) - 1).
    // This is equivalent to: given the sequence, the damage to j is x - (time(j)-1).
    // So we need x >= a_j + time(j) - 1.
    // The adversary's max is the maximum over j of that expression.
    // We want to find, for each i, the minimum possible value of that maximum over all orders?
    // No, Vasya wants to guarantee kill even in the worst possible scenario.
    // So for a fixed i, the required x is the maximum over all possible orders of the required x for that order.
    // Because the adversary will pick the order that forces the highest x.
    // So x(i) = max_{order} max_j (a_j + time_order(j) - 1).
    // We want min_i x(i).
    
    // Let's analyze the adversary's optimal strategy for a fixed i.
    // The adversary wants to maximize the maximum of (a_j + time(j) - 1).
    // This is like: we have two sequences of "delays": for left side, the delay is the number of R moves before it;
    // for right side, the delay is the number of L moves before it.
    // The adversary can choose the interleaving.
    // This is a known problem: it's equivalent to the following.
    // Consider the array a. We want to assign damages x, x-1, ..., x-n+1 to positions.
    // The assignment must be a valid BFS order from i on the path graph.
    // The valid orders are exactly those where the set of hit vertices is always an interval.
    // This is equivalent to: the permutation pi of vertices such that pi^{-1}(j) is the time.
    // The condition: for any j, the set of vertices hit before j must be an interval containing i.
    // This means that if j < k < i, then j cannot be hit after k if k is not hit yet? Actually,
    // the condition is that at any time, the hit vertices form a contiguous segment.
    // So if we consider the order of hitting, the sequence of positions is a sequence that starts at i,
    // and each subsequent position is either the left neighbor of the current segment or the right neighbor.
    // This is exactly the order of visiting nodes in a graph that is a path, starting from i,
    // where you can only visit neighbors of visited nodes. This is exactly the set of all permutations
    // that are linear extensions of the poset where j < k if j is between i and k? Not exactly.
    // Actually, the condition is: if j is between i and k, then j must be hit before k.
    // Because to reach k, you must expand through j. So the interval property implies that
    // for any j between i and k, time(j) < time(k).
    // So the poset is: for any j < k <= i, we have time(j) > time(k)? Wait.
    // If i=3, positions: 1 2 3 4 5. To hit 1, we must have hit 2 first. So time(2) < time(1).
    // To hit 5, we must have hit 4 first. So time(4) < time(5).
    // In general, moving away from i, times increase.
    // So for left side: time(i) < time(i-1) < time(i-2) < ... < time(1).
    // For right side: time(i) < time(i+1) < ... < time(n).
    // But there is no constraint between left and right sides; they can be interleaved arbitrarily.
    // So the set of valid time assignments is: time(i)=1, and for left side, times are a strictly increasing sequence
    // of length L = i-1, and for right side, length R = n-i, and the two sequences are merged in some order.
    // The damage to j is x - (time(j)-1).
    // We need x >= a_j + time(j) - 1.
    // The adversary chooses the merge order to maximize max_j (a_j + time(j) - 1).
    // We want to find, for each i, the minimum possible value of this maximum if we could choose the merge order?
    // No, the adversary chooses the merge order to maximize it. Vasya wants to minimize over i.
    // So for each i, the required x is the maximum over all merge orders of the max over j.
    // This is equivalent to: the adversary will assign the times to left and right sides in the worst way.
    // Since the times for left side are just some increasing sequence of length L, and for right side length R,
    // and the adversary can interleave them, the worst-case time for a left monster j is when all right monsters
    // are placed before it. That gives time = 1 + R + (i-j). Similarly for right.
    // But can the adversary achieve the worst-case for all monsters simultaneously? No.
    // The adversary wants to maximize the maximum. So they will focus on the monster that gives the highest value.
    // The adversary's optimal merge order is to delay the monster with the highest a_j + (something) as much as possible.
    // This is a classic problem: given two sequences of "base times" (for left: base time = i-j, for right: j-i),
    // and we can interleave them, the time of an element is its base time plus the number of elements from the other side
    // that are placed before it. The adversary wants to maximize max (a_j + time(j) - 1).
    // This is equivalent to: we have two arrays: left demands and right demands.
    // For left monster at distance d (d = i-j), its requirement is a_j + d - 1 + (number of right monsters before it).
    // For right monster at distance d, requirement is a_j + d - 1 + (number of left monsters before it).
    // The adversary can choose the order. This is similar to scheduling to maximize maximum completion time with two chains.
    // Actually, the adversary's problem: given two sets of jobs with processing times? Not exactly.
    // Let's think differently. The final required x is the maximum over all j of (a_j + time(j) - 1).
    // The adversary wants to maximize this. So they will try to make one particular j have as large time as possible.
    // The maximum possible time for a left monster j is 1 + R + (i-j) = n - j + 1.
    // The maximum possible time for a right monster j is 1 + L + (j-i) = j.
    // So an upper bound on x(i) is max( max_{j<=i} (a_j + n - j), max_{j>=i} (a_j + j - 1) ).
    // But is this upper bound achievable? The adversary can achieve the maximum time for one side by doing all of the other side first.
    // However, if the maximum of these two expressions comes from both sides, they cannot achieve both simultaneously.
    // But the adversary only needs to maximize the maximum, so they can just pick the side that gives the higher value,
    // and use the order that delays that side as much as possible. That order will give that side its worst-case times,
    // and the other side will have its best-case times (which are smaller). So the maximum over all j in that order
    // will be exactly the maximum of the worst-case times for the chosen side, and the best-case times for the other side.
    // The best-case time for a left monster is when all left monsters are before any right monster: time = 1 + (i-j).
    // The best-case time for a right monster is time = 1 + (j-i).
    // So if the adversary chooses to delay left (i.e., do all right first), the times are:
    // left: time = 1 + R + (i-j) = n - j + 1
    // right: time = 1 + (j-i)  (since right are done first, no left before them)
    // Then the required x for that order is max( max_{j<=i} (a_j + n - j), max_{j>=i} (a_j + (j-i)) ).
    // Similarly, if they delay right: left times = 1 + (i-j), right times = 1 + L + (j-i) = j.
    // Required x = max( max_{j<=i} (a_j + i - j), max_{j>=i} (a_j + j - 1) ).
    // The adversary will choose the order that gives the larger of these two maxima.
    // So x(i) = max( max_{j<=i} (a_j + n - j), max_{j>=i} (a_j + (j-i)) ) and similarly the other way?
    // Actually, the adversary can also choose any interleaving. Could an interleaving produce a higher maximum than both extremes?
    // Suppose the maximum from delaying left is M1, and from delaying right is M2.
    // Could there be an order where some left monster and some right monster both have times close to their worst-case,
    // making the max higher than both M1 and M2? No, because the worst-case for left requires all right first,
    // and worst-case for right requires all left first. You can't have both. Any interleaving gives times that are
    // convex combinations? Actually, for a left monster, its time is 1 + (i-j) + r, where r is the number of right monsters before it.
    // For a right monster, time = 1 + (j-i) + l, where l is the number of left monsters before it.
    // Note that r + (number of right monsters after it) = R, and l + (number of left after) = L.
    // The adversary wants to maximize max( a_j + time(j) - 1 ). This is equivalent to maximizing over j.
    // This is a known problem: the worst-case order for maximizing the maximum of a_j + time(j) is indeed one of the two extremes.
    // Because the function is monotone in the delays. I think it's true that the maximum over all orders is the maximum of the two extremes.
    // Let's test with a small example. n=3, i=2. a = [10, 1, 10].
    // Left: j=1, a1=10. Right: j=3, a3=10.
    // Delay left: order 2,3,1. Times: 2:1, 3:2, 1:3. Requirements: a2+0=1, a3+1=11, a1+2=12. Max=12.
    // Delay right: order 2,1,3. Times: 2:1, 1:2, 3:3. Requirements: a2+0=1, a1+1=11, a3+2=12. Max=12.
    // Interleaving? Only two orders. Both give 12.
    // Another example: a = [1, 10, 100]. i=2.
    // Delay left: order 2,3,1. Times: 2:1, 3:2, 1:3. Req: 10+0=10, 100+1=101, 1+2=3. Max=101.
    // Delay right: order 2,1,3. Times: 2:1, 1:2, 3:3. Req: 10+0=10, 1+1=2, 100+2=102. Max=102.
    // So max is 102. The adversary would choose delay right.
    // What if we interleave? Only two orders. So max is max(101,102)=102.
    // It seems the worst-case order is always one of the two extremes.
    // Proof sketch: The maximum of a set of linear functions over a permutation is a convex function? The set of achievable time vectors is the set of all linear extensions of two chains. The maximum of a_j + t_j - 1 is a convex function of the time vector. The maximum over a convex set occurs at an extreme point. The extreme points of the linear extension polytope of two chains are exactly the merges where one chain entirely precedes the other? Actually, the set of linear extensions of two chains is a poset of dimension 2. The extreme points of the order polytope correspond to permutations where the relative order of elements within each chain is fixed, and the interleaving is arbitrary. The vertices of the polytope are exactly the "shuffles" where the two chains are interleaved. But are the extreme points exactly the two where one chain entirely precedes the other? No, any shuffle is a vertex of the permutahedron? Actually, the set of permutations that are linear extensions of two chains is exactly all interleavings. The convex hull of these permutations as points in R^n (with coordinates being positions) has all these permutations as vertices? Not necessarily. But we are maximizing a linear function (since a_j + t_j - 1 is linear in t_j). The maximum of a linear function over a finite set of points is achieved at some point. The set of points is all valid time assignments. We want to find the maximum over all interleavings of max_j (a_j + t_j - 1). This is equivalent to: max over interleavings of max_j linear_function_j(t). This is not simply a linear function. But we can think: the adversary wants to maximize the maximum. This is equivalent to finding the minimum x such that for all interleavings, max_j (a_j + t_j - 1) <= x. That is, for all interleavings, for all j, a_j + t_j - 1 <= x. This must hold for the worst-case interleaving for each j. The worst-case interleaving for a given j is the one that maximizes its time. As we saw, the maximum time for left j is n - j + 1, for right j is j. So a necessary condition is: for all j <= i, a_j + n - j <= x, and for all j >= i, a_j + j - 1 <= x. Is this condition also sufficient? If x satisfies these, does it guarantee that for any interleaving, all a_j + t_j - 1 <= x? Not necessarily, because in an interleaving, some left j might have time less than its maximum, but some right k might have time more than its minimum. But we already bounded the maximum possible time for any right k by j? Wait, the maximum time for right k is k (when all left are before it). So if x >= a_k + k - 1, then even in its worst-case, it's <= x. Similarly for left. So if x satisfies both sets of inequalities, then for any interleaving, the time of any left j is <= n - j + 1, so a_j + t_j - 1 <= a_j + n - j <= x. Similarly for right. So indeed, if x >= max( max_{j<=i} (a_j + n - j), max_{j>=i} (a_j + j - 1) ), then for any interleaving, all constraints hold. Therefore, the required x for start i is exactly that maximum! Because the adversary can achieve the worst-case for the side that gives the maximum, making that constraint tight. So x(i) = max( max_{j<=i} (a_j + n - j), max_{j>=i} (a_j + j - 1) ).
    // Let's verify with the previous example: a=[1,10,100], i=2.
    // max_{j<=2} (a_j + n - j) = max(1+3-1=3, 10+3-2=11) = 11.
    // max_{j>=2} (a_j + j - 1) = max(10+2-1=11, 100+3-1=102) = 102.
    // x(2) = max(11,102) = 102. Matches our manual calculation.
    // Another example: a=[2,1,5,6,4,3], n=6. Sample 1 output 8.
    // Let's compute x(i) for each i and take min.
    // We need prefix max of (a_j + n - j) and suffix max of (a_j + j - 1).
    // Let's compute arrays:
    // L_j = a_j + n - j. For j=1..n.
    // R_j = a_j + j - 1.
    // For a given i, x(i) = max( max_{j<=i} L_j, max_{j>=i} R_j ).
    // We want min_i x(i).
    // Let's test sample 1:
    // a = [2,1,5,6,4,3]
    // n=6
    // j: 1 2 3 4 5 6
    // L: 2+6-1=7, 1+6-2=5, 5+6-3=8, 6+6-4=8, 4+6-5=5, 3+6-6=3
    // R: 2+1-1=2, 1+2-1=2, 5+3-1=7, 6+4-1=9, 4+5-1=8, 3+6-1=8
    // prefix max of L: [7,7,8,8,8,8]
    // suffix max of R: [9,9,9,9,8,8] (from right: 8,8,9,9,9,9? Let's compute properly:
    // R[6]=8, R[5]=8, R[4]=9, R[3]=7, R[2]=2, R[1]=2.
    // suffix max from j to n: for j=6:8, j=5:max(8,8)=8, j=4:max(9,8)=9, j=3:max(7,9)=9, j=2:max(2,9)=9, j=1:max(2,9)=9.
    // So suffix max array: [9,9,9,9,8,8]
    // Now x(i) = max(pref[i], suff[i]).
    // i=1: max(7,9)=9
    // i=2: max(7,9)=9
    // i=3: max(8,9)=9
    // i=4: max(8,9)=9
    // i=5: max(8,8)=8
    // i=6: max(8,8)=8
    // min = 8. Matches sample output 8!
    
    // Sample 2: 5 4 4 4 4 4
    // n=5
    // L: 4+5-1=8, 4+5-2=7, 4+5-3=6, 4+5-4=5, 4+5-5=4
    // R: 4+1-1=4, 4+2-1=5, 4+3-1=6, 4+4-1=7, 4+5-1=8
    // pref L: [8,8,8,8,8]
    // suff R: [8,8,8,8,8] (from right: 8,7,6,5,4 -> suffix max: 8,8,8,8,8)
    // x(i) = max(8,8)=8 for all i. min=8. Matches.
    
    // Sample 3: 2 1 1000000000
    // n=2
    // L: 1+2-1=2, 1000000000+2-2=1000000000
    // R: 1+1-1=1, 1000000000+2-1=1000000001
    // pref L: [2, 1000000000]
    // suff R: [1000000001, 1000000001] (R[2]=1000000001, R[1]=1, suffix max from 1: max(1,1000000001)=1000000001)
    // i=1: max(2,1000000001)=1000000001
    // i=2: max(1000000000,1000000001)=1000000001
    // min=1000000001? But sample output is 1000000000.
    // Wait, sample output is 1000000000. Our formula gives 1000000001. Something is off.
    // Let's re-evaluate sample 3 manually.
    // n=2, a=[1, 1000000000].
    // Possible starts: i=1 or i=2.
    // If start i=1: first hit monster 1 with damage x. Second hit must be monster 2 (only adjacent). Damage x-1.
    // We need x >= 1, and x-1 >= 1000000000 => x >= 1000000001.
    // If start i=2: first hit monster 2 with damage x. Second hit monster 1 with damage x-1.
    // Need x >= 1000000000, and x-1 >= 1 => x >= 2. So x >= 1000000000.
    // Minimum x = 1000000000. So answer is 1000000000.
    // Our formula for i=2: x(2) = max( max_{j<=2} L_j, max_{j>=2} R_j ).
    // L_1 = a1 + n - 1 = 1 + 2 - 1 = 2.
    // L_2 = a2 + n - 2 = 1000000000 + 0 = 1000000000.
    // max_{j<=2} L_j = 1000000000.
    // R_2 = a2 + 2 - 1 = 1000000001.
    // max_{j>=2} R_j = 1000000001.
    // So x(2) = max(1000000000, 1000000001) = 1000000001. But we know actual required x for i=2 is 1000000000.
    // Why the discrepancy?
    // Let's analyze the worst-case order for i=2. There is only one order: hit 2 then 1.
    // Times: t2=1, t1=2.
    // Requirements: x >= a2 + 0 = 1000000000; x >= a1 + 1 = 2.
    // So x >= 1000000000. The adversary has no choice; the order is fixed.
    // Our formula assumed the adversary can choose the order to maximize the max. But for i=2, there is only one order.
    // The formula max_{j>=2} R_j uses R_2 = a2 + 2 - 1 = 1000000001. That corresponds to the worst-case time for monster 2 being 2?
    // But monster 2 is the first hit, its time is always 1. The worst-case time for a right monster j is j, but that's only if it's not the first.
    // For the starting monster i, its time is always 1, regardless of side. Our formula for R_j assumed j >= i, and we used time = j for worst-case.
    // But for j = i, the time is 1, not i. So the formula R_i should be a_i + 1 - 1 = a_i, not a_i + i - 1.
    // Similarly, L_i should be a_i, not a_i + n - i.
    // So we need to treat the starting monster separately.
    // In our derivation, we said: for j < i, worst-case time = n - j + 1. For j > i, worst-case time = j. For j = i, time = 1.
    // Then the required x for a fixed i, assuming the adversary can choose the worst order for each side independently,
    // is max( a_i, max_{j<i} (a_j + n - j), max_{j>i} (a_j + j - 1) ).
    // But wait, is that correct? In the previous examples, we didn't have a problem because a_i was small.
    // Let's re-check sample 1 with this corrected formula.
    // For i=5: a_5=4. max_{j<5} L_j = max(L1..L4) = max(7,5,8,8)=8. max_{j>5} R_j = R6=8. So max(4,8,8)=8. Same.
    // For i=6: a_6=3. max_{j<6} L_j = 8. max_{j>6} empty. max(3,8)=8. Same.
    // For i=1: a_1=2. max_{j<1} empty. max_{j>1} R_j = max(R2..R6) = max(2,7,9,8,8)=9. max(2,9)=9. Same.
    // So the correction only matters when a_i is large and i is at the boundary? Actually, for i=2 in sample 3:
    // a_2=1000000000. max_{j<2} L_j = L1 = 1+2-1=2. max_{j>2} empty. So x(2) = max(1000000000, 2) = 1000000000. Correct!
    // For i=1: a_1=1. max_{j<1} empty. max_{j>1} R_j = R2 = 1000000000+2-1=1000000001. x(1)=max(1,1000000001)=1000000001. Correct.
    // So the correct formula for x(i) is:
    // x(i) = max( a_i, max_{j < i} (a_j + n - j), max_{j > i} (a_j + j - 1) )
    // Let's verify with another case. Suppose n=3, a=[100, 1, 100], i=2.
    // a_2=1. max_{j<2} L1 = 100+3-1=102. max_{j>2} R3 = 100+3-1=102. x(2)=max(1,102,102)=102.
    // Manual: start 2. Orders: 2,1,3 or 2,3,1.
    // 2,1,3: times 1,2,3. Req: 1, 100+1=101, 100+2=102 -> max 102.
    // 2,3,1: times 1,2,3. Req: 1, 100+1=101, 100+2=102 -> max 102.
    // So 102 is correct.
    // What if a=[100, 100, 1]? i=2.
    // a_2=100. L1=100+3-1=102. R3=1+3-1=3. x(2)=max(100,102,3)=102.
    // Orders: 2,1,3: times 1,2,3. Req: 100, 100+1=101, 1+2=3 -> max 101? Wait, 100+1=101, but L1 formula gave 102. Why?
    // Let's compute: a1=100, n=3, j=1. L1 = a1 + n - j = 100 + 3 - 1 = 102.
    // But in order 2,1,3: time of 1 is 2. So requirement = a1 + time - 1 = 100 + 2 - 1 = 101.
    // The worst-case time for j=1 is when we do all right first: order 2,3,1. Then time of 1 is 3. Requirement = 100 + 3 - 1 = 102.
    // So the adversary can choose order 2,3,1 to force x >= 102. In that order, time of 3 is 2, requirement = 1 + 2 - 1 = 2.
    // So max is 102. So x(2)=102 is correct.
    // Our formula gives 102. Good.
    
    // So the algorithm: compute prefix maximums of L_j = a_j + n - j for j from 1 to n.
    // Compute suffix maximums of R_j = a_j + j - 1 for j from 1 to n.
    // Then for each i from 1 to n, x_i = max( a_i, (i>1 ? pref_max[i-1] : 0), (i<n ? suff_max[i+1] : 0
