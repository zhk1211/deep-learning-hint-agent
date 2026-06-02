#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<vector<int>> adj(n + 1);
    vector<pair<int, int>> edges;
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
        edges.emplace_back(u, v);
    }

    // Check if there is a vertex with degree >= 3
    int root = -1;
    for (int i = 1; i <= n; i++) {
        if (adj[i].size() >= 3) {
            root = i;
            break;
        }
    }

    if (root == -1) {
        // Tree is a path
        if (n == 2) {
            cout << "NO\n";
        } else {
            cout << "YES\n";
            // Direct all edges in one direction along the path
            // Find an endpoint
            int start = -1;
            for (int i = 1; i <= n; i++) {
                if (adj[i].size() == 1) {
                    start = i;
                    break;
                }
            }
            vector<int> path;
            vector<bool> vis(n + 1, false);
            function<void(int)> dfs = [&](int u) {
                vis[u] = true;
                path.push_back(u);
                for (int v : adj[u]) {
                    if (!vis[v]) {
                        dfs(v);
                    }
                }
            };
            dfs(start);
            for (int i = 0; i < n - 1; i++) {
                cout << path[i] << " " << path[i + 1] << "\n";
            }
        }
        return;
    }

    // Root the tree at the vertex with degree >= 3
    vector<int> parent(n + 1, 0);
    vector<int> depth(n + 1, 0);
    vector<vector<int>> children(n + 1);
    vector<bool> visited(n + 1, false);
    queue<int> q;
    q.push(root);
    visited[root] = true;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                parent[v] = u;
                depth[v] = depth[u] + 1;
                children[u].push_back(v);
                q.push(v);
            }
        }
    }

    // Find leaves
    vector<int> leaves;
    for (int i = 1; i <= n; i++) {
        if (children[i].empty()) {
            leaves.push_back(i);
        }
    }

    // We need exactly n good pairs
    // Strategy: direct edges from root to leaves, except one leaf which is directed towards root
    // This gives n good pairs: all pairs (u, v) where u is ancestor of v, except those involving the special leaf
    // Actually, we need to count exactly n good pairs.
    // Let's direct all edges away from root, then reverse the path from one leaf to root.
    // This creates a directed tree where all edges go away from root except one path going towards root.
    // The number of good pairs will be: total pairs in the tree if all edges directed away from root = sum of subtree sizes - 1 for each node? Actually, if all edges go away from root, then good pairs are all ancestor-descendant pairs. Number of such pairs = sum_{u} (size_subtree(u) - 1) = n*(n-1)/2? No, that's for all pairs. Actually, in a rooted tree with edges directed away from root, the number of good pairs is exactly the number of pairs (u, v) such that u is an ancestor of v. That's sum_{u} (size_subtree(u) - 1). That's too many.
    // We need exactly n good pairs.
    // If we reverse the path from one leaf to root, then the only good pairs are:
    // - All pairs (u, v) where u is an ancestor of v and v is not on the reversed path? Actually, let's analyze.
    // Let the special leaf be L. Reverse edges on path from L to root.
    // Then edges: root -> children except the one on path; on path: L -> parent -> ... -> root.
    // Good pairs: 
    // 1. From any node on the path (except L?) to nodes in subtrees branching off the path? 
    // Actually, we can compute exactly: The number of good pairs will be n.
    // Let's verify: For a star with center root and leaves, if we direct all edges away from root except one leaf directed to root, then good pairs: (root, all other leaves) = degree-1, plus (that special leaf, root) = 1, plus (that special leaf, ?) no, because from that leaf we can only go to root, and from root to other leaves, so (special leaf, other leaves) are also good. So total = (root to other leaves) + (special leaf to root) + (special leaf to other leaves) = (deg-1) + 1 + (deg-1) = 2*deg -1. For n=5, deg=4, that's 7, not 5.
    // So that doesn't give n.

    // Alternative approach: We need exactly n good pairs. 
    // Consider a directed tree. The number of good pairs is the number of pairs (u,v) with a directed path from u to v.
    // In a tree, if we direct edges, the resulting graph is a DAG. The number of good pairs equals the number of pairs (u,v) such that u is an ancestor of v in the directed sense.
    // We can think of it as: choose a root, direct some edges away, some towards. 
    // It is known that the number of good pairs in a directed tree is exactly the number of pairs (u,v) such that the unique path between them is directed from u to v.
    // This is equivalent to: there exists a node r such that all edges on the path from r to u are directed towards u? Not exactly.
    // Actually, a directed tree is a tournament? No.
    // Let's think differently: We need exactly n good pairs. n is small relative to n*(n-1)/2.
    // Notice that in a directed path of length n, the number of good pairs is n*(n-1)/2 if all edges same direction, but if we flip one edge, it breaks.
    // For n=2, any direction gives 1 good pair, but we need 2 good pairs? Wait, n=2, we need exactly 2 good pairs. But there are only 2 vertices, so maximum good pairs is 1 (since (1,2) or (2,1) but not both). So n=2 is impossible. That matches sample: third test case n=2 outputs NO.
    // For n=3, we need 3 good pairs. Maximum possible is 3 (if it's a directed path 1->2->3, good pairs: (1,2),(1,3),(2,3) = 3). So for a path of 3, we can achieve 3. For a star of 3 (which is also a path), same.
    // For n=4, we need 4 good pairs. A directed path of 4 gives 6 good pairs. Too many. Can we get exactly 4? Sample 4: n=4, tree is a path? Edges: 3-1, 1-2, 2-4. That's a path 3-1-2-4. Output: YES, 1->3, 2->1, 2->4. Let's check good pairs: 
    // Edges: 1->3, 2->1, 2->4.
    // Paths: 2->1->3, so (2,1),(2,3),(1,3) good. Also 2->4, so (2,4) good. Total 4. Yes.
    // So for a path of 4, we can achieve 4 by directing edges not all same direction.
    // For n=5, sample 1: tree is not a path (has a degree 3 node? Actually edges: 1-2,2-4,1-3,3-5. So 1 has degree 3? 1 connected to 2,3. 2 connected to 1,4. 3 connected to 1,5. So degrees: 1:2, 2:2, 3:2, 4:1, 5:1. It's a path? Wait, 4-2-1-3-5 is a path. So it is a path of 5. Output: YES, 1->2, 3->1, 3->5, 4->2. Let's check: edges: 1->2, 3->1, 3->5, 4->2. Good pairs: 3->1, 3->2 (via 1), 3->5, 1->2, 4->2. That's 5. So for a path of 5, we can get 5.
    // Sample 2: n=5, tree is a star? Edges: 1-2,1-3,1-4,4-5. So 1 connected to 2,3,4; 4 connected to 1,5. So degrees: 1:3, 4:2, others:1. Not a path. Output: YES, 2->1, 3->1, 4->1, 5->4. Good pairs: (2,1),(3,1),(4,1),(5,4),(5,1) = 5. So for this tree, we can get 5.
    // So it seems possible for all n>=3? But sample 3 says n=2 is NO. So maybe for n>=3 it's always possible? Let's check n=3: path of 3, we can direct all same direction to get 3. So YES.
    // Is it always possible for n>=3? Let's test n=4 star (center with 3 leaves). Can we get 4 good pairs? 
    // Star with center 1, leaves 2,3,4. We need 4 good pairs.
    // Try: direct 2->1, 1->3, 1->4. Good pairs: (2,1), (2,3), (2,4), (1,3), (1,4) = 5. Too many.
    // Try: direct 2->1, 3->1, 1->4. Good pairs: (2,1), (3,1), (2,4)? 2->1->4 yes, (3,4)? 3->1->4 yes, (1,4). Also (2,3)? No, 2->1<-3, no path. So pairs: (2,1),(3,1),(2,4),(3,4),(1,4) = 5.
    // Try: direct 2->1, 3->1, 4->1. Good pairs: (2,1),(3,1),(4,1) = 3. Not 4.
    // Try: direct 1->2, 1->3, 4->1. Good pairs: (1,2),(1,3),(4,1),(4,2),(4,3) = 5.
    // Try: direct 1->2, 3->1, 4->1. Good pairs: (1,2),(3,1),(4,1),(3,2)? 3->1->2 yes, (4,2)? 4->1->2 yes. Total: (1,2),(3,1),(4,1),(3,2),(4,2) = 5.
    // It seems we always get either 3 or 5. Can we get 4? 
    // What if we direct 2->1, 1->3, 4->1? Then edges: 2->1, 1->3, 4->1. Good pairs: (2,1),(2,3),(4,1),(4,3)? 4->1->3 yes, (1,3). Also (2,4)? 2->1<-4 no. So pairs: (2,1),(2,3),(4,1),(4,3),(1,3) = 5.
    // What about 2->1, 3->1, 1->4? Already did, 5.
    // What about 1->2, 1->3, 1->4? 6 pairs: (1,2),(1,3),(1,4) = 3? Wait, from 1 we can go to 2,3,4. From others no outgoing. So only 3 pairs. Actually, 1->2, 1->3, 1->4 gives pairs (1,2),(1,3),(1,4) = 3.
    // So for star with 3 leaves, possible good pairs counts: 3 (all outward), 5 (one inward, two outward), 6? If we direct two inward, one outward: say 2->1, 3->1, 1->4. Pairs: (2,1),(3,1),(2,4),(3,4),(1,4) = 5. If all inward: 2->1,3->1,4->1: pairs: (2,1),(3,1),(4,1) = 3. So only 3 or 5. 4 is impossible. But n=4, we need 4. So for star with 3 leaves (n=4), it's impossible? But wait, the tree in sample 4 is a path of 4, which is possible. So for n=4, it depends on the tree structure. 
    // So the problem is not trivial: we need to determine if possible and construct.

    // Let's analyze the number of good pairs in a directed tree.
    // A directed tree is an orientation of a tree. It is a DAG. The number of good pairs is the number of pairs (u,v) with a directed path.
    // In a tree, any orientation has a unique node with outdegree 0? Not necessarily.
    // Actually, in any orientation of a tree, there is at least one source and at least one sink. But we can have multiple.
    // Let's think in terms of "good pairs" = number of pairs (u,v) such that u can reach v.
    // This is equivalent to: for each node, count how many nodes it can reach.
    // Alternatively, consider the tree as undirected. For each edge, if we direct it u->v, then u can reach v and everything v can reach. 
    // We can compute the number of good pairs by rooting the tree arbitrarily and using DP? But the orientation is what we need to choose.

    // Let's try to find a necessary and sufficient condition.
    // Suppose we have a directed tree. Consider the condensation? No cycles.
    // Let's count good pairs. For each node u, let out[u] be the set of nodes reachable from u. Then total = sum |out[u]|.
    // In a tree, if we direct edges, the reachability is determined by the unique path. u can reach v iff the path from u to v is directed from u to v.
    // So the condition is: for every edge on the path, its direction matches the direction from u to v.
    // This means that if we look at the orientation, the set of good pairs is exactly the set of pairs (u,v) such that on the path, all edges point away from u towards v.
    // This is equivalent to saying that there is no "alternating" pattern? Actually, it means that the orientation must be such that there is a node r where all edges point away from r? Not exactly, because in sample 4, edges: 1->3, 2->1, 2->4. Here, 2 can reach 1 and 4 and 3. 1 can reach 3. So reachability: 2 reaches {1,3,4}; 1 reaches {3}; 3 reaches {}; 4 reaches {}. Total = 3+1+0+0=4. There is no single root with all edges away. Instead, there is a node 2 that can reach everything except itself? Actually 2 reaches 1,3,4. So 2 is a source that reaches all others. 1 reaches 3. So it's like a source 2, and then 1 is also a source for 3? But 1 is reachable from 2. So it's a DAG with a unique source? Here sources are 2 and 4? 4 has no incoming? Edges: 2->4, so 4 has incoming from 2. So 4 is not a source. Sources: nodes with indegree 0. Indegrees: 1: from 2; 3: from 1; 4: from 2; 2: indegree 0. So only 2 is source. Sinks: 3 and 4 (outdegree 0). So it's a DAG with one source and two sinks. The number of good pairs is the number of pairs (u,v) where u is an ancestor of v in the DAG. This is like the number of pairs in a poset that is a tree order? Actually, the reachability relation is a partial order. In a tree orientation, the reachability relation is exactly the ancestor relation in the directed tree if we view it as a rooted tree with edges directed away from the root? But here, edges are not all away from a single root. However, if we take the source 2 as root, edges: 2->1, 2->4, and 1->3. So from 2, edges go to children 1 and 4; from 1, edge goes to 3. So it is a branching away from 2. So it is a rooted tree directed away from 2. Indeed, if we have a unique source, then all edges must be directed away from that source? Not necessarily: consider a tree with edges 1->2, 3->2. Then sources are 1 and 3. So multiple sources. But if there is a unique source, then all edges are directed away from it? Suppose unique source r. Then for any edge (u,v), if it were directed towards r, then the other endpoint would have a path to r? Actually, if there is a unique source, then every other node must be reachable from that source. So for any node v != r, there is a directed path from r to v. This implies that on the path from r to v, all edges are directed away from r. So all edges on that path are directed away from r. But what about edges not on any path from r? In a tree, every node is on a unique path from r. So every edge is on some path from r to a node. Thus, all edges must be directed away from r. So a unique source implies a rooted tree with all edges directed away from the root. Similarly, a unique sink implies all edges directed towards the sink.
    // In sample 4, we have a unique source 2, so it's a rooted tree directed away from 2. The number of good pairs is then the number of pairs (u,v) where u is an ancestor of v in this rooted tree. That is sum_{u} (size_subtree(u) - 1). For the tree: root 2, children 1 and 4; 1 has child 3. Subtree sizes: 3:1, 1:2, 4:1, 2:4. Sum (size-1) = (1-1)+(2-1)+(1-1)+(4-1) = 0+1+0+3 = 4. Exactly n.
    // In sample 1: edges: 1->2, 3->1, 3->5, 4->2. Let's find sources and sinks. Indegrees: 1: from 3; 2: from 1 and 4; 3: indegree 0; 4: indegree 0; 5: from 3. So sources: 3 and 4. Sinks: 2 and 5? 2 has outdegree 0? 1->2, 4->2, so 2 has outdegree 0. 5 has outdegree 0. So multiple sources. Not a single source. But number of good pairs is 5 = n. How is that achieved? Let's compute reachability: 
    // 3 -> 1,5; 3->1->2, so 3 reaches {1,2,5}. 
    // 4 -> 2, so 4 reaches {2}.
    // 1 -> 2, so 1 reaches {2}.
    // 2 reaches {}.
    // 5 reaches {}.
    // Total = 3 + 1 + 1 + 0 + 0 = 5.
    // So it's not a single source. But the total is n.
    // In sample 2: edges: 2->1, 3->1, 4->1, 5->4. Sources: 2,3,5? 5 has indegree 0? 5->4, so 5 indegree 0. 4 has indegree from 5, outdegree to 1. 1 has indegree from 2,3,4. So sources: 2,3,5. Sinks: 1. Good pairs: 2->1; 3->1; 4->1; 5->4, 5->4->1 so 5->1; total 5.
    // So multiple sources can also yield n.

    // Let's try to find a pattern. We need exactly n good pairs.
    // Consider any orientation. Let's count good pairs. For each node u, let out[u] be the number of nodes reachable from u (including itself? Usually good pairs are distinct, so not including itself). So total = sum out[u].
    // We want sum out[u] = n.
    // Note that out[u] >= 0. Also, for any edge u->v, we have out[u] > out[v] because u can reach everything v can reach plus v itself? Actually, if u->v, then reachable(u) includes v and reachable(v). So out[u] >= 1 + out[v]. So out[u] > out[v].
    // Thus, along any directed edge, the out values strictly decrease.
    // Since the graph is a tree, there must be at least one sink (out=0). And at least one source (max out).
    // Let S be the set of sinks. For each sink, out=0.
    // For each source, out is some positive integer.
    // The sum of out over all nodes is n.
    // Also, note that if we have a directed edge u->v, then out[u] >= out[v] + 1. So the out values are like a potential.
    // In fact, we can think of out[u] as the number of nodes in the "subtree" reachable from u. If the orientation is a rooted tree away from a single root, then out[u] = size_subtree(u) - 1. Then sum out = sum (size-1) = (sum size) - n. Sum of subtree sizes in a rooted tree is sum depth? Actually, sum of subtree sizes is sum over nodes of (number of descendants including itself). That sum is equal to sum of (depth(v) + 1) if we root at the source? Not exactly. But we know that in a rooted tree, sum of subtree sizes = sum of (depth(v) from root + 1)? Let's check: root depth 0, size n. Its children depth 1, size of their subtrees... Actually, sum of subtree sizes = sum_{v} (number of ancestors of v including v) = sum_{v} (depth(v) + 1). So sum (size - 1) = sum depth(v). So total good pairs = sum depth(v). For a rooted tree directed away from root, the number of good pairs is exactly the sum of depths of all nodes (with root depth 0). So we need sum depths = n.
    // In a tree, sum of depths depends on the structure. For a star with root center, depths: center 0, leaves 1. Sum depths = number of leaves = n-1. So good pairs = n-1. For a path rooted at one end, depths: 0,1,2,...,n-1. Sum = n(n-1)/2. So we can get various values.
    // But our orientation doesn't have to be a single source. However, we can always transform any orientation into a "canonical" form? Maybe we can always achieve exactly n by a specific construction if the tree is not a path? Wait, sample 1 is a path of 5, and we got n=5. Sample 4 is a path of 4, got 4. Sample 2 is not a path, got 5. So both path and non-path can achieve n. But sample 3 n=2 is impossible. Also, we saw star with 3 leaves (n=4) might be impossible? Let's test star with 3 leaves (center 1, leaves 2,3,4). Can we get 4? We tried all orientations? Let's systematically check all orientations of a 4-star. There are 2^3 = 8 orientations. We already saw some give 3, some give 5. Let's list all:
    // Edges: (1,2), (1,3), (1,4). Direct each either 1->x or x->1.
    // Let a,b,c in {0,1} where 0 means 1->x, 1 means x->1.
    // Compute out for each node.
    // If 1->x, then 1 can reach x. If x->1, then x can reach 1.
    // Also, if 1->x and y->1, then y can reach x via 1.
    // Let's compute total good pairs for each case:
    // (0,0,0): 1->2,3,4. out(1)=3, others 0. Total=3.
    // (1,1,1): 2,3,4->1. out(2)=1 (reaches 1), out(3)=1, out(4)=1, out(1)=0. Total=3.
    // (1,0,0): 2->1, 1->3, 1->4. out(2)=1 (1) + out(1)? Actually, 2 reaches 1, and from 1 reaches 3,4. So out(2)=3 (1,3,4). out(1)=2 (3,4). out(3)=0, out(4)=0. Total=5.
    // (0,1,0): symmetric, 5.
    // (0,0,1): symmetric, 5.
    // (1,1,0): 2->1, 3->1, 1->4. out(2)=2 (1,4), out(3)=2 (1,4), out(1)=1 (4), out(4)=0. Total=5.
    // (1,0,1): symmetric, 5.
    // (0,1,1): symmetric, 5.
    // So only 3 or 5. 4 is impossible. So for n=4, star is impossible. But path of 4 is possible. So the answer depends on the tree structure.

    // Let's analyze the condition for a tree to have an orientation with exactly n good pairs.
    // We need sum out[u] = n.
    // Note that out[u] is the number of nodes reachable from u.
    // Consider the functional graph? No.
    // Let's think about the number of good pairs in terms of the orientation.
    // For each edge, if we direct it u->v, then u can reach v and everything v can reach. So out[u] >= out[v] + 1.
    // This implies that if we consider the out values, they are a strictly decreasing function along any directed path.
    // In particular, the maximum out value is at some source, and minimum 0 at sinks.
    // Since the graph is a tree, the out values must be consistent: for any two adjacent nodes, one can reach the other or not? Actually, if there is an edge between u and v, either u->v or v->u. In either case, one can reach the other. So for every edge, the out values of its endpoints differ by at least 1, and the one with higher out can reach the one with lower out.
    // So the out values form a "grading" of the tree: we can assign an integer out[u] >= 0 to each node such that for every edge, the out values differ by at least 1, and the direction is from higher to lower. Moreover, the out value must equal the number of nodes reachable from u. This means that if we have out[u] = k, then there must be exactly k nodes v (including possibly u? No, not including u) such that there is a directed path from u to v. And these nodes must have out values strictly less than k, and they must be exactly the nodes in the "downward" direction.
    // Actually, if we direct edges from higher out to lower out, then the reachable set from u is exactly the set of nodes that can be reached by following edges to lower out values. In a tree, this is exactly the connected component of u in the graph where we only keep edges directed away from u? But since it's a tree, if we direct all edges from higher out to lower out, then from any node, the reachable set is the set of nodes in its "subtree" when we consider the tree rooted at the global maximum? Not necessarily, because there could be multiple local maxima. But if we have multiple sources, then there are nodes with no incoming edges. The out values must be consistent: if u->v, then out[u] > out[v]. So the out values are a topological ordering. In a tree, any orientation is a DAG, and we can assign a topological order. The out value is the number of nodes reachable, which is the number of nodes that are "greater" in the topological order? Actually, if we assign a linear order, out[u] is the number of nodes after u that are reachable. But it's not simply the position.

    // Let's try to find a necessary condition. Sum out = n.
    // Also, for each edge u->v, out[u] >= out[v] + 1. Summing this over all edges? Not directly.
    // Consider the sum of out over all nodes. We have n nodes. The average out is 1.
    // Since out values are non-negative integers, and they strictly decrease along edges, the out values must be small.
    // In fact, if there is a node with out >= 2, then it must have at least two descendants with out values decreasing. 
    // Let's consider the tree. Suppose we have an orientation. Let's count the number of pairs (u,v) with u->v edge. That's n-1 edges. Each such edge contributes to out[u] at least 1 for v, plus possibly more. 
    // Another perspective: The number of good pairs is exactly the number of pairs (u,v) such that the path from u to v is directed from u to v. This is equivalent to saying that there is no "reverse" edge on that path.
    // So if we look at the undirected tree, the orientation partitions the tree into directed paths? 
    // Actually, if we consider the set of edges, each edge has a direction. A good pair (u,v) means that on the unique path between u and v, all edges point from u to v. This means that if we traverse from u to v, we never go against an edge direction.
    // So the good pairs are exactly the pairs where the path is a directed path.
    // This is equivalent to saying that the orientation is a "poset" that is a tree. The number of good pairs is the number of comparable pairs in this poset.
    // We want exactly n comparable pairs.
    // In a poset that is a tree (i.e., its Hasse diagram is a tree), the number of comparable pairs is the number of pairs (u,v) where u < v. This is the total number of pairs minus the number of incomparable pairs.
    // In a tree poset, two nodes are incomparable if and only if their unique path has a "valley", i.e., there is a node where the path goes up then down? Actually, in a poset, the cover relations are the edges. If we have a tree, the partial order is the transitive closure of the directed edges. Two nodes are comparable if one is an ancestor of the other in the directed sense. Incomparable means that the path between them has at least one edge pointing towards the path's interior? More precisely, if we consider the tree, and we direct edges, then the path between u and v is a sequence of edges. For u to reach v, all edges must be directed from u to v. If there is any edge directed opposite, then u cannot reach v. Similarly, v cannot reach u if there is an edge directed opposite. So they are incomparable if the path has at least one edge in each direction? Actually, if the path has edges in both directions, then neither can reach the other. If all edges are in one direction, then one can reach the other. So incomparability occurs exactly when the path has a "peak" or "valley"? Let's think: Suppose the path from u to v has edges e1, e2, ..., ek. If all are directed from u to v, then u reaches v. If all are directed from v to u, then v reaches u. If there is a mix, then there must be some edge directed towards u and some towards v? Actually, if there is at least one edge in each direction, then there is a node where the direction changes. At that node, the path goes in and out? For example, u -> a <- v. Then u can reach a, v can reach a, but u and v cannot reach each other. So they are incomparable. So incomparable pairs are exactly those where the path has a node with indegree 2 on the path? Not exactly, but there is a "sink" on the path. More generally, two nodes are incomparable iff the path between them contains a node that is a sink on that path (i.e., both incident edges on the path point towards it) OR a source (both point away)? Actually, if the path has a source, then from that source you can go to both, so they are both reachable from the source, but they might not be comparable to each other. For example, a -> b <- c. Here a and c are incomparable. The path a-b-c has b as a sink (edges point to b). If we have a <- b -> c, then a and c are incomparable? b can reach both a and c, but a and c cannot reach each other. So they are incomparable. So incomparable pairs occur when the path has a node with outdegree 2 on the path? Actually, in a <- b -> c, b has outdegree 2 on the path. In a -> b <- c, b has indegree 2 on the path. So in general, two nodes are incomparable if the path between them contains a node that is either a local maximum (outdegree 2 on the path) or a local minimum (indegree 2 on the path)? But wait, if the path is a -> b -> c, then a and c are comparable (a reaches c). So comparability requires that the path is monotonic in the direction. That means there is no "turn". So the path must be a directed path. So incomparable pairs are those where the path is not a directed path.

    // Now, we want the number of comparable pairs to be exactly n.
    // Total pairs is n(n-1)/2. So we want exactly n comparable pairs, meaning n(n-1)/2 - n = n(n-3)/2 incomparable pairs.
    // For n=4, total pairs=6, comparable=4, incomparable=2.
    // For n=5, total=10, comparable=5, incomparable=5.
    // For n=3, total=3, comparable=3, incomparable=0. (Path directed consistently)
    // For n=2, total=1, comparable=1, but we need 2? Wait, n=2, we need exactly n=2 good pairs. But total pairs is 1. So
