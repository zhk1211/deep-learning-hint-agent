#include <bits/stdc++.h>
using namespace std;

int query(vector<int> &v) {
    cout << "? " << v.size();
    for (int x : v) cout << " " << x;
    cout << endl;
    int res;
    cin >> res;
    return res;
}

void solve() {
    int n;
    cin >> n;
    int m = 2 * n;
    vector<int> a(m + 1, 0);
    vector<int> pos1(n + 1, 0), pos2(n + 1, 0);
    
    // Find pairs using queries of size 2
    for (int i = 1; i <= m; i += 2) {
        if (i + 1 > m) break;
        vector<int> q = {i, i + 1};
        int r = query(q);
        if (r > 0) {
            // both are r
            a[i] = a[i + 1] = r;
            if (pos1[r] == 0) pos1[r] = i;
            else pos2[r] = i;
        }
    }
    
    // For numbers not yet placed, find their positions
    vector<int> unplaced;
    for (int x = 1; x <= n; x++) {
        if (pos1[x] == 0) unplaced.push_back(x);
    }
    
    // Find positions of unplaced numbers
    vector<int> unknown_pos;
    for (int i = 1; i <= m; i++) {
        if (a[i] == 0) unknown_pos.push_back(i);
    }
    
    // For each unplaced number, we need to find its two positions
    // We can query triples: two known zeros and one known non-zero to identify
    // Actually, we can query pairs among unknown positions to find duplicates
    // But we have limited queries. Use a different approach:
    // For each unplaced number, query a set containing one known position of a placed number
    // and two unknown positions. If MAD equals the placed number, then the two unknowns are that number?
    // Better: query all unknown positions together with one known position of each placed number.
    
    // Let's use the fact that we can query all unknown positions with one known position.
    // If we query unknown positions + a known position of value v, MAD will be v if both copies of v are present.
    // But we need to find which unknown positions have which values.
    
    // Alternative: For each unplaced number x, we can find its positions by querying pairs of unknown positions.
    // Since sum n^2 <= 1e5, we can afford O(n^2) queries? No, 3n limit.
    
    // Better: Use queries of size 3: two unknown positions and one known position of a placed number.
    // If the two unknown positions contain the same number x, then MAD of {unknown1, unknown2, known_v} will be max(x, v) if x appears twice, else v if v appears twice, else 0.
    // But we don't know x.
    
    // Actually, we can find all pairs among unknown positions by querying all unknown positions together with one known position of each placed number.
    // Let U be the set of unknown positions. For each placed number v, query U ∪ {pos1[v]}. MAD will be v if both copies of v are in U? No, v is already placed, so its other copy might be in U.
    // Wait, each number appears exactly twice. If v is placed, we already found both its positions? Not necessarily: we only found pairs that are adjacent in the initial pairing. Some numbers might have their two positions not adjacent.
    
    // Re-evaluate: The initial pairing i, i+1 only works if the two copies are adjacent. If they are not, the query returns 0.
    // So after the first pass, we have placed some numbers (those with adjacent copies). The remaining numbers have their copies separated.
    
    // For the remaining numbers, we can find them by querying triples: two unknown positions and one known position of a placed number.
    // Let unknown positions be U. For each x in unplaced, we need to find its two positions.
    // We can do this: For each unknown position i, query {i, pos1[v]} for each placed v. If MAD == v, then a[i] = v? No, MAD of {i, pos1[v]} is v only if a[i] == v, because then v appears twice. If a[i] != v, MAD is 0.
    // So we can identify each unknown position by querying it with each placed number's known position. That would take |U| * |placed| queries, which could be O(n^2) worst case.
    
    // But we have 3n queries. |U| <= 2n, |placed| <= n. Worst case 2n^2 queries.
    // However, sum n^2 <= 1e5, so total queries across test cases might be acceptable? But per test case limit is 3n.
    // We need a better approach.
    
    // Hint: "If we send two queries, each with k=2 and different indices, then the answer is the maximum of the two."
    // This suggests we can combine information from multiple queries.
    
    // Let's think differently: We can query all positions together? MAD of all 2n positions is n (since each number appears twice, the maximum duplicate is n). Not helpful.
    
    // Another idea: Use binary search or divide and conquer.
    // We can find the two positions of a number x by querying sets.
    // For a set S, MAD(S) tells us the maximum number that appears at least twice in S.
    // If we want to find positions of x, we can query S and S ∪ {some position}. If MAD changes, we can deduce something.
    
    // Let's use the following: For each number x from 1 to n, we can find its two positions by querying all positions except one. But that's too many queries.
    
    // Observe: We can find all positions of all numbers using 2n queries of size 2? No, we already tried adjacent pairs.
    
    // Let's use queries of size n+1 or similar.
    // Consider querying the first n+1 positions. MAD will be some number. That number appears twice in the first n+1 positions. We can then find its positions.
    
    // Actually, we can find all pairs by a divide and conquer approach:
    // Query the whole array: MAD = n (since all numbers appear twice). Not helpful.
    // Query half the array: if MAD is some number, that number appears twice in that half.
    // We can recursively find pairs.
    
    // But we have only 3n queries. A divide and conquer might use O(n log n) queries, which for n=300 is about 300*9=2700, but 3n=900. So O(n log n) might be too much? Actually 300*9=2700 > 900. So we need O(n) queries.
    
    // Let's think of a linear algorithm.
    
    // We can find all numbers by querying triples: {1, i, i+1} for i=2 to 2n-1.
    // MAD of {1, i, i+1}: if a[1] appears again among i,i+1, MAD = a[1]. Otherwise, if a[i]=a[i+1], MAD = a[i]. Otherwise 0.
    // This gives us information about adjacent pairs and the first element.
    
    // But we can do better: We can find the two positions of each number by using the fact that if we query a set that contains exactly one copy of x and two copies of y, MAD = y if y > x, else x if x appears twice? No, x appears only once.
    
    // Let's use the following strategy:
    // 1. Find one pair of adjacent duplicates by querying all adjacent pairs (n queries). Actually, we can query all adjacent pairs: (1,2), (3,4), ..., (2n-1,2n). This takes n queries. For each pair, if MAD > 0, we know both positions have that number.
    // 2. After this, we have some numbers placed. The remaining numbers have their two copies in different "blocks".
    // 3. Now, we have some unknown positions. We can query each unknown position with a known position of each placed number. But that's too many queries.
    
    // Alternative: For each placed number v, we can query all unknown positions together with one known position of v. MAD will be v if the other copy of v is among the unknown positions. This tells us which numbers have one copy in the unknown set. But we still need to pair them.
    
    // Actually, if we know that a number v has one copy in the unknown set, we can find its exact position by querying subsets of the unknown set with the known position of v. We can do binary search on the unknown positions to find the position of v. That would take log(|U|) queries per number. Total queries: n (initial pairs) + |placed| * log(2n) ≈ n + n * 9 = 10n, too many.
    
    // But we have only 3n. We need a more efficient way.
    
    // Let's look at the hint again: "If we send two queries, each with k=2 and different indices, then the answer is the maximum of the two."
    // This suggests that we can combine two queries of size 2 to get information about four indices.
    // For example, query {1,2} and {3,4}. If both return 0, then no adjacent duplicates. If one returns x, then that pair is (x,x). If both return the same x, then both pairs are (x,x)? But each number appears exactly twice, so if both pairs return x, then all four are x, which is impossible since x appears only twice. So they must return different numbers or 0.
    
    // Maybe we can use queries of size 3 or 4 to get more information per query.
    
    // Consider querying a set of size 4: {i, j, k, l}. MAD tells us the largest number that appears at least twice in that set. If we choose the set carefully, we can deduce the values.
    
    // Another idea: We can find the positions of all numbers by querying all possible triples? No.
    
    // Let's think about the structure: The sequence is a permutation of two copies of 1..n.
    // We can find the positions of each number by using the fact that if we query a set S, MAD(S) is the maximum number that has both copies in S.
    // If we query S and then S ∪ {p}, the MAD can increase if p is the second copy of some number larger than the current MAD.
    
    // We can build the sequence incrementally:
    // Start with an empty set. Add positions one by one. When we add a position, we can query the current set plus the new position. If MAD increases, the new position is the second copy of that number. If MAD stays the same, the new position is either a new number or a copy of a number that is already in the set but not the maximum duplicate.
    // But we don't know which number it is.
    
    // We can maintain the invariant that we know the exact values of all positions in the set. Initially empty.
    // When we add a new position p, we want to determine a[p]. We can query the set S (which we know) together with p. Let M = MAD(S ∪ {p}). If M > MAD(S), then a[p] = M, and we know that the other copy of M is in S. If M == MAD(S), then a[p] is either a number that appears once in S and now twice, but that number must be <= M. Actually, if M == MAD(S), it could be that a[p] equals some number in S that is <= M, but then that number would now appear twice, so MAD would be at least that number. If that number is > M, then M would increase. So if M stays the same, a[p] must be either a new number (not in S) or a copy of a number that is <= M but that number's other copy is already in S? Wait, if a[p] equals some number x in S, then x now appears twice. If x > M, then M would increase to x. So if M doesn't increase, then x <= M. But if x <= M and x appears twice, then MAD should be max(M, x) = M. So M remains the same. So we cannot distinguish between a[p] being a new number or a copy of an existing number <= M.
    
    // This doesn't directly give us the value.
    
    // Let's try a different approach: We can find the positions of each number by querying the whole array minus one position.
    // For each position i, query all positions except i. MAD of that set is the maximum number that appears twice in the array except possibly if the removed position was one copy of that number. If the removed position is one copy of the maximum duplicate, then that number now appears only once, so MAD will be the next highest duplicate. This can help identify the value at position i.
    // Specifically, let M = MAD of all 2n positions. M = n (since all numbers appear twice, the maximum is n). If we remove position i, and query the remaining 2n-1 positions, the MAD will be n if a[i] != n (since n still appears twice). If a[i] == n, then n appears only once, so MAD will be the next highest number that appears twice, which is n-1 (since all numbers appear twice originally, removing one copy of n leaves n-1 as the maximum duplicate). So by querying all positions except i, we can determine if a[i] == n. Then we can find both positions of n. Then we can remove those two positions and repeat for n-1, n-2, ...
    // This would take 2n queries to find all positions? Actually, to find both positions of n, we need to test each position: query all except i. If MAD drops, then a[i]=n. That takes 2n queries to find both n's. Then we remove them and repeat for n-1 on the remaining 2n-2 positions. Total queries: 2n + (2n-2) + ... + 2 = O(n^2). Too many.
    
    // But we can find both positions of n with fewer queries using binary search? We can find one position of n by binary search: divide the array into two halves, query one half. If MAD of that half is n, then both copies of n are in that half. If MAD < n, then at most one copy is in that half. We can binary search to find the positions. That would take O(log n) queries per number. Total O(n log n) ≈ 300*9=2700 > 900. Still too many.
    
    // We need O(n) total queries.
    
    // Let's reconsider the initial adjacent pairs. We used n queries to find some adjacent duplicates. What if we query overlapping pairs? For example, query (1,2), (2,3), (3,4), ..., (2n-1,2n). That's 2n-1 queries. For each pair, if MAD > 0, we know those two positions have that number. This will find all adjacent duplicates. But what if no two copies are adjacent? Then all these queries return 0. In that case, the sequence is a permutation where each number appears exactly twice, but no two same numbers are adjacent. Can we still find the sequence with O(n) queries?
    
    // If no adjacent duplicates, then for any i, a[i] != a[i+1]. We can query triples: (i, i+1, i+2). MAD of this triple is 0 if all three are distinct, or some number if there is a duplicate. Since no adjacent duplicates, the only way to have a duplicate in a triple is if a[i] == a[i+2]. So if MAD > 0, then a[i] = a[i+2] = MAD. This gives us information about positions two apart.
    // We can query all such triples: (1,2,3), (2,3,4), ..., (2n-2,2n-1,2n). That's 2n-2 queries. This will find all duplicates that are distance 2 apart.
    // In general, we can query all pairs of distance d? But we have only 3n queries.
    
    // Maybe we can find all duplicates by querying a set of size n+1? Not sure.
    
    // Let's think about the problem from the perspective of the jury's hint: "If we send two queries, each with k=2 and different indices, then the answer is the maximum of the two."
    // This hint suggests that we can combine two queries of size 2 to get the MAD of their union? Actually, if we have two disjoint sets A and B, and we know MAD(A) and MAD(B), can we determine MAD(A ∪ B)? Not exactly, because the maximum duplicate in the union could be a number that appears once in A and once in B. The hint says the answer is the maximum of the two, which is true if the sets are disjoint and there is no number that appears in both? Wait, if A and B are disjoint, then a number cannot appear in both. So the duplicates in A ∪ B are exactly the duplicates in A plus the duplicates in B. The maximum duplicate in the union is the maximum of the maximum duplicates of A and B. So MAD(A ∪ B) = max(MAD(A), MAD(B)) if A and B are disjoint. That's what the hint says.
    
    // So if we partition the indices into disjoint pairs and query each pair, we get the MAD of each pair. The maximum of all these MADs is the maximum number that appears in an adjacent pair? No, it's the maximum number that has both copies in the same pair. But if a number's copies are in different pairs, it won't be detected.
    
    // However, we can choose the partition cleverly. If we partition into pairs (1,2), (3,4), ..., we detect adjacent duplicates. If we partition into (1,3), (2,4), (5,7), ... we detect duplicates at distance 2. By choosing different partitions, we can detect duplicates at different distances. Since we have 3n queries, we can try a few different partitions.
    
    // How many partitions do we need to guarantee finding all pairs? This is related to the concept of covering all pairs of positions. We need to ensure that for every number, its two positions appear together in at least one queried set. If we query sets of size 2, we need to cover all pairs. The complete graph on 2n vertices has n(2n-1) edges. Each query of size 2 covers one edge. We have 3n queries, so we can cover at most 3n edges. But we need to cover n edges (the n pairs of identical numbers). We don't know which edges they are. We need a set of edges such that for any possible set of n disjoint edges (a perfect matching), at least one edge from each pair is in our set? Actually, we need to identify the exact matching. We can do this by querying edges and using the MAD values to deduce the matching.
    
    // If we query an edge and get MAD = x, we know that edge is a pair of x's. If we get 0, the endpoints are different.
    // So if we query enough edges to form a connected graph or something, we can deduce the values.
    
    // Consider querying a star: (1,2), (1,3), (1,4), ..., (1,2n). That's 2n-1 queries. For each query (1,i), if MAD = x, then a[1] = a[i] = x. If MAD = 0, then a[1] != a[i]. This tells us the value of a[1] (if any MAD > 0) and all positions that have the same value as a[1]. If a[1] is paired with some i, we find it. If a[1] is not paired with any? But every number appears twice, so a[1] must have a duplicate somewhere. So exactly one query (1,i) will return a[1] (the one where i is the other copy). All other queries return 0. So we can find the pair of a[1] with 2n-1 queries. Then we remove those two positions and repeat with the next position. That would take (2n-1) + (2n-3) + ... + 1 = n^2 queries. Too many.
    
    // But we can do better: After finding the pair for a[1], we know a[1] and its positions. We can then use this known value to find other pairs. For example, we can query (1, i, j) for unknown i,j. If MAD = a[1], then both i and j are a[1]? No, a[1] already has two copies. So MAD would be a[1] only if a[1] appears again, which is impossible. So MAD of (1, i, j) will be some other number if i and j are the same, or 0 if they are different. Actually, if i and j are the same number x, then MAD = max(a[1], x) if a[1] != x? Wait, the set is {a[1], x, x}. The duplicates are x (appears twice). a[1] appears once. So MAD = x. So by querying (1, i, j), we can test if i and j are the same number, and if so, what that number is. This allows us to test pairs among unknown positions using one known position.
    
    // So we can do this: Find one pair using a star query (takes O(n) queries). Then we have a known number v at positions p1, p2. Now we want to find all other pairs. We can take all remaining unknown positions and pair them up arbitrarily, and for each pair (i,j), query (p1, i, j). If MAD = x > 0, then i and j are both x. If MAD = 0, then i and j are different. This would take about n queries to test n/2 pairs. But we don't know how to pair them correctly. If we pair them randomly, we might not get all pairs. We need to find the correct pairing.
    
    // We can use the fact that if we query (p1, i, j) and get 0, we know i and j are different. We can then try to match them with other positions. This is like finding a perfect matching in a graph where edges exist between positions with the same value. We can find edges by querying triples.
    
    // Actually, we can find all pairs by querying (p1, i, j) for all pairs (i,j) in the unknown set. That would be O(n^2) queries. But we can do it in O(n) by using a different approach.
    
    // Let's think: After finding one pair (v at p1, p2), we can find the value of any unknown position i by querying (p1, i). If MAD = v, then a[i] = v (but v already has two copies, so this is impossible). So MAD will be 0 always. That doesn't help.
    
    // What if we query (p1, p2, i)? The set is {v, v, a[i]}. MAD = v (since v appears twice). So that always returns v, useless.
    
    // We need a known position of a number that has only one copy in the known set. So we need to find a number for which we know only one position. How to get that?
    
    // Alternative: Use the star query but stop early. Query (1,2), (1,3), ... until we get a non-zero MAD. Suppose (1,i) returns x. Then we know a[1]=a[i]=x. We used i queries. Now we have a known pair. Now we can use this pair to find other pairs? Not directly, because both copies of x are known.
    
    // But we can use one of the copies as a "probe" by pretending we don't know the other? No.
    
    // Let's consider a different initial step: Query (1,2), (2,3), (3,4), ..., (2n-1,2n). This is 2n-1 queries. This will find all adjacent duplicates. If we find any, we can then use them. If we find none, then no two adjacent are equal. In that case, the sequence is an alternating-like sequence? Not necessarily.
    
    // If no adjacent duplicates, then for any i, a[i] != a[i+1]. Now query (1,3), (2,4), (3,5), ..., (2n-2,2n). This is 2n-2 queries. This will find duplicates at distance 2. If we find any, we have a pair. If still none, then no duplicates at distance 1 or 2. We can continue with distance 3, etc. But we only have 3n queries. If we do distances 1,2,3,...,d, total queries ≈ d*(2n) ≈ 3n => d=1.5, so we can only do distance 1 fully, and maybe distance 2 partially. That might not be enough to find a pair if the duplicates are far apart.
    
    // But wait, the sum of n^2 over test cases is ≤ 1e5. n ≤ 300. So n is small. Maybe we can afford O(n^2) queries per test case if n is small? But the limit is 3n queries per test case, not total. So we must adhere to 3n.
    
    // Let's look for a solution that uses exactly 3n queries.
    
    // I recall a known problem: "Guess the Array" or similar interactive problems where you can query MAD. There is a known solution using 2n queries or 3n queries.
    
    // Let's search my memory: There is a Codeforces problem "MAD" or "Maximum Appearing Duplicate". The solution often involves finding the first element by querying it with every other element? No.
    
    // Another idea: We can find the entire sequence by querying all prefixes. Let P_k = {1,2,...,k}. Query P_k for k=2 to 2n. MAD(P_k) tells us the maximum duplicate in the prefix. As we add elements, MAD can increase. When we add the second copy of a number x, if x is larger than the current MAD, MAD becomes x. Otherwise MAD stays the same. This gives us information about when the second copy of each number appears.
    // Specifically, let M_k = MAD(P_k). M_1 = 0 (since size 1). For k from 2 to 2n, we query P_k. We get M_k. If M_k > M_{k-1}, then the new element a[k] is the second copy of M_k, and M_k is the maximum number that now has both copies in the prefix. Also, the first copy of M_k must be somewhere in P_{k-1}. If M_k == M_{k-1}, then a[k] is either a new number or a copy of a number ≤ M_{k-1}.
    // This gives us a way to find the second copy of each number in order of their maximum. But we still need to identify which number is which.
    
    // We can combine this with another set of queries: query suffixes or something.
    
    // Actually, we can determine the exact sequence by querying all prefixes and all suffixes? That would be 4n queries.
    
    // Let's analyze the prefix MADs. We have M_1=0, M_2,...,M_{2n}. Since all numbers appear twice, M_{2n}=n.
    // The sequence M_k is non-decreasing. It increases exactly when we encounter the second copy of a number that is larger than all previous duplicates. The amount of increase tells us which number it is? Actually, if M_k jumps from a to b, then b is the number whose second copy we just saw, and b > a. So we know that a[k] = b, and the first copy of b is somewhere before k.
    // If M_k does not increase, then a[k] is either a first copy of some number, or a second copy of a number ≤ M_{k-1}.
    // We still need to determine the exact values for the non-increasing steps.
    
    // We can also query suffixes: S_k = {k, k+1, ..., 2n}. Let N_k = MAD(S_k). N_{2n}=0, N_{2n-1}=0, etc. N_k is non-increasing as k decreases? Actually, as we add elements to the left, MAD can increase. N_k is the maximum duplicate in the suffix starting at k. When we include the first copy of a number (from the left), if its second copy is already in the suffix, MAD might increase.
    // This is symmetric.
    
    // With both prefix and suffix MADs, we might be able to deduce the positions of each number.
    
    // Let's try to see if we can determine the exact sequence from M_k and N_k. For each number x, its two positions i < j. In the prefix, M_j will be at least x (since at j, both copies are in the prefix). Actually, M_j is the maximum duplicate in P_j. Since x appears twice in P_j, M_j ≥ x. If x is the maximum duplicate in P_j, then M_j = x. Otherwise M_j > x. Similarly, in the suffix, N_i will be at least x.
    // This might give us constraints.
    
    // But we have only 3n queries. Prefixes take 2n-1 queries (k=2..2n). Suffixes take 2n-1 queries. Total 4n-2 > 3n. So we can't do both fully.
    
    // Maybe we can do prefixes and then use the remaining n queries to resolve ambiguities.
    
    // Let's think about the prefix MADs. Suppose we query all prefixes of even length? Or only certain prefixes.
    
    // Another approach: We can find the positions of each number by querying sets of size n+1. For example, query the first n+1 positions. Since there are 2n positions total, by pigeonhole, among any n+1 positions, there must be at least one duplicate. So MAD of any n+1 positions is >0. In fact, it will be the maximum number that has both copies in that set. If we choose the set to be the first n+1 positions, we get some number x. Then we know both copies of x are in the first n+1 positions. We can then find them by querying subsets.
    
    // This suggests a divide and conquer: Query the first half. If MAD is some x, then x appears twice in that half. We can then binary search to find the two positions of x within that half. After finding them, we can remove them and repeat. This is similar to the binary search idea but starting with a large set.
    
    // How many queries to find both positions of x in a set of size S? We can find one position by binary search: query left half, if MAD >= x, then both copies are in left half? Actually, if we query a subset and MAD >= x, it means x appears twice in that subset. So we can binary search to find a subset of size 2 that contains both copies? No, we want to find the exact positions. We can do: split the set into two halves. Query one half. If MAD >= x, then both copies are in that half. If MAD < x, then at most one copy is in that half, so the other half must contain at least one copy. We can recursively search the half that contains both copies. Eventually we narrow down to a set of size 2 that contains both copies? But if we narrow down to a set of size 2, MAD of that set will be x if both are x, or 0 if they are different. So we can find the pair.
    // This binary search would take O(log S) queries. For S = n+1, log(n+1) ≈ 9. Then we remove those two and repeat. Total queries: n/2 * 9? Actually, we find one pair per binary search. There are n pairs. So n * 9 = 2700 queries. Too many.
    
    // But we can find multiple pairs at once? If we query a set and get MAD = x, we know x is the maximum duplicate in that set. But there could be other duplicates in the set (smaller numbers). We only get the maximum. So we can only find the maximum duplicate at each step.
    
    // What if we query the whole array? MAD = n. Then we know n is somewhere. We can find the two n's by binary search in 2n queries? Actually, we can find both n's by querying all positions except one, as before, but that's 2n queries. Then we remove n's and repeat for n-1. Total 2n + 2(n-1) + ... = O(n^2).
    
    // But we can find both n's with O(log n) queries? Let's see: To find one n, we can binary search for a position such that removing it drops MAD from n to something else. That would take O(log n) queries per n. But we need to find both.
    
    // Actually, we can find both n's simultaneously: Query the first half. If MAD = n, then both n's are in the first half. If MAD < n, then at most one n is in the first half. We can recursively find the half that contains both n's. Once we find a half that contains both, we can split it further until we get a set of size 2 that contains both n's. That set will have MAD = n. So we can find the pair of n's in O(log n) queries. Then we remove them and repeat for n-1 on the remaining 2n-2 positions. Total queries: n * O(log n) = O(n log n) ≈ 2700. Still too many for n=300 (3n=900).
    
    // But wait, the sum of n^2 over test cases is ≤ 1e5. The maximum n is 300. The number of test cases t ≤ 3000. If n is small, O(n^2) might be acceptable per test case? But the limit is 3n queries per test case, not total. So we must have a solution that uses at most 3n queries for any n.
    
    // Let's think of a solution that uses exactly 2n or 3n queries.
    
    // I remember a solution: Query all pairs (i, i+1) for i=1..2n-1. This gives us some information. Then query all triples (i, i+1, i+2) for i=1..2n-2. Then we can deduce the sequence? Let's test.
    
    // Suppose we query all adjacent pairs: (1,2), (2,3), ..., (2n-1,2n). Let the results be P_i = MAD(a_i, a_{i+1}). P_i > 0 iff a_i = a_{i+1} = P_i.
    // Also query all triples: (1,2,3), (2,3,4), ..., (2n-2,2n-1,2n). Let T_i = MAD(a_i, a_{i+1}, a_{i+2}).
    // Can we reconstruct the sequence from these?
    
    // Consider the first three elements: a1, a2, a3.
    // We know P1 = MAD(a1,a2), P2 = MAD(a2,a3), T1 = MAD(a1,a2,a3).
    // If P1 > 0, then a1=a2=P1. Then T1 = max(P1, a3) if a3 equals something? Actually, if a1=a2=x, then T1 = max(x, a3) if a3 equals x? No, a3 is a single element, so the only duplicate is x. So T1 = x. So if P1 > 0, T1 = P1. Similarly, if P2 > 0, T1 = P2.
    // If both P1=0 and P2=0, then a1!=a2 and a2!=a3. Then T1 could be >0 only if a1=a3. So T1 > 0 implies a1=a3=T1.
    // So from these three queries on the first three elements, we can determine the values of a1,a2,a3 in most cases.
    // Let's generalize: With all P_i and T_i, we
