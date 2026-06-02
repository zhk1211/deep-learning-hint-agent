#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n, m;
        cin >> n >> m;
        vector<int> a(n), b(m);
        for (int i = 0; i < n; ++i) cin >> a[i];
        for (int i = 0; i < m; ++i) cin >> b[i];

        // Count frequencies of each value in a and b
        // Values are up to n+m, but sum of n+m over test cases <= 2e6
        int max_val = n + m;
        vector<int> cnt_a(max_val + 1, 0), cnt_b(max_val + 1, 0);
        for (int x : a) cnt_a[x]++;
        for (int y : b) cnt_b[y]++;

        // For each possible x, compute the number of y in b that are divisible by x
        // We'll use a sieve-like approach
        vector<int> div_count(max_val + 1, 0);
        for (int x = 1; x <= max_val; ++x) {
            if (cnt_a[x] == 0) continue;
            for (int mult = x; mult <= max_val; mult += x) {
                div_count[x] += cnt_b[mult];
            }
        }

        // Determine winner
        // If there exists an x such that all y divisible by x are already removed? No.
        // The game is equivalent to: Alice can only pick pairs (x,y) with x|y.
        // Bob can only pick pairs (x,y) with x not dividing y.
        // This is a bipartite game on a directed graph? Actually it's impartial?
        // Let's analyze: The game ends when no moves are possible.
        // A move consists of picking x from a and y from b satisfying the condition, and removing y.
        // x remains. So the set of available x never changes.
        // This is a game of "removing tokens from b" with constraints based on a.
        // It's known that such games can be solved by looking at the maximum matching?
        // But here the graph is bipartite between a and b, edges for Alice are (x,y) with x|y,
        // edges for Bob are (x,y) with x not dividing y.
        // However, players have different allowed moves. This is a partisan game.
        // Let's think: If there is any y that is not divisible by any x, Bob can always pick it?
        // Actually Bob needs some x that does not divide y. If for some y, all x divide y, then Bob cannot pick that y.
        // Similarly, Alice needs some x that divides y.

        // Key observation: The game is determined by the parity of the number of moves?
        // Let's consider the hint: what if b consists of one number?
        // If b has one number y. Alice can move iff there exists x in a such that x|y.
        // If she can move, she removes y and Bob has no moves -> Alice wins.
        // If she cannot move, she loses immediately -> Bob wins.
        // So with one number, Alice wins iff there is x|y.

        // General case: This is a game of "removing elements from b" with alternating turns.
        // Each turn, a player must pick a valid (x,y) and remove y.
        // Notice that the choice of x does not affect future availability of x.
        // So the game is essentially: we have a set of b elements. On Alice's turn, she can remove any y that has at least one divisor in a.
        // On Bob's turn, he can remove any y that has at least one non-divisor in a.
        // But wait: Bob's condition is "y is not divisible by x". That means there exists x in a such that x does NOT divide y.
        // So Bob can remove y if there is at least one x in a that does not divide y.
        // In other words, Bob can remove y unless every x in a divides y.
        // Alice can remove y if there is at least one x in a that divides y.

        // So the sets of removable y for each player are:
        // Alice_removable = { y in b : exists x in a with x|y }
        // Bob_removable = { y in b : exists x in a with x not dividing y }

        // Notice that Bob_removable is the complement of { y in b : for all x in a, x|y }.
        // Let S = { y in b : for all x in a, x|y }. Then Bob can remove any y not in S.
        // Alice can remove any y that has at least one divisor in a.

        // The game is impartial? No, because available moves depend on whose turn it is.
        // This is a partisan game. We can try to model it as a game on a DAG? But the state is just the multiset of remaining b.
        // Since m up to 1e6, we need a combinatorial characterization.

        // Let's analyze the game tree for small m.
        // Suppose we classify each y in b into three categories:
        // Type A: y is divisible by some x in a, AND there exists some x in a that does NOT divide y.
        //         (i.e., both players can remove y)
        // Type B: y is divisible by ALL x in a. (Then only Alice can remove y, Bob cannot)
        // Type C: y is NOT divisible by ANY x in a. (Then only Bob can remove y, Alice cannot)
        // Note: Type B means for all x in a, x|y. Type C means for all x in a, x does not divide y.
        // Are there other types? If y is divisible by some but not all, that's Type A.

        // Now, the game: players alternate removing one element from b, subject to their allowed types.
        // Alice can remove Type A or Type B.
        // Bob can remove Type A or Type C.

        // This is a game of removing tokens from three piles with different allowed moves.
        // Let countA, countB, countC be the number of elements of each type.

        // Let's think: Type B elements are "Alice-only", Type C are "Bob-only", Type A are "shared".

        // This resembles a game where each player wants to force the other to run out of moves.
        // Since players can always choose from shared pile if available, the game might reduce to parity of total moves?
        // But players can choose which pile to take from.

        // Consider that if a player takes from the shared pile (Type A), they reduce the total number of moves for both.
        // If a player takes from their exclusive pile (B for Alice, C for Bob), they only reduce their own future moves, but also bring the game closer to end.

        // Let's simulate small values to find pattern.
        // We can write a minimax DP for small counts, but we need a formula.

        // Observation: The game is symmetric if we swap roles? Not exactly.

        // Let's analyze the game as a normal-play partisan game. We can compute Grundy? No, partisan.

        // Alternative view: The game ends when no moves for the current player.
        // Suppose it's Alice's turn. She can move if countA + countB > 0.
        // Bob can move if countA + countC > 0.

        // Let's denote state by (A, B, C) with turn.
        // Terminal states: Alice to move and A+B=0 -> Alice loses. Bob to move and A+C=0 -> Bob loses.

        // We can try to solve this combinatorial game by looking at "who can force the last move".

        // Notice that Type A elements are "neutral" in the sense that both can take them.
        // Type B and C are "colored" for each player.

        // Consider the difference between B and C. If B > C, Alice has more exclusive moves than Bob.
        // She might want to force Bob to run out of shared moves while she still has exclusive moves.

        // Let's think about optimal play:
        // If it's your turn and there is a shared move (Type A), should you take it or take your exclusive move?
        // Taking shared reduces both players' options equally. Taking exclusive reduces only your own options.
        // Usually, you want to take shared when you are ahead in exclusive moves, to force opponent to run out.
        // This is similar to the game of "taking from common pile vs personal pile".

        // Let's try to derive the winner based on counts.

        // Consider the total number of moves available to Alice: A+B.
        // Total moves available to Bob: A+C.
        // The game will end when one player cannot move. The maximum possible moves is A + min(B,C)? Not exactly.

        // Let's simulate small values with a simple DP to guess pattern.
        // We'll write a quick mental DP for small A,B,C.

        // State: (A,B,C, turn) where turn=0 for Alice, 1 for Bob.
        // Alice wins if she can force a win.

        // Base: if turn=0 and A+B==0 -> Bob wins.
        // if turn=1 and A+C==0 -> Alice wins.

        // Transitions:
        // Alice's turn: can move to (A-1,B,C,1) if A>0, or (A,B-1,C,1) if B>0.
        // Bob's turn: can move to (A-1,B,C,0) if A>0, or (A,B,C-1,0) if C>0.

        // Let's compute for small A,B,C manually or deduce pattern.

        // Notice that the game is symmetric if we swap B and C and swap turns? Not exactly because Alice goes first.

        // Let's try to see if the winner is determined by comparing B and C.
        // Suppose A=0. Then Alice can only take B, Bob can only take C.
        // Game: Alice takes B, then Bob takes C, etc. The one with more exclusive moves wins if they go first?
        // Actually, if A=0, Alice moves: if B>0, she takes one B, then Bob must take C. If B=0, Alice loses immediately.
        // So with A=0, Alice wins iff B > C? Let's check:
        // B=1, C=0: Alice takes B, Bob has no moves -> Alice wins.
        // B=1, C=1: Alice takes B, Bob takes C, Alice has no moves -> Bob wins.
        // B=2, C=1: Alice takes B (B=1,C=1), Bob takes C (B=1,C=0), Alice takes B -> Bob loses -> Alice wins.
        // So with A=0, Alice wins iff B > C. (If B == C, Bob wins because Alice goes first? B=1,C=1 -> Bob wins. B=0,C=0 -> Alice loses immediately -> Bob wins. So B > C is Alice win, else Bob.)

        // Now suppose A > 0. Shared moves can be used by both.
        // Consider the effect of a shared move: it reduces both players' total moves by 1.
        // If a player takes a shared move, it's like passing the turn while reducing the "shared resource".

        // Let's think about the game as: each player wants to be the one who makes the last move.
        // The total number of moves in the game is at most A + min(B,C)? Not exactly, because players might not be forced to take shared.

        // Actually, consider optimal play: If B > C, Alice has more exclusive moves. She wants to avoid taking shared moves as long as possible, because shared moves also give Bob moves. Bob, on the other hand, wants to take shared moves to deplete Alice's advantage? Wait, if Bob takes a shared move, he also reduces his own future moves. But if he is behind in exclusive moves, he might want to take shared moves to force Alice to use her exclusive moves? Let's analyze.

        // Suppose B > C. Alice is ahead. Bob wants to prolong the game? No, Bob wants to win. Bob wins if Alice cannot move. To make Alice unable to move, Bob needs to eliminate all A and B? Actually Alice cannot move when A+B=0. So Bob wants to reduce A+B to 0 on Alice's turn. Bob can reduce A (by taking shared) or reduce C (his own exclusive, which doesn't affect Alice's moves). Bob taking C doesn't help reduce Alice's moves. Bob taking A reduces both A and B? No, taking A reduces A by 1, leaving B unchanged. So Bob taking A reduces Alice's total moves by 1. Bob taking C reduces only his own moves. So Bob should prefer taking A to reduce Alice's options. Similarly, Alice wants to reduce Bob's options, so she should prefer taking A to reduce Bob's moves, rather than taking B which only reduces her own.

        // So both players prefer taking from shared pile A as long as it's available? But wait, if Alice takes A, she reduces Bob's moves, but also reduces her own future moves. If she is ahead (B > C), she might want to take A to force Bob to run out faster? Let's test with small numbers.

        // Let's write a small script in mind:
        // State (A,B,C) with Alice to move.
        // We want to know if Alice wins.

        // Let's compute for A=1, B=1, C=0.
        // Alice options: take A -> (0,1,0) Bob's turn. Bob has A+C=0 -> Bob loses -> Alice wins.
        // Alice could take B -> (1,0,0) Bob's turn. Bob can take A -> (0,0,0) Alice's turn, A+B=0 -> Alice loses. So taking B is losing.
        // So Alice wins by taking A. So (1,1,0) Alice wins.

        // A=1, B=0, C=1. Alice to move: A+B=1, she can take A -> (0,0,1) Bob's turn. Bob has A+C=1, takes C -> (0,0,0) Alice loses. So Alice loses if she takes A. She has no B. So (1,0,1) Alice loses -> Bob wins.

        // A=1, B=1, C=1. Alice options:
        // Take A -> (0,1,1) Bob's turn. Bob can take C (only, since A=0). -> (0,1,0) Alice's turn. Alice takes B -> (0,0,0) Bob loses -> Alice wins. So taking A leads to win.
        // Take B -> (1,0,1) Bob's turn. Bob can take A -> (0,0,1) Alice loses, or take C -> (1,0,0) Alice's turn, Alice takes A -> (0,0,0) Bob loses. Bob will choose winning move for him: if he takes A, Alice loses, so Bob takes A. So taking B leads to loss.
        // So Alice wins by taking A. So (1,1,1) Alice wins.

        // A=1, B=2, C=1. Alice options:
        // Take A -> (0,2,1) Bob's turn. Bob has C=1, takes C -> (0,2,0) Alice takes B -> (0,1,0) Bob loses? Wait, after Alice takes B, state (0,1,0) Bob's turn, A+C=0 -> Bob loses. So Alice wins. So taking A wins.
        // Take B -> (1,1,1) Bob's turn. We know from above (1,1,1) with Bob to move? Let's compute (1,1,1) with Bob to move. Bob options: take A -> (0,1,1) Alice's turn. Alice can take B -> (0,0,1) Bob's turn, Bob takes C -> (0,0,0) Alice loses? Wait, after Alice takes B, state (0,0,1) Bob's turn, Bob has C=1, takes C -> (0,0,0) Alice's turn, A+B=0 -> Alice loses. So Bob wins if he takes A. Bob could take C -> (1,1,0) Alice's turn. Alice can take A -> (0,1,0) Bob loses, or take B -> (1,0,0) Bob takes A -> (0,0,0) Alice loses. Alice will choose A and win. So Bob will not take C. So Bob takes A and wins. So (1,1,1) with Bob to move is a win for Bob. Thus, if Alice takes B, she goes to (1,1,1) Bob's turn, which is losing for Alice. So Alice should take A and win. So (1,2,1) Alice wins.

        // A=2, B=0, C=0. Alice takes A -> (1,0,0) Bob's turn. Bob takes A -> (0,0,0) Alice loses. So Alice loses? Wait, Alice has A+B=2, she must take A. After she takes A, Bob takes A, then Alice has no moves. So (2,0,0) Alice loses -> Bob wins.

        // A=2, B=1, C=0. Alice options:
        // Take A -> (1,1,0) Bob's turn. Bob has A+C=1, takes A -> (0,1,0) Alice takes B -> Bob loses. So Bob loses -> Alice wins.
        // Take B -> (2,0,0) Bob's turn. Bob takes A -> (1,0,0) Alice takes A -> (0,0,0) Bob loses? Wait, after Alice takes A, state (0,0,0) Bob's turn, Bob has no moves -> Bob loses. So Alice wins? Let's trace: (2,0,0) Bob's turn. Bob takes A -> (1,0,0) Alice's turn. Alice takes A -> (0,0,0) Bob's turn -> Bob loses. So taking B also wins. So (2,1,0) Alice wins.

        // A=2, B=0, C=1. Alice options:
        // Take A -> (1,0,1) Bob's turn. Bob can take A -> (0,0,1) Alice loses, or take C -> (1,0,0) Alice takes A -> (0,0,0) Bob loses. Bob will take A and win. So taking A loses.
        // Alice has no B. So (2,0,1) Alice loses -> Bob wins.

        // A=2, B=1, C=1. Alice options:
        // Take A -> (1,1,1) Bob's turn. We determined (1,1,1) with Bob to move is Bob win. So taking A loses.
        // Take B -> (2,0,1) Bob's turn. We just saw (2,0,1) with Bob to move? Let's compute (2,0,1) Bob's turn. Bob options: take A -> (1,0,1) Alice's turn. Alice has A+B=1, takes A -> (0,0,1) Bob takes C -> (0,0,0) Alice loses. So Bob wins by taking A. Bob could take C -> (2,0,0) Alice's turn. Alice takes A -> (1,0,0) Bob takes A -> (0,0,0) Alice loses. So Bob wins either way. So (2,0,1) with Bob to move is Bob win. Thus taking B leads to Bob win.
        // So both moves lose? Then (2,1,1) Alice loses -> Bob wins.

        // Let's check A=2, B=2, C=1.
        // Alice options:
        // Take A -> (1,2,1) Bob's turn. We need (1,2,1) Bob's turn. Let's compute: Bob options: take A -> (0,2,1) Alice's turn. Alice has A+B=2, can take B -> (0,1,1) Bob takes C -> (0,1,0) Alice takes B -> Bob loses. So Alice wins. Bob could take C -> (1,2,0) Alice's turn. Alice can take A -> (0,2,0) Bob loses, or take B -> (1,1,0) Bob takes A -> (0,1,0) Alice takes B -> Bob loses. So Alice wins. So (1,2,1) with Bob to move is Alice win. So taking A leads to Alice win.
        // So (2,2,1) Alice wins.

        // Let's try to find pattern.
        // It seems the winner depends on the difference B - C and the parity of A?
        // Let's tabulate some results for small A,B,C with Alice to move:
        // We'll denote W for Alice win, L for Bob win.

        // A=0:
        // B>C -> W, B<=C -> L.

        // A=1:
        // (1,0,0): A+B=1>0, Alice takes A -> (0,0,0) Bob loses -> W? Wait, earlier we said (1,0,0) Alice takes A, then Bob has no moves -> Alice wins. So (1,0,0) W.
        // (1,1,0): W
        // (1,0,1): L
        // (1,1,1): W
        // (1,2,0): W (B>C)
        // (1,0,2): L (B<C)
        // (1,2,1): W
        // (1,1,2): ? Let's compute (1,1,2): Alice options: take A -> (0,1,2) Bob's turn. Bob has C=2, takes C -> (0,1,1) Alice takes B -> (0,0,1) Bob takes C -> (0,0,0) Alice loses. So taking A loses. Take B -> (1,0,2) Bob's turn. Bob can take A -> (0,0,2) Alice loses, or take C -> (1,0,1) Alice takes A -> (0,0,1) Bob takes C -> Alice loses. So Bob wins. So (1,1,2) L.
        // So for A=1: W if B > C? (1,0,0) B=0,C=0 -> B<=C but W! So not simply B>C.
        // (1,0,0) is W, (1,1,0) W, (1,2,0) W. (1,0,1) L, (1,1,1) W, (1,2,1) W. (1,0,2) L, (1,1,2) L, (1,2,2)? Let's check (1,2,2): Alice options: take A -> (0,2,2) Bob's turn. Bob takes C -> (0,2,1) Alice takes B -> (0,1,1) Bob takes C -> (0,1,0) Alice takes B -> Bob loses. So Alice wins? Wait, after Alice takes B, state (0,1,1) Bob's turn, Bob takes C -> (0,1,0) Alice's turn, Alice takes B -> (0,0,0) Bob's turn -> Bob loses. So Alice wins. So (1,2,2) W.
        // So for A=1: W except when B=0 and C>=1? (1,0,1) L, (1,0,2) L. Also (1,1,2) L. So pattern: if B == 0 and C > 0, L? But (1,0,0) has C=0, W. (1,1,2) has B=1, C=2, L. So maybe if B <= C and something about A?

        // Let's think differently. The game is equivalent to: each player on their turn must remove an element from b that they are allowed to. The game ends when a player has no allowed moves. This is a normal-play partisan game. We can use the theory of "poset games" or "graph games"? But the moves don't depend on which specific element is removed, only on the counts of types.

        // Notice that Type A elements are "wildcards" that both can use. Type B and C are "personal" moves.
        // This is exactly the game of "Nim with a shared heap"? Not exactly.

        // Consider the following: Alice wants to make the last move. The total number of moves Alice can make is A+B. Bob can make A+C. The game ends when one player cannot move. The maximum number of moves in the game is at most A + min(B,C) + something? Actually, if both players always take from A when possible, the game will last A + min(B,C) moves? Let's see: if both take A, A decreases. When A=0, they take from their own piles. The one with more personal moves will win if they go first when A=0? But the turn order matters.

        // Suppose both players play greedily: always take from A if available. Then the game reduces to A=0 with the turn being the player whose turn it is after A moves. Since Alice starts, after k moves, turn alternates. If A is even, after A moves it's Alice's turn again with A=0. If A is odd, it's Bob's turn with A=0.
        // Then with A=0, Alice wins iff B > C (if it's her turn) or B >= C? Let's check: A=0, Alice's turn: she wins if B > C. If B == C, she loses because she takes one, then Bob takes one, etc., Bob gets last move. If B < C, she loses.
        // If A=0 and Bob's turn: Bob wins if C > B? Actually, Bob's turn with A=0: Bob can take C. Alice can take B. Bob wins if C > B? Let's test: B=1, C=1, Bob's turn: Bob takes C -> B=1,C=0 Alice's turn, Alice takes B -> Bob loses. So Bob loses if B == C. Bob wins if C > B. If C < B, Bob loses? B=2,C=1, Bob's turn: Bob takes C -> B=2,C=0 Alice takes B -> B=1,C=0 Bob loses. So Bob wins iff C > B. So with A=0, the player to move wins iff their personal pile is strictly greater than the opponent's.

        // So if both play "take A first", the winner is determined by:
        // If A is even: Alice's turn with A=0 -> Alice wins iff B > C.
        // If A is odd: Bob's turn with A=0 -> Bob wins iff C > B, i.e., Alice wins iff C <= B? Actually Bob wins if C > B, so Alice wins if C <= B. But careful: if C == B, Bob loses, so Alice wins. So Alice wins if B >= C when A is odd.
        // So under this strategy, Alice wins if (A even and B > C) or (A odd and B >= C).

        // But is this strategy optimal? Players might deviate. For example, if taking A is bad for them, they might take from their personal pile instead.

        // Let's test this formula against our small computed cases:
        // A=0: even. Formula: B > C -> W. Matches.
        // A=1: odd. Formula: B >= C -> W.
        // Cases: (1,0,0): B=0,C=0 -> B>=C -> W. Matches.
        // (1,1,0): 1>=0 -> W. Matches.
        // (1,0,1): 0>=1 false -> L. Matches.
        // (1,1,1): 1>=1 -> W. Matches.
        // (1,2,1): 2>=1 -> W. Matches.
        // (1,1,2): 1>=2 false -> L. Matches.
        // (1,2,2): 2>=2 -> W. Matches.
        // A=2: even. Formula: B > C -> W.
        // (2,0,0): 0>0 false -> L. Matches.
        // (2,1,0): 1>0 -> W. Matches.
        // (2,0,1): 0>1 false -> L. Matches.
        // (2,1,1): 1>1 false -> L. Matches.
        // (2,2,1): 2>1 -> W. Matches.
        // (2,1,2): 1>2 false -> L? Let's test (2,1,2) manually to confirm.
        // A=2, B=1, C=2. Alice options:
        // Take A -> (1,1,2) Bob's turn. We saw (1,1,2) with Bob to move? We computed (1,1,2) with Alice to move was L. What about Bob to move? Let's compute (1,1,2) Bob's turn. Bob options: take A -> (0,1,2) Alice's turn. Alice has B=1, takes B -> (0,0,2) Bob takes C -> (0,0,1) Alice loses? Wait, after Alice takes B, state (0,0,2) Bob's turn, Bob takes C -> (0,0,1) Alice's turn, A+B=0 -> Alice loses. So Bob wins. Bob could take C -> (1,1,1) Alice's turn. We know (1,1,1) Alice to move is W for Alice. So Bob will not take C. So Bob takes A and wins. So (1,1,2) with Bob to move is Bob win. So taking A leads to loss for Alice.
        // Take B -> (2,0,2) Bob's turn. Bob options: take A -> (1,0,2) Alice's turn. (1,0,2) Alice to move: we saw L. So Bob wins. Bob could take C -> (2,0,1) Alice's turn. (2,0,1) Alice to move: we saw L. So Bob wins. So taking B loses.
        // So (2,1,2) L. Matches formula B>C? 1>2 false -> L.
        // So the formula holds for all tested cases!

        // Let's test another: A=3, B=1, C=1. A odd -> B>=C? 1>=1 -> W.
        // Compute manually: (3,1,1) Alice to move.
        // Options: take A -> (2,1,1) Bob's turn. (2,1,1) with Bob to move? We need to know. Let's compute (2,1,1) Bob's turn. Bob options: take A -> (1,1,1) Alice's turn -> W for Alice. Bob take C -> (2,1,0) Alice's turn. (2,1,0) Alice to move: we saw W. So Bob loses either way. So (2,1,1) Bob to move is Alice win. So taking A leads to win.
        // So (3,1,1) W. Matches.

        // What about A=3, B=0, C=1? A odd -> B>=C? 0>=1 false -> L.
        // (3,0,1) Alice to move: take A -> (2,0,1) Bob's turn. (2,0,1) Bob to move: we saw Bob wins. So L. Matches.

        // So the formula seems correct: Alice wins iff (A % 2 == 0 and B > C) or (A % 2 == 1 and B >= C).

        // But wait, is this always optimal? Could a player deviate by taking personal pile when A>0 to change the parity of A for the opponent? Let's analyze: Suppose it's Alice's turn, A>0. If she takes B instead of A, she reduces B by 1, leaving A unchanged. That effectively changes the state to (A, B-1, C) with Bob's turn. This might be beneficial if the formula says she loses with (A,B,C) but wins with (A,B-1,C) Bob's turn? But we need to check if such a deviation can change a losing position into a winning one.

        // Let's test a case where formula says L, but maybe Alice can win by taking B.
        // Consider A=2, B=1, C=1. Formula: A even, B>C? 1>1 false -> L. We computed L. What if Alice takes B? State becomes (2,0,1) Bob's turn. We computed (2,0,1) Bob's turn is Bob win. So still L.
        // What about A=2, B=2, C=2? Formula: even, 2>2 false -> L. Let's test if Alice can win by taking B.
        // (2,2,2) Alice options:
        // Take A -> (1,2,2) Bob's turn. We need (1,2,2) Bob's turn. Let's compute: Bob options: take A -> (0,2,2) Alice's turn. Alice has B=2, takes B -> (0,1,2) Bob takes C -> (0,1,1) Alice takes B -> (0,0,1) Bob takes C -> Alice loses? Wait, after Alice takes B, state (0,1,1) Bob's turn, Bob takes C -> (0,1,0) Alice's turn, Alice takes B -> (0,0,0) Bob's turn -> Bob loses. So Alice wins. So Bob taking A leads to Alice win. Bob could take C -> (1,2,1) Alice's turn. (1,2,1) Alice to move: we saw W. So Bob loses either way. So (1,2,2) Bob to move is Alice win. So taking A leads to win for Alice? But formula said L. Contradiction!
        // Let's re-evaluate (2,2,2) carefully.
        // State: A=2, B=2, C=2. Alice to move.
        // Option 1: take A -> (1,2,2) Bob's turn.
        // We need to analyze (1,2,2) with Bob to move.
        // Bob's options:
        //   - take A: (0,2,2) Alice's turn.
        //       Alice's turn with A=0, B=2, C=2. Alice can take B -> (0,1,2) Bob's turn.
        //           Bob's turn: A=0, B=1, C=2. Bob can take C -> (0,1,1) Alice's turn.
        //               Alice takes B -> (0,0,1) Bob's turn. Bob takes C -> (0,0,0) Alice's turn -> Alice loses.
        //           So Bob wins? Let's trace: (0,1,2) Bob's turn. Bob takes C -> (0,1,1) Alice's turn. Alice takes B -> (0,0,1) Bob's turn. Bob takes C -> (0,0,0) Alice's turn. Alice has no moves -> Alice loses. So Bob wins from (0,1,2).
        //       So if Alice takes B from (0,2,2), she goes to (0,1,2) Bob's turn, which is winning for Bob. So Alice should not take B? What else can Alice do from (0,2,2)? She has only B moves. She must take B. So (0,2,2) is losing for Alice. Thus Bob taking A from (1,2,2) leads to Bob win.
        //   - take C: (1,2,1) Alice's turn.
        //       (1,2,1) Alice to move: we computed W for Alice. So Bob loses.
        // So Bob will choose take A and win. Therefore (1,2,2) with Bob to move is Bob win.
        // So taking A from (2,2,2) leads to Bob win -> losing for Alice.

        // Option 2: take B -> (2,1,2) Bob's turn.
        // We already computed (2,1,2) Bob's turn? We computed (2,1,2) Alice to move was L. What about Bob to move?
        // Let's compute (2,
