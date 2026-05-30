// Hint3
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    string s;
    cin >> s;
    s = " " + s; // 1-indexed

    // Count leaves (excluding root)
    vector<int> leaf(n + 1, 0);
    for (int i = 2; i <= n; i++) {
        if (adj[i].size() == 1) leaf[i] = 1;
    }

    // Count fixed leaves by value
    int leaf0 = 0, leaf1 = 0, leafQ = 0;
    for (int i = 2; i <= n; i++) {
        if (leaf[i]) {
            if (s[i] == '0') leaf0++;
            else if (s[i] == '1') leaf1++;
            else leafQ++;
        }
    }

    // Count non-leaf '?' (excluding root)
    int nonLeafQ = 0;
    for (int i = 2; i <= n; i++) {
        if (!leaf[i] && s[i] == '?') nonLeafQ++;
    }

    int rootVal = (s[1] == '?' ? -1 : s[1] - '0');

    // If root is fixed
    if (rootVal != -1) {
        // Iris wants to maximize leaves with value != rootVal
        // Dora wants to minimize that
        // Iris will set leaf '?' to opposite of root, Dora will set to same as root
        // They alternate, Iris goes first
        int irisMoves = (leafQ + 1) / 2;
        int doraMoves = leafQ / 2;
        int ans;
        if (rootVal == 0) {
            ans = leaf1 + irisMoves;
        } else {
            ans = leaf0 + irisMoves;
        }
        cout << ans << "\n";
        return;
    }

    // Root is '?'
    // Iris will set root to something, then game continues
    // Iris wants to maximize final score, Dora minimizes
    // Iris can choose root = 0 or 1
    // After root is set, the game reduces to fixed root case with remaining '?' (including non-leaf '?' and leaf '?')
    // But note: non-leaf '?' can also be set by players, but they don't directly affect leaf weight except through root? 
    // Actually, weight of leaf depends only on root value and leaf value (parity of length doesn't matter? Wait, hints say weight non-zero iff root != leaf)
    // Let's verify: weight = (#10 - #01). For a path string, if we compress consecutive same characters, we get alternating sequence.
    // The difference #10 - #01 depends only on first and last character. If first=last, diff=0; if first=0,last=1 diff=1; if first=1,last=0 diff=-1.
    // So non-zero weight exactly when root != leaf.
    // So non-leaf '?' don't affect leaf weight at all! They are irrelevant.
    // So only root and leaf values matter.
    // Thus, after root is set, the game is just about leaf '?'.
    // But Iris chooses root value first, then they alternate on leaf '?'.
    // Wait, the game: Iris goes first, she can choose ANY '?' vertex, not necessarily root.
    // So she might not set root first. But root is '?', so it's available.
    // However, setting non-leaf '?' doesn't change anything, so optimal play will only involve root and leaf '?'.
    // Since non-leaf '?' are useless, players will ignore them until forced? But they can be used to waste turns.
    // The turn order: Iris, Dora, Iris, Dora, ... on all '?' vertices.
    // If there are non-leaf '?' vertices, they act as "pass" moves that don't affect score.
    // So the game is: there are leafQ leaf '?' and nonLeafQ non-leaf '?', plus root '?'.
    // Total '?' = 1 + leafQ + nonLeafQ.
    // Players alternate, Iris first.
    // They can choose any '?'. Setting root determines the target for leaves.
    // Iris wants to maximize leaves with value != root, Dora wants to minimize.
    // This is a combinatorial game. We can think of it as: Iris will eventually set root to some value, and then they fight over leaf '?'.
    // But Iris might delay setting root to gain advantage? Actually, setting root early or late doesn't change the number of moves each player gets on leaf '?'.
    // Because total moves on leaf '?' is leafQ, and they alternate on all '?'. The allocation of moves on leaf '?' depends on when root is set.
    // However, since non-leaf '?' are dummy moves, the player who sets root essentially uses one of their moves.
    // Let's analyze: total '?' = T = 1 + leafQ + nonLeafQ.
    // The game ends after T moves. Iris gets ceil(T/2) moves, Dora gets floor(T/2) moves.
    // Among these moves, some are spent on root, some on leaf '?', some on non-leaf '?'.
    // Iris can decide which moves to use on what. She wants to maximize final count of leaves with value != root.
    // Dora wants to minimize.
    // This is a classic game: Iris chooses root value (0 or 1) at some point. After root is set, the remaining leaf '?' moves: Iris will set them to opposite of root, Dora to same as root.
    // So if root is set to r, and Iris gets k moves on leaf '?' after root is set (or including the move setting root? careful), then final score = fixed_leaves_opposite(r) + k.
    // But Iris can also set leaf '?' before root is set? If she sets a leaf '?' before root is known, she doesn't know which value to choose. She might guess, but Dora can then choose root to invalidate.
    // So optimal play: Iris will set root as soon as possible to force the target, or maybe delay? Let's think.
    // Suppose Iris sets a leaf '?' to 0. Then Dora can set root to 0, making that leaf weight 0. So Iris shouldn't set leaf before root.
    // Similarly, Dora wouldn't set leaf before root because she doesn't know root's value.
    // So both players will only set non-leaf '?' or root until root is set. Once root is set, they set leaf '?' optimally.
    // So the game: Iris wants to set root to her preferred value, but Dora might set root to the opposite if she gets a chance.
    // Actually, Iris can set root on her first move. Then she gets ceil((T-1)/2) moves on leaf '?' (since one move used on root).
    // If she doesn't set root, Dora might set root on her move, choosing the value that minimizes score.
    // So Iris will definitely set root on her first move to choose the value that maximizes her final score, given the remaining moves.
    // Let's check: If Iris sets root to 0, then remaining '?' = T-1. Iris will get ceil((T-1)/2) moves on leaf '?' (since she just moved, next is Dora). She will use them to set leaf to 1.
    // Dora gets floor((T-1)/2) moves on leaf '?', setting to 0.
    // So score = leaf1 + Iris_moves_on_leaf.
    // Similarly if root = 1, score = leaf0 + Iris_moves_on_leaf.
    // Iris will choose the max.
    // But wait: What if there are non-leaf '?' that Iris is forced to use because leaf '?' run out? The moves on leaf '?' are limited by leafQ. So Iris can't necessarily use all her moves on leaf '?'.
    // She will use as many as possible on leaf '?', and the rest on non-leaf '?' (wasted).
    // So actual Iris moves on leaf '?' = min(Iris_moves_available, leafQ) but only if she wants to set them to opposite. She will always want to set leaf '?' to opposite of root.
    // Dora will use her moves on leaf '?' to set to same as root, up to leafQ.
    // So the number of leaf '?' set to opposite = min(Iris_moves, leafQ) but also limited by Dora taking some.
    // Actually, total leaf '?' = leafQ. They will be divided among Iris and Dora based on who gets to them first.
    // Since both want to set them, they will prioritize leaf '?' over non-leaf '?'.
    // So the allocation of leaf '?' moves: Iris gets ceil(leafQ / 2) if she goes first on leaf '?'? Not exactly, because the turn order on leaf '?' depends on when they start setting leaf '?'.
    // But since both want to set leaf '?' as soon as root is set, they will immediately use their moves on leaf '?' until leaf '?' are exhausted.
    // So after root is set, the remaining game is just alternating on leaf '?' then non-leaf '?'.
    // Let R be the move number when root is set (1-indexed). Iris sets root on move 1. Then moves 2,3,... are for leaf '?' and non-leaf '?'.
    // Iris's remaining moves: she gets moves 3,5,7,... (since she moved on 1, next is Dora on 2, Iris on 3, etc.)
    // So Iris gets ceil((T-1)/2) moves after root. Dora gets floor((T-1)/2) moves.
    // They will first use these moves on leaf '?'. So Iris sets min(ceil((T-1)/2), leafQ) leaves to opposite.
    // Dora sets min(floor((T-1)/2), leafQ) leaves to same.
    // But they can't set more than leafQ total. The actual number of leaf '?' set to opposite = min(Iris_moves, leafQ) but if Iris_moves + Dora_moves > leafQ, then they compete.
    // Since they alternate, the first (leafQ) moves among the remaining moves will be used on leaf '?'.
    // So the number of leaf '?' that Iris gets to set = number of Iris's moves among the first leafQ moves after root is set.
    // Since Iris moves on odd steps after root (if root was move 1), the sequence of moves after root: Dora (2), Iris (3), Dora (4), Iris (5), ...
    // So among the first k moves after root, Iris gets ceil(k/2) moves? Wait: move 2 is Dora, move 3 Iris, move 4 Dora, move 5 Iris. So among moves 2..(k+1), Iris gets floor(k/2) moves? Let's list:
    // k=1: move 2 (Dora) -> Iris 0
    // k=2: moves 2,3 -> Iris 1
    // k=3: moves 2,3,4 -> Iris 1
    // k=4: moves 2,3,4,5 -> Iris 2
    // So Iris gets floor(k/2) moves among the first k moves after root.
    // Total leaf '?' = leafQ. So k = leafQ (if leafQ <= remaining moves). Then Iris gets floor(leafQ/2) moves on leaf '?'.
    // But wait, if leafQ > remaining moves, then all remaining moves are used on leaf '?', and Iris gets ceil((T-1)/2) moves (all her remaining moves) but only if she is the one setting them? Actually if leafQ is large, they will just alternate on leaf '?' until the game ends. So Iris gets all her remaining moves on leaf '?'.
    // So Iris_moves_on_leaf = min( ceil((T-1)/2), leafQ )? Not exactly, because if leafQ is small, she might not get all her moves on leaf '?'.
    // Let's formalize: After root set, there are M = T-1 moves left. Iris gets I = ceil(M/2) moves, Dora gets D = floor(M/2) moves.
    // They both want to set leaf '?'. So they will use their moves on leaf '?' until leaf '?' are gone.
    // The total leaf '?' set = min(leafQ, M). They alternate starting with Dora (since Iris just moved).
    // So among the first L = min(leafQ, M) moves after root, Iris gets floor(L/2) moves (because Dora goes first).
    // If M > leafQ, then after leaf '?' exhausted, the remaining moves are on non-leaf '?' and don't affect score.
    // So Iris_moves_on_leaf = floor( min(leafQ, M) / 2 ).
    // Let's check: M = T-1. T = 1 + leafQ + nonLeafQ. So M = leafQ + nonLeafQ.
    // Then min(leafQ, M) = leafQ. So Iris_moves_on_leaf = floor(leafQ / 2).
    // Wait, that means Iris always gets floor(leafQ/2) moves on leaf '?' regardless of nonLeafQ? That seems odd.
    // Let's test: leafQ=1, nonLeafQ=0. T=2. Iris sets root (move 1). M=1. Dora gets move 2. She sets the only leaf '?' to same as root. Iris gets 0 moves on leaf. floor(1/2)=0. Correct.
    // leafQ=2, nonLeafQ=0. T=3. Iris root (1). Dora leaf (2). Iris leaf (3). Iris gets 1 move on leaf. floor(2/2)=1. Correct.
    // leafQ=2, nonLeafQ=1. T=4. Iris root (1). Dora leaf (2). Iris leaf (3). Dora nonLeaf (4). Iris gets 1 move on leaf. floor(2/2)=1. Correct.
    // leafQ=3, nonLeafQ=0. T=4. Iris root (1). Dora leaf (2). Iris leaf (3). Dora leaf (4). Iris gets 1 move on leaf. floor(3/2)=1. Correct.
    // So indeed, if Iris sets root on move 1, she gets exactly floor(leafQ/2) moves on leaf '?'.
    // Then score if root=0: leaf1 + floor(leafQ/2). If root=1: leaf0 + floor(leafQ/2).
    // Iris will choose root to maximize this: max(leaf1, leaf0) + floor(leafQ/2).
    // But wait, is it always optimal for Iris to set root on move 1? What if she delays and sets a non-leaf '?' first?
    // Suppose she sets a non-leaf '?' on move 1. Then Dora could set root on move 2 to the value that minimizes score.
    // If Dora sets root on move 2, then Iris's remaining moves: M = T-2. Iris gets ceil(M/2) moves after that? Let's analyze.
    // If Dora sets root on move 2, then the sequence: Iris (1) nonLeaf, Dora (2) root, then Iris (3), Dora (4), ...
    // After root set (move 2), remaining moves M = T-2. Iris gets moves 3,5,7,... among these. That's ceil(M/2) moves? M moves left, starting with Iris on move 3. So Iris gets ceil(M/2) moves, Dora gets floor(M/2).
    // Then leaf '?' allocation: they alternate starting with Iris. So among first L = min(leafQ, M) moves after root, Iris gets ceil(L/2) moves.
    // M = T-2 = leafQ + nonLeafQ - 1.
    // So Iris_moves_on_leaf = ceil( min(leafQ, M) / 2 ).
    // If leafQ <= M, then Iris gets ceil(leafQ/2). If leafQ > M, she gets ceil(M/2).
    // Dora will choose root to minimize score. She will choose root=0 if leaf1 + Iris_moves <= leaf0 + Iris_moves? Actually she wants to minimize final score.
    // Score if root=0: leaf1 + Iris_moves_on_leaf. If root=1: leaf0 + Iris_moves_on_leaf.
    // Dora will choose the smaller. So score = min(leaf1, leaf0) + Iris_moves_on_leaf.
    // Iris wants to maximize this. She can choose whether to set root on move 1 or do something else.
    // If she sets root on move 1, score = max(leaf1, leaf0) + floor(leafQ/2).
    // If she does nonLeaf on move 1, Dora sets root on move 2, score = min(leaf1, leaf0) + ceil(leafQ/2) (assuming leafQ <= M, which is leafQ + nonLeafQ - 1, so leafQ <= M iff nonLeafQ >= 1; if nonLeafQ=0, then M = leafQ - 1, so leafQ > M, then Iris_moves = ceil((leafQ-1)/2) = floor(leafQ/2) ).
    // Let's compute both scenarios.
    // Actually, Iris might also set a leaf '?' on move 1? That's bad because she doesn't know root.
    // So Iris's options: set root on move 1, or set non-leaf '?' on move 1 (if available), or maybe set nothing? She must set something.
    // If nonLeafQ > 0, she can set a non-leaf '?'. Then Dora can set root. But Dora might not set root if it's bad for her? Dora wants to minimize score. She will set root to the value that gives min score. She can also set a non-leaf '?' to delay? But delaying doesn't change the eventual allocation of leaf '?' moves? Let's see.
    // If both players just set non-leaf '?' until they are exhausted, then eventually someone must set root. The game is finite. The player who sets root determines the target, but the other player might get advantage in leaf '?' moves.
    // This is a classic "turn order" game. We can think of it as: there are leafQ important moves, and the rest are dummy. The root is a special move that sets the target. The player who sets root essentially uses a move, and then the remaining leaf '?' moves are divided.
    // We can compute the outcome if Iris sets root on her k-th move, etc. But since both play optimally, we can use minimax.
    // However, note that the score only depends on root value and how many leaf '?' Iris gets to set to opposite.
    // Let's denote the game state by (turn, root_set, root_val, leafQ_rem, nonLeafQ_rem). But that's too large.
    // Observe that non-leaf '?' are completely symmetric and don't affect score except as turn wasters. So the game is equivalent to: there are A = leafQ leaf '?' and B = nonLeafQ + 1 (including root) special '?' that are "root or dummy". But root is special because setting it determines the target.
    // Actually, setting a non-leaf '?' is exactly like a pass. Setting root is the only move that changes the game phase.
    // So we can think: initially, root is '?'. Players alternate. On a player's turn, they can either:
    // - Set root to 0 or 1 (if not set yet). This ends the "root phase".
    // - Set a leaf '?' to 0 or 1 (but they don't know root, so they would be guessing; however, they can set it to anything, but it will be evaluated later. Since they want to maximize/minimize final score, setting a leaf before root is risky. They can set it to a value, and later root is set. The final contribution of that leaf will be 1 if its value != root, else 0. So setting a leaf early is equivalent to committing to a value. The opponent can then choose root to invalidate it. So it's never optimal for Iris to set a leaf before root, because Dora can just set root to that value. Similarly, Dora wouldn't set a leaf before root because Iris could set root opposite. So in optimal play, no one sets a leaf before root.)
    // - Set a non-leaf '?' (dummy move).
    // So the root phase consists of players taking dummy moves until someone sets root. Since dummy moves don't affect anything, the game is essentially: Iris can choose to set root on her first move, or pass (by setting non-leaf). If she passes, Dora can set root or pass, etc.
    // This is a game of "who sets root". The player who sets root uses their turn, and then the leaf phase begins with the other player to move.
    // In the leaf phase, there are leafQ leaf '?'. Players alternate setting them. Iris wants to set them to opposite of root, Dora to same. They will do so optimally.
    // So if root is set by player P, and the other player Q starts the leaf phase, then the number of leaf '?' that Iris gets to set is:
    // If Iris starts leaf phase: she gets ceil(leafQ/2) moves on leaf.
    // If Dora starts leaf phase: Iris gets floor(leafQ/2) moves on leaf.
    // Additionally, the player who set root chose the root value. Iris wants to maximize final score, Dora wants to minimize.
    // So if Iris sets root, she will choose root value to maximize: max(leaf1, leaf0) + (Iris_moves_in_leaf_phase).
    // Since she used her turn to set root, Dora starts leaf phase. So Iris_moves = floor(leafQ/2).
    // Score = max(leaf1, leaf0) + floor(leafQ/2).
    // If Dora sets root, she will choose root value to minimize: min(leaf1, leaf0) + (Iris_moves_in_leaf_phase).
    // Since Dora used her turn, Iris starts leaf phase. So Iris_moves = ceil(leafQ/2).
    // Score = min(leaf1, leaf0) + ceil(leafQ/2).
    // Now, the root phase: Iris moves first. She can either set root (yielding score S1 = max(leaf1, leaf0) + floor(leafQ/2)) or pass (if nonLeafQ > 0). If she passes, Dora can either set root (yielding S2 = min(leaf1, leaf0) + ceil(leafQ/2)) or pass (if nonLeafQ still > 0 after Iris's pass). They will continue until someone sets root or non-leaf '?' run out.
    // If non-leaf '?' run out (i.e., all dummy moves used), then someone must set root. The player whose turn it is must set root (since no other moves). So we can compute the outcome based on the parity of nonLeafQ.
    // Let's denote the number of dummy moves available = nonLeafQ. Initially, it's Iris's turn. They alternate taking dummy moves. The game ends when a player decides to set root, or when dummy moves are exhausted.
    // Since both play optimally, we can compute the minimax value.
    // Let f(k, turn) be the outcome when there are k dummy moves left, and it's turn's player to move (0 for Iris, 1 for Dora). The players can either set root or take a dummy (if k>0).
    // If they set root, the game ends with the corresponding score.
    // Iris wants to maximize, Dora wants to minimize.
    // We can compute f recursively:
    // For Iris:
    //   option1: set root -> S1
    //   option2: if k>0, take dummy -> f(k-1, Dora)
    //   Iris chooses max.
    // For Dora:
    //   option1: set root -> S2
    //   option2: if k>0, take dummy -> f(k-1, Iris)
    //   Dora chooses min.
    // Base case: k=0, must set root.
    // This is a simple dynamic programming on parity. Since k up to 1e5, we can just compute it in O(1) by analyzing.
    // Let's compute S1 and S2:
    // S1 = max(leaf1, leaf0) + floor(leafQ/2)
    // S2 = min(leaf1, leaf0) + ceil(leafQ/2)
    // Note that S1 >= S2? Not necessarily. max + floor vs min + ceil. Since ceil = floor + (leafQ%2), and max >= min.
    // If leafQ is even, floor=ceil=leafQ/2. Then S1 = max + leafQ/2, S2 = min + leafQ/2. So S1 >= S2.
    // If leafQ is odd, floor = (leafQ-1)/2, ceil = (leafQ+1)/2. Then S1 = max + (leafQ-1)/2, S2 = min + (leafQ+1)/2.
    // Difference: S1 - S2 = max - min - 1. Since max - min is either 0 (if leaf0==leaf1) or >0. If leaf0==leaf1, max=min, so S1 - S2 = -1, so S1 < S2. If leaf0!=leaf1, max-min >=1, so S1 >= S2.
    // So S1 and S2 can be ordered.
    // Now, the game with k dummy moves. Iris wants to maximize, Dora minimize.
    // Let's denote the value of the game for Iris when k dummies left and turn = Iris: V_I(k), turn = Dora: V_D(k).
    // V_I(k) = max(S1, k>0 ? V_D(k-1) : -inf)
    // V_D(k) = min(S2, k>0 ? V_I(k-1) : inf)
    // We can compute these. Since S1 and S2 are constants, we can find the pattern.
    // Let's compute small k:
    // k=0:
    // V_I(0) = max(S1) = S1 (since no dummy)
    // V_D(0) = min(S2) = S2
    // k=1:
    // V_I(1) = max(S1, V_D(0)) = max(S1, S2)
    // V_D(1) = min(S2, V_I(0)) = min(S2, S1)
    // k=2:
    // V_I(2) = max(S1, V_D(1)) = max(S1, min(S2, S1))
    // V_D(2) = min(S2, V_I(1)) = min(S2, max(S1, S2))
    // Notice that max(S1, min(S2, S1)) = S1 (since S1 >= min(S2,S1) always? Actually if S1 < S2, min(S2,S1)=S1, so max(S1,S1)=S1. If S1 >= S2, min(S2,S1)=S2, max(S1,S2)=S1. So V_I(2)=S1.)
    // Similarly, min(S2, max(S1,S2)) = S2 (if S1>=S2, max=S1, min(S2,S1)=S2; if S1<S2, max=S2, min(S2,S2)=S2). So V_D(2)=S2.
    // So for k>=2, V_I(k)=S1, V_D(k)=S2? Let's check k=3:
    // V_I(3) = max(S1, V_D(2)) = max(S1, S2) = max(S1,S2)
    // V_D(3) = min(S2, V_I(2)) = min(S2, S1) = min(S1,S2)
    // So it alternates: for even k>=2, V_I=S1, V_D=S2; for odd k>=1, V_I=max(S1,S2), V_D=min(S1,S2).
    // Let's verify k=1: V_I(1)=max(S1,S2), V_D(1)=min(S1,S2). Matches.
    // k=0: V_I(0)=S1, V_D(0)=S2. Matches even k=0 pattern? 0 is even, V_I(0)=S1, V_D(0)=S2. Yes.
    // So the pattern depends on parity of k.
    // Our initial state: k = nonLeafQ, turn = Iris (since Iris goes first).
    // So answer = V_I(nonLeafQ).
    // If nonLeafQ is even: V_I = S1.
    // If nonLeafQ is odd: V_I = max(S1, S2).
    // Let's double-check with examples.

    // Example 2: n=4, edges: 1-2, 3-2, 2-4, s="???0"
    // Tree: 1-2-3, 2-4. Root=1. Leaves: 3,4? Vertex 4 has value '0', leaf? 4 is leaf (degree 1). Vertex 3 is leaf (degree 1). Vertex 2 is internal.
    // s[1]='?', s[2]='?', s[3]='?', s[4]='0'.
    // leaf0: vertex 4 is '0' -> leaf0=1.
    // leaf1: 0.
    // leafQ: vertex 3 is '?' -> leafQ=1.
    // nonLeafQ: vertex 2 is '?' and not leaf -> nonLeafQ=1.
    // leafQ=1 (odd). S1 = max(0,1) + floor(1/2) = 1 + 0 = 1.
    // S2 = min(0,1) + ceil(1/2) = 0 + 1 = 1.
    // S1=1, S2=1. max(S1,S2)=1.
    // nonLeafQ=1 (odd) -> V_I = max(S1,S2) = 1. Output 1. Matches sample.

    // Example 3: n=5, edges: 1-2,1-3,2-4,2-5, s="?1?01"
    // Root=1: '?'
    // Leaves: 3,4,5? Vertex 3: '?'? s[3]='?'? s="?1?01" -> indices: 1:'?', 2:'1', 3:'?', 4:'0', 5:'1'. Leaves: 3 (degree 1), 4 (degree 1), 5 (degree 1). Vertex 2 is internal.
    // leaf0: vertex 4='0' -> 1
    // leaf1: vertex 5='1' -> 1
    // leafQ: vertex 3='?' -> 1
    // nonLeafQ: vertex 1 is root '?', vertex 2 is '1' (fixed), so nonLeafQ=0? Wait, root is '?' but we already accounted root separately. nonLeafQ counts non-leaf '?' excluding root. So vertex 2 is '1', not '?'. So nonLeafQ=0.
    // leafQ=1 (odd). S1 = max(1,1) + 0 = 1. S2 = min(1,1) + 1 = 2.
    // nonLeafQ=0 (even) -> V_I = S1 = 1. Output 1. Matches sample.

    // Example 4: n=6, edges: 1-2,2-3,3-4,5-3,3-6, s="?0????"
    // Root=1: '?'
    // Leaves: 4,5,6? Vertex 1 degree? 1 connected to 2. Vertex 2 connected to 1,3. Vertex 3 connected to 2,4,5,6. Leaves: 4,5,6 (degree 1). Vertex 2 is internal? degree 2. Vertex 3 degree 4.
    // s: 1:'?', 2:'0', 3:'?', 4:'?', 5:'?', 6:'?'.
    // leaf0: none? 2 is '0' but not leaf. leaf1: none. leafQ: 4,5,6 -> leafQ=3.
    // nonLeafQ: vertex 3 is '?' and not leaf -> nonLeafQ=1. (root is '?' but excluded)
    // leafQ=3 (odd). S1 = max(0,0) + floor(3/2) = 0 + 1 = 1.
    // S2 = min(0,0) + ceil(3/2) = 0 + 2 = 2.
    // nonLeafQ=1 (odd) -> V_I = max(S1,S2) = 2. Output 2. Matches sample.

    // Example 5: n=5, edges: 1-2,1-3,1-4,1-5, s="11?1?"
    // Root=1: '1' (fixed)
    // Leaves: 2,3,4,5? All children of 1. s: 1:'1', 2:'1', 3:'?', 4:'1', 5:'?'.
    // leaf1: 2,4 -> 2
    // leaf0: 0
    // leafQ: 3,5 -> 2
    // Root fixed to 1. So answer = leaf0 + ceil(leafQ/2)? Wait, formula for fixed root: if root=1, Iris wants leaves with value 0. She gets ceil(leafQ/2) moves to set leaf '?' to 0? Let's re-derive fixed root.
    // Fixed root: rootVal known. Game: Iris and Dora alternate on all '?' (including non-leaf '?'). But non-leaf '?' don't affect score. So they only matter for turn order.
    // Total '?' = leafQ + nonLeafQ. Iris goes first. They alternate setting '?'. Iris will set leaf '?' to opposite of root, Dora to same. They will prioritize leaf '?'.
    // So among the first L = min(leafQ, totalMoves) moves, Iris gets ceil(L/2) moves (since she goes first). If totalMoves > leafQ, she gets ceil(leafQ/2) moves on leaf, and the rest dummy.
    // Wait, in fixed root case, the root is already set, so the game is just on the remaining '?'. Iris goes first. So she gets ceil(leafQ/2) moves on leaf '?' if leafQ <= totalMoves? Actually, totalMoves = leafQ + nonLeafQ. They alternate starting with Iris. So among the first leafQ moves, Iris gets ceil(leafQ/2) moves. If nonLeafQ is large, she still gets exactly ceil(leafQ/2) moves on leaf, because after leafQ moves, leaf '?' are exhausted. So Iris_moves_on_leaf = ceil(leafQ/2) always? Let's check: leafQ=1, nonLeafQ=0: Iris moves first, sets leaf to opposite. Gets 1 move. ceil(1/2)=1. Correct.
    // leafQ=2, nonLeafQ=0: Iris (leaf), Dora (leaf). Iris gets 1. ceil(2/2)=1. Correct.
    // leafQ=2, nonLeafQ=1: Iris (leaf), Dora (leaf), Iris (dummy). Iris gets 1. ceil(2/2)=1. Correct.
    // leafQ=3, nonLeafQ=0: Iris (leaf), Dora (leaf), Iris (leaf). Iris gets 2. ceil(3/2)=2. Correct.
    // So indeed, in fixed root case
