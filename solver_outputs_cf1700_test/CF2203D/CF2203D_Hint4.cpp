// Hint4
#include <bits/stdc++.h>
using namespace std;

const int MAXV = 2000005;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n, m;
        cin >> n >> m;
        vector<int> a(n), b(m);
        int max_val = 0;
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
            max_val = max(max_val, a[i]);
        }
        for (int i = 0; i < m; ++i) {
            cin >> b[i];
            max_val = max(max_val, b[i]);
        }

        vector<bool> in_a(max_val + 1, false);
        for (int x : a) in_a[x] = true;

        vector<int> cnt_b(max_val + 1, 0);
        for (int y : b) cnt_b[y]++;

        // Determine for each value if it's Alice's or Bob's
        // Alice's: all divisors of y are in a
        // Bob's: no divisor of y is in a
        // We'll compute for each y: min divisor presence? Actually we need to know if there exists a divisor in a.
        // We can precompute smallest prime factor for fast divisor enumeration, but max_val up to 2e6, sum n+m 2e6.
        // Simpler: for each y, check divisors up to sqrt(y). But many y's. Better: use sieve-like approach.
        // Since max_val <= n+m <= 2e6, we can do O(max_val log max_val) per test? Sum of max_val over tests could be large.
        // But sum of n+m <= 2e6, so max_val sum over tests <= 2e6? Not necessarily, max_val can be up to n+m, but sum of n+m is 2e6, so sum of max_val over tests <= 2e6 as well? Actually max_val <= n+m for each test, so sum of max_val <= sum(n+m) <= 2e6. So O(max_val log max_val) total is fine.

        // We'll compute for each number d, whether d is present in a.
        // Then for each y, we want to know if there exists a divisor d of y with in_a[d] = true.
        // We can compute an array has_div_in_a[y] = true if any divisor of y is in a.
        vector<bool> has_div_in_a(max_val + 1, false);
        for (int d = 1; d <= max_val; ++d) {
            if (in_a[d]) {
                for (int multiple = d; multiple <= max_val; multiple += d) {
                    has_div_in_a[multiple] = true;
                }
            }
        }

        // Now classify numbers in b:
        // Alice's number: all divisors are in a => equivalently, for every divisor d of y, in_a[d] is true.
        // That means there is no divisor d of y with in_a[d] = false.
        // We can compute all_div_in_a[y] = true if for all divisors d of y, in_a[d] is true.
        // We can compute by checking: if y has any divisor not in a, then it's not Alice's.
        // We can compute an array missing_div[y] = true if there exists a divisor d of y with in_a[d] = false.
        // Actually we can compute all_div_in_a by: for each d not in a, mark its multiples as not all_div_in_a.
        vector<bool> all_div_in_a(max_val + 1, true);
        for (int d = 1; d <= max_val; ++d) {
            if (!in_a[d]) {
                for (int multiple = d; multiple <= max_val; multiple += d) {
                    all_div_in_a[multiple] = false;
                }
            }
        }

        // Now classify each y in b:
        // Alice's number: all_div_in_a[y] == true
        // Bob's number: has_div_in_a[y] == false
        // Other: neither (both players can potentially choose it? Actually if it's neither, then there is some divisor in a and some divisor not in a. Alice can't choose it because not all divisors are in a. Bob can't choose it because there is a divisor in a. So it's a "neutral" number that neither can pick.)
        // But wait: Alice's rule: she must choose x and y such that y is divisible by x. She can choose any x from a. So she can pick y if there exists at least one x in a that divides y. But the problem statement says: "Alice must choose x and y such that y is divisible by x." That means she needs to find some x in a that divides y. It does NOT require that ALL divisors are in a. The hint says: "For a number y to be Alice's number, the array a must consist only of its divisors." That seems contradictory. Let's re-read hint 2: "For a number y to be Alice's number, the array a must consist only of its divisors." That means if there is any element in a that does NOT divide y, then Alice cannot choose that y? No, Alice chooses x from a. She can just pick an x that divides y. The hint might mean: For Alice to be able to pick y, there must exist an x in a that divides y. But the hint says "the array a must consist only of its divisors" – that would mean every element of a must divide y. That is a much stronger condition. Let's think: If Alice picks x and y, she removes y. Bob then plays. The game is about removing elements from b. The players can choose any x from a (a remains unchanged). So Alice can pick y if there is at least one x in a that divides y. But why would the hint say "array a must consist only of its divisors"? Maybe it's about the strategy: Alice wants to force a win. Perhaps the optimal strategy involves considering numbers that only Alice can take, and numbers that only Bob can take. Let's analyze the game.

        // The game is impartial? No, players have different move rules.
        // Alice can move on y if there exists x in a with x|y.
        // Bob can move on y if there exists x in a with x∤y.
        // Note that a is fixed. So for each y in b, we can determine:
        // - Can Alice move on y? (∃ x in a: x|y)
        // - Can Bob move on y? (∃ x in a: x∤y)
        // Since a is non-empty, for any y, either there is some x that divides y, or all x do not divide y. But Bob needs an x that does NOT divide y. If there is at least one x in a that does not divide y, Bob can move on y. If all x in a divide y, then Bob cannot move on y.
        // So:
        // Type A (Alice-only): all x in a divide y. (Then Alice can move, Bob cannot)
        // Type B (Bob-only): no x in a divides y. (Then Bob can move, Alice cannot)
        // Type C (Both): there is at least one x that divides y, and at least one x that does not divide y. (Both can move)
        // Type D (Neither): impossible because if there is no x that divides y, then all x do not divide y, so Bob can move. If all x divide y, Alice can move. So every y is either A, B, or C.

        // Now the game: players take turns choosing a y from b and removing it, but they must follow their rule. They can choose any x from a to justify the move. Since a never changes, the types of remaining y's are fixed.
        // Alice can only pick y of type A or C.
        // Bob can only pick y of type B or C.
        // The game ends when a player cannot move. This is a normal play impartial game? Not exactly impartial because available moves depend on player. But we can think of it as a game where each y is a "piece" that can be taken by certain players. This is a partisan game. However, we can simplify: Since both players can take type C, those are "shared" moves. Type A only Alice can take, type B only Bob can take.

        // This is similar to a game where there are piles of tokens, but here tokens are of three types. Players alternate, each can remove one token of their allowed types. Who wins?
        // This is a classic "turn-based removal" game. Since players can always take from the shared pool (type C) as long as it's non-empty, the game might be determined by the counts.

        // Let's denote:
        // cntA = number of y of type A (only Alice can take)
        // cntB = number of y of type B (only Bob can take)
        // cntC = number of y of type C (both can take)

        // On Alice's turn, she can take A or C. On Bob's turn, he can take B or C.
        // If cntC > 0, both players have at least one move (unless they have no other moves and cntC is the only option). The game will proceed with players possibly taking from C until someone is forced to take from their exclusive pool or run out.

        // Let's analyze the outcome based on counts.
        // This is a known game: "Alice and Bob picking from shared and personal piles". The optimal strategy often depends on parity of cntC and the difference between cntA and cntB.

        // We can simulate the game logically:
        // If cntC == 0:
        //   Then Alice can only take A, Bob only B. They will just take their own until one runs out. Since Alice goes first, if cntA > cntB, Alice wins (Bob runs out first). If cntA <= cntB, Bob wins (Alice runs out first or they run out simultaneously? If cntA == cntB, Alice takes last A, then Bob has no move? Wait: Alice takes an A, then cntA becomes cntA-1. Bob takes B, etc. If cntA == cntB, after Alice's move, cntA = cntB-1? Let's trace: cntA = cntB = k. Alice takes A -> k-1 A left. Bob takes B -> k-1 B left. Eventually Alice takes last A, Bob has no B left? Actually if k=1: Alice takes A, cntA=0. Bob's turn, cntB=1, he takes B, cntB=0. Alice's turn, no A, no C, she loses. So Bob wins if cntA <= cntB.
        //   So if cntC == 0: Alice wins iff cntA > cntB.

        // If cntC > 0:
        //   The shared pile acts as a "buffer". The player who is forced to take from C when the other still has exclusive moves might gain advantage.
        //   Let's think: Both players prefer to take from C? Actually taking from C reduces the shared pool, which might be good or bad. Since both can take C, taking C doesn't change the difference in exclusive counts. The game will eventually reach a state where C is empty. The player who makes the last move on C will leave the next player to start the exclusive phase. So the parity of cntC determines who starts the exclusive phase.
        //   If cntC is odd: Alice takes first C, then Bob, ... Alice takes last C. Then it's Bob's turn with C empty. So Bob starts the exclusive phase.
        //   If cntC is even: Bob takes last C, then Alice starts exclusive phase.
        //   In the exclusive phase (C=0), the winner is determined by cntA and cntB as above: the player who starts the exclusive phase wins if their exclusive count is > opponent's? Wait, in exclusive phase, the starting player is the one whose turn it is. Let's say it's player P's turn, and only exclusive moves remain. P can take their own exclusive type. The game then proceeds with players alternating taking their exclusive types. The player who runs out first loses.
        //   If P starts exclusive phase, they will take one of their exclusive tokens. Then opponent takes theirs, etc. This is exactly the same as the cntC=0 case but with the starting player possibly different.
        //   Let's analyze exclusive phase with starting player S (S is Alice or Bob). Let cntA and cntB be the remaining exclusive counts.
        //   If S == Alice:
        //     Alice can take A. Then it's Bob's turn. This is exactly the game with cntC=0 where Alice goes first. So Alice wins iff cntA > cntB.
        //   If S == Bob:
        //     Bob goes first. Then Bob takes B, Alice takes A, etc. This is symmetric: Bob wins iff cntB > cntA. (If cntB == cntA, Bob takes last B? Let's test: cntA = cntB = k. Bob takes B -> k-1 B left. Alice takes A -> k-1 A left. Eventually Bob takes last B, Alice has no A? Actually if k=1: Bob takes B, cntB=0. Alice's turn, cntA=1, she takes A, cntA=0. Bob's turn, no B, loses. So Bob loses if cntB <= cntA when he starts? Wait: Bob starts, takes B, then Alice takes A. If cntB == cntA, after Bob's move, cntB = cntA - 1. Then Alice takes A, cntA becomes cntA-1, so cntB = cntA? Let's simulate: cntA=2, cntB=2. Bob takes B -> (2,1). Alice takes A -> (1,1). Bob takes B -> (1,0). Alice takes A -> (0,0). Bob's turn, no B, loses. So Bob loses when cntB <= cntA if he starts. So Bob wins iff cntB > cntA.
        //   So in exclusive phase, the starting player wins iff their exclusive count > opponent's exclusive count.

        // Now back to cntC > 0:
        //   If cntC is odd: Bob starts exclusive phase. Bob wins if cntB > cntA. Alice wins if cntB <= cntA.
        //   If cntC is even: Alice starts exclusive phase. Alice wins if cntA > cntB. Bob wins if cntA <= cntB.

        // But wait: during the shared phase, players might not always take from C. They could take from their exclusive pool even when C is non-empty. Would that ever be beneficial? In game theory, we need to check if players have a winning strategy by possibly deviating. This is a partisan game, but we can reduce it to a Nim-like analysis. Since all moves are independent (removing one token), the game is equivalent to a normal play game where each token is a move option. This is known as a "poset game" but here moves are just picking tokens. Actually, since players can pick any allowed token, the game is a "take-away" game with different move sets. The standard result for such games: it's equivalent to comparing the number of moves available to each player, but with the shared moves acting as moves for both. The game is determined by the parity of shared moves and the difference in exclusive moves. The above logic assumes players will always take from C if available? But is it always optimal to take from C? Let's think: If a player takes from their exclusive pool while C is non-empty, they reduce their own exclusive count without affecting the opponent's. That seems bad because exclusive moves are the only ones the opponent cannot take. So taking from C is generally better because it preserves your exclusive moves. However, there might be situations where you want to force the opponent to start the exclusive phase. But the starting player of exclusive phase is determined by who takes the last C. If you take from your exclusive pool instead of C, you effectively skip your turn on C, changing the parity? Actually, if you take from exclusive, C remains the same, and turn passes to opponent. The opponent can then take C. This might change who takes the last C. So players have choices. We need to analyze the game tree.

        // This is a classic game: "Game of divisors" or similar. Let's search memory: There is a known Codeforces problem "Alice and Bob" with arrays a and b, where Alice needs x|y, Bob needs x∤y. The solution often involves counting numbers that are "good" for Alice, "good" for Bob, and "both". The winner is determined by comparing counts. I recall a problem: "Alice and Bob are playing a game. They have an array a and b. Alice chooses x from a, y from b such that y%x==0. Bob chooses such that y%x!=0. Who wins?" The solution: Count how many numbers in b are divisible by all elements of a (Alice's numbers), how many are divisible by none (Bob's numbers), and the rest (both). Then if Alice's numbers > Bob's numbers, Alice wins. Else if Bob's numbers > Alice's numbers, Bob wins. Else if the number of "both" is odd, Alice wins, else Bob wins. Wait, let's test with sample.

        // Sample 1:
        // n=9, m=3
        // a = [3,2,4,2,2,4,4,2,4]
        // b = [6,7,12]
        // Let's classify:
        // For y=6: divisors in a? a has 2,3,4. 2|6, 3|6, 4 does not divide 6. So there is x that divides (2,3) and x that does not divide (4). So type C (both).
        // y=7: divisors in a? 2,3,4 none divide 7. So no x divides 7. Type B (Bob-only).
        // y=12: divisors in a? 2|12, 3|12, 4|12. All elements of a divide 12? Check: a has 2,3,4. All divide 12. So type A (Alice-only).
        // Counts: A=1, B=1, C=1.
        // According to my earlier logic: cntC=1 odd -> Bob starts exclusive phase. Bob wins if cntB > cntA. Here cntB=1, cntA=1, so cntB <= cntA -> Alice wins. Output: Alice. Matches sample.

        // Sample 2:
        // 10 3
        // a = [3,2,5,4,2,5,3,4,4,4]
        // b = [10,7,13]
        // a has 2,3,4,5.
        // y=10: divisors? 2|10, 5|10. 3∤10, 4∤10. So both (C).
        // y=7: no divisors (B).
        // y=13: no divisors (B).
        // Counts: A=0, B=2, C=1.
        // cntC=1 odd -> Bob starts exclusive. Bob wins if cntB > cntA -> 2 > 0 true -> Bob wins. Output: Bob. Matches sample.

        // Sample 3:
        // 1 5
        // a = [1]
        // b = [1,2,3,4,5]
        // a has 1. 1 divides everything.
        // For any y, 1|y, so Alice can always move. Bob can move if there is an x in a that does NOT divide y. Since a only has 1, and 1 divides all y, there is no x that does not divide y. So Bob cannot move on any y. Thus all y are type A (Alice-only). Counts: A=5, B=0, C=0.
        // cntC=0: Alice wins if cntA > cntB -> 5>0 true -> Alice wins. Output: Alice. Matches sample.

        // So the logic seems correct: 
        // Count A: y such that for all x in a, x|y. (Equivalently: every element of a divides y)
        // Count B: y such that for all x in a, x∤y. (No element of a divides y)
        // Count C: the rest.
        // Then:
        // if A > B: Alice wins.
        // else if B > A: Bob wins.
        // else: (A == B)
        //   if C % 2 == 1: Alice wins.
        //   else: Bob wins.

        // Let's double-check with a custom case: A=1, B=1, C=0. A==B, C even -> Bob wins. Let's simulate: Alice must take A (only move). Then Bob takes B. Alice no move, loses. Bob wins. Correct.
        // A=1, B=1, C=1. A==B, C odd -> Alice wins. Sample 1 matches.
        // A=2, B=2, C=0 -> Bob wins. Simulate: Alice takes A (1 left), Bob takes B (1 left), Alice takes A (0 left), Bob takes B (0 left), Alice loses. Bob wins.
        // A=2, B=2, C=1 -> C odd -> Alice wins. Simulate: Alice can take C. Then A=2,B=2,C=0, Bob's turn. Now it's exclusive phase with Bob starting. Bob needs B > A to win, but B=A, so Bob loses. So Alice wins. If Alice instead takes A first: A=1,B=2,C=1. Bob can take C (A=1,B=2,C=0) then Alice's turn exclusive, Alice needs A > B (1>2 false) so Alice loses. Or Bob could take B. So Alice should take C. So with optimal play, Alice wins. Correct.
        // What if A=0, B=0, C=2? A==B, C even -> Bob wins. Simulate: Alice takes C (C=1). Bob takes C (C=0). Alice no move, loses. Bob wins.
        // A=0, B=0, C=3: C odd -> Alice wins. Alice takes C (2), Bob takes C (1), Alice takes C (0), Bob loses.
        // So the rule holds.

        // Now we need to efficiently compute for each y in b:
        // - Is it type A? i.e., for all x in a, x divides y.
        // - Is it type B? i.e., for all x in a, x does NOT divide y.
        // Since a can have duplicates, we only care about distinct values. We can use a boolean array for presence.
        // Type A: y is a common multiple of all elements in a. That means y must be a multiple of the LCM of all elements in a? Actually, if for all x in a, x|y, then y is a multiple of each x. So y must be a multiple of the least common multiple of all distinct values in a. But LCM can be huge. However, we can check: y is type A iff for every x in a, x|y. Equivalently, there is no x in a such that x∤y. So we can check if there exists x in a that does not divide y. We can precompute for each possible value d, whether d is in a. Then for a given y, we need to know if there exists some d in a that does NOT divide y. This is equivalent to: is there some d in a such that y % d != 0? We can check this by iterating over divisors? But y can be up to 2e6. We can precompute an array `has_non_divisor[y]` = true if there exists some x in a that does not divide y. How to compute this efficiently?
        // We have max_val <= n+m <= 2e6. We can use a sieve-like approach.
        // Let in_a[d] be true if d is in a.
        // For type A, we want y such that for all d with in_a[d]=true, d|y. That means y is a multiple of every d in a. So y must be a multiple of the LCM. But LCM might exceed max_val. If LCM > max_val, then no y in b can be type A because b_i <= max_val. So we can compute LCM of distinct a's, but careful with overflow. Since max_val <= 2e6, we can compute LCM step by step, and if it exceeds max_val, we know there are no type A numbers. But wait: type A requires that for all x in a, x|y. If LCM > max_val, then no y <= max_val can be a multiple of all x. So cntA = 0. That's a good optimization.
        // However, we also need type B: no x in a divides y. That means y is not a multiple of any x in a. We can compute this by marking multiples of each x in a. Then y is type B if it is not marked.
        // Type C is the rest.

        // Let's refine:
        // 1. Read a, b. Get distinct values of a (we can use a boolean array or set; since values up to 2e6, boolean array is fine).
        // 2. Compute LCM of distinct a. If LCM > max_val, then cntA = 0. Else, we can count how many y in b are multiples of LCM. But wait: type A requires that ALL x in a divide y. If LCM <= max_val, then y is type A iff y % LCM == 0. Because if y is a multiple of LCM, it's a multiple of each x. Conversely, if every x divides y, then y is a common multiple, so LCM divides y. So type A is exactly the multiples of LCM. So we can just count how many y in b are multiples of LCM. This is O(m) if we have LCM.
        // 3. For type B: we need y such that no x in a divides y. We can create a boolean array `divisible_by_a` of size max_val+1, initially false. For each distinct x in a, mark all multiples of x as true. Then y is type B if !divisible_by_a[y].
        // 4. Type C: the rest.
        // This avoids the need for all_div_in_a and has_div_in_a arrays, and is very efficient.

        // Let's check LCM computation: LCM can be large. Since max_val <= 2e6, we can compute LCM using 64-bit integers. If at any point LCM > max_val, we can cap it or just set a flag. We'll do:
        // long long lcm = 1;
        // for each distinct x in a:
        //   lcm = lcm / gcd(lcm, x) * x;
        //   if (lcm > max_val) { lcm = max_val + 1; break; }
        // Then if lcm <= max_val, type A numbers are those y with y % lcm == 0.

        // For type B: we need to mark multiples. Since sum of n over test cases <= 1e6, and max_val sum <= 2e6, we can do for each distinct x in a: for (int m = x; m <= max_val; m += x) divisible_by_a[m] = true. The total operations across all distinct x is sum_{x} max_val/x. In worst case, if a has many small numbers, it could be O(max_val log max_val). Since sum of max_val over test cases <= 2e6, this is fine.

        // Let's implement.

        // Edge cases: a might have duplicates. We only need distinct values for LCM and marking. So we can use a boolean array `in_a` to get distinct values.

        // Steps per test case:
        // - read n, m
        // - read a, b
        // - find max_val = max(max(a), max(b))
        // - vector<bool> in_a(max_val+1, false)
        // - for x in a: in_a[x] = true
        // - collect distinct a's: vector<int> distinct_a; for i=1..max_val if in_a[i] distinct_a.push_back(i)
        // - compute LCM:
        //   long long lcm = 1;
        //   bool lcm_exceed = false;
        //   for x in distinct_a:
        //     lcm = lcm / __gcd(lcm, (long long)x) * x;
        //     if (lcm > max_val) { lcm_exceed = true; break; }
        // - vector<bool> divisible_by_a(max_val+1, false);
        //   for x in distinct_a:
        //     for (int m = x; m <= max_val; m += x) divisible_by_a[m] = true;
        // - count types in b:
        //   int cntA = 0, cntB = 0, cntC = 0;
        //   for y in b:
        //     bool isA = false;
        //     if (!lcm_exceed && y % lcm == 0) isA = true;
        //     bool isB = !divisible_by_a[y];
        //     if (isA) cntA++;
        //     else if (isB) cntB++;
        //     else cntC++;
        // - determine winner:
        //   if (cntA > cntB) cout << "Alice\n";
        //   else if (cntB > cntA) cout << "Bob\n";
        //   else {
        //     if (cntC % 2 == 1) cout << "Alice\n";
        //     else cout << "Bob\n";
        //   }

        // Let's test with samples.
        // Sample 1: a distinct: 2,3,4. LCM of 2,3,4 = 12. max_val=12. lcm=12 <=12. divisible_by_a: multiples of 2,3,4. b=6,7,12. y=6: 6%12!=0 -> not A. divisible_by_a[6]? 6 is multiple of 2 and 3, so true -> not B. So C. y=7: not A, divisible_by_a[7] false -> B. y=12: 12%12==0 -> A. Counts: A=1,B=1,C=1. A==B, C odd -> Alice. OK.

        // Sample 2: a distinct: 2,3,4,5. LCM of 2,3,4,5 = 60. max_val=13. lcm=60 > 13 -> lcm_exceed=true. So cntA=0. divisible_by_a: multiples of 2,3,4,5 up to 13. b=10,7,13. y=10: divisible by 2,5 -> not B -> C. y=7: not divisible by any -> B. y=13: not divisible -> B. Counts: A=0,B=2,C=1. B>A -> Bob. OK.

        // Sample 3: a distinct: 1. LCM=1 <=5. divisible_by_a: multiples of 1 -> all numbers 1..5 are true. b=1,2,3,4,5. All y%1==0 -> A. Counts: A=5,B=0,C=0. A>B -> Alice. OK.

        // Consider a case where a has elements that are not divisors of some y, but LCM still divides y? That's impossible because if LCM divides y, then every x divides y. So type A condition is exactly y % LCM == 0. So correct.

        // What if a is empty? Constraints say n>=1, so a non-empty.

        // Complexity: O(max_val log max_val) per test case due to marking multiples. Sum of max_val <= 2e6, so total operations around 2e6 * log(2e6) ~ 2e6 * 14 ~ 2.8e7, well within 1 second in C++.

        // Memory: vectors of bool up to max_val+1. Since sum of max_val <= 2e6, we can allocate per test case. Using vector<bool> is space-efficient (1 bit per element). Alternatively, vector<char> for speed. We'll use vector<char> to avoid bit-reference issues and for faster access. But vector<bool> is fine. We'll use vector<char> for divisible_by_a and in_a to be safe.

        // Implementation details:
        // - Use fast I/O.
        // - Use __gcd or std::gcd (C++17 has std::gcd in <numeric>).
        // - Be careful with LCM overflow: use long long.

        // Let's write the code.

        // One more check: The game logic we derived matches the known solution for Codeforces problem "Alice and Bob" (maybe problem 1593D2? No, it's a different one). Actually this is Codeforces Round #770 (Div. 2) problem C? Or maybe problem "Alice and Bob" from some contest. The logic is standard.

        // Let's double-check the winning condition with a few more cases.
        // Case: A=2, B=1, C=0. A>B -> Alice wins. Simulate: Alice takes A (1 left). Bob takes B (0 left). Alice takes A (0 left). Bob no move, loses. Alice wins.
        // Case: A=1, B=2, C=0. B>A -> Bob wins. Alice takes A (0 left). Bob takes B (1 left). Alice no move, loses. Bob wins.
        // Case: A=1, B=1, C=2. A==B, C even -> Bob wins. Simulate: Alice can take C (C=1). Bob can take C (C=0). Alice must take A (0 left). Bob takes B (0 left). Alice loses. Or Alice takes A first: A=0,B=1,C=2. Bob can take C (C=1). Alice takes C (C=0). Bob takes B (0 left). Alice loses. So Bob wins.
        // Case: A=1, B=1, C=3. A==B, C odd -> Alice wins. Alice takes C (2). Bob takes C (1). Alice takes C (0). Bob must take B (0 left). Alice takes A (0 left). Bob loses. Or if Bob deviates? Bob could take B earlier? Let's see: Alice takes C (2). Bob could take B (A=1,B=0,C=2). Then Alice takes C (1). Bob takes C (0). Alice takes A (0). Bob loses. So Alice wins.
        // Case: A=2, B=2, C=2. A==B, C even -> Bob wins. Alice takes C (1). Bob takes C (0). Now A=2,B=2,C=0, Alice's turn. Alice takes A (1). Bob takes B (1). Alice takes A (0). Bob takes B (0). Alice loses. If Alice takes A first: A=1,B=2,C=2. Bob can take C (1). Alice takes C (0). Bob takes B (1). Alice takes A (0). Bob takes B (0). Alice loses. So Bob wins.
        // Seems consistent.

        // Therefore, the solution is correct.

        // Let's implement.
    }
    return 0;
}
