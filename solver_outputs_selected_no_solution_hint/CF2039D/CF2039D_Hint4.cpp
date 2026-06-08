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
        vector<int> s(m);
        for (int i = 0; i < m; ++i) {
            cin >> s[i];
        }

        if (m == 1) {
            if (n == 1) {
                cout << s[0] << '\n';
            } else {
                cout << -1 << '\n';
            }
            continue;
        }

        // We need to assign values to indices 1..n
        // Condition: a[gcd(i,j)] != gcd(a[i], a[j]) for all i<j
        // Key observation: For any i, j with gcd(i,j)=1, we need a[1] != gcd(a[i], a[j]).
        // Since a[1] is in S, we can try to make gcd(a[i], a[j]) something else.
        // Lexicographically largest: we want a[1] as large as possible, then a[2], etc.
        // Strategy: set a[1] = max(S). Then for other positions, we can use the second largest element.
        // But we must ensure condition holds for all pairs.
        // Let's check: if we set a[1] = max(S), and all other a[i] = second_max(S) for i>1.
        // Then for any i,j with gcd(i,j)=1, a[1] = max, gcd(a[i],a[j]) = second_max (if second_max divides itself).
        // So a[1] != second_max, condition holds.
        // For pairs with gcd(i,j)=g>1, we need a[g] != gcd(a[i],a[j]).
        // If we set a[g] = second_max for all g>1, and a[i],a[j] are second_max, then gcd(second_max, second_max) = second_max.
        // Then a[g] = second_max, gcd = second_max -> violation!
        // So we need a more careful assignment.

        // Let's think: we want lexicographically largest. So a[1] should be as large as possible.
        // Can we set a[1] = max(S)? Let's test if there exists an assignment with a[1] = max(S).
        // For any i,j with gcd(i,j)=1, we need a[1] != gcd(a[i],a[j]).
        // If we set all other a[k] to some value x, then gcd(x,x)=x, so we need a[1] != x.
        // So we can choose x != a[1]. Since m>=2, we can pick x = second_max.
        // Now consider pairs with gcd(i,j)=g>1. We need a[g] != gcd(a[i],a[j]).
        // If we set a[g] = x for all g>1, and a[i],a[j] are x, then gcd(x,x)=x, so a[g] = x, violation.
        // So we cannot set all a[g] to the same x for g>1.
        // We need to assign values such that for each g, a[g] is different from gcd of values at indices that are multiples of g.
        // Notice that if we set a[1] = max, and for all i>1, we set a[i] = some value that depends on i, maybe we can satisfy.
        // But we want lexicographically largest, so we want a[2] as large as possible, etc.
        // Let's analyze the condition more deeply.

        // Consider the set of indices. The condition must hold for all pairs.
        // One known construction: set a[1] = max(S), and for i>1, set a[i] = min(S) (or some other element) but then check gcd pairs.
        // Let's test with n=6, S={3,4,6}. max=6, second=4, min=3.
        // If we set a[1]=6, a[2]=4, a[3]=4, a[4]=3, a[5]=4, a[6]=3. This is the sample output: 6 4 4 3 4 3.
        // Let's check pairs:
        // (2,3): gcd=1, a[1]=6, gcd(a[2],a[3])=gcd(4,4)=4 -> ok.
        // (2,4): gcd=2, a[2]=4, gcd(a[2],a[4])=gcd(4,3)=1 -> 4!=1 ok.
        // (3,6): gcd=3, a[3]=4, gcd(a[3],a[6])=gcd(4,3)=1 -> ok.
        // (4,6): gcd=2, a[2]=4, gcd(a[4],a[6])=gcd(3,3)=3 -> 4!=3 ok.
        // So this works.

        // How to generalize? We need to assign a[i] for each i.
        // Observe that for any g, the condition for pairs (i,j) with gcd(i,j)=g involves a[g] and gcd(a[i],a[j]).
        // If we can ensure that for all i,j with gcd(i,j)=g, gcd(a[i],a[j]) is always the same value, and that value != a[g], we might be good.
        // But gcd(a[i],a[j]) can vary.
        // Let's think about setting a[i] based on the prime factors or something.

        // Another perspective: The condition is equivalent to: for all g, and for all i,j with gcd(i,j)=g, we have a[g] != gcd(a[i],a[j]).
        // In particular, for any i>1, consider pair (1,i): gcd(1,i)=1, so a[1] != gcd(a[1], a[i]). Since gcd(a[1], a[i]) divides a[1], this means a[1] cannot divide a[i]? Actually gcd(a[1], a[i]) = a[1] iff a[1] divides a[i]. So we need a[1] does not divide a[i] for any i>1. So a[i] must not be a multiple of a[1].
        // Similarly, for any g>1, consider pair (g, 2g): gcd(g,2g)=g, so a[g] != gcd(a[g], a[2g]). This means a[g] does not divide a[2g].
        // So for every g and every multiple k*g, we need a[g] does not divide a[k*g].
        // Also for pairs (i,j) with gcd(i,j)=g, we need a[g] != gcd(a[i],a[j]). This is a stronger condition.

        // Let's try to construct: We want lexicographically largest. So we want a[1] as large as possible.
        // Can we always set a[1] = max(S)? Let's check if there is any obstruction.
        // If we set a[1] = max(S), then for any i>1, we need a[1] does not divide a[i]. So a[i] cannot be a multiple of max(S). Since all elements are <= n, and max(S) <= n, the only multiple of max(S) in S could be max(S) itself (if 2*max(S) > n). But if S contains a multiple of max(S) other than max(S), we cannot use it for any i>1. But we can still use other elements.
        // What about a[1] itself? We need a[1] != gcd(a[1], a[i]) for i>1. If we set a[i] such that gcd(a[1], a[i]) = a[1], that would be bad. So we must avoid a[i] being a multiple of a[1]. So we just don't use multiples of a[1] for i>1.
        // Now for g>1: we need to assign a[g] such that for all multiples k*g, a[g] does not divide a[k*g], and also for any i,j with gcd=g, a[g] != gcd(a[i],a[j]).
        // This seems complicated.

        // Let's look for a pattern. In the sample, a[1]=6, a[2]=4, a[3]=4, a[4]=3, a[5]=4, a[6]=3.
        // Notice that a[i] for i>1 only takes values 4 and 3. And 4 is used for indices that are not multiples of 2? Actually:
        // 2:4, 3:4, 4:3, 5:4, 6:3.
        // It seems a[i] = 4 if i is prime? No, 4 is composite but gets 3.
        // Maybe a[i] depends on the smallest prime factor? 2:4, 3:4, 4:3, 5:4, 6:3. 2 and 3 are primes, get 4. 4=2^2 gets 3. 5 prime gets 4. 6=2*3 gets 3.
        // Not clear.

        // Let's think about the condition a[gcd(i,j)] != gcd(a[i],a[j]).
        // If we set a[i] = x for all i that are not multiples of something? 
        // Consider setting a[1] = max(S). For i>1, we want to assign values such that for any g, a[g] is different from gcd of values at multiples of g.
        // One simple idea: set a[i] = second_max for all i that are prime, and a[i] = min(S) for composite i>1? Let's test with sample: second_max=4, min=3. Primes: 2,3,5 -> 4. Composites: 4,6 -> 3. That matches sample! Check: 2,3,5 get 4; 4,6 get 3. Let's verify all pairs for this assignment with S={3,4,6}, max=6, second=4, min=3.
        // a = [6,4,4,3,4,3]
        // Pairs:
        // (1,2): gcd=1, a[1]=6, gcd(6,4)=2 -> ok.
        // (1,3): gcd=1, a[1]=6, gcd(6,4)=2 -> ok.
        // (1,4): gcd=1, a[1]=6, gcd(6,3)=3 -> ok.
        // (1,5): gcd=1, a[1]=6, gcd(6,4)=2 -> ok.
        // (1,6): gcd=1, a[1]=6, gcd(6,3)=3 -> ok.
        // (2,3): gcd=1, a[1]=6, gcd(4,4)=4 -> ok.
        // (2,4): gcd=2, a[2]=4, gcd(4,3)=1 -> ok.
        // (2,5): gcd=1, a[1]=6, gcd(4,4)=4 -> ok.
        // (2,6): gcd=2, a[2]=4, gcd(4,3)=1 -> ok.
        // (3,4): gcd=1, a[1]=6, gcd(4,3)=1 -> ok.
        // (3,5): gcd=1, a[1]=6, gcd(4,4)=4 -> ok.
        // (3,6): gcd=3, a[3]=4, gcd(4,3)=1 -> ok.
        // (4,5): gcd=1, a[1]=6, gcd(3,4)=1 -> ok.
        // (4,6): gcd=2, a[2]=4, gcd(3,3)=3 -> ok.
        // (5,6): gcd=1, a[1]=6, gcd(4,3)=1 -> ok.
        // All good!

        // So the construction: a[1] = max(S). For i>1: if i is prime, a[i] = second_max(S); if i is composite, a[i] = min(S).
        // But wait, what if m=2? Then second_max = min. Then primes and composites get the same value. Let's test n=6, S={3,6}. max=6, second=3. Then a[1]=6, primes: 2,3,5 get 3; composites: 4,6 get 3. So a = [6,3,3,3,3,3].
        // Check pair (2,4): gcd=2, a[2]=3, gcd(a[2],a[4])=gcd(3,3)=3 -> violation! a[2] == gcd. So this fails.
        // So when m=2, we cannot set primes and composites to the same value if that value equals a[g] for some g.
        // In this case, a[2]=3, and gcd(3,3)=3, so violation.
        // So we need a different assignment when m=2.

        // Let's analyze m=2 separately. S = {x, y} with x < y.
        // We want lexicographically largest. So a[1] should be y.
        // Then for i>1, we must assign either x or y. But we cannot assign y to any i>1 because then a[1]=y and gcd(y, a[i]) might be y if a[i] is multiple of y. Since y <= n, the only multiple is y itself. So if we assign y to some i>1, then for pair (1,i), gcd(1,i)=1, a[1]=y, gcd(a[1],a[i])=gcd(y,y)=y -> violation. So we cannot use y for any i>1. Thus all i>1 must be x.
        // So a = [y, x, x, ..., x].
        // Now check condition for all pairs.
        // For any i,j with gcd(i,j)=1, a[1]=y, gcd(a[i],a[j])=gcd(x,x)=x. So we need y != x, which is true since distinct.
        // For any i,j with gcd(i,j)=g>1, we need a[g] != gcd(a[i],a[j]).
        // Since all a[i] for i>1 are x, gcd(a[i],a[j]) = x.
        // So we need a[g] != x for all g>1.
        // But a[g] for g>1 is x (since all i>1 are x). So a[g] = x, and gcd = x -> violation!
        // The only way to avoid this is if there are no pairs with gcd(i,j)=g>1 such that both i,j > 1? But there are always such pairs if n>=4? For n=2, the only pair is (1,2) with gcd=1, so condition holds. For n=3, pairs: (1,2) gcd=1, (1,3) gcd=1, (2,3) gcd=1. All gcd=1. So for n<=3, m=2 works with a=[y,x,x]? Let's check n=3: a=[y,x,x]. Pair (2,3): gcd=1, a[1]=y, gcd(x,x)=x -> ok. So for n<=3, it works. For n>=4, we have pair (2,4) with gcd=2, a[2]=x, gcd(a[2],a[4])=gcd(x,x)=x -> violation. So m=2 only works for n<=3? But wait, sample 3: n=2, m=1, output -1. For n=2, m=2, S={1,2}? Let's test: n=2, m=2, S={1,2}. a=[2,1]. Pair (1,2): gcd=1, a[1]=2, gcd(2,1)=1 -> ok. So works.
        // So m=2 works for n<=3? Let's test n=4, m=2, S={1,2}. a=[2,1,1,1]. Pair (2,4): gcd=2, a[2]=1, gcd(1,1)=1 -> violation. So fails.
        // But is there any other assignment for m=2, n>=4? We already argued a[1] must be max, and other positions cannot be max, so they must be min. So no other assignment. Thus for m=2, solution exists iff n <= 3? Wait, what if we don't set a[1]=max? Lexicographically largest means we want a[1] as large as possible. If we set a[1]=min, then we could set others to max? Let's check: if a[1]=x, and others=y. Then pair (1,i): a[1]=x, gcd(x,y). We need x != gcd(x,y). This is possible if x does not divide y. But then for pair (2,4): gcd=2, a[2]=y, gcd(y,y)=y -> violation again. So any assignment with all i>1 equal fails for n>=4. Could we alternate? Suppose a[1]=y, a[2]=x, a[3]=x, a[4]=y? But a[4]=y is not allowed because a[1]=y and pair (1,4) gives gcd(y,y)=y. So no i>1 can be y. So all i>1 must be x. So m=2 only works for n<=3. But wait, sample 1 has n=6, m=3, works. So m>=3 is different.

        // Let's test m=3 with the prime/composite assignment. Does it always work?
        // We need S to have at least 3 distinct elements: max, second_max, min. They could be in any order, but we need them distinct.
        // In the assignment: a[1] = max(S). For i>1: if i is prime, a[i] = second_max(S); if i is composite, a[i] = min(S).
        // We need to ensure that for all pairs, condition holds.
        // Let's analyze this assignment in general.
        // Let M = max(S), S2 = second_max(S), m_val = min(S). We assume M > S2 > m_val (strictly, since distinct).
        // For any pair (i,j) with gcd(i,j)=1: a[1]=M, gcd(a[i],a[j]).
        // What can a[i] and a[j] be? They are either S2 or m_val.
        // Possible gcds: gcd(S2, S2) = S2, gcd(S2, m_val) = g1, gcd(m_val, m_val) = m_val.
        // We need M != S2, M != g1, M != m_val.
        // Since M is the maximum, M > S2 > m_val, so M != S2 and M != m_val. But we also need M != g1. g1 = gcd(S2, m_val). Could g1 equal M? Only if M divides both S2 and m_val, but M > S2, so impossible. So condition holds for gcd=1 pairs.
        // Now consider pairs with gcd(i,j)=g>1.
        // We need a[g] != gcd(a[i],a[j]).
        // What is a[g]? If g=1, it's M, but g>1. So g is either prime or composite.
        // If g is prime, a[g] = S2.
        // If g is composite, a[g] = m_val.
        // Now a[i] and a[j] are values at multiples of g. What are these values? They depend on whether the multiple is prime or composite.
        // But note: if i is a multiple of g and g>1, then i is composite (since it has a proper divisor g). Except if i=g itself and g is prime, then i is prime. But for pair (i,j) with gcd=g, we consider i and j, both multiples of g. They could be g itself, or 2g, etc.
        // So for a given g>1:
        // - If g is prime, then a[g] = S2.
        // - For any multiple k*g: if k=1, then the index is g (prime), so a[g] = S2. If k>1, then k*g is composite (since k>=2, g>=2), so a[k*g] = m_val.
        // So in the pair (i,j) with gcd=g, the values a[i] and a[j] can be:
        //   - Both S2 (if both are g itself? But i and j must be distinct, so they can't both be g. So at most one can be g.)
        //   - One S2 (if one is g) and the other m_val (if the other is a multiple > g).
        //   - Both m_val (if both are multiples > g).
        // So gcd(a[i],a[j]) can be:
        //   - gcd(S2, m_val) = g1
        //   - gcd(m_val, m_val) = m_val
        // We need a[g] != this gcd.
        // If g is prime, a[g] = S2. We need S2 != g1 and S2 != m_val.
        // S2 != m_val is true since distinct.
        // S2 != g1: g1 = gcd(S2, m_val). Could S2 equal g1? That would mean S2 divides m_val. But S2 > m_val, so S2 cannot divide m_val unless m_val=0, but positive. So S2 != g1. So condition holds when g is prime.
        // If g is composite, a[g] = m_val.
        // For multiples of g: since g is composite, all multiples k*g for k>=1 are composite (because g itself is composite). So a[i] and a[j] are always m_val.
        // Then gcd(a[i],a[j]) = gcd(m_val, m_val) = m_val.
        // We need a[g] != m_val, but a[g] = m_val. So violation!
        // So the prime/composite assignment fails when there exists a composite g > 1 that is the gcd of some pair (i,j).
        // In the sample, n=6. Composite g's: 4, 6. But wait, in the sample, a[4]=3, and we had pair (4,6) with gcd=2, not 4. Pair with gcd=4? (4,8) but n=6, so no pair with gcd=4. Pair with gcd=6? (6,6) not allowed. So the only composite g that actually appears as gcd of some pair is 2? 2 is prime. 3 is prime. 4: is there a pair with gcd=4? (4,8) no. (4,4) no. So no pair has gcd=4. 6: no pair. So the violation for composite g only matters if there exists a pair with that gcd.
        // So we need to ensure that for every composite g that actually appears as gcd(i,j) for some i<j, a[g] != gcd(a[i],a[j]).
        // In our assignment, if g is composite and appears as gcd, then all multiples of g are composite, so a[i]=m_val for all i that are multiples of g. Then gcd(a[i],a[j]) = m_val, and a[g] = m_val, violation.
        // So to fix this, we need to change a[g] for such composite g to something else, or change the assignment for multiples.
        // But we want lexicographically largest, so we want to keep a[1]=M, and for small indices we want large values.
        // Notice that in the sample, a[4]=3 (min), and there is no pair with gcd=4, so it's fine. But if n were larger, say n=8, then pair (4,8) has gcd=4. Then a[4]=3, a[8] would be composite so 3, gcd=3, violation.
        // So the prime/composite assignment only works if for every composite g, there is no pair with gcd=g. That means n < 2*g for all composite g? Actually, if n >= 2g, then (g, 2g) is a pair with gcd=g. So we need n < 2g for all composite g. That means the smallest composite is 4, so we need n < 8? n<=7. But sample n=6 works. For n=7, composite g=4: 2g=8 >7, so no pair with gcd=4. g=6: 2g=12 >7. So n=7 works. For n=8, g=4 has pair (4,8), violation. So the simple prime/composite assignment works for n <= 7? Let's test n=8 with S={3,4,6}? But S elements must be <= n, so 6 is ok. n=8, m=3, S={3,4,6}. Assignment: a[1]=6, primes: 2,3,5,7 -> 4; composites: 4,6,8 -> 3. a = [6,4,4,3,4,3,4,3]. Check pair (4,8): gcd=4, a[4]=3, gcd(a[4],a[8])=gcd(3,3)=3 -> violation. So fails.
        // So we need a better construction for larger n.

        // Let's think differently. The condition is: for all i<j, a[gcd(i,j)] != gcd(a[i],a[j]).
        // Consider the set of indices. For each g, the condition involves a[g] and the values at multiples of g.
        // One way to satisfy this is to ensure that for every g, the values at multiples of g are all multiples of some number d, and a[g] is not a multiple of d? Or something like that.
        // Another idea: set a[i] based on the number of prime factors? Or set a[i] = max(S) for i=1, and for i>1, set a[i] = some value that is coprime to a[1]? But we need lexicographically largest.

        // Let's look at the hints.
        // Hint 1: How many pairs (i,j) with gcd(i,j)=1? Many.
        // Hint 2: What should be the answer when m=n? Then S = {1,2,...,n}. We can just set a[i] = i? Check: a[gcd(i,j)] = gcd(i,j), gcd(a[i],a[j]) = gcd(i,j). They are equal, violation. So not that.
        // Hint 3: Is it always possible when m<=n? Not always, as m=1, n>1 fails.
        // Hint 4: What happens when m=1? Only possible if n=1.

        // Let's try to find a general construction for m>=3.
        // We want lexicographically largest. So we want a[1] as large as possible, then a[2], etc.
        // Suppose we set a[1] = max(S). Then for i>1, we want to assign values such that conditions hold.
        // Consider the condition for g=1: a[1] != gcd(a[i],a[j]) for all coprime i,j.
        // If we can make gcd(a[i],a[j]) = 1 for all coprime i,j, then a[1] != 1 is easy if max(S) > 1. But if max(S)=1, then m=1, already handled.
        // How to make gcd(a[i],a[j]) = 1 for coprime i,j? We could assign a[i] = some prime? But we are limited to S.
        // Another approach: set a[i] = second_max for all i that are not multiples of something, and min for others? 
        // Let's analyze the failure for composite g. The problem was that a[g] = min, and all multiples of g got min, so gcd(min,min)=min.
        // To avoid this, we can set a[g] to something else for those g that actually appear as gcd. But we want lexicographically largest, so we want a[g] to be as large as possible.
        // Notice that for g>1, a[g] only matters if there is some pair with gcd=g. The smallest such g is 2. For g=2, we need a[2] != gcd(a[2],a[4]), etc.
        // In the prime/composite assignment, a[2]=second_max, and a[4]=min. gcd(second_max, min) could be something. We need a[2] != that gcd. That was satisfied because second_max > min, so gcd < second_max.
        // The problem was when g is composite and all its multiples are composite, so they all get min, and a[g] gets min.
        // So if we change the assignment so that not all multiples of a composite g get the same value, we might avoid the issue.
        // For example, we could assign a[i] based on the smallest prime factor of i.
        // Let's try: a[1] = max. For i>1, let p be the smallest prime factor of i. If p is something, assign a value.
        // We have three values: max, second, min. We already used max for 1.
        // We need to assign second and min to indices >1 such that for any g, the values at multiples of g are not all the same, or if they are, a[g] is different.
        // Consider setting a[i] = second if i is odd? But then even indices get min. Let's test: n=8, S={3,4,6}. max=6, second=4, min=3.
        // a[1]=6. For i>1: if i even -> 3, if i odd -> 4.
        // a = [6, 3, 4, 3, 4, 3, 4, 3]
        // Check pairs:
        // (2,4): gcd=2, a[2]=3, gcd(3,3)=3 -> violation! a[2]=3, gcd=3.
        // So fails.
        // What if we assign based on i mod something?
        // We need that for every g, there is at least one multiple of g that gets a different value, so that gcd is not uniform? Actually, we need a[g] != gcd(a[i],a[j]) for all i,j with gcd=g. If all multiples of g get the same value x, then gcd(a[i],a[j]) = x for any pair, so we need a[g] != x. So if we set a[g] = x, it's bad. So we must ensure that for every g that actually appears as gcd, either a[g] is not equal to the common value, or the multiples don't all have the same value.
        // In the prime/composite assignment, for prime g, multiples are: g itself (prime, gets second), and other multiples (composite, get min). So the values are {second, min}. The gcd of any pair could be gcd(second, min) or gcd(min, min)=min. We had a[g]=second, which is different from both (since second > min >= gcd). So it worked.
        // For composite g, all multiples are composite, so they all get min. Then a[g] was min, causing failure.
        // So if we can make sure that for composite g, not all multiples get the same value, we can fix it.
        // How to make some multiple of a composite g get a different value? We could assign a[i] based on whether i is a multiple of 4? Or based on the exponent of 2 in i?
        // Let's try: a[1]=max. For i>1: if i is a power of 2? Or if i is squarefree?
        // Consider the following: we want to use second_max on some indices and min on others. We want that for any g, the set of values at multiples of g is not a singleton {min}. So we need that for every composite g, there is at least one multiple of g that gets second_max.
        // If we assign second_max to all prime numbers, then for a composite g, its multiples include g itself (composite, gets min) and also 2g, 3g, etc. Are any of these prime? Only if g=1, but g>1. For a multiple k*g to be prime, we need k*g prime, so k=1 and g prime. So if g is composite, no multiple is prime. So all multiples get min. So that fails.
        // What if we assign second_max to all numbers that are not multiples of some number? 
        // Suppose we assign second_max to all i such that i is not a multiple of min(S)? That doesn't make sense.
        // We need a property that is preserved under taking multiples? If we assign second_max to numbers that have a certain property, and we want that for every composite g, there is a multiple of g with that property.
        // For example, assign second_max to all odd numbers >1, and min to even numbers. Then for g=2 (prime), multiples are even, all min. a[2] would be min? But we want a[2] to be second_max? Let's see: if we set a[1]=max, a[2]=second_max? But 2 is even, so if we assign by parity, a[2] would be min. But we can set a[2] separately? We want lexicographically largest, so we want a[2] as large as possible. So we might want a[2]=second_max.
        // So maybe we should assign values greedily: for i=1 to n, assign the largest possible value from S that doesn't conflict with already assigned values.
        // But checking conflicts might be expensive.

        // Let's think about the condition more structurally.
        // The condition a[gcd(i,j)] != gcd(a[i],a[j]) must hold for all i<j.
        // Consider the function f(i) = a[i]. We want f(gcd(i,j)) != gcd(f(i), f(j)).
        // This is reminiscent of multiplicative functions? If f is completely multiplicative, then f(gcd(i,j)) = gcd(f(i), f(j))? Actually, if f is multiplicative and we define it on primes, then for any i,j, gcd(f(i), f(j)) = f(gcd(i,j)) if f is a divisor-preserving function? Not exactly.
        // If we set a[i] = c for all i>1, then for g>1, a[g]=c, gcd(c,c)=c, violation.
        // If we set a[i] = something like: a[1] = x, and for i>1, a[i] = y if i is odd, z if i is even. Then for g=2, multiples are even, so all z. a[2]=z, gcd(z,z)=z, violation.
        // So we need that for every g>1, there exist two multiples of g with different values, so that their gcd is not necessarily the same as a[g]? But even if they have different values, their gcd could still equal a[g]. We need to ensure it doesn't.

        // Let's try to find a construction that works for all n when m>=3.
        // Consider the following: a[1] = max(S). For i>1, let a[i] = second_max(S) if i is a prime or a power of a prime? Or something.
        // Actually, look at the sample: n=6, S={3,4,6}. a = [6,4,4,3,4,3]. Here, a[2]=4, a[3]=4, a[4]=3, a[5]=4, a[6]=3.
        // Notice that a[i] = 4 for i in {2,3,5} (primes) and a[i]=3 for i in {4,6} (composites).
        // What if we extend this to larger n? For n=8, we would have a[7]=4, a[8]=3. Then pair (4
