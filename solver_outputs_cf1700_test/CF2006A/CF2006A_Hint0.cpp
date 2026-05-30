// Hint0
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 100005;

vector<int> adj[MAXN];
char val[MAXN];
int n;

int dp[MAXN][2]; // dp[u][c] = min/max score in subtree of u, given that the value of u is c (0 or 1)
// Actually we need to compute the game value. We'll do a DP that returns the game outcome for subtree.

// We need to know for each node, if it's Iris turn or Dora turn? The game is global, not per subtree.
// But we can compute the contribution of each leaf to the score.
// The weight of a leaf depends on the whole path string.
// Let's analyze the weight formula: difference between number of "10" and "01".
// For a string S, weight = (#10) - (#01).
// Notice that (#10) - (#01) = (first char == '1' ? 1 : 0) - (last char == '1' ? 1 : 0) ? Let's check.
// Actually, consider the transitions: each time we go from 0 to 1, we get +1? Wait: "01" gives -1, "10" gives +1.
// So weight = sum over adjacent pairs (i,i+1) of f(S[i],S[i+1]) where f(1,0)=1, f(0,1)=-1, else 0.
// This is like a path integral. It equals (S[0]=='1'?1:0) - (S[last]=='1'?1:0) ? Let's test: S=10110: first 1, last 0 => 1-0=1. Correct.
// S=101: first 1, last 1 => 1-1=0. Correct.
// S=01: first 0, last 1 => 0-1=-1. Correct.
// S=00: first 0, last 0 => 0-0=0. Correct.
// So weight = (value of root) - (value of leaf) ? Wait: root is first char, leaf is last char.
// In our tree, root is vertex 1. So weight = val[1] - val[leaf]? Let's check: val[1] is 0 or 1. val[leaf] is 0 or 1.
// For leaf 5: val[1]=1, val[5]=0 => 1-0=1. Correct.
// For leaf 6: val[1]=1, val[6]=1 => 1-1=0. Correct.
// For leaf 2: val[1]=0, val[2]=1 => 0-1=-1. Correct.
// So weight = val[1] - val[leaf].
// Wow! That simplifies everything.
// Then non-zero weight means val[leaf] != val[1].
// So score = number of leaves with value different from root value.

// The game: Iris and Dora take turns setting '?' to 0 or 1. Iris wants to maximize score, Dora minimize.
// Score = count of leaves with val != val[1].
// Root value may also be '?' initially.

// We can think of this as: each leaf gives a point if its final value differs from root value.
// Iris wants to maximize points, Dora minimize.

// This is a combinatorial game on a tree with some fixed values and some '?'.
// Since the game is impartial? No, it's partisan: Iris wants max, Dora wants min.
// We can solve using DP with minimax.

// Let's denote the state by the set of unfilled '?'. But n up to 1e5, so we need a structural solution.

// Observe that the game only cares about the root value and leaf values. Internal nodes matter only for determining leaf values? But internal nodes can be '?' and players can set them, which might affect multiple leaves? Actually, the value of a leaf is just its own value, not dependent on path except root. Wait: leaf value is the value of the leaf vertex itself. The path string's last character is the leaf's value. So weight = val[1] - val[leaf]. So only root and leaf values matter! Internal nodes' values don't affect the weight at all! But they are part of the game; players can choose to set internal '?' to anything, but it doesn't affect the score. So they are just "dummy" moves that don't change the score. However, they affect the turn order. The game ends when all '?' are filled. So players might use dummy moves to waste turns.

// So the game reduces to: there is a set of important vertices: root and leaves. Some of them have fixed 0/1, some are '?'.
// There are also other '?' vertices that are irrelevant to score.
// Players alternate picking any '?' and setting it to 0 or 1. Iris wants to maximize number of leaves with value != root value. Dora wants to minimize.

// Since irrelevant '?' don't affect score, setting them is just a "pass" that changes turn. So the game is equivalent to: there are k irrelevant '?' and m important '?' (root + leaves). Players can either set an important '?' (affecting score) or an irrelevant '?' (just passing turn). Both players play optimally.

// This is a classic game: we can think of it as a game where each move a player can either make a "scoring move" (set an important variable) or a "wasting move" (set an irrelevant variable). Since both players can waste moves, the parity of total irrelevant '?' matters.

// Let's formalize. Let R be root, L be set of leaves. Some are fixed, some '?'. Let I be the set of other '?' (internal, non-root, non-leaf). Let k = |I|.
// Let a = number of leaves with fixed value 0, b = number of leaves with fixed value 1, c = number of leaves with '?'.
// Root may be fixed 0, fixed 1, or '?'.

