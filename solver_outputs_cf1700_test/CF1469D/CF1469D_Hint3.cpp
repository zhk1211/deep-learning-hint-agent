// Hint3
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
        vector<pair<int, int>> ops;
        
        // We will use a recursive/iterative approach:
        // Keep track of current maximum value > 2 and reduce it using ceil division by something close to sqrt.
        // We'll maintain a set of "active" indices that are not yet 1 or 2.
        // Actually simpler: we can just process numbers from n down to 3, reducing each to 1 using a helper.
        // The helper is the number 2 (which we keep as the final 2) or some intermediate.
        // But we need to create the 2 first. Initially a_i = i.
        // Strategy: pick a "pivot" = 2? But we need exactly one 2 at the end.
        // Standard solution: use the number 2 as the divisor for all numbers > 2, but that takes too many steps for large numbers.
        // Instead, we reduce large numbers by repeatedly dividing by something like sqrt(x) until it becomes 1.
        // We'll keep a variable 'two_idx' = 2 (the index that will remain 2). We'll also use some other numbers as intermediate divisors.
        
        // We'll simulate the process:
        vector<int> a(n + 1);
        for (int i = 1; i <= n; i++) a[i] = i;
        
        // We'll reduce all numbers from n down to 3, except we might use some as divisors.
        // The idea: for each x from n down to 3, if a[x] > 1, we repeatedly do x = ceil(x / y) where y is around sqrt(x).
        // We'll choose y = 2 if x is small, else we'll use some number close to sqrt(x) that we haven't reduced yet.
        // But we need to ensure we don't ruin the 2.
        // We'll keep index 2 as the final 2. We'll also keep index 'sq' as a helper that we reduce later.
        
        // Let's implement the known solution:
        // We'll use a recursive function that reduces a range [L, R] to all ones except one 2.
        // But simpler: we can just do operations and record them.
        
        // We'll maintain a list of indices that are > 2 and not yet 1.
        // We'll repeatedly take the largest value > 2, and divide it by something slightly smaller than it (like sqrt) until it becomes 1.
        // The divisor we choose will be an index whose value is around sqrt(current value). We can just use the number 2 for small values.
        
        // Actually, we can do this:
        // Let's keep a variable 'cur' = n. We'll reduce cur to 1 by repeatedly dividing by something.
        // We'll use a helper index 'h' which we'll also reduce later.
        // Standard approach from editorial:
        // We'll pick an index 'sq' = ceil(sqrt(n)) or something, and use it to reduce n.
        // Then we reduce 'sq' using 2, etc.
        
        // Let's just simulate with a priority queue of (value, index) for values > 2.
        // We'll also keep track of the index that currently holds value 2 (initially index 2).
        // While there is a value > 2, we take the largest, and we need a divisor. The divisor should be the smallest value >= sqrt(largest) that is available.
        // But we can just use the index with value = 2 as divisor for all? No, too many steps for large n.
        // So we'll use a "chain" of reductions.
        
        // We'll implement the following known construction:
        // For n, we choose x = ceil(sqrt(n)). We repeatedly do n = ceil(n / x) until n becomes 1. This takes about 2 steps for n=200000.
        // Then we need to reduce x to 1, but we can use 2 for that since x is about sqrt(n) <= 450.
        // For x up to 450, using 2 takes at most about 9 steps (since 2^9=512). So total steps <= n + 5.
        // We'll do this recursively: reduce the largest number using a smaller number, then reduce that smaller number using an even smaller one, etc.
        
        // We'll maintain a list of "targets" to reduce: start with n.
        // We'll also keep a set of "available" numbers that are not yet 1 or 2, but we can use them as divisors.
        // Actually, we can just do:
        // Let's create a vector of operations.
        // We'll define a function reduce(val, divisor_idx) that applies operations to reduce a[index] from val to 1 using a[divisor_idx] as divisor.
        // But we need to know the index of val. Initially a[i]=i.
        
        // We'll work with indices. Let's keep an array cur_val.
        // We'll process numbers from n down to 3. For each i, if cur_val[i] > 1, we'll reduce it to 1 using a helper.
        // The helper will be the smallest index j such that cur_val[j] >= sqrt(cur_val[i]) and cur_val[j] > 1.
        // But we can just use a fixed strategy: for i from n down to 3, we'll reduce i using the number 2 if i is small, else we'll use a number around sqrt(i).
        // To avoid complex logic, we can precompute a sequence of operations that works for any n up to 2e5.
        // The standard solution: 
        // We'll keep an array a. We'll do operations on indices.
        // We'll pick a "pivot" = 2. We'll also pick a "sqrt_pivot" = ceil(sqrt(n)).
        // We'll reduce n using sqrt_pivot until n becomes 1. Then we reduce sqrt_pivot using 2 until sqrt_pivot becomes 1.
        // But we also have other numbers from 3 to n-1 (except sqrt_pivot). They need to become 1. We can just use 2 for all of them? For numbers up to n, using 2 directly takes up to log2(n) steps, which for n=2e5 is ~18 steps per number, too many total.
        // So we need to reduce all numbers > 2 efficiently.
        // The trick: we only need to reduce numbers that are > 2. We can do it by repeatedly applying the operation with a divisor that is roughly sqrt of the current value.
        // We can process numbers in descending order. For each number x (starting from n down to 3), if it's not already 1, we reduce it to 1 by repeatedly dividing by a number y that is around sqrt(x). We can choose y as the current value of some index that we haven't reduced yet and is close to sqrt(x). But we can just use the number 2 for all? No.
        // Let's think differently: We can use the number 2 to reduce all numbers from 3 to n, but we do it in a chain: we reduce n using something, then that something using something else, etc.
        // Actually, the editorial solution: 
        // We can keep the number 2 as the final 2. For all other numbers i from 3 to n, we want to make them 1.
        // We can do this by repeatedly taking the largest number > 2, and dividing it by the smallest number that is >= sqrt(largest) and > 1. This smallest number will be used as a divisor and will itself be reduced later.
        // Since we process in descending order, the divisor will be processed later.
        // This is essentially the same as the recursive approach.
        
        // Let's implement a simple iterative method that works:
        // We'll maintain a list of indices that are not 1 or 2. Initially all from 3 to n.
        // We'll repeatedly pick the largest index i from the list (its value is cur[i]).
        // We need a divisor j from the list such that cur[j] is roughly sqrt(cur[i]). We can just pick the smallest index in the list whose value is >= sqrt(cur[i]). Since we process in descending order, we can just use the next smaller index? Not exactly.
        // But we can just use the number 2 as the divisor for all? Let's check step count: For n=200000, using 2 directly on 200000 takes ceil(log2(200000)) = 18 steps. Then we have 199997 other numbers (3 to 199999). If we use 2 on each, that's 18*199997 ~ 3.6e6 steps, way over n+5.
        // So we must use larger divisors.
        
        // The known solution uses the fact that we can reduce a number x to 1 in about 2 steps by using y = ceil(sqrt(x)). Then we need to reduce y, which we can do with 2 if y is small, or recursively.
        // So we can do a recursive function:
        // function solve(l, r): we want to reduce all numbers in [l, r] to 1, except we keep one 2 (which we'll have at index 2).
        // But we already have 2 at index 2. We need to reduce numbers > 2.
        // We can define a function reduce(x, y) that applies operations to make a[x] become 1 using a[y] as divisor, assuming a[y] is > 1.
        // We'll start with x = n, y = ceil(sqrt(n)). We'll reduce n using y. Then we'll need to reduce y using something else, say z = ceil(sqrt(y)), etc., until we hit 2.
        // This forms a chain. All other numbers (not in the chain) can be reduced directly using 2 because they are small? Wait, numbers not in the chain are from 3 to n-1, they can be large too.
        // Actually, the chain covers only a few numbers (the ones we use as divisors). The rest we can just reduce using 2? But if we have a number like 199999, using 2 takes 18 steps. That's too many if we do it for all.
        // So we need to reduce every number > 2 using a divisor close to its sqrt.
        // But we can't use a custom divisor for each number because that would require many divisors, each needing reduction.
        // The trick: we only need to reduce numbers that are > 2. We can do it by repeatedly applying the operation with the same divisor for many numbers? No.
        // Let's read the hints carefully:
        // Hint 1: Use recursion. Try to find a way to make array [1, x] consist of x-1 ones and 1 two.
        // Hint 2: Consider x = 100000. How many divisions do we need to perform to get rid of this element?
        // Hint 3: What should we choose as y? Answer: y ≈ sqrt(x).
        // So the idea: For a given x, we can reduce it to 1 by repeatedly dividing by y = ceil(sqrt(x)). This takes about 2 steps (since x^(1/2) -> x^(1/4) -> ... -> 1). Then we need to reduce y to 1, which we can do recursively.
        // So we can define a recursive function that reduces a set of numbers. But we have many numbers.
        // Actually, we can apply this to the whole array: we want to reduce all numbers from 3 to n to 1. We can do it by processing numbers in descending order. For each number i from n down to 3, if it's not 1, we reduce it to 1 by repeatedly dividing by a number j that is roughly sqrt(i). We can choose j as the current value of some index that we haven't processed yet and is close to sqrt(i). Since we process descending, j will be processed later.
        // But we need to ensure j exists. We can just use the number 2 for all? No.
        // Let's think: We can set y = 2 for all numbers? That's too many steps.
        // What if we use y = i-1? Then ceil(i/(i-1)) = 2 for i>2, then we need another step to make it 1. That's 2 steps per number, total 2*(n-2) steps, which is about 2n, too many (n+5 allowed).
        // So we need something more efficient.
        
        // The standard solution for this problem (CF 1730B? Actually it's 1730A? No, it's "Ceil Divisions" maybe) is:
        // We keep the number 2 as the final 2. We pick a number x = n. We repeatedly do x = ceil(x / y) where y = ceil(sqrt(x)). We do this until x becomes 1. The number of steps for x is at most 2 (for n=2e5, sqrt(2e5)≈447, sqrt(447)≈22, sqrt(22)≈5, sqrt(5)≈3, sqrt(3)≈2, so about 5 steps). Then we need to reduce the y's we used. We can reduce them using 2, because they are at most 447, and 447 using 2 takes about 9 steps. So total steps for the chain is small.
        // What about all other numbers from 3 to n-1? We can reduce them using 2 directly? But 199999 using 2 takes 18 steps. That's too many if we do it for all.
        // Wait, the problem allows n+5 steps total. For n=200000, n+5=200005. If we use 2 for each of the 199998 numbers, taking on average maybe 18 steps, total steps ~3.6e6, which is > 200005. So we cannot use 2 for all.
        // So we must use a different strategy for the bulk of numbers.
        // Let's re-read the problem: "Your goal is to make array a consist of n-1 ones and 1 two in no more than n+5 steps." So we have a budget of roughly n steps.
        // If we can reduce each number > 2 to 1 in exactly 1 step, that would be n-2 steps, plus some extra for the chain. But can we reduce a number to 1 in 1 step? ceil(x/y)=1 implies y >= x. But we only have numbers up to n. So we can't.
        // What about 2 steps per number? That would be 2*(n-2) ~ 2n, too many.
        // So we need a method where most numbers are reduced in 1 step, and only a few take more.
        // How can we reduce a number x to 1 in 1 step? We need y such that ceil(x/y)=1 => y >= x. The only number >= x is x itself (or larger, but we don't have larger). So we can't do it in 1 step unless we use x itself, but x != y.
        // So we must use multiple steps for some numbers, but we can share the cost.
        // The key insight: We can use the same divisor for many numbers. For example, if we have a divisor d, we can reduce many numbers x to 1 by doing ceil(x/d) repeatedly? No, ceil(x/d) is not necessarily 1. To get 1, we need d >= x. So we can't use a single divisor for many numbers unless that divisor is very large.
        // But we can use a chain: we reduce a large number using a slightly smaller number, which becomes 2, then we use that 2 to reduce others? No.
        
        // Let's look at the sample: n=4. Operations: 3 4; 4 2; 4 2. Here they reduced 3 using 4 (ceil(3/4)=1). Then they reduced 4 using 2 twice (4->2->1). So 3 took 1 step, 4 took 2 steps. Total 3 steps = n-1? n=4, n+5=9, they used 3 steps.
        // For n=3: 3 2; 3 2. 3 took 2 steps. Total 2 steps.
        // So for n=4, they used 4 as a divisor for 3. Then they reduced 4 using 2.
        // This suggests a pattern: Use the largest number as a divisor for all numbers that are smaller than it but large enough that ceil(x/y)=1. That is, if y is the largest number, then for any x <= y, ceil(x/y)=1. So we can reduce all numbers from 3 to y-1 to 1 in one step each, using y as the divisor! Then we need to reduce y to 1, which we can do using 2 (or recursively).
        // Let's check: For n=4, y=4. Numbers to reduce: 3. 3 <= 4, so ceil(3/4)=1. One step. Then reduce 4 using 2: two steps. Total steps: 1 + 2 = 3. Works.
        // For n=5: y=5. Reduce 3,4 using 5: each 1 step. Then reduce 5 using 2: ceil(5/2)=3, ceil(3/2)=2, ceil(2/2)=1 -> 3 steps. Total: 2 + 3 = 5 steps. n+5=10, okay.
        // For n=6: y=6. Reduce 3,4,5 using 6: 3 steps. Reduce 6 using 2: ceil(6/2)=3, ceil(3/2)=2, ceil(2/2)=1 -> 3 steps. Total 6 steps.
        // For n=7: y=7. Reduce 3..6 using 7: 4 steps. Reduce 7 using 2: ceil(7/2)=4, ceil(4/2)=2, ceil(2/2)=1 -> 3 steps. Total 7 steps.
        // For n=8: y=8. Reduce 3..7: 5 steps. Reduce 8 using 2: ceil(8/2)=4, ceil(4/2)=2, ceil(2/2)=1 -> 3 steps. Total 8 steps.
        // In general, if we use y = n, we need (n-2) steps to reduce numbers 3..n-1, plus the steps to reduce n to 1 using 2. The number of steps to reduce n using 2 is ceil(log2(n)). For n=200000, log2(200000) ~ 18. So total steps = (n-2) + 18 = n + 16. But the limit is n+5. So 16 > 5, fails for large n.
        // So we need to reduce the largest number faster. We can use a chain: reduce n using something like sqrt(n), then reduce that using 2. This reduces the steps for the largest number from ~18 to ~2+something.
        // But then what about the numbers between sqrt(n) and n? We can't use n as divisor because n is being reduced. We need to use some other large divisor.
        // Idea: We can pick a set of "divisors" that form a chain. For example, let x1 = n, x2 = ceil(sqrt(x1)), x3 = ceil(sqrt(x2)), ... until we reach 2. We'll use x2 to reduce x1, x3 to reduce x2, etc. For all other numbers (not in the chain), we can use the largest element in the chain that is >= them? But the chain elements are decreasing. The largest is n. If we use n as divisor for all numbers from 3 to n-1, that's too many steps for n itself. But we can use n as divisor for numbers that are close to n, and use smaller divisors for smaller numbers.
        // Actually, we can do this: We'll process numbers in descending order. We'll maintain a current "divisor" d. Initially d = n. For i from n-1 down to 3, we can reduce i using d if i <= d? But d is changing as we reduce it.
        // Let's think of a standard solution I've seen for this problem (CF 1455C? No, it's "Ceil Divisions" from Codeforces Round #680? Actually it's 1462C? No.)
        // I recall a problem: "Ceil Divisions" where you have to make array of ones and one two. The solution is to use the number 2 and a chain of numbers: n, ceil(sqrt(n)), ceil(sqrt(ceil(sqrt(n)))), ... down to 2. For each number in the chain, you reduce it using the next number in the chain. For all other numbers, you reduce them using the largest number in the chain (which is n) or using the chain element that is just larger than them? Wait, if you use n for all other numbers, n will be used many times as a divisor. But the operation is a_x = ceil(a_x / a_y). Using n as divisor doesn't change n. So we can use n as divisor for many numbers without changing n. Then after we've reduced all other numbers to 1, we reduce n using the chain. That works! Let's check:
        // Suppose we have chain: n, s1 = ceil(sqrt(n)), s2 = ceil(sqrt(s1)), ..., 2.
        // Step 1: For all i from 3 to n-1 except the chain elements, do operation (i, n). Since a_n = n, and a_i = i <= n-1 < n, ceil(i/n) = 1. So each such i becomes 1 in one step. The number of such i is (n-2) - (chain_length - 1). Chain length is small (about log log n).
        // Step 2: Now we have the chain elements left: n, s1, s2, ..., 2. We need to reduce them to 1 except 2. We can do: reduce n using s1 (a few steps), then reduce s1 using s2, etc., finally reduce the last before 2 using 2.
        // Let's count steps: 
        // - Reducing all non-chain numbers: about n steps.
        // - Reducing chain: for each chain element, it takes about 2 steps (since we use sqrt). Chain length for n=2e5: n=200000, s1=448, s2=22, s3=5, s4=3, s5=2. So chain length = 5 (including 2). Non-chain numbers = n - 2 - (5 - 1) = n - 6. Steps for non-chain = n - 6.
        // Steps for chain: reduce 200000 using 448: ceil(200000/448)=447? Actually 200000/448 ≈ 446.4, ceil = 447. Then 447/448 = 1? Wait, we need to reduce 200000 to 1 using 448. Operation: a_n = ceil(a_n / a_s1). Initially a_n=200000, a_s1=448. 200000/448 = 446.428, ceil=447. So a_n becomes 447. Next: 447/448 = 0.997, ceil=1. So 2 steps. Then we need to reduce 448 using 22: 448/22=20.36, ceil=21; 21/22=1 -> 2 steps. 22 using 5: 22/5=4.4 ceil=5; 5/5=1 -> 2 steps. 5 using 3: 5/3=1.66 ceil=2; 2/3=1 -> 2 steps. 3 using 2: 3/2=2; 2/2=1 -> 2 steps. Total chain steps = 2*5 = 10. Total steps = (n - 6) + 10 = n + 4. This is within n+5! For n=200000, n+4=200004 <= 200005. Perfect.
        // But wait: In the chain, we also have the number 2 which we don't reduce. So chain length from n down to 3 is 4 elements (n, s1, s2, s3, s4? Actually s4=3, s5=2). So non-chain numbers = n - 2 - 4 = n - 6. Steps = n - 6 + 10 = n + 4.
        // For smaller n, chain length might be smaller, but still within n+5.
        // Let's test n=3: chain: 3, 2. Non-chain: none. Steps: reduce 3 using 2: 2 steps. Total 2 <= 3+5=8. Works.
        // n=4: chain: 4, 2 (since sqrt(4)=2). Non-chain: 3. Steps: 3 using 4: 1 step. Reduce 4 using 2: 2 steps. Total 3 <= 9. Works.
        // n=5: chain: 5, 3? sqrt(5)=2.23 ceil=3. So chain: 5, 3, 2. Non-chain: 4. Steps: 4 using 5: 1 step. Reduce 5 using 3: ceil(5/3)=2; ceil(2/3)=1 -> 2 steps. Reduce 3 using 2: 2 steps. Total: 1+2+2=5. n+5=10. Works.
        // So this strategy works!
        
        // Implementation:
        // For each test case:
        // 1. Build the chain: start with cur = n. chain = [n]. while cur > 2: cur = ceil(sqrt(cur)); if cur >= 2, add cur to chain. Ensure we don't add duplicates? Actually sqrt might give same number? For n=4, sqrt=2, add 2. For n=3, sqrt=1.73 ceil=2, add 2. So chain ends with 2. We'll keep 2 at the end.
        // 2. The chain elements are indices (since a_i = i initially). We'll record operations.
        // 3. For all i from 3 to n, if i is not in the chain, do operation (i, n). This makes a_i = 1.
        // 4. Now reduce the chain elements in order: for each adjacent pair (u, v) in chain where u > v, we reduce u using v. But note: after previous operations, the values might have changed? For non-chain numbers, we used n as divisor, but n's value hasn't changed yet. So a_n is still n. The chain elements' values are still their original indices because we haven't modified them yet (except we might have modified some if they were used as divisors? No, we only used n as divisor, which doesn't change n. The other chain elements were not used as divisors yet. So their values are intact.)
        // So we can proceed: for each chain element from largest to smallest (except the last one which is 2), we reduce it to 1 using the next element in the chain. But we must be careful: when we reduce u using v, we do operations (u, v) repeatedly until a_u becomes 1. Since a_v might be > 1, we need to ensure we don't ruin a_v. But a_v will be reduced later. That's fine.
        // However, we must ensure that when we reduce u using v, a_v is not 1. Initially a_v = v > 1. So it's fine.
        // We'll simulate the reduction: while a[u] > 1, do op(u, v). But we can just compute the number of steps: it's the number of times we need to apply ceil(x/y) until x becomes 1. We can just do a while loop in code, since the total steps is small.
        // But we need to output the operations. So we'll just simulate and record.
        
        // Let's refine:
        // We'll keep an array cur_val of size n+1, initialized to i.
        // We'll have a vector of pairs for operations.
        // Step 1: Identify chain indices.
        vector<int> chain;
        int cur = n;
        chain.push_back(cur);
        while (cur > 2) {
            cur = (int)ceil(sqrt((double)cur));
            // avoid infinite loop if cur doesn't change? For cur=2, we stop. For cur=3, sqrt=1.73 ceil=2, so next is 2. So it's fine.
            if (cur >= 2) chain.push_back(cur);
            else break;
        }
        // Remove duplicates? For n=4: chain = [4, 2]. For n=5: 5 -> ceil(sqrt(5))=3 -> ceil(sqrt(3))=2 -> chain = [5,3,2]. No duplicates.
        // But for n=2? n>=3 so fine.
        // We'll use a boolean array to mark chain elements.
        vector<bool> is_chain(n+1, false);
        for (int idx : chain) is_chain[idx] = true;
        
        // Step 2: Reduce all non-chain numbers from 3 to n using n.
        for (int i = 3; i <= n; i++) {
            if (!is_chain[i]) {
                // operation (i, n)
                ops.push_back({i, n});
                cur_val[i] = (cur_val[i] + cur_val[n] - 1) / cur_val[n]; // ceil division
                // Since cur_val[i] <= n-1 and cur_val[n] = n, ceil(i/n) = 1. So we can just set to 1.
                cur_val[i] = 1;
            }
        }
        
        // Step 3: Reduce chain elements from largest to smallest (except 2).
        // The chain is in decreasing order: [n, s1, s2, ..., 2].
        for (size_t j = 0; j + 1 < chain.size(); j++) {
            int u = chain[j];
            int v = chain[j+1];
            // reduce u using v until cur_val[u] becomes 1.
            while (cur_val[u] > 1) {
                ops.push_back({u, v});
                cur_val[u] = (cur_val[u] + cur_val[v] - 1) / cur_val[v];
            }
        }
        
        // Output
        cout << ops.size() << "\n";
        for (auto [x, y] : ops) {
            cout << x << " " << y << "\n";
        }
    }
    return 0;
}
