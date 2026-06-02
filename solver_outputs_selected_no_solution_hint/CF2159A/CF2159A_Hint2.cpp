#include <bits/stdc++.h>
using namespace std;

int query(vector<int> &idx) {
    cout << "? " << idx.size();
    for (int x : idx) cout << " " << x;
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
    
    // Find pairs using queries of size 2
    vector<int> pairs;
    vector<bool> used(m + 1, false);
    
    for (int i = 1; i <= m; i++) {
        if (used[i]) continue;
        // find partner for i
        int partner = -1;
        for (int j = i + 1; j <= m; j++) {
            if (used[j]) continue;
            vector<int> q = {i, j};
            int res = query(q);
            if (res > 0) {
                partner = j;
                break;
            }
        }
        // now we know i and partner have same value, but we don't know which value
        // we need to determine the value
        // we can query with another index to find the value
        // but we can also just assign values later
        pairs.push_back(i);
        pairs.push_back(partner);
        used[i] = used[partner] = true;
    }
    
    // Now we have pairs of indices that share the same value.
    // We need to assign actual values 1..n to these pairs.
    // We can determine values by querying triples.
    // For each pair (i, j), we can find its value by querying with another index k
    // that we already know the value of, or we can use a different approach.
    
    // Actually, we can determine values by querying with a known value.
    // Let's first find the value of the first pair.
    // We can query the first pair with another index from another pair.
    // If we query (i, j, k) where k is from another pair, the MAD will be max(val(i), val(k)) if val(i) != val(k),
    // or val(i) if val(i) == val(k). But we don't know val(k) either.
    
    // Better approach: We can find the maximum value n first.
    // Query all indices? That would be 1 query of size 2n, but we have limit 3n queries.
    // Actually, we can find the value of each pair by querying with a known value.
    // We can determine the value of the first pair by querying it with another index from a different pair,
    // but we need to know the value of that other pair. This is circular.
    
    // Alternative: We can find the value of a pair by querying it with two other indices from two different pairs.
    // If we query (i, j, k, l) where (i,j) is one pair and (k,l) is another pair, the MAD will be max(val(i), val(k)).
    // But we still don't know which is which.
    
    // Let's think differently. We can find the value of each pair by querying it with an index that we already know.
    // How to get a known value? We can find the value n by querying all indices? No, too many queries.
    
    // Actually, we can determine the value of a pair by querying it with another pair's one index.
    // Suppose we have pairs (i,j) and (k,l). We query (i, k). If the result is 0, they have different values.
    // If the result is >0, they have the same value, and that value is the result.
    // So we can group pairs by their values.
    
    // Let's group pairs by querying one index from each pair with one index from another pair.
    // We can pick a representative from each pair.
    vector<int> rep;
    for (int idx = 0; idx < pairs.size(); idx += 2) {
        rep.push_back(pairs[idx]);
    }
    
    // Now we have n representatives. We need to assign values 1..n to these representatives.
    // We can find the value of each representative by querying pairs of representatives.
    // If we query (rep[i], rep[j]), the result is 0 if they have different values, or the value if they have the same value.
    // But we don't know the value yet.
    
    // We can find the maximum value n by querying all representatives? That would be one query of size n.
    // Query all representatives: MAD will be the maximum value that appears at least twice.
    // But each value appears exactly twice in the whole sequence, so among representatives, each value appears exactly once.
    // So MAD of all representatives is 0! That doesn't help.
    
    // We need to find the actual values. We can do this by querying triples.
    // For a pair (i,j), we can query (i, j, x) where x is an index from another pair.
    // The result will be max(val(i), val(x)) if val(i) != val(x), or val(i) if val(i) == val(x).
    // If we know val(x), we can determine val(i).
    
    // How to get a known value? We can find the value n by querying a pair with another index that we know is not n?
    // Actually, we can find the value n by querying a pair with an index from another pair, and then using the fact that n is the maximum.
    // But we don't know which pair has n.
    
    // Let's use a different strategy: We can determine the value of a pair by querying it with two other indices from two different pairs.
    // Suppose we have three pairs: A, B, C. We query one index from A, one from B, one from C.
    // The MAD will be the maximum value among the three if there is a duplicate, but since all three are from different pairs, they all have distinct values? Not necessarily, they could have the same value if we picked wrong representatives? No, each pair has a unique value, so representatives from different pairs have different values. So MAD of three representatives is 0.
    // That doesn't help.
    
    // Wait, the problem says the sequence contains each integer from 1 to n exactly twice. So there are n pairs of identical values.
    // If we pick one index from each pair, we get n distinct values. So any query with only representatives will have MAD 0.
    // To get a non-zero MAD, we must include both indices from at least one pair.
    
    // So to determine the value of a pair, we can query both indices of that pair together with one index from another pair.
    // Let the pair be (i,j) with value v, and the other index k with value u.
    // Query (i, j, k): the sequence is [v, v, u]. MAD is max(v, u) if v != u, or v if v == u.
    // So the result is max(v, u).
    // If we know u, we can find v: if result == u, then v <= u; if result > u, then v = result.
    // But we don't know u.
    
    // However, we can find the maximum value n by querying a pair with another index, and then using the fact that n is the maximum possible.
    // If we query a pair (i,j) with some k, and get result r, then v = r if r > u, but we don't know u.
    // But if we do this for all pairs with the same k, the maximum result we get must be n, because for the pair with value n, the result will be n (since n >= u for any u).
    // So we can find the pair with value n by querying each pair with a fixed k, and seeing which gives the maximum result.
    // But we need to know u to determine other values.
    
    // Let's pick an arbitrary index k from some pair. We don't know its value u.
    // For each other pair (i,j), query (i, j, k). Let the result be r.
    // If r == u, then v <= u. If r > u, then v = r.
    // But we don't know u. However, we know that the maximum possible value is n.
    // The maximum r we get across all pairs must be n, because for the pair with value n, r = max(n, u) = n.
    // So we can identify the pair with value n: it's the one that gives r = n.
    // Then we know u = value of k. How? If k is from the pair with value n, then u = n. But we don't know which pair k belongs to.
    // Actually, we can choose k to be from a specific pair. Let's say we pick k from the first pair. Then we query all other pairs with k.
    // The maximum result we get is n. The pair that gives n is the one with value n. But we still don't know u (value of k).
    // However, if the first pair itself has value n, then all queries will return n, and we can't distinguish.
    // So we need a different approach.
    
    // Let's think about the hints: "If we send two queries, each with k=2 and different indices, then the answer is the maximum of the two."
    // This suggests we can combine results of 2-element queries.
    // For a pair (i,j), if we query (i, j), we get the value v.
    // So we can just query each pair individually! But we don't know which indices form a pair.
    // We can find pairs by querying all pairs of indices? That would be O(n^2) queries, too many.
    
    // But we can find pairs efficiently: For each index i, we can find its partner by binary search or something?
    // Actually, we can find the partner of index 1 by querying (1, j) for j=2..2n. That's 2n-1 queries.
    // Then we remove that pair and repeat. Total queries: (2n-1) + (2n-3) + ... + 1 = n^2. Too many for n=300 (n^2=90000, sum of n^2 over test cases <= 1e5, so maybe it's acceptable? But 3n limit is 900, so n^2 is too much).
    
    // We need to use the MAD property more cleverly.
    // The hint says: "If we send two queries, each with k=2 and different indices, then the answer is the maximum of the two."
    // This means if we query (i, j) and (i, k), and get r1 and r2, then max(r1, r2) = value of i if i is paired with either j or k? Not exactly.
    // Let's analyze: Query (i, j) returns v if a_i = a_j = v, else 0.
    // So if we query (i, j) and (i, k), and get r1 and r2, then if i's partner is among {j, k}, one of them will be v, the other 0. So max is v.
    // If neither is the partner, both are 0, max is 0.
    // So we can find the partner of i by querying (i, j) for all j, but we can do it in a divide-and-conquer way?
    // We can find the partner of i by querying sets of indices.
    
    // Actually, we can find the partner of an index by querying a set of candidates.
    // If we query (i, S) where S is a set of indices, what does MAD give?
    // MAD of [a_i, a_S] will be v if a_i = v and v appears in S (i.e., i's partner is in S), or if there is some other duplicate in S.
    // But if S contains only indices that are not partners of each other (except possibly i's partner), then MAD will be v if partner in S, else 0.
    // We can ensure S has no internal duplicates by picking one index from each pair we've already found.
    
    // Let's try to find pairs one by one.
    // We can maintain a set of "available" indices. Initially all 1..2n.
    // Pick the first available index i. We want to find its partner.
    // We can query i with a subset of available indices. If we query i with a set T, and the result is 0, then i's partner is not in T.
    // If the result is v > 0, then i's partner is in T, and v is the value.
    // We can binary search on the available indices to find the partner with about log(2n) queries.
    // But we need to ensure that T doesn't contain any pair internally, otherwise MAD could be >0 even if i's partner is not there.
    // However, if T contains a pair (x,y) with value u, then MAD of [i, T] could be u if u > v, or v if v > u, etc.
    // So we can't just use arbitrary subsets.
    
    // We need to construct subsets that don't have internal duplicates.
    // We can do this by, after finding some pairs, we only use one index from each found pair in the subsets.
    // But initially we have no found pairs.
    
    // Alternative: We can find all pairs simultaneously using a randomized or clever approach.
    // Let's think about the hint again: "If we send two queries, each with k=2 and different indices, then the answer is the maximum of the two."
    // This might mean we can combine queries to get information about multiple pairs at once.
    
    // Consider we want to find the partner of index 1. We can query (1, 2), (1, 3), ..., (1, 2n). That's 2n-1 queries.
    // But we can do better: query (1, 2), (1, 3), ... but we can stop when we find the partner? The partner is the one that gives non-zero.
    // In the worst case, partner is the last one, so 2n-1 queries. That's O(n) per index, total O(n^2). Not good.
    
    // But we can use the fact that we can query multiple indices at once.
    // Suppose we query (1, 2, 3, ..., k). If the result is 0, then 1's partner is not among 2..k.
    // If the result is v > 0, then 1's partner is among 2..k, and v is the value of 1.
    // But wait, if among 2..k there is a pair, the MAD could be that pair's value, not necessarily 1's value.
    // So we need to ensure that the set 2..k has no internal duplicates.
    // How can we ensure that? We don't know the pairs yet.
    
    // We can use a different strategy: First, find all values by querying the whole set?
    // If we query all 2n indices, MAD is n (since all numbers appear twice, the maximum duplicate is n).
    // So one query gives n. That doesn't help much.
    
    // Let's look at the sample interaction:
    // n=2. Sequence: 2 2 1 1.
    // Queries:
    // ? 2 2 1 -> MAD([a2,a1]) = MAD([2,2]) = 2
    // ? 2 1 3 -> MAD([a1,a3]) = MAD([2,1]) = 0
    // ? 3 1 3 4 -> MAD([a1,a3,a4]) = MAD([2,1,1]) = 1
    // Then they output ! 2 2 1 1.
    // They used 3 queries for n=2. 3n = 6, so they had room.
    
    // Notice that they queried (2,1) got 2, (1,3) got 0, (1,3,4) got 1.
    // From (2,1)=2, they know a1=a2=2.
    // From (1,3)=0, they know a1 != a3.
    // From (1,3,4)=1, they know that among a1,a3,a4, the MAD is 1. Since a1=2, the duplicate must be from a3,a4, so a3=a4=1.
    // So they found pairs by querying triples.
    
    // General strategy:
    // We can find pairs by querying triples of the form (i, j, k) where we already know i and j are a pair (value v).
    // Then if we query (i, j, k), the result is max(v, a_k) if a_k != v, or v if a_k = v.
    // But if we don't know v, we can still use this to find pairs.
    
    // Let's try to find one pair first.
    // We can find a pair by querying (1, 2), (1, 3), ... until we get a non-zero. That's O(n) queries.
    // Once we have one pair (i, j) with value v, we can use it to find other pairs.
    // For any other index k, we can query (i, j, k). The result r = max(v, a_k).
    // If r == v, then a_k <= v. If r > v, then a_k = r.
    // But we don't know v yet! We only know that i and j are a pair, but we don't know their value.
    // However, we can determine v by querying (i, j) alone? That would give v. But we already queried (i, j) to find they are a pair? Actually, we found they are a pair by querying (i, j) and getting non-zero. That non-zero is exactly v. So we do know v!
    // So the process:
    // 1. Find a pair by querying (1, x) for x=2..2n until we get non-zero. This takes at most 2n-1 queries, but on average n. Let's say we find pair (1, p) with value v.
    // 2. Now we have a known pair (1, p) with known value v.
    // 3. For each remaining index k (not 1 or p), we can query (1, p, k). The result r = max(v, a_k).
    //    If r == v, then a_k <= v. If r > v, then a_k = r.
    //    But we need to find the partner of k. We can group indices by their value.
    //    Actually, if r > v, we immediately know a_k = r. Then we can find its partner by querying (k, other) with known value? Or we can just remember that we have an index with value r, and later when we find another index with the same r, they are a pair.
    //    If r == v, then a_k <= v. We don't know the exact value, but we know it's <= v.
    //    We can then try to find the partner of k among other indices that also gave r == v.
    //    But we can do better: We can use the known pair to "probe" and find the maximum value n.
    //    Actually, if we query (1, p, k) and get r, then if r > v, we know a_k = r. The maximum r we get across all k will be n.
    //    So we can find all values > v easily.
    //    What about values <= v? We can then use a pair with a higher value to probe the remaining.
    //    This suggests a recursive/divide-and-conquer approach: find the maximum value, then split.
    
    // Let's formalize:
    // We have a set of indices S (initially all 1..2n).
    // We want to find the pairs.
    // We can find the maximum value in S by querying a known pair? But we don't have a known pair yet.
    // We can find a pair by brute force in O(|S|) queries. But we can do better.
    
    // Actually, we can find the maximum value without finding a pair first.
    // Query all indices in S: MAD is the maximum value that appears at least twice. Since all values appear exactly twice in the whole sequence, if S is the whole set, MAD = n.
    // If S is a subset, MAD is the maximum value that has both occurrences in S.
    // So if we query S, we get the maximum value that is completely contained in S.
    // This is a powerful primitive!
    
    // Let's use this: We can find the maximum value in the whole sequence by querying all 2n indices: it returns n.
    // But we already know n is the maximum. We need to find which indices have value n.
    // We can binary search to find one index with value n?
    // If we take a subset T, and query T, if the result is n, then both occurrences of n are in T.
    // If the result is < n, then at least one occurrence of n is outside T.
    // We can use this to find the two indices with value n in O(log n) queries!
    // Once we find the pair for n, we can remove them and recurse on the remaining with maximum n-1, and so on.
    // This would take O(n log n) queries, which is well within 3n.
    
    // Let's detail the binary search for a specific value v (starting with v=n).
    // We have a set of indices S (initially all 1..2n). We know that the maximum value completely contained in S is v (we can query S to get v).
    // We want to find the two indices in S that have value v.
    // We can split S into two halves L and R.
    // Query L: if MAD(L) == v, then both v's are in L. Otherwise, if MAD(L) < v, then at least one v is in R.
    // But we need to be careful: MAD(L) could be v even if only one v is in L? No, MAD is the maximum duplicate, so it requires at least two occurrences. So MAD(L) == v iff both v's are in L.
    // So we can binary search to isolate one index with value v.
    // Actually, we can find one index by maintaining a set that contains at least one v.
    // Start with S. We know S contains both v's.
    // We want to find a subset that contains exactly one v? Not necessary.
    // We can find the two indices by splitting: if we split S into L and R, and query L. If MAD(L) == v, then both are in L, so we recurse on L. If MAD(L) != v, then they are not both in L, so they must be split: one in L, one in R. Then we can binary search within L to find the one in L, and similarly in R.
    // But we can just find one index with value v by binary search:
    // We want to find a subset T of S such that T contains exactly one v. How?
    // We can take a subset T, query T. If MAD(T) == v, then T contains both v's. If MAD(T) < v, then T contains at most one v.
    // So we can start with S, and repeatedly halve it. At each step, we take a half H. If MAD(H) == v, then both are in H, so we set S = H. If MAD(H) < v, then H contains at most one v, so the other half must contain the other v. We can then pick one half that contains at least one v and continue.
    // Actually, we can just find one index by:
    // Let current set = S. While |current set| > 1:
    //   Split current set into two halves A and B.
    //   Query A.
    //   If MAD(A) == v, then both v's are in A, so set current set = A.
    //   Else, MAD(A) < v, so A contains at most one v. Then B must contain at least one v. Set current set = B.
    // This will shrink the set while always containing at least one v. Eventually we get a single index. That index has value v.
    // But wait: if A contains exactly one v, MAD(A) < v (since no duplicate of v). So we correctly identify that A has at most one v. Then we go to B, which has the other v. So we end up with one index of value v.
    // This takes O(log n) queries.
    // Once we have one index i with value v, we need to find its partner j.
    // We can find j by binary search on the remaining indices (excluding i) to find another index with value v.
    // We know the partner is in S \ {i}. We can do a similar binary search: maintain a set that contains the partner.
    // Initially, the set is S \ {i}. We know it contains exactly one v (the partner).
    // We can binary search: split into A and B. Query A ∪ {i}? If we query A ∪ {i}, the MAD will be v if A contains the partner (since then we have two v's). If A does not contain the partner, MAD(A ∪ {i}) < v.
    // So we can binary search to find the partner in O(log n) queries.
    // Then we have the pair for v. We remove them from S, and repeat for v-1.
    // Total queries: For each value from n down to 1, we spend O(log n) to find one index, and O(log n) to find its partner. That's O(n log n) queries. With n=300, log n ~ 9, so ~5400 queries per test case? But sum of n^2 <= 1e5, so n <= 300, but there can be many test cases. 5400 is > 3n=900. So O(n log n) is too much.
    
    // We need to reduce queries to O(n). The 3n limit suggests we can do about 1.5 queries per element on average.
    // We need a more efficient way to find pairs.
    
    // Let's think about the hint again: "If we send two queries, each with k=2 and different indices, then the answer is the maximum of the two."
    // This implies we can combine pairwise queries to get information.
    // Suppose we want to find the partner of index 1. We can query (1, 2), (1, 3), ..., (1, 2n). That's 2n-1 queries.
    // But we can do it in 1 query by querying (1, 2, 3, ..., 2n)? No, that gives MAD of the whole set, which is n, not necessarily the value of 1.
    // However, if we query (1, 2, 3, ..., 2n) and get n, that doesn't tell us 1's partner.
    
    // What if we query (1, 2, 3, ..., k) for some k? If 1's partner is among 2..k, and there are no other duplicates in 2..k, then MAD = value of 1.
    // But we can't guarantee no other duplicates.
    
    // We can use the fact that we can find the maximum value in a set by querying the set.
    // Maybe we can find all pairs by a divide-and-conquer that only uses O(n) queries?
    // Consider we have a set S. We query S to get the maximum value v in S.
    // Then we split S into two halves L and R. We query L. If MAD(L) == v, then both v's are in L. Otherwise, they are split.
    // This is like a segment tree. We can recursively find all pairs.
    // The total number of queries would be the number of nodes in a binary tree where we stop when a node has no pairs? Actually, we only need to query a node if it contains at least one complete pair. The number of such nodes is O(n). Because each pair contributes to O(log n) nodes? Wait, each pair is completely contained in some nodes. The total number of nodes that contain at least one complete pair is bounded by O(n log n) in the worst case? Actually, if we build a segment tree over 2n leaves, the number of nodes that contain at least one complete pair is at most the number of nodes that cover at least one pair. Each pair is an interval of length 2? No, the indices of a pair are not necessarily adjacent. So a pair can be split across many nodes.
    // This might not be efficient.
    
    // Let's look for a known solution or pattern.
    // The problem is from Codeforces, likely a recent one. The hint says "If we send two queries, each with k=2 and different indices, then the answer is the maximum of the two."
    // This suggests we can determine the value of an index by querying it with two different indices.
    // Suppose we want to find a_1. We can query (1, 2) and (1, 3). Let r1 = query(1,2), r2 = query(1,3).
    // If a_1 = a_2, then r1 = a_1, r2 = 0 (if a_1 != a_3) or r2 = a_1 (if a_1 = a_3). So max(r1, r2) = a_1.
    // If a_1 != a_2 and a_1 != a_3, then r1 = 0, r2 = 0, max = 0.
    // If a_1 != a_2 but a_2 = a_3, then r1 = 0, r2 = a_2 (since MAD of [a_1, a_3] where a_3 = a_2, but a_1 is different, so MAD is a_2). So max could be a_2, which is not a_1.
    // So max of two pairwise queries with a common index gives the value of that index only if its partner is among the other two, OR if the other two are a pair with a higher value? Actually, if the other two are a pair with value > a_1, then max could be that value, which is not a_1.
    // So this doesn't reliably give a_1.
    
    // The hint might be interpreted differently: If we send two queries, each with k=2 and different indices (meaning the two queries have no common indices?), then the answer is the maximum of the two results? That doesn't make sense.
    
    // Let's read the hint carefully: "If we send two queries, each with k=2 and different indices, then the answer is the maximum of the two."
    // Maybe it means if we query (i, j) and (i, k), and we get r1 and r2, then max(r1, r2) is the value of i if i's partner is j or k? But as we saw, it could be the value of the pair (j,k) if they are a pair.
    // However, if we ensure that j and k are not a pair, then max(r1, r2) = a_i if i's partner is among {j,k}, else 0.
    // So if we can find a set of indices that are all from different pairs, we can test i against them.
    
    // We can build a set of representatives from different pairs incrementally.
    // Start with an empty set of representatives.
    // For each index i from 1 to 2n:
    //   We want to find its partner. We can query i against all current representatives? But we don't know if i's partner is among them.
    //   Actually, if we query i with a representative r, we get a_i if a_i = a_r, else 0.
    //   So if we query i with all representatives, we can check if i matches any of them. If yes, we found its partner (the representative) and we know the value.
    //   If no match, then i's partner is not among the representatives. So i must be paired with some other new index. We can then find its partner by querying i with all remaining indices? That's still O(n^2).
    
    // But we can use the MAD of larger sets to find the partner.
    // Suppose we have a set of representatives R (all from different pairs). We know their values? Not necessarily.
    // If we query R ∪ {i}, the MAD will be the maximum value that appears twice. Since R has all distinct values, the only possible duplicate is if i matches one of them, or if R contains a duplicate? No, R has distinct values. So MAD(R ∪ {i}) = a_i if i matches some r in R, else 0 (if a_i is not in R) OR if a_i is in R but i is not the partner? Wait, if a_i is in R, that means there is some r in R with a_r = a_i. Then i and r are a pair. So MAD will be a_i.
    // So querying R ∪ {i} tells us if i's partner is in R, and if so, what the value is.
    // If MAD is 0, then i's partner is not in R.
    // This is a great way to test if i matches any representative in one query!
    // So we can do:
    // R = empty
    // For i = 1 to 2n:
    //   if i is already paired, skip.
    //   Query R ∪ {i}. Let v = result.
    //   If v > 0:
    //     Then i's partner is in R, and the value is v. We need to find which representative has value v.
    //     We can binary search within R to find the representative with value v? But we don't know the values of representatives.
    //     However, we can just query i with each representative? That would be O(|R|) queries, which could be O(n) per i, total O(n^2).
    //     But we can find the partner by binary search on R using the same idea: split R into two halves, query i with one half. If MAD > 0, the partner is in that half. This takes O(log n) queries.
    //   If v == 0:
    //     Then i's partner is not in R. So we need to find i's partner among the remaining indices (which are not in R and not i).
    //     We can add i to R as a new representative. But we don't know its value yet.
    //     Later, when we find its partner, we will know the value.
    // This way, we process each index once. When we find a match with R, we remove the matched representative from R? Actually, we found a pair: i and some r in R. We can record the pair, and remove r from R. Then we continue.
    // But we still need to find the partner for the new representatives we add.
    // Let's simulate:
    // Start with R empty.
    // i=1: R empty, query R∪{1} = {1} -> MAD of single element is 0. So no match. Add 1 to R. R = {1}.
    // i=2: query R∪{2} = {1,2}. If a1=a2, MAD = a1. Else 0.
    //   If match, we need to find which one in R matches. R has only 1, so it's 1. Pair (1,2) found. Remove 1 from R. R = {}.
    //   If no match, add 2 to R. R = {1,2}.
    // i=3: query R∪{3}. If match, binary search in R to find partner. If no match, add 3 to R.
    // ...
    // This process will find all pairs. How many queries?
    // For each i, we do one query to test if it matches R. If it matches, we do O(log |R|) queries to find the partner. If it doesn't match, we just add to R (no extra query).
    // The total number of matches is n (since there are n pairs). Each match costs O(log n) queries. So total queries = 2n (one per i) + n * O(log n) = O(n log n). Still O(n log n).
    // But wait, the initial query for each i is R∪{i}. The size of R grows up to n. So the query size is up to