// The final score is: if root is 0: score = number of leaves with value 1 = b + (number of '?' leaves set to 1).
// If root is 1: score = number of leaves with value 0 = a + (number of '?' leaves set to 0).

// Iris wants to maximize this, Dora minimize. They set '?' leaves and root (if '?') to 0/1.

// This is a finite perfect information game. We can solve it by analyzing optimal strategies.

// Since the game is symmetric, we can consider the difference between setting a leaf to 0 vs 1. For a given root value, setting a leaf to the opposite value gives +1 score, same value gives 0. So each leaf '?' is a "point" that Iris wants to claim (by making it opposite to root) and Dora wants to deny (by making it same as root). But root value itself might be '?', which determines which value is "good" for Iris.

// Let's denote the state by (root fixed?, root value if fixed, number of leaf '?', number of fixed leaves of each type, number of irrelevant '?', turn).

// Because players can also set the root if it's '?'. Setting root determines the target. Iris will want to set root to the value that maximizes the final score given the remaining leaf '?'. Dora will want to minimize.

// This is a small game, we can compute the minimax value by hand for all cases.

// Let's define a function f(turn, root_state, leaf_counts, k) but we can simplify.

// Let's denote:
// - If root is fixed to 0: Iris wants to maximize (b + number of leaf '?' set to 1). Dora wants to minimize.
// - If root is fixed to 1: Iris wants to maximize (a + number of leaf '?' set to 0).
// - If root is '?': the player who sets root will choose the value that favors them, knowing the remaining game.

// Also, there are k irrelevant '?' that just pass turn.

// This is a standard "turn-based allocation" game. Since each leaf '?' can be set by either player, and they alternate, the outcome depends on the number of leaf '?' and who moves first.

// Let's analyze the subgame where root is already fixed (say to 0). There are c leaf '?', and k irrelevant '?'. Iris wants to maximize the number of leaf '?' set to 1; Dora wants to minimize that (i.e., set them to 0). They alternate, starting with some player. They can also waste moves on irrelevant '?'.

// This is equivalent to: there are c "valuable" moves. On a move, a player can either take a valuable move (set a leaf '?' to their preferred value) or waste a move. The game ends when all c are taken. The player who takes a valuable move gets to choose the value. Iris wants 1s, Dora wants 0s. So each valuable move taken by Iris gives +1 to the count of 1s; taken by Dora gives +0 (since Dora sets to 0). So the final number of 1s among the c leaves is exactly the number of valuable moves taken by Iris.

// This is a game where players alternate picking from a set of c items, with the option to pass by using a waste move. The waste moves are limited to k total (both players combined). This is a classic "taking turns" game. The outcome is determined by the parity of c and who gets the extra moves.

// Actually, since both players can waste, the player who wants to waste will do so only if it benefits them. We can think of it as: the game is played on c items. Players alternate. Iris wants to maximize the number of items she picks; Dora wants to minimize. This is exactly the game where Iris picks first, they alternate, and the total number of items picked by Iris is ceil(c/2) if no waste moves? But with waste moves, a player can skip a turn, effectively passing the turn to the opponent. This can change who gets the last item.

// Let's analyze the game with c items and k waste moves available to both players. The game starts with some player to move. A waste move can be used by any player at any time, but each waste move consumes one '?' from the pool. Since both players have access to the same pool, the total number of moves in the game is c + k. The game ends when all c items are taken. The waste moves just delay the taking of items.

// This is equivalent to: there are c items. Players alternate taking items. However, before taking an item, a player may choose to skip their turn by using a waste move, if any remain. This is the same as saying that the sequence of turns is determined by who uses waste moves.

// We can think of it as a game of "who takes the item" on each turn. Since both players are rational, they will use waste moves only to change the parity of who gets the last few items.

// Let's solve this subgame formally. Let c be the number of leaf '?'. Let k be the number of waste moves. Let the first player be P1 (Iris if it's her turn, Dora if it's her turn). We want to know how many items P1 gets (assuming P1 wants to maximize her items, P2 wants to minimize P1's items). But careful: In the root-fixed subgame, Iris wants to maximize the number of 1s, Dora wants to minimize. So if Iris is the first player, she wants to maximize her taken items; if Dora is first, Dora wants to minimize Iris's items, which is equivalent to maximizing her own taken items (since total items c, Iris's items = c - Dora's items). So in both cases, the first player wants to maximize the number of items they personally take? Wait: If Dora is first, she wants to minimize Iris's items. Dora taking an item means she sets it to 0, so Iris gets 0 from that item. So Dora wants to take as many items as possible. So indeed, both players want to take as many items as possible. So the game is symmetric: each player wants to maximize the number of items they take. The items are identical. So it's a game of picking items alternately, with the option to pass.

