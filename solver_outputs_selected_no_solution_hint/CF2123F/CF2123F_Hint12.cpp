#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<int> p(n + 1);
        p[1] = 1;
        if (n % 2 == 0) {
            for (int i = 2; i <= n; i += 2) {
                p[i] = i + 1;
                p[i + 1] = i;
            }
        } else {
            if (n == 3) {
                p[1] = 1;
                p[2] = 2;
                p[3] = 3;
            } else {
                p[1] = 1;
                p[2] = 4;
                p[3] = 6;
                p[4] = 2;
                p[5] = 8;
                p[6] = 3;
                p[7] = 10;
                p[8] = 5;
                p[9] = 12;
                p[10] = 7;
                p[11] = 14;
                p[12] = 9;
                p[13] = 11;
                // Actually, we need a general construction for odd n >= 5
                // Let's reset and do it properly
                // For odd n >= 5, we can set p[1]=1, p[2]=4, p[3]=6, p[4]=2, p[5]=8, p[6]=3,
                // then for i from 7 to n step 2: p[i] = i+1, p[i+1] = i, but careful with n odd.
                // Actually, the sample for n=13 is: 1 12 9 6 10 8 7 4 3 5 11 2 13
                // Let's analyze: 1 is fixed. Then pairs: (2,12), (3,9), (4,6), (5,10), (6,8), (7,7)? no 7 is fixed? Wait sample: index 7 has 7? No, sample: 1 12 9 6 10 8 7 4 3 5 11 2 13
                // indices: 1:1, 2:12, 3:9, 4:6, 5:10, 6:8, 7:7, 8:4, 9:3, 10:5, 11:11, 12:2, 13:13
                // Fixed points: 1,7,11,13? Actually 7 is fixed, 11 is fixed, 13 is fixed. That's 4 fixed points. But sample says only two fixed points: 1 and 5? Wait sample note says: "we see that there are only two fixed points, namely, 1 and 5." That's for n=6. For n=13, output is 1 12 9 6 10 8 7 4 3 5 11 2 13. Let's check fixed points: p[1]=1, p[7]=7, p[11]=11, p[13]=13. That's 4 fixed points. But is it minimal? The problem says "minimum number of fixed points". For odd n, maybe we can't do better than 1? Wait, hint 5: "Is it possible to construct a good permutation with less than two fixed points?" For n=2, output is 1 2 (two fixed points). For n=3, output is 1 2 3 (three fixed points). For n=6, output has two fixed points. So minimum fixed points can be 1? Let's check n=5. Can we have 1 fixed point? Let's try: p[1]=1. Need gcd(p[i],i)>1 for i=2..5. i=2: p[2] must be even. i=3: p[3] multiple of 3. i=4: p[4] even. i=5: p[5] multiple of 5. So p[2],p[4] are even numbers from {2,4}. p[3] is 3. p[5] is 5. Then p[1]=1. That uses all numbers. So p = [1,2,3,4,5] or [1,4,3,2,5]. Both have fixed points at 1,3,5? Actually p[3]=3, p[5]=5. So 3 fixed points. Can we avoid p[3]=3? If we put 6? But n=5, max is 5. So p[3] must be 3. Similarly p[5] must be 5. So for n=5, minimum fixed points is 3? But sample for n=13 has 4 fixed points. Let's think: For odd n, the odd numbers that are prime? Actually, for any odd i, if i is prime > n/2? Not necessarily. The problem is about constructing a permutation with minimum fixed points. The hints suggest graph components. Let's follow hints.

                // Hint 10: How many connected components will the graph have?
                // Vertices 2..n. Edges if gcd>1. Components: primes > n/2 are isolated (only connected to themselves? Actually gcd(p, p)=p>1, so self-loop? But we need permutation, so we can map a vertex to itself. But we want to minimize fixed points. If a vertex is isolated (no other vertex shares a common divisor >1), then in any good permutation, that vertex must map to itself, because it can only map to a vertex with gcd>1, and the only such vertex is itself (since gcd(x,x)=x>1). So isolated vertices are forced fixed points.
                // Which vertices are isolated? A vertex v is isolated if for all u != v, gcd(u,v)=1. That means v is a prime > n/2? Actually, if v is prime and v > n/2, then any multiple of v would be at least 2v > n, so no other vertex shares a factor >1 with v. Also v=1 is not in 2..n. So primes > n/2 are isolated. Also 1 is special? But 1 is not in 2..n. For i=1, condition is only for i>=2. So p[1] can be anything? But we want to minimize fixed points, so we can set p[1]=1 (fixed) or not? If we set p[1]!=1, then 1 is not a fixed point, but we might increase fixed points elsewhere? Actually, 1 is not constrained by gcd condition. So we can set p[1] to any number, and that number will not be a fixed point at index 1 unless p[1]=1. But if we set p[1]=x (x!=1), then x is used, and some other index must take 1. But 1 at index i>1 would violate gcd(1,i)=1, not >1. So 1 cannot be placed at any index i>=2. Therefore, 1 must be placed at index 1. So p[1]=1 is forced. So 1 is always a fixed point.
                // So fixed points include 1 and all primes > n/2. Can we avoid fixed points for other numbers? For even numbers, we can pair them up. For odd composite numbers, they share factors with other numbers.
                // So minimum fixed points = 1 + (number of primes in (n/2, n]).
                // Let's verify: n=2: primes >1 and <=2: 2. So 1+1=2. Output: 1 2. Correct.
                // n=3: primes >1.5 and <=3: 2,3. So 1+2=3. Output: 1 2 3. Correct.
                // n=6: primes >3 and <=6: 5. So 1+1=2. Output has fixed points 1 and 5. Correct.
                // n=13: primes >6.5 and <=13: 7,11,13. So 1+3=4. Output has fixed points 1,7,11,13. That's 4. So minimum is 4. Sample output has exactly those fixed points. So the construction should achieve exactly that many fixed points.
                // So the problem reduces to constructing a permutation where the only fixed points are 1 and primes > n/2, and all other indices i (2<=i<=n) are not fixed and satisfy gcd(p[i],i)>1.
                // How to construct? For even n, we can simply swap adjacent pairs: (2,3), (4,5), ..., (n-2,n-1)? But wait, gcd(2,3)=1, not >1. So that fails. The sample for n=6: 1 4 6 2 5 3. Pairs: (2,4), (3,6), (4,2), (5,5) - 5 is prime >3, fixed. (6,3). So they paired 2 with 4, 3 with 6. For even n, we can pair each even number with the next odd number? But gcd(even, odd) might be 1. We need gcd>1. So we need to pair numbers that share a common factor.
                // A known construction: For even n, we can do: p[1]=1. For i from 2 to n step 2: p[i] = i+1, p[i+1] = i. Check gcd(i, i+1)=1, so that fails. So we need a different pairing.
                // Observe that for any even number 2k, we can pair it with 2k+2? But then we need a permutation.
                // Actually, we can use the following: For i from 2 to n, if i is even, p[i] = i+2 (if i+2 <= n) else ...? Not a permutation.
                // Let's think about the graph. We want a derangement on the non-isolated vertices (except 1). The non-isolated vertices are those that are not primes > n/2. They form a graph where each vertex has at least one neighbor. We need a perfect matching or a set of cycles covering all these vertices, such that each edge (u,v) has gcd(u,v)>1. Then we set p[u]=v, p[v]=u for each edge in the matching, or follow cycles. Since we want to minimize fixed points, we want to avoid fixed points on these vertices. So we need a derangement on this set.
                // For even n, all numbers 2..n are non-isolated? Primes > n/2: if n is even, n/2 is integer. Primes > n/2 and <= n. For n=6, primes >3: 5. So 5 is isolated. Others: 2,3,4,6. We can pair 2 with 4 (gcd=2), and 3 with 6 (gcd=3). That works.
                // For n=8: primes >4: 5,7. Isolated: 5,7. Others: 2,3,4,6,8. Pair 2-4, 3-6, 8? 8 is even, can pair with? 8 and 4? But 4 already paired. 8 and 6? gcd(8,6)=2. So we can pair 8 with 6, but 6 is paired with 3. So we need a cycle: 3->6->8->? 8 needs a neighbor. 8 and 4? 4 is paired with 2. So we can do a cycle: 2->4->8->6->3->? 3 needs a neighbor. 3 and 9? 9>8. So 3 can only pair with 6. So cycle: 3->6, 6->8, 8->4, 4->2, 2->? 2 can pair with 4 or 6 or 8. So cycle: 2->4->8->6->3->? back to 2? gcd(3,2)=1. So not a cycle. We need a matching that covers all non-isolated vertices. Is it always possible? Yes, we can pair each even number with another even number, and odd composite numbers with their multiples. But we need a systematic construction.
                // Let's look at sample for n=13: 1 12 9 6 10 8 7 4 3 5 11 2 13
                // Indices: 2:12, 3:9, 4:6, 5:10, 6:8, 7:7, 8:4, 9:3, 10:5, 11:11, 12:2, 13:13.
                // Non-isolated: 2,3,4,5,6,8,9,10,12. (7,11,13 isolated). 5 is not isolated? 5 is prime, but 5 <= 13/2=6.5, so 5 has multiple 10 <=13. So 5 is not isolated. Indeed, 5 is paired with 10. So 5 is not fixed. Good.
                // How are they paired? 2<->12 (gcd 2), 3<->9 (gcd 3), 4<->6 (gcd 2), 5<->10 (gcd 5), 8<->? 8 is at index 6, p[6]=8, and p[8]=4? Wait p[8]=4, but 4 is already paired with 6. So it's not a simple matching. Let's trace:
                // p[2]=12, p[12]=2 -> 2-cycle.
                // p[3]=9, p[9]=3 -> 2-cycle.
                // p[4]=6, p[6]=8, p[8]=4 -> 3-cycle: 4->6->8->4. Check gcd: gcd(4,6)=2, gcd(6,8)=2, gcd(8,4)=4. All >1.
                // p[5]=10, p[10]=5 -> 2-cycle.
                // So the non-isolated vertices are partitioned into cycles of length >=2. This avoids fixed points on them.
                // So the construction for odd n: we need to handle the odd composite numbers and even numbers. The isolated primes > n/2 are left as fixed points.
                // How to construct systematically for any n?
                // We can do the following:
                // Place 1 at index 1.
                // For each prime p > n/2, set p[p] = p.
                // For the remaining numbers, we can form cycles. One common construction:
                // For even n: we can pair i with i+2? But need permutation.
                // Actually, there is a known solution: 
                // For n even: output 1, then for i=2 to n step 2: output i+1, i? But that fails gcd. So we need to swap differently.
                // Let's think of a simple construction that works for all n.
                // We can use the following idea: 
                // For i from 2 to n:
                // if i is even: p[i] = i+2 (if i+2 <= n and i+2 is not isolated? But isolated are primes > n/2, which are odd. So i+2 is even, not isolated). But we need a permutation.
                // Alternatively, we can shift even numbers by 2 positions, and odd numbers by something.
                // Let's look at the cycle for even numbers: we can do a cycle of all even numbers: 2->4->6->...->n->2. Check gcd: gcd(2k, 2k+2)=2, so all edges have gcd>=2. That works! But what about odd composite numbers? They need to be in cycles with other numbers. Odd composite numbers have odd factors. They can be paired with their multiples. For example, 9 can pair with 3, 15 with 5 or 3, etc.
                // But if we put all even numbers in one cycle, we use all even numbers. Then the odd numbers (except 1 and isolated primes) must be paired among themselves. But odd numbers that are composite might not share factors with each other? For example, 9 and 15 share factor 3. 9 and 21 share 3. 15 and 25 share 5. But what about 9 and 25? gcd=1. So we need to carefully pair odd composites.
                // Alternatively, we can pair each odd composite with an even multiple. For example, 3 with 6, 9 with 18? But 18 may be >n. So we need to use available even numbers.
                // The sample for n=13 uses a 3-cycle involving even numbers and one odd? Actually 4,6,8 are all even. 3 and 9 are odd. 5 and 10 (even). So odd composites are paired with even numbers or with other odd numbers that share a factor.
                // Let's try to find a general construction that is easy to code.
                // Observe that we can always do the following:
                // For all even numbers, we can form a cycle: 2 -> 4 -> 6 -> ... -> n (if n even) or n-1 (if n odd) -> 2. But if n is odd, the largest even is n-1. So cycle of evens: 2,4,6,...,n-1. That covers all even numbers. Then we need to place the odd numbers (except 1 and isolated primes). The odd numbers that are not isolated are those that have a multiple <= n. For each such odd number x, we can pair it with 2x (which is even and already in the even cycle). But if we do that, we break the even cycle. We can instead form cycles that include both even and odd numbers.
                // Another idea: For each odd number x (not isolated), we can pair it with 2x. But 2x might be > n. If 2x > n, then x > n/2, so x is an isolated prime (since if x is composite, it has a proper factor, but if x > n/2 and composite, its smallest factor is at least 2, so 2x > n, but it could have factor f < x, and f might be <= n/2. For example, n=15, x=9 > 7.5, 9 is composite, factor 3 <= 7.5. So 9 is not isolated because gcd(9,3)>1. So isolated are exactly primes > n/2. So for odd composite x > n/2, they are not isolated. But 2x > n, so we cannot pair with 2x. We need to pair them with a smaller multiple, like 3x? That's even larger. So we need to pair them with a factor. For example, 9 paired with 3. So odd composites can be paired with their factors.
                // So a systematic construction:
                // Let S be the set of numbers from 2 to n excluding the isolated primes (primes > n/2).
                // We want to partition S into cycles of length >=2 such that each edge has gcd>1.
                // We can do this by considering the prime factors. For each prime p <= n/2, we can form a cycle involving all multiples of p that are not yet assigned? But careful with overlaps.
                // A known construction from editorial: 
                // For even n: 
                //   p[1] = 1
                //   for i = 2 to n step 2:
                //       p[i] = i+1
                //       p[i+1] = i
                // But this fails gcd for (2,3) etc. So we need to swap differently.
                // Wait, the sample for n=6 is not that. It's 1 4 6 2 5 3. So they did something else.
                // Let's search for a pattern that works for all n.
                // Consider the following construction:
                // We can set p[1] = 1.
                // For i from 2 to n:
                //   if i is even: p[i] = i+2 (if i+2 <= n) else ... 
                // But we need a permutation.
                // Let's try to build a cycle of all even numbers and then insert odd numbers by breaking the cycle.
                // For example, for n=6: evens: 2,4,6. Cycle: 2->4->6->2. Then we have odd 3. We can insert 3 between 6 and 2? 6->3->2. Check gcd: gcd(6,3)=3>1, gcd(3,2)=1. Fails. So we need to insert odd numbers such that both edges have gcd>1.
                // For 3, we can insert between 6 and 9? 9 not present. So we pair 3 with 6 and 9? Not possible.
                // In sample: 3 is paired with 9, and 9 is paired with 3. So they form a 2-cycle. 5 is paired with 10. So odd composites are paired with their even multiples (if 2x <= n) or with other odd multiples? For 9, 2*9=18>13, so 9 paired with 3. So 9 is paired with its factor 3. So odd composites > n/2 are paired with their factors.
                // So we can do:
                // For each odd number x from 3 to n:
                //   if x is prime and x > n/2: p[x] = x (fixed)
                //   else if x * 2 <= n: pair x with 2x. But then 2x is even, and we need to pair 2x with x. So we can set p[x] = 2x, p[2x] = x. This uses both.
                //   else (x > n/2 and x is composite): x has a proper factor d (2 <= d <= sqrt(x)). Since x <= n, d <= n/2. We can pair x with d. But d might already be paired with 2d? We need to resolve conflicts.
                // This suggests a greedy assignment.
                // Let's look at the sample n=13:
                // x=3: 2*3=6 <=13 -> pair 3 with 6.
                // x=5: 2*5=10 <=13 -> pair 5 with 10.
                // x=7: prime >6.5 -> fixed.
                // x=9: 2*9=18>13, composite, factor 3. 3 is already paired with 6. So we need to form a cycle: 3->6->? and 9->? We can do 3->9, 9->6, 6->3? Check gcd: 3-9 (3), 9-6 (3), 6-3 (3). That's a 3-cycle. In sample, they did 3->9, 9->3 (2-cycle) and 6 is in a different cycle with 4 and 8. So they didn't pair 3 with 6. Instead, they paired 3 with 9, and 6 with 4 and 8.
                // So another approach: pair each odd number with the next odd number that shares a factor? 
                // Let's think about the graph components. The graph is connected for numbers that share prime factors. Actually, all even numbers are connected (via 2). All multiples of 3 are connected, etc. The whole set of numbers 2..n excluding isolated primes might be connected? Not necessarily. For n=13, evens: 2,4,6,8,10,12 are connected. Multiples of 3: 3,6,9,12. So 3 and 9 are connected to evens via 6 and 12. Multiples of 5: 5,10. So 5 connected via 10. So the whole non-isolated set is connected! Is it always connected? For any n, the non-isolated vertices (all numbers except 1 and primes > n/2) form a connected graph? Let's check: Any even number is connected to 2. Any odd composite number x has a prime factor p <= sqrt(x) <= n/2 (since x <= n, if x > n/2, its smallest factor is at most sqrt(x) <= sqrt(n) <= n/2 for n>=4). So p <= n/2. Then p is either even (p=2) or odd prime <= n/2. If p=2, x is even, contradiction. So p is odd prime <= n/2. Then p has multiple 2p <= n (since p <= n/2). So p is connected to 2p (even). Thus x is connected to p, which is connected to 2p, which is connected to 2. So all non-isolated vertices are in one connected component! Therefore, we can find a single cycle covering all non-isolated vertices? Not necessarily a single cycle, but we can find a derangement on this connected component. Since it's connected, we can find a Hamiltonian cycle? Not necessarily, but we can find a collection of cycles covering all vertices. The problem is to output any such permutation with minimum fixed points. So we just need to construct one.
                // A simple construction: 
                // 1. Place 1 at index 1.
                // 2. Place all primes > n/2 at their own indices.
                // 3. For the remaining numbers, we can do the following:
                //    - List all even numbers in increasing order: 2, 4, 6, ..., E (where E is the largest even <= n).
                //    - List all odd numbers that are not isolated primes: 3, 5, 7, 9, 11, ... up to n, but excluding primes > n/2.
                //    - We can interleave them in a cycle. For example, we can do a cycle: 2 -> 3 -> 4 -> 5 -> 6 -> ...? Check gcd: gcd(2,3)=1, fails.
                //    - We need each consecutive pair to have gcd>1.
                //    - Since all these numbers are in one component, we can build a cycle by connecting each number to a multiple or factor.
                // A known construction from Codeforces: 
                // For n even: 
                //   p[1] = 1
                //   for i = 2; i <= n; i += 2:
                //       p[i] = i + 1
                //       p[i+1] = i
                //   Then swap p[2] and p[n]? Let's test n=6: 2->3, 3->2, 4->5, 5->4, 6->? Wait, loop goes to n, so i=2: p[2]=3, p[3]=2; i=4: p[4]=5, p[5]=4; i=6: p[6]=7 (out of bounds). So that's for even n, but fails gcd on (2,3). So we need to modify.
                // Another idea: For even n, we can do:
                //   p[1] = 1
                //   for i = 2; i <= n; i += 2:
                //       p[i] = i + 2 (if i+2 <= n) else p[i] = 2
                //   But then odd numbers are not placed.
                // Let's look at the sample for n=6: 1 4 6 2 5 3
                // Indices: 2->4, 3->6, 4->2, 5->5, 6->3.
                // So they paired 2 with 4, 3 with 6, and 5 is fixed.
                // For n=8, we can try: 1, then pair 2-4, 3-6, 5-10? 10>8. So 5 is prime >4? 5 > 4, so 5 is isolated. So fixed points: 1,5,7. Remaining: 2,3,4,6,8. Pair 2-4, 3-6, 8? 8 can pair with 4? 4 already paired. 8 with 6? 6 paired. So we need a cycle: 2-4-8-6-3-? 3 needs a partner. 3 can pair with 6 or 9. So cycle: 3->6->8->4->2->? 2 needs partner. 2 can pair with 4,6,8. So we can close the cycle: 2->4->8->6->3->? back to 2? gcd(3,2)=1. So we need to insert 3 between two evens that are multiples of 3? 6 is multiple of 3. So we can do: 2->4->8->6->3->6? No, permutation must be bijection. So we need a cycle that includes 3 and all evens. For example: 2->4->8->6->3->? 3 needs to go to an even multiple of 3. The only even multiple of 3 <=8 is 6. So 3 must go to 6. But 6 is already pointed to by 8. So we can do: 8->3, 3->6, 6->? 6 can go to 4? gcd(6,4)=2. Then 4->2, 2->8? gcd(2,8)=2. So cycle: 2->8->3->6->4->2. Check: p[2]=8, p[8]=3, p[3]=6, p[6]=4, p[4]=2. All gcd>1. And 5,7 fixed. That works! So for n=8, permutation: 1,8,6,2,5,4,7,3? Let's list indices: 1:1, 2:8, 3:6, 4:2, 5:5, 6:4, 7:7, 8:3. Check gcd: i=2:8 gcd=2; i=3:6 gcd=3; i=4:2 gcd=2; i=5:5 gcd=5; i=6:4 gcd=2; i=7:7 gcd=7; i=8:3 gcd=1? Wait gcd(8,3)=1. p[8]=3, i=8, gcd(3,8)=1. That fails! So my cycle 2->8->3->6->4->2 means p[2]=8, p[8]=3, p[3]=6, p[6]=4, p[4]=2. Then p[8]=3, but i=8, gcd(3,8)=1. So that edge is invalid. I need gcd(p[i],i)>1. So for i=8, p[8] must share a factor with 8. So p[8] must be even. So 8 can only map to an even number. So in the cycle, the edge from 8 must go to an even number. So 8->3 is invalid. So 8 must map to an even number. Similarly, any even index must map to an even number? Not necessarily: if i is even, p[i] could be odd if gcd(even, odd)>1, e.g., i=6, p[6]=3, gcd(6,3)=3>1. So even index can map to odd multiple of 3. But for i=8, odd multiples of 2? No, odd numbers are not multiples of 2. So for i=8, any odd number is coprime to 8 unless it shares another factor. 8's prime factors: only 2. So any odd number is coprime to 8. Therefore, p[8] must be even. So 8 can only map to an even number. In general, if i is a power of 2 (like 2,4,8,16,...), then any odd number is coprime to i. So p[i] must be even. For i=2, p[2] must be even. For i=4, p[4] must be even. For i=8, p[8] must be even. So powers of 2 must map to even numbers. 
                // This suggests a construction that respects parity constraints.
                // Let's look at the sample n=13 again: p[2]=12 (even), p[4]=6 (even), p[6]=8 (even), p[8]=4 (even), p[10]=5 (odd, but 10 is even, 5 is odd, gcd(10,5)=5>1, so okay because 10 has factor 5). p[12]=2 (even). So even indices can map to odd numbers if the even index has an odd factor. 10 has odd factor 5, so it can map to 5. 6 has odd factor 3, so it can map to 3 (as in n=6, p[6]=3). 8 has no odd factor, so it must map to even.
                // So we need to assign numbers such that for each i, p[i] shares a common factor >1 with i.
                // A known simple construction from the editorial:
                // We can do the following:
                // Let the set of numbers be S = {2,3,...,n} \ {primes > n/2}.
                // We can sort S in increasing order, and then rotate by one position? But need gcd>1.
                // Actually, there is a very simple construction:
                // For i from 2 to n:
                //   if i is even: p[i] = i+2 (if i+2 <= n) else p[i] = 2
                //   if i is odd: p[i] = i-2 (if i-2 >= 3) else p[i] = something
                // But this doesn't handle isolated primes and 1.
                // Let's search for a pattern that works for all n.
                // Consider the following algorithm:
                // Initialize p as identity permutation.
                // For i = 2 to n:
                //   if i is not a prime > n/2:
                //      we want to swap it with something.
                // We can pair each even number with the next even number, and each odd composite with its double or half.
                // Let's try to construct by iterating i from 2 to n:
                // If i is a prime > n/2, skip (leave fixed).
                // Otherwise, if i is even and i+2 <= n and i+2 is not a prime > n/2, we can set p[i] = i+2 and p[i+2] = i. But then we need to handle overlaps.
                // Alternatively, we can do a cycle of all even numbers: 2 -> 4 -> 6 -> ... -> largest even -> 2. Then for odd numbers that are not isolated, we can insert them into the cycle by breaking an edge between two evens that are both multiples of the odd number's factor. For example, for odd 3, we can break the edge between 6 and 8? But 6 and 8 are both even, we can insert 3: 6 -> 3 -> 8? Check gcd(6,3)=3, gcd(3,8)=1. So we need both evens to be multiples of 3. So we can break the edge between two multiples of 3, like 6 and 12. Insert 3: 6 -> 3 -> 12. gcd(6,3)=3, gcd(3,12)=3. Works. Then we need to pair the odd number with something else if it's not 3? For 9, we can insert between 9 and 18? 18 may not exist. So we can pair 9 with 3? But 3 is already used. So we can form a separate cycle for multiples of 3: 3 -> 9 -> 6 -> 12 -> ... -> 3. 
                // This suggests grouping by smallest prime factor.
                // A known construction from Codeforces round 1787C? Actually, this is a known problem: "Good Permutation" or similar. I recall a solution:
                // For n even:
                //   p[1] = 1
