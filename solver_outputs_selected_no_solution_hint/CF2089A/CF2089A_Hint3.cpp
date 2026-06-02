#include <bits/stdc++.h>
using namespace std;

bool is_prime(int x) {
    if (x < 2) return false;
    for (int i = 2; i * i <= x; ++i)
        if (x % i == 0) return false;
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<int> p(n);
        // We want c_i = ceil(prefix_sum / i) to be prime for many i.
        // Observation: if we set p_1 = 2, p_2 = 1, then c_1 = 2, c_2 = 2 (both prime).
        // For i >= 3, we can try to make c_i = 2 or 3 (both primes) by controlling prefix sums.
        // Let prefix sum S_i. We want ceil(S_i / i) = 2 or 3.
        // ceil(S_i / i) = 2  <=>  S_i in (i, 2i]
        // ceil(S_i / i) = 3  <=>  S_i in (2i, 3i]
        // We can achieve this by placing numbers appropriately.
        // A known construction: start with [2, 1], then for i from 3 to n, place i at position i.
        // Let's check: p = [2, 1, 3, 4, 5, ..., n]
        // S_1 = 2 -> c_1 = 2 (prime)
        // S_2 = 3 -> c_2 = 2 (prime)
        // S_3 = 6 -> c_3 = 2 (prime)
        // S_4 = 10 -> c_4 = 3 (prime)
        // S_5 = 15 -> c_5 = 3 (prime)
        // S_6 = 21 -> c_6 = 4 (not prime) -> fails for n>=6.
        // So we need a better construction.
        // Hint: "How can we get rid of non-prime c_i?" suggests we can adjust the permutation to avoid non-prime c_i.
        // Notice that c_i is always between 2 and n+1 roughly. We want many of them to be 2 or 3.
        // If we can keep prefix sums such that S_i is close to 2i or 3i, we get prime c_i.
        // Another idea: place numbers so that prefix sums increase slowly.
        // Let's try to make c_i = 2 for as many i as possible.
        // For c_i = 2, we need S_i <= 2i. Since S_i >= 1+2+...+i = i(i+1)/2, this is only possible for small i.
        // For larger i, we need c_i = 3, requiring S_i <= 3i.
        // The sum of first k numbers is k(k+1)/2. For k around n, this is O(n^2), much larger than 3n.
        // So we cannot keep c_i small for all i.
        // Wait, the problem only requires at least floor(n/3) - 1 primes among c_i.
        // So we don't need all c_i to be prime, just about n/3 of them.
        // We can try to make c_i prime for i in some specific positions.
        // Observation: If we put all numbers in increasing order except some adjustments, c_i will eventually become large and non-prime.
        // But we can control c_i by placing large numbers early to increase prefix sum quickly, making c_i larger, but we want primes.
        // Actually, we want c_i to be prime. Primes are dense enough that we might just rely on random chance? But we need a constructive solution.
        // Let's think about the hints: "What if we just put all primes in the beginning?" "It won't work." "How can we get rid of non-prime c_i?"
        // Maybe we can make c_i = 2 for i=1,2,3 and then c_i = 3 for some more, and then we don't care.
        // Let's analyze the required number of primes: floor(n/3) - 1.
        // For n=2: floor(2/3)-1 = 0-1 = -1? Actually floor(2/3)=0, so 0-1 = -1, but we need at least 0 primes? The problem says "at least floor(n/3)-1 prime numbers". For n=2, floor(2/3)=0, so at least -1 primes, which is always true. But in sample, they output 2 primes. So it's fine.
        // For n=3: floor(3/3)-1 = 1-1 = 0. Sample output has 3 primes.
        // For n=5: floor(5/3)-1 = 1-1 = 0. Sample output has 5 primes.
        // So the requirement is very weak. We can easily satisfy it.
        // In fact, we can just output any permutation and likely have enough primes? But we need a guaranteed construction.
        // Let's try to make the first few c_i prime, and then we don't care.
        // If we set p = [2, 1, 3, 4, 5, ..., n], we get c = [2,2,2,3,3,4,4,5,5,...]. Primes: 2,2,2,3,3,5,5,... So many primes.
        // For n=6: c = [2,2,2,3,3,4] -> primes: 2,2,2,3,3 (5 primes). floor(6/3)-1 = 2-1=1. So 5 >= 1, works.
        // For n=7: c = [2,2,2,3,3,4,4] -> primes: 2,2,2,3,3 (5 primes). floor(7/3)-1 = 2-1=1. Works.
        // For n=8: c = [2,2,2,3,3,4,4,5] -> primes: 2,2,2,3,3,5 (6 primes). floor(8/3)-1 = 2-1=1. Works.
        // For n=9: c = [2,2,2,3,3,4,4,5,5] -> primes: 2,2,2,3,3,5,5 (7 primes). floor(9/3)-1 = 3-1=2. Works.
        // For n=10: c = [2,2,2,3,3,4,4,5,5,6] -> primes: 2,2,2,3,3,5,5 (7 primes). floor(10/3)-1 = 3-1=2. Works.
        // It seems this simple construction works for all n? Let's check larger n.
        // For n=100: c_i will be roughly i/2 + something. Many c_i will be composite, but we only need about 33 primes. The first few are primes, and then occasionally we hit primes. Is it guaranteed? Not necessarily, but we can prove it works?
        // Actually, the problem guarantees a solution exists, but we need to output one. The simple [2,1,3,4,...,n] might not always have enough primes. For example, if n is large, c_i grows, and primes become sparse. We need floor(n/3)-1 primes. For n=100, we need 32 primes. The first 100 numbers have 25 primes. c_i takes values from 2 to about 50. There are 15 primes up to 50. So we might not get 32 primes. So the simple construction fails for large n.
        // We need a better construction.
        // Let's think: we want c_i to be prime for many i. c_i = ceil(S_i / i). We can control S_i by placing numbers.
        // If we place numbers such that S_i is always just above a multiple of i, we can make c_i equal to that multiple.
        // For example, if we want c_i = 2, we need S_i in (i, 2i]. If we want c_i = 3, need S_i in (2i, 3i].
        // We can try to keep S_i close to 2i or 3i for as long as possible.
        // Notice that S_i is the sum of a permutation. The minimum possible S_i is 1+2+...+i = i(i+1)/2. The maximum is n+(n-1)+...+(n-i+1) = i(2n-i+1)/2.
        // For c_i to be 2, we need S_i <= 2i. But i(i+1)/2 <= 2i => i+1 <= 4 => i <= 3. So only i=1,2,3 can have c_i=2 if we use the smallest numbers. But we can use larger numbers to increase S_i? No, we want S_i small to keep c_i small. So we must use the smallest numbers first. So c_i=2 only for i<=3.
        // For c_i=3, we need S_i <= 3i. i(i+1)/2 <= 3i => i+1 <= 6 => i <= 5. So i<=5 can have c_i=3 with smallest numbers.
        // For c_i=5, need S_i <= 5i => i(i+1)/2 <= 5i => i+1 <= 10 => i <= 9.
        // In general, for c_i = p (prime), we need i(i+1)/2 <= p*i => i+1 <= 2p => i <= 2p-1.
        // So if we put numbers in increasing order, c_i will be roughly i/2. The number of primes among c_i will be about the number of primes up to n/2, which is about n/(2 log n). For n=1e5, n/(2 log n) ~ 50000/(11.5) ~ 4300, which is much larger than n/3 ~ 33333? Wait, n/3 = 33333, 4300 is less. So we need about 33333 primes, but we only get 4300. So increasing order fails.
        // We need to make c_i prime more often. How? By making c_i smaller? But we can't make c_i smaller than i/2 in increasing order. To make c_i smaller, we need to use even smaller numbers? But we already use the smallest numbers. So c_i cannot be smaller than about i/2 if we use the smallest numbers. So c_i grows at least linearly. The number of primes up to n/2 is about n/(2 log n), which is less than n/3 for large n. So we cannot rely on c_i being small primes.
        // Alternative: make c_i large primes? But large primes are sparse.
        // Wait, the requirement is floor(n/3) - 1 primes. For n=1e5, that's 33332 primes. The number of primes up to n is about 9592. So we cannot have that many distinct primes. But c_i can repeat primes. The same prime can appear multiple times. So we can have many c_i equal to the same prime.
        // So we want to make c_i equal to a few small primes repeatedly.
        // How to make c_i = 2? Only for i<=3.
        // How to make c_i = 3? Only for i<=5 if we use smallest numbers. But if we use larger numbers, S_i increases, so c_i increases. To keep c_i = 3 for larger i, we need S_i <= 3i. But S_i is at least i(i+1)/2. So i(i+1)/2 <= 3i => i <= 5. So impossible for i>5.
        // So we cannot have c_i = 3 for i>5.
        // What about c_i = 5? Need i(i+1)/2 <= 5i => i <= 9.
        // In general, for a fixed prime p, we can have c_i = p only for i <= 2p-1.
        // So to have many c_i equal to a prime, we need that prime to be large. But if p is large, 2p-1 is large, so we can have many i with c_i = p. But we need S_i <= p*i. Since we use the smallest numbers, S_i = i(i+1)/2. So we need i(i+1)/2 <= p*i => i+1 <= 2p => i <= 2p-1. So if we set p ~ n/2, then we can have i up to n. But then p is about n/2, which might not be prime. We need p to be prime.
        // So we can choose a prime p around n/2, and then for all i <= 2p-1 (which is about n), we can have c_i = p, provided we can keep S_i <= p*i. But S_i = i(i+1)/2. For i = 2p-1, S_i = (2p-1)*2p/2 = p(2p-1) = p*i. So S_i = p*i exactly. Then c_i = ceil(p*i / i) = p. For i < 2p-1, S_i < p*i, so c_i <= p. But we want c_i = p exactly? Actually ceil(S_i/i) = p means S_i in ((p-1)i, p*i]. So we need S_i > (p-1)i. With S_i = i(i+1)/2, we need i(i+1)/2 > (p-1)i => i+1 > 2(p-1) => i > 2p-3. So for i > 2p-3, c_i = p. For smaller i, c_i < p. But those smaller i might have c_i equal to smaller primes.
        // So if we put numbers in increasing order 1,2,3,...,n, then for a prime p, c_i = p for i in [2p-2, 2p-1]? Let's check: i=2p-2: S_i = (2p-2)(2p-1)/2 = (p-1)(2p-1) = 2p^2 -3p +1. p*i = p(2p-2) = 2p^2 -2p. S_i = 2p^2 -3p +1 <= 2p^2 -2p for p>=1. So S_i <= p*i. Also (p-1)i = (p-1)(2p-2) = 2p^2 -4p +2. S_i = 2p^2 -3p +1 > 2p^2 -4p +2 for p>1. So S_i in ((p-1)i, p*i], so c_i = p. For i=2p-3: S_i = (2p-3)(2p-2)/2 = (2p-3)(p-1) = 2p^2 -5p +3. p*i = p(2p-3) = 2p^2 -3p. S_i <= p*i. (p-1)i = (p-1)(2p-3) = 2p^2 -5p +3. S_i = exactly (p-1)i, so ceil is p-1. So c_i = p-1. So indeed, with increasing order, c_i takes each integer value k for exactly two i's: i=2k-2 and i=2k-1? Let's test: k=2: i=2,3 -> c_2=2, c_3=2. k=3: i=4,5 -> c_4=3, c_5=3. k=4: i=6,7 -> c_6=4, c_7=4. So c_i = floor(i/2) + 1? Actually for i=1: c_1=1. So pattern: c_1=1, then for i>=2, c_i = floor((i+2)/2)? Let's check: i=2: floor(4/2)=2, i=3: floor(5/2)=2, i=4: floor(6/2)=3, i=5: floor(7/2)=3. Yes, c_i = floor((i+2)/2) for i>=2.
        // So with increasing order, c_i takes all integer values from 1 to about n/2. The number of primes among these is the number of primes up to n/2, which is about n/(2 log n). For n=1e5, that's ~4300, which is less than 33333. So increasing order fails.
        // We need to modify the permutation to make c_i equal to a prime for many more i.
        // How can we make c_i stay at a prime for a longer range? We need S_i to stay within ((p-1)i, p*i] for a larger range of i. With increasing order, S_i grows quadratically, so it quickly exceeds p*i. To keep S_i <= p*i, we need to slow down the growth of S_i. That means we should use smaller numbers later? But we already used the smallest numbers first. If we delay some small numbers, we can slow down the prefix sum growth.
        // Idea: instead of 1,2,3,4,5,..., we can put some larger numbers early to increase S_i quickly at the beginning, then put small numbers later to slow down the growth. But we want c_i to be prime. If we put large numbers early, c_i becomes large quickly, maybe hitting a prime. Then we can keep c_i at that prime by carefully controlling S_i.
        // Let's think about making c_i = p for a long range. We want S_i in ((p-1)i, p*i]. The width of this interval is i. So we have some flexibility. We can adjust the permutation to keep S_i within this interval for many i.
        // Suppose we want c_i = 2 for i=1,2,3. That's easy.
        // Then we want c_i = 3 for i=4,5,... up to some k. To have c_i=3, we need S_i in (2i, 3i]. With increasing order, S_4=10, 3*4=12, so c_4=3. S_5=15, 3*5=15, c_5=3. S_6=21, 3*6=18, so S_6 > 18, c_6=4. So we lost c_i=3 at i=6. To keep c_i=3 for i=6, we need S_6 <= 18. But with numbers 1..6, the minimum sum is 21. So impossible. We would need to use numbers smaller than 1..6? But we must use a permutation of 1..n. So we can't have sum less than 21 for the first 6 numbers. So c_i=3 is impossible for i>=6.
        // In general, for any prime p, the maximum i for which we can have c_i = p is when the minimum possible sum of i distinct positive integers is <= p*i. The minimum sum is i(i+1)/2. So we need i(i+1)/2 <= p*i => i+1 <= 2p => i <= 2p-1. So the maximum length of a run of c_i = p is from i=2p-2 to 2p-1? Actually, we can have c_i = p for i up to 2p-1, but we also need S_i > (p-1)i. The minimum sum i(i+1)/2 might be <= (p-1)i for small i. So c_i = p only for i near 2p. Specifically, for i such that (p-1)i < S_i <= p*i. With minimum sum, S_i = i(i+1)/2. So we need i(i+1)/2 > (p-1)i => i+1 > 2(p-1) => i > 2p-3. So with the smallest possible numbers, c_i = p exactly for i = 2p-2 and 2p-1. If we use larger numbers, S_i is larger, so the condition S_i > (p-1)i is easier, but S_i <= p*i becomes harder. So to maximize the range of i with c_i = p, we want S_i as small as possible. So using the smallest numbers is optimal for keeping c_i small. But that gives only 2 occurrences per prime.
        // So we cannot have a long run of the same prime if we use the smallest numbers. But we can have many different primes. The number of primes up to n/2 is about n/(2 log n). For n=1e5, that's 4300. We need 33333 primes. So we need each prime to appear many times. But we just saw each prime can appear at most 2 times with the smallest numbers. So we need a different strategy.
        // Wait, the requirement is floor(n/3) - 1 primes among c_i. For n=1e5, that's 33332. There are 1e5 c_i values. We need about 1/3 of them to be prime. If we can make c_i prime for 1/3 of the indices, we win.
        // How can we make c_i prime? We can try to make c_i equal to 2 or 3 as much as possible, but we saw that's limited. What if we make c_i jump between primes? For example, we can make c_i = 2 for i=1,2,3, then c_i = 3 for i=4,5, then c_i = 5 for i=6..9, then c_i = 7 for i=10..13, etc. In general, for a prime p, we can have c_i = p for i in [2p-2, 2p-1] if we use the smallest numbers. That gives 2 occurrences per prime. The number of primes up to n/2 is about n/(2 log n). So total prime occurrences = 2 * n/(2 log n) = n/log n. For n=1e5, n/log n ~ 100000/11.5 ~ 8700, which is still less than 33333. So we need more.
        // But wait, we are not forced to use the smallest numbers in order. We can rearrange to make S_i grow slower? No, the minimum sum of i distinct numbers from 1..n is fixed as i(i+1)/2. So S_i >= i(i+1)/2. So c_i >= ceil((i+1)/2). So c_i grows at least linearly. The number of primes up to n/2 is about n/(2 log n). The number of indices i is n. So the density of primes among possible c_i values is about 1/(2 log n). To have 1/3 of c_i prime, we need the density to be 1/3, which is much higher. So we must have many c_i taking the same prime value. But we saw that for a fixed prime p, the condition c_i = p requires S_i in ((p-1)i, p*i]. The minimum S_i is i(i+1)/2. So we need i(i+1)/2 <= p*i => i+1 <= 2p => i <= 2p-1. So the maximum i for which c_i can be p is 2p-1. Also, we need S_i > (p-1)i. With minimum S_i, this requires i > 2p-3. So with minimum S_i, c_i = p only for i = 2p-2, 2p-1. But if we make S_i larger than the minimum, we can have S_i > (p-1)i for smaller i, but then we risk exceeding p*i. So we can potentially have c_i = p for a range of i if we carefully control S_i to be just above (p-1)i and just below p*i. The width of the interval is i. So we can have S_i increase slowly enough to stay in this interval for several consecutive i.
        // How slowly can S_i increase? S_i = S_{i-1} + p_i. The minimum increase is 1 (if we add the smallest available number). So S_i can increase by at least 1 each step. The interval ((p-1)i, p*i] has width i. So if we start at the bottom of the interval, we can increase S_i by up to i over the next step and still stay in the interval? Actually, for index i, the interval is ((p-1)i, p*i]. For index i+1, the interval is ((p-1)(i+1), p(i+1)]. The overlap of these intervals? We want S_{i+1} = S_i + x to be in the new interval. The new lower bound is (p-1)(i+1) = (p-1)i + (p-1). The old upper bound is p*i. So if S_i is near p*i, then S_{i+1} could be up to p*i + x. We need S_{i+1} <= p(i+1) = p*i + p. So we need x <= p. Also we need S_{i+1} > (p-1)(i+1) = (p-1)i + p-1. If S_i > (p-1)i, then S_{i+1} > (p-1)i + x. We need (p-1)i + x > (p-1)i + p-1 => x > p-1. So x must be exactly p? Actually x > p-1 and x <= p, so x = p. So to stay in the c=p regime from i to i+1, we need to add exactly p. But p is a number we might not have available. And we need to do this for many steps, adding p each time. But we only have one of each number. So we can't add p repeatedly.
        // This suggests that keeping c_i constant for many consecutive i is difficult because the required increment is around p, but we have limited numbers.
        // Alternative: let c_i vary among primes. We don't need long runs of the same prime; we just need many primes overall. Maybe we can make c_i alternate between 2 and 3? But we saw c_i=2 only for i<=3, c_i=3 only for i<=5. So that's only 5 primes.
        // Wait, maybe we can make c_i larger primes by using larger numbers early. For example, if we put n, n-1, ... early, S_i becomes large quickly, so c_i becomes large. Large primes are sparse, but we only need n/3 primes. If we can make c_i hit primes with density 1/3, we win. Is the density of primes around some value high enough? No, prime density around x is 1/log x. For x ~ n, density is 1/log n, which is < 1/3 for n>20. So we can't rely on random hits.
        // We need a systematic way to make c_i prime.
        // Let's re-read the problem: "among c_1..c_n there must be at least floor(n/3) - 1 prime numbers." For n=2, floor(2/3)-1 = -1, so 0 primes needed. For n=3, 0 primes needed. For n=4, floor(4/3)-1 = 1-1=0. For n=5, 0. For n=6, floor(6/3)-1 = 2-1=1. So for n<=5, we need 0 primes. For n=6, we need 1 prime. For n=7, 1. For n=8, 1. For n=9, floor(9/3)-1 = 3-1=2. So the requirement is very weak for small n, but for large n, it's about n/3.
        // Maybe there is a simple construction that guarantees n/3 primes. What if we output the permutation such that c_i is always either 2 or 3? We saw that's impossible for large n. But maybe we can make c_i = 2 or 3 for the first few, and then make c_i = something else prime?
        // Let's think about the hint: "How can we get rid of non-prime c_i?" This suggests we can modify the permutation to turn non-prime c_i into prime c_i. Maybe we can swap some elements to change specific c_i.
        // Consider the increasing order permutation: 1,2,3,...,n. c_i = floor((i+2)/2) for i>=2. The non-primes are when floor((i+2)/2) is composite. We want to change those c_i to prime. How can we change c_i? By swapping elements, we change prefix sums. Swapping an earlier element with a later element increases some prefix sums and decreases others? Actually, if we swap p_i and p_j (i<j), then for k between i and j-1, S_k changes by (p_j - p_i). So we can increase or decrease S_k. This changes c_k = ceil(S_k/k). We can try to adjust S_k so that c_k becomes prime.
        // Maybe we can make all c_i equal to 2 or 3 by carefully placing numbers? Let's test if it's possible for n=6 to have all c_i prime. Minimum sum for i=6 is 21. To have c_6 <= 3, we need S_6 <= 18. Impossible. So c_6 >= 4. 4 is not prime. So for n>=6, c_6 cannot be prime if we want c_i <=3? Wait, c_6 could be 5 (prime). To have c_6=5, we need S_6 in (24, 30]. Minimum S_6 is 21, maximum is 6+5+4+3+2+1=21? Actually maximum sum of 6 numbers from 1..6 is 21. So S_6 is exactly 21. So c_6 = ceil(21/6) = 4. So for n=6, c_6 is always 4, which is composite. So we cannot make all c_i prime for n=6. But the requirement is only 1 prime for n=6. So we can have some composites.
        // So the goal is to have at least n/3 primes. We can afford many composites.
        // Let's try to construct a permutation that yields many primes. One idea: place numbers such that prefix sums are often just above a multiple of i that is prime. For example, we want S_i ≈ p*i for some prime p. We can try to make p = 2 for small i, p = 3 for next, p = 5 for next, etc. But we need to control S_i.
        // Another idea: use the permutation where we put all odd numbers first, then even numbers? Or something like that.
        // Let's analyze the c_i sequence for some permutations.
        // Permutation: n, n-1, ..., 1. Then S_i is large. c_i will be around n. Primes are sparse.
        // Permutation: 1, 3, 2, 5, 4, 7, 6, ... (alternating). Not sure.
        // Let's search for a pattern. The sample outputs for n=2,3,5 are [2,1], [2,1,3], [2,1,3,4,5]. This is exactly the "2,1,3,4,5,...,n" construction. For n=5, it works and gives all primes. For n=6, as we saw, c_6=4 composite, but we still have primes at i=1,2,3,4,5 (5 primes) which is >=1. So [2,1,3,4,5,6] works for n=6. For n=7: [2,1,3,4,5,6,7] gives c: 2,2,2,3,3,4,4 -> primes: 2,2,2,3,3 (5 primes) >=1. For n=8: c: 2,2,2,3,3,4,4,5 -> primes: 2,2,2,3,3,5 (6) >=1. For n=9: c: 2,2,2,3,3,4,4,5,5 -> primes: 2,2,2,3,3,5,5 (7) >=2. For n=10: c: 2,2,2,3,3,4,4,5,5,6 -> primes: 2,2,2,3,3,5,5 (7) >=2. For n=11: c: 2,2,2,3,3,4,4,5,5,6,6 -> primes: 2,2,2,3,3,5,5 (7) >= floor(11/3)-1 = 3-1=2. Works.
        // Let's check n=100. c_i = floor((i+2)/2). Primes occur when floor((i+2)/2) is prime. That is when i+2 is twice a prime. So i = 2p-2 or 2p-1. So for each prime p, we get two indices. Number of primes up to 51 (since max c_i ~51) is 15. So 30 primes. floor(100/3)-1 = 33-1=32. 30 < 32. So for n=100, [2,1,3,...,100] fails! We need 32 primes, but we only get 30. So the simple construction fails for n=100.
        // We need a better construction for large n.
        // Let's think: we need about n/3 primes. The simple construction gives about 2 * (number of primes up to n/2) ≈ n/log n. For n=100, n/log n ≈ 100/4.6 ≈ 21.7, but we got 30 because 100 is small. For n=1000, n/log n ≈ 1000/6.9 ≈ 145, but we need 333 primes. So it fails badly for large n.
        // We need a construction that yields c_i prime for about 1/3 of indices. How can we achieve that?
        // Notice that c_i = ceil(S_i/i). If we can make S_i = p*i for many i, then c_i = p. So we want S_i to be a multiple of i. That means the average of the first i numbers is an integer p. So we want the sum of the first i numbers to be divisible by i, and the quotient to be prime.
        // Can we partition the set {1,...,n} into prefixes with prime averages? This is like finding a permutation where each prefix average is prime.
        // There is a known result: for any n, there exists a permutation of 1..n such that all prefix averages are integers? Not necessarily. But we only need many of them to be prime.
        // Let's try to construct a permutation where c_i = 2 for i=1,2,3; c_i = 3 for i=4,5; c_i = 5 for i=6,7,8,9; c_i = 7 for i=10,11,12,13; etc. In general, for the k-th prime p_k, we want c_i = p_k for i in some range. The range for p_k could be from 2p_k - something to 2p_k + something? Actually, to have c_i = p, we need S_i in ((p-1)i, p*i]. If we want this for a block of consecutive i, we need to add numbers such that S_i stays in this interval.
        // Suppose we have a block of length L where we want c_i = p. Let the block start at i = s. We need S_s in ((p-1)s, p*s]. Then for each next index, we add a number x. We need S_{i+1} = S_i + x to be in ((p-1)(i