// This is a well-known game: with c items and k passes, the first player can guarantee taking ceil(c/2) items if k is even? Let's derive.

// Let's denote the state by (c, k, turn). We want the number of items the first player gets.
// Base: if c=0, first player gets 0.
// If k=0, it's just alternating: first player gets ceil(c/2).
// If k>0, the first player can either take an item (c -> c-1, turn flips, k unchanged) or use a pass (k -> k-1, turn flips, c unchanged). The opponent will then choose optimally to minimize/maximize? Actually both want to maximize their own items. So it's a zero-sum game with payoff = items for first player.

// Let F(c, k) be the number of items the first player gets assuming optimal play (both maximizing their own items).
// Then F(c, 0) = ceil(c/2).
// For k>0:
// If first player takes an item: she gets 1 + (c-1 - F(c-1, k))? Wait: after she takes, it's opponent's turn with c-1 items and k passes. The opponent will get F(c-1, k) items from the remaining. So first player's total = 1 + ( (c-1) - F(c-1, k) )? No: total items = c. First player's items = 1 + (items she gets from the rest). But after she takes, the opponent is the first player in the subgame with c-1 items. The opponent will get F(c-1, k) items. So the first player will get the rest: (c-1) - F(c-1, k) items from the remaining, plus the 1 she just took. So total = 1 + (c-1 - F(c-1, k)) = c - F(c-1, k).
// If first player passes: she gets 0 now, and it's opponent's turn with c items and k-1 passes. The opponent will get F(c, k-1) items. So first player gets c - F(c, k-1).
// First player will choose the maximum of these two: F(c, k) = max( c - F(c-1, k), c - F(c, k-1) ).
// This is equivalent to F(c, k) = c - min( F(c-1, k), F(c, k-1) ).

// We can compute F for small c,k and see pattern.
// F(c,0) = ceil(c/2).
// F(0,k) = 0.
// Let's compute:
// c=1, k=0: F=1.
// c=1, k=1: F = 1 - min(F(0,1), F(1,0)) = 1 - min(0,1) = 1.
// c=1, k=2: F = 1 - min(F(0,2), F(1,1)) = 1 - min(0,1) = 1.
// So for c=1, F=1 always.
// c=2, k=0: F=1.
// c=2, k=1: F = 2 - min(F(1,1), F(2,0)) = 2 - min(1,1) = 1.
// c=2, k=2: F = 2 - min(F(1,2), F(2,1)) = 2 - min(1,1) = 1.
// c=3, k=0: F=2.
// c=3, k=1: F = 3 - min(F(2,1), F(3,0)) = 3 - min(1,2) = 2.
// c=3, k=2: F = 3 - min(F(2,2), F(3,1)) = 3 - min(1,2) = 2.
// c=4, k=0: F=2.
// c=4, k=1: F = 4 - min(F(3,1), F(4,0)) = 4 - min(2,2) = 2.
// c=4, k=2: F = 4 - min(F(3,2), F(4,1)) = 4 - min(2,2) = 2.
// It seems F(c,k) = ceil(c/2) for all k? Let's check c=5, k=0: F=3. c=5, k=1: 5 - min(F(4,1), F(5,0)) = 5 - min(2,3) = 3. Yes.
// So F(c,k) is always ceil(c/2)? Wait, is there any case where passing helps? Let's test c=0, k=1: F=0. c=0, k=2: 0.
// What if c=2, k=3? F=1.
// It appears that the option to pass never changes the outcome because min(F(c-1,k), F(c,k-1)) is always F(c-1,k)? Let's check if F(c,k-1) can be smaller than F(c-1,k). For c=1, F(1,k-1)=1, F(0,k)=0, min=0, so F=1. For c=2, F(2,k-1)=1, F(1,k)=1, min=1. For c=3, F(3,k-1)=2, F(2,k)=1, min=1, so F=2. So F(2,k)=1 is smaller than F(3,k-1)=2. So min is F(c-1,k) for c>=2? Actually for c=3, F(2,k)=1, F(3,k-1)=2, min=1. So F(3,k)=2. So passing would give 3-2=1, which is worse than taking (3-1=2). So first player prefers taking. So indeed, passing is never beneficial for the first player if they want to maximize their items? But wait, the opponent also wants to maximize their items. In our game, both players want to maximize their own items. So the first player will never pass if it reduces their items. But could passing be beneficial to change the parity for later? It seems not, because the game is symmetric and the total number of items is fixed. The first player can always guarantee at least ceil(c/2) by just taking whenever possible. The opponent cannot force the first player to get less because the opponent also wants to take items, but taking items reduces the pool. Actually, if the first player passes, the opponent gets the first move on the same c, which gives the opponent ceil(c/2) items, leaving floor(c/2) for the first player, which is worse or equal. So passing is never strictly better. So F(c,k) = ceil(c/2) for all k.

