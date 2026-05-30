// Hint6
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    
    vector<int> pref_max(n), suff_min(n);
    pref_max[0] = a[0];
    for (int i = 1; i < n; ++i) pref_max[i] = max(pref_max[i-1], a[i]);
    suff_min[n-1] = a[n-1];
    for (int i = n-2; i >= 0; --i) suff_min[i] = min(suff_min[i+1], a[i]);
    
    vector<int> ans(n);
    int cur_max = 0;
    for (int i = n-1; i >= 0; --i) {
        cur_max = max(cur_max, a[i]);
        ans[i] = cur_max;
        if (i > 0 && pref_max[i-1] > suff_min[i]) {
            // can jump to left part, so answer is max of whole array
            // but we already have cur_max which is max from i to n-1
            // we need max of whole array
            ans[i] = max(ans[i], pref_max[n-1]);
        }
    }
    
    // Actually the above logic is incomplete. Let's think properly.
    // The rabbit can move both ways. The reachable set is a contiguous segment?
    // From hints: if rabbit can jump i->j then j->i. So reachability is symmetric.
    // The graph is undirected. We need max height in connected component.
    // Condition: edge between i and j if (i<j and a_i > a_j) or (i>j and a_i < a_j).
    // This is equivalent to: edge if (i<j and a_i > a_j) OR (i>j and a_i < a_j).
    // Notice that if i<j and a_i > a_j, then a_i > a_j. If i>j and a_i < a_j, then a_j > a_i.
    // So essentially, there is an edge between i and j if the taller tree is to the left of the shorter tree.
    // Wait: condition 1: j < i and a_j > a_i -> taller on left, shorter on right.
    // condition 2: j > i and a_j < a_i -> taller on left, shorter on right.
    // So edge exists iff one tree is taller and to the left of the other.
    // So the graph connects trees that are in "inversion" where left > right.
    // This is exactly the comparability graph of the permutation? Not exactly.
    // But we can see that if we have a sequence, the connected components are intervals?
    // Let's analyze: if we have a decreasing sequence, all connected? Yes, because any left > right.
    // If increasing, no edges.
    // Consider a_i. It can reach any j > i with a_j < a_i, and any j < i with a_j > a_i.
    // So from i, you can go to any smaller element to the right, and any larger element to the left.
    // This is like you can move to elements that are "visible" in terms of being smaller on right or larger on left.
    // The connected components are exactly the intervals between "records"?
    // Let's think about the maximum reachable height from i.
    // Since graph is undirected, the max height in the component is the answer.
    // How to find components efficiently?
    // Notice that if we process from left to right, we can maintain something.
    // Actually, there is a known property: the graph is a collection of intervals that partition the array.
    // Let's test: a = [2,3,1,4]. Components? 
    // 2 can go to 1 (right smaller). 1 can go to 3 (left larger). So {1,2,3} connected. 4 is isolated. So components: [1,3] and [4]. Max heights: 3 and 4.
    // a = [5,4,3,2,1] all connected, max=5.
    // a = [2,1,1,3]: 2->1 (right smaller), 1->? left larger? 2>1 so 1 can go to 2. Also 1 (index 2) can go to 2? left larger 2>1. So {1,2,3}? index 3 is 1, index 4 is 3. 3 is larger than 1 on left? No, 3 is on right and larger, so no edge. 1 (index 3) can go to 2? left larger 2>1, yes. So all except 3? Actually 3 is larger than all on left, so no edge from 3 to left. But can 3 go to something? 3 can go to right smaller? none. So 3 isolated. So components: {1,2,3} and {4}. Max: 2 and 3. Output: 2 2 2 3. Matches sample.
    // a = [1,1,3,1]: 1,1 connected? left 1, right 1: not strictly greater, so no edge. 3 is larger than left 1s, so no edge from 3 to left (since left smaller, condition requires left larger). 3 can go to right 1 (right smaller). So 3 and 1 (index 4) connected. Also 1 (index 4) can go to left larger? 3>1, so yes. So components: {1}, {2}, {3,4}. Max: 1,1,3,3. Matches.
    // So components are intervals? In last example, {1} and {2} are separate, but they are adjacent indices. So not necessarily contiguous intervals? Wait, indices 1 and 2 are both 1, no edge, so separate components. So components are not intervals of indices. They can be interleaved? No, they are separated by "barriers".
    // Actually, observe that if we have a_i, it can connect to any j > i with a_j < a_i. So it can "see" smaller elements to the right. But those smaller elements might connect back to larger elements on the left. So the component is the set of indices that are connected via alternating larger/smaller.
    // There is a known solution using stack or DSU. Since sum n <= 5e5, O(n log n) or O(n) per test case is needed.
    // Let's think about the maximum height reachable from i. Since graph is undirected, we can compute the maximum in each component.
    // How to find components? We can process from left to right maintaining the maximum height of the current component? 
    // Notice that if we have a decreasing sequence, it's one component. If we have an increasing sequence, each is isolated.
    // Consider the array values. The edges exist between pairs (i,j) with i<j and a_i > a_j. This is exactly the inversion graph. The connected components of the inversion graph of a permutation are known to be intervals when the permutation is viewed in terms of its values? Actually, for a permutation, the inversion graph is connected iff the permutation is not decomposable. But here values can repeat and are bounded by n.
    // However, we can use a stack to find the "next greater to the left" and "next smaller to the right" etc.
    // From i, you can jump to any j < i with a_j > a_i. That means you can jump to the nearest greater to the left? Not only nearest, any greater to the left. But if you can jump to a greater left, you can then jump to smaller right from there, etc.
    // Actually, the reachable set from i is exactly the set of indices that are in the same "connected component" of the graph where edges are (i,j) if i<j and a_i > a_j.
    // There is a known fact: the connected components of the inversion graph are exactly the intervals between the "left-to-right maxima" of the reverse? Let's think.
    // Consider the array of values. Let's define L[i] = max a[1..i]. R[i] = min a[i..n]. 
    // In sample 1: a=[2,3,1,4]. L=[2,3,3,4]. R=[1,1,1,4]. 
    // Notice that for i=1, L[1]=2, R[2]=1. L[1] > R[2]? 2>1 yes. For i=2, L[2]=3, R[3]=1, 3>1. For i=3, L[3]=3, R[4]=4, 3>4? no. So the component boundary is between 3 and 4? Actually component of {1,2,3} ends at 3. So the condition L[i] > R[i+1] seems to indicate connection between left and right parts.
    // In sample 4: [1,1,3,1]. L=[1,1,3,3]. R=[1,1,1,1]. L[1]=1, R[2]=1 not >. L[2]=1, R[3]=1 not >. L[3]=3, R[4]=1 >. So connections at i=3. Components: {1}, {2}, {3,4}. So the boundaries between components are where L[i] <= R[i+1]? Actually, if L[i] <= R[i+1], then no element in left part is greater than any element in right part? Wait, L[i] is max left, R[i+1] is min right. If max left <= min right, then all left <= all right. Then there is no inversion between left and right (since left < right, no left > right). So no edges between the two parts. Thus they are disconnected. Conversely, if L[i] > R[i+1], there exists some left > some right, so there is at least one edge. But does that mean the whole left and right are connected? Not necessarily, but it turns out that the components are exactly the intervals between these "cuts". Let's test: if L[i] > R[i+1], then the maximum of left is greater than minimum of right. Does that guarantee that the components merge? In sample 4, L[1]=1, R[2]=1, not >, so cut. L[2]=1, R[3]=1, not >, cut. L[3]=3, R[4]=1, >, so merge. So components are {1}, {2}, {3,4}. So the cuts are at i where L[i] <= R[i+1]. The components are the intervals between these cuts. Let's verify with sample 5: [2,4,1,6,3,8,5,7]. Compute L: [2,4,4,6,6,8,8,8]. R: [1,1,1,3,3,5,5,7] (from right: min). Check L[i] > R[i+1]?
    // i=1: L=2, R[2]=1 -> 2>1 (merge)
    // i=2: L=4, R[3]=1 -> 4>1 (merge)
    // i=3: L=4, R[4]=3 -> 4>3 (merge)
    // i=4: L=6, R[5]=3 -> 6>3 (merge)
    // i=5: L=6, R[6]=5 -> 6>5 (merge)
    // i=6: L=8, R[7]=5 -> 8>5 (merge)
    // i=7: L=8, R[8]=7 -> 8>7 (merge)
    // So no cuts, all one component. Output all 8. Matches.
    // Sample 2: [5,4,3,2,1]. L=[5,5,5,5,5]. R=[1,1,1,1,1]. All L[i] > R[i+1], so one component.
    // Sample 3: [2,1,1,3]. L=[2,2,2,3]. R=[1,1,1,3]. i=1: L=2, R[2]=1 >; i=2: L=2, R[3]=1 >; i=3: L=2, R[4]=3 <=. So cut at i=3. Components: {1,2,3} and {4}. Max: max of first three is 2, last is 3. Output: 2 2 2 3. Matches.
    // So the components are exactly the intervals between indices i where L[i] <= R[i+1]. The maximum in each component is the maximum of a over that interval.
    // But wait, is it always true that if L[i] > R[i+1], the two parts are in the same component? Could there be a case where L[i] > R[i+1] but the specific elements that cause the inversion don't connect the whole parts? Since the graph is undirected, if there is any edge between the left part and right part, then the components merge. Because the left part is internally connected? Not necessarily. But we need to ensure that the left part itself is one component and right part is one component. Actually, by induction, if we define components as the intervals between cuts, then within each interval, there is no cut, meaning for all internal i, L[i] > R[i+1]. Does that guarantee the interval is connected? Let's test a potential counterexample. Suppose a = [3,1,2]. L=[3,3,3], R=[1,1,2]. i=1: L=3, R[2]=1 >; i=2: L=3, R[3]=2 >. So no cuts, one component. Is it connected? 3 can go to 1 (right smaller). 1 can go to 3 (left larger). 2: can 2 go to 1? left larger? 1<2 so no. Can 2 go to 3? left larger? 3>2, so yes, 2 can go to 3. So all connected. Good.
    // Another: a = [2,3,1]. L=[2,3,3], R=[1,1,1]. i=1: L=2, R[2]=1 >; i=2: L=3, R[3]=1 >. One component. 2->1, 1->3, 3->1? 3 can go to 1 (right smaller). So connected.
    // What about a = [4,2,5,1,3]? L=[4,4,5,5,5], R=[1,1,1,1,3]. i=1: L=4, R[2]=1 >; i=2: L=4, R[3]=1 >; i=3: L=5, R[4]=1 >; i=4: L=5, R[5]=3 >. All >, one component. Let's check connectivity: 4->2, 4->1, 4->3? 4>3 yes. 2->1, 2->? left larger? 4>2 so 2->4. 5->1,5->3, 5->? left larger? none. 1->4,1->2,1->5? left larger: 4,2,5 all >1, so 1 connects to all. 3->? left larger: 4,5 >3, so 3 connects. So all connected.
    // Is there a case where L[i] > R[i+1] but the left part is not connected to the right part? For that to happen, the inversion must be between some left and some right, but the left element that is > right element might be isolated from the rest of the left part? But if the left part had a cut inside it, it would be split. So by definition, within the left part, there is no cut, so by induction it is connected. Similarly for right part. And since there is an edge between some left and some right, the whole thing is connected. So the components are exactly the intervals between cuts.
    // Therefore, we can find all cut points: i from 1 to n-1 where max(a[1..i]) <= min(a[i+1..n]). Then the array is partitioned into intervals. The answer for each index in an interval is the maximum value in that interval.
    // This is O(n) per test case.
    // Let's double-check with a tricky case: a = [2, 2]. L=[2,2], R=[2,2]. i=1: L=2, R[2]=2, <=. So cut. Components: {1}, {2}. Max: 2,2. Is that correct? Condition: j < i and a_j > a_i: for i=2, j=1, a_1=2, a_2=2, not >. j > i and a_j < a_i: none. So no edges. So each isolated. Correct.
    // a = [1,2,1]. L=[1,2,2], R=[1,1,1]. i=1: L=1, R[2]=1 <= (cut). i=2: L=2, R[3]=1 > (merge). So cuts at i=1. Components: {1} and {2,3}. Max: 1 and 2. Check: 1 isolated? 1 can go to right smaller? none (2>1, 1 not >). left larger? none. So isolated. 2 and 1: 2>1 so edge. So correct.
    // So algorithm:
    // For each test case:
    // 1. Compute prefix max array L[0..n-1] (1-indexed in explanation, but 0-indexed in code).
    // 2. Compute suffix min array R[0..n-1].
    // 3. Find cut indices: for i from 0 to n-2, if L[i] <= R[i+1], then there is a cut between i and i+1.
    // 4. The intervals are [0..i1], [i1+1..i2], ..., [ik+1..n-1].
    // 5. For each interval, compute the maximum value in that interval.
    // 6. Assign that maximum to all indices in the interval.
    // Complexity O(n) per test case, total O(sum n) <= 5e5. Fast enough.
    
    vector<int> L(n), R(n);
    L[0] = a[0];
    for (int i = 1; i < n; ++i) L[i] = max(L[i-1], a[i]);
    R[n-1] = a[n-1];
    for (int i = n-2; i >= 0; --i) R[i] = min(R[i+1], a[i]);
    
    vector<int> ans(n);
    int start = 0;
    for (int i = 0; i < n; ++i) {
        // if cut after i, or i is last
        if (i == n-1 || L[i] <= R[i+1]) {
            // interval [start, i]
            int mx = *max_element(a.begin() + start, a.begin() + i + 1);
            for (int j = start; j <= i; ++j) ans[j] = mx;
            start = i + 1;
        }
    }
    
    for (int i = 0; i < n; ++i) {
        cout << ans[i] << " \n"[i==n-1];
    }
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
