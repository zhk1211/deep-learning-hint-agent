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

        // pos[x] = index of value x in p
        vector<int> pos(n + 1);
        for (int i = 1; i <= n; ++i) {
            pos[p[i]] = i;
        }

        // We need to check if for every x from 1 to n-1,
        // the segment [pos[x], pos[x+1]] (or [pos[x+1], pos[x]]) is "connected"
        // by allowed swaps. The condition: for all k in [min(pos[x], pos[x+1]), max(pos[x], pos[x+1]) - 1],
        // we must have s[k] == 'R' or s[k+1] == 'L'? Actually the known condition:
        // It is possible to sort iff for every i from 1 to n-1, the interval between pos[i] and pos[i+1]
        // does not contain a "blocked" adjacent pair. A pair (k, k+1) is blocked if s[k] == 'L' and s[k+1] == 'R'.
        // Because then you cannot move an element from left of k to right of k+1 or vice versa.
        // So we maintain the set of indices k where s[k] == 'L' and s[k+1] == 'R'.
        // For each adjacent value pair (i, i+1), we need that there is no such k in the interval between their positions.
        // We can maintain a segment tree or Fenwick tree over "bad" positions, and check if the sum in interval is 0.
        // Since n,q up to 2e5, we can use a Fenwick tree.

        vector<int> bad(n + 2, 0); // bad[k] = 1 if s[k]=='L' and s[k+1]=='R'
        for (int i = 1; i < n; ++i) {
            if (s[i] == 'L' && s[i + 1] == 'R') {
                bad[i] = 1;
            }
        }

        // Fenwick tree for prefix sums of bad
        vector<int> bit(n + 2, 0);
        auto add = [&](int idx, int val) {
            for (; idx <= n; idx += idx & -idx) {
                bit[idx] += val;
            }
        };
        auto sum = [&](int idx) {
            int res = 0;
            for (; idx > 0; idx -= idx & -idx) {
                res += bit[idx];
            }
            return res;
        };
        auto range_sum = [&](int l, int r) {
            if (l > r) return 0;
            return sum(r) - sum(l - 1);
        };

        for (int i = 1; i <= n; ++i) {
            if (bad[i]) add(i, 1);
        }

        // Count how many adjacent value pairs are "bad" (i.e., have at least one blocked pair between them)
        int bad_pairs = 0;
        vector<int> pair_bad(n, 0); // pair_bad[i] for pair (i, i+1), i from 1 to n-1
        for (int i = 1; i < n; ++i) {
            int l = pos[i], r = pos[i + 1];
            if (l > r) swap(l, r);
            // interval of indices between them: [l, r-1]
            if (range_sum(l, r - 1) > 0) {
                pair_bad[i] = 1;
                bad_pairs++;
            }
        }

        // Process queries
        while (q--) {
            int idx;
            cin >> idx;
            // Flip s[idx]
            // This affects bad[idx-1] and bad[idx] (if they exist)
            // We need to update bad array, BIT, and pair_bad for pairs that involve these positions.

            // Positions to check: idx-1 and idx
            vector<int> to_update;
            if (idx - 1 >= 1 && idx - 1 < n) to_update.push_back(idx - 1);
            if (idx >= 1 && idx < n) to_update.push_back(idx);

            // For each affected bad position, we will temporarily remove its effect, update it, then add back.
            // But we also need to know which value pairs are affected. A value pair (i, i+1) is affected if its interval
            // [min(pos[i], pos[i+1]), max(pos[i], pos[i+1]) - 1] contains any of the changed bad positions.
            // Since n is up to 2e5, we can just recompute pair_bad for all pairs? That would be O(n) per query, too slow.
            // Instead, we can maintain for each bad position k, the set of value pairs whose interval covers k.
            // But intervals are defined by pos[i] and pos[i+1]. We can precompute for each k, which pairs cover it?
            // There are n-1 pairs, each covers a contiguous range of indices. We could use a difference array approach
            // to add pair index to all k in its interval. Then for each k we have a list of pairs that cover it.
            // Since sum of lengths of intervals is O(n^2) worst-case, we need a better way.
            // Alternative: Instead of maintaining bad_pairs count, we can answer each query by checking if bad_pairs == 0.
            // To update bad_pairs efficiently, we need to know which pairs change status when a bad[k] flips.
            // We can precompute for each k the list of pairs (i) such that k is the ONLY bad position in their interval?
            // That's complicated.
            // Better approach: Use a segment tree over the array pos[1..n] to check if the whole permutation is "connected".
            // Actually, the condition "can sort" is equivalent to: for every i from 1 to n-1, the segment between pos[i] and pos[i+1]
            // contains no bad index. This is equivalent to: the maximum pos[1..i] and minimum pos[i+1..n]? Not exactly.
            // Another known solution: Maintain the set of indices i where the condition fails. We can use a segment tree
            // that stores for each i the number of bad positions in [pos[i], pos[i+1])? No.
            // Let's think differently: The array is sortable iff for every x from 1 to n-1, the subarray p[ min(pos[x], pos[x+1]) .. max(pos[x], pos[x+1]) ] is "sortable" locally? 
            // Actually, the necessary and sufficient condition is: For every i from 1 to n-1, there is no k in [pos[i], pos[i+1]-1] such that s[k]=='L' and s[k+1]=='R'.
            // We can maintain a segment tree over the array pos[1..n] that checks if the whole sequence is "valid".
            // Define an array a of length n-1: a[i] = 1 if the interval for pair (i,i+1) contains a bad position, else 0.
            // We need to know if sum a[i] == 0.
            // When a bad position k flips, which a[i] change? Those i where k is in [min(pos[i], pos[i+1]), max(pos[i], pos[i+1]) - 1].
            // This is equivalent to: i such that min(pos[i], pos[i+1]) <= k and max(pos[i], pos[i+1]) > k.
            // We can precompute for each k the list of i that satisfy this. Since each pair (i,i+1) contributes its interval length to the total sum of lengths, and total sum of interval lengths over all pairs is at most O(n^2) in worst case (e.g., pos = [1,2,3,...,n] then intervals are length 1 each, sum = n-1; worst case is when permutation is reversed, intervals are large, sum = O(n^2)). But n up to 2e5, O(n^2) is too much.
            // However, we can use a different observation: The condition is equivalent to: For every i from 1 to n-1, the maximum position of values 1..i is less than the minimum position of values i+1..n? Not exactly.
            // Let's re-read the hints: "Consider the parity of the distance between the elements of the permutation." Hmm.
            // Another known solution: The swaps allowed are adjacent swaps, but only in directions allowed by s. This is like having a graph where edges exist between i and i+1 if s[i]=='R' or s[i+1]=='L'? Actually, you can swap i and i+1 if s[i]=='R' (swap right) or s[i+1]=='L' (swap left). So an adjacent swap is allowed if the edge between i and i+1 is "open". The condition for being able to sort is that the graph is connected? No, because you can only move elements within connected components. The permutation is sortable iff for every i, the value i is in the same connected component as its target position i. Since the target positions are 1..n in order, this means that for every i, the position pos[i] must be in the same component as i. The components are intervals separated by "blocked" edges. An edge (k, k+1) is blocked if s[k]=='L' and s[k+1]=='R'. So the array is partitioned into blocks of consecutive indices where inside each block you can freely sort. The permutation is sortable iff for every block, the set of values that belong to that block (i.e., their target positions are in that block) is exactly the set of values currently in that block. In other words, for every block [L, R], the set {p[L], ..., p[R]} must equal {L, ..., R}. This is a known condition.
            // So we can maintain the blocks defined by bad positions. A bad position k means edge (k, k+1) is blocked, so it separates blocks.
            // Initially, blocks are intervals between bad positions. We need to check if for each block, the values in it are exactly the range of indices.
            // This is equivalent to: max(p[L..R]) == R and min(p[L..R]) == L.
            // We can maintain a segment tree over the array p to get range max and min.
            // When s[idx] flips, it affects bad[idx-1] and bad[idx]. This may merge or split blocks.
            // We can maintain the set of blocks, and for each block check if it's valid. We need to answer if all blocks are valid.
            // Since n,q up to 2e5, we can use a set of block boundaries and a segment tree for range max/min.
            // Let's implement this.

            // We'll flip s[idx] and update bad array.
            // But we need to efficiently update the set of blocks and check validity.
            // We can maintain a set of "blocked" edges: indices k where bad[k] = 1.
            // The blocks are intervals [start, end] where start is either 1 or a bad index + 1, and end is either n or a bad index.
            // We can maintain a data structure that stores for each block whether it is valid.
            // When a bad edge is added or removed, we split or merge blocks.
            // We can use a segment tree over the array p to get max and min in a range.
            // We'll also maintain a count of invalid blocks. If count == 0, answer YES, else NO.

            // Let's precompute initial blocks.
            // We'll need a segment tree for range max and min.
            // Since n up to 2e5, we can use iterative segment tree or recursive.

            // We'll implement iterative segment tree for max and min.
            int N = n;
            vector<int> seg_max(2 * N), seg_min(2 * N);
            for (int i = 0; i < N; ++i) {
                seg_max[N + i] = p[i + 1];
                seg_min[N + i] = p[i + 1];
            }
            for (int i = N - 1; i > 0; --i) {
                seg_max[i] = max(seg_max[i << 1], seg_max[i << 1 | 1]);
                seg_min[i] = min(seg_min[i << 1], seg_min[i << 1 | 1]);
            }
            auto query_max = [&](int l, int r) {
                l += N - 1; r += N - 1;
                int res = 0;
                while (l <= r) {
                    if (l & 1) res = max(res, seg_max[l++]);
                    if (!(r & 1)) res = max(res, seg_max[r--]);
                    l >>= 1; r >>= 1;
                }
                return res;
            };
            auto query_min = [&](int l, int r) {
                l += N - 1; r += N - 1;
                int res = n + 1;
                while (l <= r) {
                    if (l & 1) res = min(res, seg_min[l++]);
                    if (!(r & 1)) res = min(res, seg_min[r--]);
                    l >>= 1; r >>= 1;
                }
                return res;
            };

            // Set of blocked edges (k where bad[k]=1)
            set<int> blocked;
            for (int i = 1; i < n; ++i) {
                if (bad[i]) blocked.insert(i);
            }
            // Also insert 0 and n as sentinels? We'll handle boundaries.
            // Function to check if a block [L, R] is valid:
            auto is_valid = [&](int L, int R) -> bool {
                if (L > R) return true;
                int mn = query_min(L, R);
                int mx = query_max(L, R);
                return (mn == L && mx == R);
            };

            // Compute initial invalid blocks count
            int invalid_blocks = 0;
            // We'll iterate over blocks
            int prev = 1;
            for (int k : blocked) {
                int L = prev;
                int R = k;
                if (!is_valid(L, R)) invalid_blocks++;
                prev = k + 1;
            }
            // Last block
            int L = prev;
            int R = n;
            if (!is_valid(L, R)) invalid_blocks++;

            // Now process queries
            // We need to update s[idx] and bad, and blocked set, and invalid_blocks.
            // We'll do it per query.
            // But wait, the loop over q is already running. We need to embed this logic inside the query loop.
            // However, we already have the initial state. We'll just process each query by updating.
            // We'll need to flip s[idx] and update bad[idx-1] and bad[idx].
            // Let's do it step by step.

            // We'll write a lambda to update a specific bad position k (add or remove).
            // But careful: when we flip s[idx], it affects bad[idx-1] and bad[idx].
            // We'll compute the new bad values for those positions, compare with old, and update blocked set and invalid_blocks accordingly.

            // We'll do this inside the query loop. But we already have the initial state computed before the loop.
            // So we can just process each query by:
            // - flip s[idx]
            // - for each affected k in {idx-1, idx} (if 1<=k<n):
            //      new_bad = (s[k]=='L' && s[k+1]=='R')
            //      if new_bad != bad[k]:
            //          if new_bad == 1: add blocked edge k
            //          else: remove blocked edge k
            //          update invalid_blocks

            // We need a function to add a blocked edge k:
            // It splits a block [L, R] into [L, k] and [k+1, R].
            // We need to find the block containing k. Since blocked edges are between indices, the block before adding k is [L, R] where L is previous blocked edge + 1 (or 1) and R is next blocked edge (or n).
            // We can find L and R using the blocked set.
            // Then we remove the validity of [L, R], and add validities of [L, k] and [k+1, R].
            // Similarly for removal: merge [L, k] and [k+1, R] into [L, R].

            // We'll implement this.

            // But we are inside the query loop. We need to process each query.
            // Let's restructure: we'll move the initial state computation before the query loop, then inside the loop we update.

            // However, the code above already started the query loop. We need to adjust.
            // We'll break out of the current structure and rewrite cleanly.

            // Actually, we can just continue from where we are. We have initial invalid_blocks and blocked set.
            // Now we process queries:
            // For each query idx:
            //   flip s[idx]
            //   vector<int> affected = {idx-1, idx}
            //   for each k in affected:
            //      if k < 1 || k >= n) continue;
            //      bool new_bad = (s[k] == 'L' && s[k+1] == 'R');
            //      if (new_bad == bad[k]) continue;
            //      if (new_bad) {
            //          // add blocked edge k
            //          // find current block containing k: it is [L, R] where L = (prev blocked < k ? prev blocked + 1 : 1), R = (next blocked > k ? next blocked : n)
            //          auto it = blocked.lower_bound(k);
            //          int R = (it == blocked.end()) ? n : *it;
            //          int L = (it == blocked.begin()) ? 1 : (*prev(it) + 1);
            //          // remove validity of [L, R]
            //          if (!is_valid(L, R)) invalid_blocks--;
            //          // add validities of [L, k] and [k+1, R]
            //          if (!is_valid(L, k)) invalid_blocks++;
            //          if (!is_valid(k+1, R)) invalid_blocks++;
            //          blocked.insert(k);
            //          bad[k] = 1;
            //      } else {
            //          // remove blocked edge k
            //          // find the two blocks: [L, k] and [k+1, R]
            //          auto it = blocked.find(k);
            //          int L = (it == blocked.begin()) ? 1 : (*prev(it) + 1);
            //          int R = (next(it) == blocked.end()) ? n : *next(it);
            //          // remove validities of [L, k] and [k+1, R]
            //          if (!is_valid(L, k)) invalid_blocks--;
            //          if (!is_valid(k+1, R)) invalid_blocks--;
            //          // add validity of [L, R]
            //          if (!is_valid(L, R)) invalid_blocks++;
            //          blocked.erase(it);
            //          bad[k] = 0;
            //      }
            //   }
            //   output (invalid_blocks == 0 ? "YES" : "NO")

            // But wait: we need to be careful with the order of updates when both idx-1 and idx are affected.
            // They are independent because they are different edges. We can process them sequentially.

            // Let's implement this.

            // We'll need the query_max and query_min functions defined earlier.
            // We'll also need the is_valid lambda.

            // We'll now process queries.
            // But we are already inside the while(q--) loop from the initial code. We need to replace that loop.
            // Let's just write the new code from scratch to avoid confusion.
        }
    }
    return 0;
}
