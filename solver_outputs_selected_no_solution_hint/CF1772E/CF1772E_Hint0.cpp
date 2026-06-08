#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<int> p(n);
        for (int i = 0; i < n; ++i) {
            cin >> p[i];
        }

        // Check if already sorted ascending or descending (should not happen per statement)
        // We need to find the longest prefix that is already in correct position for ascending
        // and the longest suffix for descending, etc.

        // Let's compute:
        // a = length of longest prefix where p[i] == i+1
        // b = length of longest suffix where p[i] == i+1
        // c = length of longest prefix where p[i] == n - i
        // d = length of longest suffix where p[i] == n - i

        int a = 0, b = 0, c = 0, d = 0;

        // ascending prefix
        for (int i = 0; i < n; ++i) {
            if (p[i] == i + 1) a++;
            else break;
        }
        // ascending suffix
        for (int i = n - 1; i >= 0; --i) {
            if (p[i] == i + 1) b++;
            else break;
        }
        // descending prefix
        for (int i = 0; i < n; ++i) {
            if (p[i] == n - i) c++;
            else break;
        }
        // descending suffix
        for (int i = n - 1; i >= 0; --i) {
            if (p[i] == n - i) d++;
            else break;
        }

        // Now we need to determine the winner.
        // The game is symmetric: First wants ascending, Second wants descending.
        // The key is to see how many elements are already in the correct position for each player,
        // and how many are "free" (not in correct position for either).
        // Actually, we need to consider the "distance" to win for each player if they can freely rearrange
        // the elements that are not fixed by the opponent.

        // Let's define:
        // For First: he needs to fix elements that are not in ascending order.
        // He can only rearrange blue elements. Initially all red.
        // He can turn red to blue. Second can also turn red to blue.
        // The players can also rearrange blue elements arbitrarily.
        // This is a known game: "Permutation Game" from Codeforces Round #...
        // The solution: count how many elements are already in correct position for First (ascending)
        // and for Second (descending). Let cnt1 = number of i with p[i] == i+1.
        // cnt2 = number of i with p[i] == n - i.
        // But also we need to consider elements that are in correct position for both? That's only possible
        // if i+1 == n-i => 2i+1 = n => n odd and i = (n-1)/2, and p[i] == i+1 == n-i.
        // So there can be at most one such element.

        int cnt1 = 0, cnt2 = 0, both = 0;
        for (int i = 0; i < n; ++i) {
            if (p[i] == i + 1) cnt1++;
            if (p[i] == n - i) cnt2++;
            if (p[i] == i + 1 && p[i] == n - i) both++;
        }

        // The number of elements that are "free" (not in correct position for either) is n - cnt1 - cnt2 + both.
        // But the game is more subtle: players can "steal" elements that the opponent needs by coloring them blue.
        // The known solution: 
        // Let x = number of elements that are in correct position for First but NOT for Second.
        // Let y = number of elements that are in correct position for Second but NOT for First.
        // Let z = number of elements that are in correct position for both (0 or 1).
        // Let w = number of elements that are in correct position for neither.
        // Then:
        // If x + z <= y -> Second wins? Actually we need to analyze turns.
        // The game: First wants to make all elements ascending. He can only change elements that are not already in correct position for him? Actually he can also change elements that are already correct, but that would be stupid.
        // The optimal strategy: Each player will try to color elements that the opponent needs, to prevent the opponent from rearranging them.
        // The known result from CF problem "Permutation Game" (maybe 1763C? Actually it's 1764D? No, it's 1763B? Let's think.)
        // There is a problem "Game on Permutation" with similar rules. The solution:
        // Let a = number of indices where p[i] == i+1 (First's fixed points)
        // Let b = number of indices where p[i] == n-i (Second's fixed points)
        // Let c = number of indices where both hold.
        // Then:
        // If a >= n - a? No.
        // Actually, the game is about who can force a win.
        // The key observation: The first player can win if he can make all elements ascending before the second player can make all descending.
        // Since players can skip, it's about who can achieve their goal in fewer "colorings" given that they can rearrange blue elements freely.
        // But they can also rearrange blue elements to mess up the opponent's progress? No, rearranging blue elements doesn't change the set of blue elements, only their order. So if a player has colored some elements blue, they can rearrange them to help themselves, but they cannot change the colors.
        // The opponent can also color elements blue, and then those elements become "free" for both to rearrange? Actually, once an element is blue, either player can rearrange it? The rule: "rearrange the elements of the permutation in such a way that all red elements keep their positions (note that blue elements can be swapped with each other, but it's not obligatory)". So blue elements can be rearranged by the player who takes the action. So if an element is blue, the player on their turn can rearrange all blue elements arbitrarily. That means if a player wants to win, they need to have all elements that are not in correct position for them to be blue, and then they can rearrange them to the correct order. But the opponent might have also colored some elements blue, and those elements might be needed by the first player in specific positions. However, the first player can rearrange all blue elements, including those colored by the opponent, to his advantage. So if an element is blue, it is under the control of the player whose turn it is. So the game is about who can first make all elements that are not already in their correct positions become blue, while preventing the opponent from doing the same.

        // Let's think differently: The game is symmetric. The first player wants ascending, the second wants descending.
        // An element is "good" for First if it is already in ascending position. It is "good" for Second if it is already in descending position.
        // If an element is good for First but not for Second, First doesn't need to color it; Second might want to color it to prevent First from using it? Actually, if it's already in correct position for First, First is happy with it staying red. Second might want to color it blue so that he can later rearrange it to his descending order? But if Second colors it blue, then on First's turn, First can rearrange it back to ascending? Wait, if an element is blue, either player can rearrange it. So if Second colors an element that was correct for First, then on First's turn, First can just put it back to the correct ascending position. So coloring an element that is already correct for the opponent doesn't help, because the opponent can just fix it again on their turn. However, if the opponent has no turn before you win? The game ends when a player achieves their goal. So if you can win on your turn, you don't care about the opponent's next turn.

        // The known solution from similar problems (e.g., Codeforces Round #827 (Div. 4) problem G? No, that's different.)
        // Let's search memory: There is a problem "Game of Permutation" from CF 1760F? No.
        // Actually, I recall a problem: "First" wins if the number of elements that are not in correct position for First is less than the number of elements that are not in correct position for Second? Not exactly.

        // Let's analyze the sample:
        // 1) 1 2 4 3 -> n=4
        //    First's correct: 1,2 are correct (positions 1,2). 3 and 4 are swapped.
        //    Second's correct: descending: 4,3,2,1. Here p = [1,2,4,3]. Correct for Second: position 1 should be 4 (is 1), position 2 should be 3 (is 2), position 3 should be 2 (is 4), position 4 should be 1 (is 3). None are correct for Second.
        //    cnt1 = 2, cnt2 = 0, both = 0.
        //    Output: First.
        // 2) 2 3 1 -> n=3
        //    First's correct: none? p[0]=2 should be 1, p[1]=3 should be 2, p[2]=1 should be 3. cnt1=0.
        //    Second's correct: descending: 3,2,1. p[0]=2 should be 3, p[1]=3 should be 2 -> correct! p[2]=1 should be 1 -> correct! So cnt2=2 (positions 2 and 3? Actually indices: i=1: p[1]=3 == n-i = 3-1=2? Wait n=3, n-i: i=0 -> 3, i=1 -> 2, i=2 -> 1. p[1]=3, n-1=2 -> not equal. Let's recalc: p = [2,3,1]. i=0: p=2, n-i=3 -> no. i=1: p=3, n-i=2 -> no. i=2: p=1, n-i=1 -> yes. So cnt2=1. both=0. Output: Tie.
        // 3) 3 4 5 2 1 -> n=5
        //    First: p[0]=3!=1, p[1]=4!=2, p[2]=5!=3, p[3]=2!=4, p[4]=1!=5 -> cnt1=0.
        //    Second: descending: 5,4,3,2,1. p[0]=3!=5, p[1]=4==4 (yes), p[2]=5!=3, p[3]=2==2 (yes), p[4]=1==1 (yes). cnt2=3. Output: Second.
        // 4) 1 5 6 3 2 4 -> n=6
        //    First: p[0]=1 yes, p[1]=5!=2, p[2]=6!=3, p[3]=3!=4, p[4]=2!=5, p[5]=4!=6 -> cnt1=1.
        //    Second: descending: 6,5,4,3,2,1. p[0]=1!=6, p[1]=5==5 yes, p[2]=6!=4, p[3]=3!=3? Wait n-i: i=3 -> 6-3=3, p[3]=3 -> yes. p[4]=2==2 yes, p[5]=4!=1. So cnt2=3 (positions 2,4,5? indices 1,3,4). Output: Tie.

        // Let's compute x = cnt1 - both, y = cnt2 - both, z = both, w = n - cnt1 - cnt2 + both.
        // Sample 1: cnt1=2, cnt2=0, both=0 -> x=2, y=0, z=0, w=2. First wins.
        // Sample 2: cnt1=0, cnt2=1, both=0 -> x=0, y=1, z=0, w=2. Tie.
        // Sample 3: cnt1=0, cnt2=3, both=0 -> x=0, y=3, z=0, w=2. Second wins.
        // Sample 4: cnt1=1, cnt2=3, both=0? Check both: any i where i+1 == n-i? n=6, i+1 = 6-i => 2i=5 => no. So both=0. x=1, y=3, z=0, w=2. Tie.

        // Pattern: 
        // If x + z > y + w? No.
        // Let's think in terms of "moves needed".
        // First needs to color all elements that are not already in correct ascending position. That is n - cnt1 elements. But he can also use elements that Second has colored? Actually, if an element is blue, First can rearrange it. So First doesn't need to color elements that are already blue. However, Second might color elements that First needs, but First can still use them. So the only obstacle for First is if Second wins before him.
        // The game is sequential. The first player can win if he can achieve ascending order on his turn before the second player achieves descending order on his turn.
        // A player can win on their turn if all elements that are not in their correct positions are blue. Because then they can rearrange them to the correct order.
        // Initially, no elements are blue. On a turn, a player can color one red element blue, or rearrange blue elements, or skip.
        // So to win, a player must have colored all necessary elements blue. They can also benefit from the opponent coloring elements blue, because those become blue and can be rearranged.
        // So the total number of elements that need to become blue for First to win is: the number of elements that are not in correct ascending position. But if some of those are already blue (colored by either player), First doesn't need to color them. So First wins when the set of blue elements includes all indices where p[i] != i+1.
        // Similarly, Second wins when the set of blue elements includes all indices where p[i] != n-i.
        // Both players can color one red element blue per turn. They can also skip.
        // So the game reduces to: There is a set of indices. First wants to cover set A = {i | p[i] != i+1}. Second wants to cover set B = {i | p[i] != n-i}. On each turn, the player whose turn it is can add one index to the blue set (if it's not already blue). They can also pass. The game ends when the blue set covers A (First wins) or covers B (Second wins). If both happen simultaneously? The player whose turn it is wins? Actually, if on First's turn, after his action, the blue set covers A, he wins immediately, even if it also covers B. Similarly for Second.
        // This is a classic game: two players want to cover their target sets by taking turns adding elements to a common pool. They can also skip. The sets A and B are known.
        // Let's denote:
        // Only A: elements in A but not in B.
        // Only B: elements in B but not in A.
        // Both: elements in both A and B.
        // Neither: elements in neither.
        // Note: A = not ascending correct, B = not descending correct.
        // So:
        // onlyA = indices where p[i] != i+1 but p[i] == n-i.
        // onlyB = indices where p[i] == i+1 but p[i] != n-i.
        // bothAB = indices where p[i] != i+1 and p[i] != n-i.
        // neither = indices where p[i] == i+1 and p[i] == n-i.
        // Wait, check: A = not ascending correct. So if p[i] == i+1, then i not in A. If p[i] != i+1, then i in A.
        // B = not descending correct. If p[i] == n-i, then i not in B. If p[i] != n-i, then i in B.
        // So:
        // onlyA: in A but not B => p[i] != i+1 and p[i] == n-i.
        // onlyB: in B but not A => p[i] == i+1 and p[i] != n-i.
        // bothAB: in A and B => p[i] != i+1 and p[i] != n-i.
        // neither: not in A and not in B => p[i] == i+1 and p[i] == n-i.
        // Let's denote:
        // a = |onlyA|, b = |onlyB|, c = |bothAB|, d = |neither|.
        // Then:
        // cnt1 = number of i with p[i] == i+1 = b + d.
        // cnt2 = number of i with p[i] == n-i = a + d.
        // both = d.
        // So x = cnt1 - both = b, y = cnt2 - both = a, z = d, w = c.
        // So the earlier x,y,z,w correspond to b,a,d,c respectively.
        // Now, the game: First wants to cover A = onlyA ∪ bothAB = a ∪ c. Second wants to cover B = onlyB ∪ bothAB = b ∪ c.
        // The common pool is the set of blue elements. Initially empty.
        // Players alternate, First starts. On a turn, a player can add one element to the pool (if it's not already there) or skip.
        // First wins if pool ⊇ A. Second wins if pool ⊇ B.
        // This is a well-known game. The outcome depends on the sizes of these sets.
        // Let's analyze:
        // First can always just color elements in A. Second can color elements in B.
        // They can also color elements in the intersection c, which helps both.
        // They can also color elements in d? d is neither, so coloring them doesn't help either player directly, but it might be a waiting move? Actually, if a player colors an element in d, it doesn't help them cover their set, but it uses a turn. So it's equivalent to skipping, but worse because it might help the opponent? No, it doesn't help the opponent either because d is not in B. So coloring d is just a skip that also reduces the number of red elements. But since the game can go on for 100^500 turns, skipping is allowed. So players will only color elements that help them, or maybe to block the opponent? But they cannot block the opponent from coloring elements because the opponent can just color them on their turn. The only blocking is by winning first.
        // So the game is essentially: First needs to color a + c elements (all of A). Second needs to color b + c elements (all of B). They share the c elements. They take turns coloring one element per turn. First goes first.
        // If First colors an element in a, it only helps him. If he colors an element in c, it helps both. If Second colors an element in b, it only helps him. If he colors an element in c, it helps both.
        // The optimal strategy: Each player will prioritize coloring elements that are unique to them? Or shared?
        // Let's think: Suppose First colors an element in c. Then that element becomes blue, so it counts for both A and B. That means Second is one step closer to winning as well. So First might want to avoid coloring c if possible, and instead color a. But if First colors a, Second can color b. Eventually, the c elements remain. Who will be forced to color c?
        // This is similar to a game where players want to complete their sets, and there is an intersection.
        // Let's denote the number of turns needed for First if he only colors a and c: he needs to color all a and all c. But Second might color some c before him.
        // Actually, the game ends as soon as one player's set is covered. So if at any point the pool covers A, First wins immediately, even if it's Second's turn? No, the win condition is checked after a player's action. So if on Second's turn, after his action, the pool covers A, does First win? The problem says: "The first player wins if the permutation is sorted in ascending order... If the game lasts for 100^500 turns and nobody wins, it ends in a draw." It doesn't specify that the win is checked only on the player's turn. It says "The first player wins if the permutation is sorted in ascending order." That is a state condition. So if after Second's move the permutation becomes ascending, First wins. So the game ends immediately when the condition is met, regardless of whose turn it is.
        // So if the pool covers A, First wins. If it covers B, Second wins. If it covers both, then whoever's condition is met first? Actually, if a move makes both conditions true simultaneously, then both win? But the problem says "The first player wins if... The second player wins if..." It doesn't specify tie-breaking if both happen at once. But can both happen at once? If the pool covers both A and B, then the permutation can be rearranged to both ascending and descending? That would require the permutation to be both ascending and descending, which is only possible if n=1. But n>=3. So it's impossible for the same permutation to be both ascending and descending. However, the pool covering A means that all elements not in ascending position are blue, so the player whose turn it is can rearrange them to ascending. But if the pool also covers B, then the same blue set can be rearranged to descending. But the permutation can't be both at the same time. The win condition is about the actual permutation, not the potential. The rule: "The first player wins if the permutation is sorted in ascending order." So the permutation itself must be sorted. If the pool covers A, the player on their turn can choose to rearrange the blue elements to make it ascending. But if they don't, the permutation might not be ascending. So the game doesn't end automatically when the pool covers the set; the player must actually perform the rearrangement to win. But they can do that on their turn. So if the pool covers A, on First's turn, he can rearrange to win. If the pool covers A on Second's turn, Second will not rearrange to ascending because that would make First win. So Second will not do that. So First can only win on his own turn. Similarly, Second can only win on his own turn. So the game is: players take turns. On your turn, if your target set is a subset of the blue set, you can rearrange to win. Otherwise, you can color an element or skip.
        // So the win condition is: at the start of your turn, or after your action, if your target set is covered, you can win. But you can only win on your turn because you need to take the rearrange action.
        // So the game is a race: First wants to make A ⊆ blue on his turn. Second wants to make B ⊆ blue on his turn.
        // They can color one element per turn. They can also skip.
        // This is exactly the game of "who can complete their set first" with shared elements.
        // Let's analyze the race.
        // Let a = |onlyA|, b = |onlyB|, c = |bothAB|.
        // First needs a + c elements. Second needs b + c elements.
        // First can color elements from a or c. Second can color from b or c.
        // If a player colors from c, it helps both.
        // The optimal strategy: 
        // First will try to color a elements first, because they don't help Second.
        // Second will try to color b elements first.
        // They will only color c elements if they have to.
        // The game ends when one player has all their needed elements blue.
        // Let's simulate the race optimally.
        // Suppose First colors only a, Second colors only b. They alternate: First colors a, Second colors b, etc.
        // After some turns, one of them runs out of their exclusive elements. Then they must start coloring c.
        // Let's denote the number of turns taken.
        // Let t be the turn number (1-indexed). First moves on odd t, Second on even t.
        // First's progress: number of a he has colored + number of c colored by either.
        // Second's progress: number of b he has colored + number of c colored by either.
        // They both want to minimize the number of turns until their set is complete, and maximize the opponent's.
        // This is a classic game. The outcome depends on the comparison of a and b.
        // Let's think: If a > b, then First has more exclusive elements than Second. That means Second will run out of exclusive elements first. Then Second will be forced to color c elements, which also help First. So First might win.
        // Conversely, if b > a, Second might win.
        // But we also have the initial turn advantage.
        // Let's formalize:
        // Let's say the game proceeds in phases. First, both color their exclusive elements if available.
        // First colors an a-element on turn 1. Second colors a b-element on turn 2. First colors a on turn 3, etc.
        // This continues until one player exhausts their exclusive elements.
        // Suppose a <= b. Then First will run out of a-elements first. After a turns of First (which happen on turns 1,3,...,2a-1), all a-elements are blue. During this time, Second has colored a b-elements (on turns 2,4,...,2a). So there are b - a b-elements left.
        // Now, on First's next turn (turn 2a+1), he has no a-elements left. He must either color a c-element or skip. If he colors a c-element, it helps both. If he skips, Second will continue coloring b-elements. So First will likely color a c-element to progress towards his win. But coloring c also helps Second. So the race becomes: First needs c more elements (all c must be blue). Second needs (b - a) + c more elements. But note that the c-elements are shared. So the total remaining for First: c. For Second: (b - a) + c.
        // Now, both players will color c-elements. They alternate. First just colored a c-element on turn 2a+1. Then Second on turn 2a+2 can color a b-element or a c-element. Second wants to win as soon as possible. He needs (b - a) exclusive and c shared. He can color his exclusive b-elements without helping First. So he will color b-elements as long as he has them. So after First starts coloring c, Second will continue coloring b-elements until they are exhausted. So on turns 2a+2, 2a+4, ..., Second colors b-elements. There are (b - a) such turns. During these turns, First is coloring c-elements on his turns (2a+3, 2a+5, ...). So after Second exhausts his b-elements, how many c-elements are left?
        // Let's count turns carefully.
        // Let a <= b.
        // Turns 1 to 2a: First colors a a-elements, Second colors a b-elements. Remaining: a=0, b' = b - a, c full.
        // Turn 2a+1 (First): colors a c-element. c remaining = c-1.
        // Now, while b' > 0:
        //   Turn 2a+2 (Second): colors a b-element. b'--.
        //   Turn 2a+3 (First): colors a c-element. c--.
        //   ...
        // This continues until b' = 0. How many turns does it take for Second to exhaust b'? He needs b' turns. During these b' turns, First gets ceil(b'/2) or floor(b'/2) turns? Let's see: The pattern after turn 2a+1: Second moves on even turns, First on odd turns. So if b' > 0, the sequence of turns: Second (2a+2), First (2a+3), Second (2a+4), First (2a+5), ... until Second's b' are exhausted. The number of Second turns needed is b'. The number of First turns during this period is either b' or b'-1 depending on who gets the last turn. Since Second starts this phase, if b' is the number of Second turns, then First gets b' turns if the last turn of this phase is First's? Actually, if Second takes b' turns, they are on even turns. The last Second turn is turn 2a + 2b'. The First turns during this phase are turns 2a+3, 2a+5, ..., up to possibly 2a+2b'-1 or 2a+2b'+1? Let's just simulate the counts.
        // After Second exhausts b', the remaining c is reduced by the number of First turns in this phase. Let that be k1. Then c_remaining = c - 1 - k1.
        // Now, both players only need c_remaining. They both need all remaining c elements. They will alternate coloring c. First moves next? After Second's last b-element, the next turn is First's (since Second just moved). So First will color a c-element, then Second, etc. They both need the same number of c elements. The player who gets to color the last c-element will win on that turn? Actually, if a player colors the last c-element, then after their turn, the blue set covers all c, and since they already have their exclusive elements (First has a=0 left, but he already has all a? Wait, First already colored all a. Second already colored all b. So both have their exclusive sets complete. The only missing are c. So whoever colors the last c-element makes the blue set cover both A and B? Actually, A = a ∪ c, B = b ∪ c. If all a, b, c are blue, then both A and B are covered. So on the turn when the last c-element is colored, the player who colored it can then rearrange to win. But wait, if both sets are covered, the player whose turn it is can win. So the one who colors the last c-element wins.
        // So the game reduces to: after exclusive elements are exhausted, the remaining c elements are contested. The player who makes the last move on c wins.
        // So we need to determine who gets to color the last c-element.
        // Let's compute the total number of turns needed for First to win if he follows this strategy, and for Second to win if he follows his strategy. But they can deviate: e.g., First might start coloring c before exhausting a, if that helps him win faster? But coloring c helps the opponent, so it's only beneficial if it allows you to win before the opponent would win otherwise. Usually, you want to delay coloring c as much as possible.
        // This is a known impartial game? Actually, it's a partizan game because the sets are different.
        // There is a known solution for this specific problem (I think it's from Codeforces Round 907 (Div. 2) problem C? Or maybe 1764C? Let's search memory: There is a problem "Game on Permutation" with exactly these rules. The solution is:
        // Let a = number of elements that are in correct position for First but not Second? Wait, we defined a = onlyA = elements that are wrong for First but correct for Second. That is a = cnt2 - both.
        // b = onlyB = elements correct for First but wrong for Second = cnt1 - both.
        // c = both wrong = n - cnt1 - cnt2 + both.
        // Then:
        // If a + c <= b: Second wins? Let's test with samples.
        // Sample 1: a=0, b=2, c=2. a+c=2, b=2 -> a+c <= b -> 2<=2 -> Second? But output is First. So not that.
        // Maybe: If b + c <= a: First wins? Sample 1: b+c=4, a=0 -> 4<=0 false.
        // Let's think differently.
        // Let's denote the number of turns each player needs if they only color their own needed elements, ignoring the opponent's interference.
        // First needs to color a + c elements. Second needs b + c.
        // They alternate. First goes first.
        // If First colors an element, it reduces his needed count by 1. If it's a c-element, it also reduces Second's needed count by 1.
        // This is similar to a game where players have "exclusive" and "shared" tasks.
        // The known result from similar problems (e.g., "C. Game on Permutation" from CF 1760? Actually, I recall a problem: "First" wins if a > b. "Second" wins if b > a. "Tie" if a == b? Let's test.
        // Sample 1: a=0, b=2 -> b > a -> Second? But output First. So no.
        // Maybe it's based on the number of elements that are already in correct position? cnt1 and cnt2.
        // Sample 1: cnt1=2, cnt2=0 -> First wins.
        // Sample 2: cnt1=0, cnt2=1 -> Tie.
        // Sample 3: cnt1=0, cnt2=3 -> Second wins.
        // Sample 4: cnt1=1, cnt2=3 -> Tie.
        // So when cnt1 > cnt2? Sample 1: 2>0 First. Sample 4: 1<3 Tie. Sample 3: 0<3 Second. Sample 2: 0<1 Tie.
        // Maybe: If cnt1 > cnt2: First wins. If cnt2 > cnt1 + 1? Sample 3: cnt2=3, cnt1=0 -> 3 > 1 -> Second. Sample 4: cnt2=3, cnt1=1 -> 3 > 2? 3>2 yes, but output Tie. So not exactly.
        // Let's look at the differences: cnt1 - cnt2.
        // Sample 1: 2. Sample 2: -1. Sample 3: -3. Sample 4: -2.
        // Output: First, Tie, Second, Tie.
        // Maybe: If cnt1 > cnt2: First. If cnt2 > cnt1 + 1: Second. Else Tie? Sample 4: cnt2=3, cnt1=1 -> cnt2 > cnt1+1? 3 > 2 yes -> Second, but output Tie. So no.
        // Let's consider the number of elements that are not in correct position for either: w = n - cnt1 - cnt2 + both.
        // Sample 1: w=2. Sample 2: w=2. Sample 3: w=2. Sample 4: w=2. All have w=2? Sample 1: n=4, cnt1=2, cnt2=0, both=0 -> w=2. Sample 2: n=3, cnt1=0, cnt2=1, both=0 -> w=2. Sample 3: n=5, cnt1=0, cnt2=3, both=0 -> w=2. Sample 4: n=6, cnt1=1, cnt2=3, both=0 -> w=2. Interesting, all samples have w=2. So w might not be the deciding factor.
        // Let's try to derive the game formally.
        // Let A = set of indices where p[i] != i+1. B = set where p[i] != n-i.
        // First wants to cover A. Second wants to cover B.
        // Let a = |A \ B|, b = |B \ A|, c = |A ∩ B|.
        // First needs a+c, Second needs b+c.
        // The game: players alternate coloring elements. They can color any element not yet colored. They can also skip.
        // This is a combinatorial game. We can think of it as: each player wants to be the first to have their set covered. Since they can skip, it's a race.
        // Let's analyze the race optimally.
        // Suppose both players
