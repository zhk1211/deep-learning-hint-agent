// Hint2
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

        // If m == 1, only possible if n == 1, else impossible
        if (m == 1) {
            if (n == 1) {
                cout << S[0] << '\n';
            } else {
                cout << "-1\n";
            }
            continue;
        }

        // We need to assign a_i from S such that a_{gcd(i,j)} != gcd(a_i, a_j)
        // Key observation: if we set a_1 = max(S), and for i > 1 set a_i = min(S) 
        // (or some other element), we can satisfy conditions.
        // But we need lexicographically largest, so we want a_1 as large as possible,
        // and then a_2 as large as possible, etc.
        // However, condition for i=1, j>1: a_1 != gcd(a_1, a_j) => a_1 != gcd(a_1, a_j)
        // This holds if a_j does not divide a_1, or if a_j divides a_1 but a_1 != a_j.
        // Actually gcd(a_1, a_j) = a_j if a_j divides a_1, else < a_j.
        // So a_1 != a_j is sufficient? Not exactly: if a_j divides a_1 and a_j = a_1, then gcd = a_1, so a_1 = gcd, violation.
        // If a_j divides a_1 and a_j < a_1, then gcd = a_j, so a_1 != a_j holds.
        // If a_j does not divide a_1, gcd < a_j <= a_1, so a_1 != gcd holds.
        // So condition for i=1 reduces to: a_1 != a_j for all j>1? Wait, if a_j = a_1, then gcd = a_1, violation.
        // So we must have a_j != a_1 for all j>1.
        // Also for i>1, j>i: a_{gcd(i,j)} != gcd(a_i, a_j). If we set all a_i (i>1) to the same value x, then gcd(a_i, a_j) = x.
        // Then we need a_{gcd(i,j)} != x. Since gcd(i,j) can be 1, we need a_1 != x. Also for other gcd values, we need a_k != x.
        // If we set all a_i for i>1 to x, and a_1 = y (y != x), then for any pair i,j>1, gcd(i,j) could be 1, giving a_1 = y != x, ok.
        // If gcd(i,j) = d > 1, then a_d must be != x. But if d>1, a_d is also x (since all i>1 are x), then a_d = x, violation.
        // So we cannot set all i>1 to the same value if there exists any pair with gcd > 1.
        // Actually, for n>=2, there are pairs with gcd > 1 (e.g., (2,4) gcd=2). So we need a_2 != x if a_2 = x? Wait, a_2 is x, then a_{gcd(2,4)} = a_2 = x, and gcd(a_2,a_4)=x, violation.
        // So we need a more clever assignment.

        // Let's think: condition a_{gcd(i,j)} != gcd(a_i, a_j). 
        // If we set a_i = f(i) for some function f. 
        // One known construction: set a_1 = max(S), and for i>1, set a_i = min(S) if min(S) != max(S). But we saw issue with gcd>1.
        // However, if we set a_i = min(S) for all i>1, then for pair (2,4): a_2 = min, a_4 = min, gcd(min,min)=min, a_{gcd(2,4)}=a_2=min, violation.
        // So we need to break that. Maybe set a_i = something else for prime indices? 
        // Another idea: if we set a_i = S[0] (smallest) for all i except i=1, and set a_1 = S[m-1] (largest), then for any pair i,j>1, if gcd(i,j)=1, a_1 != S[0] ok. If gcd(i,j)=d>1, then a_d = S[0] (since d>1), and gcd(a_i,a_j)=S[0], violation. So we need a_d != S[0] for all d>1 that appear as gcd of some pair. That means we cannot have any index >1 with value S[0] if it can be a gcd of two indices >1. But every d>1 can be gcd of some pair (e.g., d and 2d). So all a_d for d>1 must be != S[0]. But then we need to assign different values to indices >1. This suggests we might need to assign a_i based on i.

        // Let's analyze the condition more deeply. 
        // For any i, j, let d = gcd(i,j). Then a_d != gcd(a_i, a_j).
        // Consider i=j, not allowed. For i=1, j>1: d=1, so a_1 != gcd(a_1, a_j). As argued, this means a_j != a_1.
        // For i>1, j>1: d can be 1 or >1.
        // If we set a_i = c for all i>1 (constant), then for any pair with d>1, a_d = c, gcd(a_i,a_j)=c, violation. So constant for i>1 fails if there exists any pair with d>1. Since n>=2, there is always such pair if n>=3? Actually for n=2, only pair (1,2) d=1, so constant for i>1 works: a_1 != c. So for n=2, we can set a_1 = max, a_2 = min (if min != max). If m=1, only one value, then a_1=a_2, violation. So n=2, m>=2 works.
        // For n>=3, we need to vary a_i for i>1.

        // Observation: The condition only involves gcd of indices. This is reminiscent of multiplicative functions. 
        // Maybe we can set a_i based on the prime factorization of i? 
        // Another approach: Since we want lexicographically largest, we want a_1 as large as possible, then a_2 as large as possible, etc.
        // We can try to assign a_i greedily from i=1 to n, picking the largest available element from S that doesn't violate conditions with already assigned indices.
        // But checking all pairs is O(n^2). We need a structural property.

        // Let's test small n with S = {1,2,3,...} maybe.
        // Condition: a_{gcd(i,j)} != gcd(a_i, a_j).
        // If we set a_i = i? Then a_{gcd(i,j)} = gcd(i,j), and gcd(a_i,a_j) = gcd(i,j). So equality holds, violation.
        // If we set a_i = something like: a_1 = large, and for i>1, a_i = something such that gcd(a_i, a_j) is always different from a_{gcd(i,j)}.
        // Notice that if we set a_i = 1 for all i>1, then gcd(a_i,a_j)=1. Then we need a_{gcd(i,j)} != 1. So a_d != 1 for all d>1 that appear as gcd. That means a_2, a_3, ... must not be 1. But we set them to 1, contradiction. So 1 cannot be used for i>1 if there is any pair with d>1.
        // What if we set a_i = p_i where p_i is some prime? Not necessarily in S.

        // Let's think about the structure of valid arrays. 
        // For any i, consider j = 2i. Then gcd(i,2i) = i. So condition: a_i != gcd(a_i, a_{2i}). 
        // Since gcd(a_i, a_{2i}) divides a_i, the only way a_i != gcd(a_i, a_{2i}) is if gcd(a_i, a_{2i}) < a_i. That means a_{2i} is not a multiple of a_i. In other words, a_i does not divide a_{2i}. 
        // More generally, for any i and any multiple k*i, gcd(i, k*i) = i, so a_i != gcd(a_i, a_{k*i}). This implies a_i does not divide a_{k*i}. 
        // So for any i, and any j that is a multiple of i, a_i must not divide a_j.
        // This is a strong condition! It means the sequence cannot have a_i dividing a_j whenever i divides j.
        // Conversely, if i divides j, then a_i ∤ a_j (a_i does not divide a_j).
        // Is this condition sufficient? Let's check: For any i,j, let d = gcd(i,j). Then d divides i and d divides j. So a_d ∤ a_i and a_d ∤ a_j. But we need a_d != gcd(a_i, a_j). Does a_d ∤ a_i and a_d ∤ a_j imply a_d != gcd(a_i, a_j)? Not necessarily. For example, a_d = 6, a_i = 4, a_j = 10. gcd(4,10)=2, a_d=6 != 2. But could there be a case where a_d divides both a_i and a_j? If a_d divides both, then a_d divides gcd(a_i, a_j). But we have a_d ∤ a_i, so a_d cannot divide a_i, so it cannot divide both. So a_d cannot be a divisor of gcd(a_i, a_j). However, a_d could be equal to gcd(a_i, a_j) even if it doesn't divide a_i? No, if a_d = gcd(a_i, a_j), then a_d divides a_i and a_j, contradiction. So indeed, if for all i dividing j, a_i ∤ a_j, then for any i,j with d=gcd(i,j), we have a_d ∤ a_i and a_d ∤ a_j, so a_d cannot divide gcd(a_i, a_j), hence a_d != gcd(a_i, a_j). Wait, is it possible that a_d = gcd(a_i, a_j) without dividing a_i? No, gcd(a_i, a_j) always divides a_i. So if a_d = gcd(a_i, a_j), then a_d divides a_i, contradiction. So the condition "a_i ∤ a_j for all i|j" is sufficient!
        // Let's verify necessity: We already showed for j=2i, we get a_i ∤ a_{2i}. For general i|j, let j = k*i. Then gcd(i, j) = i, so a_i != gcd(a_i, a_j). If a_i divided a_j, then gcd(a_i, a_j) = a_i, violation. So a_i ∤ a_j is necessary.
        // Therefore, the condition is exactly: For all 1 <= i < j <= n with i|j, a_i does not divide a_j.
        // This is much simpler!

        // So we need to assign a_i from S such that for no pair (i,j) with i|j, a_i | a_j.
        // And we want lexicographically largest.

        // Now, S is a subset of {1,...,n} (since x <= n). We need to pick a_i in S.
        // Lexicographically largest means we want a_1 as large as possible, then a_2 as large as possible, etc.
        // Since the condition only restricts that a_i does not divide a_j for i|j, we can think of it as: for each i, a_i cannot be a divisor of any a_j for j multiple of i. But we are assigning forward, so when assigning a_i, we need to ensure that no already assigned a_j (j|i) divides a_i? Wait, condition is i|j, so for a given i, the condition involves a_i and a_j for j multiples of i. If we assign in increasing order of i, then when we assign a_i, the multiples j > i are not assigned yet. But we also have the reverse: for j < i, if j|i, then a_j ∤ a_i. So when assigning a_i, we must ensure that for all divisors j of i (j < i), a_j does not divide a_i. That is the only constraint from already assigned elements. The future elements (multiples of i) will have to satisfy that a_i does not divide them, but we can handle that later by not picking multiples of a_i? Actually, when we later assign a_j for j multiple of i, we must ensure a_i ∤ a_j. So our choice of a_i restricts future choices: a_i cannot divide any future a_j for j multiple of i. But since we want lexicographically largest, we might want to pick a_i as large as possible, but picking a large number might divide fewer numbers? Actually, larger numbers have fewer multiples within n. But we need to consider the set S. 

        // Let's think greedily: For i=1, we want the largest possible element in S. But we must ensure that for all j>1, a_1 ∤ a_j. That means a_1 cannot divide any other a_j. Since a_1 is the first element, we can just pick the largest element in S, say M. Then for all j>1, we must ensure that a_j is not a multiple of M. But M is in S, and S subset of [1,n]. If M > n/2, then no multiple of M (except M itself) is <= n. So if M > n/2, then a_1 = M is safe because no other element can be a multiple of M (since next multiple is 2M > n). So we can safely set a_1 = M, and then for other i, we don't have to worry about a_1 dividing them. If M <= n/2, then there exist multiples of M in [1,n]. But those multiples might not be in S. However, we are allowed to pick any element from S for a_j. We can simply avoid picking multiples of M for any j>1. Since we want lexicographically largest, we might want to pick large numbers for a_j as well, but we must avoid multiples of M. Is it always possible? Maybe we can set a_1 = max(S), and then for i>1, we set a_i = max(S \ {multiples of a_1})? But we also have conditions among i>1: for i|j, a_i ∤ a_j. So we need to satisfy the same condition recursively on the set of indices >1, but with the additional restriction that no element is a multiple of a_1. Also, note that the condition for i>1 only involves indices >1, except when gcd(i,j)=1, which involves a_1. But we already handled a_1 by ensuring a_1 ∤ a_j for all j>1. Wait, the condition for i>1, j>1 with gcd(i,j)=1: a_1 != gcd(a_i, a_j). But our derived condition a_i ∤ a_j for i|j does not cover the case gcd(i,j)=1 because 1 does not divide i or j? Actually, 1 divides everything, so i|j condition includes i=1. So we already have a_1 ∤ a_j for all j>1. But does that imply a_1 != gcd(a_i, a_j) when gcd(i,j)=1? Let's check: We need a_1 != gcd(a_i, a_j). We know a_1 ∤ a_i and a_1 ∤ a_j. Could a_1 = gcd(a_i, a_j)? If a_1 = gcd(a_i, a_j), then a_1 divides a_i, contradiction. So yes, it's covered. So the condition "for all i|j, a_i ∤ a_j" is exactly equivalent to the original condition. Great!

        // So the problem reduces to: Assign a_i in S such that for all i|j, a_i does not divide a_j. Lexicographically largest.

        // Now, how to construct lexicographically largest? 
        // We can process i from 1 to n. For each i, we want to pick the largest possible value from S that does not violate the condition with any divisor j of i (i.e., a_j ∤ a_i). Also, we must consider that picking a_i might restrict future multiples, but since we are picking the largest possible, maybe we can just pick the largest available that is not a multiple of any a_j for j|i. But wait, the condition is a_j ∤ a_i, which means a_i is NOT a multiple of a_j. So a_i cannot be a multiple of any a_j where j|i. So when choosing a_i, we must avoid any value in S that is a multiple of some a_j for j|i (j < i). That's the only constraint from the past. There is no constraint from the future because we can always choose future values to not be multiples of a_i? But we must ensure that for future j with i|j, a_i ∤ a_j, i.e., a_j is not a multiple of a_i. So when we later choose a_j, we must avoid multiples of a_i. But if we greedily pick the largest possible for a_i, we might pick a value that has many multiples, which could restrict future choices and maybe lead to impossibility. However, we want lexicographically largest, so we want a_i as large as possible. If we pick a large a_i, it might have fewer multiples (since multiples are larger). Actually, larger numbers have fewer multiples <= n. So picking a larger a_i is less restrictive for the future! Because if a_i is large, its multiples are even larger and might exceed n, so they don't constrain anything. If a_i is small, it has many multiples, which could force future a_j to avoid those multiples, potentially making it impossible if S doesn't have enough non-multiples. So to maximize lexicographical order and also to keep feasibility, we should pick the largest possible element that is not a multiple of any a_j for j|i. But is that always optimal? Let's test.

        // Consider S = {2,3,4,6}, n=6. 
        // i=1: divisors: none. Largest in S not multiple of anything: 6. Pick a_1=6.
        // i=2: divisors: 1. a_1=6. We need a_2 not multiple of 6. Multiples of 6 in S: 6 (but 6 is taken? We can reuse elements? The problem says a_i in S, it doesn't say distinct. So we can reuse elements. So a_2 can be 6? But 6 is a multiple of 6, so a_1=6 divides a_2=6, violation. So a_2 cannot be a multiple of 6. So a_2 cannot be 6. Available: 2,3,4. Largest is 4. Pick a_2=4.
        // i=3: divisors: 1. a_1=6. a_3 not multiple of 6. Available: 2,3,4. Largest 4. Pick a_3=4.
        // i=4: divisors: 1,2. a_1=6, a_2=4. a_4 cannot be multiple of 6 or 4. Multiples of 6: 6. Multiples of 4: 4. So cannot be 4 or 6. Available: 2,3. Largest 3. Pick a_4=3.
        // i=5: divisors: 1. a_1=6. Cannot be multiple of 6. Available: 2,3,4. Largest 4. Pick a_5=4.
        // i=6: divisors: 1,2,3. a_1=6, a_2=4, a_3=4. Cannot be multiple of 6,4. Multiples of 4: 4. So cannot be 4,6. Available: 2,3. Largest 3. Pick a_6=3.
        // Result: 6,4,4,3,4,3. Matches sample output! So greedy works for sample.

        // Let's test another case: n=4, S={1,2,3,4}. 
        // i=1: pick 4.
        // i=2: divisors 1. Cannot be multiple of 4. Available: 1,2,3. Largest 3. Pick 3.
        // i=3: divisors 1. Cannot be multiple of 4. Available: 1,2,3. Largest 3. Pick 3.
        // i=4: divisors 1,2. a_1=4, a_2=3. Cannot be multiple of 4 or 3. Multiples of 4: 4. Multiples of 3: 3. So cannot be 3,4. Available: 1,2. Largest 2. Pick 2.
        // Result: 4,3,3,2. Check conditions: i=1,j=2: a_1=4, gcd(4,3)=1 !=4 ok. i=2,j=4: gcd(2,4)=2, a_2=3, gcd(a_2,a_4)=gcd(3,2)=1 !=3 ok. i=1,j=4: a_1=4, gcd(4,2)=2 !=4 ok. Seems valid. Lexicographically largest? Could we have 4,4,...? i=2 cannot be 4 because multiple of 4. So 4,3 is max. Then i=3 max is 3. i=4 max is 2. So yes.

        // What about S={2,4}, n=4? 
        // i=1: pick 4.
        // i=2: cannot be multiple of 4. Multiples of 4 in S: 4. So only 2 available. Pick 2.
        // i=3: pick 2 (since 4 is multiple of 4? Wait, 4 is multiple of 4, so cannot pick 4. So pick 2).
        // i=4: divisors 1,2. a_1=4, a_2=2. Cannot be multiple of 4 or 2. Multiples of 4: 4. Multiples of 2: 2,4. So no available elements! So greedy fails? But is there any valid array? Let's check manually: n=4, S={2,4}. Need a_i in {2,4}. Condition: a_1 ∤ a_2, a_1 ∤ a_3, a_1 ∤ a_4, a_2 ∤ a_4. If a_1=4, then a_2,a_3,a_4 cannot be multiples of 4, so they must be 2. Then a_2=2, a_4=2. But a_2=2 divides a_4=2, violation. If a_1=2, then a_2 cannot be multiple of 2, so a_2 must be 4. Then a_4 cannot be multiple of a_1=2 or a_2=4. Multiples of 2: 2,4. Multiples of 4: 4. So a_4 cannot be 2 or 4, impossible. So no solution. Greedy correctly fails at i=4, so we output -1. But wait, our greedy algorithm didn't output -1; it just couldn't find an element. So we need to check if at any step no element is available, then no solution exists. But is it possible that a different choice earlier would have allowed a solution? In this case, we tried both possibilities for a_1? Our greedy picked the largest, which was 4. If we picked 2 for a_1, then i=2: cannot be multiple of 2, so must pick 4. i=3: cannot be multiple of 2, pick 4. i=4: divisors 1,2. a_1=2, a_2=4. Cannot be multiple of 2 or 4. No element. So both fail. So greedy correctly identifies impossibility.

        // But is greedy always correct? We need to prove that picking the largest possible element at each step is optimal for lexicographical order and does not lead to a dead end if a solution exists. This is typical for lexicographically largest constructions: if there is a solution, the greedy choice of the largest possible element that can be extended to a full solution is optimal. We need to ensure that if we can pick a larger element now, it doesn't restrict future choices more than a smaller element would. Actually, picking a larger element is less restrictive because it has fewer multiples. So if we pick a larger element, the set of forbidden values for future indices (multiples of this element) is a subset of the forbidden set if we picked a smaller element (since a smaller element has more multiples). Therefore, picking a larger element can only increase the set of available choices for the future. So greedy is safe: if there is a solution with a smaller element at position i, we can replace it with a larger element (provided it doesn't violate constraints with past) and still have a valid solution? Wait, we need to check constraints with past: the larger element must not be a multiple of any a_j for j|i. If it is not, then it's valid. And since it's larger, it has fewer multiples, so it imposes fewer restrictions on future. So if there exists a solution with a_i = x, and we can choose y > x that also satisfies the past constraints, then we can replace x with y and still complete the rest? Not necessarily, because the future elements might have been chosen specifically to avoid multiples of x, but they might accidentally be multiples of y? But y > x, so multiples of y are a subset of multiples of x? Not exactly: multiples of y are not necessarily multiples of x. For example, x=2, y=3. Multiples of 2: 2,4,6,... Multiples of 3: 3,6,9,... They are different. So picking y=3 instead of x=2 changes the forbidden set. It might forbid some elements that were allowed under x, and allow some that were forbidden. So it's not a simple subset relation. So greedy might not be trivially safe.

        // Let's test a case where greedy might fail. Suppose S = {2,3,6}, n=6.
        // Greedy: i=1: pick 6.
        // i=2: cannot be multiple of 6. Available: 2,3. Largest 3. Pick 3.
        // i=3: cannot be multiple of 6. Available: 2,3. Largest 3. Pick 3.
        // i=4: divisors 1,2. a_1=6, a_2=3. Cannot be multiple of 6 or 3. Multiples of 6: 6. Multiples of 3: 3,6. So available: 2. Pick 2.
        // i=5: cannot be multiple of 6. Available: 2,3. Largest 3. Pick 3.
        // i=6: divisors 1,2,3. a_1=6, a_2=3, a_3=3. Cannot be multiple of 6 or 3. Available: 2. Pick 2.
        // Result: 6,3,3,2,3,2. Check condition: i=2,j=4: a_2=3, a_4=2, gcd(3,2)=1, a_{gcd(2,4)}=a_2=3 !=1 ok. i=3,j=6: a_3=3, a_6=2, gcd=1, a_3=3 !=1 ok. i=2,j=6: gcd=2, a_2=3, gcd(3,2)=1 !=3 ok. Seems valid.
        // Is there a lexicographically larger? Could we have a_2=6? No, multiple of a_1=6. a_2=3 is max. So greedy works.

        // Another test: S = {2,4,8}, n=8? But x<=n, so S subset of [1,n]. Let's try S={2,4,8}, n=8.
        // Greedy: i=1: pick 8.
        // i=2: cannot be multiple of 8. Available: 2,4. Largest 4. Pick 4.
        // i=3: pick 4.
        // i=4: divisors 1,2. a_1=8, a_2=4. Cannot be multiple of 8 or 4. Multiples of 8: 8. Multiples of 4: 4,8. So available: 2. Pick 2.
        // i=5: pick 4.
        // i=6: divisors 1,2,3. a_1=8, a_2=4, a_3=4. Cannot be multiple of 8 or 4. Available: 2. Pick 2.
        // i=7: pick 4.
        // i=8: divisors 1,2,4. a_1=8, a_2=4, a_4=2. Cannot be multiple of 8,4,2. Multiples of 2: 2,4,8. So no available! Greedy fails at i=8. Is there a solution? Let's try a different assignment. Maybe a_1=4? 
        // If a_1=4: i=2: cannot be multiple of 4. Available: 2,8. Largest 8. Pick 8.
        // i=3: pick 8.
        // i=4: divisors 1,2. a_1=4, a_2=8. Cannot be multiple of 4 or 8. Multiples of 4: 4,8. Multiples of 8: 8. So available: 2. Pick 2.
        // i=5: pick 8.
        // i=6: divisors 1,2,3. a_1=4, a_2=8, a_3=8. Cannot be multiple of 4 or 8. Available: 2. Pick 2.
        // i=7: pick 8.
        // i=8: divisors 1,2,4. a_1=4, a_2=8, a_4=2. Cannot be multiple of 4,8,2. No available. Fail.
        // a_1=2: i=2: cannot be multiple of 2. Available: 4,8. Largest 8. Pick 8.
        // i=3: pick 8.
        // i=4: divisors 1,2. a_1=2, a_2=8. Cannot be multiple of 2 or 8. Multiples of 2: 2,4,8. Multiples of 8: 8. So available: none? 4 is multiple of 2, so not allowed. So fail at i=4.
        // So no solution for S={2,4,8}, n=8. Greedy correctly fails.

        // What about a case where greedy fails but a solution exists? We need to find a counterexample if any.
        // Consider S = {3,5,6,10}, n=10? But x<=n, so 10 is allowed. Let's try to construct a case where picking a smaller element at some step allows a solution, but picking the largest leads to dead end.
        // Suppose S = {2,3,6}, n=6 we already did, greedy worked.
        // S = {2,3,4,6,12}? n=12. 
        // Greedy: i=1: pick 12.
        // i=2: cannot be multiple of 12. Available: 2,3,4,6. Largest 6. Pick 6.
        // i=3: pick 6.
        // i=4: divisors 1,2. a_1=12, a_2=6. Cannot be multiple of 12 or 6. Multiples of 12: 12. Multiples of 6: 6,12. Available: 2,3,4. Largest 4. Pick 4.
        // i=5: pick 6.
        // i=6: divisors 1,2,3. a_1=12, a_2=6, a_3=6. Cannot be multiple of 12 or 6. Available: 2,3,4. Largest 4. Pick 4.
        // i=7: pick 6.
        // i=8: divisors 1,2,4. a_1=12, a_2=6, a_4=4. Cannot be multiple of 12,6,4. Multiples of 12:12; 6:6,12; 4:4,8,12. But 8 not in S. So available: 2,3. Largest 3. Pick 3.
        // i=9: divisors 1,3. a_1=12, a_3=6. Cannot be multiple of 12,6. Available: 2,3,4. Largest 4. Pick 4.
        // i=10: divisors 1,2,5. a_1=12, a_2=6, a_5=6. Cannot be multiple of 12,6. Available: 2,3,4. Largest 4. Pick 4.
        // i=11: pick 6.
        // i=12: divisors 1,2,3,4,6. a_1=12, a_2=6, a_3=6, a_4=4, a_6=4. Cannot be multiple of 12,6,4. Available: 2,3. Largest 3. Pick 3.
        // Result seems valid. 

        // Let's try to find a counterexample systematically. The condition is: for each i, a_i cannot be a multiple of any a_j for j|i. This is equivalent to: a_i is not in the union of multiples of a_j for j|i. Since we want lexicographically largest, we want to pick the largest element not in that forbidden set. If we pick a smaller element, the forbidden set for future indices might be larger (since smaller numbers have more multiples). So picking larger seems always better or equal. But wait: if we pick a larger element, it might be a multiple of some future element? No, the condition is only about past elements dividing future elements. So picking a larger element now only adds its multiples to the forbidden set for future indices that are multiples of i. Since larger numbers have fewer multiples, the forbidden set is smaller. So picking the largest possible element minimizes the restrictions on the future. Therefore, if there is any solution, the greedy choice of the largest available element that satisfies past constraints will also lead to a solution. This is a standard exchange argument: if there is a solution where a_i = x, and we can choose y > x that is not forbidden by past, then we can replace x with y and adjust the future? But we must ensure that the future elements in the solution are not multiples of y. Since y > x, the set of multiples of y is not necessarily a subset of multiples of x. So some future element might be a multiple of y but not a multiple of x. In that case, that future element would become invalid. However, we can change that future element to something else? But we want to keep the solution valid. So we need to show that if there is a solution with a_i = x, then there is also a solution with a_i = y (the largest possible) without changing anything else? That might not be true. But we can change the future elements as well. Since we are
