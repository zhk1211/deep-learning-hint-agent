#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n, m;
        cin >> n >> m;
        vector<int> S(m);
        for (int i = 0; i < m; ++i) {
            cin >> S[i];
        }

        if (m == 1) {
            if (n == 1) {
                cout << S[0] << '\n';
            } else {
                cout << -1 << '\n';
            }
            continue;
        }

        // We need to assign a[1..n] from S
        // Condition: a[gcd(i,j)] != gcd(a[i], a[j]) for all i<j
        // Key observation: For any i, j with gcd(i,j)=1, we need a[1] != gcd(a[i], a[j]).
        // Since a[1] is fixed, we must avoid having gcd(a[i], a[j]) = a[1] for any coprime pair.
        // If we set a[1] = max(S), then for any other elements we must ensure that no two have gcd equal to max(S).
        // A simple construction: set a[1] = max(S), and for all other positions, use the second largest element from S.
        // Check if this works: For i,j with gcd(i,j)=1, a[1] = max(S), gcd(a[i],a[j]) = second_max (if both are second_max) or gcd(second_max, something else).
        // If second_max does not divide max(S) or second_max != max(S), it's fine. But we need to ensure condition holds for all pairs.
        // Actually, the known solution: if m >= 2, we can always construct by placing the largest element at position 1, and the second largest at all other positions.
        // But we must verify the condition for all pairs, not just coprime ones.
        // Let's test: a[1] = largest, a[i] = second_largest for i>1.
        // For any i,j: if i=1, j>1: gcd(1,j)=1, a[1] = L, gcd(a[1],a[j]) = gcd(L, X) where X = second_largest.
        // We need L != gcd(L, X). Since X < L, gcd(L, X) <= X < L, so L != gcd(L, X) holds.
        // For i,j > 1: gcd(i,j) could be 1 or >1.
        // If gcd(i,j)=1, a[1] = L, gcd(a[i],a[j]) = gcd(X, X) = X. Need L != X, which is true since L > X.
        // If gcd(i,j)=g>1, then a[g] is either L (if g=1) or X (if g>1). Since g>1, a[g] = X.
        // gcd(a[i],a[j]) = gcd(X, X) = X. So a[g] = X, gcd = X, they are equal! This violates condition.
        // So simple construction fails for pairs with gcd>1.
        // We need a more careful assignment.

        // Let's think differently: The condition must hold for all pairs.
        // Consider the case m = n. Then we can just output S in some order? No, condition still applies.
        // Hint 2: What should be the answer when m = n? Probably we can assign a[i] = S[i-1]? Not necessarily.
        // Let's analyze small n.
        // For n=2, m=2: S={x,y} with x<y. We need a[1]!=gcd(a[1],a[2]) and a[1]!=gcd(a[1],a[2]) (same). So a[1] != gcd(a[1],a[2]).
        // If we set a[1]=y, a[2]=x, then gcd(y,x) <= x < y, so condition holds. Lexicographically largest: a[1]=y, a[2]=x. Works.
        // For n=3, m=3: S={1,2,3}. Try lexicographically largest: a[1]=3. Then we need to assign a[2], a[3].
        // Pairs: (1,2): a[1]=3 != gcd(a[1],a[2]) = gcd(3,a[2]) -> always true if a[2] not multiple of 3? Actually gcd(3,a[2]) <= a[2] < 3, so true.
        // (1,3): similar.
        // (2,3): gcd(2,3)=1, so a[1]=3 != gcd(a[2],a[3]). So we need gcd(a[2],a[3]) != 3. Since max element is 3, gcd can't be 3 unless both are 3. So we just avoid both being 3.
        // Lexicographically largest: a[2]=3, a[3]=2? Then gcd(3,2)=1 !=3 ok. But check (2,3) with gcd(2,3)=1: a[1]=3, gcd(a[2],a[3])=1, 3!=1 ok.
        // So array [3,3,2] works? Check pair (1,2): a[1]=3, gcd(3,3)=3 -> equal! Violates. So a[2] cannot be 3 if a[1]=3 because gcd(1,2)=1, a[1]=3, gcd(a[1],a[2])=3. So a[2] must not equal a[1] if a[1] is used at position 1? Actually condition: a[gcd(1,2)] = a[1] != gcd(a[1],a[2]). If a[1]=a[2]=3, then gcd(3,3)=3, so a[1]=3 equals gcd=3 -> violation. So a[2] cannot be equal to a[1] if a[1] is the max? Not necessarily, but if a[1]=a[2]=x, then gcd(x,x)=x, so a[1]=x equals gcd=x -> violation. So a[2] must be different from a[1] if we put a[1] at position 1? Actually a[1] is fixed. For pair (1,2), a[1] != gcd(a[1],a[2]). If a[1]=a[2], gcd=a[1], violation. So a[2] != a[1]. Similarly, for any j, if gcd(1,j)=1 (which is all j>1), we need a[1] != gcd(a[1],a[j]). If a[j] is a multiple of a[1], then gcd(a[1],a[j]) = a[1], violation. So a[j] must not be a multiple of a[1]. Also if a[j] = a[1], violation. So for all j>1, a[j] cannot be a multiple of a[1] (including a[1] itself). So a[1] must be chosen such that there exists at least one element in S not dividing it? Actually we need to assign all other positions with elements that are not multiples of a[1]. Since we want lexicographically largest, we want a[1] as large as possible. But if the largest element is chosen as a[1], then all other elements must not be multiples of it. Since all elements are <= n, and the largest element is max(S), any other element is <= max(S). The only multiples of max(S) in S could be max(S) itself. So if we choose a[1] = max(S), then we just need to avoid using max(S) for any other position. That's possible if m>=2. So a[1] = max(S) is valid if we have another element.
        // Now for positions i>1, we want lexicographically largest, so we want to put the largest possible element at each position from left to right, subject to constraints.
        // Constraints for i>1: For any j>i, we need a[gcd(i,j)] != gcd(a[i],a[j]). Also for pairs involving 1, already handled by a[1] choice and a[i] not multiple of a[1].
        // This seems complicated. Let's look for patterns or known solution.
        // Hint 4: What happens when m=1? Only possible if n=1, else -1. We handled that.
        // Hint 5: Can you solve when m=n? Maybe we can set a[i] = something like S[n-i]? Not sure.
        // Let's search for a general construction.
        // Consider the condition: a[gcd(i,j)] != gcd(a[i],a[j]).
        // If we set all a[i] for i>=2 to the same value x, and a[1] = y, with y > x and x not dividing y.
        // Check pair (i,j) with i,j>=2, gcd(i,j)=g>=1.
        // If g=1, a[1]=y, gcd(a[i],a[j])=gcd(x,x)=x. Need y != x, true.
        // If g>1, then a[g] = x (since g>=2), gcd(a[i],a[j])=x. So a[g] = x equals gcd = x -> violation!
        // So we cannot have all i>=2 equal.
        // What if we alternate? Or use a special assignment based on prime factors?
        // Another idea: Since condition only restricts pairs, maybe we can assign a[i] based on the number of prime factors or something.
        // Let's think about the case m=n. We have all numbers 1..n. We need a permutation of 1..n.
        // Is there a known construction? For n=3, we found [3,2,1]? Check: a=[3,2,1].
        // (1,2): a[1]=3, gcd(3,2)=1 -> ok.
        // (1,3): a[1]=3, gcd(3,1)=1 -> ok.
        // (2,3): gcd(2,3)=1, a[1]=3, gcd(2,1)=1 -> ok.
        // So [3,2,1] works. Lexicographically largest? [3,2,1] is larger than [3,1,2]? Compare: first element same, second: 2 vs 1, so [3,2,1] larger. Is [3,3,2] allowed? No, a[2] cannot be 3. So [3,2,1] is max? What about [3,2,2]? 2 is not in S if S={1,2,3}? It is. But we need all elements from S, but can repeat? The problem says a_i in S, but S is a set of unique integers. It doesn't say we must use each exactly once. We can repeat. So for m=3, n=3, S={1,2,3}, we can use [3,2,2]? Check: (1,2): a[1]=3, gcd(3,2)=1 ok. (1,3): a[1]=3, gcd(3,2)=1 ok. (2,3): gcd(2,3)=1, a[1]=3, gcd(2,2)=2, 3!=2 ok. So [3,2,2] works and is lexicographically larger than [3,2,1]. Can we do [3,3,2]? No, a[2]=3 fails. So [3,2,2] is valid. What about [3,2,3]? a[3]=3, check (1,3): a[1]=3, gcd(3,3)=3 -> fail. So max is [3,2,2]. So we can repeat elements.
        // So the problem allows repetitions.
        // Now, back to general construction. We want lexicographically largest. So we want a[1] as large as possible. As argued, a[1] can be max(S) if there is another element not equal to max(S) (i.e., m>=2). If m=1, only n=1 works.
        // So set a[1] = max(S).
        // Now for i=2..n, we want to assign the largest possible element from S such that all constraints with previous indices are satisfied, and also we don't ruin future possibilities? Since we want lexicographically largest, we can greedily assign the largest possible element at each position i, provided that there exists some completion for the rest. But maybe the constraints are local enough that greedy works.
        // Let's analyze constraints for a[i] when assigning position i.
        // For any j < i, we need a[gcd(i,j)] != gcd(a[i], a[j]).
        // Also for future j > i, we will need a[gcd(i,j)] != gcd(a[i], a[j]). But at the time of assigning a[i], we don't know a[j]. However, we can try to ensure that no matter what a[j] is, the condition can be satisfied. This seems hard.
        // Alternative: Maybe there is a simple pattern. Let's look at sample 1: n=6, m=3, S={3,4,6}. Output: 6 4 4 3 4 3.
        // a = [6,4,4,3,4,3]
        // Check conditions? They claim it works.
        // Let's see pattern: a[1]=6 (max). a[2]=4, a[3]=4, a[4]=3, a[5]=4, a[6]=3.
        // Notice that for i>=2, the values are either 4 or 3. 4 is the second largest, 3 is the smallest.
        // Maybe the construction is: a[1] = max(S). For i>1, if i is prime? No, 2,3,5 are prime but got 4,4,4; 4 and 6 are composite got 3,3.
        // Let's list indices: 1:6, 2:4, 3:4, 4:3, 5:4, 6:3.
        // Maybe it's based on the smallest prime factor? 2:4, 3:4, 4:3, 5:4, 6:3. Not clear.
        // Another idea: For i>1, we can set a[i] = second_max if i is odd? 2 even->4, 3 odd->4, 4 even->3, 5 odd->4, 6 even->3. No.
        // Let's check gcd conditions for this array:
        // We need a[gcd(i,j)] != gcd(a[i],a[j]).
        // If we set a[i] for i>1 to be either x or y, with x > y, and x not multiple of a[1], y not multiple of a[1].
        // In sample, a[1]=6, x=4, y=3. 4 and 3 are not multiples of 6.
        // Now consider pair (i,j) with i,j>1. Let g = gcd(i,j).
        // If g=1, a[1]=6, gcd(a[i],a[j]) is gcd of two numbers from {4,3}. Possible gcds: gcd(4,4)=4, gcd(4,3)=1, gcd(3,3)=3. None equal 6. So condition holds.
        // If g>1, then a[g] is either 4 or 3 (since g>=2). We need a[g] != gcd(a[i],a[j]).
        // So we need to assign values to indices such that for any pair with gcd=g>1, the gcd of their values is not equal to the value at g.
        // This is a kind of graph coloring or assignment problem on the divisibility poset.
        // Notice that if we assign a[i] based on the number of distinct prime factors? Or maybe we can set a[i] = second_max for all i that are not multiples of something?
        // Let's test a simpler construction: a[1] = max(S). For i>=2, set a[i] = second_max. Does it work? We already saw it fails for g>1 because a[g] = second_max and gcd(second_max, second_max) = second_max. So we need to break that.
        // To avoid a[g] = gcd(a[i],a[j]) when both are second_max, we can ensure that for any g>1, there is at least one pair (i,j) with gcd(i,j)=g? Actually condition must hold for ALL pairs. So if there exists any pair with gcd(i,j)=g>1 and both a[i]=a[j]=second_max, then a[g] must not be second_max. So if we set a[g] = something else (like the smallest element) for all g>1, then we can set a[i]=second_max for all i that are not someone's gcd? But every index >1 can be a gcd of some pair (e.g., (g, 2g)). So if we set a[g] = smallest for all g>1, then for pair (g, 2g), gcd=g, a[g]=smallest, gcd(a[g],a[2g]) = gcd(smallest, a[2g]). If a[2g] is second_max, then gcd(smallest, second_max) could be something. We need that gcd(smallest, second_max) != smallest. This is true if smallest does not divide second_max? Not necessarily. But we can choose smallest and second_max such that smallest does not divide second_max? In sample, smallest=3, second_max=4, gcd(3,4)=1 !=3. So it works.
        // Let's try: a[1] = max. For all i from 2 to n, if i is a "gcd" of some pair? Actually every i>1 is a gcd of (i, 2i) if 2i<=n. So we cannot set all i>1 to second_max. We need to assign some indices to a different value to break the equality.
        // In sample, indices 4 and 6 got 3, others got 4. 4 and 6 are composite? 2,3,5 got 4. So primes got second_max, composites got smallest? 4 is composite, 6 composite. 2,3,5 prime. What about 1? 1 is special. So pattern: a[1]=max. For i>1, if i is prime, a[i]=second_max; if i is composite, a[i]=smallest. Let's test this pattern on n=6, S={3,4,6}: max=6, second_max=4, smallest=3. Primes: 2,3,5 -> 4. Composites: 4,6 -> 3. Matches sample!
        // Check if this works in general. Let's test on n=4, S={1,2,3,4} (m=n). Pattern: max=4, second_max=3, smallest=1? Wait S is sorted: 1,2,3,4. second_max=3, smallest=1. Primes: 2,3 -> a[2]=3, a[3]=3. Composites: 4 -> a[4]=1. Array: [4,3,3,1]. Check conditions:
        // (1,2): a[1]=4, gcd(4,3)=1 ok.
        // (1,3): a[1]=4, gcd(4,3)=1 ok.
        // (1,4): a[1]=4, gcd(4,1)=1 ok.
        // (2,3): gcd=1, a[1]=4, gcd(3,3)=3 ok.
        // (2,4): gcd=2, a[2]=3, gcd(a[2],a[4])=gcd(3,1)=1. 3!=1 ok.
        // (3,4): gcd=1, a[1]=4, gcd(3,1)=1 ok.
        // Works! Is it lexicographically largest? Could we have [4,3,3,2]? 2 is in S. Check (2,4): a[2]=3, gcd(3,2)=1, 3!=1 ok. (3,4): gcd(3,4)=1, a[1]=4, gcd(3,2)=1 ok. So [4,3,3,2] works and is larger than [4,3,3,1]. So our pattern is not optimal for lexicographically largest.
        // We want to put the largest possible element at each position. For position 4, we could put 2 instead of 1. So we need a better assignment.
        // Let's analyze constraints for each position independently if possible.
        // For i>1, what values can a[i] take? It must not be a multiple of a[1] (as argued). Also, for any j < i, we need a[gcd(i,j)] != gcd(a[i],a[j]). Since we are assigning left to right, when assigning a[i], a[j] for j<i are already fixed. So we can check this condition for all j<i. But we also need to ensure that future assignments are possible. However, if we just greedily pick the largest available element from S that satisfies all constraints with previous indices, and if we can always complete the rest, then greedy works. Is it always possible to complete? Maybe we can always fill the rest with the smallest element? Let's test.
        // Suppose we have S with at least 2 elements. Let max = S.back(), min = S[0] (if m>=2, min < max). We know a[1] = max. For i>1, we want to pick the largest element from S that is not a multiple of max (so any element < max is fine since max is the largest, no other element can be a multiple of max except max itself). So all other elements are allowed by the multiple condition. Now for j<i, we need a[gcd(i,j)] != gcd(a[i], a[j]). Since a[gcd(i,j)] is already fixed (as gcd(i,j) < i), we can just check if the candidate a[i] satisfies this for all j<i. If we pick the largest such candidate, can we always fill the remaining positions with some element? The remaining positions will have their own constraints with previous indices, but they can always use the smallest element? Not necessarily, because the smallest element might cause a[gcd(i,j)] == gcd(smallest, a[j]) for some future j. But if we set all remaining to the smallest element, we need to check if that final array is valid. If it is, then greedy choice is safe.
        // Let's test this greedy on n=4, S={1,2,3,4}:
        // a[1]=4.
        // i=2: candidates from S: 3,2,1 (4 is multiple of 4? 4 is multiple of 4, so excluded). Check j=1: gcd(1,2)=1, a[1]=4, gcd(a[2],a[1]) = gcd(candidate,4). We need 4 != gcd(candidate,4). For candidate=3: gcd(3,4)=1 !=4 ok. candidate=2: gcd(2,4)=2 !=4 ok. candidate=1: gcd(1,4)=1 !=4 ok. Largest is 3. So a[2]=3.
        // i=3: candidates: 3,2,1. Check j=1: gcd(1,3)=1, a[1]=4, gcd(candidate,4) !=4. All ok. j=2: gcd(2,3)=1, a[1]=4, gcd(candidate, a[2]=3) !=4. For candidate=3: gcd(3,3)=3 !=4 ok. candidate=2: gcd(2,3)=1 !=4 ok. candidate=1: gcd(1,3)=1 !=4 ok. Largest is 3. So a[3]=3.
        // i=4: candidates: 3,2,1. Check j=1: gcd(1,4)=1, a[1]=4, gcd(candidate,4) !=4. All ok. j=2: gcd(2,4)=2, a[2]=3, gcd(candidate, a[2]=3) !=3. For candidate=3: gcd(3,3)=3 ==3 -> fail. candidate=2: gcd(2,3)=1 !=3 ok. candidate=1: gcd(1,3)=1 !=3 ok. j=3: gcd(3,4)=1, a[1]=4, gcd(candidate, a[3]=3) !=4. candidate=2: gcd(2,3)=1 !=4 ok. candidate=1: gcd(1,3)=1 !=4 ok. Largest valid is 2. So a[4]=2.
        // Result: [4,3,3,2] which is valid and lexicographically larger than [4,3,3,1]. So greedy works for this case.
        // Let's test on sample 1: n=6, S={3,4,6}. max=6, others: 4,3.
        // a[1]=6.
        // i=2: candidates: 4,3. j=1: gcd(1,2)=1, a[1]=6, gcd(c,6)!=6. 4: gcd(4,6)=2!=6 ok. 3: gcd(3,6)=3!=6 ok. Largest 4 -> a[2]=4.
        // i=3: candidates: 4,3. j=1: gcd(1,3)=1, a[1]=6, gcd(c,6)!=6 ok. j=2: gcd(2,3)=1, a[1]=6, gcd(c,4)!=6. 4: gcd(4,4)=4!=6 ok. 3: gcd(3,4)=1!=6 ok. Largest 4 -> a[3]=4.
        // i=4: candidates: 4,3. j=1: ok. j=2: gcd(2,4)=2, a[2]=4, gcd(c,4)!=4. 4: gcd(4,4)=4 ==4 fail. 3: gcd(3,4)=1 !=4 ok. j=3: gcd(3,4)=1, a[1]=6, gcd(3,4)=1!=6 ok. So only 3 valid -> a[4]=3.
        // i=5: candidates: 4,3. j=1..4:
        // j=1: ok.
        // j=2: gcd(2,5)=1, a[1]=6, gcd(c,4)!=6. 4: gcd(4,4)=4!=6 ok. 3: gcd(3,4)=1!=6 ok.
        // j=3: gcd(3,5)=1, a[1]=6, gcd(c,4)!=6 ok.
        // j=4: gcd(4,5)=1, a[1]=6, gcd(c,3)!=6. 4: gcd(4,3)=1!=6 ok. 3: gcd(3,3)=3!=6 ok.
        // Largest 4 -> a[5]=4.
        // i=6: candidates: 4,3. j=1..5:
        // j=1: ok.
        // j=2: gcd(2,6)=2, a[2]=4, gcd(c,4)!=4. 4: gcd(4,4)=4 fail. 3: gcd(3,4)=1 ok.
        // j=3: gcd(3,6)=3, a[3]=4, gcd(c,4)!=4. 3: gcd(3,4)=1 ok.
        // j=4: gcd(4,6)=2, a[2]=4, gcd(c,3)!=4. 3: gcd(3,3)=3!=4 ok.
        // j=5: gcd(5,6)=1, a[1]=6, gcd(c,4)!=6. 3: gcd(3,4)=1 ok.
        // So only 3 valid -> a[6]=3.
        // Result: [6,4,4,3,4,3] matches sample!
        // So greedy works for these cases.
        // But is greedy always correct? We need to ensure that when we pick the largest valid element at position i, there always exists a valid completion for the rest. If we can always complete with the smallest element (or some element), then greedy is safe. Let's think: after assigning a[i], the remaining positions are >i. Their constraints only involve indices <= them. If we set all remaining positions to the smallest element in S (which is not a multiple of a[1]), will it satisfy all constraints? Not necessarily, because for a future position k, we need a[gcd(k, j)] != gcd(a[k], a[j]) for all j<k. If we set a[k] = min, then for j where a[j] is something, we need a[gcd(k,j)] != gcd(min, a[j]). Since a[gcd(k,j)] is already fixed (as gcd(k,j) < k), this might fail. But we can always choose a[k] not as min but as some element that works. The question is: could it be that at some position, no element works? If that happens, our greedy choice at an earlier position might have been wrong. But maybe the constraints are such that if there is any solution, the greedy algorithm finds one. This is typical for lexicographically largest constructions where constraints are "local" in some sense.
        // Let's try to find a counterexample where greedy fails.
        // Consider n=5, S={1,2,3,4,5} (m=n). Greedy:
        // a[1]=5.
        // i=2: candidates 4,3,2,1. Check j=1: gcd(1,2)=1, a[1]=5, gcd(c,5)!=5. All ok since c<5. Largest 4 -> a[2]=4.
        // i=3: candidates 4,3,2,1. j=1: ok. j=2: gcd(2,3)=1, a[1]=5, gcd(c,4)!=5. All ok. Largest 4 -> a[3]=4.
        // i=4: candidates 4,3,2,1. j=1: ok. j=2: gcd(2,4)=2, a[2]=4, gcd(c,4)!=4. c=4: gcd(4,4)=4 fail. c=3: gcd(3,4)=1 ok. c=2: gcd(2,4)=2 ok. c=1: gcd(1,4)=1 ok. j=3: gcd(3,4)=1, a[1]=5, gcd(c,4)!=5. All ok. Largest valid is 3? Wait check c=3: j=2 gives gcd(3,4)=1 !=4 ok. So 3 is valid. c=2: j=2 gives gcd(2,4)=2 !=4 ok. So 2 is also valid. Largest is 3. So a[4]=3.
        // i=5: candidates 4,3,2,1. Check j=1..4:
        // j=1: ok.
        // j=2: gcd(2,5)=1, a[1]=5, gcd(c,4)!=5 ok.
        // j=3: gcd(3,5)=1, a[1]=5, gcd(c,4)!=5 ok.
        // j=4: gcd(4,5)=1, a[1]=5, gcd(c,3)!=5 ok.
        // So all candidates are valid? Check j=2: gcd(2,5)=1, condition is a[1]=5 != gcd(c,4). For c=4: gcd(4,4)=4 !=5 ok. c=3: gcd(3,4)=1 !=5 ok. c=2: gcd(2,4)=2 !=5 ok. c=1: gcd(1,4)=1 !=5 ok. So all valid. Largest is 4. So a[5]=4.
        // Array: [5,4,4,3,4]. Check if valid:
        // (1,2): a[1]=5, gcd(5,4)=1 ok.
        // (1,3): 5, gcd(5,4)=1 ok.
        // (1,4): 5, gcd(5,3)=1 ok.
        // (1,5): 5, gcd(5,4)=1 ok.
        // (2,3): gcd=1, a[1]=5, gcd(4,4)=4 ok.
        // (2,4): gcd=2, a[2]=4, gcd(4,3)=1, 4!=1 ok.
        // (2,5): gcd=1, a[1]=5, gcd(4,4)=4 ok.
        // (3,4): gcd=1, a[1]=5, gcd(4,3)=1 ok.
        // (3,5): gcd=1, a[1]=5, gcd(4,4)=4 ok.
        // (4,5): gcd=1, a[1]=5, gcd(3,4)=1 ok.
        // All good. So greedy works.
        // What about a case where S is missing some numbers? E.g., n=6, S={2,3,6}. max=6, others: 3,2.
        // Greedy:
        // a[1]=6.
        // i=2: candidates 3,2. j=1: gcd(1,2)=1, a[1]=6, gcd(c,6)!=6. 3: gcd(3,6)=3!=6 ok. 2: gcd(2,6)=2!=6 ok. Largest 3 -> a[2]=3.
        // i=3: candidates 3,2. j=1: ok. j=2: gcd(2,3)=1, a[1]=6, gcd(c,3)!=6. 3: gcd(3,3)=3!=6 ok. 2: gcd(2,3)=1!=6 ok. Largest 3 -> a[3]=3.
        // i=4: candidates 3,2. j=1: ok. j=2: gcd(2,4)=2, a[2]=3, gcd(c,3)!=3. 3: gcd(3,3)=3 fail. 2: gcd(2,3)=1 ok. j=3: gcd(3,4)=1, a[1]=6, gcd(2,3)=1!=6 ok. So a[4]=2.
        // i=5: candidates 3,2. j=1..4:
        // j=1: ok.
        // j=2: gcd(2,5)=1, a[1]=6, gcd(c,3)!=6. 3: gcd(3,3)=3!=6 ok. 2: gcd(2,3)=1!=6 ok.
        // j=3: gcd(3,5)=1, a[1]=6, gcd(c,3)!=6 ok.
        // j=4: gcd(4,5)=1, a[1]=6, gcd(c,2)!=6. 3: gcd(3,2)=1!=6 ok. 2: gcd(2,2)=2!=6 ok.
        // Largest 3 -> a[5]=3.
        // i=6: candidates 3,2. j=1..5:
        // j=1: ok.
        // j=2: gcd(2,6)=2, a[2]=3, gcd(c,3)!=3. 3: gcd(3,3)=3 fail. 2: gcd(2,3)=1 ok.
        // j=3: gcd(3,6)=3, a[3]=3, gcd(c,3)!=3. 2: gcd(2,3
