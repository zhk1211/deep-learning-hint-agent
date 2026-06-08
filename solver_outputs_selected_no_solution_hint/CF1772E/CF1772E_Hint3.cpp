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

        // Find longest prefix that is already sorted ascending
        int pref = 0;
        while (pref < n && p[pref] == pref + 1) {
            ++pref;
        }

        // Find longest suffix that is already sorted descending
        int suff = 0;
        while (suff < n && p[n - 1 - suff] == n - suff) {
            ++suff;
        }

        // Count elements that are in correct position for ascending
        int fixed_asc = 0;
        for (int i = 0; i < n; ++i) {
            if (p[i] == i + 1) {
                ++fixed_asc;
            }
        }

        // Count elements that are in correct position for descending
        int fixed_desc = 0;
        for (int i = 0; i < n; ++i) {
            if (p[i] == n - i) {
                ++fixed_desc;
            }
        }

        // First player wants ascending, second wants descending.
        // The game reduces to: players can lock elements by coloring them blue.
        // The key is the number of elements that are already in the correct position
        // for each player, and the number of elements that are "free" (not fixed for either).
        // Actually, the analysis from hints leads to comparing counts of "red" elements
        // that are smaller than all blue elements, etc. But the known solution pattern:
        // Let a = number of elements already in correct ascending position.
        // Let b = number of elements already in correct descending position.
        // Let c = n - a - b + (number of elements that are in both correct positions? but permutation has no duplicates, so intersection is at most 1 if n odd and middle matches both).
        // Actually, elements that are correct for both ascending and descending are those where p[i] == i+1 and p[i] == n-i, which implies i+1 == n-i => 2i+2 = n => i = (n-2)/2, only possible if n even? Wait: i+1 = n-i => 2i = n-1 => i = (n-1)/2. So if n is odd, the middle element can be correct for both. But the problem states permutation is not fully sorted ascending or descending, so not all elements are fixed for one side.

        // Known solution from editorial:
        // Let A = number of positions where p[i] == i+1 (good for first)
        // Let B = number of positions where p[i] == n-i (good for second)
        // Let C = n - A - B + (number of positions where both hold, which is at most 1)
        // Actually, positions where both hold are counted in both A and B, so we need intersection size.
        int both = 0;
        for (int i = 0; i < n; ++i) {
            if (p[i] == i + 1 && p[i] == n - i) {
                ++both;
            }
        }

        int a = fixed_asc;
        int b = fixed_desc;
        int c = n - a - b + both; // elements that are wrong for both

        // The game outcome depends on a, b, c.
        // First player wins if a + c <= b? Actually, the known condition:
        // If a > b + c: First wins immediately? No.
        // Let's derive from the turn-based coloring game.
        // The players will color elements that are wrong for them but right for the opponent?
        // The typical solution:
        // First wins if a > b + c? Wait, check sample:
        // Sample 1: 1 2 4 3 -> a=2 (1,2), b=0? p=[1,2,4,3], descending correct: p[0]==4? no, p[1]==3? no, p[2]==2? no, p[3]==1? no. So b=0. both=0. c = 4-2-0=2. a=2, b=0, c=2. a > b? yes. Output First.
        // Sample 2: 2 3 1 -> a=0? p[0]=2!=1, p[1]=3!=2, p[2]=1!=3. b: p[0]==3? no, p[1]==2? no, p[2]==1? yes -> b=1. both=0. c=3-0-1=2. a=0, b=1, c=2. Output Tie.
        // Sample 3: 3 4 5 2 1 -> a: p[0]=3!=1, p[1]=4!=2, p[2]=5!=3, p[3]=2!=4, p[4]=1!=5 => a=0. b: p[0]==5? no, p[1]==4? yes, p[2]==3? no, p[3]==2? yes, p[4]==1? yes => b=3. both=0. c=5-0-3=2. a=0, b=3, c=2. Output Second.
        // Sample 4: 1 5 6 3 2 4 -> a: p[0]=1 yes, p[1]=5!=2, p[2]=6!=3, p[3]=3!=4, p[4]=2!=5, p[5]=4!=6 => a=1. b: p[0]==6? no, p[1]==5? yes, p[2]==4? no, p[3]==3? yes, p[4]==2? yes, p[5]==1? no => b=3. both=0. c=6-1-3=2. a=1, b=3, c=2. Output Tie.

        // Pattern: 
        // If a > b + c: First wins? Sample 1: a=2, b+c=2 -> not >, equal. But output First.
        // If b > a + c: Second wins? Sample 3: b=3, a+c=2 -> b > a+c, output Second.
        // Else Tie? Sample 2: a=0, b=1, c=2 -> b <= a+c? 1 <= 2 yes, a <= b+c? 0 <= 3 yes -> Tie. Sample 4: a=1, b=3, c=2 -> a <= b+c (1<=5), b <= a+c (3<=3) -> Tie.
        // Sample 1: a=2, b=0, c=2 -> a <= b+c? 2 <= 2 yes, b <= a+c? 0 <= 4 yes -> would be Tie by that rule, but output First. So rule is not symmetric.

        // Let's think: First player can win if he can force ascending before second forces descending.
        // The players can color elements. Coloring an element removes it from being swapped by the opponent? Actually, red elements keep positions, blue can be rearranged. So if an element is blue, it can be moved. If it's red, it's locked.
        // First wants ascending, so he wants to lock elements that are already in correct ascending position, and color blue the ones that are wrong, so he can rearrange them. Second wants descending, so he wants to lock elements correct for descending, and color blue the wrong ones.
        // But they share the same set of elements. If an element is correct for both, it's good for both to lock it? Actually, if it's correct for both, neither wants to move it, so both would like it red. But they can't both lock it; once one player colors it blue, the other might move it? Wait, coloring is an action: change color of one red element to blue. Once blue, it can be rearranged. So if an element is correct for ascending but wrong for descending, First wants it red (locked), Second wants it blue (so he can move it to descending position). So they have opposing interests on elements that are correct for only one side.
        // Elements wrong for both: both want them blue so they can move them.
        // Elements correct for both: both want them red.

        // The game is sequential. The known solution from similar problems (e.g., Codeforces Round #789 Div2 C? Actually this is CF 1787C? No, it's from a known problem "Permutation Game"): 
        // Let x = number of elements that are correct for ascending but not for descending.
        // Let y = number of elements that are correct for descending but not for ascending.
        // Let z = number of elements wrong for both.
        // Let w = number of elements correct for both (0 or 1).
        // Then a = x + w, b = y + w, c = z.
        // First wants to lock x and w, color z and y blue. Second wants to lock y and w, color z and x blue.
        // The players take turns. They can also skip.
        // The game ends when one player can rearrange the blue elements to achieve their goal. But they can only rearrange if all elements that need to be moved are blue, and the red elements are already in correct positions.
        // So First wins if at some point, all elements that are not in correct ascending position are blue, and he can rearrange them to ascending. That means the set of red elements is a subset of the ascending-correct positions. Similarly for Second.

        // This is a game of "who can collect enough blue elements first". 
        // Actually, the known solution: 
        // If a > b + c: First wins? But sample 1 contradicts.
        // Let's re-evaluate sample 1: a=2, b=0, c=2. First wins. 
        // Maybe the condition is: First wins if a > b? No, sample 4: a=1, b=3, a < b but Tie.
        // Let's simulate sample 1: n=4, p=[1,2,4,3]. Ascending correct: 1,2. Descending correct: none. Wrong both: 3,4? Actually 4 is at pos3, should be 4 for ascending? pos3 is index 2 (0-based), value 4 -> correct for ascending? p[2]=4, i+1=3? Wait, ascending: p[i] should be i+1. p[2]=4, i+1=3 -> not correct. p[3]=3, i+1=4 -> not correct. So a=2 (positions 0,1). Descending: p[i] should be n-i = 4-i. p[0]=1, should be 4 -> no. p[1]=2, should be 3 -> no. p[2]=4, should be 2 -> no. p[3]=3, should be 1 -> no. So b=0. c=2 (positions 2,3). Both=0.
        // First wants to lock 1,2 and make 3,4 blue. Second wants to lock nothing and make everything blue? Actually Second wants descending, so he wants to lock elements that are correct for descending (none), and make the rest blue. So Second wants all elements blue. First wants only 3,4 blue.
        // Turn 1: First can color one red element blue. He colors 3 or 4. Say he colors 3 (value 4). Now blue: {4}. Red: {1,2,3}. 
        // Turn 2: Second can color something. If he colors 1 or 2, he helps First? Actually, if Second colors 1 (value 1), then 1 becomes blue. Then First on his next turn could rearrange blue elements? But blue elements are {4,1}. He can swap them? He can rearrange blue elements arbitrarily. But red elements keep positions. Red: {2,3}. Positions: pos0: red 1? Wait, after coloring 1 blue, pos0 becomes blue? Actually, the element 1 at pos0 is now blue. So pos0 is blue. pos1: red 2. pos2: red 3? Wait, we colored value 4 at pos2 blue. So pos2 is blue. pos3: red 3. So red positions: pos1 (value 2) and pos3 (value 3). First wants ascending: he needs pos0=1 (blue, can be moved), pos1=2 (red, correct), pos2=3 (red? currently red 3 is at pos3, pos2 is blue with value 4), pos3=4 (blue). He can rearrange blue elements: he has values 1 and 4 in blue. He can place 1 at pos0, 4 at pos3. Then red at pos1 is 2, red at pos2? pos2 is blue, so he can put 3 there? But 3 is red at pos3! He cannot move red elements. So he cannot put 3 at pos2 because 3 is red and stuck at pos3. So he needs 3 to be blue. So First needs both 3 and 4 blue. Second coloring 1 doesn't hurt First, but it gives First more blue elements to play with? Actually, First only needs to make the wrong elements blue. If Second colors a correct element blue, First can still win if he can rearrange everything. But if Second colors a correct element blue, that element is no longer locked in correct position, so First would have to move it back? But he can just move it back during rearrangement. So it doesn't prevent First from winning, as long as all elements that need to be moved are blue. The only way to prevent First from winning is to lock an element that First needs to move, i.e., keep it red. So Second wants to keep elements that are wrong for ascending (i.e., not in a) as red, so First cannot move them. But Second also wants to win himself. He wins if all elements wrong for descending are blue. So Second wants to color elements that are wrong for descending (i.e., not in b) blue. 

        // This is a race: First wants to color all elements not in a blue. Second wants to color all elements not in b blue. They can also color elements that are in the opponent's target set to disrupt? Actually, if First colors an element that is in b (correct for descending), that element becomes blue, which helps Second because Second wants it blue? Wait, Second wants elements not in b to be blue. Elements in b he wants red. So if First colors an element in b blue, he is helping Second by making one of Second's "wanted red" elements blue, which Second can then move? But Second wants them red to lock them in descending order. If they become blue, Second can still move them to descending order during rearrangement, but he would have to rearrange them. However, if they are red, they are already in correct descending position and locked. So Second prefers them red. So First coloring an element in b blue is bad for Second? Actually, it forces that element to be movable, so Second might have to waste a turn moving it back? But Second can just leave it and rearrange at the end. The real issue is that if an element is blue, it can be rearranged by either player. So if First makes an element blue, he can later rearrange it to ascending order, which might conflict with Second's descending order. So it's a shared pool of blue elements. The player who wants to rearrange needs all elements that are not already in their desired positions to be blue. So the game is about who can first achieve the condition: all elements outside their fixed set are blue.

        // Let S1 = set of indices where p[i] != i+1 (wrong for ascending). First wins if all indices in S1 are blue.
        // Let S2 = set of indices where p[i] != n-i (wrong for descending). Second wins if all indices in S2 are blue.
        // Initially all red.
        // On a turn, a player can color one red element blue, or skip, or rearrange blue elements (which doesn't change colors, but can win if condition met).
        // So the game is: players take turns coloring elements blue. They can also win immediately if the condition is already met (but initially it's not, because permutation is not sorted). They can also skip.

        // This is a classic impartial game? But players have different goals.
        // Let's denote:
        // A = S1 \ S2 = elements wrong for ascending but correct for descending. (First wants them blue, Second wants them red.)
        // B = S2 \ S1 = elements wrong for descending but correct for ascending. (Second wants them blue, First wants them red.)
        // C = S1 ∩ S2 = elements wrong for both. (Both want them blue.)
        // D = complement = elements correct for both. (Both want them red.)

        // Note: |A| = b - both? Wait: S1 wrong for ascending = not a. S2 wrong for descending = not b.
        // A = (not a) and (b) = b \ a = b - both.
        // B = (not b) and (a) = a \ b = a - both.
        // C = (not a) and (not b) = c.
        // D = a ∩ b = both.

        // So:
        // A = b - both
        // B = a - both
        // C = c
        // D = both

        // First wants to color A ∪ C blue. Second wants to color B ∪ C blue.
        // They can also color elements from the other's set, but that might be suboptimal.
        // The game ends when one player's target set is entirely blue. They can then rearrange and win.

        // This is a game where each player wants to color a subset of elements. They take turns coloring one element. They can also color elements outside their target set, which might delay the opponent? Actually, coloring an element in B (which First wants red) makes it blue, which helps Second. So First should never color B. Similarly Second should never color A. They both want to color C. They might also color D? D is correct for both, both want it red. Coloring D blue helps the opponent? If D becomes blue, then it's movable, so First would have to move it to ascending position (which is also its current position) and Second would have to move it to descending position (same). So it doesn't hurt much, but it wastes a turn. So optimal play: players will only color elements in their target set (A ∪ C for First, B ∪ C for Second). They will not color the opponent's exclusive set or D.

        // So the game reduces to: There are A, B, C sets. First needs to color all A and C. Second needs to color all B and C. They alternate coloring one element from their own target set. They can also skip. Who wins?

        // Let x = |A|, y = |B|, z = |C|.
        // First needs x+z blue. Second needs y+z blue.
        // Initially 0 blue.
        // Turn order: First, Second, First, Second...
        // They can skip. Skipping is equivalent to passing, but if both skip, game ends in draw after 100^500 turns. So skipping is only useful if you are waiting for the opponent to make a move that benefits you? But here, making a move only progresses your own goal. So you never want to skip unless you are forced to? Actually, if you color an element, you get closer to winning. The only reason to skip is if coloring any element would cause you to lose? But coloring an element from your target set never hurts you; it only helps you. Coloring outside your target set helps opponent, so you wouldn't do that. So you always have a move that helps you (unless your target set is already fully blue, then you win immediately). So skipping is never beneficial. Thus, players will just color one element from their target set each turn.

        // So it's a race: First colors one from A∪C per turn, Second colors one from B∪C per turn. They share C. So the total number of turns needed for First to color all his target is x+z (if he colors all of them himself). But Second might color some of C, which also helps First! Because if Second colors an element from C, it becomes blue, which counts for First's target. So First's progress is advanced by both his own colorings of A∪C and Second's colorings of C. Similarly, Second's progress is advanced by his own colorings of B∪C and First's colorings of C.

        // Let's model: Let f be number of elements First has colored from A∪C. Let s be number of elements Second has colored from B∪C. But they both can color C. The total blue elements in A∪C is f + (number of C colored by Second). The total blue in B∪C is s + (number of C colored by First).

        // Since they play optimally, they will choose which elements to color. First will prioritize coloring A or C? Second will prioritize coloring B or C? They might want to color C to prevent the opponent from coloring it? Actually, if First colors C, he progresses his own goal and also progresses Second's goal (since C is in Second's target). If First colors A, he only progresses his own goal. So First would prefer to color A over C, because coloring C helps the opponent. Similarly, Second prefers B over C. So optimal strategy: each player first colors their exclusive elements (A for First, B for Second). Only when their exclusive set is exhausted, they start coloring C.

        // Let's verify: Suppose First colors A first. Second colors B first. After x turns, First has colored all A. After y turns, Second has colored all B. Then they both need to color C. They will alternate coloring C. The first one to finish C wins? But they also need the opponent's help? Actually, if First has colored all A, he still needs C blue. He can color C himself. Second also needs C blue. They will both color C. The game ends when one player's entire target set is blue. That happens when the last element of C is colored. Whoever's turn it is when the last C is colored? But both need C blue. So the moment the last C is colored, both players' target sets become fully blue simultaneously? Wait, if both need C blue, and C becomes fully blue, then both conditions are met at the same time. Who wins? The problem says: "The first player wins if the permutation is sorted in ascending order. The second player wins if it is sorted in descending order." If both happen at the same time, what happens? The game ends when a player wins. If both conditions are met simultaneously, who is declared winner? The problem doesn't specify, but typically in such games, the player whose turn it is might win, or it might be a tie? Let's check sample 2: a=0, b=1, c=2. x = b - both = 1, y = a - both = 0, z = c = 2. First needs x+z=3, Second needs y+z=2. First colors A (size 1) on turn 1. Second has no B (y=0), so he must color C. Turn 2: Second colors C. Now C has 1 blue, 1 left. Turn 3: First colors the last C. Now C is fully blue. First's target: A done, C done -> wins? But Second's target: B done (0), C done -> also wins? Both conditions met. Sample output is Tie. So if both win at the same time, it's a tie.

        // Sample 1: a=2, b=0, c=2. x = 0, y = 2, z = 2. First needs 2, Second needs 4. First has no A, so he colors C. Second colors B (size 2). Turn 1: First colors C (1 left). Turn 2: Second colors B (1 left). Turn 3: First colors last C. Now First's target fully blue -> First wins. Second's target: B has 1 left, C done -> not yet. So First wins. Output First. Matches.

        // Sample 3: a=0, b=3, c=2. x = 3, y = 0, z = 2. First needs 5, Second needs 2. Turn 1: First colors A (x=2 left). Turn 2: Second colors C (z=1 left). Turn 3: First colors A (x=1 left). Turn 4: Second colors last C. Now Second's target fully blue -> Second wins. First's target: A has 1 left, C done -> not yet. So Second wins. Output Second. Matches.

        // Sample 4: a=1, b=3, c=2. x = b - both = 3, y = a - both = 1, z = 2. First needs x+z=5, Second needs y+z=3. Turn 1: First colors A (x=2 left). Turn 2: Second colors B (y=0 left). Turn 3: First colors A (x=1 left). Turn 4: Second colors C (z=1 left). Turn 5: First colors A (x=0 left). Now First has colored all A, still needs C. Turn 6: Second colors last C. Now Second's target: B done, C done -> wins. First's target: A done, C done -> also wins? Wait, at turn 6, Second colors last C. After his move, C is fully blue. Both targets become fully blue simultaneously. So it's a tie. Output Tie. Matches.

        // So the rule is: simulate the race. First colors A first, then C. Second colors B first, then C. They alternate. We need to determine who reaches their target first, or if both reach at the same time.

        // Let's formalize:
        // x = b - both
        // y = a - both
        // z = c
        // First needs to color x elements from A and z elements from C. Second needs to color y elements from B and z elements from C.
        // They share C. First will color A first, Second will color B first.
        // Let's simulate turns:
        // Turn number 1 (First): if x > 0, color A (x--); else color C (z--).
        // Turn 2 (Second): if y > 0, color B (y--); else color C (z--).
        // And so on.
        // We need to find the first turn where either First's remaining (x+z) == 0 or Second's remaining (y+z) == 0.
        // But note: when a player colors C, it reduces z for both. So we can just simulate until one player's total needed reaches 0. Since n up to 5e5, we can simulate in O(n) per test case, but sum n 5e5, so O(n) total is fine. However, we can also compute directly.

        // Let's compute directly:
        // First will take x turns to clear A. During these x turns, Second will take min(x, y) turns to clear B? Actually, they alternate. So after k turns, First has taken ceil(k/2) turns, Second has taken floor(k/2) turns.
        // Let f_turns = number of turns First has taken, s_turns = number of turns Second has taken.
        // First's progress: he colors A first. So he spends min(f_turns, x) on A, and max(0, f_turns - x) on C.
        // Second's progress: he spends min(s_turns, y) on B, and max(0, s_turns - y) on C.
        // The total C colored by both is max(0, f_turns - x) + max(0, s_turns - y).
        // The game ends when either:
        // First's remaining = (x - min(f_turns, x)) + (z - (max(0, f_turns - x) + max(0, s_turns - y))) <= 0
        // or Second's remaining = (y - min(s_turns, y)) + (z - (max(0, f_turns - x) + max(0, s_turns - y))) <= 0
        // Actually, the remaining C is z minus total C colored by both. But careful: C colored by both cannot exceed z. So we cap at z.

        // We can just simulate turn by turn until one wins or both win simultaneously. Since total n is 5e5, we can simulate each test case in O(x+y+z) = O(n). That's acceptable.

        // But we can also do it in O(1) with math. Let's derive:
        // Let's consider the race for C after A and B are exhausted.
        // First finishes A after 2*x - 1 turns? Actually, First takes x turns to clear A. Since First moves on odd turns, his x-th turn is turn number 2x-1. At that point, Second has taken x-1 turns (since Second moves after First). So Second has colored min(x-1, y) of B.
        // Similarly, Second finishes B after 2*y turns (his y-th turn is turn 2y). At that point, First has taken y turns.
        // They then both color C. The one who finishes their exclusive set first will start coloring C earlier.
        // We can compute the exact turn when the last C is colored, and who colors it.

        // Let's define:
        // We can just simulate with a while loop, but since n is up to 5e5, and sum n 5e5, simulation is O(n) total, which is perfectly fine. We'll just simulate the process.

        // However, we must be careful: the simulation might take up to n steps per test case, sum n 5e5, so at most 5e5 iterations total, very fast.

        // Let's implement simulation:
        // x = b - both
        // y = a - both
        // z = c
        // turn = 0 (0 for First, 1 for Second)
        // while (true):
        //   if turn == 0:
        //     if x > 0: x--
        //     else if z > 0: z--
        //     else: // nothing to color? but First can skip. If he has no moves, he skips. But if x==0 and z==0, he already won? Actually, if his target is 0, he wins immediately before his turn? The win condition is checked at the start of turn? The problem says: "The first player wins if the permutation is sorted in ascending order." This is a state condition. So after any action, if the state is sorted, that player wins. So we should check win condition after each action.
        //   else: // Second's turn
        //     if y > 0: y--
        //     else if z > 0: z--
        //     else: skip
        //   After the move, check if First's target (x+z==0) or Second's target (y+z==0).
        //   If both are 0: return Tie.
        //   If only First's target 0: return First.
        //   If only Second's target 0: return Second.
        //   turn ^= 1.

        // But wait: what if a player has no moves (x=0, z=0 for First) but it's not their turn? They would have already won on the previous turn. So we check after each move.

        // Let's test with samples.

        // Sample 1: x=0, y=2, z=2.
        // turn 0 (First): x=0, z>0 -> z=1. Check: First target: x+z=1 !=0. Second: y+z=2+1=3 !=0.
        // turn 1 (Second): y>0 -> y=1. Check: First: 0+1=1, Second: 1+1=2.
        // turn 0: z>0 -> z=0. Check: First: 0+0=0, Second: 1+0=1. First wins. Output First. Correct.

        // Sample 2: x=1, y=0, z=2.
        // turn 0: x>0 -> x=0. Check: First: 0+2=2, Second: 0+2=2.
        // turn 1: y=0, z>0 -> z=1. Check: First: 0+1=1, Second: 0+1=1.
        // turn 0: x=0, z>0 -> z=0. Check: First: 0, Second: 0. Both 0 -> Tie. Correct.

        // Sample 3: x=3, y=0, z=2.
        // turn 0: x=2. First: 2+2=4, Second: 0+2=2.
        // turn 1: z=1. First: 2+1=3, Second: 0+1=1.
        // turn 0: x=1. First: 1+1=2, Second: 0+1=1.
        // turn 1: z=0. First: 1+0=1, Second: 0+0=0 -> Second wins. Correct.

        // Sample 4: x=3, y=1, z=2.
        // turn 0: x=2. First: 2+2=4, Second: 1+2=3.
        // turn 1: y=0. First: 2+2=4, Second: 0+2=2.
        // turn 0: x=1. First: 1+2=3, Second: 0+2=2.
        // turn 1: z=1. First: 1+1=2, Second: 0+1=1.
        // turn 0: x=0. First: 0+1=1, Second: 0+1=1.
        // turn 1: z=0. First: 0, Second: 0 -> Tie. Correct.

        // So simulation works.

        // But wait: Is it always optimal for First to color A before C? What if First colors C first to prevent Second from getting it? But coloring C helps Second. Let's test if there's a case where First should color C before A. Suppose x=1, y=10, z=1. First needs 2, Second needs 11. If First colors A first: turn 0: x=0. turn 1: Second colors B (y=9). turn 2: First colors C (z=0) -> First wins. If First colored C first: turn 0: z=0. turn 1: Second colors B (y=9). turn 2: First colors A (x=0) -> First still wins on turn 2. Same. What if x=2, y=1, z=1? First needs 3, Second needs 2. If First colors A first: turn 0: x=1. turn 1: Second colors B (y=0). turn 2: First colors A (x=0). turn 3: Second colors C (z=0) -> Second wins on turn 3? Check: after turn 3, Second's target: y+z=0 -> Second wins. First's target: x+z=0? x=0, z=0 -> also 0. So both win -> Tie. If First colored C first: turn 0: z=0. turn 1: Second colors B (y=0). turn 2: First colors A (x=1). turn 3: Second has no moves? y=0, z=0, so Second skips. turn 4: First colors A (x=0) -> First wins? Check after turn 2: First target: x+z=1+0=1, Second: 0+0=0 -> Second wins immediately after turn 2? Wait, after turn 2, Second's target is 0, so Second wins on turn 2. So First loses faster. So coloring C first is worse. So optimal is to color exclusive set first.

        // What if both have exclusive sets, but one is much larger? The simulation with priority to exclusive sets seems correct based on the game theory: you don't want to help the opponent by coloring shared elements before you have to.

        // Therefore, the simulation approach is correct.

        // Let's implement it efficiently.

        int both = 0;
        for (int i = 0;
