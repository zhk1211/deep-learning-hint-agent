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

        // If n == 1, any element works, lexicographically largest is the max in S
        if (n == 1) {
            cout << S.back() << '\n';
            continue;
        }

        // For n >= 2, condition must hold for all pairs (i, j) with gcd(i, j) = 1
        // This forces a_1 != gcd(a_i, a_j) for all coprime i, j.
        // In particular, for any i > 1, gcd(1, i) = 1, so a_1 != gcd(a_1, a_i) = gcd(a_1, a_i).
        // Since gcd(a_1, a_i) divides a_1, the only way a_1 != gcd(a_1, a_i) is if gcd(a_1, a_i) < a_1,
        // i.e., a_i is not a multiple of a_1. So a_1 cannot divide any other a_i.
        // Also, for any i, j > 1 with gcd(i, j) = 1, we need a_1 != gcd(a_i, a_j).
        // To maximize lexicographically, we want a_1 as large as possible from S.
        // Then we need to assign a_i for i >= 2 such that no a_i is a multiple of a_1,
        // and for any coprime i, j >= 2, gcd(a_i, a_j) != a_1.
        // The simplest construction: pick a_1 = max(S). Then for all i >= 2, pick the largest element
        // in S that is NOT a multiple of a_1. If no such element exists, impossible.
        // But we also need to ensure that for coprime i, j >= 2, gcd(a_i, a_j) != a_1.
        // If we set all a_i (i>=2) to the same value x (not multiple of a_1), then gcd(x, x) = x.
        // We need x != a_1, which is satisfied. Also, for any i, j with gcd(i, j) = 1, a_{gcd(i,j)} = a_1,
        // and gcd(a_i, a_j) = x, so condition holds as long as x != a_1.
        // But wait: what about pairs where gcd(i, j) > 1? The condition only requires a_{gcd(i,j)} != gcd(a_i, a_j).
        // If we set all a_i (i>=2) to x, then for any i, j with gcd(i, j) = g > 1, we have a_g != gcd(x, x) = x.
        // Since a_g is either a_1 (if g=1) or x (if g>1). For g>1, a_g = x, so we need x != x, which is false!
        // So we cannot set all a_i (i>=2) to the same value if there exists any pair with gcd > 1.
        // Indeed, for any i, j with gcd(i, j) = g > 1, we need a_g != gcd(a_i, a_j).
        // If a_i = a_j = x, then gcd(a_i, a_j) = x, and a_g = x (since g>1), so condition fails.
        // Therefore, we need a more careful assignment.

        // Let's analyze the condition more deeply.
        // For any i, j, let g = gcd(i, j). We need a_g != gcd(a_i, a_j).
        // Consider the case where g = 1: a_1 != gcd(a_i, a_j) for all coprime i, j.
        // This implies that a_1 cannot be a common divisor of any a_i, a_j with coprime indices.
        // In particular, for any i > 1, taking j = 1 (gcd(1, i) = 1), we get a_1 != gcd(a_1, a_i).
        // Since gcd(a_1, a_i) divides a_1, the only way it's not equal to a_1 is if a_i is not a multiple of a_1.
        // So no a_i (i>=2) can be a multiple of a_1.
        // Also, for any i, j > 1 with gcd(i, j) = 1, we need a_1 != gcd(a_i, a_j).
        // This means that for any two coprime indices > 1, their gcd cannot be a_1.
        // If we set all a_i (i>=2) to the same value x, then gcd(x, x) = x, so we need x != a_1, which is fine.
        // But as we saw, the problem is with g > 1.

        // Let's think about indices > 1. For any i, j with gcd(i, j) = g > 1, we need a_g != gcd(a_i, a_j).
        // If we set a_i = x for all i >= 2, then a_g = x, and gcd(a_i, a_j) = x, so condition fails.
        // So we cannot have all a_i equal for i >= 2.
        // What if we set a_i based on something like the smallest prime factor? 
        // Actually, we can use the fact that the condition only restricts pairs with gcd(i, j) = g.
        // If we assign a_i such that for any g > 1, a_g is some value, and for indices i that are multiples of g,
        // we ensure that gcd(a_i, a_j) != a_g when gcd(i, j) = g.
        // This seems complicated.

        // Let's reconsider the problem from a different angle.
        // The condition must hold for all pairs (i, j). This is a strong condition.
        // Notice that for any i, taking j = 2i (if 2i <= n), gcd(i, 2i) = i, so we need a_i != gcd(a_i, a_{2i}) = gcd(a_i, a_{2i}).
        // Since gcd(a_i, a_{2i}) divides a_i, the only way it's not equal to a_i is if a_{2i} is not a multiple of a_i.
        // So for any i such that 2i <= n, a_{2i} cannot be a multiple of a_i.
        // Similarly, for any i and any multiple k*i <= n, gcd(i, k*i) = i, so a_{k*i} cannot be a multiple of a_i.
        // This means that for any i, all multiples of i (greater than i) must have values that are not multiples of a_i.
        // In particular, for i=1, all a_j (j>=2) cannot be multiples of a_1.
        // For i=2, all a_{2k} (k>=2) cannot be multiples of a_2.
        // This suggests a kind of divisibility constraint.

        // Let's try to construct the lexicographically largest array.
        // We want a_1 as large as possible. Let a_1 = max(S).
        // Then for i=2, we want a_2 as large as possible from S, but not a multiple of a_1.
        // Also, for any j that is a multiple of 2 (like 4,6,8...), a_j cannot be a multiple of a_2.
        // This seems like we can assign values greedily from left to right, ensuring that for each i,
        // a_i is the largest available element in S that is not a multiple of any a_g where g divides i and g < i.
        // Wait, is that sufficient? Let's check the condition for any pair (i, j) with gcd(i, j) = g.
        // We need a_g != gcd(a_i, a_j). If we ensure that for every i, a_i is not a multiple of any a_g for g|i, g<i,
        // then for any pair (i, j) with gcd(i, j) = g, we have that g divides both i and j.
        // So a_i is not a multiple of a_g, and a_j is not a multiple of a_g.
        // Then gcd(a_i, a_j) cannot be a multiple of a_g? Not necessarily. They could share a factor that is a multiple of a_g?
        // Actually, if a_i and a_j are both not multiples of a_g, their gcd could still be a_g if a_g divides both.
        // But we only ensured they are not multiples of a_g, meaning a_g does not divide a_i and a_g does not divide a_j.
        // So a_g cannot divide gcd(a_i, a_j) either, because if a_g | gcd(a_i, a_j), then a_g | a_i and a_g | a_j.
        // Therefore, gcd(a_i, a_j) cannot be equal to a_g (since a_g does not divide it, unless a_g = 0 which is impossible).
        // So the condition a_g != gcd(a_i, a_j) is automatically satisfied if we ensure that for every i,
        // a_i is not divisible by any a_g for proper divisors g of i.
        // Is that true? Let's verify: Suppose for all i, for all g|i, g<i, we have a_g ∤ a_i (a_g does not divide a_i).
        // Take any pair (i, j) with gcd(i, j) = g. Then g|i and g|j. If g < i and g < j, then a_g ∤ a_i and a_g ∤ a_j,
        // so a_g ∤ gcd(a_i, a_j), hence gcd(a_i, a_j) != a_g. If g = i (so i divides j), then gcd(i, j) = i.
        // Then condition is a_i != gcd(a_i, a_j). Since a_i divides a_i, gcd(a_i, a_j) is a divisor of a_i.
        // For it to be unequal to a_i, we need that a_i does not divide a_j. But our condition only says
        // for g=i, we need a_i ∤ a_j when i|j and i<j. That's exactly the condition we have: for any proper multiple j of i,
        // a_i does not divide a_j. So it holds. Similarly if g=j.
        // Therefore, the necessary and sufficient condition is: for all 1 <= i < j <= n with i|j, a_i ∤ a_j.
        // In other words, no element should be a multiple of any element at an index that properly divides it.
        // This is a much simpler condition!

        // Let's double-check with the sample: n=6, S={3,4,6}. Lexicographically largest array satisfying i|j => a_i ∤ a_j.
        // We want a_1 max = 6. Then a_2 cannot be multiple of 6 => from S, 3 and 4 are not multiples of 6. Max is 4.
        // a_3 cannot be multiple of a_1=6 => 3 or 4. Max is 4.
        // a_4: divisors 1,2. Cannot be multiple of a_1=6 or a_2=4. S: 3,4,6. 4 is multiple of 4? 4 is multiple of 4, so not allowed. 6 multiple of 6? 6 is multiple of 6, not allowed. 3 is not multiple of 6 or 4. So a_4=3.
        // a_5: divisors 1. Cannot be multiple of 6. Max is 4.
        // a_6: divisors 1,2,3. Cannot be multiple of a_1=6, a_2=4, a_3=4. S: 3,4,6. 4 multiple of 4, 6 multiple of 6, 3 not multiple of 6,4,4. So a_6=3.
        // Result: 6,4,4,3,4,3. Matches sample!

        // So the problem reduces to: assign a_i from S such that for any i|j, a_i does not divide a_j.
        // And we want lexicographically largest.

        // This is a greedy problem on a poset (divisibility lattice).
        // We can process indices from 1 to n. For each i, we need to pick the largest element in S that is not divisible by any a_g for g|i, g<i.
        // Since n <= 1e5, we can do this efficiently.
        // We can maintain for each possible value x in S, whether it is "forbidden" due to being a multiple of some already placed a_g.
        // But we need to check for each i, the largest available element.
        // Since S is sorted, we can iterate S from largest to smallest and pick the first that is not forbidden.
        // However, forbidding multiples of a_g: when we place a_g, we must forbid all multiples of a_g that are in S.
        // But note: the condition is only for indices that are multiples of g. So if we place a_g at index g,
        // we only need to forbid multiples of a_g for indices that are multiples of g. But we are processing indices in order,
        // so when we are at index i, we need to know which values are forbidden due to divisors of i.
        // So we can precompute for each index i, the set of divisors g < i. But that's too slow if done naively.

        // Alternative: Since we only care about divisibility of values, we can maintain an array "bad" for each value in S,
        // indicating if it's forbidden. When we place a value v at index i, we must forbid v and all its multiples in S
        // for all future indices that are multiples of i. But we don't know which indices are multiples of i yet.
        // However, we can process indices in increasing order, and for each i, we consider all divisors g of i.
        // For each divisor g, we have placed a_g. We need to ensure a_i is not a multiple of a_g.
        // So at index i, we can iterate over divisors g of i (g < i), and for each a_g, we mark all multiples of a_g in S as forbidden temporarily? That would be too slow if we do it per index.

        // Better: Since n <= 1e5, the total number of divisor pairs (g, i) with g|i is O(n log n). We can precompute divisors for each i.
        // For each i, we need to find the largest element in S that is not a multiple of any a_g for g|i, g<i.
        // We can maintain for each value x in S, a count of "how many divisors of i have a_g that divide x".
        // But that's per index, still heavy.

        // Another perspective: The condition "a_i is not a multiple of a_g for any g|i, g<i" means that if we consider the set of placed values at divisors, a_i must not be in the union of their multiples.
        // Since S is a subset of [1, n], we can precompute for each value x in S, its multiples in S.
        // We can maintain a boolean array "available" for S elements. Initially all available.
        // When we place a_g, we cannot simply mark all its multiples as unavailable for all future indices, because they might be allowed for indices that are not multiples of g.
        // For example, if a_2 = 4, then for index 4 (multiple of 2), 4 is forbidden. But for index 3 (not multiple of 2), 4 is allowed.
        // So the forbiddenness depends on the index.

        // So we need a way to quickly check, for a given i, which values in S are multiples of any a_g for g|i.
        // Since the number of divisors of i is small on average (max ~128 for n=1e5), we can just iterate over divisors g of i,
        // and for each a_g, we need to check if a candidate value x is a multiple of a_g.
        // To find the largest x in S that is not a multiple of any a_g, we can iterate S from largest to smallest,
        // and for each x, check all divisors g of i: if a_g divides x, then x is invalid.
        // The number of divisors d(i) is at most 128 for n=1e5. m can be up to n. So worst-case per index: O(m * d(i)) which is too slow (1e5 * 1e5).

        // We need a faster way. Notice that we only need the largest valid element. We can precompute for each value x in S,
        // the set of its divisors that are also in S? Not exactly.

        // Let's think about the structure of S. S is sorted. We can maintain a segment tree or something?
        // Alternatively, we can use the fact that we want lexicographically largest, so we want to pick large values early.
        // Maybe we can determine the assignment by looking at the prime factorization?

        // Let's re-read the problem: n up to 1e5, sum n up to 3e5. m up to n.
        // We can afford O(n log n) or O(n * d(n)) where d(n) is small? Actually sum of d(i) over i=1..n is O(n log n).
        // If we can answer each index in O(d(i) * something small), it might pass.
        // But iterating over all elements of S for each index is too much.

        // Observation: The condition only cares about divisibility among values. Since values are <= n, we can use a sieve-like approach.
        // For each index i, we want the largest x in S such that for all g|i, g<i, a_g ∤ x.
        // This is equivalent to: x is not in the set of multiples of any a_g.
        // We can maintain an array "forbidden_count" for each value in S? But forbidden depends on i.

        // Wait, we process i from 1 to n. When we are at i, we only care about divisors g < i. We have already assigned a_g.
        // We can maintain for each value x in S, a counter of how many divisors of i have a_g that divide x.
        // But that counter would need to be updated for each i based on its divisors. That's O(d(i) * (number of multiples of a_g in S)).
        // Still too slow.

        // Another idea: Since we want lexicographically largest, we can try to place the largest possible value at each step.
        // We can maintain a set of available values from S. Initially all S.
        // When we place a value v at index i, we cannot simply remove v and its multiples from the set, because they might be allowed for indices not divisible by i.
        // However, we can delay the "forbidding" to when we actually process an index that is a multiple of i.
        // For each index j that is a multiple of i, v and its multiples are forbidden at j.
        // So we can precompute for each index j, a list of "forbidden values" coming from its divisors.
        // But that list could be large.

        // Let's consider the constraints: sum n <= 3e5. We can afford O(n sqrt(n)) maybe? sqrt(1e5)=316, n sqrt(n) ~ 3e7, might be okay in C++ with optimizations.
        // But we need to be careful.

        // Let's try to design an algorithm:
        // We have S sorted. We will assign a_i for i=1..n.
        // For each i, we need to find the largest x in S such that for all g|i, g<i, a_g does not divide x.
        // We can precompute divisors for all numbers up to n using sieve in O(n log n).
        // For each i, we iterate over its divisors g < i. For each such g, we have a_g.
        // We need to check if a candidate x is divisible by any a_g.
        // Instead of iterating over all x in S, we can iterate over divisors g, and for each a_g, we can mark its multiples in S as "bad" for this i.
        // But we need to do this efficiently. Since S is a subset of [1, n], we can have an array "val_to_idx" mapping value to its index in S (or -1 if not present).
        // For a given a_g, we can iterate over its multiples in S. The number of multiples of a value v in S is at most n/v.
        // Sum over all divisors g of i of (n / a_g) could be large.

        // But note that we only need the largest valid x. We can start from the largest element in S and go downwards, checking validity.
        // To check validity of x, we need to see if any a_g (g|i, g<i) divides x.
        // We can precompute for each x in S, the list of its divisors that are also in S? Not exactly, we need divisors that are a_g for some g|i.
        // Since we process i in order, we can maintain for each value v in S, a "last_forbidden_by" or something? Not sure.

        // Let's think differently: The condition a_i not divisible by any a_g for g|i is equivalent to: for every prime factor? Not exactly.

        // Maybe we can use the fact that we want lexicographically largest, so we want a_1 as large as possible.
        // Then for i>1, we want a_i as large as possible but not divisible by a_1 (if 1|i, which is always true for i>1).
        // So a_i cannot be a multiple of a_1 for all i>=2.
        // Additionally, for i with more divisors, more restrictions.
        // This suggests that the lexicographically largest array might be obtained by a greedy algorithm that always picks the largest available element that doesn't violate the condition with already placed divisors.
        // And we can check validity quickly by maintaining for each value in S a "blocked" count? No.

        // Let's consider an alternative representation: For each index i, the set of forbidden values is the union over g|i, g<i of multiples of a_g.
        // We can precompute for each value x in S, the set of indices i where x is forbidden? That's the opposite.

        // Since n is only up to 1e5, we can maybe do this: For each i, we want the largest x in S such that no a_g divides x.
        // We can maintain a boolean array "possible" of size m, initially all true.
        // For each i, we iterate over divisors g. For each a_g, we iterate over its multiples in S and mark them as false for this i.
        // But we need to reset for next i. Resetting would be O(m) per i, too slow.
        // However, we can use a versioned array or timestamp technique: have an array "bad_version" for each value in S, initially 0.
        // For each i, we have a current version number i. When we process divisor g, for each multiple of a_g in S, we set its bad_version to i.
        // Then to check if x is valid, we just check if bad_version[x] != i.
        // The total work over all i is sum over i of sum over g|i of (number of multiples of a_g in S).
        // This could be large if a_g is small. For example, if a_1 = 1, then multiples of 1 are all elements in S, and we do this for every i>1, leading to O(n * m) which is 1e10, too slow.
        // But a_1 cannot be 1 if we want lexicographically largest? Actually a_1 can be 1 if S contains 1. But if a_1=1, then for all i>=2, a_i cannot be a multiple of 1, which means no a_i can be any integer (since every integer is a multiple of 1). So impossible unless n=1. So if 1 is in S and n>1, it's impossible. So we can rule out a_1=1 for n>1.
        // In general, a_1 will be large, so its multiples in S are few. But what about other divisors? They could be small.
        // For example, a_2 could be 2, then multiples of 2 are many. But we only process multiples of a_g for indices i that are multiples of g. So for a_2=2, we only mark its multiples when i is a multiple of 2. That's n/2 indices. For each such i, we mark multiples of 2 in S. Number of multiples of 2 in S is at most n/2. So total work for a_2 could be (n/2) * (n/2) = n^2/4, still too large.

        // We need a better approach.

        // Let's re-express the condition: For all i|j, a_i ∤ a_j.
        // This is equivalent to saying that the sequence a_1, a_2, ..., a_n is such that the divisibility poset of indices is "anti-chain" in terms of divisibility of values? Not exactly.

        // Consider the prime factorization. Maybe we can assign values based on the exponent of a prime? 
        // Another idea: Since we want lexicographically largest, we can try to set a_i to the maximum possible value from S that is not a multiple of any a_g for g|i.
        // We can maintain for each value x in S, the set of indices i where x is allowed? That seems hard.

        // Let's look at the sample: S={3,4,6}. a_1=6. Then for i=2, allowed: 3,4. Pick 4. For i=3, allowed: 3,4. Pick 4. For i=4, divisors 1,2: a_1=6, a_2=4. Forbidden: multiples of 6 and 4 in S. Multiples of 6: 6. Multiples of 4: 4. So allowed: 3. Pick 3. etc.
        // Notice that the forbidden set for i=4 is just {4,6}. We can find the largest allowed by checking from largest to smallest in S, and for each, check if it's a multiple of any a_g.
        // Checking if x is a multiple of a_g is just x % a_g == 0.
        // So for each i, we can iterate over S in reverse, and for each x, iterate over divisors g of i, check if a_g divides x. If none do, pick x.
        // The complexity: For each i, we might scan many elements of S until we find a valid one. In worst case, we might scan O(m) elements per i, and for each, check O(d(i)) divisors. Total O(n * m * d(i)) worst-case.
        // But maybe in practice, the largest valid element is found quickly? Not guaranteed.

        // We can optimize by precomputing for each x in S, the list of divisors g (indices) that would forbid it? No, because a_g is not known in advance.

        // Wait, we process i in increasing order. When we are at i, we already know a_g for all g < i.
        // We can maintain a data structure that can answer: given a set of "forbidden divisors" (the a_g for g|i), what is the largest element in S not divisible by any of them?
        // Since the forbidden divisors are a set of numbers (values of a_g), we can think of it as: we have a set of numbers F (the a_g for g|i). We want max x in S such that for all f in F, f ∤ x.
        // This is equivalent to: x is not in the union of multiples of f.
        // We can precompute for each f, the set of its multiples in S. But F changes per i.

        // Since |F| = d(i) - 1 (excluding i itself), which is at most ~128, we can do the following:
        // For each i, we have a set of forbidden values F. We want the largest x in S not in the union of multiples of elements in F.
        // We can start from the largest element in S and go down. For each x, we check if it's a multiple of any f in F. If not, we pick it.
        // To make the check fast, we can precompute for each x in S, the list of its divisors? Actually we need to check if any f in F divides x. Since F is small, we can just iterate over F and check x % f == 0.
        // The number of steps we go down in S before finding a valid x might be small on average? In worst case, if many elements are forbidden, we might go through many. But the total number of times we reject an element across all i could be bounded?
        // Consider that each time we reject an element x for index i, it's because some f in F divides x. That f is a_g for some g|i. Note that x is a multiple of f. Since we process i in increasing order, could we bound the number of rejections?
        // Not obviously.

        // Let's think about the structure of the greedy assignment. We want lexicographically largest. This means we want a_1 as large as possible. Then a_2 as large as possible given a_1, etc.
        // This is exactly the greedy algorithm: for i=1..n, pick the largest available element from S that satisfies the condition with all previously picked a_g for g|i.
        // Is this greedy always optimal for lexicographically largest? Yes, because lexicographic order compares the first index where they differ. So we must maximize a_1 first, then a_2, etc. The choice of a_i does not restrict future choices in a way that would allow a larger a_i later? Actually, picking a larger a_i might restrict future indices more, but since we are maximizing the current index, and any valid completion will have the same prefix up to i-1, we want the largest possible a_i that can be extended to a full valid array. So we need to ensure that the choice of a_i leaves at least one valid assignment for the remaining indices. But if we just pick the largest a_i that doesn't violate the condition with previous, it might lead to a dead end later. However, the problem asks for the lexicographically largest array, not just a greedy choice that might fail. We need to be sure that the greedy choice is safe (i.e., there exists a completion). Is it always safe to pick the largest valid element at each step? Not necessarily, because picking a larger element might restrict future choices more, potentially making it impossible to fill later indices. But we want the lexicographically largest, so if there is any valid array with a larger a_i, we must take it. So we need to check if there exists any valid completion with this a_i. That sounds like backtracking, which is exponential.

        // However, the problem likely has a simpler characterization. Let's analyze the condition more deeply.

        // Condition: for all i|j, a_i ∤ a_j.
        // This is equivalent to: the function f(i) = a_i is such that the divisibility poset on indices is mapped to a divisibility poset on values with no divisibility relations along the poset edges? Actually it's an anti-chain condition: if i|j and i≠j, then a_i ∤ a_j.
        // This is reminiscent of assigning values such that the "value" at a node is not a multiple of the value at any ancestor in the divisibility tree.
        // The divisibility poset (1..n, |) is a DAG. We want to assign values from S to nodes such that no node has a value that is a multiple of an ancestor's value.
        // This is exactly a graph coloring-like problem where the "color" is a number, and we want no ancestor to divide the descendant's color.
        // We want the lexicographically largest assignment (by index order).

        // Notice that the condition only involves pairs where one divides the other. It does not restrict pairs that are incomparable (e.g., 2 and 3). So the only constraints are along the divisibility chains.
        // For a chain of indices 1, 2, 4, 8, ... we need a_1 ∤ a_2, a_1 ∤ a_4, a_2 ∤ a_4, etc.
        // This means that along any chain, the values must be strictly "non-dividing" in the sense that no earlier value divides a later value.
        // In particular, if we consider the prime factorization, we can think of assigning values such that for each prime, the exponent of that prime in a_i is not monotonically increasing along chains? Not exactly, because divisibility is about all primes simultaneously.

        // Let's consider the maximal elements in S. The largest element in S is a candidate for a_1. If we set a_1 = max(S), then for all i>1, a_i cannot be a multiple of max(S). Since max(S) is the largest, its multiples in S can only be itself (if it appears multiple times? But S has unique integers). So multiples of max(S) in S is just {max(S)}. So the restriction from a_1 is simply that no other a_i can be max(S). That's easy.
        // What about a_2? If we set a_2 to the second largest element in S (if not max), then its multiples in S might include some elements. But we only need to avoid those multiples for indices that are multiples of 2.
        // This suggests that we can assign values in decreasing order of S, placing them at indices that are "available" and satisfy the condition.

        // Another perspective: Since we want lexicographically largest, we want to put the largest possible values at small indices. So we should try to put the largest values at indices 1,2,3,... as much as possible.
        // The only restriction is that if we put a value v at index i, then for all multiples j of i, we cannot put any multiple of v.
        // So if we put a large value v at a small index, we "block" its multiples from being placed at multiples of that index.
        // To maximize lexicographically, we want to use the largest values early. So we should assign the largest value to index 1. Then the second largest value to index 2 (if not blocked), etc.
        // But we must also ensure that we don't run out of values for later indices. Since n can be up to 1e5 and m up to n, we might have enough values.

        // Let's try to formalize a greedy algorithm that always picks the largest available value that doesn't conflict with already placed values at divisor indices.
        // We need to check if this greedy can fail (i.e., lead to a dead end). If it can fail, we need a way to backtrack or we need to find a different construction.

        // Consider a small example: n=4, S={2,3,4,6}. 
        // Greedy: a_1 = max = 6.
        // a_2: cannot be multiple of 6 => allowed: 2,3,4. Max is 4.
        // a_3: cannot be multiple of 6 => allowed: 2,3,4. Max is 4.
        // a_4: divisors 1,2. a_1=6, a_2=4. Cannot be multiple of 6 or 4. Allowed: 2,3. Max is 3.
        // Result: 6,4,4,3. Check conditions: 1|2: 6∤4 ok; 1|3: 6∤4 ok; 1|4: 6∤3 ok; 2|4: 4∤3 ok. Valid.
        // What if S={2,4,6}? n=4.
        // a_1=6. a_2: allowed 2,4 -> 4. a_3: allowed 2,4 -> 4. a_4: divisors 1,2 -> a_1=6, a_2=4. Forbidden multiples of 6 and 4. Multiples of 6: 6. Multiples of 4: 4. Allowed: 2. So a_4=2. Valid.
        //
