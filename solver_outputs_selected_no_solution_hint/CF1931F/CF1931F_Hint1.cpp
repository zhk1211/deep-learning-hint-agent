#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, k;
    cin >> n >> k;
    vector<vector<int>> screens(k, vector<int>(n));
    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> screens[i][j];
        }
    }
    if (k == 1) {
        cout << "YES\n";
        return;
    }
    // Find two screenshots with different authors
    int idx1 = 0, idx2 = 1;
    // Determine relative order from first two screenshots
    vector<int> order;
    // We'll try to deduce the true order from the first screenshot,
    // but we need to place the author correctly.
    // The true order is a permutation. For each screenshot, the author is at front,
    // and the rest appear in the same relative order as in the true order.
    // So we can extract the relative order of all other participants from any screenshot.
    // Let's take the first screenshot, remove its author, that gives the relative order of others.
    // The true order is some cyclic shift? No, it's a linear order.
    // Actually, if we have true order P, then for author x, the screenshot is [x] + (P with x removed).
    // So the relative order of all elements except x is exactly as in P.
    // Thus from one screenshot we get the relative order of n-1 elements.
    // We need to insert the author somewhere to get full P.
    // With two screenshots, we can deduce the position of the first author relative to the second.
    
    // Let's build a candidate true order.
    // We can try to deduce from the first two screenshots.
    int a1 = screens[0][0], a2 = screens[1][0];
    // From screen 1: order of others is screens[0][1..n-1]
    // From screen 2: order of others is screens[1][1..n-1]
    // In true order P, a1 is somewhere, a2 is somewhere.
    // The relative order of all elements except a1 is screens[0][1..n-1].
    // The relative order of all elements except a2 is screens[1][1..n-1].
    // We can find the position of a1 in the second screenshot's list (excluding a2? Actually a1 appears in screen 2 at some position after index 0 if a1 != a2).
    // Since authors are distinct, a1 != a2.
    // In screen 2, a1 appears somewhere. Its position relative to others in screen 2 tells us where a1 is in the true order relative to those others.
    // But screen 2's order of others is P without a2. So a1's position in screen 2 is exactly its position in P, except if a2 was before a1 in P, then a1's index in screen 2 is its true index minus 1.
    // We can reconstruct P by taking the order from screen 1 (which is P without a1) and inserting a1 at the correct position.
    // How to find correct position? Compare with screen 2.
    // Let's find the index of a1 in screen 2's list (0-based, but author at 0 is a2). Let pos = find(screens[1].begin(), screens[1].end(), a1) - screens[1].begin().
    // In screen 2, the elements are [a2] + (P without a2). So a1 is at some position pos >= 1.
    // In P, a1 is before all elements that appear after it in screen 2, and after all elements that appear before it in screen 2 (except a2, which is not in that list).
    // But we also know the order of elements from screen 1: P without a1.
    // We can combine: the true order P must be consistent with both.
    // Let's build P by starting with screen 1's list without a1: that's P_without_a1 = screens[0][1..n-1].
    // Now we need to insert a1. In screen 2, a1 appears at position pos. The elements before a1 in screen 2 (indices 1..pos-1) are exactly the elements that are before a1 in P (since a2 is removed, and a2 could be before or after a1, but we don't know). Actually, if a2 is before a1 in P, then in screen 2, the elements before a1 are exactly the elements before a1 in P (since a2 is removed, the relative order of others is preserved). If a2 is after a1 in P, then in screen 2, the elements before a1 are exactly the elements before a1 in P, because a2 is after and doesn't affect. So in both cases, the set of elements before a1 in screen 2 is exactly the set of elements before a1 in P. And their order is the same as in P.
    // So we can determine the prefix of P before a1: it's the elements of screen 2 from index 1 to pos-1.
    // But we also have P_without_a1 from screen 1. In P_without_a1, the elements before a1 (in P) appear in the same relative order, but we don't know where a1 splits that list.
    // However, we can find the split point in P_without_a1: the prefix of P_without_a1 that consists exactly of the elements before a1 in screen 2.
    // Let before_a1 = vector<int>(screens[1].begin()+1, screens[1].begin()+pos);
    // This set of elements must appear as a prefix of P_without_a1? Not necessarily contiguous in P_without_a1? Wait, in P_without_a1, the order is exactly P with a1 removed. So the elements before a1 in P appear in the same order as in P, and they form a prefix of P_without_a1? No, because a1 is removed, the elements before a1 remain in their original order, and they will be at the beginning of P_without_a1, followed by the elements after a1. So yes, the elements before a1 in P form a prefix of P_without_a1. So we can just check: the first (pos-1) elements of P_without_a1 should be exactly before_a1. If they match, then a1 should be inserted after those elements. If they don't match, then maybe a2 is before a1 and the prefix is different? Let's analyze carefully.
    
    // Let P be the true permutation. For any author x, screenshot(x) = [x] + (P \ {x}).
    // So for author a1: screenshot1 = [a1] + P_without_a1, where P_without_a1 is P with a1 removed.
    // For author a2: screenshot2 = [a2] + P_without_a2.
    // In P, let the position of a1 be p1 (1-indexed), and a2 be p2.
    // In P_without_a1, the elements before a1 in P are exactly the first p1-1 elements.
    // In screenshot2, a1 appears at some position. Since screenshot2 = [a2] + P_without_a2, the position of a1 in screenshot2 (1-indexed, after author) is:
    // if p2 < p1: a1 is at position p1 (because a2 is before a1, so removing a2 shifts a1 left by 1, so its index in P_without_a2 is p1-1, plus 1 for author gives p1).
    // if p2 > p1: a1 is at position p1 (because a2 is after a1, removing a2 doesn't affect a1's index, so its index in P_without_a2 is p1-1, plus 1 gives p1).
    // So in both cases, the position of a1 in screenshot2 (0-based index in the array, where index 0 is a2) is exactly p1? Wait: if p2 < p1, then in P, a1 is at p1. In P_without_a2, the elements before a1 are those with indices < p1 in P, except a2. So there are p1-2 elements before a1 in P_without_a2. Then in screenshot2, a1 is at index (p1-2)+1 = p1-1? Let's do 0-based indexing for screenshot arrays.
    // Let's use 0-based indices for arrays. screens[i][0] is author.
    // Let P be 0-indexed array of length n. Let pos(x) be index of x in P.
    // For author a, screenshot = [a] + (P without a). The subarray after author is P with a removed, preserving order.
    // For a1: screenshot1[1..n-1] = P without a1.
    // For a2: screenshot2[1..n-1] = P without a2.
    // In screenshot2, find index of a1. Since a1 != a2, a1 appears in screenshot2[1..n-1]. Let idx = index of a1 in screenshot2 (1..n-1). Then idx is the number of elements before a1 in P_without_a2.
    // In P, the number of elements before a1 is pos(a1). In P_without_a2, if pos(a2) < pos(a1), then a2 is before a1, so it is removed, so number of elements before a1 in P_without_a2 is pos(a1) - 1. If pos(a2) > pos(a1), then number is pos(a1). So idx = pos(a1) if pos(a2) > pos(a1), else idx = pos(a1) - 1.
    // Thus pos(a1) is either idx or idx+1.
    // But we also have screenshot1: P_without_a1. The first pos(a1) elements of P_without_a1 are exactly the elements before a1 in P.
    // In screenshot2, the elements before a1 (indices 1..idx) are exactly the elements before a1 in P, except possibly a2 if a2 is before a1. But a2 is not in screenshot2[1..n-1] because it's the author. So the elements before a1 in screenshot2 are exactly the elements before a1 in P, minus a2 if a2 is before a1. But wait, if a2 is before a1, then in P_without_a2, the elements before a1 are the elements before a1 in P except a2. So the set of elements before a1 in screenshot2 is exactly the set of elements before a1 in P, possibly missing a2. But a2 is not in the list at all. So we cannot directly see a2.
    // However, we can use screenshot1 to see the full set of elements before a1 in P: they are the first pos(a1) elements of P_without_a1.
    // So we can determine pos(a1) by comparing the prefix of P_without_a1 with the elements before a1 in screenshot2.
    // Let before_in_s2 = vector of elements in screenshot2 from index 1 to idx-1 (if idx>=1). These are exactly the elements before a1 in P, except possibly a2 if a2 is before a1. But a2 is not in screenshot2[1..], so if a2 is before a1, then before_in_s2 will have size pos(a1)-1, and will match the first pos(a1)-1 elements of P_without_a1, and the next element in P_without_a1 would be a2? Actually, in P_without_a1, the elements before a1 are all elements before a1 in P. If a2 is before a1, then a2 is among them. Since a2 is not a1, a2 appears in P_without_a1. So in P_without_a1, the first pos(a1) elements include a2. In before_in_s2, a2 is missing. So before_in_s2 will be exactly the first pos(a1) elements of P_without_a1 but with a2 removed. So they won't match directly if we just compare prefixes.
    // Alternative approach: build a directed graph of relative orders.
    // For each screenshot, we know that for the author x, the order of others is the true order with x removed. So for any two distinct participants u, v (both != x), their relative order in the screenshot is the same as in the true order. So we can add directed edges u -> v if u appears before v in any screenshot where neither is the author.
    // If we have enough screenshots, we might get a total order. But we only have k screenshots, and k can be small.
    // However, note that the authors are all distinct. So for any pair (u,v), there is at most one screenshot where one of them is the author? Actually, there could be a screenshot where u is author, and another where v is author. In the screenshot where u is author, v appears, so we get the relative order of v with respect to all others except u. But we don't get u's position relative to others from that screenshot.
    // But we can combine information: from screenshot of u, we know the order of all others. From screenshot of v, we know the order of all others. The only missing relative order is between u and v. We can deduce it by comparing the positions of some third element w in both screenshots? Actually, if we have a third element w, in u's screenshot, w appears somewhere. In v's screenshot, w appears somewhere. But that doesn't directly give u vs v.
    // However, we can deduce u vs v by looking at the position of v in u's screenshot and u in v's screenshot? No, u's screenshot doesn't contain u, v's doesn't contain v.
    // Wait: In u's screenshot, v is present. The position of v in u's screenshot tells us how many elements are before v in the true order, except u. So if we know the true order of all elements except u, we can insert u by comparing with another screenshot.
    // Let's think differently: The true order is uniquely determined if we have at least two screenshots with different authors? Not always, consider n=3, screenshots: [1,2,3] and [2,1,3]. True order could be [1,2,3] or [2,1,3]? Let's check: If true order is [1,2,3], then screenshot of 1 is [1,2,3], screenshot of 2 is [2,1,3]. If true order is [2,1,3], screenshot of 1 is [1,2,3]? Wait: if true order is [2,1,3], then for author 1, remove 1 -> [2,3], so screenshot is [1,2,3]. For author 2, remove 2 -> [1,3], screenshot is [2,1,3]. So both true orders produce the same screenshots! So it's ambiguous. But the problem asks if there EXISTS at least one order. So if multiple exist, answer is YES. So we just need to check consistency.
    // Actually, the condition for existence is that we can assign a total order consistent with all screenshots. This is equivalent to: the directed graph of known relative orders must be acyclic, and we must be able to extend to a total order. But since we only have constraints of the form "u is before v" for pairs where neither is the author in some screenshot, we can just check if there's any cycle in these constraints. If acyclic, we can always topologically sort to get a total order. But wait, is that sufficient? What about the authors' positions? The constraints from screenshots only give relative orders among non-author elements. The authors' positions are not constrained relative to others by their own screenshots. However, if we have multiple screenshots, we might get constraints involving authors from other screenshots. For example, in screenshot of 1, we see 2 and 3: 2 before 3. In screenshot of 2, we see 1 and 3: 1 before 3. So we have 2->3 and 1->3. We don't have 1 vs 2. So any total order with 3 last and 1,2 in any order works. So acyclic is enough? But we also need to ensure that the authors are at the top of their own screenshots. That is automatically satisfied if we take the total order and generate screenshots: the author will be at top because we remove them and put them first. Wait, the screenshot definition: "each person sees himself at the top of the list". So given a true order, the screenshot for person x is [x] followed by the true order with x removed. So if we construct any total order, the screenshots generated will have the author at top. So the only constraints from a screenshot are the relative order of the other n-1 elements. So the problem reduces to: given k permutations of subsets (each missing one distinct element), can we find a total order of all n elements such that for each screenshot, the order of the n-1 elements matches the given order? This is exactly the problem of checking consistency of partial orders. We can just collect all pairwise constraints: for each screenshot, for i from 1 to n-2, we have a_{i} before a_{i+1} in the true order. These are constraints between two elements that are both not the author. So we add directed edges a_{i} -> a_{i+1}. Then we check if the graph has a cycle. If it has a cycle, impossible. If acyclic, we can always complete to a total order (any topological sort). But wait: is that always sufficient? Consider n=3, k=2, screenshots: [1,3,2] and [2,1,3]. From first: 3->2. From second: 1->3. Edges: 1->3, 3->2. Acyclic, total order 1,3,2 works. Check: true order [1,3,2] gives screenshot 1: [1,3,2] (matches), screenshot 2: [2,1,3] (matches). Yes.
    // What about the case where k=1? Then no constraints between any two elements (since the only screenshot has author at top, and we don't get constraints involving the author). So graph has no edges, acyclic, always YES. That matches sample.
    // But wait, sample 1 has test case: 5 1 -> YES. Sample 2: 4 4 -> YES. Sample 3: 6 2 -> YES. Sample 4: 3 3 -> YES. Sample 5: 10 2 -> NO. Let's test sample 5: n=10, k=2. Screenshots:
    // 1: 1 2 3 4 5 6 7 8 9 10
    // 2: 10 9 8 7 6 5 4 3 2 1
    // Authors: 1 and 10.
    // From first: 2->3, 3->4, 4->5, 5->6, 6->7, 7->8, 8->9, 9->10.
    // From second: 9->8, 8->7, 7->6, 6->5, 5->4, 4->3, 3->2, 2->1.
    // Edges: 2->3->4->5->6->7->8->9->10 and 9->8->7->6->5->4->3->2->1. This has cycles (e.g., 2->3 and 3->2). So NO. Correct.
    // Sample 6: 1 1 -> YES.
    // Sample 7: 5 2
    // 1: 1 2 3 5 4
    // 2: 2 1 3 5 4
    // Authors 1 and 2.
    // From 1: 2->3, 3->5, 5->4.
    // From 2: 1->3, 3->5, 5->4.
    // Edges: 2->3, 3->5, 5->4, 1->3. Acyclic. YES.
    // Sample 8: 3 3
    // 3 1 2
    // 2 3 1
    // 1 3 2
    // Authors: 3,2,1.
    // From 3: 1->2
    // From 2: 3->1
    // From 1: 3->2
    // Edges: 1->2, 3->1, 3->2. Acyclic (3->1->2). YES.
    // Sample 9: 5 4
    // 3 5 1 4 2
    // 2 5 1 4 3
    // 1 5 4 3 2
    // 5 1 4 3 2
    // Authors: 3,2,1,5.
    // From 3: 5->1, 1->4, 4->2
    // From 2: 5->1, 1->4, 4->3
    // From 1: 5->4, 4->3, 3->2
    // From 5: 1->4, 4->3, 3->2
    // Edges: 5->1, 1->4, 4->2, 4->3, 5->4, 3->2, etc. Let's check cycles: 5->1->4->3->2, 5->4, etc. No cycle? 4->2 and 4->3, 3->2. No cycle. YES.
    // Sample 10: 3 3
    // 1 3 2
    // 2 1 3
    // 3 2 1
    // Authors: 1,2,3.
    // From 1: 3->2
    // From 2: 1->3
    // From 3: 2->1
    // Edges: 3->2, 1->3, 2->1 => cycle 1->3->2->1. NO. Correct.
    // So the condition is simply: build directed graph on n vertices, for each screenshot, for j from 1 to n-2, add edge a[j] -> a[j+1]. Then check if graph has a cycle. If no cycle, YES, else NO.
    // But wait, is that always sufficient? Consider a case where the constraints force a total order, but the authors' positions in that total order might contradict the screenshots? No, because the screenshots only provide constraints among non-author elements. If we find any total order consistent with those constraints, then for each screenshot, the order of the other elements will match the constraints. But will it exactly match the given screenshot? The screenshot gives a specific permutation of the n-1 elements. Our constraints only enforce the relative order of adjacent pairs in that permutation. But if the graph is acyclic, any topological sort will respect all pairwise constraints that are implied by the edges. However, the screenshot gives a total order of the n-1 elements. The edges we added are only between consecutive elements in that screenshot. But if the graph is acyclic, does there exist a total order that exactly matches the given permutation for each screenshot? Not necessarily! Because the screenshot might have elements in an order that is not fully captured by adjacent pairs if there are other constraints? Wait, if we only add edges between consecutive elements in the screenshot, we are only enforcing that those specific pairs are in that order. But the screenshot itself is a total order of the n-1 elements. If we take the transitive closure of the edges from all screenshots, we get a partial order. The screenshot's total order is a linear extension of the constraints from that screenshot alone. But when we combine screenshots, the partial order might have multiple linear extensions. The question is: does there exist AT LEAST ONE total order that is a linear extension of the combined partial order AND also, when we remove the author for each screenshot, we get exactly the given permutation? Actually, if the total order is a linear extension of the combined partial order, then for any screenshot, the relative order of the n-1 elements in the total order must respect all constraints from that screenshot. But does it have to be exactly the given permutation? The constraints from that screenshot are exactly the total order of those n-1 elements. If we only add adjacent pairs, we are not capturing the full total order. For example, suppose a screenshot has order [2,3,1,4] for author 1. The adjacent pairs are 2->3, 3->1, 1->4. If we only enforce these, a total order like [2,1,3,4] would satisfy 2->3 and 1->4, but violates 3->1. So it's not a linear extension of the constraints from that screenshot. But wait, we added 3->1 as an edge. So [2,1,3,4] has 1 before 3, which violates 3->1. So it would be rejected. So any linear extension of the graph we built will respect all the adjacent pairs from all screenshots. But does respecting all adjacent pairs guarantee that the relative order of the n-1 elements in the total order exactly matches the screenshot? Not necessarily! Consider a screenshot with order [2,4,3] (n=4, author 1). Adjacent pairs: 2->4, 4->3. A total order [2,3,4] respects 2->4? No, 2 is before 4, yes. 4->3? 4 is before 3? In [2,3,4], 3 is before 4, so 4->3 is violated. So it would be rejected. What about [2,4,1,3]? For the screenshot, the other elements are 2,4,3. In total order [2,4,1,3], the relative order of 2,4,3 is 2,4,3. That matches. So if we only add adjacent pairs, any total order that respects those adjacent pairs will have the elements of that screenshot in the exact same relative order? Let's check: Suppose we have a set S of n-1 elements. We are given a total order of S: s1, s2, ..., s_{n-1}. We add edges s_i -> s_{i+1} for all i. In any linear extension of the whole graph (which includes these edges), the relative order of the elements in S must be a linear extension of the chain s1->s2->...->s_{n-1}. The only linear extension of a total order is the total order itself. So yes, if we add all adjacent pairs from the screenshot, the transitive closure forces the exact total order of those n-1 elements. Because from s1->s2 and s2->s3, transitivity gives s1->s3, etc. So any linear extension must have s1 before s2 before s3 ... before s_{n-1}. So the relative order of S in any linear extension is exactly the given permutation. So adding only adjacent edges is sufficient to enforce the full permutation via transitivity. Thus, the condition is exactly: the directed graph with edges from consecutive elements in each screenshot (excluding the author) must be acyclic. If acyclic, we can take any topological sort as the true order, and it will produce exactly the given screenshots. Wait, is that true? Let's verify: If we take a topological sort T of the graph, then for any screenshot with author a, the elements other than a appear in T in some order. Since the graph contains the chain of that screenshot, T must respect that chain, so the order of those elements in T is exactly the chain order. So T without a is exactly the screenshot's order. Then the screenshot generated from T for author a is [a] + (T without a), which matches the given screenshot. So yes, any topological sort works. So the problem reduces to checking if the directed graph has a cycle.
    // But wait: What about the author's position? The author is not involved in the edges from their own screenshot. But they might appear in other screenshots. In other screenshots, they are not the author, so they appear in the chain and we add edges involving them. So their position relative to others is constrained by other screenshots. So the graph includes all participants. If acyclic, we get a total order of all n. Then for each screenshot, the author is at the top of their own screenshot by construction (since we put them first and then the rest in T order). So it's consistent.
    // Is there any case where the graph is acyclic but no total order can produce the screenshots? Suppose we have screenshots that force a cycle involving the author? But the author is not in their own chain, so they might not be constrained relative to some elements. But if the graph is acyclic, we can always topologically sort. The only potential issue: In the generated screenshot, the author must be at the top. But our construction puts the author at the top explicitly. So it's fine.
    // Let's test a tricky case: n=3, k=2. Screenshots: [1,2,3] and [2,3,1]. Authors 1 and 2.
    // From 1: 2->3.
    // From 2: 3->1.
    // Edges: 2->3, 3->1. Acyclic. Topological sort: 2,3,1. Check: true order [2,3,1]. Screenshot 1: [1,2,3]? Remove 1 from [2,3,1] gives [2,3], so screenshot [1,2,3] matches. Screenshot 2: [2,3,1]? Remove 2 from [2,3,1] gives [3,1], so screenshot [2,3,1] matches. Works.
    // Another case: n=3, k=2. [1,3,2] and [2,1,3]. Edges: 3->2, 1->3. Acyclic. Sort: 1,3,2. Works.
    // What about n=4, k=2. [1,2,4,3] and [2,1,4,3]. Edges: 2->4, 4->3 from first; 1->4, 4->3 from second. Acyclic. Sort: 1,2,4,3 or 2,1,4,3. Both work? Check 2,1,4,3: screenshot 1: [1,2,4,3]? Remove 1 -> [2,4,3], so [1,2,4,3] matches. Screenshot 2: [2,1,4,3]? Remove 2 -> [1,4,3], so [2,1,4,3] matches. Works.
    // So the condition seems correct.
    // But wait, the problem statement says: "The screenshots were taken within a short period of time, and the order of participants has not changed." So the true order is fixed. Our task is to determine if there exists at least one order consistent with all screenshots. So cycle detection is necessary and sufficient.
    // Let's double-check with a potential counterexample. Suppose n=4, k=2. Screenshots: [1,2,3,4] and [2,4,3,1]. Authors 1 and 2.
    // From 1: 2->3, 3->4.
    // From 2: 4->3, 3->1.
    // Edges: 2->3, 3->4, 4->3 (cycle 3->4 and 4->3). So NO. Indeed, from first, 3 before 4; from second, 4 before 3. Contradiction.
    // What if the cycle doesn't involve direct contradiction but a longer cycle? e.g., 1->2, 2->3, 3->1. That's a cycle, NO.
    // So algorithm: For each test case, build graph with n nodes. For each screenshot, for j from 1 to n-2, add directed edge from a[j] to a[j+1]. Then check for cycles using DFS (with states 0=unvisited, 1=visiting, 2=visited). If cycle found, output NO, else YES.
    // Complexity: Sum of n*k <= 2e5, so total edges added <= sum over test cases of (k*(n-2)) <= sum n*k <= 2e5. Graph has n nodes per test case, sum of n over test cases? The constraint is sum n*k <= 2e5, but n can be up to 2e5, k up to n. However, sum n*k is bounded, so total edges is small. We can just clear graph per test case.
    // We need to handle multiple test cases efficiently. Use vector<vector<int>> adj, and resize per test case.
    // DFS for cycle detection:
    // vector<int> state(n+1, 0);
    // function<bool(int)> dfs = [&](int u) {
    //     state[u] = 1;
    //     for (int v : adj[u]) {
    //         if (state[v] == 1) return true;
    //         if (state[v] == 0 && dfs(v)) return true;
    //     }
    //     state[u] = 2;
    //     return false;
    // };
    // For i from 1 to n, if state[i]==0, if dfs(i) -> cycle.
    // Note: The graph may have multiple edges between same nodes? The screenshots could have the same adjacent pair in multiple screenshots. That's fine, we can just add the edge multiple times, or use set to avoid duplicates. But duplicates don't affect cycle detection (they just add redundant edges, but DFS still works). However, to save memory/time, we could use set or just let duplicates be. Since total edges <= 2e5, duplicates are fine.
    // But wait: The sum of n*k <= 2e5, but n itself can be up to 2e5. The number of nodes per test case can be large, but sum of n over test cases might be large if k is small? The constraint is n*k <= 2e5 for each test case? Actually, the problem says: "n * k <= 2 * 10^5" for each test case? No, it says: "n * k <= 2 * 10^5" in the constraints? Let's read: "1 <= k <= n <= 2 * 10^5, n * k <= 2 * 10^5" — this is per test case? Usually it means the sum over test cases. The statement: "It is guaranteed that the sum of n * k for all test cases does not exceed 2 * 10^5." So total n*k over all test cases <= 2e5. That means the total number of elements in all screenshots is <= 2e5. So total edges added is at most total elements minus something, so <= 2e5. The sum of n over test cases could be larger? If k=1, n can be 2e5, and n*k = 2e5. So one test case with n=2e5, k=1. Then sum n = 2e5. If multiple test cases, sum n could be up to 2e5 as well because n*k >= n (since k>=1). So sum n <= sum n*k <= 2e5. So total nodes across all test cases is also bounded by 2e5. So we can safely allocate per test case.
    // So algorithm is O(total n*k) time and O(total n) memory.
    // Let's implement.
    // Edge case: n=1. Then k=1. Screenshot has only one element. No edges. Graph has 1 node, no edges, acyclic -> YES. Our loop for j from 1 to n-2 will not run (since n-2 = -1). So fine.
    // Also, note that authors are distinct, but we don't need that explicitly.
    // Let's test with sample.
    // Sample 1: 10 test cases. We'll trust.
    // Potential pitfall: The graph might have up to 2e5 nodes, recursion depth in DFS could be up to 2e5, which might
