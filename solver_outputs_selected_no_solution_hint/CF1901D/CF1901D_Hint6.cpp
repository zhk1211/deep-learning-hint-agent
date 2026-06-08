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
    
    // We need to find min x such that for any possible order of hits
    // (which is any permutation that is a valid "chain" starting from chosen i),
    // the damage sequence x, x-1, ..., x-(n-1) kills all monsters.
    // The worst-case order for a given i is when the monster that requires the most damage
    // gets hit as late as possible (receiving less damage).
    // The chain order is constrained: it's a sequence that starts at i and then
    // each next monster is adjacent to some already hit monster.
    // This is equivalent to: the order is a permutation where the set of hit monsters
    // is always a contiguous segment? Not exactly. Actually, the process generates
    // a spanning tree (a path) that is a Hamiltonian path in the line graph.
    // The possible orders are exactly those that can be obtained by starting at i
    // and repeatedly adding an adjacent unvisited vertex.
    // This is equivalent to: the order is a permutation where the first element is i,
    // and for each k, the set of first k elements forms a contiguous segment.
    // Because on a line, if you start at i and always add an adjacent vertex,
    // the visited set is always an interval.
    // So the possible orders are exactly the sequences that start at i and
    // then expand the interval to the left or right one by one.
    // Thus, for a fixed start i, the order is determined by a sequence of choices:
    // at each step, if the current interval is [L, R], you can next hit L-1 or R+1.
    // The last monster hit will be one of the ends of the whole array (1 or n).
    // The worst-case for a fixed start i is when the monster that needs the highest
    // "required x" is hit as late as possible.
    // For a monster at position j, the damage it receives is x - (t-1) where t is its hit time.
    // So we need x >= a_j + t - 1.
    // For a fixed start i, the worst-case order maximizes max_j (a_j + t_j - 1).
    // We need to find min over i of that max over all valid orders.
    // But we can think differently: we want to find the minimum x such that there exists
    // a start i and an assignment of times t_j (a permutation of 0..n-1, with t_i=0)
    // that is a valid expansion order, and a_j + t_j <= x for all j.
    // Equivalently, x >= max_j (a_j + t_j).
    // We want to minimize max_j (a_j + t_j) over all valid (i, order).
    // Since the order is an interval expansion, the times are determined by the order
    // we add elements to the interval.
    // Let's analyze the constraints on t_j.
    // For a fixed start i, the interval expands. The time t_j is the step at which j is added.
    // The last monster is either 1 or n. The time for the last monster is n-1.
    // The first monster is i, time 0.
    // For any j, the time t_j is at least the distance from i to j? Not exactly.
    // Because you can go left and right in any order. But the time to reach j is at least
    // the number of steps needed to expand the interval from i to cover j.
    // Actually, if you start at i, to reach j, you must have covered all positions between i and j.
    // So the earliest time you can hit j is |i-j| (if you go straight towards j).
    // The latest time you can hit j is n-1 - (distance from j to the opposite end)?
    // Let's think: the last monster is one of the ends. If j is not an end, it can be hit earlier.
    // The worst-case for a fixed i is when we delay hitting the monster with the largest a_j as much as possible.
    // But we can choose the order to minimize the maximum of a_j + t_j.
    // This is a known problem: we want to assign times t_j (0..n-1) such that the visited set is always an interval,
    // and we minimize max(a_j + t_j). This is equivalent to finding a permutation that is a "linear extension"
    // of the interval expansion order.
    // There is a known result: the optimal start is either at a position where a_i is maximum,
    // or we can compute the answer by considering the two possible last monsters (1 or n).
    // Let's follow the hints.
    // Hint 1: damage to a single monster: if hit at time t (0-indexed), damage = x - t.
    // Hint 2: consider the last monster.
    // Hint 3: consider the first monster.
    // Hint 4: what if we try to hit the first monster as the last one?
    // Hint 5: consider the case when the first monster gets hit as the last one.
    // Hint 6: what happens if we try to hit the second monster as the last one?
    // The hints suggest we should think about the order of hitting the ends.
    // The last monster must be either 1 or n. So the last damage is x - (n-1).
    // So we need x >= a_last + n - 1.
    // The first monster is i, damage x. So x >= a_i.
    // But we can choose i. The worst-case order will try to maximize the required x.
    // For a fixed start i, the adversary (worst-case order) will choose the sequence of expansions
    // to maximize max_j (a_j + t_j). We want to pick i to minimize that maximum.
    // This is a minimax problem.
    // Let's denote f(i) = min over valid orders starting at i of max_j (a_j + t_j).
    // We want min_i f(i).
    // How to compute f(i)? The order is an interval expansion. The times t_j are determined by
    // the order we add elements to the left or right. This is equivalent to: we have a sequence of
    // choices: at each step, we can expand left or right. The time t_j is the step when j is added.
    // We want to assign times to minimize the maximum of a_j + t_j.
    // This is similar to scheduling with precedence constraints: the interval must grow continuously.
    // Actually, we can think of it as: we have a rooted tree (the path) with root i, and we want to
    // order the vertices such that a vertex is visited only if its parent (the neighbor towards i) is visited.
    // But the tree is just a line, so the precedence is: for j < i, all vertices between j and i must be visited before j?
    // Wait: if we start at i, to hit j < i, we must have hit all k between j and i? Not necessarily.
    // The rule: each new target is adjacent to some already hit monster. So to hit j, we need a chain of adjacent hits.
    // Since we start at i, the first time we can hit j is when the interval [j, i] is fully hit.
    // But we could hit some on the right, then come back to left. The only constraint is that the set of hit monsters
    // is always an interval. So the order is exactly a sequence that starts at i and adds elements to the left or right
    // of the current interval. So the time t_j is the step at which j is added.
    // For a fixed i, the set of possible t_j is: t_i = 0. For j < i, t_j must be > t_{j+1}? Actually, if we go left,
    // we must hit i-1 before i-2? Not necessarily: we could hit i-1, then go right to i+1, then left to i-2.
    // The only constraint is that when we hit j, all vertices between j and i must have been hit already?
    // Let's check: suppose i=3, n=5. Order: 3, 4, 2, 5, 1. Is this valid?
    // Start at 3. Interval [3,3]. Next hit 4 (adjacent to 3). Interval [3,4].
    // Next hit 2 (adjacent to 3). Interval [2,4].
    // Next hit 5 (adjacent to 4). Interval [2,5].
    // Next hit 1 (adjacent to 2). Interval [1,5]. Valid.
    // So t_2 = 2, t_1 = 4. The times are not monotonic with distance.
    // But notice: the time t_j is at least the distance from i to j? Distance from 3 to 1 is 2, but t_1=4. So no.
    // Actually, the time t_j is exactly the step when the interval expands to include j.
    // The interval expands one by one. So the number of elements in the interval before j is added is the number of
    // elements that were added before j. The only constraint is that when j is added, all elements between j and the
    // current interval must be already in the interval. Since the interval is always contiguous, this means that
    // when j is added, the interval must be exactly [L, R] and j = L-1 or j = R+1.
    // So the order is a sequence of left/right expansions.
    // This is equivalent to: we have a sequence of n-1 choices (L or R). The time t_j is the step when we choose
    // the direction that adds j.
    // For a fixed start i, the times are determined by the order of expansions.
    // We want to find an order that minimizes max_j (a_j + t_j).
    // This is a known problem: "minimum possible maximum of a_j + t_j for interval expansion".
    // Let's think about the last monster. The last monster is either 1 or n. Its time is n-1.
    // So in any order, the last monster gets time n-1. So x must be at least a_1 + n-1 or a_n + n-1,
    // depending on which is last. But we can choose which one is last by our expansion order.
    // Actually, the last monster is always one of the ends. So the final interval is [1,n].
    // The last step adds either 1 or n. So the last monster is either 1 or n.
    // Therefore, in any valid order, max(a_1 + t_1, a_n + t_n) >= min(a_1, a_n) + n-1? Not exactly.
    // If we make 1 last, t_1 = n-1, so a_1 + n-1. If we make n last, t_n = n-1, so a_n + n-1.
    // We can choose which end is last by our order. So we can achieve max(a_1 + n-1, a_n + something)...
    // But the other end will be hit earlier. For example, if we make 1 last, then n is hit at some time < n-1.
    // So the maximum over all j will be at least max( a_1 + n-1, max_{j} (a_j + t_j) ).
    // We want to minimize this maximum by choosing the order and start i.
    // Let's consider the case where we hit the first monster as the last one (Hint 4,5).
    // That means we choose an order that ends at 1. Then t_1 = n-1. The start i can be anywhere.
    // If we end at 1, the interval expands from i to cover [1,n], and the last step is adding 1.
    // This implies that just before the last step, the interval was [2, n]. So the second-to-last step added either 2 or n?
    // Actually, if the last step adds 1, the previous interval must be [2, n]. So the order must eventually cover [2,n]
    // before finally adding 1. That means all monsters except 1 are hit before 1.
    // So t_j < n-1 for all j != 1.
    // In such an order, what is the best we can do? We want to minimize max_j (a_j + t_j).
    // Since t_1 = n-1, we have a_1 + n-1 as a lower bound for x.
    // For other j, we can choose the order to minimize their a_j + t_j.
    // This is equivalent to: we have a line from 2 to n, we start at some i in [2,n], and we want to hit all of them
    // in some order (interval expansion on [2,n]) and then finally hit 1. The times for j in [2,n] will be their times
    // in the expansion on [2,n] (which is a sequence of length n-1). Then we add 1 at the end.
    // So the problem reduces to: on the subarray a[2..n], we want to find an expansion order (starting at some i in [2,n])
    // that minimizes max_{j=2..n} (a_j + t_j), where t_j are 0..n-2 in some valid order.
    // Then the overall max is max( a_1 + n-1, that value ).
    // Similarly, if we end at n, we get max( a_n + n-1, best on a[1..n-1] ).
    // But we can also end at 1 or n depending on which gives smaller max.
    // However, is it always optimal to have the last monster be one of the ends? Yes, by the rules, the last monster is always an end.
    // So the last monster is either 1 or n. So the answer is min( max(a_1 + n-1, solve(a[2..n])), max(a_n + n-1, solve(a[1..n-1])) )?
    // But wait: the start i can be anywhere. In the case where we end at 1, the start i could be 1? No, if we start at 1, then 1 is first, not last.
    // If we want 1 to be last, we cannot start at 1. So i must be in [2,n].
    // So the subproblem solve(a[L..R]) is: given an array of length m, we want to find an expansion order starting at some index in [L,R]
    // that minimizes max_j (a_j + t_j) where t_j are 0..m-1. This is exactly the same problem but on a subarray.
    // So we can define a recursive function? But n is up to 3e5, so we need O(n) or O(n log n).
    // Let's analyze the subproblem. We want to find min over start i and over valid orders of max_j (a_j + t_j).
    // Notice that the order is an interval expansion. This is equivalent to: we have a sequence of choices left/right.
    // The times t_j are the steps. We want to assign times to minimize the maximum of a_j + t_j.
    // This is similar to: we have a set of items with weights a_j, and we want to order them in a way that respects
    // the "nested intervals" property? Actually, the interval expansion order on a line is exactly the order of
    // a depth-first traversal of a path? No, it's more like: we start at i, then we can go left or right, but we can
    // interleave. The set of possible orders is exactly the set of permutations that are "bitonic" in some sense?
    // Let's think about the times. For a fixed start i, the times t_j satisfy:
    // For any j < k < i, we must have t_j > t_k? Not necessarily. In the example 3,4,2,5,1 with i=3:
    // t_3=0, t_4=1, t_2=2, t_5=3, t_1=4. Here j=1, k=2, i=3: t_1=4 > t_2=2. So t_1 > t_2.
    // j=2, k=3: t_2=2 > t_3=0. So for j < k <= i, t_j > t_k? Actually, t_2 > t_3, t_1 > t_2. So it's decreasing as we go left from i.
    // But wait: could we have t_2 < t_4? t_2=2, t_4=1, so t_2 > t_4. So left side times are not necessarily monotonic with right side.
    // However, there is a known characterization: the order of hitting is a sequence where the indices of hit monsters
    // form a "mountain" shape? Not exactly.
    // Let's consider the last monster again. The last monster is always an end of the current subarray.
    // So if we consider the whole array, the last monster is either 1 or n.
    // If we fix the last monster to be, say, 1, then the problem on [2..n] is independent, except that the times in [2..n]
    // are 0..n-2, and then we add n-1 for 1. So the max over [2..n] is exactly the answer for the subarray [2..n]
    // (with the same definition of answer, i.e., min over start and order of max(a_j + t_j)).
    // So if we define ans(L, R) as the minimum possible max over that subarray, then:
    // ans(1, n) = min( max(a_1 + n-1, ans(2, n)), max(a_n + n-1, ans(1, n-1)) ).
    // But wait: is it always true that the optimal order for the whole array can be obtained by deciding which end is last,
    // and then optimally ordering the rest? The rest is a subarray, and we can start anywhere in that subarray.
    // The times for the subarray are exactly 0..m-1 where m = length of subarray. Then the last end gets time m.
    // So the max over the subarray is exactly ans(subarray) with times 0..m-1.
    // But is it possible that the optimal order for the whole array does NOT have the last monster as the one that gives the max?
    // The answer x must be >= a_j + t_j for all j. The last monster has time n-1. So x >= a_last + n-1.
    // If we choose the last monster to be the one with smaller a_last + n-1, we might get a smaller x.
    // But could there be an order where the maximum is achieved at some interior monster, and both ends have smaller a_j + t_j,
    // and the maximum is smaller than both a_1 + n-1 and a_n + n-1? No, because one of the ends has time n-1.
    // So x >= max(a_1 + n-1, a_n + n-1) is NOT necessarily true, because we can choose which end is last.
    // The last monster is either 1 or n. So x >= min(a_1, a_n) + n-1? Actually, we can choose which one is last.
    // So x >= min_{end in {1,n}} (a_end + n-1) if we can make that end last. But we also have to consider the other end,
    // which will have some time < n-1. So x must be at least the max over all j.
    // So the recurrence ans(L, R) = min( max(a_L + len-1, ans(L+1, R)), max(a_R + len-1, ans(L, R-1)) ) where len = R-L+1.
    // Let's test this recurrence on small examples.
    // Example 1: n=6, a = [2,1,5,6,4,3]
    // Let's compute manually.
    // We need to compute ans(1,6).
    // Base case: len=1, ans(i,i) = a_i (since time 0, x >= a_i).
    // len=2: ans(i,i+1) = min( max(a_i+1, a_{i+1}), max(a_{i+1}+1, a_i) )? Wait, recurrence for len=2:
    // ans(L,R) = min( max(a_L + 1, ans(L+1,R)), max(a_R + 1, ans(L,R-1)) ). For len=2, L+1=R, so ans(L+1,R)=a_R, ans(L,R-1)=a_L.
    // So ans = min( max(a_L+1, a_R), max(a_R+1, a_L) ).
    // For a=[2,1]: max(2+1,1)=3, max(1+1,2)=2 -> min=2. So ans=2. But wait, if we start at 2 (value 1), time 0: damage x. Then hit 1 (value 2), time 1: damage x-1. Need x>=1 and x-1>=2 -> x>=3. If start at 1: x>=2, x-1>=1 -> x>=2. So min x=2. Correct.
    // Now let's compute for example 1 using recurrence.
    // a = [2,1,5,6,4,3]
    // We'll compute DP for all subarrays? That would be O(n^2) which is too slow.
    // But maybe the recurrence simplifies. Notice that ans(L,R) only depends on ans(L+1,R) and ans(L,R-1).
    // This is similar to a game where we take from ends. But we are minimizing the maximum.
    // Let's try to see if there's a greedy approach.
    // Consider the recurrence: ans(L,R) = min( max(a_L + (R-L), ans(L+1,R)), max(a_R + (R-L), ans(L,R-1)) ).
    // Because len-1 = R-L.
    // So ans(L,R) = min( max(a_L + d, ans(L+1,R)), max(a_R + d, ans(L,R-1)) ) where d = R-L.
    // This DP is O(n^2). We need O(n).
    // Let's analyze the structure. The answer for the whole array is the minimum over all valid orders.
    // There is a known solution: the answer is max_{i} (a_i + min(i-1, n-i))? Let's test.
    // For example 1: a = [2,1,5,6,4,3]
    // i=1: 2 + min(0,5)=2
    // i=2: 1 + min(1,4)=2
    // i=3: 5 + min(2,3)=7
    // i=4: 6 + min(3,2)=8
    // i=5: 4 + min(4,1)=5
    // i=6: 3 + min(5,0)=3
    // max = 8. Output is 8. Matches!
    // Example 2: [4,4,4,4,4] n=5
    // i=1: 4+0=4
    // i=2: 4+1=5
    // i=3: 4+2=6
    // i=4: 4+1=5
    // i=5: 4+0=4
    // max = 6? But output is 8. So that formula gives 6, but answer is 8. So not correct.
    // Wait, let's recompute example 2 manually. n=5, all 4.
    // We need min x such that there exists start i and order where damage x, x-1, x-2, x-3, x-4 kills all.
    // If we start at 3 (middle), we can order: 3,2,4,1,5. Times: 0,1,2,3,4.
    // Damages: x, x-1, x-2, x-3, x-4.
    // Need x>=4, x-1>=4 -> x>=5, x-2>=4 -> x>=6, x-3>=4 -> x>=7, x-4>=4 -> x>=8. So x>=8.
    // If we start at 2: order 2,1,3,4,5. Times: 0,1,2,3,4. x>=4, x-1>=4->x>=5, x-2>=4->x>=6, x-3>=4->x>=7, x-4>=4->x>=8. Same.
    // If we start at 1: order 1,2,3,4,5. Times: 0,1,2,3,4. x>=4, x-1>=4->5, x-2>=4->6, x-3>=4->7, x-4>=4->8. x=8.
    // So answer is 8.
    // My formula max(a_i + min(i-1, n-i)) gives 6, which is wrong.
    // So the simple formula is not correct.
    // Let's go back to the recurrence. For all 4, n=5.
    // Compute ans(1,5) using recurrence:
    // Base: len=1: ans(i,i)=4.
    // len=2: ans(1,2)=min(max(4+1,4), max(4+1,4))=min(5,5)=5.
    // ans(2,3)=5, ans(3,4)=5, ans(4,5)=5.
    // len=3: ans(1,3)=min(max(4+2, ans(2,3)), max(4+2, ans(1,2))) = min(max(6,5), max(6,5)) = min(6,6)=6.
    // ans(2,4)=min(max(4+2, ans(3,4)), max(4+2, ans(2,3))) = min(6,6)=6.
    // ans(3,5)=min(max(4+2, ans(4,5)), max(4+2, ans(3,4))) = 6.
    // len=4: ans(1,4)=min(max(4+3, ans(2,4)), max(4+3, ans(1,3))) = min(max(7,6), max(7,6)) = 7.
    // ans(2,5)=min(max(4+3, ans(3,5)), max(4+3, ans(2,4))) = 7.
    // len=5: ans(1,5)=min(max(4+4, ans(2,5)), max(4+4, ans(1,4))) = min(max(8,7), max(8,7)) = 8.
    // So recurrence gives 8. Correct.
    // So the recurrence is correct. But we need to compute ans(1,n) efficiently.
    // Notice that the recurrence is: ans(L,R) = min( max(a_L + d, ans(L+1,R)), max(a_R + d, ans(L,R-1)) ) with d = R-L.
    // This is similar to a known DP for "minimum maximum damage" on a line.
    // Let's try to understand what ans(L,R) represents. It's the minimum x such that we can start somewhere in [L,R]
    // and hit all monsters in [L,R] in some order (interval expansion) with damages x, x-1, ..., x-(len-1).
    // The recurrence says: the last monster hit in [L,R] must be either L or R. If it's L, then the damage to L is x - (len-1),
    // so we need x >= a_L + len-1. And the rest [L+1,R] must be hit with damages x, x-1, ..., x-(len-2) (since the last step is for L).
    // But wait: if L is last, then the order on [L+1,R] uses times 0..len-2. The damages are x, x-1, ..., x-(len-2).
    // So the condition for [L+1,R] is exactly that there exists an order on [L+1,R] with max(a_j + t_j) <= x, where t_j are 0..len-2.
    // That is exactly ans(L+1,R) <= x. So we need x >= max(a_L + len-1, ans(L+1,R)).
    // Similarly if R is last.
    // So the recurrence is correct.
    // Now, how to compute ans(1,n) in O(n)?
    // Let's expand the recurrence. It looks like we are building the answer by considering the order of removing ends.
    // Actually, the process of choosing which end is last is equivalent to: we have a sequence of choices of which end to remove last.
    // This is like we are constructing a binary tree of removals.
    // But we can think of it as: we want to find a permutation of the indices that is a valid order (interval expansion).
    // There is a known greedy solution: the answer is the maximum over i of (a_i + min(i-1, n-i))? That didn't work for all 4.
    // Wait, for all 4, min(i-1, n-i) for i=3 is 2, so 4+2=6, but answer is 8. So that's not it.
    // Let's think differently. The recurrence is similar to the "optimal binary search tree" but with max.
    // Notice that ans(L,R) is non-decreasing with respect to interval length? Actually, as length increases, the added constant d increases.
    // Let's try to find a pattern. For a fixed array, we can compute ans(1,n) by simulating the recurrence from small to large intervals.
    // But O(n^2) is too slow. We need a faster way.
    // Observe that the recurrence only depends on the values a_i and the distance from the ends.
    // Maybe we can binary search the answer x, and check if there exists a valid order with max <= x.
    // For a given x, we want to know if there is a start i and an order such that a_j + t_j <= x for all j.
    // This is equivalent to: can we assign times t_j (0..n-1) that form a valid interval expansion order, with t_j <= x - a_j.
    // Since t_j are exactly 0..n-1 in some order, we need to find a permutation that is a valid order and respects t_j <= x - a_j.
    // But t_j are determined by the order. This is like scheduling with deadlines.
    // Let's think about the condition for a fixed x. We want to hit all monsters. The last monster must have a_j + n-1 <= x.
    // So we need a_1 + n-1 <= x or a_n + n-1 <= x? Actually, the last monster is either 1 or n. So at least one of them must satisfy a + n-1 <= x.
    // But that's necessary, not sufficient.
    // Consider the process backwards: we start with the last monster (which is an end), remove it, and then the problem reduces to the subarray.
    // So a valid order exists iff we can recursively remove ends such that at each step, the removed end has a_i + (current length - 1) <= x.
    // Because if we consider the reverse order: we start with the full array, and we repeatedly remove the last monster hit.
    // The last monster hit is an end. So we remove an end, and the remaining array is a subarray.
    // The damage that monster received is x - (len-1). So we need a_end + len-1 <= x.
    // Then we are left with a subarray of length len-1, and the damages for that subarray are x, x-1, ..., x-(len-2).
    // So the condition for the subarray is the same: there must exist an order on the subarray with max <= x.
    // So the condition for a given x is: can we reduce the array to empty by repeatedly removing an end (left or right) such that
    // at the time of removal (when current length is L), the removed end satisfies a_end + L - 1 <= x.
    // This is exactly the condition that there exists a sequence of removals from ends such that each removed element satisfies the inequality.
    // This is a known problem: given an array, can we remove all elements by popping from ends, with the condition that when we pop an element,
    // its value + (current length - 1) <= x. We want to know if it's possible.
    // And we want the minimum x.
    // This is much easier! Because the order of removals is exactly the reverse of the hitting order.
    // In the hitting order, we start with one monster, then expand. In reverse, we start with all monsters, and remove the last hit, which is an end.
    // So the condition is: there exists a sequence of removals from ends such that for each removal, if the current array length is L,
    // the removed end (say value v) satisfies v + L - 1 <= x.
    // And we want to find the minimum x for which this is possible.
    // Note that the start index i in the forward process corresponds to the last remaining element in the reverse process.
    // So the reverse process removes all elements except one, which is the start.
    // So we need to check if we can remove n-1 elements from the ends, leaving one element, such that each removal satisfies the condition.
    // This is a standard greedy check: we can simulate the process. For a given x, we can try to remove ends as long as they satisfy the condition.
    // But we have a choice: we can remove left or right. We want to see if there is a sequence that leaves a non-empty array (or empty? Actually we remove n-1 elements, leaving 1).
    // So we need to check if there exists a sequence of removals that reduces the array to a single element, with all removals valid.
    // This is equivalent to: can we remove elements from ends in some order such that at each step, the removed element satisfies v + (current length - 1) <= x.
    // Notice that the condition depends on the current length. As we remove elements, the length decreases, so the condition becomes easier (L-1 decreases).
    // So if we can remove an element now, we could also remove it later (since L-1 would be smaller). So we should remove elements as early as possible?
    // Actually, if an element satisfies the condition now, it will also satisfy it later because L-1 decreases. So if we can remove it now, we might as well remove it.
    // But we have two ends. We can choose which one to remove. The greedy strategy: if both ends satisfy, it doesn't matter which we remove?
    // Not necessarily, because removing one end might expose a new end that was previously interior, and that new end might not satisfy the condition.
    // But the new end's value is fixed, and the length is now smaller, so it might be easier to satisfy later.
    // Actually, if we remove an end, the new end (which was adjacent) now has a smaller L-1, so its condition is easier.
    // So if we can remove an end now, we should remove it, because delaying might not help (the other end might become harder? No, L decreases, so condition becomes easier).
    // Wait, the condition is v + L - 1 <= x. As L decreases, L-1 decreases, so the condition becomes easier to satisfy.
    // So if an end satisfies the condition at current length L, it will also satisfy at any smaller length.
    // Therefore, if we can remove an
