#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n, q;
        cin >> n >> q;
        vector<int> p(n + 1);
        for (int i = 1; i <= n; ++i) {
            cin >> p[i];
        }
        string s;
        cin >> s;
        s = " " + s; // 1-indexed

        // We need to check if the permutation can be sorted.
        // The key observation: we can swap any two elements if the segment between them
        // has alternating L/R pattern? Actually, the operations allow swapping adjacent
        // elements if the direction matches. This is equivalent to: we can move an element
        // left if there is a sequence of R's? Wait, the operation: if s_i = L, swap p_i and p_{i-1}.
        // So to move an element left, we need L at its current position. To move right, we need R.
        // This is like a sorting network where each adjacent pair can be swapped in one direction.
        // The permutation can be sorted iff for every i, the element that should be at position i
        // can reach position i. More precisely, the allowed swaps generate a set of reachable permutations.
        // It is known that such a system sorts the array iff for every i, the maximum of p[1..i] is <= i
        // when considering the "blocked" connections? Let's think.

        // Hint: Consider parity of distance. If p_i < p_j and distance is even, we can swap them.
        // Actually, the operations allow swapping adjacent elements in specific directions.
        // This is equivalent to: we can reverse any segment that has alternating L and R?
        // Let's analyze: if we have ... R L ... then we can swap the middle two? 
        // Wait, s_i = R means we can swap p_i and p_{i+1}. s_i = L means swap p_i and p_{i-1}.
        // So if we have s = R L, then at position i (R) we can swap right, at i+1 (L) we can swap left.
        // This allows us to bubble an element through.

        // Another perspective: The array is divided into segments by "RR" and "LL"? 
        // Actually, if we have s_i = R and s_{i+1} = L, then we can swap p_i and p_{i+1} using either operation.
        // If we have s_i = L and s_{i+1} = R, then we cannot directly swap p_i and p_{i+1} because 
        // to swap them we would need s_i = R (swap right) or s_{i+1} = L (swap left). Neither is available.
        // So the barrier is when we have LR (L then R). At such boundaries, elements cannot cross.
        // Therefore, the array is partitioned into blocks separated by "LR" boundaries.
        // Within a block, the pattern is like R...RL...L? Actually, if there is no LR, then the string
        // is of the form R* L*. In such a block, we can fully sort the subarray? Let's check.
        // If we have RRRLLL, can we sort any permutation? We can bubble elements right using R, and left using L.
        // It is known that such a block can be sorted iff the set of elements in the block is exactly the set of positions.
        // But wait, the operations only allow adjacent swaps in one direction. However, with both R and L available,
        // we can perform any adjacent swap? Actually, if we have both R and L at different positions, we can move elements
        // around. But can we reverse a segment? For example, R L allows swapping the middle two. With RRR LLL, we can
        // bubble sort. So within a block that has no LR, we can sort the subarray arbitrarily. 
        // But is that true? Consider a block of length 3 with s = RRL. Positions: 1:R, 2:R, 3:L.
        // Can we swap p_1 and p_3? We can swap p_2 and p_3 (using L at 3), then p_1 and p_2 (using R at 1), then p_2 and p_3 again.
        // So yes, we can perform any permutation within a block that has no LR.
        // However, if there is an LR, that acts as a wall: elements cannot cross from left of L to right of R.
        // Because to move an element from left of L to right of R, it would need to cross the boundary between the L and R.
        // At that boundary, we have s_i = L and s_{i+1} = R. To swap them, we would need s_i = R or s_{i+1} = L, neither is true.
        // So elements cannot cross an LR boundary.
        // Therefore, the array is partitioned into segments separated by indices i where s_i = L and s_{i+1} = R.
        // Within each segment, the string has no LR, so it is of the form R* L* (since it starts with R and ends with L? 
        // Actually, the whole string starts with R and ends with L. Within a segment, it could be all R's or all L's? 
        // But if a segment is separated by LR boundaries, then inside the segment there is no LR. So it must be that 
        // all R's come before all L's. So each segment is a contiguous block of R's followed by L's.
        // In such a segment, we can sort the subarray arbitrarily. 
        // But wait: is it always possible to sort any permutation within such a segment? 
        // Let's test: segment with s = R R L. Positions 1,2,3. We can swap (1,2) using R at 1, (2,3) using L at 3.
        // This is exactly the adjacent swaps available in a standard bubble sort? Actually, we can swap (1,2) and (2,3).
        // That's all adjacent swaps. So we can generate any permutation. Yes, because the swaps generate the full symmetric group.
        // So within a segment without LR, we can sort arbitrarily.
        // Therefore, the whole array can be sorted iff for every segment, the set of elements in that segment is exactly the set of positions of that segment.
        // In other words, the maximum element in each segment must be <= the maximum position of that segment, and the minimum element >= minimum position.
        // Since it's a permutation, it's equivalent to: for every segment [L, R], the set of values in that segment is exactly {L, L+1, ..., R}.
        // This is equivalent to: for every i from 1 to n-1, if the prefix [1..i] contains exactly the values {1..i}, then it's okay.
        // But we have segments. So we need that for each segment boundary (which are the LR boundaries), the prefix up to that boundary is sorted.
        // Actually, if we consider the partition by LR boundaries, then elements cannot cross boundaries. So the elements in the left part must be exactly the smallest ones.
        // So the condition is: for every i such that s_i = L and s_{i+1} = R, we must have max(p[1..i]) == i.
        // Because the left segment ends at i, and the right segment starts at i+1. Since no elements can cross, the left segment must contain exactly {1..i}.
        // Is that sufficient? What about the boundaries at the ends? The whole array is a segment from 1 to n. The condition at the LR boundaries ensures that each segment contains the correct set.
        // But wait: what if there is no LR boundary? Then the whole array is one segment, and we can sort it always? But the problem says s_1 = R and s_n = L. So if there is no LR, then the string is R...RL...L. That's one segment. Can we always sort it? Let's test: n=3, s = RRL. p = [2,3,1]. Can we sort? We can swap (1,2) -> [3,2,1], swap (2,3) -> [3,1,2], swap (1,2) -> [1,3,2], swap (2,3) -> [1,2,3]. Yes. So it seems we can sort any permutation in a single segment. So the only obstacles are the LR boundaries.
        // But wait: the sample 1: n=5, p = [1,4,2,5,3], s = RLRLL. Let's find LR boundaries: s = R L R L L. LR at i=1? s_1=R, s_2=L -> not LR. LR is L followed by R. So we look for i such that s_i=L and s_{i+1}=R. Here s_2=L, s_3=R -> i=2 is LR boundary. s_4=L, s_5=L -> no. So segments: [1,2] and [3,5]. For segment [1,2], values should be {1,2}. p[1..2] = [1,4] -> max is 4 != 2. So initially it's impossible? But the sample says after first query (change index 2 from L to R), s becomes R R R L L. Then LR boundaries? s_3=R, s_4=L -> no LR. s_4=L, s_5=L -> no. So no LR boundaries. Then it's sortable. Output YES. That matches.
        // After second query (change index 4 from L to R): s = R R R R L? Wait, initial after first query: R R R L L. Change index 4: L->R gives R R R R L. LR boundaries? s_4=R, s_5=L -> no LR. So still sortable? Output YES. Sample says YES.
        // After third query (change index 3 from R to L): s = R R L R L. LR boundaries: s_3=L, s_4=R -> i=3 is LR. Segments: [1,3] and [4,5]. Check segment [1,3]: values should be {1,2,3}. p[1..3] = [1,4,2] -> max=4 != 3. So impossible. Output NO. Matches sample.
        // So the condition is exactly: for every i from 1 to n-1, if s[i] == 'L' and s[i+1] == 'R', then we need max(p[1..i]) == i.
        // But wait: is that sufficient? Consider a segment that is not the whole array. Within that segment, we can sort arbitrarily. So if the segment has the correct set of values, we can sort it. So yes.
        // However, we also need to consider the boundaries at the ends? The whole array must be sorted eventually. If all segments have correct sets, then sorting each segment individually yields a sorted array. So condition is necessary and sufficient.
        // But wait: what about the case where there is an LR boundary but the left segment is empty? Not possible because i from 1 to n-1.
        // So we just need to maintain the set of "bad" indices i where s[i]=='L' and s[i+1]=='R' and max(p[1..i]) != i.
        // If the number of bad indices is 0, then YES, else NO.
        // We need to handle updates: flip s[i]. This can affect boundaries at i-1 and i.
        // So we can precompute prefix maximums of p: pref_max[i] = max(p[1..i]).
        // Then an index i (1 <= i < n) is bad if s[i]=='L' && s[i+1]=='R' && pref_max[i] != i.
        // We maintain a count of bad indices.
        // For each query, we flip s[i]. We need to update the status of boundaries at i-1 (if i-1 >= 1) and i (if i < n).
        // We can write a function to check if an index j is bad, and update the count accordingly.
        // Since n and q sum to 2e5, O(1) per query is fine.

        vector<int> pref_max(n + 1, 0);
        for (int i = 1; i <= n; ++i) {
            pref_max[i] = max(pref_max[i-1], p[i]);
        }

        auto is_bad = [&](int idx) -> bool {
            if (idx < 1 || idx >= n) return false;
            return (s[idx] == 'L' && s[idx+1] == 'R' && pref_max[idx] != idx);
        };

        int bad_cnt = 0;
        for (int i = 1; i < n; ++i) {
            if (is_bad(i)) bad_cnt++;
        }

        while (q--) {
            int idx;
            cin >> idx;
            // Flip s[idx]
            // Before flipping, remove contributions from boundaries idx-1 and idx
            if (is_bad(idx-1)) bad_cnt--;
            if (is_bad(idx)) bad_cnt--;
            s[idx] = (s[idx] == 'L' ? 'R' : 'L');
            // After flipping, add contributions
            if (is_bad(idx-1)) bad_cnt++;
            if (is_bad(idx)) bad_cnt++;
            cout << (bad_cnt == 0 ? "YES" : "NO") << "\n";
        }
    }
    return 0;
}
