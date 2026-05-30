// Hint1
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, m;
    long long k;
    cin >> n >> m >> k;
    
    // The shortest path from (1,1) to (n,m) has length (n-1)+(m-1) = n+m-2
    long long min_len = n + m - 2;
    
    // If k < min_len, impossible
    if (k < min_len) {
        cout << "NO\n";
        return;
    }
    
    // The parity condition: any walk from (1,1) to (n,m) has length parity same as (n+m) mod 2
    // Because each step changes x+y by 1, so after L steps, parity of (x+y) is (1+1+L) mod 2 = L mod 2.
    // At the end, x+y = n+m, so L mod 2 = (n+m) mod 2.
    if ((k - min_len) % 2 != 0) {
        cout << "NO\n";
        return;
    }
    
    // Now we need to construct a coloring and a walk of length k+1 (k segments)
    // We can always do it by creating a cycle near the end that can be traversed back and forth
    // to adjust the length by any even number.
    
    cout << "YES\n";
    
    // We'll construct horizontal edges: n rows, m-1 columns
    vector<vector<char>> horiz(n, vector<char>(m-1, 'R'));
    // Vertical edges: n-1 rows, m columns
    vector<vector<char>> vert(n-1, vector<char>(m, 'R'));
    
    // We need to ensure there is a valid alternating walk.
    // Strategy: create a path from (1,1) to (n,m) that alternates colors.
    // Then add a small cycle (a 2x2 square) somewhere to allow length extension.
    // The cycle will be at the bottom-right: cells (n-1, m-1), (n-1, m), (n, m-1), (n, m).
    // We'll set colors so that the main path goes through this cycle in a way that we can loop.
    
    // Let's design the main path: go right along row 1 to column m, then down along column m to row n.
    // But we need alternating colors. We'll set colors along this path to alternate.
    // Path: (1,1) -> (1,2) -> ... -> (1,m) -> (2,m) -> ... -> (n,m)
    // Edges on this path: horizontal edges in row 1 from col 1 to m-1, and vertical edges in column m from row 1 to n-1.
    
    // We'll set them alternating starting with 'R' (or 'B') such that the cycle at the end can be colored consistently.
    // Actually, we need the cycle to have alternating colors around it so that we can go around multiple times.
    // A 2x2 cycle has 4 edges. To be able to traverse it back and forth while alternating colors,
    // the cycle must have colors that alternate around it. That means opposite edges should have same color?
    // Let's analyze: Suppose we enter the cycle at (n-1,m-1) from left? Actually we enter from (n-1,m-1) to (n-1,m)?
    // Better: We'll make the main path go down column m-1? Let's design carefully.
    
    // Alternative construction from known solution:
    // We can make the entire grid colored in a checkerboard pattern? But we need a specific walk.
    // Known trick: Color the grid such that there is a Hamiltonian path from (1,1) to (n,m) with alternating colors,
    // and also a 2x2 cycle at the end that can be used to add extra steps.
    // The cycle will be at the bottom-right corner: cells (n-1, m-1), (n-1, m), (n, m-1), (n, m).
    // We'll set the colors of the edges of this cycle to alternate: e.g., top edge (n-1, m-1)-(n-1,m) = R,
    // right edge (n-1,m)-(n,m) = B, bottom edge (n,m-1)-(n,m) = R, left edge (n-1,m-1)-(n,m-1) = B.
    // Then we can go around the cycle: (n-1,m-1) -> (n-1,m) -> (n,m) -> (n,m-1) -> (n-1,m-1) -> ...
    // This cycle has 4 edges, alternating colors, so we can loop any even number of extra steps.
    
    // Now we need to connect (1,1) to this cycle and then to (n,m) with alternating path.
    // We can make the path: from (1,1) go right to (1,m-1), then down to (n-1,m-1), then enter the cycle.
    // After looping, exit to (n,m).
    // Let's define the path edges:
    // Horizontal edges in row 1: (1,1)-(1,2), (1,2)-(1,3), ..., (1,m-2)-(1,m-1) -> these should alternate.
    // Vertical edges in column m-1: (1,m-1)-(2,m-1), ..., (n-2,m-1)-(n-1,m-1) -> alternate.
    // Then cycle edges as above.
    // Finally, from cycle to (n,m): we can go from (n-1,m) to (n,m) (vertical edge) or from (n,m-1) to (n,m) (horizontal edge).
    // The walk will end at (n,m). We need the last edge to be part of the alternating sequence.
    
    // Let's assign colors step by step to ensure alternation along the intended path.
    // We'll define the path sequence of edges:
    // 1. (1,1) -> (1,2) : horiz[1][1] (0-indexed: row 0, col 0)
    // 2. (1,2) -> (1,3) : horiz[0][1]
    // ...
    // m-2. (1,m-2) -> (1,m-1) : horiz[0][m-3]
    // m-1. (1,m-1) -> (2,m-1) : vert[0][m-2]
    // m. (2,m-1) -> (3,m-1) : vert[1][m-2]
    // ...
    // m + n-3. (n-2,m-1) -> (n-1,m-1) : vert[n-3][m-2]
    // Now at (n-1,m-1). We'll enter the cycle.
    // Cycle edges in order: 
    // a. (n-1,m-1) -> (n-1,m) : horiz[n-2][m-2] (since row n-1 is index n-2, col m-1 to m is index m-2)
    // b. (n-1,m) -> (n,m) : vert[n-2][m-1] (col m is index m-1)
    // c. (n,m) -> (n,m-1) : horiz[n-1][m-2] (row n is index n-1, col m to m-1 is index m-2)
    // d. (n,m-1) -> (n-1,m-1) : vert[n-2][m-2] (col m-1 is index m-2)
    // After looping, we need to end at (n,m). We can exit the cycle at (n-1,m) and go to (n,m) (edge b again) or at (n,m) directly.
    // The walk will end at (n,m). The last edge must be part of the alternating sequence.
    // If we do an even number of extra steps (multiple of 2), the parity of the path length changes by even number, so the alternation at the junction is preserved if we design correctly.
    
    // Let's set colors along the main path (before cycle) alternating starting with 'R'.
    // We'll set:
    // horiz[0][0] = 'R'
    // horiz[0][1] = 'B'
    // horiz[0][2] = 'R'
    // ... alternating
    // vert[0][m-2] = continue alternating from last horizontal edge.
    // vert[1][m-2] = continue, etc.
    
    // Then we need to set the cycle edges such that the alternation holds when we go around.
    // Suppose we arrive at (n-1,m-1) via edge vert[n-3][m-2]. Let its color be C.
    // Then the next edge (n-1,m-1)->(n-1,m) must be opposite of C.
    // Then (n-1,m)->(n,m) opposite of that, etc.
    // Also, when we finish looping and go to (n,m), the last edge must be consistent with the walk.
    // We can always adjust the cycle colors to satisfy this, because we have freedom.
    
    // Let's compute the colors along the path to (n-1,m-1).
    // Path length to (n-1,m-1) in terms of edges: (m-2) horizontal + (n-2) vertical = n+m-4 edges.
    // The color of the last edge (entering (n-1,m-1)) depends on the starting color and the number of edges.
    // If we start with 'R' at first edge, then edge index i (0-based) has color 'R' if i even, 'B' if i odd.
    // So the edge entering (n-1,m-1) is at index (n+m-5) (since total edges before cycle = n+m-4, indices 0 to n+m-5).
    // Its color = (n+m-5) % 2 == 0 ? 'R' : 'B'.
    
    // Now we need to set the cycle edges. Let's denote the four cycle edges in order: A, B, C, D as above.
    // We want the sequence of edges along the walk: ... -> (n-1,m-1) via edge E (color C_E), then A, B, C, D, A, B, C, D, ... and finally exit.
    // To alternate, A must be opposite of C_E.
    // Then B opposite of A, C opposite of B, D opposite of C, then A opposite of D (which is consistent if 4-cycle, since D opposite of C, C opposite of B, B opposite of A, so D same as B? Wait: A opp B, B opp C => A same as C? No: if A=R, B=B, C=R, D=B. Then A opp D? R vs B -> yes opposite. So cycle alternates correctly: R, B, R, B. Then after D (B), next A (R) is opposite, good.
    // So we just need A = opposite of C_E.
    // Then we can set A, B, C, D accordingly.
    
    // After looping, we need to end at (n,m). The walk can end after completing a cycle edge that lands on (n,m).
    // (n,m) is reached via edge B: (n-1,m)->(n,m) or via edge C: (n,m-1)->(n,m)? Actually C goes from (n,m) to (n,m-1), so if we traverse C backwards, it's (n,m-1)->(n,m). But our defined direction for C is (n,m)->(n,m-1). We can traverse edges in either direction; the color is the same regardless of direction. So we can end at (n,m) after traversing B (arriving at (n,m)) or after traversing C in reverse (arriving at (n,m)). To make it simple, we can just stop after B.
    // The walk will be: main path to (n-1,m-1), then optionally loop some number of times (each loop adds 4 edges), then finally go A, B to end at (n,m). That adds 2 edges after the loops.
    // Total extra edges beyond min path? Let's compute base path without loops: main path to (n-1,m-1) (n+m-4 edges), then A, B (2 edges) -> total n+m-2 edges = min_len. Good.
    // If we add L loops (each 4 edges), total edges = min_len + 4L. This gives lengths min_len, min_len+4, min_len+8, ...
    // But we need to be able to achieve any k >= min_len with same parity. k-min_len is even, but could be 2 mod 4? For example, min_len+2. Can we get +2?
    // Yes, we can also just go A, B, C, D, A, B? That would be 6 edges after main path? Actually we need to end at (n,m). If we do one full loop (4 edges) we end at (n-1,m-1) again, then we need A, B to finish -> total extra = 4+2 = 6? That's +6. To get +2, we could go A, B, C (reverse?) Let's see: after A, B we are at (n,m). If we go C (from (n,m) to (n,m-1)), then D (to (n-1,m-1)), then A, B again -> that's 4 edges and we end at (n,m) again. So that's a loop of 4 edges that starts and ends at (n,m). So we can add any multiple of 4.
    // But what about +2? We can do: main path to (n-1,m-1), then A, B (at (n,m)), then C (to (n,m-1)), then D (to (n-1,m-1)), then A, B (to (n,m))? That's 6 extra. Not 2.
    // To get +2, we can modify the path: instead of going directly from (n-1,m-1) to (n-1,m) to (n,m), we could go (n-1,m-1) -> (n,m-1) -> (n,m). That's two edges: vert[n-2][m-2] (down) and horiz[n-1][m-2] (right). That's also min_len. But we can combine with the cycle to get +2? Actually, if we have a cycle, we can go around part of it. For example, from (n-1,m-1) go A to (n-1,m), then B to (n,m), then C to (n,m-1), then D to (n-1,m-1), then A to (n-1,m), then B to (n,m). That's 6 extra. Not 2.
    // Wait, the known solution for this problem uses a different construction: they make a cycle that can be traversed to add 2 steps. How? By having a cycle of length 2? Not possible in grid. They use a cycle of 4 edges but they enter and exit at different points to add 2.
    // Let's think: Suppose we have a cycle of 4 edges. If we enter the cycle at one corner and leave at the opposite corner, we traverse 2 edges of the cycle. That adds 2 extra edges compared to the direct edge? But there is no direct edge between opposite corners. The min path between opposite corners of a 2x2 square is 2 edges. So entering and leaving at opposite corners via the cycle also takes 2 edges (the other two edges of the cycle). So no extra.
    // To add extra steps, we need to go around the cycle multiple times. Each full loop adds 4. To add 2, we can go around half the cycle and then come back? That would retrace edges, which is allowed. For example, from (n-1,m-1) go A to (n-1,m), then B to (n,m), then go back B to (n-1,m), then A to (n-1,m-1), then A to (n-1,m), then B to (n,m). That adds 4 extra (B, A, A, B) but we visited edges multiple times. The condition only requires that consecutive segments in the walk have different colors. If we go back and forth on the same edge, the color is the same, so two consecutive segments would have the same color! That violates the condition. So we cannot immediately retrace an edge. We must alternate. So going back and forth on the same edge is forbidden because the same edge would be used twice in a row, same color. So we cannot do that.
    // Therefore, we can only add steps in multiples of 4 using a 4-cycle? But the problem allows any k with same parity. How to get +2?
    // Let's re-read the problem: "out of k segments connecting two consecutive points in this walk, no two consecutive segments have the same color". So the walk is a sequence of points, and we look at the colors of the segments between consecutive points. If we go A, B, A, the segments are A, B, A. Their colors: color(A), color(B), color(A). Consecutive segments: (A,B) and (B,A). They are different if color(A) != color(B) and color(B) != color(A), which is true if color(A) != color(B). So we can alternate between two edges of different colors. So we can bounce between two adjacent edges of different colors. For example, if we have a vertex with two incident edges of different colors, we can go back and forth: edge1, edge2, edge1, edge2,... This adds 2 segments each time we go edge1 then edge2? Actually, going from vertex v to u via edge1, then back to v via edge1? That would be same edge twice in a row -> same color -> invalid. But if we go v -> u via edge1, then u -> w via edge2, then w -> u via edge2? That would be edge1, edge2, edge2 -> consecutive edge2, edge2 same color invalid. So we need to alternate edges. So we can bounce between two edges that share a vertex and have different colors. For example, at vertex (n-1,m), we have edges: vertical to (n,m) (color B) and horizontal to (n-1,m-1) (color R). If we go (n-1,m-1) -> (n-1,m) (R), then (n-1,m) -> (n,m) (B), then (n,m) -> (n-1,m) (B) -> invalid because B followed by B. So we cannot immediately go back along the same edge. We need to go to a different vertex. So to bounce, we need a triangle? But grid has no triangles. So we need a cycle of length 4 to bounce? Actually, we can do: (n-1,m-1) -> (n-1,m) (R) -> (n,m) (B) -> (n,m-1) (R) -> (n-1,m-1) (B) -> (n-1,m) (R) -> ... This uses the 4-cycle. Each step alternates. This adds 4 steps per loop. To add 2 steps, we could stop in the middle of the loop? But we must end at (n,m). If we stop after (n-1,m-1) -> (n-1,m) -> (n,m), that's the base path. If we do one more step to (n,m-1), we are not at (n,m). So we need to end at (n,m). So we can only add multiples of 4? But the problem allows any even difference. How is that possible?
    // Let's check the sample: n=4, m=5, k=11. min_len = 4+5-2=7. k=11 -> difference 4, which is multiple of 4. Sample 4: n=3, m=3, k=12588. min_len=4. 12588-4=12584, divisible by 4? 12584/4=3146, yes. Sample 5: n=4, m=4, k=8. min_len=6. 8-6=2, which is 2 mod 4. And output is YES. So +2 is possible! How?
    // Let's analyze sample 5: n=4, m=4, k=8. They output a coloring. Let's try to understand the construction for +2.
    // Maybe they use a different cycle location or a different path that allows adding 2 by using a "dead end" branch? For example, go to some vertex, then go to a neighbor and back, but using two different edges? If a vertex has degree 2, you can go in one edge and out the other, but to come back you need to go out and in? Actually, if you have a path that goes to a leaf? But grid has no leaves. However, you can create a "spur" that goes to a corner and back? But corners have degree 2. If you go to a corner, you must leave via the other edge. You can't immediately turn back because that would be the same edge. So you need to go corner -> neighbor -> other neighbor -> corner? That's a cycle.
    // Let's think differently: The walk can revisit points. The condition is on segments, not points. So we can have a walk that goes ... -> A -> B -> C -> D -> C -> B -> A -> ... where edges are different. This is like going back and forth on a path of length >1. For example, if we have a path of 3 edges with alternating colors: R, B, R. We can go: start at left, go R to middle, go B to right, go B? No, can't go back along B because same color. But we can go: left -> middle (R), middle -> right (B), right -> middle (B) invalid. So we need the return edge to be different. If the path has colors R, B, R, then from right, the only edge back is B, which would be same as the edge we just took (B). So we can't immediately go back. We would need to go to another vertex. So a simple path doesn't allow bouncing.
    // What about a vertex with degree 3? We can go in on one edge, out on a second, back in on a third? But we need to return to the same vertex to continue. Actually, we can do: v1 -> v2 (edge1), v2 -> v3 (edge2), v3 -> v2 (edge2) invalid. So we need edge2 and edge3 to be different. If v2 has edges to v1, v3, v4 with colors R, B, R. Then v1 -> v2 (R), v2 -> v3 (B), v3 -> v2 (B) invalid. v3 -> v4? But v3 might not connect to v4. So we need a cycle.
    // The only way to add 2 steps while alternating colors is to have a cycle of length 4 where we enter and exit at adjacent vertices? Let's calculate: Suppose we have a 4-cycle with vertices A, B, C, D in order. Colors: AB=R, BC=B, CD=R, DA=B. Base path goes from somewhere to A, then to B, then to C (end). That's 2 edges inside the cycle. If we want to add 2 steps, we could go from A to D to C instead of A to B to C. That's also 2 edges. No extra. To add extra, we need to go A->B->C->D->A->B->C. That's 6 edges inside, extra 4. So +4.
    // How to get +2? Maybe by using a different base path that is not the shortest? The condition only requires existence of a walk of length k+1. The walk doesn't have to be simple. We can have a walk that goes from (1,1) to (n,m) and has length k+1. We can make the walk longer by adding detours that add 2 steps. For example, go from (1,1) down to (2,1), then right to (2,2), then up to (1,2), then right to (1,3)... This is a longer path. But we need to color the whole grid such that there exists some walk of length k+1. We are free to choose the walk. So we can design the coloring to allow a specific walk that has the exact length.
    // So we don't need a generic cycle that can add any even number; we just need to construct one walk of the exact length. Since k can be up to 1e9, we need a systematic way to extend the walk by 2 or 4. The known solution from Codeforces: If k >= min_len and parity matches, it's always possible. The construction: create a path that goes right along the first row, then down the last column, but with a "detour" at the beginning or end that can be adjusted to increase length by 2. Specifically, they use a 2x2 square at the start or end to add 2 steps by going around 3 edges of the square instead of 1? Let's think.
    // Consider the start at (1,1). We can go (1,1) -> (1,2) -> (2,2) -> (2,1) -> (1,1) -> (1,2) -> ... This uses a 2x2 square. The edges: (1,1)-(1,2) = R, (1,2)-(2,2) = B, (2,2)-(2,1) = R, (2,1)-(1,1) = B. Then we can loop around this square. But we need to eventually go to (n,m). We can leave the square from (1,2) to (1,3) etc. So we can add any number of loops (each 4 steps) by going around the square. But again, multiples of 4. To add 2, we can go halfway around the square and then exit? For example, from (1,1) go to (1,2) (R), then to (2,2) (B), then to (2,1) (R), then to (1,1) (B), then to (1,2) (R), then continue right. That's 4 extra steps (R,B,R,B) before continuing. Still 4.
    // What if we go: (1,1) -> (2,1) -> (2,2) -> (1,2) -> continue right. That's 3 edges instead of 1 direct edge from (1,1) to (1,2)? Direct is 1 edge. So we added 2 edges. Let's check colors: We need the path to alternate. Suppose we set colors: (1,1)-(1,2) = R (direct). If we take detour: (1,1)-(2,1) = B, (2,1)-(2,2) = R, (2,2)-(1,2) = B. Then the sequence of edges from (1,1) to (1,2) via detour is B, R, B. Then the next edge from (1,2) to (1,3) must be opposite of B, so R. If we had taken direct edge R, the next edge would need to be opposite of R, so B. So the parity of the number of edges to reach (1,2) changes by 2 (odd vs even? 1 vs 3, both odd, so parity of edge count is same? 1 mod 2 = 1, 3 mod 2 = 1. So the color of the edge entering (1,2) is the same? Actually, the condition is on the color of the segment between pi and pi+1. The alternation requires that the color of segment i and i+1 are different. So if we have a sequence of segments, the colors must alternate. So if we have a walk from start to some point, the color of the last segment depends on the starting color and the number of segments modulo 2. If we start with color C1 on first segment, then segment t has color C1 if t is odd, opposite if t is even. So if we take a path of length L to reach a point, the color of the last segment is determined by L mod 2. If we take two different paths of lengths L1 and L2 to the same point, and L1 and L2 have the same parity, then the last segment color will be the same parity, so we can continue with the same next color. If they have different parity, the required next color would be different. So to be able to substitute a detour, the detour must have the same parity of length as the direct segment? Actually, we are constructing a single walk. We don't need to substitute; we just design the walk from start to end with the exact length. We can choose the path arbitrarily. So we can just make the walk go through a detour that adds 2 steps. For example, at the very beginning, instead of going right from (1,1) to (1,2), we go down to (2,1), right to (2,2), up to (1,2). That adds 2 extra steps. Then we continue along the first row to the right, then down the last column. The total length will be min_len + 2. We can also add more 2-step detours? But we can only do this if the grid allows such a detour. At the start, we have a 2x2 square available if n>=2 and m>=2. Since n,m>=3, we can do this. But we need to ensure the coloring supports the alternating walk along this detour and the rest of the path.
    // Let's design a coloring that allows a walk of any length k with same parity as min_len. The idea: we can create a "main path" that has length min_len, and then we can add "loops" of length 2 by using a 2x2 square at the start or end. Wait, the detour adds 2, but can we add multiple 2s? We can repeat the detour? If we go around the 2x2 square multiple times, we add 4 each time. To add another 2, we could do the detour at the end similarly. But we need to add arbitrary even number up to 1e9. So we need a way to add 2 repeatedly. How? By having a cycle of length 2? Not possible. But we can have a cycle of length 4 that we can traverse partially to add 2? Let's analyze the detour: (1,1) -> (2,1) -> (2,2) -> (1,2). This path has 3 edges. The direct edge is 1 edge. Difference is 2. Can we do this detour multiple times? To do it again, we would need to go back to (1,1) and do it again. But going back would require more edges. For example, from (1,2) we could go down to (2,2), left to (2,1), up to (1,1), then do the detour again. That would be a loop: (1,1) -> (2,1) -> (2,2) -> (1,2) -> (2,2) -> (2,1) -> (1,1). That's 6 edges, and we end at (1,1) again. Then we can start over. That loop adds 6 edges? Actually, from (1,1) to (1,1) via that loop is 6 edges. But we want to eventually go to (n,m). So we can insert this loop at the beginning. Each loop adds 6 edges? Let's count: start at (1,1), go detour to (1,2) (3 edges), then go back to (1,1) via (2,2) and (2,1) (3 edges: (1,2)-(2,2), (2,2)-(2,1), (2,1)-(1,1)). Total 6 edges to return to (1,1). Then we can do it again. So we can add multiples of 6. Not 2.
    // But we can also just go back and forth on the detour path? For example, (1,1) -> (2,1) -> (2,2) -> (1,2) -> (2,2) -> (2,1) -> (1,1) -> (2,1) -> ... This alternates edges. Let's check colors: We need to assign colors to the edges of this 2x2 square such that any alternating walk is possible. The square has 4 edges. We can set them to alternate around the square: e.g., top edge R, right edge B, bottom edge R, left edge B. Then consider the walk: start at (1,1). Go left? No, left edge is (1,1)-(2,1) color B. Go B to (2,1). Then bottom edge (2,1)-(2,2) color R. Then right edge (2,2)-(1,2) color B. Then top edge (1,2)-(1,1) color R. Then left edge (1,1)-(2,1) color B... This walk alternates: B, R, B, R, B,... So we can loop around the square indefinitely. Each full loop is 4 edges and returns to (1,1). So we can add any multiple of 4. Still not 2.
    // How does sample 5 achieve +2? n=4,m=4,k=8. min_len=6. k=8 -> +2. Let's try to deduce their coloring from the output:
    // Sample 5 output:
    // YES
    // B B R
    // R B R
    // B R R
    // R R B
    // B R R B
    // B B B B
    // B R R R
    // Let's parse: n=4, m=4.
    // Horizontal edges (4 rows, 3 columns):
    // Row1: B B R
    // Row2: R B R
    // Row3: B R R
    // Row4: R R B
    // Vertical edges (3 rows, 4 columns):
    // Row1: B R R B
    // Row2: B B B B
    // Row3: B R R R
    // Let's try to find a walk of length 9 (k+1=9 points, 8 segments) from (1,1) to (4,4) alternating.
    // Start (1,1). Edges from (1,1): right to (1,2) is B, down to (2,1) is B. Both B. If we start with B, next must be R.
    // Let's try to find a path of length 8. Since min_len=6, we need 2 extra. Maybe they use a detour that adds 2 by going into a dead end and coming back? But grid has no dead ends. However, we can go to a corner and come back? Corner (1,1) has degree 2. If we go (1,1)->(1,2) (B), then (1,2)->(1,1) (B) invalid. So can't.
    // Maybe they use a cycle of length 4 but enter and exit at the same vertex? That adds 4. +2 seems impossible with only 4-cycles. Wait, could it be that the walk is not a simple path plus loops, but a more complex walk that goes back and forth on a path of length 2? For example, if we have two adjacent edges of different colors, we can go back and forth between their endpoints? Let's say vertices A-B-C. Edges AB=R, BC=B. Walk: A->B (R), B->C (B), C->B (B) invalid. So we need three vertices with edges of alternating colors. Suppose A-B=R, B-C=B, C-D=R. Then we can go A->B (R), B->C (B), C->B (B) invalid. To go back from C to B, we must use edge BC which is B, same as the edge we just used (B). So we can't immediately go back. We need to go C->D (R), then D->C (R) invalid. So we can't bounce on
