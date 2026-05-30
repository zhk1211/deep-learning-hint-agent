// Hint0
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, m;
    long long k;
    cin >> n >> m >> k;
    
    // The walk length is k+1 points, so k segments.
    // The shortest path from (1,1) to (n,m) has length (n-1)+(m-1) = n+m-2.
    long long min_len = n + m - 2;
    
    if (k < min_len) {
        cout << "NO\n";
        return;
    }
    
    // We need to check parity: the parity of k must match the parity of min_len.
    // Because any extra steps must come in pairs (go and return) to keep the same parity.
    if ((k - min_len) % 2 != 0) {
        cout << "NO\n";
        return;
    }
    
    cout << "YES\n";
    
    // We will construct a coloring that allows a walk of length k.
    // The idea: create a path that goes right along row 1, then down along column m,
    // but we can add a 2x2 "detour" at the start to adjust parity/length.
    // Actually, we can always construct a valid coloring for any valid (n,m,k).
    
    // We'll define horizontal edges: hor[i][j] for edge between (i,j) and (i,j+1), 1-indexed.
    // vertical edges: ver[i][j] for edge between (i,j) and (i+1,j).
    
    vector<vector<char>> hor(n+1, vector<char>(m, 'R')); // hor[i][j] for j=1..m-1
    vector<vector<char>> ver(n, vector<char>(m+1, 'R')); // ver[i][j] for i=1..n-1, j=1..m
    
    // We'll construct a specific pattern that works.
    // Let's make the main path: (1,1) -> (1,2) -> ... -> (1,m) -> (2,m) -> ... -> (n,m).
    // We need alternating colors along this path.
    // We'll set colors along this path first.
    
    // Horizontal edges on row 1: (1,1)-(1,2), (1,2)-(1,3), ..., (1,m-1)-(1,m)
    // We'll alternate starting with 'R' at (1,1)-(1,2).
    for (int j = 1; j <= m-1; j++) {
        hor[1][j] = (j % 2 == 1) ? 'R' : 'B';
    }
    
    // Vertical edges on column m: (1,m)-(2,m), (2,m)-(3,m), ..., (n-1,m)-(n,m)
    // The last horizontal edge on row 1 is (1,m-1)-(1,m) with color determined by parity of m-1.
    // If m-1 is odd, that edge is 'R'; if even, 'B'.
    // Then the first vertical edge (1,m)-(2,m) must be the opposite color.
    char last_hor_color = hor[1][m-1];
    char first_ver_color = (last_hor_color == 'R') ? 'B' : 'R';
    for (int i = 1; i <= n-1; i++) {
        ver[i][m] = ((i % 2 == 1) ? first_ver_color : (first_ver_color == 'R' ? 'B' : 'R'));
        // Actually alternate: if i=1 -> first_ver_color, i=2 -> opposite, etc.
        // Better: ver[i][m] = ((i % 2 == 1) ? first_ver_color : (first_ver_color == 'R' ? 'B' : 'R'));
    }
    // Let's fix: 
    for (int i = 1; i <= n-1; i++) {
        if (i == 1) ver[i][m] = first_ver_color;
        else ver[i][m] = (ver[i-1][m] == 'R') ? 'B' : 'R';
    }
    
    // Now we need to fill the rest of the grid arbitrarily but must ensure that the walk exists.
    // The walk we will use: go right along row 1 to (1,m), then down to (n,m).
    // But we might need extra steps to reach length k.
    // We can add a detour at the beginning: (1,1) -> (2,1) -> (2,2) -> (1,2) -> then continue right.
    // This adds 2 extra steps (length increases by 2) and we can repeat it if needed.
    // However, we must ensure the coloring supports this detour with alternating colors.
    
    // Let's design the coloring to support the detour at the top-left 2x2 block.
    // We'll set:
    // hor[1][1] = 'R' (already set as j=1 -> 'R')
    // ver[1][1] = 'B' (edge (1,1)-(2,1))
    // hor[2][1] = 'R' (edge (2,1)-(2,2))
    // ver[1][2] = 'B' (edge (1,2)-(2,2))? Wait, we need alternating: 
    // Path: (1,1) -R-> (1,2) is the normal path. But detour: (1,1) -B-> (2,1) -R-> (2,2) -B-> (1,2) -R-> (1,3)...
    // So we need: ver[1][1] = 'B', hor[2][1] = 'R', ver[1][2] = 'B'.
    // Then from (1,2) we go to (1,3) with 'R'? But hor[1][2] is currently 'B' (since j=2 even -> 'B').
    // That would conflict: after ver[1][2]='B', the next edge hor[1][2] must be 'R', but we set it 'B'.
    // So we need to adjust the colors on row 1 to accommodate the detour.
    
    // Alternative: We can make the detour at the end (bottom-right) or just use a different construction.
    // Actually, we can always construct a valid coloring by making the whole grid a checkerboard pattern,
    // but we need to ensure the specific walk exists.
    
    // Let's use a known solution: 
    // Color the grid such that all horizontal edges on row 1 are 'R', all vertical edges on column m are 'B',
    // and everything else is 'R' or 'B' in a way that doesn't create conflicts.
    // But we need alternating colors along the walk.
    
    // Simpler approach: We can always construct a walk that goes right along row 1, then down column m,
    // and if we need extra length, we can loop around a 2x2 square at the start or end.
    // We'll set the colors of the 2x2 at (1,1)-(2,2) to allow a loop.
    
    // Let's define:
    // hor[1][1] = 'R' (between (1,1) and (1,2))
    // ver[1][1] = 'B' (between (1,1) and (2,1))
    // hor[2][1] = 'R' (between (2,1) and (2,2))
    // ver[1][2] = 'B' (between (1,2) and (2,2))
    // Then from (1,2) we go to (1,3). To alternate, hor[1][2] must be 'R' (since ver[1][2] is 'B').
    // But then hor[1][2] = 'R', hor[1][3] = 'B', etc. So row 1 becomes: R, R, B, R, B, ...? That breaks alternation on the main path.
    // The main path along row 1 must alternate. So we cannot have two consecutive 'R's.
    
    // Therefore, the detour cannot be at the very beginning if we want to keep the main path alternating.
    // Instead, we can put the detour at the end: near (n,m).
    // Or we can make the main path not strictly along row 1 and column m, but a snake-like path that covers the whole grid.
    // But we only need to output a coloring, not the walk. The walk just needs to exist.
    
    // Let's think differently: We can color the entire grid in a checkerboard pattern.
    // For a grid, color each edge based on the parity of (i+j) of its endpoints.
    // For horizontal edge between (i,j) and (i,j+1): color = ( (i+j) % 2 == 0 ? 'R' : 'B' ) or something.
    // Then any path will have alternating colors automatically because moving to adjacent cell changes parity.
    // Actually, if we color edges such that all edges incident to a cell with even i+j are one color and odd i+j are another,
    // then any walk will alternate colors? Let's check: 
    // If we color an edge based on the cell it comes from? Not exactly.
    // Standard: In a bipartite graph (grid is bipartite), if we color all edges from one partition to the other with alternating colors based on direction, we can achieve that any path alternates.
    // But we need a specific walk from (1,1) to (n,m) of length k.
    // If we use a checkerboard coloring of edges: For horizontal edges, color 'R' if i is odd, 'B' if i is even? That might not guarantee alternation.
    
    // Let's use the construction from the sample: 
    // In sample 1 (4,5,11): 
    // Horizontal:
    // R R B B
    // R R R R
    // B B B R
    // R R B B
    // Vertical:
    // R B B R B
    // R B B B B
    // B B R R R
    // This seems specific.
    
    // Another known solution: 
    // We can always construct a coloring by making the first row and last column alternating, and then filling the rest arbitrarily but ensuring no constraints are violated? Actually, the only requirement is that there EXISTS a walk. So we can design the coloring specifically to support one particular walk.
    
    // Let's design a walk that goes: 
    // (1,1) -> (1,2) -> ... -> (1,m) -> (2,m) -> ... -> (n,m).
    // This is the shortest path of length n+m-2.
    // To increase length by 2, we can add a "detour" at the corner (1,m) or (n,m) or (1,1).
    // For example, at (1,m): instead of going directly down to (2,m), we go (1,m) -> (1,m-1) -> (2,m-1) -> (2,m) -> (3,m)...
    // This adds 2 steps. We can repeat this detour as many times as needed.
    // We need to color edges to support this detour with alternating colors.
    
    // Let's define the main path with alternating colors starting with 'R' at (1,1)-(1,2).
    // Then we set colors along row 1 and column m accordingly.
    // For the detour at (1,m), we need:
    // (1,m-1) -> (1,m) is some color C1.
    // (1,m) -> (1,m-1) would be same edge, but we go back, so color is C1.
    // Then (1,m-1) -> (2,m-1) must be opposite of C1.
    // Then (2,m-1) -> (2,m) must be opposite of that, so same as C1.
    // Then (2,m) -> (3,m) must be opposite of C1.
    // But originally, (1,m) -> (2,m) was supposed to be opposite of C1. Now we replaced it with the detour, and the edge (2,m)-(3,m) must be opposite of the last edge of detour, which is C1. So (2,m)-(3,m) must be opposite of C1. That matches the original alternation if we set (1,m)-(2,m) to opposite of C1? Wait, let's trace carefully.
    
    // Let's assign colors along the intended walk.
    // We'll construct the walk dynamically: we can decide the walk first, then color edges along it alternatingly, and fill the rest arbitrarily.
    // But we need to output the whole grid coloring.
    
    // Idea: We can always construct a coloring by making the entire grid a "striped" pattern that allows a Hamiltonian path from (1,1) to (n,m) with alternating colors, and then we can extend the path by looping around a 2x2 square somewhere.
    // Actually, there is a known solution for this problem (CF 1898C). 
    // The solution: 
    // - If k < n+m-2 or (k - (n+m-2)) % 2 != 0: NO.
    // - Else: YES.
    // - Coloring: 
    //   For horizontal edges: 
    //     For each row i, for each column j from 1 to m-1:
    //       if i == 1 and j == 1: color 'R' (or 'B', but we'll follow a pattern)
    //       else if i == n and j >= something? 
    //   Actually, the editorial provides a simple construction:
    //   Color all edges of the grid in a checkerboard pattern such that the path (1,1) -> (1,2) -> ... -> (1,m) -> (2,m) -> ... -> (n,m) is alternating.
    //   Then, to allow extra length, we can use a 2x2 block at the top-left or bottom-right.
    //   But we need to ensure the coloring supports the loop.
    
    // Let's implement the construction from the editorial:
    // We'll set the colors of the main path as alternating.
    // Then we'll set the colors of the "detour" edges to allow a loop at the start or end.
    // Specifically, we can make the loop at the end: near (n,m).
    // If we need extra length, we can go (n-1,m) -> (n-1,m-1) -> (n,m-1) -> (n,m) instead of directly (n-1,m) -> (n,m).
    // This adds 2 steps.
    // We can repeat this loop as many times as needed by going back and forth? Actually, we can only do it once because we need to end at (n,m). But we can do multiple loops if we go back and forth multiple times? No, we need to end at (n,m). We can do the loop multiple times by going (n-1,m) -> (n-1,m-1) -> (n,m-1) -> (n,m) -> (n,m-1) -> (n-1,m-1) -> (n-1,m) -> (n,m) ... but that would require alternating colors on the same edges, which is fixed. So we can only use each edge once in the walk? No, we can traverse edges multiple times. But the colors are fixed. If we go back and forth on the same edge, the colors would be same for consecutive segments? Wait: if we go A-B-A, the segments are A-B and B-A, which are the same edge, so same color. That would violate the alternating condition. So we cannot immediately go back on the same edge. Therefore, loops must use a cycle of length at least 4.
    
    // So we can have a 4-cycle detour that we can traverse multiple times? Actually, to repeat a detour, we would need to go around a cycle multiple times. But a cycle of length 4 has alternating colors if colored properly. We can go around it multiple times, each time adding 4 steps. But we need to add steps in multiples of 2. A 4-cycle adds 4 steps, but we can also do a 2-step detour? No, 2-step would require going back on the same edge, which is invalid. So the minimum extra steps we can add is 4? But the parity condition allows any even number of extra steps. How to add 2 extra steps? 
    // Actually, we can add 2 extra steps by taking a detour that is not a cycle but a path that rejoins later. For example, the detour at the corner: (1,m-1) -> (1,m) -> (2,m) is normal (2 steps). Detour: (1,m-1) -> (2,m-1) -> (2,m) is also 2 steps. So we can replace a straight line with a "staircase" step, which adds 0 extra steps? No, both are 2 steps. To add 2 steps, we need to replace 1 step with 3 steps? That's impossible because we must move to adjacent cells. 
    
    // Let's re-evaluate: The shortest path length is n+m-2. Any path from (1,1) to (n,m) has length L = n+m-2 + 2*d, where d is the number of "detours" (each detour adds 2 steps). A detour can be: instead of going right then down, go down then right. That doesn't change length. To increase length, we need to go away from the destination and come back. The smallest such detour is going right, down, left, down? That adds 2 steps? Let's see: Suppose we are at (i,j) and want to go to (i,j+1) then (i+1,j+1). Normal: (i,j) -> (i,j+1) -> (i+1,j+1) (2 steps). Detour: (i,j) -> (i+1,j) -> (i+1,j+1) -> (i,j+1) -> (i+1,j+1)? That's 4 steps, adding 2 extra. Yes! So we can add a 2-step detour by going around a 2x2 block: (i,j) -> (i+1,j) -> (i+1,j+1) -> (i,j+1) -> (i+1,j+1). This adds 2 steps (normally 2 steps, now 4 steps). So we can add 2 steps at a time by using a 2x2 loop.
    
    // So we can construct a walk that uses the shortest path, but at some corner, we insert this 2x2 loop as many times as needed to reach length k.
    // We need to color the edges of that 2x2 block to allow this loop with alternating colors.
    
    // Let's design the coloring around a 2x2 block at the end: near (n,m).
    // The normal path near the end: ... -> (n-1,m) -> (n,m).
    // We can insert the loop: (n-1,m-1) -> (n-1,m) -> (n,m)  (normal)
    // Loop version: (n-1,m-1) -> (n,m-1) -> (n,m) -> (n-1,m) -> (n,m) ? Wait, we need to start from (n-1,m-1) and end at (n,m).
    // Normal: (n-1,m-1) -> (n-1,m) -> (n,m) (2 steps)
    // Loop: (n-1,m-1) -> (n,m-1) -> (n,m) -> (n-1,m) -> (n,m) (4 steps)
    // So we need edges:
    // (n-1,m-1)-(n,m-1): vertical
    // (n,m-1)-(n,m): horizontal
    // (n,m)-(n-1,m): vertical (this is the reverse of (n-1,m)-(n,m))
    // (n-1,m)-(n,m): vertical again? Wait, the last step is (n-1,m) -> (n,m). That's the same edge as (n,m)-(n-1,m). So we traverse it twice? But consecutive segments would be (n,m)-(n-1,m) and (n-1,m)-(n,m), which are the same edge, same color -> invalid.
    // So that loop is invalid because it repeats an edge consecutively.
    
    // Let's find a valid 4-step detour that doesn't repeat an edge consecutively.
    // Consider the 2x2 block with corners (n-1,m-1), (n-1,m), (n,m-1), (n,m).
    // We want a path from (n-1,m-1) to (n,m) of length 4 that alternates colors.
    // Possible path: (n-1,m-1) -> (n-1,m) -> (n,m) -> (n,m-1) -> (n,m) ? Again repeats (n,m)-(n,m-1) and (n,m-1)-(n,m).
    // Another: (n-1,m-1) -> (n,m-1) -> (n,m) -> (n-1,m) -> (n,m) repeats (n-1,m)-(n,m).
    // Any path of length 4 in a 2x2 from one corner to the opposite corner must repeat an edge if it ends at the opposite corner? Let's check all simple paths of length 4 from (1,1) to (2,2) in a 2x2:
    // (1,1) -> (1,2) -> (2,2) -> (2,1) -> (2,2) repeats (2,1)-(2,2).
    // (1,1) -> (2,1) -> (2,2) -> (1,2) -> (2,2) repeats (1,2)-(2,2).
    // So any length-4 path between opposite corners in a 2x2 must repeat an edge at the end. That's invalid.
    
    // What about a detour that is not at the corner but somewhere in the middle?
    // Suppose we are going along row 1: (1,j) -> (1,j+1). We can detour: (1,j) -> (2,j) -> (2,j+1) -> (1,j+1) -> (1,j+2). That's length 4 instead of 2, adding 2 steps. And no edge is repeated consecutively.
    // Let's check: edges: (1,j)-(2,j) [ver], (2,j)-(2,j+1) [hor], (2,j+1)-(1,j+1) [ver], (1,j+1)-(1,j+2) [hor].
    // This works! So we can insert this detour anywhere along the top row.
    
    // So we can construct the walk as: go right along row 1, but whenever we need extra steps, we do a "down-right-up" detour.
    // We can repeat this detour multiple times at different columns or even at the same column? If we do it at the same column, we would go down, right, up, then down again? That might repeat edges.
    // But we can just do the detour at multiple consecutive columns to add as many steps as needed.
    // Since k can be up to 1e9, we need to add many steps. We can just loop around a 2x2 block many times? But we cannot repeat edges consecutively. However, we can go around a cycle of length 4 multiple times if we don't repeat edges consecutively? Actually, going around a 4-cycle: (1,1)->(1,2)->(2,2)->(2,1)->(1,1) is a cycle. If we go around it multiple times, the sequence of edges would be: e1, e2, e3, e4, e1, e2, ... The transition from e4 to e1: e4 is (2,1)-(1,1), e1 is (1,1)-(1,2). These are different edges, so colors can alternate if we set them properly. So we can loop around a 4-cycle as many times as we want! That adds 4 steps per loop. But we need to add steps in multiples of 2. 4 is a multiple of 2, so we can add any even number >= 2? 2 is not a multiple of 4. So we need a way to add exactly 2 steps (mod 4). 
    
    // Wait, the parity condition is (k - (n+m-2)) % 2 == 0. So the extra steps needed is even. It could be 2, 4, 6, ...
    // If we can only add 4-step loops, we can only achieve extra steps that are multiples of 4. But we need to handle extra steps = 2 mod 4 as well.
    // So we need a detour that adds exactly 2 steps (mod 4). The "down-right-up" detour adds 2 steps (replaces 2 steps with 4 steps, net +2). That detour uses 4 edges and doesn't repeat consecutively. Can we repeat it multiple times? If we do it at different columns, each adds 2 steps. So we can add any even number of steps by doing the detour at multiple columns. But we have limited columns (m up to 16). So we can only add at most (m-1) detours, each adding 2 steps, total extra up to 2*(m-1). But k can be huge (1e9). So we need a way to add many steps without using many columns.
    
    // We need a cycle that we can traverse many times. A 4-cycle can be traversed multiple times, adding 4 steps each time. But we also need to handle the case where extra steps is 2 mod 4. We can combine one 2-step detour with many 4-step cycles.
    // So we can have a 4-cycle somewhere that we can loop around as many times as needed, and a 2-step detour elsewhere to adjust the modulo.
    
    // Let's design the grid to have a 4-cycle at the top-left that we can loop around, and the rest of the path goes to the end.
    // Actually, we can make the entire walk loop around a 4-cycle many times before proceeding to the end.
    // For example, start at (1,1), loop around the 2x2 block (1,1)-(2,2) many times, then go to (1,2) and continue to (n,m).
    // But we must end at (n,m). So we can loop at the beginning.
    
    // Let's construct the coloring to support:
    // - A 4-cycle at top-left: (1,1) -> (1,2) -> (2,2) -> (2,1) -> (1,1). We can loop this any number of times.
    // - Then from (1,1) we go to (1,2) (already part of the cycle) and then continue right along row 1 to (1,m), then down column m to (n,m).
    // But we need alternating colors along the entire walk.
    
    // Let's assign colors to the 4-cycle to be alternating. For a 4-cycle, we can color edges such that consecutive edges have different colors. This is possible if we alternate. For example:
    // (1,1)-(1,2): R
    // (1,2)-(2,2): B
    // (2,2)-(2,1): R
    // (2,1)-(1,1): B
    // Then going around: R, B, R, B, R, B,... works.
    // Now, after looping, we want to exit the cycle and go to (1,3). We need to transition from some edge of the cycle to the next edge without repeating color.
    // Suppose we finish the loop at (1,1) and then go to (1,2) (which is R). But the last edge of the loop was (2,1)-(1,1) which is B. So B -> R is okay. Then from (1,2) we go to (1,3). The edge (1,2)-(1,3) must be B (since (1,1)-(1,2) is R). So we set hor[1][2] = B.
    // Then we continue alternating along row 1: hor[1][3] = R, hor[1][4] = B, etc.
    // Then down column m: we need to alternate from the last horizontal edge on row 1.
    // This seems consistent.
    
    // But we also need to ensure that the rest of the grid doesn't create any unwanted constraints? There are no constraints other than the existence of the walk. So we can fill the rest arbitrarily.
    
    // However, we must be careful: the walk we intend might not be the only one, but we just need one to exist. So we can set the colors exactly to support our intended walk, and fill the rest with 'R' or 'B' arbitrarily (e.g., all 'R').
    
    // Let's detail the intended walk:
    // We have variables: we need to achieve exactly k steps.
    // Let L0 = n+m-2 (shortest path length).
    // extra = k - L0.
    // We can add steps by looping around the 2x2 at top-left. Each full loop adds 4 steps.
    // We can also add a 2-step detour by doing a "down-right-up" somewhere, e.g., at the end.
    // But we can also just loop the 4-cycle extra/4 times, and if extra%4 == 2, we can do one extra 2-step detour.
    // However, we need to ensure the walk is valid and ends at (n,m).
    
    // Let's design the walk as follows:
    // Start at (1,1).
    // Loop around the 2x2 block (1,1)-(2,2) 'loop_count' times.
    // Then go from (1,1) to (1,2) to (1,3) ... to (1,m).
    // Then go down column m to (n,m).
    // But if extra%4 == 2, we need an additional 2 steps. We can insert a detour at the end: instead of going directly from (n-1,m) to (n,m), we go (n-1,m) -> (n-1,m-1) -> (n,m-1) -> (n,m). This adds 2 steps.
    // Let's check if this detour works with alternating colors.
    // The normal path near the end: ... -> (n-1,m-1) -> (n-1,m) -> (n,m). 
    // Detour: ... -> (n-1,m-1) -> (n,m-1) -> (n,m) -> (n-1,m) -> (n,m)? Again, the last step repeats (n-1,m)-(n,m). So that's invalid.
    // Alternative detour at the end: (n-1,m-1) -> (n-1,m) -> (n,m) -> (n,m-1) -> (n,m)? Repeats (n,m)-(n,m-1) and (n,m-1)-(n,m).
    // So a 2-step detour at the very end corner is impossible without repeating an edge.
    
    // What about a detour earlier in column m? For example, at (i,m) for i < n-1.
    // Normal: (i,m) -> (i+1,m).
    // Detour: (i,m) -> (i,m-1) -> (i+1,m-1) -> (i+1,m). This adds 2 steps.
    // Let's check edges: (i,m)-(i,m-1) [hor], (i,m-1)-(i+1,m-1) [ver], (i+1,m-1)-(i+1,m) [hor], then continue to (i+2,m) via (i+1,m)-(i+2,m) [ver].
    // This works! No consecutive same edge.
    // So we can insert this detour at any row i (1 <= i <= n-1) along column m.
    // We can even insert multiple such detours at different rows if we need more than 2 extra steps? But we already have the 4-cycle for large additions. We only need at most one 2-step detour to handle extra%4 == 2.
    
    // So the plan:
    // - If extra%4 == 0: just loop the 4-cycle extra/4 times, then take shortest path.
    // - If extra%4 == 2: loop the 4-cycle (extra-2)/4 times, then take shortest path but with one detour at some row on column m.
    
    // But wait: the shortest path itself goes through the 4-cycle? The shortest path from (1,1) to (n,m) goes (1,1)->(1,2)->...->(1,m)->(2,m)->...->(n,m). It does not go through (2,1) or (2,2). So the 4-cycle is a detour from the shortest path. If we loop the 4-cycle, we are adding steps at the beginning. That's fine.
    
    // Let's define the walk explicitly:
    // Start at (1,1).
    // For 'loop_count' times:
    //   (1,1) -> (1,2) -> (2,2) -> (2,1) -> (1,1)
    // Then (1,1) -> (1,2) -> (1,3) -> ... -> (1,m)
    // Then for i from 1 to n-1:
    //   if we need to insert detour at row i:
    //     (i,m) -> (i,m-1) -> (i+1,m-1) -> (i+1,m)
    //   else:
    //     (i,m) -> (i+1,m)
    // End at (n,m).
    
    // We need to assign colors to all edges used in this walk such that consecutive edges have different colors.
    // We also need to output colors for all edges in the grid. Unused edges can be anything.
    
    // Let's assign colors step by step according to the walk.
    // We'll build the walk sequence and set colors as we go, ensuring alternation.
    // But we need to output the entire grid before knowing the walk? We can just define the colors based on the pattern we choose.
    
    // Let's fix the colors of the 4-cycle:
    // hor[1][1] = 'R'  // (1,1)-(1,2)
    // ver[1][2] = 'B'  // (1,2)-(2,2)  (ver[1][2] is edge between (1,2) and (2,2))
    // hor[2][1] = 'R'  // (2,1)-(2,2)
    // ver[1][1] = 'B'  // (1,1)-(2,1)
    // Check cycle: (1,1) -R-> (1,2) -B-> (2,2) -R-> (2,1) -B-> (1,1). Alternates: R, B, R, B. Good.
    
    // Now, after the loop, we are at (1,1). We go to (1,2) via hor[1][1] = 'R'. Then we continue to (1,3): we need hor[1][2] = 'B' (since last was 'R').
    // Then hor[1][3] = 'R', hor[1][4] = 'B', ... alternating.
    // So for j=1..m-1: hor[1][j] = (j%2==1)?'R':'B'. This matches our earlier assignment for j=1: 'R', j=2: 'B', etc.
    
    // Now we go down column m. The last horizontal edge on row 1 is hor[1][m-1]. Its color is ((m-1)%2==1)?'R':'B'.
    // Then we go (1,m) -> (2,m): ver[1][m] must be the opposite.
    // Then (2,m) -> (3,m): ver[2][m] opposite of ver[1][m], etc.
