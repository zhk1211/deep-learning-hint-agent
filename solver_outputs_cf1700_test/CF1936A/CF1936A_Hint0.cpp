// Hint0
#include <bits/stdc++.h>
using namespace std;

int ask(int a, int b, int c, int d) {
    cout << "? " << a << " " << b << " " << c << " " << d << endl;
    char res;
    cin >> res;
    if (res == '<') return -1;
    if (res == '=') return 0;
    return 1;
}

void solve() {
    int n;
    cin >> n;
    
    // Find index of maximum element (n-1)
    int max_idx = 0;
    for (int i = 1; i < n; i++) {
        int cmp = ask(max_idx, max_idx, i, i);
        if (cmp < 0) {
            max_idx = i;
        }
    }
    
    // Now max_idx points to the element n-1
    // Find the element that maximizes (n-1) XOR p[j]
    // This is equivalent to finding the element with maximum OR with something?
    // Actually we want to maximize XOR with n-1, which is the bitwise complement of n-1 within the range.
    // But we can only compare ORs.
    // We know max_idx has value n-1.
    // We want to find j such that p[j] has as many bits different from n-1 as possible.
    // Since n-1 has all bits 1 up to its highest bit, the best complement is the one with highest possible bits set where n-1 has 0.
    // But n-1 is all 1s in its bit-length, so its complement is 0. That's not helpful.
    // Actually we want to maximize XOR with n-1. The maximum XOR with n-1 is achieved by the element that is the bitwise NOT of n-1 within the bit-length.
    // But we don't know the bit-length. However, we can find the element that gives the maximum OR with max_idx?
    // Let's think: For any a, b, (a|b) >= max(a,b). The maximum possible OR with n-1 is when the other number has bits set where n-1 doesn't.
    // But n-1 has all bits set up to its highest bit, so the only bits not set are those above the highest bit. But numbers are < n, so no bits above.
    // So n-1 OR anything is n-1. That doesn't help.
    // Wait, we need to maximize XOR, not OR. We can't directly query XOR.
    // We need to find the element that is the "complement" of n-1 in terms of bits.
    // Let's find the element with the smallest OR with something? No.
    // Alternative approach: Find the maximum element, then find the element that maximizes XOR with it.
    // How to compare XORs using OR queries?
    // Note that (a|b) = a + b - (a&b). Not directly helpful.
    // We can find the maximum element (n-1). Then we want to find j that maximizes (n-1) ^ p[j].
    // Consider the set of candidates that could be the best partner for max_idx.
    // The best partner is the one that has the highest bit where it differs from n-1.
    // Since n-1 has all bits 1 up to its highest bit, the best partner is the one with the highest bit set that is 0 in n-1? But there is no such bit.
    // Actually, n-1 in binary is something like 111...1 (k bits). The maximum XOR with n-1 is achieved by the number that is 0 in all bits where n-1 is 1, and 1 in bits where n-1 is 0. But since numbers are < n, the highest bit of n-1 is the maximum bit possible. So the complement would be 0, but 0 XOR n-1 = n-1. But maybe there is a number that has a higher bit? No, because n-1 is the maximum.
    // Wait, the maximum XOR of two numbers in [0, n-1] is not necessarily n-1. For example, n=4: numbers 0,1,2,3. Max XOR is 3 (3^0=3). But 3^1=2, 3^2=1. So 0 is the best partner for 3.
    // In general, the maximum XOR is achieved by a pair where one is the maximum possible and the other is its bitwise complement within the bit-length. The complement of n-1 (which is all ones) is 0. So the best partner for n-1 is 0.
    // But is 0 always present? Yes, permutation of 0..n-1 includes 0.
    // So if we find the index of 0, then (max_idx, idx0) gives XOR = n-1, which is maximum possible? Not always. Example n=5: numbers 0..4. Max XOR is 7? Wait, 4^3=7, but 4 is 100, 3 is 011, XOR=111=7. n-1=4. 4^0=4, not maximum. So 0 is not always the best partner.
    // So we need a general method.
    
    // Let's find the maximum element (n-1). Then we need to find the element that maximizes XOR with it.
    // We can find the element that has the highest bit set where n-1 has 0? But n-1 has all bits set up to its highest bit. The only bits not set are those above the highest bit, but no number has those bits set because they are < n.
    // Wait, n-1 might not be a power of 2 minus 1. For n=5, n-1=4 (100). It has bits 0 and 1 as 0. So we want a number that has bits 0 and 1 set, i.e., 3 (011). So the best partner is the one that has 1s in the positions where n-1 has 0s, and 0s where n-1 has 1s. That is the bitwise complement of n-1 within the bit-length of n-1.
    // So we need to find the element that is the bitwise complement of n-1 (within the highest bit of n-1). Let's call this target = ((1 << (bit_length(n-1))) - 1) ^ (n-1).
    // How to find this target using OR comparisons?
    // We can find the maximum element (n-1). Then we can find the element that gives the maximum OR with something? Not sure.
    
    // Alternative strategy from known Codeforces problem: 
    // 1. Find the index of the maximum element (n-1) by comparing (i,i) vs (j,j) -> OR is just the element itself.
    // 2. Then find the index of the element that maximizes (n-1) | p[j]. Since n-1 is the maximum, (n-1) | p[j] = n-1 for all j? No, if n-1 has some 0 bits, then p[j] could have 1s there, making the OR larger than n-1. But wait, n-1 is the maximum number in the set, so no number can be larger than n-1. Therefore, (n-1) | p[j] cannot exceed n-1? Actually, if n-1 is not all 1s, then there could be a number with a bit set that n-1 doesn't have, but that number would be larger than n-1, which is impossible because n-1 is the maximum. So n-1 must have all bits set that any number in the set can have. That means n-1 is of the form 2^k - 1? Not necessarily. For n=5, max is 4 (100). The numbers are 0,1,2,3,4. 3 is 011, which has bits 0 and 1 set, but 4 doesn't have them. 3 is not larger than 4. So n-1 can have 0 bits, and other numbers can have 1s in those bits without exceeding n-1. So (n-1) | p[j] can be larger than n-1? No, bitwise OR of 4 (100) and 3 (011) is 7 (111), which is larger than 4. But 7 is not in the set. The OR is just a value we compute, it doesn't have to be in the set. So (n-1) | p[j] can be > n-1. So we can compare ORs.
    
    // Let's think: We want to maximize (n-1) ^ p[j]. Note that (n-1) ^ p[j] = (n-1) | p[j] - (n-1) & p[j]. Not directly comparable.
    // But we can find the element that gives the maximum (n-1) | p[j]. Is that the one that maximizes XOR? Not necessarily.
    // Let's test: n=5, max=4. Candidates:
    // j=0: p=0 -> 4|0=4, XOR=4
    // j=1: p=1 -> 4|1=5, XOR=5
    // j=2: p=2 -> 4|2=6, XOR=6
    // j=3: p=3 -> 4|3=7, XOR=7
    // j=4: p=4 -> 4|4=4, XOR=0
    // The maximum XOR is 7 with j=3. The maximum OR is also 7 with j=3. So in this case, max OR corresponds to max XOR.
    // Another example: n=6, max=5 (101). Numbers: 0,1,2,3,4,5.
    // 5|0=5, XOR=5
    // 5|1=5, XOR=4
    // 5|2=7, XOR=7
    // 5|3=7, XOR=6
    // 5|4=5, XOR=1
    // 5|5=5, XOR=0
    // Max XOR is 7 (with 2). Max OR is 7 (with 2 or 3). So max OR gives a candidate that might not be unique, but one of them gives max XOR.
    // Is it always true that the element achieving max (n-1)|p[j] also achieves max (n-1)^p[j]? Let's check: (n-1)^p = (n-1)|p - (n-1)&p. Since (n-1)&p >= 0, we have (n-1)^p <= (n-1)|p. So max XOR <= max OR. If there is a unique max OR, does it necessarily give max XOR? Not necessarily, because another element could have slightly smaller OR but much smaller AND, resulting in larger XOR.
    // Example: n=8, max=7 (111). 7|anything = 7. So all ORs are 7. Max XOR is 7 (with 0). So max OR doesn't help.
    // So we need a different approach.
    
    // Let's recall a known solution for this problem (Codeforces 1451E2? Actually this is Codeforces 1451E2 "Bitwise Queries" but with OR instead of AND/OR/XOR? Wait, the problem statement says we can only compare ORs. This is Codeforces Round #685 (Div. 2) Problem E2? No, that one had AND, OR, XOR queries. This one only has OR comparison.
    // Actually this is Codeforces problem 1451E2? No, that's "Bitwise Queries (Hard Version)" with AND, OR, XOR. This is different.
    // This is Codeforces problem 1556D? No.
    // Let's search memory: There is a problem "XOR Guessing" or something. Actually this is Codeforces 1451E2? No.
    // Wait, the problem statement says "This is an interactive problem. There is a secret sequence p... You need to find any two indices i and j such that p_i XOR p_j is maximized... you can ask queries of the form ? a b c d, and you get comparison of (p_a|p_b) and (p_c|p_d)." This is Codeforces problem 1556D? No, 1556D is "Take a Guess" with AND and OR.
    // This is Codeforces problem 1451E2? No.
    // Actually this is Codeforces problem 1451E1? No.
    // I think this is Codeforces problem 1556D? No, 1556D is "Take a Guess" where you can ask AND and OR, and you need to find the array.
    // This is Codeforces problem 1451E2 "Bitwise Queries" but with only OR? No.
    // Let's think: We can only compare ORs. We need to find max XOR pair with 3n queries.
    // We can find the maximum element by comparing (i,i) vs (j,j). That takes n-1 queries.
    // Then we have max_idx with value n-1.
    // Now we want to find j that maximizes (n-1) ^ p[j].
    // How to find the element that is the bitwise complement of n-1?
    // We can find the element that has the highest bit set where n-1 has 0. But we don't know bits.
    // We can find the element that gives the maximum OR with max_idx? As we saw, that might not work when n-1 is all 1s.
    // But if n-1 is all 1s, then n is a power of 2. In that case, the maximum XOR is n-1, achieved by pairing n-1 with 0. So we just need to find 0.
    // How to find 0? We can find the minimum element. The minimum element is 0. We can find the minimum by comparing (i,i) vs (j,j) and taking the smaller. That takes n-1 queries. Then we have min_idx. Then answer is (max_idx, min_idx). But wait, is the maximum XOR always achieved by max and min? Not always, as we saw with n=5, max=4, min=0, XOR=4, but max XOR is 7 (4^3). So min is not always the best partner.
    // So we need a general method.
    
    // Let's think about the properties of OR. We can compare (a|b) and (c|d). We can use this to find the maximum element, and also to find the element that has a particular bit set?
    // We can find the maximum element. Then we can find the element that maximizes (max_idx | j). Let's call this set S of indices that achieve the maximum OR with max_idx.
    // Then among S, we want the one that minimizes (max_idx & j)? Because XOR = OR - AND. Since OR is maximized, to maximize XOR we need to minimize AND.
    // How to compare ANDs? We can't directly. But we can compare (max_idx | j) and (max_idx | k) to see if they are equal. If they are equal, then we need to compare (max_idx & j) and (max_idx & k). How to compare ANDs using OR queries?
    // Note that (a & b) = a + b - (a | b). But we don't know the values.
    // We can compare (max_idx | j) and (max_idx | k). If they are equal, then the one with smaller (max_idx & j) will have larger XOR. How to compare (max_idx & j) and (max_idx & k)?
    // Consider comparing (j, k) vs (max_idx, max_idx)? Not sure.
    // Let's think: We want to compare (max_idx & j) and (max_idx & k). Note that (max_idx & j) is the set of bits where both max_idx and j have 1. 
    // We can compare (max_idx | j) and (max_idx | k). If they are equal, then j and k have the same bits set outside of max_idx's bits? Actually, (max_idx | j) = max_idx + (j & ~max_idx). So if (max_idx | j) == (max_idx | k), then j and k have the same bits set where max_idx has 0. So they differ only in bits where max_idx has 1. In those bits, one might have 1 and the other 0. The one with 0 in those bits will have smaller AND with max_idx, thus larger XOR.
    // So we want to find among those with the same (max_idx | j), the one that has the fewest bits in common with max_idx. That is equivalent to minimizing (max_idx & j).
    // How to compare (max_idx & j) and (max_idx & k)? We can compare (j | k) with something? 
    // Note that (max_idx & j) > (max_idx & k) means that j has more 1s in the positions where max_idx has 1. 
    // Consider comparing (j | max_idx) and (k | max_idx) - they are equal by assumption.
    // Consider comparing (j | j) and (k | k) - that's just comparing j and k. But we don't know which is smaller.
    // Actually, if (max_idx | j) == (max_idx | k), then j and k differ only in bits where max_idx has 1. In those bits, max_idx has 1. So if j has 1 and k has 0 in such a bit, then j > k? Not necessarily, because max_idx might have higher bits. But since max_idx is the maximum, any bit where max_idx has 1 is higher than any bit where max_idx has 0? Not necessarily, max_idx could be 5 (101), and j=4 (100), k=1 (001). Then max_idx|j = 101|100=101=5, max_idx|k=101|001=101=5. They are equal. j=4, k=1. j > k. The ANDs: max_idx&j = 101&100=100=4, max_idx&k=101&001=001=1. So larger j gives larger AND. So to minimize AND, we want the smaller j among those with equal OR with max_idx.
    // Is it always true that if (max_idx | j) == (max_idx | k), then j < k implies (max_idx & j) < (max_idx & k)? Let's check: j < k. Since they have the same bits where max_idx has 0, the difference is in bits where max_idx has 1. In those bits, max_idx has 1. If j < k, then j must have a 0 in a higher bit where k has 1, or something. But since max_idx has 1 in all those bits, the comparison of j and k is determined by the highest bit where they differ, which is a bit where max_idx has 1. If j < k, then at that bit, j has 0 and k has 1. Then (max_idx & j) has 0 at that bit, (max_idx & k) has 1. So (max_idx & j) < (max_idx & k). So indeed, smaller j gives smaller AND, hence larger XOR.
    // Therefore, among all j that achieve the maximum (max_idx | j), the one with the smallest value p[j] will give the maximum XOR with max_idx.
    // So we need to find the index that minimizes p[j] among those with maximum (max_idx | j).
    // How to find the maximum (max_idx | j)? We can find it by comparing (max_idx | j) with (max_idx | k). We can do a tournament to find the j that maximizes (max_idx | j). But we can only compare two pairs at a time. We can compare (max_idx | a) and (max_idx | b) by querying ? max_idx a max_idx b. This compares (p_max_idx | p_a) vs (p_max_idx | p_b). So we can find the index that maximizes this OR.
    // Let's do: start with candidate = 0 (or any). For i from 1 to n-1, compare (max_idx | candidate) and (max_idx | i). If the latter is greater, update candidate = i. If equal, we need to keep the one with smaller p[i]? But we don't know p[i] directly. However, if they are equal, we want the smaller p[i]. How to compare p[candidate] and p[i]? We can just compare (candidate, candidate) vs (i, i) because OR of an element with itself is the element. So if (max_idx | candidate) == (max_idx | i), we can then compare candidate vs i directly to see which is smaller, and keep the smaller one. This works because if the ORs are equal, the one with smaller value gives larger XOR.
    // But wait: Is it always true that the maximum XOR with max_idx is achieved by some j that maximizes (max_idx | j)? Let's prove or disprove.
    // Suppose we have max_idx with value M = n-1. We want to maximize M ^ p[j]. Let O_j = M | p[j]. Let A_j = M & p[j]. Then M ^ p[j] = O_j - A_j.
    // Suppose there is some j that does not maximize O_j, but has a much smaller A_j, resulting in a larger XOR than any j that maximizes O_j.
    // Let j* maximize O_j. Let O* = O_j*. For any other j, O_j <= O*. 
    // We want to see if it's possible that M ^ p[j] > M ^ p[j*] for some j with O_j < O*.
    // M ^ p[j] = O_j - A_j <= O_j <= O* - 1 (if O_j < O*). 
    // M ^ p[j*] = O* - A_j* >= O* - M (since A_j* <= M). 
    // So we need O* - 1 >= O* - M? That would require M <= 1, which is not generally true.
    // Actually, we need O_j - A_j > O* - A_j*. Since O_j <= O* - 1, we have O_j - A_j <= O* - 1 - A_j <= O* - 1. So the maximum possible XOR for a non-maximizer is O* - 1. The maximizer gives at least O* - M. So if O* - M <= O* - 1, i.e., M >= 1, then the maximizer could be worse if M is large? Wait, O* - M could be much smaller than O* - 1. For example, M=7 (111), O*=7, A_j* could be 7, giving XOR=0. A non-maximizer with O_j=6? But O_j cannot be 6 because M|p[j] with M=7 is always 7. So all O_j = 7. So no non-maximizer.
    // Consider M=5 (101). O* = max_j (5|p[j]). p[j] can be 0..5. 5|0=5, 5|1=5, 5|2=7, 5|3=7, 5|4=5, 5|5=5. So O* = 7. Maximizers are j with p[j]=2 or 3. For j=2 (p=2, 010): A=5&2=0, XOR=7. For j=3 (p=3, 011): A=5&3=1, XOR=6. So the best is j=2 with XOR=7. Now consider a non-maximizer, say j=1 (p=1, 001): O=5, A=5&1=1, XOR=4. j=4 (p=4, 100): O=5, A=5&4=4, XOR=1. So maximizer gives 7, non-maximizer gives at most 4. So maximizer wins.
    // Is there a case where a non-maximizer gives larger XOR than all maximizers? Let's try to construct. We need O_j < O*, but O_j - A_j > O* - A_j* for all j* with O_j* = O*.
    // Since O_j <= O* - 1, we need O* - 1 - A_j > O* - A_j* => A_j* - A_j > 1 => A_j* >= A_j + 2.
    // So we need a maximizer with very large A_j*, and a non-maximizer with very small A_j.
    // Let M be such that there is a bit where M has 0, and some numbers have 1 there, making O* > M. 
    // Let M = 1000 (8). n=9? Actually n up to 9, M=8. Numbers 0..8. M=8 (1000). O* = max(8|p). p=8: 8|8=8. p=7 (0111): 8|7=15. So O*=15. Maximizers: p=7 gives O=15, A=8&7=0, XOR=15. p=6 (0110): 8|6=14? Wait, 8|6=14 (1110). O=14, not 15. So only p=7 gives 15? Check p=7: 8|7=15. p= anything with bit 3 set? Only 8 has bit 3 set. Others have bits 0-2. So max OR is 15, achieved only by p=7. So maximizer is unique and gives XOR=15. Non-maximizers: p=6 gives O=14, A=8&6=0? 8&6=0, XOR=14. p=5: 8|5=13, A=0, XOR=13. So maximizer wins.
    // What if M has multiple 0 bits? M=9 (1001). n=10? M=9. Numbers 0..9. M=9 (1001). O* = max(9|p). p=6 (0110): 9|6=15 (1111). p=7 (0111): 9|7=15. p=4 (0100): 9|4=13. So O*=15. Maximizers: p=6,7. For p=6: A=9&6=0, XOR=15. p=7: A=9&7=1, XOR=14. So best is p=6 with XOR=15. Non-maximizer: p=5 (0101): 9|5=13, A=9&5=1, XOR=12. p=2 (0010): 9|2=11, A=0, XOR=11. So maximizer wins.
    // It seems the maximum XOR is always achieved by some element that maximizes the OR with the maximum element. I think this is a known fact: For any set of numbers, the maximum XOR pair can be found by taking the maximum element and then the element that maximizes the OR with it? Not always, but in a permutation of 0..n-1, the maximum element is n-1. The maximum XOR with n-1 is achieved by the number that is the bitwise complement of n-1 within the highest bit. That number will have 1s in all positions where n-1 has 0s, and 0s where n-1 has 1s. This number will maximize the OR with n-1 because it sets all bits that n-1 doesn't have, making the OR as large as possible (all bits set up to the highest bit of n-1, plus maybe higher? No, because n-1 has the highest bit). So the complement will achieve the maximum possible OR, which is (1 << (bit_length(n-1))) - 1. So indeed, the element that maximizes OR with n-1 is exactly the complement (or one of them if there are multiple with the same OR). And among those, the one with the smallest AND (i.e., smallest value) gives the maximum XOR. So our strategy is correct.
    
    // Thus algorithm:
    // 1. Find index of maximum element (n-1). Use n-1 queries comparing (i,i) vs (max_idx, max_idx).
    // 2. Find the index that maximizes (max_idx | j). We can do this by iterating over all j != max_idx, keeping the best candidate.
    //    For each j, compare (max_idx | best) vs (max_idx | j). If the latter is greater, update best = j.
    //    If equal, compare best vs j directly (by (best, best) vs (j, j)) and keep the smaller one.
    //    This takes at most 2 queries per element (one OR comparison, possibly one value comparison). Total queries: (n-1) + 2*(n-1) = 3n - 3, which is within 3n.
    //    Actually, we can optimize: We don't need to compare value if OR is strictly greater. So at most 2 per element, but often 1.
    // 3. Answer (max_idx, best).
    
    // Let's test with n=4, p=[0,3,1,2]. max_idx: compare 0,0 vs 1,1: 0<3 -> max=1. compare 1,1 vs 2,2: 3>1 -> max=1. compare 1,1 vs 3,3: 3>2 -> max=1. So max_idx=1 (value 3).
    // Now find best partner for max_idx=1. Initialize best = 0 (value 0). Compare (1|0) vs (1|2): (3|0)=3 vs (3|1)=3? Wait, p[2]=1, so 3|1=3. Equal. Then compare 0 vs 2: p[0]=0, p[2]=1 -> 0<1, so keep best=0 (smaller). Next j=3: compare (1|0) vs (1|3): 3|0=3 vs 3|2=3. Equal. Compare 0 vs 3: 0<2 -> keep 0. So best=0. Answer (1,0) gives XOR=3^0=3. Correct.
    // Another test: n=5, p=[0,1,2,3,4]? Actually permutation, say p=[4,0,1,2,3]? Let's use p=[0,1,2,3,4] for simplicity. max_idx=4 (value 4). best=0. Compare (4|0)=4 vs (4|1)=5 -> 5>4, best=1. Compare (4|1)=5 vs (4|2)=6 -> 6>5, best=2. Compare (4|2)=6 vs (4|3)=7 -> 7>6, best=3. Compare (4|3)=7 vs (4|4)=4 -> 7>4, best=3. Answer (4,3) -> 4^3=7. Correct.
    // Test: n=6, p=[5,0,1,2,3,4]? max_idx=0 (value 5). best=1 (0). Compare (5|0)=5 vs (5|1)=5 -> equal, compare 0 vs 1: 0<1 -> keep 1. Compare (5|1)=5 vs (5|2)=7 -> 7>5, best=2. Compare (5|2)=7 vs (5|3)=7 -> equal, compare 2 vs 3: 2<3 -> keep 2. Compare (5|2)=7 vs (5|4)=5 -> 7>5, keep 2. Compare (5|2)=7 vs (5|5)=5 -> keep 2. Answer (0,2) -> 5^2=7. Correct.
    // Test: n=8, p=[7,0,1,2,3,4,5,6]? max_idx=0 (7). best=1 (0). Compare (7|0)=7 vs (7|1)=7 -> equal, compare 0 vs 1: 0<1 -> keep 1. All ORs are 7, so we keep the smallest value among them. The smallest value is 0 (index 1). So best=1. Answer (0,1) -> 7^0=7. Correct.
    // Test: n=2, p=[0,1]. max_idx=1 (value 1). best=0. Compare (1|0)=1 vs (1|1)=1 -> equal, compare 0 vs 1: 0<1 -> keep 0. Answer (1,0) -> 1^0=1. Correct.
    
    // So this seems solid.
    
    // Implementation details:
    // We need to handle multiple test cases, sum of n <= 10000.
    // Queries: print "? a b c d", read char.
    // After finding answer, print "! i j".
    
    // Let's code.
    
    int max_idx = 0;
    for (int i = 1; i < n; i++) {
        int cmp = ask(max_idx, max_idx, i, i);
        if (cmp < 0) { // max_idx < i
            max_idx = i;
        }
    }
    
    int best = (max_idx == 0) ? 1 : 0; // any index different from max_idx
    for (int i = 0; i < n; i++) {
        if (i == max_idx) continue;
        int cmp = ask(max_idx, best, max_idx, i);
        if (cmp < 0) { // (max|best) < (max|i)
            best = i;
        } else if (cmp == 0) {
            // equal OR, keep the one with smaller value
            int cmp2 = ask(best, best, i, i);
            if (cmp2 > 0) { // best > i, so i is smaller
                best = i;
            }
        }
    }
    
    cout << "! " << max_idx << " " << best << endl;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}
