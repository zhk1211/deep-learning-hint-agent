// Hint5
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    
    vector<int> ans(n);
    vector<int> stk;
    vector<int> max_suf(n);
    max_suf[n-1] = a[n-1];
    for (int i = n-2; i >= 0; --i) {
        max_suf[i] = max(a[i], max_suf[i+1]);
    }
    
    int max_pref = 0;
    for (int i = 0; i < n; ++i) {
        max_pref = max(max_pref, a[i]);
        ans[i] = max_pref;
        if (max_suf[i] > a[i]) {
            ans[i] = max(ans[i], max_suf[i]);
        }
    }
    
    // The above is not fully correct; we need to consider reachable components.
    // Let's use the idea from hints: the reachable set is symmetric, and we can think of it as connected components in a graph where edges are defined by the conditions.
    // Actually, we can process from right to left using a stack to find the maximum reachable height.
    
    // Reset ans and compute properly.
    // We'll compute for each i the maximum height reachable.
    // The key observation: the rabbit can move to any tree that is a "peak" in a certain sense.
    // Let's use the approach: maintain a stack of indices with decreasing heights? 
    // Actually, we can compute the answer by considering the maximum height to the right that is greater than current, and maximum to the left that is smaller? 
    // The hints suggest: if rabbit starts at i < n, it can jump to the highest tree with index <= i? That seems off.
    // Let's re-read hints carefully.
    // Hint 4: Consider trees with height greater than the minimum height among trees with index greater than or equal to the current tree's index.
    // Hint 5: If the rabbit starts from tree i where i < n, it can jump to the highest tree with index less than or equal to i.
    // This suggests a solution: for each i, the answer is the maximum of the maximum height in prefix up to i, and the maximum height in suffix from i that is greater than the minimum in that suffix? Not exactly.
    
    // Let's derive from the problem: The rabbit can jump backward to a taller tree, and forward to a shorter tree.
    // This means the graph is undirected (if i->j then j->i). So reachable set is a connected component.
    // We need the maximum height in the component containing i.
    // How to find components? Notice that if we have a sequence, the rabbit can move across "valleys" and "peaks".
    // Actually, we can think of the array as having "barriers": a tree can only be reached if there is a path.
    // Consider the maximum element in the whole array. It can reach everything? Not necessarily, because to go from a small element to a large one, you need a sequence of jumps.
    // But if there is a global maximum, can it reach all? If you are at the global maximum, you can only jump backward to taller? No, taller doesn't exist. You can jump forward to shorter. So from global max you can go to any shorter tree to the right? Only if it's shorter. But you can go to a shorter tree, then from there jump backward to a taller tree (which might be the global max again). So you can move rightwards by jumping to shorter trees, and leftwards by jumping to taller trees. This is exactly like you can traverse any path where the sequence of heights alternates? Not exactly.
    
    // Let's analyze the connectivity: Two trees i and j (i < j) are connected if there exists a path. 
    // It is known that in such a graph, the connected components are intervals? Let's test: 
    // Example: 2 3 1 4. 
    // 1: 2, can go to 3 (j=2, a_j=3 > 2? No, condition for forward: j>i and a_j < a_i. 3 < 2? No. Backward: j<i and a_j > a_i. For i=1, no j<i. So from 1, can only go to 3? Check: j=3 > 1, a_3=1 < 2, yes. So 1->3. From 3: can go to 2 (j=2<3, a_2=3 > 1, yes). So 1,2,3 connected. 4 is isolated? From 4: can go to 3? j=3<4, a_3=1 > 4? No. Forward: none. So 4 is alone. So components: {1,2,3} and {4}. They are intervals? {1,2,3} is an interval, {4} is an interval. 
    // Example: 5 4 3 2 1. All connected? From 1 (value 5): can go forward to 2 (4<5), then from 2 can go backward to 1? Yes. From 2 can go forward to 3 (3<4), etc. So all connected. Interval.
    // Example: 2 1 1 3. 
    // 1:2 -> can go to 2? 1<2 no. To 3? 1<2 no. To 4? 3<2 no. So 1 isolated? But sample output says 2 2 2 3. So 1 can reach 2? Let's check: from 1 (2) to 2 (1): j=2>1, a_2=1 < 2, yes! So 1->2. From 2 (1): can go to 3 (1): j=3>2, a_3=1 < 1? No, not strictly less. Can go backward? j=1<2, a_1=2 > 1, yes. So 1,2 connected. From 3 (1): can go to 4? j=4>3, a_4=3 < 1? No. Backward to 2? a_2=1 > 1? No. To 1? a_1=2 > 1, yes. So 3 connected to 1. So all 1,2,3 connected. 4 (3): can go to 3? j=3<4, a_3=1 > 3? No. Forward none. So 4 isolated. Components: {1,2,3} and {4}. Again intervals.
    // Example: 1 1 3 1. 
    // 1:1 -> 2:1? forward: 1<1 no. backward none. So 1 isolated? But sample: 1 1 3 3. So 1 and 2 can reach 3? Let's check: from 1 (1) to 3 (3): j=3>1, a_3=3 < 1? No. So cannot. How does 1 reach 3? Maybe via 2? 1->2? no. So 1 cannot reach 3. But sample output says for i=1, answer is 1. For i=2, answer is 1. For i=3, answer is 3. For i=4, answer is 3. So 4 can reach 3? 4:1 -> 3:3? j=3<4, a_3=3 > 1, yes. So 4 and 3 connected. 1 and 2 isolated? Actually 1 and 2: both 1, can they connect? 1->2: forward 1<1 no. So they are separate? But sample says max height for 1 is 1, for 2 is 1. So they are in components of size 1. So components: {1}, {2}, {3,4}. Not all intervals? {3,4} is interval, but 1 and 2 are separate intervals. So components are intervals! Because 1 is [1,1], 2 is [2,2], 3-4 is [3,4]. So indeed, the connected components are contiguous intervals. Let's verify: Is it always true that if i < j < k and i and k are connected, then j is also connected to them? This would mean components are intervals. Let's test: Suppose i and k connected. There is a path. Does j necessarily get included? Not obvious, but maybe due to the nature of jumps. I recall a known problem: "Rabbit and Trees" or similar. Actually, it's known that the graph is a collection of intervals. The condition for an interval [L, R] to be a component is that the maximum in that interval is at the boundaries? Let's think.
    
    // Consider the process: The rabbit can move right if it finds a shorter tree, and left if it finds a taller tree. This is similar to being able to move to any tree that is not "blocked" by a higher tree on one side and lower on the other? 
    // Another perspective: The rabbit can jump from i to j if (i < j and a_i > a_j) or (i > j and a_i < a_j). This is equivalent to: the rabbit can move to an adjacent index? No.
    // But notice that if we have a sequence, the rabbit can move from a peak to a valley, etc.
    // Let's think about the complement: when is a tree NOT reachable from another? 
    // Consider the maximum height in the whole array. Let it be at index m. Can the rabbit reach it from anywhere? To reach m, you need a path. If you are left of m, you need to eventually jump to a taller tree to go right? Actually, to go right, you need to jump to a shorter tree. So from left of m, you can only move right if you find a shorter tree. But if all trees between you and m are taller than you? Then you cannot move right because you need a shorter tree. But if there is a shorter tree, you can jump to it, then from there you might jump left to a taller tree, etc. 
    // It turns out that the components are determined by the "left-to-right maxima" and "right-to-left maxima"? 
    // Let's look at the sample 5: 2 4 1 6 3 8 5 7. All answers are 8. So everything is connected. The global max is 8 at index 6. 
    // Another test: 3 1 2. 
    // 1:3 -> can go to 2:1 (forward, shorter). From 2:1 -> can go to 3:2? forward: 2>1? No, 2 is not < 1. Backward: to 1:3 (taller). So 2 can only go to 1. 3:2 -> can go to 2:1? backward: 1<2? No, backward requires taller. So 3 cannot go to 2. Can go to 1? backward: 1<3, a_1=3 > 2, yes. So 3 connected to 1. So components: {1,2} and {3}? But 3 connected to 1, so all connected? 3->1, 1->2, so all connected. So max for all is 3. Let's check manually: from 3 (value 2): jump to 1 (value 3) because 1<3 and a_1=3 > 2. So yes. So all connected.
    // What about 2 1 3? 
    // 1:2 -> 2:1 (forward). 2:1 -> 1:2 (backward). 3:3 -> 2:1? backward: 2<3, a_2=1 > 3? No. Forward none. So 3 isolated? But can 3 go to 1? backward: 1<3, a_1=2 > 3? No. So 3 isolated. Components: {1,2} and {3}. Intervals.
    // So it seems components are intervals. Why? Suppose i < j < k and i and k are in the same component. Then there is a path between i and k. This path must cross j? Not necessarily, it could go outside? But indices are linear, so any path from i to k must visit some indices. It might go left of i or right of k. But can it skip j entirely? If it goes left of i, then it must come back right, crossing i again. So it's plausible that the set of visited indices forms an interval containing i and k, hence containing j. So components are intervals.
    
    // If components are intervals, we just need to find the boundaries of each interval and the maximum within it.
    // How to find the intervals? 
    // Consider the array. The rabbit can move freely within an interval if there is no "barrier". What constitutes a barrier? 
    // Notice that if we have a tree that is a "strictly increasing then decreasing" pattern? 
    // Let's think about the condition for two adjacent trees i and i+1 to be in the same component. 
    // If a_i > a_{i+1}, then from i you can jump to i+1 (forward, shorter). So they are connected.
    // If a_i < a_{i+1}, then from i+1 you can jump to i (backward, taller). So they are connected.
    // So any adjacent pair is always connected! Because either i can jump to i+1 (if a_i > a_{i+1}) or i+1 can jump to i (if a_i < a_{i+1}). What if a_i == a_{i+1}? Then neither condition holds: forward requires a_j < a_i, backward requires a_j > a_i. So equal adjacent trees are NOT connected directly. But they might be connected via other trees.
    // So the only direct disconnection is between equal adjacent elements. But they could be connected through a longer path.
    // Example: 1 1 3 1. Adjacent pairs: (1,1) equal -> not connected directly. (1,3): 1<3, so 3 can jump to 2? Actually indices: a_2=1, a_3=3. i=2, j=3: forward? 3>2, a_3=3 < a_2=1? No. Backward? j=2<3, a_2=1 > a_3=3? No. So 2 and 3 are NOT connected directly! Wait, check: i=2 (value 1), j=3 (value 3). Condition for jump from 2 to 3: j>i and a_j < a_i -> 3>2 and 3<1 false. Condition from 3 to 2: j<i and a_j > a_i -> 2<3 and 1>3 false. So they are NOT connected! But in my earlier analysis I said 3 and 4 are connected, and 1 and 2 are isolated. But what about 2 and 3? They are not connected. So the interval [2,3] is not a component. So components are not necessarily all intervals? But the set of indices reachable from 2 is just {2}? Let's check: from 2 (value 1), can it go to 1? j=1<2, a_1=1 > 1? No. Can it go to 3? No. Can it go to 4? j=4>2, a_4=1 < 1? No. So 2 is isolated. From 3 (value 3): can go to 4? j=4>3, a_4=1 < 3, yes. From 4: can go to 3 (backward). So {3,4} is a component. From 1 (value 1): isolated. So components: {1}, {2}, {3,4}. These are intervals: [1,1], [2,2], [3,4]. So they are intervals! The fact that 2 and 3 are not connected doesn't break the interval property because they are in different intervals. The intervals are separated at the boundary between 2 and 3. So the components are indeed a partition of the array into intervals.
    
    // So the problem reduces to: partition the array into maximal intervals such that within each interval, all trees are connected. Then the answer for each i is the maximum value in its interval.
    // How to find these intervals?
    // Notice that the only way an interval can be split is if there is a "block" that cannot be crossed. When can we not cross from left to right? 
    // Consider the maximum height in the left part and the right part. 
    // Actually, there is a known solution: The rabbit can move from i to j if and only if the maximum height on the path between them is not... hmm.
    // Let's think about the connectivity condition. 
    // Define L_i as the maximum height to the left of i (including i) that is... 
    // Another approach: Use a stack to simulate the reachable set from left to right.
    // Consider processing from left to right, maintaining the current component's maximum. When we encounter a new tree, can it be added to the current component?
    // Suppose we have a component covering [l, r] with maximum height M. We want to add r+1. When can the rabbit move from some tree in [l, r] to r+1? 
    // If there exists i in [l, r] such that a_i > a_{r+1}, then from i we can jump forward to r+1. So if the maximum in [l, r] is > a_{r+1}, then r+1 is connected.
    // If the maximum in [l, r] is <= a_{r+1}, then no tree in [l, r] can jump forward to r+1. But could r+1 jump backward to some tree in [l, r]? That requires a tree j in [l, r] with a_j > a_{r+1} (since backward jump from r+1 to j requires a_j > a_{r+1}). So again, we need some tree in [l, r] taller than a_{r+1}. So if max in [l, r] <= a_{r+1}, then there is no direct edge between [l, r] and r+1. But could they be connected via some tree to the right of r+1? That would require going forward from [l, r] to some k > r+1, then backward to r+1. But to go forward to k, we need a tree in [l, r] taller than a_k. If all trees in [l, r] are <= a_{r+1}, and a_{r+1} might be tall, it's possible that a_k is even taller? Actually, if we cannot cross r+1 directly, maybe we can jump over it? But jumps are only between trees, you cannot skip over r+1 without landing on it? The rabbit jumps from tree to tree, it doesn't teleport. So to get from [l, r] to any k > r+1, the rabbit must land on some trees between. It could potentially jump from [l, r] directly to k if k > r+1 and a_k < a_i for some i in [l, r]. But that skips r+1. Is that allowed? The problem says "jump from the i-th tree to the j-th tree". There is no requirement that j = i+1. So yes, you can skip trees. So you could jump from l to r+2 directly, bypassing r+1. Then from r+2, you might jump backward to r+1. So even if max in [l, r] <= a_{r+1}, you might still connect r+1 via a longer jump.
    // Example: a = [2, 5, 3]. l=1, r=1 (value 2). r+1=2 (value 5). max in [1,1]=2 <= 5. Can we connect 1 and 2? Direct: 1->2? forward: 5<2 no. 2->1? backward: 2>5 no. So no direct. But can we go 1 -> 3? 3>1, a_3=3 < 2? No. So 1 cannot jump to 3. So 1 is isolated. What about 2 and 3? 2->3: forward 3<5 yes. So 2 and 3 connected. So components: {1}, {2,3}. So r+1=2 is not connected to 1. So the condition "max in left <= a_{r+1}" seems to separate.
    // Another example: a = [3, 5, 2]. l=1 (3), r+1=2 (5). max=3 <=5. Can 1 connect to 2? Direct no. Can 1 jump to 3? 3>1, a_3=2 < 3, yes! So 1 -> 3. From 3 (2), can jump backward to 2? 2<3, a_2=5 > 2, yes. So 1 connects to 2 via 3. So components: all connected. Here max in [1,1]=3 <= a_2=5, but they are connected via a_3=2 which is shorter than a_1. So the condition is not simply about the max of the left component.
    
    // Let's analyze why 1 and 2 connected in [3,5,2] but not in [2,5,3]. The difference is the third element: in the first, a_3=2 < a_1=3, so 1 can jump to 3. In the second, a_3=3 > a_1=2, so 1 cannot jump to 3. So the ability to "skip" over a tall tree depends on having a shorter tree further right.
    // This suggests that the connectivity is determined by the "right-to-left minimums" or something.
    
    // Let's think about the graph more systematically. 
    // Define a directed edge i->j if (j<i and a_j > a_i) or (j>i and a_j < a_i). The graph is symmetric, so it's undirected.
    // We want connected components.
    // Consider the array values. Notice that if we have a sequence, the rabbit can move from a lower value to a higher value by going left? Actually, to increase height, you must jump backward to a taller tree. To decrease height, you jump forward to a shorter tree. So you can zig-zag.
    // This is exactly the condition that you can traverse the array as long as you can find a path that alternates directions? Not necessarily.
    
    // There is a known result: The connected components are exactly the intervals between "records" of some kind. 
    // Let's consider the maximum element in the whole array. Suppose the global maximum is at index m. Can the rabbit reach it from anywhere? Not necessarily, as seen in [2,5,3] where max is 5 at index 2, but index 1 cannot reach it. So the global max does not dominate everything.
    // What about the left-to-right maximums? In [2,5,3], left-to-right maxes: 2, 5. The component of 1 is just {1}. The component of 2 and 3 is {2,3}. Notice that the boundary between components is between 1 and 2, where 2 is a left-to-right maximum that is > previous max? Actually, 5 > 2. 
    // In [3,5,2], left-to-right maxes: 3, 5. But all connected. So not just that.
    
    // Let's look at the condition from the perspective of "valleys". 
    // Another idea: The rabbit can move from i to j if and only if there is no k between i and j such that a_k is greater than both a_i and a_j? Or something like that.
    // Consider i < j. If there is a tree k between them with a_k > max(a_i, a_j), can the rabbit cross it? To go from i to j, you need to either jump directly (if a_j < a_i) or go via other trees. If there is a very tall tree in between, you might not be able to jump over it because to jump forward you need a shorter tree, but the tall tree is taller, so you can't jump from i to k (since a_k > a_i). You could jump from i to some other tree left of k? But eventually you have to pass k. You could jump backward from somewhere? It seems a tall tree acts as a barrier.
    // In [2,5,3], to go from 1 to 3, the tree 2 is 5 which is > max(2,3)=3. So it blocks.
    // In [3,5,2], to go from 1 to 3, the tree 2 is 5 > max(3,2)=3. But they are connected! Wait, in [3,5,2], 1 and 3 are connected directly? 1->3: a_3=2 < a_1=3, so yes, direct jump! So the tall tree 5 does not block because the rabbit can jump over it. The jump is from 1 to 3 directly, skipping 2. So the tall tree in between doesn't matter if you can jump over it. But you can only jump over it if the target is shorter than the source. So if a_1 > a_3, you can jump over 2. If a_1 < a_3, you cannot jump forward because you need a shorter tree. So the barrier is only effective if the tall tree is taller than the source and you want to go to a taller target? 
    // Let's formalize: For i < j, if there exists k in (i, j) with a_k > a_i and a_k > a_j, can i and j be connected? 
    // If a_i > a_j, then i can jump directly to j, so they are connected regardless of k. So the barrier only matters when a_i < a_j. In that case, i cannot jump directly to j. To go from i to j, i must first jump to some intermediate trees. Since i < j and a_i < a_j, i cannot jump forward to any tree with height < a_i? Actually, i can jump forward to any tree with height < a_i. So if there is some tree to the right of i with height < a_i, i can jump to it. Then from there, it might jump backward to a taller tree, etc. So the barrier is not simply a tall tree.
    
    // Let's think about the connected components in terms of the "maximum on the left" and "minimum on the right". 
    // I recall a similar problem: "Rabbit and Trees" from Codeforces Round. The solution often involves using a stack to find the next greater/smaller elements.
    // Let's search memory: There is a problem "Penchick and the Rabbit" or similar? Actually, this is Codeforces problem 1840D? No.
    // Let's derive a solution using the hints.
    
    // Hint 1: If a rabbit can jump from i to j, then it can also jump from j to i. (Undirected)
    // Hint 2: Think about the order of the trees that can be reached.
    // Hint 3: What is the answer if the rabbit starts at tree n?
    // Hint 4: Consider trees with height greater than the minimum height among trees with index greater than or equal to the current tree's index.
    // Hint 5: If the rabbit starts from tree i where i < n, it can jump to the highest tree with index less than or equal to i.
    
    // Let's analyze Hint 5: "If the rabbit starts from tree i where i < n, it can jump to the highest tree with index less than or equal to i." This seems to say that from any i (except maybe n), the rabbit can reach the maximum height in the prefix up to i. Is that true? In sample 1: a = [2,3,1,4]. For i=1, highest tree with index <=1 is 2. Can rabbit reach 2? Yes, it can reach 3? Actually max in prefix up to 1 is 2. But the answer for i=1 is 3, which is greater than 2. So Hint 5 might be incomplete or I misread. "it can jump to the highest tree with index less than or equal to i" – maybe it means the highest tree among those with index <= i that are reachable? Or maybe it's a step in the algorithm.
    // Let's read Hint 4: "Consider trees with height greater than the minimum height among trees with index greater than or equal to the current tree's index." This suggests we should look at the suffix minimums or something.
    
    // Let's try to compute the answer for each i by simulating the reachable set using a stack that maintains some "components".
    // We can process from right to left. 
    // Let ans[i] be the maximum height reachable from i.
    // Consider the rightmost tree n. From n, the rabbit can only jump backward to a taller tree. So it can reach any tree j < n with a_j > a_n. And from those, it can go further. So the reachable set from n is all trees that are connected to n. 
    // If we process from right to left, we can maintain the maximum height of the current component. When we are at i, we want to know if i can connect to the component to its right. 
    // When can i connect to the component starting at i+1? 
    // Let the component to the right be [i+1, R] with maximum height M. 
    // i can connect to this component if there is some j in [i+1, R] such that either (i < j and a_i > a_j) or (j < i? no, i is left). So we need a tree in the right component that is shorter than a_i (so i can jump forward to it) OR a tree in the right component that is taller than a_i (so that tree can jump backward to i). Actually, if there is a tree j > i with a_j > a_i, then j can jump backward to i. So i connects to the right component if there exists j > i in the component with a_j > a_i OR a_j < a_i. That is, if the component contains any tree with height different from a_i? But if all trees in the right component are equal to a_i, then no jump is possible. But if there is any tree not equal to a_i, then either it's taller (can jump backward to i) or shorter (i can jump forward to it). So i connects to the right component unless all trees in the right component have exactly the same height as a_i. But wait, is that sufficient? What if the right component has trees both taller and shorter, but they are not directly reachable from i? The condition only requires existence of at least one tree j in the component such that the edge exists. If such an edge exists, then i is connected to j, and since j is in the component, i joins the component. So indeed, i connects to the right component if and only if the right component contains at least one tree with height != a_i. 
    // Let's test this hypothesis.
    // Example: [2,3,1,4]. Process right to left.
    // i=4: component {4}, max=4.
    // i=3: a_3=1. Right component {4} has height 4 != 1. So 3 connects. New component {3,4}, max=4.
    // i=2: a_2=3. Right component {3,4} has heights 1 and 4. Both !=3. So 2 connects. New component {2,3,4}, max=4.
    // i=1: a_1=2. Right component has heights 3,1,4. All !=2. So 1 connects. Component {1,2,3,4}, max=4. But sample says answer for 1 is 3, not 4. So this hypothesis is wrong! 1 cannot reach 4. Why? Because even though there is a tree with height !=2 in the right component, the edge might not exist? Let's check: from 1 (2) to 4 (4): forward? 4>1, a_4=4 < 2? No. Backward? 4<1? No. So no direct edge. To reach 4, 1 must go through 2 or 3. 1->3 (1<2) works. From 3 (1), can go to 4? 4>3, a_4=4 < 1? No. From 3, can go to 2? 2<3, a_2=3 > 1, yes. From 2 (3), can go to 4? 4>2, a_4=4 < 3? No. Backward? 4<2? No. So from 2 cannot go to 4. So 1,2,3 are connected, but 4 is not connected to them. So my condition "exists a tree with different height" is not sufficient because the edge must exist between i and some tree in the component, but that tree might not be directly reachable if the component is not fully connected? Wait, the component {3,4} is connected? 3 and 4: 3->4? forward: 4>3, a_4=4 < 1? No. 4->3? backward: 3<4, a_3=1 > 4? No. So 3 and 4 are NOT connected! So my assumption that {3,4} was a component was wrong. Indeed, in sample, 4 is isolated. So the right component from 3 is just {4}? But 3 cannot connect to 4. So when processing from right to left, we need to correctly form components.
    
    // So how to correctly form components?
    // Let's analyze the condition for i to connect to the component to its right. 
    // Suppose we have a component C = [i+1, R]. We know that within C, all trees are connected. We want to know if i can connect to C. 
    // i can connect to C if there exists j in C such that (a_i > a_j) [i jumps forward] or (a_i < a_j) [j jumps backward]. 
    // So we need either min(C) < a_i or max(C) > a_i. 
    // If both min(C) >= a_i and max(C) <= a_i, then all trees in C have height equal to a_i. In that case, no edge exists. But if C has all heights equal to a_i, then C is just a block of equal heights. But wait, if C has all equal heights, are they connected to each other? Adjacent equal heights are not connected directly. But they could be connected via other trees? If they are all equal and there are no other trees, they are isolated from each other. So a component of equal heights would actually be a set of isolated trees, not a single component. So our assumption that C is a connected component implies that C cannot consist entirely of equal heights unless it's a single tree. So if C has more than one tree, it must have at least two different heights. Therefore, either min(C) < max(C). So either min(C) < a_i or max(C) > a_i is likely true unless a_i is exactly between them? Actually, if min(C) < a_i < max(C), then both conditions hold. If a_i <= min(C), then max(C) > min(C) >= a_i, so max(C) > a_i holds. If a_i >= max(C), then min(C) < max(C) <= a_i, so min(C) < a_i holds. So in all cases, as long as C is a non-trivial component (size > 1), we have either min(C) < a_i or max(C) > a_i. The only exception is if C is a single tree and its height equals a_i. So i connects to C unless C is a single tree with the same height as a_i.
    // But wait, in the example [2,3,1,4], when i=3 (a_3=1), the right component is {4} with height 4. min=4, max=4. a_3=
