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
        edges.push_back({u, v});
    }
    
    if (n == 2) {
        cout << "NO\n";
        return;
    }
    
    vector<int> deg(n + 1);
    for (int i = 1; i <= n; i++) deg[i] = adj[i].size();
    
    vector<int> leaves;
    for (int i = 1; i <= n; i++) {
        if (deg[i] == 1) leaves.push_back(i);
    }
    
    if (leaves.size() < 2) {
        cout << "NO\n";
        return;
    }
    
    // We need exactly n good pairs.
    // In a directed tree, number of good pairs = n - number of leaves in the directed tree
    // where a leaf in directed tree is a vertex with out-degree 0.
    // We need n good pairs => n - L = n => L = 0, impossible.
    // Wait, re-read hints: "The number of good pairs in the tree is equal to the total number of vertices minus the number of leaves in the tree."
    // In directed tree, leaf = vertex with out-degree 0.
    // So good pairs = n - L. We want n good pairs => L = 0, impossible.
    // But sample has n=5, good pairs=5. Let's check sample 1: vertices 3,1,2,4,5.
    // Out-degrees: 3->1, 3->5, 1->2, 4->2. Out-deg: 3:2, 1:1, 4:1, 2:0, 5:0.
    // Leaves (out=0): 2 and 5. L=2. n - L = 5 - 2 = 3, not 5.
    // So hint 5 is wrong? Let's re-read: "The number of good pairs in the tree is equal to the total number of vertices minus the number of leaves in the tree."
    // Maybe they mean leaves in the underlying undirected tree? No.
    // Let's compute good pairs in sample 1: (3,5), (3,1), (3,2), (1,2), (4,2) = 5.
    // n=5, leaves in directed tree (out=0) = 2 (2 and 5). 5-2=3 !=5.
    // So hint 5 is not correct as stated. Let's think.
    // Actually, in a DAG that is a directed tree (edges directed away from some root?), good pairs = sum over all vertices of (size of reachable set - 1).
    // For a directed tree where edges are oriented from parent to child (arborescence), number of good pairs = sum_{v} (subtree_size(v) - 1) = total pairs in each subtree.
    // That equals sum_{v} (sz[v] - 1) = (sum sz[v]) - n.
    // Sum of subtree sizes in a rooted tree = sum_{v} depth(v) + something? Actually sum of subtree sizes = sum_{v} (number of descendants + 1) = n + sum_{v} number of descendants.
    // Number of descendants sum = total good pairs? If root is r, then good pairs = number of pairs (u,v) where u is ancestor of v. That's exactly sum_{v} (depth(v)).
    // So good pairs = sum of depths (with root depth 0).
    // In sample 1, root? Edges: 3->1, 3->5, 1->2, 4->2. This is not a single arborescence; it has two sources: 3 and 4. It's a forest of two arborescences.
    // Good pairs = pairs within each arborescence. For 3: depths: 3(0),1(1),5(1),2(2). Sum depths = 0+1+1+2=4. For 4: depth 0, 2 is already counted? Wait 2 is child of 1 and 4? Actually 2 has two incoming edges: from 1 and 4. That's not a tree anymore, it's a DAG with a vertex having indegree 2. But the problem says "assign a direction to each edge of this tree", so the underlying graph is a tree, but directed edges can form a DAG, not necessarily a directed tree. So hints about subtrees might be misleading.
    // Let's re-read problem: "assign a direction to each edge of this tree" -> we orient each edge arbitrarily. The resulting directed graph has exactly n-1 edges, and underlying undirected graph is a tree. It may have cycles? No, because it's a tree, orienting edges cannot create cycles since tree has no cycles. So it's a DAG. But it's not necessarily a directed tree (arborescence) because a vertex can have indegree >1.
    // Good pairs = number of reachable pairs (u,v) with u!=v.
    // We need exactly n good pairs.
    // Let's analyze small n.
    // n=2: edges 1-2. Orientations: 1->2: good pairs: (1,2) =1. 2->1: (2,1)=1. So max good pairs =1, need n=2 -> impossible. Sample 3 says NO.
    // n=3: tree 1-2, 2-3. Orientations? Let's try to get 3 good pairs.
    // If we orient 1->2, 2->3: good pairs: (1,2),(1,3),(2,3) =3. Yes! So n=3 possible.
    // n=4: star with center 1, leaves 2,3,4. Can we get 4 good pairs?
    // Orient all edges outward from center: 1->2,1->3,1->4. Good pairs: (1,2),(1,3),(1,4) =3. Not 4.
    // Orient all inward to center: 2->1,3->1,4->1. Good pairs: (2,1),(3,1),(4,1) =3.
    // Mix: 1->2, 3->1, 4->1. Good pairs: (1,2), (3,1), (4,1), (3,2)? 3->1->2 so (3,2) yes. (4,2) yes. Total: (1,2),(3,1),(4,1),(3,2),(4,2) =5. >4.
    // So we can get 5, but need 4. Can we get exactly 4? Try 1->2, 1->3, 4->1. Good: (1,2),(1,3),(4,1),(4,2),(4,3) =5. Still 5.
    // Try 2->1, 1->3, 1->4. Good: (2,1),(2,3),(2,4),(1,3),(1,4) =5.
    // Seems for star n=4, min good pairs? Let's find min. If we orient edges to create a directed path: 2->1, 1->3, 4->1? That's not a path. Tree is star. Any orientation: each leaf either points to center or away. Let a = number of leaves pointing to center, b = number pointing away, a+b=3. Center has indegree a, outdegree b. Good pairs: from each leaf pointing to center: can reach center and all leaves that center points to. So leaf i (inward) reaches center + b leaves. That's 1+b. From center: reaches b leaves. From outward leaves: reach nothing. Total = a*(1+b) + b. We want this =4. a+b=3. Try a=1,b=2: 1*(1+2)+2=3+2=5. a=2,b=1: 2*(1+1)+1=4+1=5. a=3,b=0: 3*(1+0)+0=3. a=0,b=3: 0+3=3. So possible totals: 3 or 5. Cannot get 4. So n=4 star impossible? But sample 4 has n=4 and output YES. Sample 4 tree: edges 3-1, 1-2, 2-4. It's a path 3-1-2-4. Let's check if we can get 4 good pairs. Path of 4 vertices: orient all same direction: 3->1->2->4: good pairs = 6 (all forward pairs). Too many. We need 4. Can we get 4? Sample output: 1->3, 2->1, 2->4. Let's compute: edges: 1->3, 2->1, 2->4. Reachability: 2->1, 2->4, 2->3 (via 1), 1->3. Also 2->1, 2->4, 2->3, 1->3. That's 4 pairs: (2,1),(2,4),(2,3),(1,3). Exactly 4. So path works.
    // So condition for exactly n good pairs?
    // Let's think about the structure. The directed graph is a DAG with underlying tree. Number of good pairs = number of pairs (u,v) such that there is a directed path from u to v.
    // In a tree orientation, every edge is a directed edge. The graph is a set of directed trees (arborescences) but with possible sharing of vertices? Actually, since underlying is a tree, if we have two edges pointing to the same vertex, that vertex has indegree 2, but outdegree can be anything. The graph is a collection of directed edges. There can't be cycles. It's a DAG. The reachability relation is a partial order.
    // We can think of it as: we have a tree. We orient each edge. This is equivalent to choosing a direction for each edge. The number of good pairs is the number of pairs (u,v) such that in the tree, the unique path between u and v is directed from u to v.
    // So a pair (u,v) is good iff all edges on the path from u to v are oriented towards v.
    // So we need exactly n such pairs.
    // Let's consider the tree. For each vertex, we can compute the number of good pairs starting from it? Or we can think about the "flow" of directions.
    // Another perspective: In any orientation of a tree, the number of good pairs is equal to the number of pairs (u,v) such that the path is consistently oriented. This is equivalent to: for each vertex, if we consider the orientation of edges incident to it, we can define something.
    // Let's root the tree arbitrarily. For each edge, we have a direction either parent->child or child->parent. A pair (u,v) is good if the path from u to v goes only in the direction of edges. This means if we consider the lowest common ancestor LCA of u and v, then the path from u to LCA must be all child->parent (upward), and from LCA to v must be all parent->child (downward). So u must be in the "upward" part and v in the "downward" part relative to LCA.
    // So a good pair corresponds to a vertex LCA such that u is in a subtree where edges are oriented towards LCA, and v is in a subtree where edges are oriented away from LCA.
    // This suggests we can compute the number of good pairs by considering each vertex as LCA and counting combinations of incoming and outgoing branches.
    // Let's define for each vertex, we have some incident edges. Some are directed towards it (in-edges), some away (out-edges). For a fixed vertex x, consider the connected components of the tree after removing x. Each component is connected to x by exactly one edge. That edge is either directed towards x or away from x. If it's towards x, then all vertices in that component can reach x (if the entire component is oriented consistently? Actually, for a vertex u in that component to reach x, the path from u to x must be entirely directed towards x. That requires that all edges on that path are directed towards x. But the orientation of edges within the component is not necessarily all towards x. So it's not simply "if the edge incident to x is towards x, then all vertices in that component can reach x". Only those vertices from which there is a directed path to x. So it's more complex.
    // However, note that the underlying graph is a tree. If we orient edges, the reachability relation is transitive. The number of good pairs can be computed by DP? Maybe we can find a necessary and sufficient condition for exactly n good pairs.
    // Let's try to find the maximum number of good pairs. What is the maximum? If we orient all edges towards a single sink? That would be all edges pointing to one vertex. Then from any vertex, you can reach the sink, but not necessarily others. Actually, if all edges point to a single vertex r, then for any u, the path from u to r is directed towards r. So u can reach r. Can u reach v? Only if v is on the path from u to r and the edges are towards r, which they are. So u can reach all vertices on the path from u to r. That means the good pairs are exactly the pairs (u,v) where v is an ancestor of u in the rooted tree (with root r). That's exactly the number of pairs where v is on the path from u to r. That's sum over u of (depth of u). In a tree, sum of depths can be up to O(n^2) for a line, max is n(n-1)/2. But we only have n-1 edges, so max good pairs is n(n-1)/2? Wait, if we have a line and orient all edges in one direction, say 1->2->3->...->n, then good pairs = n(n-1)/2. That's much larger than n. So we can get many good pairs. But we want exactly n, which is relatively small.
    // So we need to limit the number of good pairs to exactly n. That means the orientation must be such that very few pairs are good.
    // What is the minimum number of good pairs? If we orient edges to form a directed tree where every vertex has outdegree at most 1? Actually, if we orient edges such that every vertex has outdegree 1 except one sink with outdegree 0, then the graph is a functional graph, but since it's a tree, it's a directed tree pointing to a sink. In that case, good pairs = sum of depths = number of pairs (u,v) where v is on path from u to sink. That's at least n-1 (if star, sum depths = n-1). Can we get less? If we have multiple sinks? If there are multiple sinks, then some vertices cannot reach others. The minimum good pairs might be n-1? Let's test n=3: we got 3 good pairs with a path. Can we get 2? n=3, tree 1-2-3. Orient 1->2, 3->2. Good pairs: (1,2), (3,2) =2. That's n-1=2. So minimum is n-1? For n=4 path, can we get 3? Orient 1->2, 2<-3, 3->4? Let's try: 1->2, 3->2, 3->4. Good: (1,2), (3,2), (3,4) =3. Yes, n-1=3. So minimum good pairs seems to be n-1. Is it always possible to get n-1? Yes, by orienting all edges towards a single edge? Actually, if we pick a leaf and orient all edges towards it, we get a sink, and good pairs = sum depths = n-1 for a star? For a star with center 1, leaves 2,3,4, all edges towards center: 2->1,3->1,4->1. Good pairs: (2,1),(3,1),(4,1) =3 = n-1. So minimum is n-1. Can we get less than n-1? If we have multiple sinks, some vertices might not reach any other? But every vertex has at least one incident edge. If a vertex has outdegree 0, it's a sink. If we have two sinks, then there is a path between them? In a tree, between any two vertices there is a unique path. If both are sinks, then all edges on the path must be directed away from both? Impossible because edges have direction. Actually, if we have two sinks, the path between them must have edges directed towards both ends? That would require an edge pointing in both directions, impossible. So there can be at most one sink in the whole graph? Wait, consider 1->2<-3->4. Sinks: 2 and 4. Path between 2 and 4: 2-3-4. Edges: 2<-3 and 3->4. So from 2 to 4: edge 2<-3 is opposite, so no path. From 4 to 2: 4->3 is opposite to 3->2? Actually 3->2 is edge 3->2, so from 4 to 2: 4->3->2, that's a path! So 4 can reach 2. So 2 is not a sink? Sink means outdegree 0. 2 has outdegree 0, but 4 can reach 2, so 2 is reachable from 4. So 2 is a sink but there is a path from 4 to 2. So multiple sinks can exist, and there can be paths between them. In a DAG, sinks can have paths from other sinks? No, if there is a path from u to v, then u can reach v, so v is reachable from u. If both are sinks, u has outdegree 0, so u cannot have an outgoing edge, but a path from u to v would require u to have an outgoing edge. So if u is a sink, there cannot be a path from u to any other vertex. So sinks cannot reach each other. In my example, 4 can reach 2, so 4 is not a sink because it has an outgoing edge (4->3). So 4 has outdegree 1. So only 2 is sink. So indeed, in any orientation of a tree, there can be at most one sink? Let's check: Suppose we have two sinks u and v. Since the graph is weakly connected (underlying tree), there is a path between u and v. On that path, there must be a vertex with two outgoing edges? Actually, if both are sinks, then all edges incident to u must be incoming to u. Similarly for v. On the path between u and v, the edge incident to u must be directed towards u. The edge incident to v must be directed towards v. Then there must be a vertex on the path where the direction changes, which would have two outgoing edges or two incoming? Let's trace: u --a-- ... --b-- v. Edge u-a is towards u, so a->u. Edge b-v is towards v, so b<-v? Actually v is sink, so edge incident to v must be incoming to v, so b->v. So we have a->u and b->v. On the path from a to b, the edges must be directed consistently? They could be a mix. But consider the vertex where the direction flips from towards u to towards v. That vertex would have one edge pointing towards u and one pointing towards v, so it has outdegree 2? Actually, if an edge points towards u, that means it's directed from the next vertex towards u. So if we go from u to v, the edge u-a is a<-u? Wait, if u is sink, all edges incident to u are incoming to u. So edge between u and a is a->u. So direction is a->u. Similarly, edge between v and b is b->v. So on the path a ... b, the edges could be directed in various ways. But there must be at least one vertex with outdegree >=2? Not necessarily. Consider a path of 3 vertices: u - x - v. u sink: x->u. v sink: x->v. Then x has outdegree 2. So x is not a sink. So sinks u and v are sinks, but there is a path from x to both. So multiple sinks can exist, but they cannot reach each other. So minimum good pairs: can we have less than n-1? Suppose we have a tree and we orient edges to minimize good pairs. Each edge orientation contributes to some paths. The total number of good pairs is the number of pairs (u,v) with directed path. In any orientation, consider the sum of outdegrees. Since there are n-1 edges, sum of outdegrees = n-1. The number of vertices with outdegree 0 (sinks) is at least 1. For each sink, no good pairs start from it. For other vertices, they have outdegree >=1. But a vertex with outdegree 1 might still not reach many vertices if the path stops. The minimum good pairs might be exactly n-1. Let's test n=5, can we get 4? We need n=5 good pairs in sample, they got 5. Can we get 4? If minimum is n-1=4, then we need exactly n, which is n, so we need one more than minimum. So the problem asks if we can achieve exactly n good pairs. Since minimum is n-1, n is just one above minimum. So we need to find if we can get exactly n good pairs, and if so, construct.
    // Let's verify minimum is n-1. Is it always possible to get n-1? Yes, by making a single sink and all edges pointing towards it, we get a directed tree with root as sink. Then good pairs = sum of depths. In a tree, sum of depths can be as low as n-1 (star) and as high as n(n-1)/2 (line). So we can achieve n-1 by making the tree a star? But we cannot change the tree structure; it's given. If the tree is a line, making all edges point to one end gives sum depths = 0+1+2+...+(n-1) = n(n-1)/2, which is much larger than n-1. So we cannot always achieve n-1. The minimum good pairs depends on the tree structure. For a given tree, what is the minimum number of good pairs over all orientations? We want to know if we can achieve exactly n. So we need to find the minimum possible good pairs for the given tree. If min > n, then NO. If min <= n, can we always achieve n? Maybe we can increase good pairs by 1 by flipping one edge? Let's explore.
    // Let's formalize: For a tree T, an orientation is a function d: E -> {0,1} (direction). Good pairs = number of pairs (u,v) such that the unique path from u to v is directed from u to v.
    // We can think of this as: For each vertex, we can define its "reachability set". The total good pairs = sum_v |R(v)|, where R(v) is the set of vertices reachable from v (excluding v).
    // Alternatively, we can count by edges: A pair (u,v) is good iff all edges on the path are oriented along the path. This is equivalent to saying that there is no edge on the path that is oriented opposite to the path direction.
    // So if we consider the orientation, we can define for each edge a direction. A path is good if it doesn't contain any "backward" edge relative to the path.
    // This is similar to: The orientation defines a partial order. The number of good pairs is the number of comparable pairs in this partial order.
    // In a tree, the partial order is a set of constraints. The number of comparable pairs can be computed by DP.
    // Let's root the tree arbitrarily. For each edge, we have a direction: either up (child -> parent) or down (parent -> child). A pair (u,v) is good if the path from u to v goes up from u to LCA and then down from LCA to v, and all edges on the up part are up, all edges on the down part are down. So u must be in a "upward" region from LCA, and v in a "downward" region.
    // This means that for each vertex x, if we look at the edges incident to it, some are up (from child to x), some are down (from x to child). For a pair (u,v) with LCA = x, u must be in a subtree of a child where the edge is up (child->x) and the entire path from u to that child is up? Actually, for u to reach x, the path from u to x must be entirely up. That means in the subtree of that child, all edges on the path from u to that child must be oriented towards x. That is a condition on the orientation within that subtree. Similarly, for v, the path from x to v must be entirely down.
    // So the number of good pairs with LCA x is: (sum over children c with edge c->x of (number of vertices in c's subtree that can reach x)) * (sum over children c with edge x->c of (number of vertices in c's subtree reachable from x)).
    // But note that a vertex u in c's subtree can reach x iff the entire path from u to c is oriented towards c, and the edge c->x is towards x. So it's exactly the number of vertices in that subtree that can reach c (including c itself) if we consider the orientation within that subtree. This suggests a DP.
    // Let's define for each vertex v, if we consider the subtree rooted at v (with parent p), we can compute two values: 
    // up[v] = number of vertices in subtree of v (including v) that can reach v via a directed path entirely within the subtree (i.e., all edges on path towards v).
    // down[v] = number of vertices in subtree of v (including v) that v can reach via a directed path entirely within the subtree (i.e., all edges on path away from v).
    // But note that the orientation of the edge between v and its parent p affects whether v can reach p or p can reach v. So we need to consider the direction of the edge (v,p).
    // Let's root the tree at 1. For each vertex v, we have children. For each child c, the edge (v,c) is either v->c or c->v.
    // If edge is v->c, then v can reach c, and through c, v can reach all vertices that c can reach (down[c]). Also, c cannot reach v via this edge (since it's opposite). So for v, the set of vertices reachable from v within its subtree is: v itself, plus for each child c with edge v->c, the entire down[c] (since v->c and then c can reach down[c]). Also, if edge is c->v, then c can reach v, so v can be reached from c, but v cannot reach c. So for v's reachability to children, only outgoing edges count.
    // Similarly, for vertices that can reach v within its subtree: v itself, plus for each child c with edge c->v, the entire up[c] (since up[c] can reach c, and c->v).
    // So we can compute for each vertex v:
    // down[v] = 1 + sum_{c: v->c} down[c]
    // up[v] = 1 + sum_{c: c->v} up[c]
    // These are sizes of reachability sets within the subtree, assuming the orientation inside the subtree is already processed.
    // Now, the total number of good pairs in the whole tree can be computed by considering each vertex as LCA? Actually, the total good pairs = sum over all vertices v of (number of pairs (u,w) such that LCA is v and u can reach v and v can reach w, with u != w). But careful: u and w could be the same? No, u!=w. Also, u could be v itself? If u=v, then the pair is (v,w) where v can reach w. That is counted when LCA is v? Actually, if u=v, then LCA is v, and the path is just down from v to w. So that's included if we consider u can be v. Similarly, w can be v. So the pairs with LCA v are: (u,w) where u is in the "up" set of v (including v), w is in the "down" set of v (including v), and u != w. The number of such pairs is up[v] * down[v] - 1 (since the pair (v,v) is not good). But wait, is every such pair actually good? If u can reach v and v can reach w, then by concatenation u can reach w. So yes, all such pairs are good. But are there good pairs whose LCA is not v? Every good pair has a unique LCA. So total good pairs = sum_{v} (up[v] * down[v] - 1). However, we must be careful: up[v] and down[v] as defined above only consider the subtree of v. But u could be outside the subtree of v? For LCA v, u must be in the subtree of v (since v is ancestor of u). Actually, if v is LCA of u and w, then both u and w are in the subtree of v, and they are in different child subtrees or one is v. So u is in the subtree of v. So up[v] should count vertices in the subtree of v that can reach v. That's exactly what we defined. Similarly, down[v] counts vertices in the subtree of v that v can reach. So the formula sum_{v} (up[v] * down[v] - 1) correctly counts all good pairs, provided we define up[v] and down[v] for the whole tree? But our DP defined up[v] and down[v] only within the subtree of v when rooted at 1. However, for a vertex v, its "up" set might also include vertices from outside its subtree? No, because if u is outside the subtree of v, then the path from u to v goes through v's parent. Then LCA of u and v would be an ancestor of v, not v. So for LCA v, both u and w must be in the subtree of v. So our subtree DP is sufficient if we compute up[v] and down[v] as the sizes of the sets within the subtree of v (including v) that can reach v and that v can reach, respectively. But note: the definition of up[v] includes v itself. So up[v] >= 1. Similarly down[v] >= 1.
    // Let's verify with sample 1: Tree: 1-2, 2-4, 1-3, 3-5. Root at 1.
    // Edges: 1-2, 1-3, 2-4, 3-5.
    // Sample orientation: 1->2, 3->1, 3->5, 4->2. Let's write directions relative to root 1:
    // 1->2 (down), 3->1 (up), 3->5 (down from 3? 3 is child of 1, edge 3->1 is up, so 3->5 is within 3's subtree: 3->5 is down from 3). 4->2: 4 is child of 2, edge 4->2 is up (since 2 is parent of 4, 4->2 is towards 2).
    // So orientation: 
    // 1: children 2 (down), 3 (up)
    // 2: child 4 (up)
    // 3: child 5 (down)
    // 4: leaf
    // 5: leaf
    // Compute DP bottom-up:
    // Leaf 4: up[4] = 1 (itself), down[4] = 1.
    // Leaf 5: up[5] = 1, down[5] = 1.
    // Vertex 2: child 4 with edge 4->2 (up). So up[2] = 1 + up[4] = 2. down[2] = 1 (no down children). So up[2]=2, down[2]=1.
    // Vertex 3: child 5 with edge 3->5 (down). So down[3] = 1 + down[5] = 2. up[3] = 1. So up[3]=1, down[3]=2.
    // Vertex 1: children 2 (down), 3 (up).
    // down[1] = 1 + down[2] = 1+1=2.
    // up[1] = 1 + up[3] = 1+1=2.
    // Now total good pairs = sum_v (up[v]*down[v] - 1):
    // v=1: 2*2-1=3
    // v=2: 2*1-1=1
    // v=3: 1*2-1=1
    // v=4: 1*1-1=0
    // v=5: 1*1-1=0
    // Total = 3+1+1 = 5. Matches sample.
    // So formula works!
    // Now we want total good pairs = n.
    // So we need sum_v (up[v]*down[v] - 1) = n.
    // Note that sum_v 1 = n, so sum_v up[v]*down[v] = 2n.
    // Also, up[v] and down[v] are integers >=1.
    // For each leaf v, up[v]=down[v]=1, so up[v]*down[v]=1.
    // For internal vertices, up[v]*down[v] >= 1.
    // We need sum = 2n.
    // Since there are n vertices, the average of up[v]*down[v] is 2.
    // So most vertices must have up[v]*down[v] = 1 or 2, and a few can be larger but then others must be smaller? But minimum is 1. So to have sum 2n, we need many vertices with product 1, some with product 2, and maybe some with product >2 but then we need others with product <1 impossible. So the only way is that for all vertices, up[v]*down[v] is either 1 or 2, and the number of vertices with product 2 is exactly n? Wait, sum = 2n. If we have k vertices with product 2, and the rest (n-k) with product 1, then sum = 2k + 1*(n-k) = n + k. We need n + k = 2n => k = n. So all vertices must have up[v]*down[v] = 2. But is that possible? For a leaf, product is 1. So leaves would have product 1, contradicting k=n. So maybe my formula for total good pairs is missing something? Let's re-evaluate.
    // In sample 1, n=5, sum of products = 4+2+2+1+1 = 10 = 2n. So sum is 2n. Leaves 4 and 5 have product 1. Vertices 2 and 3 have product 2. Vertex 1 has product 4. Sum = 4+2+2+1+1=10. So it's not all 2. So my equation sum = 2n is correct, but the distribution can have some >2 and some <2. The sum is exactly 2n. So we need sum_v up[v]*down[v] = 2n.
    // Now, up[v] and down[v] are determined by the orientation. We want to find if there exists an orientation such that sum = 2n.
    // Let's analyze the properties of up and down.
    // For each vertex v, up[v] is the number of vertices in its subtree that can reach v. This includes v and some vertices from subtrees of
