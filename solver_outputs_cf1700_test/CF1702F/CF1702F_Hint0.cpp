// Hint0
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
        vector<int> a(n), b(n);
        for (int i = 0; i < n; ++i) cin >> a[i];
        for (int i = 0; i < n; ++i) cin >> b[i];

        multiset<int> ma(a.begin(), a.end());
        multiset<int> mb(b.begin(), b.end());

        while (!ma.empty()) {
            int x = *ma.rbegin();
            int y = *mb.rbegin();

            if (x == y) {
                ma.erase(prev(ma.end()));
                mb.erase(prev(mb.end()));
            } else if (x > y) {
                // need to reduce x? but we cannot change a.
                // Actually we can only change b, so if x > y, we must increase y or decrease x?
                // Wait: we can only change b. So if x > y, we cannot match y to x by increasing y?
                // We can double y, but that makes it larger, not smaller. So if x > y, we cannot match.
                // But we can halve x? No, we cannot change a.
                // So if x > y, we must halve x? No, we cannot change a.
                // Let's think: we can only change b. So if the largest in a is greater than largest in b,
                // we cannot match it because we can only increase b (double) or decrease b (halve).
                // If x > y, we could halve x? No, a is fixed.
                // So the only way is if x can be obtained from y by doubling? But x > y, so doubling y might reach x.
                // But we are comparing largest elements. If x > y, we cannot match x with y because y is smaller.
                // We could try to halve x? No.
                // Actually, we can only change b. So if x > y, we must increase y (double) to match x, but that makes y larger.
                // But if we double y, it becomes even larger, so it won't match x if x > y.
                // Wait: if x > y, doubling y makes it larger, so it could become equal to x? Yes, if y*2 == x.
                // But we are comparing largest elements. If x > y, we could double y to get x? But then y becomes larger,
                // and we might have other elements. However, we are processing from largest to smallest.
                // If x > y, we cannot match x with y by halving y (since y is smaller). We could double y, but then y becomes larger,
                // and we would need to match that larger value with something in a. But a's largest is x, which is smaller than the doubled y.
                // So doubling y would make it exceed x, and then we would need to halve it later? That seems inefficient.
                // Let's reconsider the operation: we can double or halve b elements.
                // We want to transform each b_i into some a_j.
                // The operation is reversible? Halving is not exactly reversible because of floor.
                // But we can think of each number as having a set of reachable values by repeatedly doubling or halving.
                // For a given b, we can reach any number that is obtained by repeatedly applying *2 or /2 (floor).
                // Notice that if we have a number, we can always reduce it by halving until it becomes 1.
                // So we can always make numbers smaller. Making them larger is limited by doubling.
                // A common approach: try to match the largest elements first.
                // If the largest in a is greater than the largest in b, we cannot match it because we can only make b larger by doubling,
                // but that would make it even larger, so it would never match a smaller number.
                // Wait: if x > y, we could halve x? No, a is fixed.
                // So if x > y, it's impossible? Let's test with sample: a = {4,24,5,2}, b = {4,1,6,11}. Sorted a: 2,4,5,24; b: 1,4,6,11.
                // Largest a=24, largest b=11. 24 > 11. But we can double 11 to 22, then 44? No, we doubled 6 to 12 to 24.
                // So we didn't match 24 with 11. We matched 24 with 6 by doubling twice.
                // So the largest in a (24) was matched with a smaller element in b (6) by doubling it.
                // So the largest in a is not necessarily matched with the largest in b.
                // So the greedy approach of matching largest with largest doesn't work directly.

                // Alternative: For each element in a, we can try to reduce it by halving until we find a match in b.
                // Since we can only change b, we can think of reducing a elements? No, a is fixed.
                // But we can simulate the process: we can always halve b elements, so we can reduce any b element to a smaller number.
                // We can also double b elements, but doubling is only useful if we need a larger number.
                // However, note that if we have a number in a, we can try to see if some b can be transformed into it.
                // Since we can halve b arbitrarily, we can always reduce b to any number that is obtained by repeated halving.
                // So for a given a_i, we can check if there is some b_j that can reach a_i by a sequence of doublings and halvings.
                // But the operations are not symmetric: halving has floor, so doubling then halving might not return to the original.
                // However, we can think in terms of the "base" number by removing all factors of 2.
                // Let f(x) be x with all factors of 2 removed (i.e., divide by 2 while even).
                // Then any number y can be transformed into x if and only if f(x) == f(y) and the number of doublings/halvings can adjust the power of 2?
                // Not exactly: because of floor, halving an odd number reduces it, and then further halving can change the odd part.
                // Example: 3 -> halve -> 1. f(3)=3, f(1)=1. They are different. So 3 can become 1 by halving.
                // So the odd part can change by halving (when odd, floor division by 2 removes the odd part and reduces the number).
                // So the transformation is not just about powers of 2.

                // Let's think differently: We can always halve any number repeatedly until it becomes 1.
                // So from any b, we can reach any number that is <= b and can be obtained by repeated halving.
                // But we can also double, so we can reach numbers larger than b.
                // However, doubling is only useful if we need a larger number. But we can also halve after doubling.
                // Notice that if we double then halve, we might get back to the original or something else.
                // Actually, the set of reachable numbers from x is all numbers that can be obtained by repeatedly applying:
                // x -> 2x, or x -> floor(x/2).
                // This is equivalent to: we can multiply by 2 any number of times, and we can divide by 2 (floor) any number of times.
                // But the order matters because of floor.
                // However, we can think of the process as: we can always reduce a number by halving until it becomes 1.
                // So if we have a target a_i, we can try to see if some b_j can be reduced to a_i by halving, or if a_i can be reduced to some b_j by halving?
                // Wait, we can only change b. So we need to transform b_j into a_i.
                // We can double b_j to make it larger, then halve it to make it smaller? That seems redundant.
                // Actually, doubling increases the number, halving decreases it. So to reach a_i from b_j, if b_j < a_i, we must double it some number of times, possibly with some halvings in between? But halving would reduce it, so we wouldn't want to halve if we need to increase. So if b_j < a_i, we can only double it until it becomes >= a_i, and then we can halve it to reduce to exactly a_i? But halving after doubling might not hit a_i exactly because of floor.
                // Example: b=3, a=5. 3*2=6, halve 6 -> 3. Not 5. 3*2*2=12, halve 12->6, halve 6->3. So we can't get 5.
                // So it's not always possible.

                // Let's think about the reverse: if we can transform b into a, then we can also transform a into b by reversing operations? Not exactly, because halving is not invertible.
                // But we can consider the process from a's perspective: we can apply the inverse operations? The inverse of doubling is halving (exact), and the inverse of halving is doubling? But halving floor is not invertible.
                // However, note that if we have a multiset a, we can try to match elements by reducing them (halving) until they match some b. But we are allowed to change b, not a.
                // But we can think of it as: we can apply operations to b to match a. Alternatively, we can apply the "inverse" operations to a to match b? If we can transform b to a, then starting from a, we can apply the reverse operations to get b? The reverse of doubling is halving (exact), and the reverse of halving is doubling? But halving floor is not invertible because multiple numbers halve to the same number. However, if we consider the set of numbers that can be transformed into a given number, it's the set of numbers that can be reduced to it by halving and doubling? Actually, if b can be transformed to a, then a can be transformed to b? Not necessarily, because operations are not symmetric. But we can think: if we have a sequence of operations from b to a, we can reverse the sequence: replace *2 with /2, and /2 with *2. But /2 in the original might be floor, so reversing it with *2 might not give the exact original if the number was odd. However, if we only care about existence, we can try to match by reducing both a and b to some "canonical" form?

                // Another common solution: Use a max-heap or multiset for a and b. While a is not empty, take the largest element from a. If it exists in b, remove both. Else, if the largest in a is larger than the largest in b, then we cannot match it because we can only increase b (by doubling) but that would make it even larger, so we would never match a smaller number? Wait, if largest a > largest b, we could halve the largest a? But we can't change a. So we must change some b to match that large a. But the largest b is smaller, so we need to increase some b to match that large a. But if we increase the largest b by doubling, it becomes even larger, so it won't match the large a (which is smaller than the doubled value). So we must use a smaller b and double it multiple times to reach the large a. But then that smaller b, after doubling, might become larger than the current largest b. So the largest in b might not be the one we use to match the largest in a.
                // So the greedy of matching largest with largest doesn't work.

                // Let's think differently: We can always halve any number. So if we have a number in a, we can repeatedly halve it until it becomes 1. At each step, we can check if that halved value exists in b. If it does, we can match them by doubling that b element back to the original a? But wait, if we halve a, we get a smaller number. If that smaller number is in b, then we can double that b element to get back to a? Not necessarily, because halving a might involve floor, and doubling the halved value might not give back a if a was odd. Example: a=5, halve -> 2. If b contains 2, can we get 5 from 2? 2*2=4, 4*2=8, halve 8->4, etc. We can't get 5. So that doesn't work.

                // Let's consider the set of numbers that can be transformed into a given target x. Starting from x, what numbers can be transformed into x? We can apply operations in reverse: from x, we can either halve (if we came from doubling) or double (if we came from halving). But since halving floor is not invertible, from x we can go to 2x (reverse of halving) or to x/2 if x is even? Actually, if we had y and we halved it to get x, then y could be 2x or 2x+1. So from x, the possible predecessors are 2x and 2x+1. And if we had y and doubled it to get x, then y must be x/2, and x must be even. So from x, we can go to x/2 if x is even (reverse of doubling), or to 2x and 2x+1 (reverse of halving). So the set of numbers that can reach x is all numbers that can be reduced to x by repeatedly applying: if even, can divide by 2; or can go to floor(y/2) = x, meaning y is 2x or 2x+1. So we can think of a tree where each node x has children: x*2, x*2+1, and if x is even, x/2? Wait, we are going backwards from target to source. So from target x, we can go to possible sources: if we think of the forward operation: source y -> target x. Operations: y -> 2y (so x=2y => y=x/2, x even); or y -> floor(y/2) = x => y = 2x or 2x+1. So from x, the possible sources are: if x even, x/2; and always 2x and 2x+1. So we can generate all numbers that can be transformed into x by starting from x and going up this tree. But this tree grows exponentially.

                // However, we have a multiset b. We want to match each a_i to some b_j such that b_j can be transformed into a_i. This is equivalent to: for each a_i, there must be some b_j that can reach a_i. And each b_j can be used only once.
                // Since we can only halve or double b, we can think of the process as: we can reduce b_j by halving until we get a number that matches some a_i, and then we can double it back? But we don't need to double back; we just need to reach a_i. So we can just halve b_j repeatedly until it equals some a_i. But we can also double before halving? Doubling before halving might allow us to reach numbers that are not reachable by just halving? Example: b=3, a=5. Halving 3 gives 1. Can't reach 5. Double 3 to 6, halve 6 to 3. Still can't. Double 3 to 12, halve to 6, halve to 3. So no. What about b=6, a=5? Halve 6 -> 3, no. Double 6 -> 12, halve -> 6, no. So 6 can't reach 5. But 5 can be reached from 10? 10 halve -> 5. So if b=10, we can halve to 5. So to reach an odd number a, we need a b that is either a, or 2a, or 4a, etc., or numbers that after halving become a? Actually, if we halve an odd number, we get floor(odd/2). So to get an odd number a, we need a b such that after some halvings we get a. If we halve an even number, we get an integer. If we halve an odd number, we get a smaller number. So to get a, we can start from b and halve until we get a. That means b must be in the set of numbers that eventually yield a when repeatedly halved. That set is: a, 2a, 2a+1, 4a, 4a+1, 4a+2, 4a+3, ... i.e., all numbers whose binary representation is a shifted left by some number of bits, with any lower bits. In other words, b can be transformed into a if and only if a can be obtained by repeatedly halving b. That is, if we halve b enough times, we get a. This is equivalent to: a is a prefix of b in binary? More precisely, if we write b in binary, then repeatedly halving (floor) is just shifting right by one bit. So a can be obtained from b by halving if and only if a is equal to b shifted right by some number of bits. That is, a = floor(b / 2^k) for some k >= 0. So b must be in the range [a * 2^k, a * 2^k + 2^k - 1] for some k. That is, b must be such that a is a prefix of b's binary representation. So the condition is: a can be obtained by halving b. But we can also double b before halving? Doubling b is just shifting left. If we double b then halve, we might get back to b or something else. But if we double b and then halve multiple times, we are essentially shifting left then right. That is equivalent to just shifting right by some amount (if we halve more than we doubled) or shifting left (if we double more than we halved). But we can also interleave them. However, note that doubling then halving is just the identity if we halve immediately after doubling? 2b halved is b. If we double, then do some other operations, then halve, it might be different. But actually, any sequence of doublings and halvings can be simplified: each doubling adds a 0 at the right (in binary), each halving removes the rightmost bit. So the net effect is that we can change the number of bits, and we can also change the lower bits by halving odd numbers? Wait, halving an odd number removes the rightmost 1 and shifts. So it's not just shifting; it's shifting and losing the LSB. So the set of reachable numbers from b is all numbers that can be obtained by: starting from b, we can repeatedly either shift left (add 0) or shift right (remove LSB). This is equivalent to: we can change the length of the binary string, and we can also drop the LSB at any time. So from b, we can reach any number that is a prefix of b (by halving) or any number that is b shifted left by some amount (by doubling). But we can also drop some LSBs after doubling? For example, b=3 (11), double to 6 (110), halve to 3 (11). Not new. Double to 12 (1100), halve to 6 (110), halve to 3 (11). So we only get numbers that are prefixes of b shifted left? Actually, if we double, we add zeros on the right. Then if we halve, we remove bits from the right. So we can effectively remove any number of bits from the right, but we can also add zeros on the right before removing? That just means we can have any number of trailing zeros that we can remove. So the set of numbers reachable from b is: all numbers that can be written as floor(b * 2^k / 2^m) for some k,m >= 0. That is, we can multiply by a power of 2 and then divide by a power of 2 (floor). This is equivalent to: we can shift b left by k bits, then shift right by m bits. If k >= m, we get b shifted left by (k-m). If k < m, we get b shifted right by (m-k), i.e., floor(b / 2^{m-k}). So the reachable numbers are exactly: b * 2^p for any p >= 0, and floor(b / 2^q) for any q >= 0. In other words, we can only multiply by powers of 2, or divide by powers of 2 (floor). We cannot do combinations that yield something else? What if we double, halve, double, halve? That's just multiplying by 2, dividing by 2, etc. The net effect is still just a power of 2 multiplication or division. Because doubling and halving are inverses except for the floor. But if we halve an odd number, we lose information. However, if we double first, we make it even, then halving gives back the original. So any sequence of operations can be reduced to: we can optionally halve some number of times (maybe zero), then optionally double some number of times. But wait, if we halve first, we might lose bits, then doubling will add zeros, so we get a different number. So the order matters. But the set of reachable numbers is exactly: starting from b, we can go down by halving any number of times, and from any of those, we can go up by doubling any number of times. So the reachable set is: for any q >= 0, let b_q = floor(b / 2^q). Then for any p >= 0, b_q * 2^p is reachable. So the reachable numbers are all numbers of the form floor(b / 2^q) * 2^p. That is, we can remove some lower bits (by halving), then add some zeros (by doubling). So the binary representation of a reachable number is: take the binary of b, remove some number of least significant bits (maybe zero), then append some number of zeros. So it's a prefix of b (maybe the whole b) followed by some zeros. In other words, a number x is reachable from b if and only if there exists some q such that x / 2^p = floor(b / 2^q) for some p? Actually, x = floor(b / 2^q) * 2^p. This means that if we remove all trailing zeros from x, we get a number that is a prefix of b (i.e., can be obtained by halving b some number of times). More precisely, let odd(x) be x with all factors of 2 removed (i.e., x >>= __builtin_ctz(x)). Then x is reachable from b iff odd(x) is reachable from b by halving only? Because doubling only adds trailing zeros. So x is reachable from b iff odd(x) can be obtained from b by halving some number of times. And halving b gives a sequence of numbers: b, floor(b/2), floor(b/4), ... So odd(x) must appear in that sequence. That is, odd(x) must be equal to floor(b / 2^q) for some q. And since odd(x) is odd, it can only be obtained if at some halving step we get that odd number. So condition: x is reachable from b iff by repeatedly halving b, we eventually get odd(x). But wait, what if odd(x) is not odd? It is odd by definition. So the condition is: if we repeatedly halve b, we must hit odd(x). But note that halving b eventually reaches 1. So odd(x) must be in the halving sequence of b. That means odd(x) must be a prefix of b in binary? Actually, halving b is just shifting right. So the sequence of numbers is b, b>>1, b>>2, ... So odd(x) must equal b>>q for some q. That means odd(x) is a prefix of b's binary representation (the higher bits). So x is reachable from b iff the odd part of x is a prefix of b (i.e., can be obtained by right-shifting b some number of times). Equivalently, if we write b in binary, then odd(x) must be a prefix of that binary string. And then we can add any number of trailing zeros to get x.

                // So for each a_i, we can compute its odd part (a_i with all trailing zeros removed). Then we need to match it with some b_j such that odd(a_i) is a prefix of b_j. But we also need to account for the fact that we can double b_j? Wait, we are transforming b_j into a_i. So we need a_i to be reachable from b_j. According to above, a_i is reachable from b_j iff odd(a_i) is in the halving sequence of b_j. That is, odd(a_i) = floor(b_j / 2^q) for some q. So we need to find for each a_i a b_j such that by halving b_j we can get odd(a_i). But note that we can also double b_j before halving? That would just add zeros then remove them, which doesn't change the odd part. So it's equivalent. So the condition is: for each a_i, there must be some b_j such that odd(a_i) can be obtained by halving b_j. And each b_j can be used only once.

                // Now, how to check this efficiently? We can process the numbers by their odd parts. For each b_j, we can generate all numbers that can be obtained by halving it (i.e., its prefixes). But there are many prefixes (up to log2(10^9) ~ 30). So we can insert all prefixes of each b_j into a multiset or frequency map. Then for each a_i, we need to find if its odd part is in that set. But wait: a_i itself might have trailing zeros. We need to match a_i exactly, not just its odd part. Because we need to transform b_j into a_i, not just into odd(a_i). But if we can transform b_j into odd(a_i), then we can double it to get a_i? Yes, because a_i = odd(a_i) * 2^p. So if we can reach odd(a_i) from b_j, we can then double it p times to get a_i. So it's sufficient to match odd(a_i) with some b_j's halving sequence. But we must also ensure that the number of doublings we do doesn't conflict with anything? No, we can always double as many times as we want. So the condition is exactly: for each a_i, there exists a b_j such that odd(a_i) is in the halving sequence of b_j. And each b_j can be used for at most one a_i? But wait, if we use a b_j to match an a_i, we transform b_j into a_i. That consumes b_j. So we need a matching between a_i and b_j. So we need to assign each a_i to a distinct b_j such that odd(a_i) is a prefix of b_j.

                // But is that condition sufficient? Let's test with sample 2: a = {1,4,17}, b = {4,5,31}. 
                // a odd parts: 1, 1 (since 4=1*4), 17. b halving sequences:
                // 4: 4,2,1
                // 5: 5,2,1
                // 31: 31,15,7,3,1
                // We need to match odd(a_i): 1, 1, 17. We have two 1's from a. We can match them with b's prefixes: 4 gives 1, 5 gives 1. But 17 is not in any halving sequence of b. 31's sequence: 31,15,7,3,1. No 17. So NO. Correct.

                // Sample 3: a = {4,7,10,13,14}, b = {2,14,14,26,42}
                // a odd parts: 1 (4), 7, 5 (10), 13, 7 (14). So odd parts: 1,7,5,13,7.
                // b halving sequences:
                // 2: 2,1
                // 14: 14,7,3,1
                // 14: same
                // 26: 26,13,6,3,1
                // 42: 42,21,10,5,2,1
                // We need to match: 1,7,5,13,7.
                // 1 can be matched with 2 (gives 1) or 14 (gives 1) etc.
                // 7 can be matched with 14 (gives 7).
                // 5 can be matched with 42 (gives 5).
                // 13 can be matched with 26 (gives 13).
                // 7 can be matched with the other 14.
                // So YES.

                // Sample 4: a = {2,2,4,4,4}, b = {28,46,62,71,98}
                // a odd parts: 1,1,1,1,1 (since 2=1*2, 4=1*4). So all odd parts are 1.
                // b halving sequences all eventually reach 1. So we just need to match five 1's. We have five b's, each can reach 1. So YES.

                // Sample 5: a = {1,2,10,16,64,80}, b = {20,43,60,74,85,99}
                // a odd parts: 1, 1 (2), 5 (10), 1 (16), 1 (64), 5 (80). So odd parts: 1,1,5,1,1,5.
                // b halving sequences:
                // 20: 20,10,5,2,1 -> has 5,1
                // 43: 43,21,10,5,2,1 -> has 5,1
                // 60: 60,30,15,7,3,1 -> has 1
                // 74: 74,37,18,9,4,2,1 -> has 1
                // 85: 85,42,21,10,5,2,1 -> has 5,1
                // 99: 99,49,24,12,6,3,1 -> has 1
                // We need two 5's and four 1's. We have three b's that can give 5 (20,43,85) and all can give 1. So we can match. YES.

                // So the condition seems correct: a_i can be matched with b_j iff odd(a_i) appears in the halving sequence of b_j. And we need a perfect matching.

                // Now, how to find a matching? We can use a greedy approach: process a_i in decreasing order of something? Or we can use a multiset of available "odd parts" from b. For each b_j, we can generate all numbers in its halving sequence. But we can only use each b_j once. So if we match an a_i with a b_j, we consume that b_j. So we need to decide which b_j to use for which a_i. Since multiple a_i might have the same odd part, we need to assign them to different b_j that can provide that odd part.

                // One common solution: Use a max-heap or sort a and b. For each a_i (from largest to smallest), try to find a b_j that can be transformed into a_i. But we can also think in terms of the odd parts. Since the operation on b is halving or doubling, we can simulate the process by trying to reduce b_j to match a_i. A known solution: put all b into a multiset. For each a_i in decreasing order, if a_i is in b, remove it. Else, if a_i is even, we can halve it and try again? Wait, we can only change b, not a. But we can think of it as: we want to match a_i with some b_j. If a_i is larger than all b, we might need to double some b. But doubling b increases it. So if a_i is very large, we might need to double a smaller b. But we can also halve a_i? No, we cannot change a. But we can think of the reverse: starting from a_i, we can apply the inverse operations to see which b_j could have become a_i. The inverse operations: if a_i is even, it could have come from a_i/2 by doubling; or it could have come from 2*a_i or 2*a_i+1 by halving. So from a_i, we can go to a_i/2 (if even), or to 2*a_i and 2*a_i+1. This is a tree. But we want to find if there is some b_j in this tree. Since b is given, we can instead try to reduce a_i by halving (if even) until we find a match in b? But wait, if a_i is odd, we cannot halve it (we can, but floor(odd/2) is not the inverse of doubling). Actually, if a_i is odd, it could not have been produced by doubling something (since doubling always yields even). So an odd a_i must have been produced by halving some number. That means some b_j was halved to get a_i. So b_j must be either 2*a_i or 2*a_i+1. So if a_i is odd, the only way to get it is to halve a number that is either 2*a_i or 2*a_i+1. So we can look for those in b. If not found, we can't match? But we could also halve a larger number multiple times. For example, a_i=5 (odd). It could come from halving 10 or 11. If we don't have 10 or 11, we could halve 20,21,22,23? Halving 20 gives 10, then halving 10 gives 5. So 5 can come from 20,21,22,23? 20->10->5; 21->10->5; 22->11->5; 23->11->5. So indeed, any number in the range [5*2^k, 5*2^k + 2^k - 1] can be halved k times to get 5. So the set of b that can produce a_i is all numbers whose binary representation starts with the binary of a_i. That is, a_i is a prefix of b. So if we write a_i in binary, then any b that has a_i as a prefix (i.e., b >> k == a_i for some k) can be transformed into a_i by halving k times. And if we need to double first? That would just add zeros to b, then halve. But that's equivalent to b having a_i as a prefix after removing trailing zeros? Actually, if we double b first, we add zeros, then halve, we might get a_i if a_i is a prefix of the doubled b. But doubling b just shifts it left. So if we double b then halve, we are essentially looking at prefixes of b shifted left. That is the same as saying a_i is a prefix of b shifted left? But we can always double b as many times as we want. So if we can double b, we can make it larger, and then halve it to get a_i. That means a_i must be a prefix of b * 2^p for some p. But b * 2^p is just b shifted left. Its prefixes are either prefixes of b (if we halve more than p times) or b followed by some zeros (if we halve less than p times). So a_i could be b followed by some zeros? That means a_i = b * 2^q. So if a_i is b times a power of 2, then we can double b to get a_i directly. So the condition is: a_i can be obtained from b either by halving b some number of times (so a_i is a prefix of b), or by doubling b some number of times (so b is a prefix of a_i). In other words, either a_i is a prefix of b, or b is a prefix of a_i. But wait, what about the case where we
