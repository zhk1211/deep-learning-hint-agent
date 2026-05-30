// Hint1
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

        // If n == 1, any element works, but we need lexicographically largest
        if (n == 1) {
            cout << S.back() << "\n";
            continue;
        }

        // For n >= 2, we need to check if there is an element that can be placed at index 1
        // such that a[1] != gcd(a[1], a[1]) = a[1] -> impossible if a[1] is used at both places.
        // Actually condition: a[gcd(i,j)] != gcd(a[i], a[j])
        // For i=1, j=2: a[1] != gcd(a[1], a[2])
        // So we need a[1] != gcd(a[1], a[2]) => a[1] does not divide a[2] or a[2] does not divide a[1]?
        // Actually gcd(a[1], a[2]) = a[1] iff a[1] divides a[2]. So we need a[1] does NOT divide a[2].
        // Similarly for other pairs.

        // Observation: For any i, a[i] must be such that for all j > i, a[gcd(i,j)] != gcd(a[i], a[j]).
        // In particular, for i=1, j=2..n: a[1] != gcd(a[1], a[j]) => a[1] does not divide a[j] for any j>=2.
        // So a[1] cannot divide any other element in the array.
        // Also for i=2, j=4: a[2] != gcd(a[2], a[4]) => a[2] does not divide a[4].
        // More generally, for any i, a[i] cannot divide a[2i], a[3i], ... within bounds.

        // To maximize lexicographically, we want the largest possible a[1] from S that does not divide any other element we might place.
        // But we also need to fill the rest. The condition only restricts divisibility along multiples.

        // Let's think: If we set a[1] = x, then for all j>=2, x cannot divide a[j].
        // So all a[j] must not be multiples of x.
        // Also for i>1, a[i] cannot divide a[2i], a[3i], etc.

        // We can try to construct the array greedily from left to right.
        // For position i, we want the largest element in S that is valid given previous assignments.
        // Valid means: for all k < i such that k divides i (i.e., i is a multiple of k), we must have a[k] does not divide candidate.
        // Also for all k < i such that i divides k? No, condition is symmetric? Actually condition is for pairs (i,j) with i<j.
        // So when placing a[i], we only need to check constraints with smaller indices j where i is a multiple of j? Wait:
        // The condition is a[gcd(i,j)] != gcd(a[i], a[j]).
        // For a fixed i and j>i, if we are placing a[i], we don't know a[j] yet. But we can enforce that a[i] does not divide any future a[j] where gcd(i,j)=i (i.e., i divides j).
        // So when placing a[i], we must ensure that for all multiples j of i (j > i), a[i] does not divide a[j]. But we haven't placed a[j] yet.
        // So we need to choose a[i] such that there exists some assignment for its multiples that avoids being multiples of a[i].
        // This seems complicated.

        // Alternative approach: Notice that the condition a[gcd(i,j)] != gcd(a[i], a[j]) is equivalent to saying that for any i, a[i] does not divide a[j] whenever i divides j.
        // Because if i divides j, then gcd(i,j)=i, so condition becomes a[i] != gcd(a[i], a[j]). gcd(a[i], a[j]) = a[i] iff a[i] divides a[j]. So we need a[i] does NOT divide a[j].
        // If i does not divide j, then gcd(i,j) = g < i, and condition involves a[g] and gcd(a[i], a[j]). This is automatically satisfied if we ensure the divisibility condition for all pairs where one divides the other? Not necessarily, but maybe we can construct an array where all elements are coprime? But they are from S, not necessarily coprime.

        // Let's analyze small n:
        // n=2: condition for (1,2): a[1] != gcd(a[1], a[2]) => a[1] does not divide a[2].
        // So we need a[1] from S, a[2] from S, a[1] does not divide a[2]. Lexicographically largest: choose largest possible a[1] such that there exists a[2] in S not divisible by a[1]. Then choose largest possible a[2] (which is just the largest element in S not divisible by a[1]).
        // n=3: pairs (1,2): a[1] does not divide a[2]; (1,3): a[1] does not divide a[3]; (2,3): gcd(2,3)=1, so a[1] != gcd(a[2], a[3]). So we also need a[1] != gcd(a[2], a[3]).
        // This additional condition might force a[1] to be something specific.

        // Let's try to find a pattern. The hints say: "first see the nature of the sequences that satisfy the conditions."
        // Maybe the only way to satisfy all conditions is to have a[1] be the maximum element that does not divide any other element, and then for other positions, we can put the maximum possible element that is not a multiple of a[1]? But we also have conditions between other indices.

        // Consider the condition a[gcd(i,j)] != gcd(a[i], a[j]). If we set all a[i] to be the same value x, then for any i,j, gcd(a[i], a[j]) = x, and a[gcd(i,j)] = x, so condition fails. So we cannot have all equal.
        // If we set a[1] = x, and for all i>1, a[i] = y where y is not a multiple of x, then for any pair (i,j) with i,j>1, gcd(i,j) could be 1, then condition is a[1] != gcd(y,y)=y. So we need x != y, which is true if x != y. But if gcd(i,j) > 1, say g>1, then a[g] = y (since g>1), and gcd(a[i], a[j]) = y, so condition fails because a[g] = y equals gcd = y. So we cannot have all non-1 indices equal to the same y if there exists a pair with gcd > 1. For n>=3, there are indices >1 with gcd>1 (e.g., 2 and 4). So we need more variety.

        // Observation: The condition must hold for all pairs. In particular, for any i and j=2i, we have gcd(i,2i)=i, so a[i] != gcd(a[i], a[2i]) => a[i] does not divide a[2i]. So along the chain of multiples, no element can divide the next.
        // This suggests that if we consider the poset of indices ordered by divisibility, we need an assignment such that no element divides another along an edge. This is like an antichain condition on the divisibility poset of the multiset of values? Actually it's a condition on the values: if i divides j, then a[i] does not divide a[j].

        // To maximize lexicographically, we want a[1] as large as possible. a[1] cannot divide any a[j] for j>1. So a[1] must be an element that does not divide any other element we place. If we place the largest element in S as a[1], say M, then M cannot divide any other element. Since M is the maximum, it cannot divide any other element in S (unless there is a multiple of M in S, but M is max, so no multiple > M, and M divides itself but we don't place M elsewhere). So M is always a valid candidate for a[1] if we can fill the rest without using multiples of M. But we also need to satisfy other conditions.

        // Let's test with sample: n=6, m=3, S={3,4,6}. Lexicographically largest array: 6 4 4 3 4 3.
        // a[1]=6. 6 does not divide 4 or 3. Good.
        // a[2]=4. Check pairs: (2,4): gcd=2, a[2]=4, gcd(a[2],a[4])=gcd(4,3)=1, 4!=1 ok. (2,6): gcd=2, a[2]=4, gcd(4,3)=1 ok.
        // a[3]=4. (3,6): gcd=3, a[3]=4, gcd(4,3)=1 ok.
        // a[4]=3. (4,?) no multiples within n=6 except maybe? 4*2=8>6. So no multiples.
        // a[5]=4. (5,?) no multiples.
        // a[6]=3.
        // So it works.

        // Notice that a[1]=6, a[2]=4, a[3]=4, a[4]=3, a[5]=4, a[6]=3. The values at indices >1 are not all the same; they alternate? Actually indices 2,3,5 are 4; indices 4,6 are 3. This pattern might be related to the number of prime factors or something.

        // Another idea: The condition a[i] does not divide a[j] for i|j. If we assign values based on the exponent of 2 in the index? Not sure.

        // Let's think about a construction that always works if possible. We want lexicographically largest, so we want to put the largest possible element at each position from left to right, ensuring that there exists a completion for the rest.

        // Since n up to 1e5, we need an efficient check. Maybe we can determine the maximum possible a[1] by checking if there is an element in S that is not a divisor of any other element in S? But we also need to fill the rest.

        // Consider the necessary condition for a[1]: For all j>=2, a[1] does not divide a[j]. So all a[j] must be chosen from S \ {multiples of a[1]}. If S contains an element x and also a multiple of x, then x cannot be a[1] if we need to use that multiple somewhere? Actually we could still use x as a[1] if we don't use its multiple. But we want lexicographically largest, so we might want to use the multiple as a[1] instead.

        // Let's try to see if there is a simple characterization: The array is valid iff for every i, a[i] is the maximum element in S that is not a multiple of any a[j] for j|i and j<i? Not exactly.

        // Let's look at the condition a[gcd(i,j)] != gcd(a[i], a[j]). This is equivalent to: For any g, and any i,j with gcd(i,j)=g, we have a[g] != gcd(a[i], a[j]). In particular, if we set a[i] to be a function of i, maybe we can satisfy it by making a[i] "coprime" in some sense.

        // Another perspective: The condition is similar to saying that the sequence a is a "GCD-free" mapping. There is known result: If we define a[i] = f(i) where f is a completely multiplicative function? Not sure.

        // Let's try to find a pattern by brute force for small n to see what the lexicographically largest array looks like for various S.
        // Since we can't run code here, we can reason.

        // Suppose S contains all numbers from 1 to n. What is the lexicographically largest valid array?
        // For n=2: S={1,2}. a[1] cannot divide a[2]. Largest a[1]=2, then a[2] cannot be multiple of 2, so a[2]=1. Array: 2 1. Valid? (1,2): a[1]=2, gcd(2,1)=1, 2!=1 ok.
        // n=3: S={1,2,3}. Try a[1]=3. Then a[2], a[3] cannot be multiples of 3, so from {1,2}. We want lexicographically largest, so a[2]=2. Then a[3] cannot be multiple of 3, and also condition (2,3): gcd(2,3)=1, so a[1] != gcd(a[2],a[3]) => 3 != gcd(2, a[3]). If a[3]=2, gcd(2,2)=2, 3!=2 ok. If a[3]=1, gcd(2,1)=1, 3!=1 ok. Largest a[3]=2. So array: 3 2 2. Check (1,2): 3!=gcd(3,2)=1 ok. (1,3): 3!=gcd(3,2)=1 ok. (2,3): a[1]=3 != gcd(2,2)=2 ok. So 3 2 2 works.
        // n=4: S={1,2,3,4}. Try a[1]=4. Then others from {1,2,3} (no multiples of 4). a[2]=3. a[3]=3? Check (2,4): gcd=2, a[2]=3, gcd(a[2],a[4]) -> a[4] unknown. We need to assign a[4]. a[4] cannot be multiple of a[2]=3? Actually condition for (2,4): a[2] != gcd(a[2],a[4]) => 3 != gcd(3, a[4]). So a[4] cannot be a multiple of 3. So a[4] from {1,2}. Also (1,4): a[1]=4 != gcd(4, a[4]) => a[4] cannot be multiple of 4 (already). (3,4): gcd=1, a[1]=4 != gcd(a[3],a[4]). If a[3]=3, a[4]=2, gcd=1, 4!=1 ok. If a[3]=3, a[4]=1, gcd=1 ok. Largest a[4]=2. So array: 4 3 3 2. Check (2,3): gcd=1, a[1]=4 != gcd(3,3)=3 ok. (2,4): a[2]=3 != gcd(3,2)=1 ok. (3,4): a[1]=4 != gcd(3,2)=1 ok. So 4 3 3 2 works.
        // n=5: S={1,2,3,4,5}. Try a[1]=5. Others from {1,2,3,4}. a[2]=4. a[3]=4? Check (2,4): a[2]=4 != gcd(4, a[4]) => a[4] not multiple of 4. a[4] from {1,2,3}. Largest a[4]=3. a[3]=4, a[5]=? (1,5): a[1]=5 != gcd(5, a[5]) => a[5] not multiple of 5. (5 has no multiples in S anyway). (2,?): no multiples of 2 except 4 which is already assigned? Actually 2*2=4, 2*3=6>5. So only (2,4) already handled. (3,?): 3*2=6>5. So a[5] can be largest from {1,2,3,4} not multiple of 5? All are not multiples of 5. But also condition (5,?) with others: (1,5) done, (2,5): gcd=1, a[1]=5 != gcd(a[2],a[5]) = gcd(4, a[5]). If a[5]=4, gcd=4, 5!=4 ok. If a[5]=3, gcd=1 ok. So a[5]=4. Array: 5 4 4 3 4. Check (3,5): gcd=1, a[1]=5 != gcd(4,4)=4 ok. (4,5): gcd=1, a[1]=5 != gcd(3,4)=1 ok. So 5 4 4 3 4 works.
        // n=6: S={1,2,3,4,5,6}. Try a[1]=6. Others from {1,2,3,4,5}. a[2]=5. a[3]=5? Check (2,4): a[2]=5 != gcd(5, a[4]) => a[4] not multiple of 5. a[4] from {1,2,3,4}. Largest a[4]=4. a[3]=5, a[5]=? (1,5): a[1]=6 != gcd(6, a[5]) => a[5] not multiple of 6. (2,?): multiples of 2: 4,6. (2,6): a[2]=5 != gcd(5, a[6]) => a[6] not multiple of 5. (3,6): a[3]=5 != gcd(5, a[6]) => same. a[5] can be 5? Check (5,?) no multiples. But (2,5): gcd=1, a[1]=6 != gcd(5,5)=5 ok. (3,5): gcd=1, 6!=gcd(5,5)=5 ok. (4,5): gcd=1, 6!=gcd(4,5)=1 ok. So a[5]=5. a[6]: from {1,2,3,4} not multiple of 5 (all are not), not multiple of 6 (all are not), and check (2,6): a[2]=5 != gcd(5, a[6]) => a[6] not multiple of 5 (ok). (3,6): a[3]=5 != gcd(5, a[6]) ok. (4,6): gcd=2, a[2]=5 != gcd(a[4], a[6]) = gcd(4, a[6]). If a[6]=4, gcd=4, 5!=4 ok. If a[6]=2, gcd=2, 5!=2 ok. Largest a[6]=4. Array: 6 5 5 4 5 4. Check (2,3): gcd=1, a[1]=6 != gcd(5,5)=5 ok. (2,4): a[2]=5 != gcd(5,4)=1 ok. (3,4): gcd=1, 6!=gcd(5,4)=1 ok. (3,6): a[3]=5 != gcd(5,4)=1 ok. (4,6): a[2]=5 != gcd(4,4)=4 ok. So 6 5 5 4 5 4 works. Is it lexicographically largest? Let's see if we can do better at a[2]. Could a[2]=6? No, 6 is used at a[1] and we can reuse? S has unique elements but we can reuse elements in the array. The problem says "a_i in S", S is a set of unique integers, but the array can have duplicates. So we can reuse 6. But if a[2]=6, then condition (1,2): a[1]=6, gcd(6,6)=6, 6==6 violates. So a[2] cannot be 6. Could a[2]=5? We did that. Could a[2]=4? Then a[1]=6, a[2]=4. Check (1,2): 6!=gcd(6,4)=2 ok. Then we want largest a[3]. a[3] could be 6? Check (1,3): 6!=gcd(6,6)=6 violates. So a[3] cannot be 6. Could be 5? Then array: 6 4 5 ... Let's see if we can get larger lexicographically. Lexicographically, we compare first element: both 6. Second element: 5 vs 4, 5 is larger. So 6 5 ... is better than 6 4 ... So our greedy choice of a[2]=5 seems correct.

        // From these examples, a pattern emerges: a[1] is the maximum element in S. Then for i>1, a[i] is the maximum element in S that is not a multiple of a[1]? Not exactly: in n=6, a[2]=5, a[3]=5, a[4]=4, a[5]=5, a[6]=4. Notice that a[4]=4, which is a multiple of a[2]=5? No. But a[4]=4 is a multiple of a[1]=6? No. Actually, the values seem to be determined by the largest element that is "available" and doesn't violate the condition with its divisors.

        // Let's try to formalize: For each index i, the condition with its multiples j (j = k*i) is that a[i] does not divide a[j]. So if we process indices in increasing order, when we set a[i], we must ensure that for all future multiples j, we do not set a[j] to a multiple of a[i]. This means that a[i] should be chosen such that there exists at least one element in S that is not a multiple of a[i] for each multiple? Actually we just need to be able to fill the multiples with something not divisible by a[i]. Since we want lexicographically largest, we want to pick the largest possible a[i] such that the set of available elements for its multiples (which must not be multiples of a[i]) is non-empty? But we also have to satisfy conditions from other divisors.

        // Notice that in the examples, the values at indices > 1 are always from S \ {multiples of a[1]}. And they seem to be the maximum possible at each step given the constraints from smaller indices that divide them.

        // Let's think about the structure of the divisibility poset. The indices form a tree-like structure if we consider the "smallest prime factor" or something? Actually, the condition only restricts pairs where one divides the other. For a given i, the constraints on a[i] come from all divisors d of i (d < i). For each such d, we must have a[d] does not divide a[i]. So a[i] cannot be a multiple of any a[d] for d|i, d<i. Additionally, a[i] will impose constraints on its multiples: a[i] cannot divide a[j] for j multiple of i.

        // So if we process indices in increasing order, when we are at i, we know all a[d] for d|i, d<i. We must choose a[i] from S such that for all d|i, d<i, a[d] does not divide a[i]. That is, a[i] is not a multiple of any a[d]. To maximize lexicographically, we want the largest such element. But we also need to ensure that this choice leaves a feasible completion for the rest. Is it always possible to complete if we just greedily pick the largest valid element at each step? Let's test with a potential counterexample.

        // Suppose S = {2, 3, 6}. n=3. Greedy: a[1] = max S = 6. Then for i=2: divisors d=1. a[1]=6 does not divide a[2] => a[2] cannot be multiple of 6. Valid: 2,3. Largest is 3. So a[2]=3. For i=3: divisors d=1. a[1]=6 does not divide a[3] => a[3] cannot be multiple of 6. Valid: 2,3. Largest is 3. So a[3]=3. Array: 6 3 3. Check (2,3): gcd=1, a[1]=6 != gcd(3,3)=3 ok. So it works. What if S = {2, 4, 6}? n=3. a[1]=6. i=2: cannot be multiple of 6 => valid: 2,4. Largest 4. a[2]=4. i=3: valid: 2,4. Largest 4. a[3]=4. Array: 6 4 4. Check (2,3): gcd=1, a[1]=6 != gcd(4,4)=4 ok. Works.

        // What if S = {3, 4, 6} as in sample? n=6. Greedy: a[1]=6. i=2: valid not multiple of 6 => {3,4}. Largest 4. a[2]=4. i=3: divisors 1. valid not multiple of 6 => {3,4}. Largest 4. a[3]=4. i=4: divisors 1,2. a[1]=6 does not divide a[4] => not multiple of 6. a[2]=4 does not divide a[4] => not multiple of 4. Valid: {3} (since 4 is multiple of 4, 6 is multiple of 6). So a[4]=3. i=5: divisors 1. valid not multiple of 6 => {3,4}. Largest 4. a[5]=4. i=6: divisors 1,2,3. a[1]=6 => not multiple of 6. a[2]=4 => not multiple of 4. a[3]=4 => not multiple of 4. Valid: {3}. a[6]=3. Array: 6 4 4 3 4 3. Matches sample!

        // So the greedy algorithm: For i from 1 to n, choose the largest element in S that is not a multiple of any a[d] for d|i, d<i. If at any point no such element exists, then no solution exists (output -1). This seems to work for the examples and is intuitive.

        // But is it always correct? We need to ensure that this greedy choice doesn't lead to a dead end later. Since we are only forbidding multiples of chosen elements, and we always pick the largest possible, we are leaving the maximum flexibility for future indices because larger numbers have more multiples? Actually, larger numbers might have fewer multiples within S? But we are forbidding multiples of the chosen number. If we pick a larger number, it might be a divisor of fewer elements in S (since larger numbers have fewer multiples in a bounded set). So picking the largest valid element minimizes the set of forbidden multiples for future indices. This is good for feasibility. So greedy should be optimal.

        // Let's test a potential failure: S = {2, 3, 4, 6}, n=4. Greedy: a[1]=6. i=2: valid not multiple of 6 => {2,3,4}. Largest 4. a[2]=4. i=3: valid not multiple of 6 => {2,3,4}. Largest 4. a[3]=4. i=4: divisors 1,2. not multiple of 6, not multiple of 4. Valid: {2,3}. Largest 3. a[4]=3. Array: 6 4 4 3. Check (2,4): a[2]=4 != gcd(4,3)=1 ok. (3,4): gcd=1, a[1]=6 != gcd(4,3)=1 ok. Works.

        // What if S = {2, 4, 8}? n=3. a[1]=8. i=2: not multiple of 8 => {2,4}. Largest 4. a[2]=4. i=3: not multiple of 8 => {2,4}. Largest 4. a[3]=4. Array: 8 4 4. Check (2,3): gcd=1, a[1]=8 != gcd(4,4)=4 ok. Works.

        // What if S = {2, 4}? n=2. a[1]=4. i=2: not multiple of 4 => {2}. a[2]=2. Array: 4 2. Works.

        // What if S = {2}? n=2. a[1]=2. i=2: not multiple of 2 => {} empty. So no solution. Output -1. Sample 3: n=2, m=1, S={2} gives -1. Matches.

        // What if S = {1, 2}? n=2. a[1]=2. i=2: not multiple of 2 => {1}. a[2]=1. Array: 2 1. Works.

        // What if S = {1, 2, 3}? n=3. a[1]=3. i=2: not multiple of 3 => {1,2}. Largest 2. a[2]=2. i=3: not multiple of 3 => {1,2}. Largest 2. a[3]=2. Array: 3 2 2. Works.

        // What if S = {1, 2, 4}? n=3. a[1]=4. i=2: not multiple of 4 => {1,2}. Largest 2. a[2]=2. i=3: not multiple of 4 => {1,2}. Largest 2. a[3]=2. Array: 4 2 2. Check (2,3): gcd=1, a[1]=4 != gcd(2,2)=2 ok. Works.

        // What if S = {1, 3, 5}? n=3. a[1]=5. i=2: not multiple of 5 => {1,3}. Largest 3. a[2]=3. i=3: not multiple of 5 => {1,3}. Largest 3. a[3]=3. Array: 5 3 3. Check (2,3): gcd=1, a[1]=5 != gcd(3,3)=3 ok. Works.

        // It seems the greedy algorithm always works if a solution exists. But we need to prove that if greedy fails (i.e., at some step no valid element), then no solution exists. Suppose at step i, the set of allowed elements is empty. This means for every x in S, there is some divisor d of i (d < i) such that a[d] divides x. Since we picked a[d] greedily as large as possible, could a different choice of a[d] have made some x available? If we had picked a smaller a[d], it might divide fewer elements? Actually, if a number is smaller, it might divide more elements (e.g., 2 divides more numbers than 4). So picking a smaller a[d] would only increase the set of multiples, thus forbidding more elements. So our greedy choice of largest possible a[d] minimizes the set of forbidden multiples. Therefore, if even with this minimal restriction we have no valid element, then any other choice would also have no valid element. So the greedy algorithm is correct.

        // Thus the algorithm:
        // - Read n, m, S.
        // - Create a boolean array or set for quick check if an element is in S. Since elements are up to n, we can use a vector<bool> of size n+1.
        // - We need to efficiently find the largest element in S that is not a multiple of any a[d] for d|i, d<i.
        // - Since n up to 1e5, we can precompute divisors for each i? Or we can iterate over multiples? For each i, we need to check all divisors d < i. The number of divisors is small (max ~128 for n=1e5). So we can just find all divisors of i on the fly.
        // - To check if an element x is forbidden, we need to see if there exists a divisor d of i such that a[d] divides x. Since a[d] is already chosen, we can just check for each divisor d: if x % a[d] == 0, then x is forbidden.
        // - We want the largest x in S that is not forbidden. Since S is sorted, we can iterate from largest to smallest and check the condition. If we find one, assign a[i] = x and break. If none, output -1.
        // - Complexity: For each i, we iterate over divisors of i (O(sqrt(i)) or precomputed). For each candidate x from S (in worst case m candidates), we check all divisors. That could be O(n * m * d) which is too slow if m is large (up to n). We need a faster way.

        // Optimization: Instead of iterating over all x in S for each i, we can maintain the set of available elements. Initially all S are available. When we assign a[i] = x, we need to forbid all multiples of x for future indices? Actually, the condition is: for future j that are multiples of i, a[i] cannot divide a[j]. So when we set a[i], we should mark all multiples of a[i] as forbidden for all multiples of i. But that's complicated because the forbiddenness depends on the index.

        // Alternative: For each i, we need the largest element in S that is not a multiple of any a[d] for d|i. Since the number of divisors is small, we can just check each element in S? But m can be 1e5, and n up to 1e5, so n*m = 1e10 too large.

        // We need a more efficient way to find the largest valid element. Notice that the forbidden condition is: x is a multiple of some a[d]. So the set of forbidden elements is the union over d|i of multiples of a[d] that are in S. We want the largest element in S not in this union.

        // Since S is sorted, we can start from the largest element and go down until we find one that is not a multiple of any a[d]. But in the worst case, we might check many elements. However, the number of distinct a[d] is at most the number of divisors of i, which is small. The multiples of these a[d] in S might be many. But we can precompute for each possible value v in S, its divisors? Actually, we want to know if x is a multiple of any a[d]. We can precompute for each x in S, the set of its divisors that are in S? Not exactly.

        // Another idea: Since we only care about the largest valid element, we can maintain a data structure that supports "find largest element not divisible by any number in a given set". The set of divisors a[d] is small. We can just iterate x from largest to smallest in S, and for each x, check if it's divisible by any a[d]. The check is O(number of divisors). If we do this for each i, the total time could be O(n * d + number of checks * d). In the worst case, if we often go deep into S before finding a valid element, it could be O(n * m * d). But maybe in practice, the largest element is often valid? Not necessarily.

        // Let's think about the structure: a[d] are elements we've already placed. They are all from S. The largest element in S is often not a multiple of smaller elements unless S contains many multiples. But if S contains many multiples, the greedy might fail early.

        // We can optimize by precomputing for each element in S, the largest element in