// But wait! This assumes that both players always want to take items. Is that true in our original game? In the root-fixed subgame, Iris wants to maximize the number of 1s, Dora wants to minimize. So Iris wants to take items (set to 1), Dora wants to take items (set to 0). So yes, both want to take items. So the subgame outcome is simply: the number of leaf '?' that end up as 1 is exactly the number of items taken by Iris. Since both players want to take items, they will never waste a move if they can take an item. They will only waste if there are no items left? But the game ends when all items are taken. So waste moves are only used when c=0? But if c>0, a player would always prefer to take an item rather than waste, because taking gives them +1 item, while wasting gives 0 and gives the opponent the chance to take an item. So waste moves are effectively irrelevant as long as there are items. The only time a player might waste is if they are forced to? But they are never forced; they can always take an item if c>0. So waste moves are never used until all items are taken. But the game ends exactly when all items are taken. So waste moves are never used at all! They are just dead moves that no one will ever play because playing them only helps the opponent. So the presence of irrelevant '?' doesn't affect the subgame at all! The players will simply ignore them and only play on the leaf '?' and root '?'. The irrelevant '?' will be left for the end, and they will be filled by whoever is forced to, but they don't change the score. However, they do affect whose turn it is when the important moves are exhausted? Wait: The game ends only when ALL '?' are filled, including irrelevant ones. So after all important '?' are set, there might still be irrelevant '?' left. The players will then be forced to fill them, alternating. But since they don't affect the score, the score is already determined by the important moves. The order of filling irrelevant '?' doesn't change the score. So the irrelevant '?' just add a number of "forced passes" at the end, but they don't affect the important moves because players will always prioritize important moves. So the irrelevant '?' have no strategic value. They just change the total number of moves, but since players can choose which '?' to fill, they will always fill important ones first. So the game on important '?' is played as if irrelevant '?' don't exist. The only thing that matters is who makes the first move on the important '?' set.

// But wait: What if a player wants to waste a move to change the turn order for the important moves? They could fill an irrelevant '?' instead of an important one. But as we argued, passing is never beneficial in the subgame where both want to take items. So they won't do that. So indeed, irrelevant '?' are completely irrelevant.

// Therefore, the game reduces to: We have root and leaves. Some are fixed, some '?'. Players alternate setting the '?' among {root} U {leaves}. Iris wants to maximize score = number of leaves with value != root. Dora wants to minimize.

// Let's denote:
// - Root fixed to 0: Iris wants to maximize number of 1s among leaves. Dora wants to minimize.
// - Root fixed to 1: Iris wants to maximize number of 0s among leaves.
// - Root '?': The player who sets root will choose the value that favors them, given the remaining leaf '?' and turn.

// Let's analyze the game with only root and leaves as '?'. Let L be the set of leaves. Let c = number of leaf '?'. Let a = number of leaves fixed to 0, b = number of leaves fixed to 1.
// Let r be the state of root: 0, 1, or ?.
// Let turn be 0 for Iris, 1 for Dora.

// We want to compute the final score under optimal play.

// Since the game is small, we can compute the minimax value by considering all possible moves. But we need a closed form.

// Let's first consider the case where root is already fixed. Say root=0. Then the game is: there are c leaf '?'. Iris wants to maximize the number of 1s among them; Dora wants to minimize. Both players can set a leaf '?' to 0 or 1. Since Iris wants 1s, she will always set a leaf to 1. Dora will always set a leaf to 0. So the game is exactly the item-taking game: c items, Iris takes -> 1, Dora takes -> 0. The final number of 1s among the c leaves is the number of items taken by Iris. As we argued, both want to take items, so they will never pass. So the outcome depends only on who moves first and c. If Iris moves first, she gets ceil(c/2) items. If Dora moves first, Dora gets ceil(c/2) items, so Iris gets floor(c/2) items. So the number of 1s from '?' leaves = (turn == Iris ? ceil(c/2) : floor(c/2)). Then total score = b + that number.

