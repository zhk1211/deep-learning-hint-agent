// Hint2
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

        // Find longest prefix that matches 1,2,3...
        int pref = 0;
        for (int i = 0; i < n; ++i) {
            if (p[i] == i + 1) {
                ++pref;
            } else {
                break;
            }
        }

        // Find longest suffix that matches n, n-1, ...
        int suff = 0;
        for (int i = n - 1; i >= 0; --i) {
            if (p[i] == n - (n - 1 - i)) { // n - (n-1-i) = i+1? Wait, check: for i=n-1, value should be n; for i=n-2, value n-1.
                // Actually p[i] should equal n - (n-1 - i) = i+1? No, descending: position i from end: last is n, second last n-1...
                // Let's compute: position i (0-indexed). The descending order at i is n - i.
                if (p[i] == n - i) {
                    ++suff;
                } else {
                    break;
                }
            }
        }

        // The elements that are already in correct position for ascending (pref) and descending (suff) are "free" for respective players.
        // The remaining elements are "neutral" initially.
        int neutral = n - pref - suff;

        // First player needs to color all elements that are not in ascending position.
        // Second player needs to color all elements that are not in descending position.
        // But note: elements in pref are already good for First, elements in suff are already good for Second.
        // The game: players take turns coloring one red element blue. They can also rearrange blue elements to win instantly if all needed are blue.
        // The key insight from hints: The winner is the one who can color all "their" required elements first.
        // First needs to color: n - pref elements (all except those already in ascending prefix).
        // Second needs to color: n - suff elements.
        // However, they share some elements: an element might be needed by both if it's in neither pref nor suff.
        // Actually, if an element is in pref, it's already correct for First, so First doesn't need to color it.
        // If it's in suff, it's correct for Second, so Second doesn't need to color it.
        // All other elements (neutral) are needed by both.
        // So First's required count = neutral + suff (since suff elements are not in ascending position).
        // Second's required count = neutral + pref.
        // But careful: The players can also skip turns. The optimal play is a race to color their required elements.
        // Since they alternate, First moves first.
        // Let a = neutral + suff (First's needed), b = neutral + pref (Second's needed).
        // First wins if a <= b? Not exactly, because they take turns.
        // Actually, they can also use the rearrange action to win immediately when all their required elements are blue.
        // So the game is: each turn, a player can color one red element blue (or skip). They want to be the first to have all their required elements blue.
        // This is a classic game: two players, each needs to collect a set of items. Some items are shared (neutral). First needs A items, Second needs B items.
        // First moves first. They can claim one item per turn. They can also claim items the opponent needs (to block). But here, coloring an element blue makes it available for rearrangement for both? Wait: "change the color of one red element to blue" - once blue, it can be rearranged by either player? The rules: "rearrange the elements of the permutation in such a way that all red elements keep their positions (note that blue elements can be swapped with each other, but it's not obligatory)". So blue elements are free to move for both players. So if First colors an element blue, Second can also use it later. So it's not blocking; it's helping both. But the win condition is: First wins if permutation becomes ascending. That requires all elements to be in correct positions. If some elements are still red, they are fixed and might be wrong. So First needs all elements that are not already in ascending position to become blue (so they can be moved to correct spots). Similarly Second needs all elements not in descending position to become blue.
        // So both players want to turn red elements blue. They share the same pool of red elements. They both benefit from any element turning blue. So it's a cooperative race? Actually, they are opponents: each wants to reach their own win condition first. Since turning an element blue helps both, a player will only turn an element blue if it brings them closer to winning without giving the opponent a faster win.
        // This is a classic impartial game? Let's analyze: Let A = set of positions where p[i] != i+1 (First's bad positions). B = set of positions where p[i] != n - i (Second's bad positions). The players need to eliminate all bad positions for themselves. They can eliminate a position by coloring it blue. Once blue, it's no longer bad for either (since it can be rearranged). So they both want to color positions in their respective bad sets. The intersection A ∩ B are positions bad for both. A \ B are bad only for First. B \ A are bad only for Second.
        // Let x = |A \ B|, y = |B \ A|, z = |A ∩ B|.
        // First needs to color all positions in A = x + z.
        // Second needs to color all positions in B = y + z.
        // They alternate coloring one red element blue. They can also skip. They can also rearrange to win instantly if all their bad positions are blue.
        // This is a known game: "Coloring game" from Codeforces. The optimal strategy: each player will prioritize coloring the elements that are only bad for them (x for First, y for Second), because coloring shared elements (z) helps the opponent as much as themselves. So they will first color their exclusive bad elements. After that, they might need to color shared elements.
        // Let's simulate optimal play:
        // First moves first. They will color an element from x if x>0, else from z if z>0, else skip (but if no bad left, they win immediately).
        // Second will similarly color from y if y>0, else from z.
        // The game ends when one player has all their bad elements blue. They can then use rearrange to win.
        // Since they can rearrange instantly, the moment the last required element is colored, that player wins on that turn (or even immediately if it's already their turn? Actually, the action of coloring makes it blue, then they could rearrange as part of the same turn? The rules: "On their turn, the player can do one of three actions". So they can only do one action per turn. So if they color the last needed element, they cannot also rearrange on the same turn. The opponent then gets a turn and might rearrange and win? But wait: after coloring, the element is blue. The opponent can then on their turn use rearrange to win if all their needed elements are blue. So it's a race: if First colors the last element they need, they end their turn. Then it's Second's turn. If Second also has all their needed elements blue, they can rearrange and win. So First only wins if after their coloring, they have all needed blue AND Second does NOT have all needed blue. Otherwise, if both have all needed, the player whose turn it is next wins? Actually, if both have all needed, the next player can rearrange and win. So First wants to reach a state where they have all needed but Second doesn't, and then it's Second's turn (so Second cannot win immediately). Or First could win by rearranging on their turn if they already had all needed at the start of their turn. But initially, no blue elements, so they need to color.
        // This is a well-known problem: "First" wins if x + z <= y? Let's derive.
        // Consider the counts. First needs x+z, Second needs y+z.
        // First will color x's, Second will color y's. They will avoid coloring z's as long as possible because it helps the opponent.
        // So the game proceeds: First colors an x if available, else z. Second colors a y if available, else z.
        // Let's simulate turns:
        // Turn 1 (First): if x>0, x--; else if z>0, z--; else First wins (already all blue? but initially no blue, so impossible).
        // Turn 2 (Second): if y>0, y--; else if z>0, z--; else Second wins.
        // Turn 3 (First): if x>0, x--; else if z>0, z--; else First wins.
        // ...
        // The game ends when one player's required count becomes 0. But note: after a player's turn, if their required count is 0, they cannot win immediately because they used their action to color. They must wait until their next turn to use rearrange. However, if their required count becomes 0 during the opponent's turn? That can't happen because only the player whose turn it is changes the state. So a player's required count can only become 0 on their own turn (by coloring the last needed element). Then the opponent gets a turn. If the opponent also has required count 0, they win. Otherwise, the first player will win on their next turn.
        // So effectively, the player who first reduces their required count to 0 wins, UNLESS the opponent's required count is also 0 at that moment, in which case the opponent wins on their next turn.
        // Let's define a = x+z (First's needed), b = y+z (Second's needed).
        // First wins if a < b? Let's test with examples.
        // Example 1: 4, p = [1,2,4,3]. n=4.
        // Ascending: 1,2,3,4. Descending: 4,3,2,1.
        // pref: p[0]=1 ok, p[1]=2 ok, p[2]=4 !=3 -> pref=2.
        // suff: p[3]=3 !=1? Wait descending: p[3] should be 1, but it's 3. p[2] should be 2, but it's 4. So suff=0.
        // neutral = n - pref - suff = 4-2-0=2.
        // x = |A\B|: A = positions where p[i]!=i+1: indices 2,3 (values 4,3). B = positions where p[i]!=n-i: n-i: index0:4, index1:3, index2:2, index3:1. p[0]=1!=4 -> bad; p[1]=2!=3 -> bad; p[2]=4!=2 -> bad; p[3]=3!=1 -> bad. So B = all indices. A = {2,3}. A\B = empty. x=0.
        // B\A = {0,1} -> y=2.
        // z = A∩B = {2,3} -> z=2.
        // a = x+z = 2, b = y+z = 4.
        // First needs 2, Second needs 4. First wins? Output says First. So a < b gives First.
        // Example 2: 3, [2,3,1]. n=3.
        // Asc: 1,2,3. Desc: 3,2,1.
        // pref: p[0]=2!=1 -> 0.
        // suff: p[2]=1!=1? Wait descending: p[2] should be 1, it is 1. p[1] should be 2, it is 3!=2 -> suff=1.
        // neutral = 3-0-1=2.
        // A: p[0]=2!=1 bad; p[1]=3!=2 bad; p[2]=1!=3 bad -> all bad. B: p[0]=2!=3 bad; p[1]=3!=2 bad; p[2]=1!=1 good? Wait p[2]=1, n-i=1, so good. So B = {0,1}. A = {0,1,2}. A\B = {2} -> x=1. B\A = {} -> y=0. z = A∩B = {0,1} -> z=2.
        // a = 1+2=3, b = 0+2=2. a > b. Output: Tie. So not simply a < b.
        // Let's simulate: First needs 3, Second needs 2.
        // Turn 1: First has x=1, so colors an x (index 2). Now x=0, z=2. First's needed = 0+2=2. Second's needed = 0+2=2.
        // Turn 2: Second has y=0, so must color z. z=1. Second's needed becomes 1. First's needed still 2.
        // Turn 3: First has x=0, colors z. z=0. First's needed becomes 1. Second's needed is 1.
        // Turn 4: Second has no y, z=0, so Second's needed is 1? Wait after Turn 3, z=0. Second's needed = y+z = 0+0=0? Actually after Turn 2, z=1, Second's needed = 1. After Turn 3, z=0, Second's needed = 0. So at the start of Turn 4, Second's needed is 0. Second can rearrange and win. So Second wins? But output is Tie. Let's re-check: after Turn 3, First colored the last z. First's needed becomes 1 (since x=0, z=0? Wait: initially a=3. After Turn 1: x=0, z=2 -> a=2. After Turn 2: z=1 -> a=2 (First's needed still 2 because z decreased but First also needs z). After Turn 3: z=0 -> a=1? Actually First needs x+z. x=0, z=0 -> a=0? No: First needs all bad positions. Initially bad positions for First: A = {0,1,2}. After coloring index 2 (Turn 1), that position is blue, so no longer bad. Remaining bad for First: {0,1}. After Turn 2, Second colors index 0 or 1 (z). Say index 0. Now blue: {2,0}. Bad for First: {1}. After Turn 3, First colors index 1. Now blue: all. Bad for First: empty. So First's needed becomes 0 at the end of Turn 3. But First used their turn to color, so they cannot rearrange. Turn 4: Second's turn. Second's bad positions: B = {0,1}. After Turn 1: blue {2} (not in B). B still {0,1}. After Turn 2: Second colors index 0 -> blue {2,0}. B becomes {1}. After Turn 3: First colors index 1 -> blue all. B becomes empty. So at start of Turn 4, Second's needed is 0. Second can rearrange and win? But the rules: Second wins if permutation becomes descending. With all blue, Second can rearrange to descending and win. So why output Tie? Let's read problem statement: "If the game lasts for 100^500 turns and nobody wins, it ends in a draw." Maybe players can skip? In this scenario, Second would win on Turn 4. But sample says Tie. Let's re-analyze: Maybe the players can also choose to skip. If First sees that coloring the last z will give Second the win, First might skip instead. Let's see: After Turn 2: blue {2,0}. First's bad: {1}. Second's bad: {1}. It's First's turn (Turn 3). If First colors index 1, then both have all blue, and Second wins next turn. If First skips, then Turn 3: First skips. Turn 4: Second's turn. Second's bad: {1}. If Second colors index 1, then Second's bad becomes empty, but Second used turn to color, so cannot rearrange. Then Turn 5: First's turn. First's bad is also empty now (since index 1 is blue). First can rearrange to ascending and win. So Second would not color index 1; Second would skip. So both skip forever -> Tie. So optimal play: they will not make a move that allows the opponent to win immediately. So the game is about who can force a win without giving the opponent a win.
        // This is a classic impartial game analyzed in Codeforces Round 917 (Div. 2) Problem D? Actually it's Codeforces Round 917 Div. 2 Problem C? No, it's "Game on Permutation" from some contest. The known solution: 
        // Let a = number of elements that are only bad for First (x), b = number of elements only bad for Second (y), c = number of elements bad for both (z).
        // First wins if a + c <= b. Second wins if b + c < a. Otherwise Tie.
        // Let's test with examples:
        // Example 1: a=0, b=2, c=2. a+c=2 <= b=2 -> First wins. Correct.
        // Example 2: a=1, b=0, c=2. a+c=3 <= b=0? No. b+c=2 < a=1? No. So Tie. Correct.
        // Example 3: 5, [3,4,5,2,1]. n=5.
        // Asc: 1,2,3,4,5. Desc: 5,4,3,2,1.
        // pref: p[0]=3!=1 -> 0.
        // suff: p[4]=1!=1? Wait descending: p[4] should be 1, it is 1. p[3] should be 2, it is 2. p[2] should be 3, it is 5!=3 -> suff=2.
        // neutral = 5-0-2=3.
        // A: all indices where p[i]!=i+1: i=0:3!=1 bad; i=1:4!=2 bad; i=2:5!=3 bad; i=3:2!=4 bad; i=4:1!=5 bad -> all bad.
        // B: all indices where p[i]!=n-i: n-i: 5,4,3,2,1. p[0]=3!=5 bad; p[1]=4!=4? p[1]=4, n-1=4 -> good. p[2]=5!=3 bad; p[3]=2!=2 good; p[4]=1!=1 good. So B = {0,2}.
        // A\B = {1,3,4} -> a=3. B\A = {} -> b=0. A∩B = {0,2} -> c=2.
        // a+c=5 <= b=0? No. b+c=2 < a=3? Yes, 2 < 3 -> Second wins. Output: Second. Correct.
        // Example 4: 6, [1,5,6,3,2,4]. n=6.
        // Asc: 1,2,3,4,5,6. Desc: 6,5,4,3,2,1.
        // pref: p[0]=1 ok; p[1]=5!=2 -> pref=1.
        // suff: p[5]=4!=1; p[4]=2!=2? Wait descending: p[5] should be 1, is 4 -> suff=0.
        // neutral = 6-1-0=5.
        // A: p[0]=1 ok; rest bad? p[1]=5!=2 bad; p[2]=6!=3 bad; p[3]=3!=4 bad; p[4]=2!=5 bad; p[5]=4!=6 bad. So A = {1,2,3,4,5}.
        // B: p[i]!=6-i: 6-i: 6,5,4,3,2,1. p[0]=1!=6 bad; p[1]=5!=5? p[1]=5, 6-1=5 -> good; p[2]=6!=4 bad; p[3]=3!=3 good; p[4]=2!=2 good; p[5]=4!=1 bad. So B = {0,2,5}.
        // A\B = {1,3,4} -> a=3. B\A = {0} -> b=1. A∩B = {2,5} -> c=2.
        // a+c=5 <= b=1? No. b+c=3 < a=3? No (3<3 false). So Tie. Output: Tie. Correct.
        // So the condition works.

        // Now we need to compute a, b, c efficiently.
        // We already have pref and suff.
        // But careful: pref is the longest prefix where p[i] == i+1. However, an element could be in correct ascending position but not in the prefix? No, if it's in correct position, it must be part of the prefix if all previous are also correct. Because if any previous is wrong, the prefix stops. So the set of indices where p[i] == i+1 is exactly the first pref indices. Similarly, the set of indices where p[i] == n-i is exactly the last suff indices? Wait: descending correct positions: p[i] == n-i. This is a suffix? For i = n-1, n-i = 1; i = n-2, n-i = 2; ... So the correct descending positions are at the end. But they might not be contiguous if there's a mistake earlier? Actually, if p[i] == n-i, it's correct for descending. But could there be a correct descending element not at the suffix? Suppose n=4, descending is [4,3,2,1]. If p = [2,3,4,1], then p[1]=3 which is n-1=3, so correct, but p[0]=2 !=4, so pref=0, suff? p[3]=1 correct, p[2]=4 !=2, so suff=1. The correct descending elements are at indices 1 and 3. They are not contiguous. So our simple pref/suff calculation only finds the longest prefix of ascending and longest suffix of descending. But the sets of correct positions for ascending and descending might not be contiguous. However, the problem hints and typical solution use exactly the longest prefix and suffix. Let's check if the sets of correct positions are always exactly the prefix for ascending and suffix for descending? No, as shown above, there can be scattered correct positions. But wait: the permutation is not sorted in either order initially. But could there be a correct ascending element after a wrong one? Yes, e.g., [2,1,3] -> p[2]=3 correct. But then pref=0. The set of correct ascending positions is {2}. But our formula using pref and suff might be different. Let's test with such a permutation: n=3, p=[2,1,3]. Asc: 1,2,3. Desc: 3,2,1.
        // pref: p[0]=2!=1 -> 0.
        // suff: p[2]=3!=1 -> 0.
        // neutral = 3.
        // A: p[0]=2!=1 bad; p[1]=1!=2 bad; p[2]=3=3 good -> A={0,1}.
        // B: p[0]=2!=3 bad; p[1]=1!=2 bad; p[2]=3!=1 bad -> B={0,1,2}.
        // a = A\B = {} =0. b = B\A = {2} =1. c = A∩B = {0,1} =2.
        // a+c=2 <= b=1? No. b+c=3 < a=0? No. Tie.
        // Let's simulate: First needs {0,1}, Second needs {0,1,2}. First will color 0 or 1. Second will color 2 (only in B\A). Then First colors the other. Then both have all? Actually after First colors one, Second colors 2, First colors the last, then First's needed 0, Second's needed 0? Second needs {0,1,2}. After First colors 0, blue {0}. Second colors 2, blue {0,2}. First colors 1, blue all. First's needed 0, Second's needed 0. Turn 4: Second can rearrange and win? But wait, after First's second turn, it's Second's turn. Second can win. So Second wins? But our formula gave Tie. Let's check carefully: Turn 1: First colors 0 (or 1). Now First's bad: {1}. Second's bad: {1,2}. Turn 2: Second can color 2 (only in B\A). Now Second's bad: {1}. First's bad: {1}. Turn 3: First must color 1? If First colors 1, then First's bad empty, Second's bad empty. Turn 4: Second wins. So First would not color 1; First would skip. Then Turn 4: Second's turn. Second's bad: {1}. If Second colors 1, then Second's bad empty, but First's bad also empty? First's bad was {1} before; after Second colors 1, First's bad becomes empty. Then Turn 5: First can win. So Second would skip. So both skip -> Tie. So Tie is correct. Our formula works.

        // But what if there is a correct ascending element not in prefix? Does it affect the counts? In the formula, we need to correctly compute a = number of elements that are bad for First but good for Second, b = bad for Second but good for First, c = bad for both.
        // Notice: an element good for First means p[i] == i+1. Good for Second means p[i] == n-i.
        // So we can just iterate and count:
        // a = count of i where p[i] != i+1 and p[i] == n-i.
        // b = count of i where p[i] == i+1 and p[i] != n-i.
        // c = count of i where p[i] != i+1 and p[i] != n-i.
        // The remaining are good for both? But can an element be good for both? That would require i+1 == n-i => 2i = n-1 => n odd, i = (n-1)/2. So the middle element can be good for both if it's in correct position. In that case, it's not bad for either, so it doesn't affect a,b,c. Our counts above correctly ignore it.
        // Let's test with n=3, p=[2,1,3] using these direct counts:
        // i=0: p=2, i+1=1, n-i=3 -> !=1, !=3 -> c++.
        // i=1: p=1, i+1=2, n-i=2 -> !=2, ==2 -> a++.
        // i=2: p=3, i+1=3, n-i=1 -> ==3, !=1 -> b++.
        // a=1, b=1, c=1.
        // a+c=2 <= b=1? No. b+c=2 < a=1? No. Tie. Same result.
        // But earlier with pref/suff we got a=0,b=1,c=2. Different counts but same result? Let's check condition: a+c=2, b=1 -> 2<=1 false; b+c=3, a=0 -> 3<0 false -> Tie. Both give Tie. But are the conditions equivalent? The known solution uses exactly the direct counts a,b,c as defined above (bad only for First, bad only for Second, bad for both). The pref/suff method is a shortcut that works when the correct positions are exactly prefix and suffix? Actually, in many Codeforces solutions, they compute:
        // cntA = number of positions where p[i] == i+1 and p[i] != n-i (good for First, bad for Second) -> this is b.
        // cntB = number of positions where p[i] != i+1 and p[i] == n-i (bad for First, good for Second) -> this is a.
        // cntC = number of positions where p[i] != i+1 and p[i] != n-i (bad for both) -> c.
        // Then if cntB + cntC <= cntA -> First wins; else if cntA + cntC < cntB -> Second wins; else Tie.
        // Note: cntB is a, cntA is b. So condition: a + c <= b -> First; b + c < a -> Second; else Tie.
        // This matches our direct counts.

        // Let's verify with Example 1 using direct counts:
        // n=4, p=[1,2,4,3]
        // i=0: p=1, i+1=1, n-i=4 -> good First, bad Second -> b++.
        // i=1: p=2, i+1=2, n-i=3 -> good First, bad Second -> b++.
        // i=2: p=4, i+1=3, n-i=2 -> bad both -> c++.
        // i=3: p=3, i+1=4, n-i=1 -> bad both -> c++.
        // a=0, b=2, c=2. a+c=2 <= b=2 -> First. Correct.

        // Example 2: [2,3,1]
        // i=0: p=2, i+1=1, n-i=3 -> bad both -> c++.
        // i=1: p=3, i+1=2, n-i=2 -> bad First, good Second -> a++.
        // i=2: p=1, i+1=3, n-i=1 -> good First, bad Second -> b++.
        // a=1, b=1, c=1. a+c=2 <= b=1? No. b+c=2 < a=1? No. Tie. Correct.

        // Example 3: [3,4,5,2,1]
        // i=0: p=3, i+1=1, n-i=5 -> bad both -> c.
        // i=1: p=4, i+1=2, n-i=4 -> bad First, good Second -> a.
        // i=2: p=5, i+1=3, n-i=3 -> bad both -> c.
        // i=3: p=2, i+1=4, n-i=2 -> bad First, good Second -> a.
        // i=4: p=1, i+1=5, n-i=1 -> good First, bad Second -> b.
        // a=2, b=1, c=2. a+c=4 <= b=1? No. b+c=3 < a=2? No? Wait 3 < 2 false. But earlier we got Second wins. Let's recompute carefully:
        // i=0: 3 !=1, !=5 -> c.
        // i=1: 4 !=2, ==4 -> a.
        // i=2: 5 !=3, !=3? 5 !=3 and n-i=5-2=3? Actually n=5, n-i=5-2=3. 5!=3 -> bad both? Wait: p[2]=5, i+1=3, n-i=3. So p[2] !=3 and !=3 -> bad both -> c.
        // i=3: p[3]=2, i+1=4, n-i=2. p[3]=2 !=4, ==2 -> a.
        // i=4: p[4]=1, i+1=5, n-i=1. p[4]=1 !=5? 1!=5, ==1 -> good Second? Actually good for Second means p[i] == n-i. n-i=1, p[4]=1 -> good Second. Bad for First means p[i] != i+1. i+1=5, 1!=5 -> bad First. So this is a (bad First, good Second). Wait, earlier I said b. Let's correct:
        // Good for First: p[i] == i+1.
        // Good for Second: p[i] == n-i.
        // i=4: p=1, i+1=5 -> bad First. n-i=1 -> good Second. So this is a (bad First, good Second).
        // So a count: i=1, i=3, i=4 -> a=3.
        // b count: good First, bad Second: none? Check i=0: bad both; i=1: a; i=2: c; i=3: a; i=4: a. So b=0.
        // c count: i=0, i=2 -> c=2.
        // So a=3, b=0, c=2. a+c=5 <= b=0? No. b+c=2 < a=3? Yes, 2<3 -> Second wins. Correct. My earlier manual count missed i=4 as a.

        // So the direct counting method is correct and simple. It avoids the prefix/suffix confusion.

        // Algorithm:
        // For each test case:
        // Read n, array p (1-indexed values).
        // Initialize a=0, b=0, c=0.
        // For i from 0 to n-1:
        //   bool goodFirst = (p[i] == i+1);
        //   bool goodSecond = (p[i] == n - i);
        //   if (!goodFirst && goodSecond) a++;
        //   else if (goodFirst && !goodSecond) b++;
        //   else if (!goodFirst && !goodSecond) c++;
        //   // if both good, ignore.
        // Then apply conditions:
        // if (a + c <= b) cout << "First\n";
        // else if (b + c < a) cout << "Second\n";
        // else cout << "Tie\n";

        // Complexity: O(n) per test case, total O(sum n) <= 5e5. Fast enough.

        // Let's double-check with a case where both good: n=3, p=[2,2,2] not permutation. For permutation, both good only possible at middle if n odd and p[mid] == mid+1 == n-mid. Example: n=3, p=[1,2,3] but that's sorted ascending, not allowed by input. n=3, p=[3,2,1] sorted descending, not allowed. So input guarantees not both sorted, but could there be a permutation with middle element correct for both while others are wrong? e.g., n=3, p=[2,1,3]? middle i=1: p=1, i+1=2, n-i=2 -> both bad. n=5, p=[2,1,3,5,4]? i=2: p=3, i+1=3, n-i=3 -> both good. This permutation is not sorted ascending or descending. So it's possible. Our code ignores both good, which is correct because it's not bad for either.

        // Test this case: n=5, p=[2,1,3,5,4]
        // i=0: p=2, i+1=1, n-i=5 -> c.
        // i=1: p=1, i+1=2, n-i=4 -> c.
        // i=2: p=3, i+1=3, n-i=3 -> both good, ignore.
        // i=3: p=5, i+1=4, n-i=2 -> c.
        // i=4: p=4, i+1=5
