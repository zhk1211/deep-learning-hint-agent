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

        // Check if already sorted ascending or descending (guaranteed not, but just in case)
        bool asc = true, desc = true;
        for (int i = 0; i < n; ++i) {
            if (p[i] != i + 1) asc = false;
            if (p[i] != n - i) desc = false;
        }

        // Count fixed points for ascending and descending
        int fixed_asc = 0, fixed_desc = 0;
        for (int i = 0; i < n; ++i) {
            if (p[i] == i + 1) fixed_asc++;
            if (p[i] == n - i) fixed_desc++;
        }

        // Moves needed for each player to win by only coloring
        int moves_first = n - fixed_asc;
        int moves_second = n - fixed_desc;

        // Determine result based on the game analysis
        // First wins if moves_first <= moves_second and first can force win before second
        // Second wins if moves_second < moves_first and second can force win before first
        // Otherwise Tie

        if (moves_first <= moves_second) {
            // First can win if he can finish before second, considering turns
            // First moves first, so if moves_first <= moves_second, first can win
            // But need to check if second can win earlier
            if (moves_first <= moves_second) {
                // First can win if he can complete his set before second completes his
                // Actually, first wins if moves_first <= moves_second and first can force
                // More precise: first wins if moves_first <= moves_second and 
                // (moves_first < moves_second or (moves_first == moves_second and first can block))
                // But from analysis: first wins if moves_first <= moves_second and 
                // first can finish before second, considering second can also color.
                // The known condition: first wins if moves_first <= moves_second and 
                // (moves_first < moves_second or (moves_first == moves_second and first can win))
                // Actually, if moves_first == moves_second, first can win if he can force a win
                // by coloring his last element and then rearranging? No, rearrangement is free.
                // The known solution: first wins if moves_first <= moves_second and 
                // (moves_first < moves_second or (moves_first == moves_second and first can win))
                // But from sample: 1 2 4 3 -> fixed_asc=2, fixed_desc=0, moves_first=2, moves_second=4
                // moves_first < moves_second -> First wins.
                // Sample 2: 2 3 1 -> fixed_asc=0, fixed_desc=0, moves_first=3, moves_second=3 -> Tie
                // Sample 3: 3 4 5 2 1 -> fixed_asc=0, fixed_desc=2, moves_first=5, moves_second=3 -> Second wins
                // Sample 4: 1 5 6 3 2 4 -> fixed_asc=1, fixed_desc=0, moves_first=5, moves_second=6 -> Tie? Wait output is Tie.
                // moves_first=5, moves_second=6, moves_first < moves_second, but output Tie.
                // So condition is not simply moves_first <= moves_second.
                // Need deeper analysis.

                // Let's recompute sample 4: n=6, p = [1,5,6,3,2,4]
                // fixed_asc: p[0]=1 ok, p[1]=5!=2, p[2]=6!=3, p[3]=3!=4, p[4]=2!=5, p[5]=4!=6 -> fixed_asc=1
                // fixed_desc: p[0]=1!=6, p[1]=5!=5? wait p[1]=5, desc order: 6,5,4,3,2,1 -> index 1 should be 5, yes p[1]=5 ok.
                // p[2]=6 should be 4? desc: index2=4, but p[2]=6 -> no. p[3]=3 should be 3? index3=3, yes p[3]=3 ok.
                // p[4]=2 should be 2? index4=2, yes p[4]=2 ok. p[5]=4 should be 1? no.
                // So fixed_desc: indices 1,3,4 -> fixed_desc=3? Wait check: 
                // desc: [6,5,4,3,2,1]
                // i=0: p[0]=1 !=6
                // i=1: p[1]=5 ==5 ok
                // i=2: p[2]=6 !=4
                // i=3: p[3]=3 ==3 ok
                // i=4: p[4]=2 ==2 ok
                // i=5: p[5]=4 !=1
                // So fixed_desc = 3. moves_second = 6-3 = 3.
                // moves_first = 5, moves_second = 3. moves_first > moves_second, so second should win? But output Tie.
                // So my calculation of fixed_desc is wrong? Let's re-evaluate sample 4 output: Tie.
                // According to sample, it's Tie. So maybe my moves counting is not the whole story.

                // Let's think: The players can rearrange blue elements freely. 
                // The game is about coloring elements that are not in the correct position for their goal.
                // But they can also rearrange blue elements to help or hinder.
                // Actually, the known solution from Codeforces problem "Red and Blue" or similar:
                // The result depends on the number of elements that are already in correct position for each player.
                // Let a = number of elements already in correct position for ascending (first player).
                // Let b = number of elements already in correct position for descending (second player).
                // Let c = number of elements that are in correct position for both? That's impossible because ascending and descending only share positions when n is odd and middle element is same? 
                // For n odd, middle element is (n+1)/2, in both ascending and descending it's the same. So that element can be fixed for both.
                // Let both = number of elements that are correct for both (only possible middle element if n odd and p[mid] = mid).
                // Then first needs to color n - a elements. Second needs to color n - b elements.
                // But they share the both elements? Actually, if an element is correct for both, it's already in position for both, so neither needs to color it.
                // The game: On a turn, a player can color one red element blue, or rearrange blue elements, or skip.
                // The key insight: The first player wants to achieve ascending order. He can only rearrange blue elements. 
                // So he wants to color all elements that are not in ascending position. But he can also use elements that second player colored, if they are blue.
                // However, second player wants descending order. They can color elements that are not in descending position.
                // They can interfere by coloring elements that the opponent needs, but those elements become blue and can be rearranged by either player.
                // Actually, once an element is blue, either player can rearrange it. So coloring an element gives both players the ability to move it.
                // This is a game of "who can finish their set first".
                // Let's denote:
                // x = number of elements that are in correct position for ascending but NOT for descending.
                // y = number of elements that are in correct position for descending but NOT for ascending.
                // z = number of elements that are in correct position for both (only possible middle element).
                // w = number of elements that are in neither correct position.
                // Then a = x + z, b = y + z.
                // First needs to color the elements that are not in ascending position: these are y + w.
                // Second needs to color the elements that are not in descending position: these are x + w.
                // But note: if first colors an element from y, that element was already correct for descending, so second loses a fixed point? 
                // Actually, if an element is colored blue, it can be moved. So if first colors an element that was correct for descending, 
                // second can no longer rely on it staying in place; he would need to color it himself? But it's already blue, so he can also move it.
                // The game is symmetric in some sense.
                // The known solution: 
                // Let only_first = count of positions where p[i] == i+1 but p[i] != n-i.
                // Let only_second = count of positions where p[i] == n-i but p[i] != i+1.
                // Let both = count of positions where p[i] == i+1 and p[i] == n-i.
                // Then first needs to color n - (only_first + both) = only_second + (n - only_first - only_second - both) = only_second + neither.
                // Second needs to color n - (only_second + both) = only_first + neither.
                // Let need_first = only_second + neither.
                // Let need_second = only_first + neither.
                // But note: neither = n - only_first - only_second - both.
                // So need_first = n - only_first - both.
                // need_second = n - only_second - both.
                // This matches earlier moves_first and moves_second.
                // In sample 4: 
                // p = [1,5,6,3,2,4]
                // only_first: i=0: 1==1, 1!=6 -> yes. i=3: 3==4? no. i=4: 2==5? no. So only_first = 1.
                // only_second: i=1: 5==5, 5!=2 -> yes. i=3: 3==3, 3!=4 -> yes. i=4: 2==2, 2!=5 -> yes. So only_second = 3.
                // both: none, since n even.
                // neither = 6 - 1 - 3 - 0 = 2.
                // need_first = 3 + 2 = 5.
                // need_second = 1 + 2 = 3.
                // So need_first=5, need_second=3.
                // According to sample, output Tie. So why doesn't second win with need_second=3 < need_first=5?
                // Because first can also color elements that second needs? But second needs fewer moves.
                // Let's simulate: second needs 3 moves to color his needed elements. First needs 5 moves.
                // Second moves second. If both just color their needed elements, second would finish on his 3rd turn (turn 6 overall? Actually turn order: 1: first, 2: second, 3: first, 4: second, 5: first, 6: second...)
                // Second's turns: 2, 4, 6. So on turn 6, second makes his 3rd move and wins? But first could win earlier? First would need 5 moves, turns 1,3,5,7,9 -> turn 9. So second is faster.
                // But why Tie? Because first can interfere by coloring elements that second needs? But second needs only 3 elements. If first colors one of those, it becomes blue, and second can still use it? Actually, if first colors an element that second needs, that element is now blue. Second still needs to have all elements in descending order. If that element is blue, second can rearrange it. But does second still need to "color" it? The win condition is just that the permutation is sorted descending. The color doesn't matter for winning, only for the ability to rearrange. So if first colors an element that second needs, second doesn't need to color it anymore; it's already blue and can be rearranged by either player. But second wants descending order. If first colors it, first might rearrange it to a wrong position? But second can also rearrange it on his turn. So coloring an element helps both players.
                // The key: A player can only win if all elements are in their correct positions. The game ends immediately when the permutation becomes sorted ascending or descending.
                // If first colors an element that second needs, second's "need" count doesn't decrease because second still needs that element to be in correct position, but he can now move it. However, second could have moved it anyway if he colored it himself. The difference is who spends a turn coloring it.
                // Actually, the standard analysis for this game (Codeforces Round 907 Div.2, Problem C? Or similar) says:
                // Let a = number of elements only in correct position for first.
                // Let b = number of elements only in correct position for second.
                // Let c = number of elements in correct position for both.
                // Let d = number of elements in neither.
                // Then first needs to color b + d elements. Second needs to color a + d elements.
                // The game result:
                // If a + d <= b, then first wins? No.
                // The known result from similar problem "C. Game on Permutation" or "Red and Blue"?
                // Actually, this is Codeforces problem "Game on Permutation" from Round 907? Let's search memory.
                // There is a problem: "Two players, permutation, color red/blue, rearrange blue, first wants ascending, second wants descending."
                // The solution: 
                // Let cntA = number of elements that are already in ascending position.
                // Let cntB = number of elements that are already in descending position.
                // Let cntAB = number of elements that are in both (only possible middle element).
                // Then first needs to color n - cntA elements. Second needs n - cntB.
                // But they can share colored elements.
                // The game is determined by comparing the number of "exclusive" needs.
                // Actually, the known condition:
                // If cntA >= n - cntB + 1? No.
                // Let's derive:
                // First can win if he can force the permutation to ascending before second forces descending.
                // The only way to win is to have all elements in your order. You can only rearrange blue elements.
                // So you need to make all elements that are not in your order become blue, and then rearrange them.
                // But if the opponent has already colored some elements, you can use them.
                // So the total number of coloring moves needed by first is the number of elements that are not in ascending order and are still red. Initially all red.
                // First can color elements. Second can also color elements. Both can use any blue elements.
                // So first's goal is to have all elements either already in ascending position or colored blue. 
                // Initially, the set of elements not in ascending position is S1. First needs S1 to become blue.
                // Second needs the set S2 (not in descending position) to become blue.
                // They take turns coloring one red element blue.
                // This is a game where each player wants to complete their set. They can also color elements from the opponent's set, which helps the opponent.
                // So it's a race with shared resources.
                // Let x = |S1 \ S2| = elements not in ascending but in descending = only_second.
                // Let y = |S2 \ S1| = elements not in descending but in ascending = only_first.
                // Let z = |S1 ∩ S2| = elements in neither = neither.
                // Let w = elements in both = both.
                // Note: S1 = only_second ∪ neither. S2 = only_first ∪ neither.
                // First needs to color S1. Second needs to color S2.
                // They can color any red element. If first colors an element in S2 \ S1 (only_first), that element is already in ascending position, so first doesn't need it, but second does. By coloring it, first reduces second's remaining set? Actually, second needs S2 to be blue. If first colors an element in S2, it becomes blue, so second no longer needs to color it. So first can help second by coloring second's needed elements. But first wants to win, so he wouldn't want to help second unless forced.
                // This is a classic impartial game? It's a partisan game.
                // The known solution from similar problems (e.g., Codeforces 1790E? No.)
                // Let's think: Both players will prioritize coloring elements that are in their own set but not in the opponent's set, because those are exclusive needs. 
                // If a player colors an element from the intersection (neither), it helps both equally.
                // If a player colors an element from the opponent's exclusive set, it helps the opponent and not themselves.
                // So optimal play: Each player will first color their exclusive elements (only_first for second? Wait: first's exclusive needs are only_second. Second's exclusive needs are only_first.)
                // Actually, first needs S1 = only_second ∪ neither. His exclusive part is only_second. 
                // Second needs S2 = only_first ∪ neither. His exclusive part is only_first.
                // So first wants to color only_second, second wants to color only_first.
                // They will avoid coloring neither because it helps both equally, and they will avoid coloring the opponent's exclusive because it helps opponent.
                // But if a player runs out of exclusive elements, they might have to color neither or opponent's exclusive.
                // The game ends when one player's needed set is fully blue. Then that player can rearrange and win (unless the other player already won).
                // Since rearrangement is free and can be done on the same turn as coloring? The rules: "On their turn, the player can do one of three actions: rearrange...; change color...; skip." So a player cannot color and rearrange on the same turn. They must spend a turn to rearrange. But they can rearrange at any time, including when the needed set is already blue. So if after some coloring moves, the needed set is blue, the player can on their next turn rearrange and win, unless the opponent wins on their turn.
                // So the game is about who completes their set first, considering turn order.
                // Let's denote:
                // a = only_first (first's already correct, second's not)
                // b = only_second (second's already correct, first's not)
                // c = neither
                // d = both
                // First needs to color b + c elements.
                // Second needs to color a + c elements.
                // First will color b elements first (his exclusive needs). Second will color a elements first.
                // After b and a are exhausted, they will color c elements.
                // The number of turns each needs:
                // First needs b + c colors. Second needs a + c colors.
                // But they share the c colors. So total colors needed = a + b + c? Actually, if they both need c, they can share the coloring of c. Once a c element is colored, it counts for both.
                // So the total number of coloring moves to satisfy both is max(a, b) + c? Not exactly.
                // Let's simulate with optimal play:
                // Both players will first color their exclusive elements. 
                // First colors from b, second colors from a.
                // They will do this until one player's exclusive set is empty. 
                // Suppose b <= a. Then first will finish his exclusive b elements after b turns (but he goes first, so turn 1,3,...,2b-1). Second will have colored b elements from a as well (turns 2,4,...,2b). After 2b turns, first has no exclusive left, second still has a - b exclusive left.
                // Now first must either color from c or from second's exclusive (a). If he colors from a, he helps second. If he colors from c, he helps both. He wants to delay second's win. So he will color from c, because coloring from a reduces second's remaining count directly. Actually, coloring from a reduces second's needed count by 1, while coloring from c reduces both by 1. Since first is behind (he needs c more, while second needs (a-b) + c), first wants to minimize the advantage. 
                // This is a known game: "The player who needs fewer exclusive elements has an advantage."
                // The result can be determined by comparing a and b.
                // Let's look at sample 4: a = only_first = 1, b = only_second = 3, c = neither = 2.
                // a=1, b=3. So first has more exclusive needs (b=3) than second (a=1).
                // Second has fewer exclusive needs. So second should win? But output Tie.
                // Let's simulate sample 4 with optimal play:
                // n=6, p=[1,5,6,3,2,4]
                // a=1 (element 1 at pos0), b=3 (elements 5 at pos1, 3 at pos3, 2 at pos4), c=2 (elements 6 at pos2, 4 at pos5).
                // First needs b+c=5, second needs a+c=3.
                // Turn 1 (First): He will color one of his exclusive b. Say he colors element 5 (pos1) blue.
                // Turn 2 (Second): He will color his exclusive a. He colors element 1 (pos0) blue. Now a=0.
                // Turn 3 (First): He still has b=2 left. He colors another exclusive, say element 3 (pos3) blue.
                // Turn 4 (Second): He has no exclusive left (a=0). He must color from c or from b. If he colors from c, he helps both. If he colors from b, he helps first. He wants to win, so he should color from c, because that reduces his needed count without reducing first's needed count as much? Actually, both need c. If he colors a c element, his needed count decreases by 1, first's needed count also decreases by 1. But first still needs b=1 more exclusive. So after coloring c, second's needed = c-1 + 0 = c-1, first's needed = b + c-1. Second is still ahead. If he colors from b, first's needed decreases by 1, second's needed stays same. That's worse for second. So second colors from c. He colors element 6 (pos2) blue.
                // Turn 5 (First): He has b=1 left. He colors his last exclusive, element 2 (pos4) blue. Now b=0.
                // Turn 6 (Second): Now both have no exclusive. c=1 left (element 4 at pos5). Second needs c=1, first needs c=1. Second colors the last c element blue.
                // Now all needed elements are blue. The permutation currently: all elements are blue? Actually, we colored: 5,1,3,6,2,4. All elements are blue. 
                // On turn 6, second just colored. Can he rearrange and win? The rules: On their turn, they can do ONE action. He chose to color. He cannot also rearrange. So he must wait until his next turn to rearrange. 
                // Turn 7 (First): Now first gets a turn. All elements are blue. First can rearrange to ascending order and win! 
                // So first wins on turn 7. But wait, second could have rearranged on turn 6 instead of coloring? Let's check: On turn 4, second colored c. On turn 5, first colored last b. On turn 6, second's turn. At the start of turn 6, what is the state? 
                // After turn 5: colored: 5,1,3,6,2. Only element 4 is red. 
                // Second needs descending order. The elements needed for descending: a=0, c=1 (element 4). So second still needs element 4 to be blue to be able to rearrange it. But can second win without coloring element 4? He can only rearrange blue elements. Element 4 is red, so it stays in position 5 (value 4). For descending order, position 5 should be 1. So element 4 is in the wrong place. He cannot move it because it's red. So he cannot win by rearranging on turn 6; he must color element 4 or skip. If he colors element 4, he uses his turn, then first wins on turn 7. If he skips, first will color element 4 on turn 7? But first can also rearrange on turn 7 if all needed are blue? Wait, after turn 5, first's needed: b=0, c=1 (element 4). So first also needs element 4 blue. So on turn 6, second must do something. If second colors element 4, then all blue, but turn passes to first who rearranges and wins. If second skips, first on turn 7 can color element 4 and then cannot rearrange same turn, so second would get turn 8 and could rearrange and win? Let's see:
                // Turn 6: Second skips.
                // Turn 7: First colors element 4. Now all blue. First cannot rearrange because he used his action to color.
                // Turn 8: Second can now rearrange to descending and win.
                // So second would win if he skips on turn 6! But wait, can second rearrange on turn 6? No, because element 4 is red. So he cannot win by rearranging. He can skip. If he skips, first on turn 7 can either color element 4 or rearrange? First cannot rearrange because element 4 is red and needed for ascending? Ascending: position 5 should be 6, but element 4 is there. So first also cannot win by rearranging. So first must color element 4. Then second wins on turn 8. So second can force a win by skipping on turn 6? But then first on turn 7 might also skip? If both skip, game goes on. But first wants to win, so he would color element 4, giving second the win. So first would not color element 4; he might skip as well. Then it's a draw if both keep skipping? But the game ends after 100^500 turns as draw. So optimal play: second can force a win if he skips? But wait, if second skips on turn 6, first on turn 7 can rearrange? No, element 4 is red, so first cannot rearrange to ascending because element 4 is not in correct place and cannot be moved. So first cannot win. First can color element 4, then second wins. First can skip. If both skip forever, it's a draw. But second wants to win. Can second force a win? He could on turn 6 color element 4 and then first wins, which is bad. He could skip, then first might color element 4, then second wins. But first, seeing that coloring element 4 leads to second winning, will also skip. So it becomes a draw. So second cannot force a win if first also skips. But can second do something else? On turn 4, second colored c. What if on turn 4, second colored from b instead? Let's explore alternative strategies.

                // This is getting complicated. There is a known solution for this exact problem. I recall it's from Codeforces Round 907 (Div. 2) Problem C: "Game on Permutation"? No, that's different. There's a problem "Red and Blue" from some round. Actually, I think it's Codeforces 1790E? No.
                // Let's search memory: There is a problem "Two Players, Permutation, Red/Blue, Rearrange" — I think it's Codeforces 1787C? No.
                // Wait, I remember a problem: "First player wants ascending, second wants descending. They can color elements blue. Blue elements can be swapped arbitrarily. Determine winner." 
                // The solution: 
                // Let a = number of elements that are in correct position for ascending but not descending.
                // Let b = number of elements that are in correct position for descending but not ascending.
                // Let c = number of elements that are in correct position for both.
                // Let d = number of elements that are in neither.
                // Then if a + d <= b, first wins? No.
                // Actually, the known condition:
                // If a + d < b, then second wins? No.
                // Let's derive from sample 1: 1 2 4 3 -> a: 1,2 correct for asc, not desc? 1 at pos0: asc yes, desc no. 2 at pos1: asc yes, desc no. 4 at pos2: asc no, desc no? desc: pos2 should be 2, so no. 3 at pos3: asc no, desc no? desc: pos3 should be 1, so no. So a=2, b=0, c=0, d=2. a=2, b=0, d=2. Output First.
                // Sample 2: 2 3 1 -> a: none? asc: pos0=2!=1, pos1=3!=2, pos2=1!=3. desc: pos0=2!=3, pos1=3!=2, pos2=1!=1? pos2=1==1, so b=1 (element 1 at pos2). a=0, b=1, c=0, d=2. Output Tie.
                // Sample 3: 3 4 5 2 1 -> asc: none? pos0=3!=1, pos1=4!=2, pos2=5!=3, pos3=2!=4, pos4=1!=5. desc: pos0=3!=5, pos1=4!=4? pos1=4==4, so b=1. pos2=5!=3, pos3=2==2? pos3=2==2, so b=2. pos4=1==1, so b=3. So a=0, b=3, c=0, d=2. Output Second.
                // Sample 4: 1 5 6 3 2 4 -> a=1 (1), b=3 (5,3,2), c=0, d=2. Output Tie.
                // So summarizing:
                // Case 1: a=2, b=0, d=2 -> First
                // Case 2: a=0, b=1, d=2 -> Tie
                // Case 3: a=0, b=3, d=2 -> Second
                // Case 4: a=1, b=3, d=2 -> Tie
                // Let's see if there's a pattern with a, b, d.
                // In case 1: a + d = 4, b = 0 -> First wins when a+d > b? But case 3: a+d=2, b=3 -> Second wins. So maybe if a+d > b, First wins; if b > a+d, Second wins; if a+d == b, Tie? 
                // Check case 2: a+d=2, b=1 -> a+d > b, but output Tie. So not that.
                // Maybe compare a and b directly? Case 1: a=2 > b=0 -> First. Case 3: b=3 > a=0 -> Second. Case 2: a=0, b=1 -> b > a, but Tie. Case 4: a=1, b=3 -> b > a, but Tie.
                // So when a and b are both non-zero? Case 2: a=0, b=1 -> Tie. Case 4: a=1, b=3 -> Tie. So maybe if both a and b are positive, it's Tie? But case 1: a=2, b=0 -> First. Case 3: a=0, b=3 -> Second. So if one of them is zero, the other wins? But wait, case 2: a=0, b=1 -> b is non-zero, a=0, so according to that, Second should win, but it's Tie. So not simply zero.
                // Let's re-examine case 2: n=3, p=[2,3,1]. a=0, b=1, d=2. Why Tie? 
                // Simulate: First needs b+d=3, second needs a+d=2.
                // Turn 1: First must color something. He can color from b (element 1 at pos2) or from d (elements 2,3). If he colors from b, he helps himself. If he colors from d, he helps both. He wants to win, so he colors from b? But b=1, so he colors element 1. Now element 1 is blue.
                // Turn 2: Second needs a+d=2. a=0, so he needs to color both d elements. He colors one d element, say element 2.
                // Turn 3: First needs b=0, d=1 (element 3). He colors element 3. Now all blue.
                // Turn 4: Second can rearrange to descending? Descending order: [3,2,1]. Currently permutation is [2,3,1] with all blue. Second can rearrange to [3,2,1] and win. So second wins? But output Tie. Why? Because first could have rearranged on turn 3? On turn 3, first colored element 3. He cannot rearrange same turn. So second wins on turn 4. So why Tie?
                // Wait, maybe first can win on turn 3 by rearranging instead of coloring? On turn 3, state: after turn 2, colored: element 1 (pos2) and element 2 (pos0). Red: element 3 (pos1). Permutation: [2,3,1] with 2 and 1 blue, 3 red. First wants ascending [1,2,3]. Can he rearrange blue elements to achieve ascending? Blue elements are 2 and 1. Their current positions: 2 at pos0, 1 at pos2. He can swap them: put 1 at pos0, 2 at pos2. Then permutation becomes [1,3,2]. But element 3 is red and at pos1, should be 2 for ascending. So not sorted. He cannot move element 3. So he cannot win by rearranging. So he must color. So second should win. But sample says Tie. So my simulation must be missing something.
                // Let's read the problem statement carefully: "rearrange the elements of the permutation in such a way that all red elements keep their positions (note that blue elements can be swapped with each other, but it's not obligatory);"
                // So when rearranging, red elements stay fixed, blue elements can be permuted arbitrarily among the positions of blue elements? Or can they be placed anywhere? "rearrange the elements of the permutation in such a way that all red elements keep their positions" means the red elements cannot move. The blue elements can be rearranged, meaning they can be placed into any positions, but the red elements' positions are fixed. So the blue elements can be permuted among the positions that are not occupied by red elements? Actually, if red elements keep their positions, then the blue elements must occupy the remaining positions. So you can permute the blue elements arbitrarily among the positions that are currently occupied by blue elements? Or can you move a blue element to a position currently occupied by a red element? No, because red elements keep their positions, so a red element stays in its position. So a blue element cannot take a red element's position. Therefore, the set of positions occupied by blue elements is exactly the set of positions that are not red. So you can only rearrange blue elements among those positions. You cannot move a blue element to a red position. So the red elements act as fixed points.
                // In my simulation, after turn 2: red = {element 3 at pos1}. Blue = {element 2 at pos0, element 1 at pos2}. The blue positions are pos0 and pos2. The red position is pos1. So first can only swap elements at pos0 and pos2. He cannot put a blue element into pos1. So ascending order requires pos1 to be 2. But pos1 is red with element 3. So ascending is impossible until element 3 becomes blue. So first cannot win by rearranging. So he must color element
