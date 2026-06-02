#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<vector<int>> adj(n + 1);
    vector<pair<int, int>> edges;
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
        edges.emplace_back(u, v);
    }

    // Find a leaf
    int leaf = -1;
    for (int i = 1; i <= n; ++i) {
        if (adj[i].size() == 1) {
            leaf = i;
            break;
        }
    }

    // If n == 2, only one edge, can't get exactly 2 good pairs
    if (n == 2) {
        cout << "NO\n";
        return;
    }

    // Root the tree at the leaf
    vector<int> parent(n + 1, 0);
    vector<int> order;
    function<void(int, int)> dfs = [&](int u, int p) {
        parent[u] = p;
        order.push_back(u);
        for (int v : adj[u]) {
            if (v != p) {
                dfs(v, u);
            }
        }
    };
    dfs(leaf, 0);

    // We need exactly n good pairs.
    // Strategy: direct all edges towards the leaf, except one edge from the leaf's neighbor to leaf reversed?
    // Actually, we can achieve n good pairs by making a "star" orientation from a center.
    // Let's pick the leaf's neighbor as the center.
    int center = adj[leaf][0];
    
    // Direct all edges away from center, except the edge center->leaf we direct leaf->center.
    // This gives: from leaf you can reach center, from center you can reach all others.
    // Good pairs: (leaf, center) + (center, any other) = 1 + (n-2) = n-1. Need one more.
    // If we direct leaf->center and all others center->x, then from leaf you can reach center and all x (via center).
    // So leaf can reach n-1 vertices. Center can reach n-2 others (all except leaf). Total = (n-1)+(n-2) = 2n-3 > n for n>3.
    // So that's too many.

    // Alternative: direct all edges towards the leaf. Then leaf is sink, all paths go to leaf.
    // Good pairs: any u can reach leaf, and if u is ancestor of v, u can reach v.
    // In a rooted tree with all edges pointing to root, number of good pairs = sum of subtree sizes? Actually it's n + sum_{v != root} (size_subtree(v) - 1)?
    // Let's compute: For each node, it can reach all nodes in its subtree. So total = sum_{v} size_subtree(v).
    // We want this sum to be n.

    // If we direct all edges towards the leaf (root), then size_subtree(leaf)=1, for others size_subtree includes itself and children.
    // Sum of subtree sizes = n + sum_{v != leaf} (size_subtree(v) - 1). This is at least n + (n-1)*0? Actually minimum when tree is a star centered at leaf? No, leaf is leaf, so star centered at leaf's neighbor: leaf has size 1, center has size n, others size 1. Sum = 1 + n + (n-2)*1 = 2n-1 > n.
    // So that's too big.

    // We need exactly n. Observe that n is small relative to typical number of pairs.
    // The only way to have exactly n good pairs is if the directed graph is a collection of directed paths that don't intersect much.
    // Actually, consider a directed tree where every node has out-degree at most 1. Then it's a set of paths ending at sinks.
    // Number of good pairs = sum over each node of (length of path from it to its sink). This can be n if we have one path of length n? But tree has n-1 edges, so a single path of n nodes has n-1 edges, good pairs = n*(n-1)/2 > n for n>3.
    // So we need something else.

    // Let's think about the hint: remove one edge. The tree splits into two components of sizes a and b (a+b=n).
    // If we direct the edge from component A to component B, then all nodes in A can reach all nodes in B. That gives a*b good pairs across the cut.
    // Additionally, there are good pairs within each component.
    // Total good pairs = a*b + good_pairs(A) + good_pairs(B).
    // We want total = n.
    // Since a*b >= a+b-1? Actually a*b >= a+b-1 for a,b>=1, with equality only when one of them is 1.
    // So if a>1 and b>1, a*b >= 4 > n? Not necessarily, n could be large. But a*b grows quadratically.
    // To keep total small, we likely need a=1 or b=1 for every edge? That would mean the tree is a star.
    // Let's test star: center c, leaves l1..lk (k=n-1). Direct all edges from leaves to center. Then good pairs: each leaf can reach center (k pairs). Center can reach nobody. Total = k = n-1. Not n.
    // Direct all edges from center to leaves: center can reach all leaves (k pairs). Leaves can reach nobody. Total = k = n-1.
    // Direct some leaves to center, some center to leaves: Suppose x leaves point to center, y leaves point from center (x+y=k). Then leaves pointing to center can reach center and also the y leaves? No, from leaf to center, then center to leaf? But center has edges to y leaves, so yes, leaf -> center -> leaf. So those x leaves can reach center and y leaves. That's x*(1+y) pairs. Center can reach y leaves. Total = x*(1+y) + y. We want this to be n = k+1.
    // x(1+y)+y = x + xy + y = x+y + xy = k + xy. So we need xy = 1. Since x,y integers, x=1,y=1, so k=2, n=3. For n=3 star, we can have 1 leaf->center, center->1 leaf. Total pairs = 3. Works.
    // For n>3, star cannot give exactly n.

    // So maybe the answer is NO for n>3? But sample has n=5 and YES.
    // Sample 1: n=5, tree is a path? Edges: 1-2,2-4,1-3,3-5. It's two paths attached at 1. They directed: 1->2, 3->1, 3->5, 4->2. Let's analyze: 3->5, 3->1, 1->2, 4->2. Good pairs: (3,5),(3,1),(3,2),(1,2),(4,2) = 5. So it works.
    // How? The tree is not a star. They have a structure where some nodes have out-degree >1? 3 has out-edges to 1 and 5. 1 has out-edge to 2. 4 has out-edge to 2. So 2 is sink. 
    // Let's compute using cut: remove edge 1-2. Components: A={1,3,5}, B={2,4}. Edge directed 1->2. a=3,b=2. a*b=6. But total is 5, so internal good pairs must be -1? Impossible. So my cut formula assumes all edges between components are directed one way? Actually there is only one edge between components, so it's correct. But total good pairs = 6 + good(A) + good(B). We have total 5, so good(A)+good(B) = -1, impossible. So my cut reasoning is flawed: good pairs across cut are not all a*b because not all nodes in A can reach all in B? Wait, if edge is 1->2, then from any node in A, can we reach 1? Yes, if there is a path to 1. In A, edges are 3->1, 3->5. So from 3 we can reach 1. From 5 we can reach? 5 has no outgoing edges? Actually 3->5 means 3 can reach 5, but 5 cannot reach 3 or 1. So 5 cannot reach 1. Thus 5 cannot reach B. So not all a*b pairs are good. So the cut formula a*b only holds if the component is strongly connected? No, if all edges in A are directed towards the cut edge, then all nodes can reach the cut edge. Here, 5 is not directed towards 1. So the number of nodes in A that can reach the cut edge is less than a.

    // So we need to design a directed tree with exactly n good pairs.
    // Let's think about a tree where we direct all edges towards a single sink, except we make one node have out-degree 2? 
    // In sample 1, sink is 2. Nodes that can reach 2: 1,3,4. 5 cannot reach 2. Good pairs: (3,5),(3,1),(3,2),(1,2),(4,2). That's 5.
    // Notice that 3 can reach 5,1,2. 1 can reach 2. 4 can reach 2. 5 can reach nothing. 2 can reach nothing.
    // So the number of good pairs = sum of (number of reachable nodes from each node). 
    // For sink, 0. For 5, 0. For 4, 1 (2). For 1, 1 (2). For 3, 3 (5,1,2). Total = 5.
    // This is exactly n when the reachable sets form a chain? Not exactly.

    // Let's try to construct a general solution.
    // Consider rooting the tree arbitrarily. Suppose we direct all edges from child to parent, except for one subtree where we direct from parent to child? 
    // Let's analyze the number of good pairs if we direct all edges towards a root R. Then every node can reach all its ancestors up to R. So the number of good pairs from a node u is the depth of u (number of nodes on path to R, including R? Actually if u can reach R, and all nodes on path, then number of reachable nodes = distance to R + 1? Wait, if edges are u->parent, then u can reach parent, grandparent, ..., R. So reachable set = ancestors including R. Size = depth(u) (if depth of R is 0, u's depth is distance). So total good pairs = sum_{u} depth(u). This is sum of depths.
    // We want this sum to be n.
    // Sum of depths in a tree of n nodes can vary. Minimum is when tree is a star with root at center: depths: center 0, leaves 1. Sum = n-1. Maximum is a line with root at one end: depths 0,1,2,...,n-1 sum = n(n-1)/2.
    // So we can achieve n if we can get sum of depths = n.
    // For n=5, sum of depths = 5. In a tree of 5 nodes, can we have sum of depths = 5? 
    // If root has depth 0, we need sum of depths = 5. With star, sum=4. With line, sum=10. We need something in between.
    // Example: root 1, children 2,3. 2 has child 4. 3 has child 5. Depths: 1:0, 2:1, 3:1, 4:2, 5:2. Sum = 0+1+1+2+2 = 6. Close.
    // If we make 4 a child of 1 instead: depths: 1:0, 2:1, 3:1, 4:1, 5:2. Sum = 0+1+1+1+2 = 5. Bingo!
    // That tree: edges 1-2,1-3,1-4,3-5. Direct all towards 1: 2->1, 3->1, 4->1, 5->3. Good pairs: 2->1, 3->1, 4->1, 5->3, 5->1. Total = 5. That's exactly sample 2! Sample 2 output: 2->1, 3->1, 4->1, 5->4? Wait sample 2 output has 5->4, but 4 is leaf? In sample 2, edges: 1-2,1-3,1-4,4-5. They output: 2->1, 3->1, 4->1, 5->4. That's all towards 1 except 5->4 is towards 4, but 4->1 so 5 can reach 1 via 4. So it's essentially all towards 1. Depths: 1:0, 2:1, 3:1, 4:1, 5:2. Sum=5. So sample 2 is exactly the "all towards root" with sum of depths = n.

    // So the problem reduces to: can we root the tree at some node and direct all edges towards the root such that the sum of depths equals n? And if so, output those directions.
    // But wait, in sample 1, the directions are not all towards a single root. 3->5 and 3->1 and 1->2 and 4->2. Here, 2 is sink, but 3 points to 1 and 5, so 3 has out-degree 2. That's not all towards root. So there are other configurations.

    // However, the "all towards root" is a simple construction. Let's check if we can always achieve sum of depths = n by choosing the root appropriately.
    // We need to find a root and a tree (the given tree) such that sum of depths = n.
    // The sum of depths depends on the root. For a fixed tree, we can compute sum of depths for each possible root using rerooting DP.
    // Then we just check if any root gives sum = n. If yes, direct all edges towards that root.
    // But is it always possible when the answer is YES? The sample 1 has sum of depths for any root? Let's compute for sample 1 tree: edges 1-2,2-4,1-3,3-5.
    // Root at 1: depths: 1:0, 2:1, 3:1, 4:2, 5:2. Sum=6.
    // Root at 2: depths: 2:0, 1:1, 4:1, 3:2, 5:3. Sum=7.
    // Root at 3: depths: 3:0, 1:1, 5:1, 2:2, 4:3. Sum=7.
    // Root at 4: depths: 4:0, 2:1, 1:2, 3:3, 5:4. Sum=10.
    // Root at 5: depths: 5:0, 3:1, 1:2, 2:3, 4:4. Sum=10.
    // None gives sum=5. Yet the answer is YES with a different orientation. So "all towards root" is not the only way.

    // So we need a more general approach.
    // Let's think about the structure of a directed tree with exactly n good pairs.
    // In a directed tree, the underlying graph is a tree, so there are no cycles. The reachability is a partial order.
    // The number of good pairs is the number of pairs (u,v) such that v is reachable from u.
    // In a tree, if we direct edges, the reachability graph is a DAG. The number of pairs can be computed as sum over nodes of (number of nodes in its reachable set).
    // We want this sum to be exactly n.
    // Since there are n nodes, the average size of reachable set is 1. So most nodes must have reachable set of size 1 (only themselves? No, if a node can reach itself? The problem says "two distinct vertices", so self-pairs are not counted. So reachable set size here means number of other vertices reachable. So average is 1. So many nodes have 0 or 1 reachable nodes.
    // In sample 1, reachable counts: 3:3, 1:1, 4:1, 2:0, 5:0. Sum=5.
    // In sample 2, reachable counts: 5:2 (4,1), 2:1 (1), 3:1 (1), 4:1 (1), 1:0. Sum=5.
    // Notice that in both, there is exactly one node with reachable count >1, and it's 3 in sample 1, 5 in sample 2. The rest have 0 or 1.
    // Also, the structure seems to be: a directed path from a "source" to a "sink", and some "branches" that are directed towards the path? 
    // In sample 1: source 3, path 3->1->2, and branch 3->5, 4->2. Actually 3->5 is a branch out of source, 4->2 is a branch into sink.
    // In sample 2: source 5, path 5->4->1, and branches 2->1, 3->1.

    // Let's try to formalize. Suppose we have a directed tree. Consider the set of nodes with out-degree 0 (sinks). In a tree, if we direct edges, there must be at least one sink. Actually, in any finite DAG there is a sink. In a directed tree, there can be multiple sinks? If there are two sinks, they are in different parts, and there is no path between them. The number of sinks could be multiple.
    // In sample 1, sinks are 2 and 5? 2 has out-degree 0, 5 has out-degree 0. Yes, two sinks.
    // In sample 2, sink is 1 only? 1 has out-degree 0. 2,3,4,5 have out-degree 1. So one sink.

    // Let's think about the condition for exactly n good pairs.
    // Consider the condensation of the directed tree? No cycles.
    // Let's root the undirected tree arbitrarily, say at 1. We need to assign directions.
    // This is equivalent to choosing for each edge a direction. There are 2^(n-1) possibilities. We need to find one with sum of reachable = n.
    // We can use DP on tree to count number of good pairs for a given orientation? But we need to construct one.

    // Another perspective: In a directed tree, the number of good pairs is equal to the number of pairs (u,v) such that the unique path between u and v in the undirected tree is directed from u to v consistently. That is, all edges on the path go from u towards v.
    // So a pair (u,v) is good iff the path is a directed path from u to v.
    // So we are counting the number of directed paths in the tree.
    // We want exactly n directed paths (of length >=1).
    // Since there are n-1 edges, the maximum number of directed paths is when all edges are directed the same way along a line? No, that gives n(n-1)/2.
    // We want exactly n.

    // Let's consider the degrees. For a node, its out-degree and in-degree.
    // If a node has out-degree d, it can start paths to... 
    // Maybe we can think of it as: each edge contributes to some paths. A directed path of length L contains L edges. The total number of directed paths is the sum over all pairs of (1 if path is directed). This is like counting the number of pairs (u,v) with u ancestor of v in some rooted tree if we direct all edges away from root? No, that gives all pairs where u is ancestor of v.

    // Let's try to construct a solution for general n.
    // Observe that in both samples, the number of good pairs is n, and the tree has a node with out-degree 2 (sample 1: node 3) or a node with in-degree 3 (sample 2: node 1). 
    // In sample 1, node 3 has out-degree 2, node 1 has in-degree 1, out-degree 1, node 2 has in-degree 2, out-degree 0.
    // In sample 2, node 1 has in-degree 3, out-degree 0; node 4 has in-degree 1, out-degree 1; node 5 has out-degree 1, in-degree 0.

    // Let's try to see if we can always achieve n by making a "broom" structure: a directed path, with all other edges directed towards the path.
    // Suppose we have a directed path P = v1 -> v2 -> ... -> vk. All other nodes are connected to this path by edges directed towards the path. That is, if a node u is not on P, it has a single edge to some node on P, and that edge is directed u -> vi (so u points to the path). Then what are the good pairs?
    // Nodes on P: vi can reach vj for j>i. So number of pairs within P = k(k-1)/2.
    // Nodes off P: each such node u can reach the vi it points to, and then all vj for j>i. So u can reach (k - i + 1) nodes on P? Actually vi, vi+1, ..., vk. That's k-i+1 nodes. Also, can u reach other off-path nodes? No, because edges are only towards the path, and from path there are no edges back to off-path nodes (since they point to path). So u cannot reach other off-path nodes. So total good pairs = sum_{u off P} (k - i_u + 1) + k(k-1)/2.
    // We want this to equal n.
    // Let m = number of nodes off P. Then n = k + m.
    // Total pairs = k(k-1)/2 + sum_{u} (k - i_u + 1).
    // Notice that sum_{u} (k - i_u + 1) = m*(k+1) - sum_{u} i_u.
    // So total = k(k-1)/2 + m(k+1) - sum i_u.
    // We want this to be n = k+m.
    // So k(k-1)/2 + m(k+1) - sum i_u = k+m.
    // => k(k-1)/2 + m k + m - sum i_u = k+m
    // => k(k-1)/2 + m k - sum i_u = k
    // => sum i_u = k(k-1)/2 + m k - k = k(k-1)/2 + k(m-1).
    // sum i_u is the sum of indices on P that the off-path nodes attach to.
    // Each off-path node attaches to exactly one node on P (since tree). The indices i_u are between 1 and k.
    // We have m off-path nodes. The sum of their indices must equal the above.
    // Also, the tree structure must be valid: the off-path nodes form trees attached to P, but since they all point to P, they must be directed towards the attachment point. That means in the subtree hanging off vi, all edges must be directed towards vi. That is exactly the "all towards root" within that subtree, with root vi. So the number of good pairs within that subtree (including the edge to vi) is exactly the sum of depths in that subtree when rooted at vi. But we already accounted for the paths from off-path nodes to P and within P. However, within the off-path subtree, there could be additional good pairs between off-path nodes? If the subtree is directed towards vi, then any node in the subtree can reach its ancestors up to vi. So if there are multiple nodes in the same off-path branch, they can reach each other? For example, if u -> w -> vi, then u can reach w and vi. So u can reach w, which is another off-path node. Our previous count only considered u reaching P, not other off-path nodes. So we missed those pairs.
    // So the simple broom model is incomplete because off-path nodes can reach each other if they are on the same branch directed towards P.

    // Let's refine: The whole tree is directed towards a set of sinks? Actually, if we direct all edges towards a single sink, we get sum of depths. If we have multiple sinks, it's more complex.

    // Let's go back to the cut idea. The hint says: "Think of what happens when you remove one edge from the tree."
    // Removing an edge splits the tree into two components. Suppose we direct the edge from component A to component B. Then any node in A that can reach the edge's tail can reach all nodes in B that are reachable from the edge's head. But not necessarily all nodes in B are reachable from the head, because B might have edges directed away from the head.
    // However, if we consider the directed tree as a whole, for any edge, the number of good pairs that cross that edge is (number of nodes in A that can reach the tail) * (number of nodes in B that are reachable from the head). Let's denote for each node, the size of its "reachable set" downstream? Actually, if we direct edges, we can define for each node the set of nodes reachable from it. The total good pairs is sum |R(u)|.
    // For an edge u->v, the number of pairs (x,y) such that the path from x to y uses this edge is (number of nodes that can reach u) * (number of nodes reachable from v). Because any x that can reach u can then go u->v and then to any y reachable from v. And these pairs are exactly those whose path includes this edge.
    // So total good pairs = sum over edges e=(u->v) of (reach_to_u * reach_from_v).
    // Here reach_to_u = number of nodes x such that there is a directed path from x to u. reach_from_v = number of nodes y such that there is a directed path from v to y.
    // Note that reach_to_u includes u itself? Yes, path of length 0. But the problem counts distinct vertices, so if x=u, y=v, that's a good pair. So including u in reach_to_u and v in reach_from_v gives the pair (u,v). So it's correct to include self.
    // So total = sum_{u->v} (in_size(u) * out_size(v)), where in_size(u) is the number of nodes that can reach u (including u), out_size(v) is the number of nodes reachable from v (including v).
    // This is a very useful formula!

    // Let's verify with sample 1: edges: 3->5, 3->1, 1->2, 4->2.
    // Compute in_sizes and out_sizes.
    // Node 5: out=1 (itself), in: who can reach 5? 3 can, 5 can. So in(5)=2.
    // Node 3: out: can reach 5,1,2,3? 3->5, 3->1, 1->2, so 3 can reach 3,5,1,2. out(3)=4. in: who can reach 3? only 3 itself. in(3)=1.
    // Node 1: out: can reach 1,2. out(1)=2. in: who can reach 1? 3,1. in(1)=2.
    // Node 4: out: can reach 4,2. out(4)=2. in: who can reach 4? only 4. in(4)=1.
    // Node 2: out: 2. out(2)=1. in: who can reach 2? 3,1,4,2. in(2)=4.
    // Edges:
    // 3->5: in(3)*out(5) = 1*1 = 1? Wait out(5)=1, in(3)=1. Product=1. But the pair (3,5) is good. Correct.
    // 3->1: in(3)*out(1) = 1*2 = 2. Pairs crossing this edge: (3,1) and (3,2). Correct.
    // 1->2: in(1)*out(2) = 2*1 = 2. Pairs: (1,2) and (3,2). But (3,2) already counted in 3->1? Wait, (3,2) uses edges 3->1 and 1->2. It should be counted in exactly one edge? The formula says sum over edges of in(tail)*out(head). For pair (3,2), it uses edges 3->1 and 1->2. Which edge's product counts it? in(3)*out(1) counts pairs (x,y) where x can reach 3, y reachable from 1. Since 3 can reach 3, and 2 reachable from 1, (3,2) is counted in 3->1. Also in(1)*out(2) counts pairs where x can reach 1, y reachable from 2. 3 can reach 1, so (3,2) is also counted in 1->2. So it's double counted! That's a problem. The formula sum_{u->v} in(u)*out(v) overcounts pairs that use multiple edges. Because a pair (x,y) uses all edges on the path from x to y. So it would be counted for each edge on that path. So the sum is not the number of good pairs, but the sum over edges of (number of pairs whose path includes that edge). That sum equals sum_{good pair} (length of path). So it's the total length of all good paths, not the count of pairs.
    // So that formula is not directly the number of good pairs.

    // Let's correct: The number of good pairs is the number of pairs (x,y) with x!=y and path from x to y. This is not easily expressed as sum over edges of something simple without overcounting.

    // Alternative: In a tree, if we direct edges, the reachability is a partial order. The number of pairs is the number of pairs (u,v) such that u is an ancestor of v in the reachability DAG. Since the underlying graph is a tree, the reachability DAG is a subgraph of the transitive closure of the tree. Actually, if there is a directed path from u to v, it must follow the unique undirected path. So the edges on that path must all be directed from u towards v. So the condition is: for the unique path between u and v, every edge is directed away from u and towards v. That means if we look at the path, the directions must be consistent along the path.

    // This is equivalent to: Assign a direction to each edge. Then for any pair (u,v), they are good iff the path between them is a directed path.

    // Now, consider the tree. Let's pick an arbitrary root, say 1. For each edge, we can think of it as either pointing "up" (towards root) or "down" (away from root). Then a path between u and v goes up from u to LCA, then down to v. For this path to be directed from u to v, all edges on the up part must be directed towards the root? Wait, if u is deeper, going up means edges are from child to parent. So for the path from u to v, the segment from u to LCA must have all edges directed from child to parent (i.e., upwards). The segment from LCA to v must have all edges directed from parent to child (downwards). So the path is directed from u to v iff all edges on the u-LCA path are up-edges, and all edges on the LCA-v path are down-edges.
    // So a pair (u,v) is good iff u is in a "up-subtree" of LCA and v is in a "down-subtree" of LCA, and the directions are consistent.

    // This suggests we can assign each edge a direction: either up (towards root) or down (away from root). Then the good pairs are those where u and v are in different subtrees of some node, with u in a subtree connected by up-edges all the way to the node, and v in a subtree connected by down-edges all the way from the node.

    // This is still complex.

    // Let's look for a pattern. In both samples, the number of good pairs is exactly n. Notice that in any directed graph, the number of good pairs is at least n-1? Not necessarily. For n=2, max is 1. For n=3, if we have a path 1->2->3, good pairs: (1,2),(1,3),(2,3) = 3. If we have 1->2, 3->2, good pairs: (1,2),(3,2) = 2. So we can have less than n-1? n=3, n-1=2, we got 2. So it's possible to have exactly n? For n=3, n=3, we got 3 with the path. So n=3 works. For n=4, can we get 4? Let's try: star with center 1, leaves 2,3,4. Direct 2->1, 3->1, 1->4. Pairs: 2->1, 2->4, 3->1, 3->4, 1->4 = 5. Too many. Direct 2->1, 1->3, 1->4: pairs: 2->1,2->3,2->4,1->3,1->4 = 5. Direct all leaves to center: 2->1,3->1,4->1: pairs: 2->1,3->1,4->1 = 3. Not 4. Direct center to all leaves: 1->2,1->3,1->4: pairs = 3. So star cannot give 4. What about a path 1-2-3-4? Direct all 1->2->3->4: pairs = 6. Direct 1->2, 2<-3, 3->4? That is 1->2, 3->2, 3->4. Pairs: 1->2, 3->2, 3->4. Also 1 can reach 2, but can 1 reach 4? No, because 2<-3 edge is opposite. So pairs: (1,2), (3,2), (3,4) = 3. Not 4. Direct 1->2, 2->3, 4->3: pairs: 1->2,1->3,2->3,4->3 = 4. Yes! Tree: 1-2-3-4. Direct 1->2, 2->3, 4->3. Good pairs: (1,2),(1,3),(2,3),(4,3) = 4. So n=4 works. This is a path with a "V" shape at the end? Actually it's a path where the last edge is directed towards the middle. So the structure is: a directed path from 1 to 3, and a leaf 4 pointing to 3. So it's a broom: path 1->2->3, and leaf 4->3. This matches our earlier broom model but with off-path node pointing to the path. Let's compute with broom formula: P = 1->2->3, k=3. Off-path: 4 attached to 3 (i=3). m=1. Total pairs = k(k-1)/2 + m(k+1) - sum i_u = 3*2/2 + 1*4 - 3
