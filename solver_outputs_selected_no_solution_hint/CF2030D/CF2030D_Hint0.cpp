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

        // pos[x] = position of value x in p
        vector<int> pos(n + 1);
        for (int i = 1; i <= n; ++i) {
            pos[p[i]] = i;
        }

        // We maintain a set of "bad" indices i (1 <= i <= n-1)
        // such that the segment [1..i] is not sortable with the given L/R constraints.
        // Condition: max_{j=1..i} pos[j] > i, but we need to check if the segment is "blocked".
        // Actually, a segment [1..i] is sortable iff for all j in [1..i], pos[j] <= i
        // AND there is no "barrier" inside the segment that prevents moving elements across.
        // The barriers are positions k where s[k] = 'R' and s[k+1] = 'L' (i.e., cannot move right from k to k+1).
        // More precisely, an element can move from a to b (a < b) only if there is no index k in [a, b-1] with s[k]='R' and s[k+1]='L'.
        // So the permutation is sortable iff for every i from 1 to n-1, the prefix [1..i] contains all values 1..i,
        // OR there is a "bridge" (i.e., not a barrier) that allows moving elements across the boundary i.
        // Equivalent condition: For every i, if max_{j=1..i} pos[j] > i, then it must be possible to move the missing small elements from right to left.
        // This is possible iff there is no barrier inside the range [i, max_pos]? Actually, we need to check if the set of values 1..i are all within some connected component of the "allowed swaps" graph.
        // The allowed swaps graph has edges between i and i+1 if s[i]='R' or s[i+1]='L'. The graph is a set of segments (intervals) where inside each segment you can arbitrarily permute.
        // The permutation is sortable iff for every i, the values 1..i all lie within the same connected component that also contains the positions 1..i? Wait.
        // Actually, if we consider the graph where we can swap adjacent elements if allowed, the permutation is sortable iff for every i, the set of positions of values 1..i is exactly the set of positions 1..i? No, because we can move elements within a connected component.
        // The correct condition: The permutation can be sorted iff for every i from 1 to n, the positions of values 1..i form a contiguous segment in the "allowed swaps" graph? Not exactly.
        // Let's think: We can sort the array iff we can move each value x to position x. This is possible iff for every x, position x and pos[x] are in the same connected component of the allowed swaps graph.
        // But since the allowed swaps graph is just a set of intervals (because edges are only between adjacent indices), the connected components are contiguous intervals.
        // Let the components be intervals [L_j, R_j]. Within each component, we can arbitrarily permute the elements. So the array is sortable iff for every component, the set of values that start in that component is exactly the set of positions of that component.
        // That is, for each component [L, R], the multiset of values initially in that component must be {L, L+1, ..., R}.
        // Equivalently, for every i, the number of values <= i that are in positions <= i must be i? Not exactly, because components can cross i.
        // Better: For each i from 1 to n-1, if i and i+1 are in different components (i.e., s[i]='R' and s[i+1]='L'? Wait, edge exists if s[i]='R' OR s[i+1]='L'. So the only way there is NO edge between i and i+1 is if s[i]='L' and s[i+1]='R'. That is a barrier.
        // So components are separated by barriers: indices i such that s[i]='L' and s[i+1]='R'.
        // The array is sortable iff for every barrier at i, the prefix [1..i] contains exactly the values 1..i.
        // Because if there is a barrier between i and i+1, no element can cross from left to right or right to left. So the left component must contain exactly the smallest i values.
        // So condition: For every i from 1 to n-1, if s[i]=='L' and s[i+1]=='R', then max_{j=1..i} pos[j] == i (i.e., all values 1..i are in positions 1..i).
        // Let's verify with examples. In sample 1: n=5, p=[1,4,2,5,3], s=RLRLL. Barriers: check i where s[i]=='L' and s[i+1]=='R'.
        // s[1]='R', s[2]='L' -> no barrier. s[2]='L', s[3]='R' -> barrier at i=2. s[3]='R', s[4]='L' -> no. s[4]='L', s[5]='L' -> no.
        // Barrier at i=2. Check if values 1..2 are in positions 1..2. pos[1]=1, pos[2]=3. max pos = 3 > 2, so not sortable. Indeed initial s gives NO? But sample says after first query (change index 2) s becomes RRRLL, then YES. Let's check barriers for RRRLL: s[1]='R', s[2]='R' -> no. s[2]='R', s[3]='R' -> no. s[3]='R', s[4]='L' -> no. s[4]='L', s[5]='L' -> no. No barriers, so always sortable? But wait, s[1]='R' and s[n]='L' are guaranteed, but barriers can exist. If no barriers, the whole array is one component, so any permutation is sortable. So YES. Matches.
        // After second query (change index 4): s becomes RRLRL? Wait, initial s=RLRLL. Query 2 changes index 2: RLRLL -> RRRLL. Query 4 changes index 4: RRRLL -> RRRRL. Query 3 changes index 3: RRRRL -> RRLRL. Let's check barriers for RRLRL: s[1]='R', s[2]='R' -> no. s[2]='R', s[3]='L' -> no. s[3]='L', s[4]='R' -> barrier at i=3. s[4]='R', s[5]='L' -> no. Barrier at i=3. Check values 1..3: pos[1]=1, pos[2]=3, pos[3]=5. max pos = 5 > 3, so NO. Sample output after third query is NO. Matches.
        // So condition is correct.

        // We need to maintain the set of barriers and check if for each barrier i, max_{j=1..i} pos[j] == i.
        // Equivalently, we can maintain the maximum position of values 1..i for each i, and check if any barrier violates the condition.
        // Since n and q sum to 2e5, we can precompute prefix maximums of pos[1..i] and then maintain a data structure (e.g., set or segment tree) of "bad" barriers.
        // A barrier i is bad if pref_max[i] > i.
        // We need to answer if there is any bad barrier. If none, YES; else NO.

        // Precompute pref_max[i] = max(pos[1..i]).
        vector<int> pref_max(n + 1, 0);
        for (int i = 1; i <= n; ++i) {
            pref_max[i] = max(pref_max[i-1], pos[i]);
        }

        // Identify initial barriers and maintain count of bad barriers.
        // We can use a set of bad barriers, or just a counter.
        // Since queries only flip one character, we can update the barrier status at i-1 and i (because changing s[i] affects barriers at i-1 and i).
        // A barrier exists at index k (1 <= k <= n-1) if s[k]=='L' and s[k+1]=='R'.
        // We'll maintain a boolean array is_bad[k] for k=1..n-1, and a count of bad barriers.
        vector<bool> is_barrier(n + 1, false); // is_barrier[k] true if barrier at k
        vector<bool> is_bad(n + 1, false);
        int bad_count = 0;

        auto update_barrier = [&](int k) {
            if (k < 1 || k >= n) return;
            bool was_bad = is_bad[k];
            bool now_barrier = (s[k] == 'L' && s[k+1] == 'R');
            bool now_bad = now_barrier && (pref_max[k] > k);
            if (was_bad != now_bad) {
                if (now_bad) bad_count++;
                else bad_count--;
                is_bad[k] = now_bad;
            }
            is_barrier[k] = now_barrier;
        };

        // Initialize barriers
        for (int k = 1; k < n; ++k) {
            update_barrier(k);
        }

        // Process queries
        while (q--) {
            int idx;
            cin >> idx;
            // flip s[idx]
            s[idx] = (s[idx] == 'L') ? 'R' : 'L';
            // barriers at idx-1 and idx may change
            update_barrier(idx - 1);
            update_barrier(idx);
            // Note: pref_max doesn't change because p is reset each query, and pos is fixed.
            // So we just check bad_count.
            if (bad_count == 0) {
                cout << "YES\n";
            } else {
                cout << "NO\n";
            }
        }
    }
    return 0;
}