// Similarly, if root=1, Iris wants 0s. So she will set leaves to 0, Dora sets to 1. Then the number of 0s from '?' leaves = (turn == Iris ? ceil(c/2) : floor(c/2)). Total score = a + that number.

// Now, if root is '?', the game is more complex. The players can choose to set root or a leaf. Setting root determines the target. Iris wants to set root to the value that maximizes the eventual score; Dora wants to minimize.

// Let's denote the state as (c, a, b, turn). Root is '?'. We want to compute the minimax score.

// We can define a function solve(c, a, b, turn) returning the final score.

// Base: if c=0 and root is '?', then the only move is to set root. The player whose turn it is will set root to the value that favors them. If Iris sets root: she will choose between root=0 (score = b) and root=1 (score = a). She picks max(a,b). If Dora sets root: she picks min(a,b). So if c=0, score = (turn == Iris ? max(a,b) : min(a,b)).

// If c>0, the player can either:
// - Set a leaf: they will set it to their preferred value given the current root? But root is not set yet! So if a player sets a leaf, they must decide its value without knowing the root. But they know the future game. This is a game of perfect information, so they can anticipate the eventual root value. Setting a leaf to 0 or 1 will change a or b? Actually, setting a leaf to 0 increases a by 1, setting to 1 increases b by 1. The leaf is no longer '?', so c decreases by 1. The turn passes to the opponent. The root remains '?'.
// - Set root: the player chooses root=0 or root=1. Then the game becomes the root-fixed subgame with the remaining c leaf '?' and the opponent to move.

// So we can write a recursive minimax. But n up to 1e5, so we need O(1) per test case. We can find a pattern.

// Let's analyze the game with root '?'. Let's denote the state by (c, a, b, turn). Since a and b only matter for the final score, and the game is symmetric, we can think of the difference.

// Notice that if a player sets a leaf, they can choose 0 or 1. Which one is better? It depends on what they expect the root to be. If they think root will be 0, then setting leaf to 1 gives +1 score (since score = b + ...), setting to 0 gives 0. If root will be 1, setting leaf to 0 gives +1, setting to 1 gives 0. So a player wants to set the leaf to the value opposite to the eventual root. But they don't know the root yet. However, they can force the root? Actually, the player who sets root will decide it. So if Iris sets a leaf, she might try to influence the future root decision? But the root decision is made by some player later. This is a game of perfect information; we can solve it by backward induction.

// Let's try to compute the minimax value for small c and all a,b,turn. Since a and b are just numbers, we can see the pattern.

// Let f(c, a, b, turn) be the score.
// If turn == Iris (maximizer):
// f(c,a,b,Iris) = max(
//     // option 1: set root to 0
//     score if root=0 and opponent moves on c leaves = b + (c leaf game with Dora first) = b + floor(c/2)
//     // option 2: set root to 1
//     a + floor(c/2)
//     // option 3: set a leaf to 0 -> state (c-1, a+1, b, Dora)
//     f(c-1, a+1, b, Dora)
//     // option 4: set a leaf to 1 -> state (c-1, a, b+1, Dora)
//     f(c-1, a, b+1, Dora)
// )
// If turn == Dora (minimizer):
// f(c,a,b,Dora) = min(
//     // set root to 0
//     b + ceil(c/2)   // because Dora sets root, then Iris moves first on leaves? Wait: if Dora sets root, the turn passes to Iris. So the leaf game is with Iris first. So number of 1s from leaves = ceil(c/2) if root=0? Actually if root=0, Iris wants 1s, so she gets ceil(c/2) items. So score = b + ceil(c/2).
//     // set root to 1
//     a + ceil(c/2)   // Iris wants 0s, so she gets ceil(c/2) items of 0s. Score = a + ceil(c/2).
//     // set leaf to 0
//     f(c-1, a+1, b, Iris)
//     // set leaf to 1
//     f(c-1, a, b+1, Iris)
// )

// Let's test small values. We can write a quick script in mind or deduce pattern.

// Notice that the options of setting root give a value that depends only on a, b, c and the parity of c. The leaf options recurse.

// Let's try to see if setting a leaf is ever better than setting root. For Iris, setting root gives max(a,b) + floor(c/2). Setting a leaf gives f(c-1, a+1, b, Dora) or f(c-1, a, b+1, Dora). Since Dora will then minimize, she might set root or leaf.

// Let's compute for c=1.
// c=1, a, b, Iris:
// Set root: max(a,b) + floor(1/2) = max(a,b) + 0 = max(a,b).
// Set leaf to 0: state (0, a+1, b, Dora). f(0, a+1, b, Dora) = min(a+1, b) (since Dora sets root).
// Set leaf to 1: state (0, a, b+1, Dora) = min(a, b+1).
// So Iris will choose max( max(a,b), min(a+1,b), min(a,b+1) ).
// If a >= b: max(a,b)=a. min(a+1,b)=b (since a>=b => a+1 > b). min(a,b+1): if a >= b+1, then min=a? Actually min(a,b+1) = b+1 if a > b+1? Wait: a and b are integers. If a >= b+1, then min(a,b+1)=b+1. If a == b, then min(a,b+1)=a=b. So:
// Case a > b: max=a. min(a+1,b)=b. min(a,b+1)=b+1 (if a > b+1) or a (if a == b+1). So max of these is a. So Iris gets a.
// Case a == b: max=a. min(a+1,a)=a. min(a,a+1)=a. So a.
// Case a < b: symmetric, gets b.
// So for c=1, Iris gets max(a,b). So setting root is optimal (or equivalent).

// c=1, Dora:
// Set root: min(a,b) + ceil(1/2) = min(a,b) + 1.
// Set leaf to 0: f(0, a+1, b, Iris) = max(a+1, b).
// Set leaf to 1: f(0, a, b+1, Iris) = max(a, b+1).
// Dora wants min. So she will choose min( min(a,b)+1, max(a+1,b), max(a,b+1) ).
// Let's test a=0, b=0: min(0,0)+1=1. max(1,0)=1. max(0,1)=1. So min=1.
// a=1, b=0: min(1,0)+1=1. max(2,0)=2. max(1,1)=1. min=1.
// a=0, b=1: symmetric, 1.
// a=2, b=0: min=0+1=1. max(3,0)=3. max(2,1)=2. min=1.
// a=1, b=1: min=1+1=2. max(2,1)=2. max(1,2)=2. min=2.
// a=2, b=1: min=1+1=2. max(3,1)=3. max(2,2)=2. min=2.
// a=3, b=0: min=0+1=1. max(4,0)=4. max(3,1)=3. min=1.
// So Dora gets min(a,b)+1 if min(a,b)+1 <= max(a+1,b) and max(a,b+1). It seems min(a,b)+1 is always <= the maxes? For a=2,b=0: min+1=1, maxes are 3 and 2, so 1 is min. For a=1,b=1: min+1=2, maxes=2, so 2. For a=0,b=0: 1. So Dora's optimal is min(a,b)+1? But wait, if a=0,b=0, min+1=1. If she sets leaf to 0, state goes to Iris with (0,1,0) -> max(1,0)=1. So same. So for c=1, Dora gets min(a,b)+1.

// Now c=2, Iris:
// Set root: max(a,b) + floor(2/2) = max(a,b) + 1.
// Set leaf to 0: f(1, a+1, b, Dora). We need f(1, a', b', Dora). From above, for c=1, Dora gets min(a',b')+1. So f(1, a+1, b, Dora) = min(a+1, b) + 1.
// Set leaf to 1: f(1, a, b+1, Dora) = min(a, b+1) + 1.
// So Iris chooses max( max(a,b)+1, min(a+1,b)+1, min(a,b+1)+1 ) = max( max(a,b), min(a+1,b), min(a,b+1) ) + 1.
// As before, the max of those three is max(a,b). So Iris gets max(a,b) + 1.
// So setting root is optimal.

// c=2, Dora:
// Set root: min(a,b) + ceil(2/2) = min(a,b) + 1.
// Set leaf to 0: f(1, a+1, b, Iris). f(1, a', b', Iris) = max(a',b') (from c=1 Iris). So = max(a+1, b).
// Set leaf to 1: f(1, a, b+1, Iris) = max(a, b+1).
// Dora wants min of these.
// Let's test a=0,b=0: min+1=1. max(1,0)=1. max(0,1)=1. min=1.
// a=1,b=0: min+1=1. max(2,0)=2. max(1,1)=1. min=1.
// a=0,b=1: 1.
// a=2,b=0: min+1=1. max(3,0)=3. max(2,1)=2. min=1.
// a=1,b=1: min+1=2. max(2,1)=2. max(1,2)=2. min=2.
// a=2,b=1: min+1=2. max(3,1)=3. max(2,2)=2. min=2.
// a=3,b=0: min+1=1. max(4
