#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, k;
    cin >> n >> k;
    
    // The greedy algorithm always takes at least n turns (each pair discarded separately)
    // and at most 2n-1 turns (worst case: all pairs interleaved)
    if (k < n || k > 2 * n - 1) {
        cout << "NO\n";
        return;
    }
    
    // We need to construct a sequence of length 2n with each number 1..n exactly twice
    // such that the greedy algorithm takes exactly k turns.
    
    // The greedy algorithm works as follows:
    // - It maintains a set of "seen" numbers (flipped once, not yet matched).
    // - If there is a pair in the seen set, it matches them immediately (1 turn).
    // - Otherwise, it flips the first unseen card, then:
    //   * If that number is in the seen set, it matches with the seen one (1 turn).
    //   * Otherwise, it flips the next unseen card. If they match, they are discarded (1 turn).
    //     If not, both become "seen" (this turn ends without discarding, but the algorithm
    //     discards exactly two cards per turn? Wait, the problem says "In each turn, you flip exactly two cards.
    //     If the two cards have the same number, you discard the two cards. Otherwise, you flip them back."
    //     So a turn always flips two cards, and discards them only if they match.
    //     But the greedy algorithm description: "If there are two cards that you have flipped previously and have the same number, flip those two cards." -> this discards them.
    //     "Otherwise, flip the first card that you have never flipped so far as the first one... Afterwards, if there is another card that you have flipped previously and has the number x, flip that card." -> this discards them.
    //     "Otherwise, flip the first card that you have never flipped so far as the second one." -> if they match, discard; else flip back.
    // So in every turn, exactly two cards are flipped. If they match, they are discarded. If not, they are flipped back and become "seen" (flipped previously).
    // The algorithm ends when all cards are discarded. The number of turns is the number of times we flip two cards.
    // Since we discard exactly two cards per turn (either immediately or later when matched), total turns = number of discarding events.
    // Actually, each turn discards either 0 or 2 cards. But eventually all 2n cards are discarded, so total discarding events = n (each pair discarded once).
    // However, a turn may discard 0 cards (when two different cards are flipped and become seen). So total turns = n + (number of turns where 0 cards discarded).
    // Let's call a turn "bad" if it flips two different unseen cards (so 0 discarded, both become seen).
    // Let b = number of bad turns. Then total turns = n + b.
    // We need total turns = k, so b = k - n.
    // b can range from 0 to n-1. (If b = n, then we would have 2n seen cards but no discarding? Actually maximum b is n-1 because after n-1 bad turns we have 2n-2 seen cards, then the next turn must match something.)
    // So k must be between n and 2n-1 inclusive. Already checked.
    
    // Now we need to construct a sequence that yields exactly b bad turns.
    // Let's analyze the greedy algorithm on a sequence.
    // We process cards from left to right. We maintain a set S of seen numbers (flipped once, waiting for match).
    // At each step, we look at the first card not yet flipped (or discarded). Let its position be i.
    // If there is a pair in S (i.e., two cards with same number in S), we match them: discard both, remove from S. This is 1 turn, 0 bad.
    // Else, we flip card i. Let its number be x.
    // If x is in S, we match it with the seen one: discard both, remove x from S. 1 turn, 0 bad.
    // Else, we flip the next unseen card j. Let its number be y.
    // If x == y, we discard both. 1 turn, 0 bad.
    // Else, we add both x and y to S. 1 turn, 1 bad.
    
    // So bad turns happen exactly when we flip two consecutive unseen cards with different numbers, and neither was in S.
    // To get b bad turns, we need exactly b such events.
    
    // We can design the sequence to control this.
    // Idea: place numbers in a pattern that forces bad turns at the beginning, then good turns later.
    // For example, to get b bad turns, we can put b+1 distinct numbers interleaved in a way that the first 2b cards are all distinct and cause b bad turns.
    // Let's try: sequence: 1, 2, 3, ..., b+1, then their second copies in some order?
    // Actually, we need exactly two copies of each number 1..n.
    // Suppose we want b bad turns. We can use numbers 1..b+1 for the bad part, and the rest for good part.
    // Consider: a1, a2, ..., a_{2b} are the first 2b cards. They should be all distinct? No, we need pairs eventually.
    // Let's simulate: start with empty S.
    // Turn 1: flip first unseen (pos1), number x1. Not in S. Flip next unseen (pos2), number x2. If x1 != x2, bad turn, add both to S.
    // Turn 2: S has {x1, x2}. No pair in S (since x1 != x2). Flip first unseen (pos3). If pos3's number is in S, we match and discard (good turn). But we want another bad turn, so pos3 should not be in S. So pos3 must be a new number x3. Then flip pos4, also new number x4 != x3. Bad turn, add both to S.
    // So to get b bad turns, we need the first 2b cards to be all distinct numbers? But we only have two copies of each number. So we can have at most b distinct numbers in the first 2b cards if each appears twice? Wait, if they are all distinct, we need 2b distinct numbers, but we only have numbers 1..n. So we need n >= 2b? But b <= n-1, so 2b could be up to 2n-2, which is > n for n>2. So we cannot have all distinct.
    // We need to reuse numbers but in a way that they don't match immediately.
    // The key: a bad turn happens when we flip two unseen cards with different numbers, and neither is in S.
    // If we flip a card whose number is already in S, it matches and discards (good turn).
    // So to delay matching, we must ensure that when we flip a card, its counterpart is not in S yet.
    // We can achieve b bad turns by having b "pairs" of numbers that are separated such that their first occurrences cause bad turns, and their second occurrences cause good turns later.
    // Actually, each bad turn introduces two new numbers into S. These numbers will later be matched in good turns (each match removes two from S, but note that a match can happen between a newly flipped card and one in S, or between two cards in S).
    // The total number of cards is 2n. The process ends when S is empty and all cards discarded.
    // Let's think in terms of a sequence of operations.
    // We can design the sequence as follows:
    // For b bad turns, we need 2b cards that are the first occurrences of b pairs? Wait, each bad turn flips two unseen cards, both become seen. So after b bad turns, we have 2b seen cards (all distinct numbers? Not necessarily distinct, but if any two were the same, they would have been matched immediately when the second was flipped? Actually, if we flip a card and its number is already in S, we match it immediately, so that turn would not be bad. So in a bad turn, both flipped cards must have numbers not currently in S. Therefore, after b bad turns, S contains 2b cards, and all their numbers are distinct. So we need at least 2b distinct numbers to have b bad turns. But we only have n distinct numbers total. So we must have 2b <= n, i.e., b <= n/2. But k can be up to 2n-1, so b = k-n can be up to n-1. For n=3, k=5 gives b=2, but 2b=4 > n=3. So the sample 4 has n=3, k=5, output YES with sequence 1 2 3 1 2 3. Let's simulate that sequence to see how it gets b=2 bad turns.
    // Sequence: 1 2 3 1 2 3
    // Turn 1: flip 1, flip 2 (different, both unseen) -> bad, S={1,2}
    // Turn 2: S has 1,2 (no pair). Flip first unseen: 3. 3 not in S. Flip next unseen: 1. 1 is in S! So we match 1 with the seen 1? Wait, algorithm: "Afterwards, if there is another card that you have flipped previously and has the number x, flip that card." Here x=3, no seen 3. So we flip next unseen: 1. But 1 is in S. However, the algorithm says: "Otherwise, flip the first card that you have never flipped so far as the second one." It doesn't check if the second card's number is in S. It just flips the next unseen card. Then after flipping both, if they have the same number, discard; else flip back. Here first=3, second=1, different, so they are flipped back. But wait, 1 was already seen! The algorithm doesn't care; it just flips the next unseen card. So this turn: flip 3 (unseen), then flip 1 (unseen? But 1 was flipped in turn 1, so it is not "never flipped so far". The rule: "flip the first card that you have never flipped so far as the second one." In turn 2, the cards never flipped so far are positions 3,4,5,6 (values 3,1,2,3). So the first never-flipped is position 3 (value 3). Then for the second card, we flip the first never-flipped card including in this turn? "including in this turn" means we consider cards not yet flipped in this turn or before. So after flipping position 3, the never-flipped cards are positions 4,5,6. So we flip position 4 (value 1). So we flipped 3 and 1. They are different, so turn ends, both become seen. Now S was {1,2} from turn 1. After turn 2, we add 3 and 1? But 1 is already in S! So S would have two 1s? That would mean a pair in S. But the algorithm says: "If there are two cards that you have flipped previously and have the same number, flip those two cards." At the start of turn 2, S had 1 and 2 (distinct). After turn 2, we have flipped 1 again, so now there are two 1s in the flipped-previously set. So at the start of turn 3, the algorithm will see two 1s and flip them immediately. So turn 3: flip the two 1s (positions 1 and 4? Actually the two 1s are at positions 1 and 4). They match, discarded. Then S becomes {2,3}. Turn 4: S has 2 and 3 (no pair). Flip first unseen: position 5 (value 2). 2 is in S, so we match with the seen 2? Wait, algorithm: "Afterwards, if there is another card that you have flipped previously and has the number x, flip that card." Here x=2, and there is a seen 2 (position 2). So we flip that seen 2. They match, discarded. S becomes {3}. Turn 5: S has {3}. No pair. Flip first unseen: position 6 (value 3). 3 is in S, match with seen 3. Discard. Done. Total turns = 5. Bad turns: turn 1 and turn 2? Turn 1 was bad (flipped 1 and 2, both unseen, different). Turn 2 flipped 3 and 1: 3 was unseen, 1 was already seen? But the rule for bad turn: we flipped two cards, they were different, and we didn't discard them. So turn 2 also resulted in no discard. So that's a bad turn too. So b=2. But note that in turn 2, the second card (1) was already in S. So the condition "neither is in S" is not necessary for a bad turn. A turn is bad if the two flipped cards are different, regardless of whether they are in S or not. The only good turns are when the two flipped cards match (either because we found a pair in S, or we flipped a card and its match was in S and we flipped that match, or we flipped two unseen cards that happen to be the same number). So a turn is good iff the two cards have the same number. Bad iff they have different numbers.
    // So b = number of turns where the two flipped cards have different numbers.
    // In the greedy algorithm, we always flip two cards. They are discarded only if they match. So total turns = number of times we flip two cards. Since we eventually discard all pairs, the number of good turns (matching) is exactly n (each pair discarded in one turn). The number of bad turns is total turns - n.
    // So we need exactly b = k - n bad turns.
    // Now, how to construct a sequence with exactly b bad turns?
    // Let's analyze the process differently. The algorithm can be seen as: we scan the sequence from left to right. We maintain a set of "open" cards (seen once, waiting for match). When we see a card, if its number is open, we close it (good turn, discards both). If not, we open it. But we also have the rule: if there are two open cards with the same number, we close them immediately (good turn). This is essentially the same as matching parentheses? Not exactly.
    // Actually, the algorithm is equivalent to: we process cards in order. We maintain a stack? No, because we can match any two open cards of the same number, not necessarily the most recent.
    // Let's think about the sequence of operations. Each turn we flip two cards. The first card is always the first unseen card. The second card is either:
    // - a card that matches the first (if its number is open), or
    // - if there is a pair of open cards with the same number, we flip those two (but this happens at the start of a turn, before flipping the first unseen). So this is a separate case.
    // So the algorithm prioritizes matching open pairs.
    // This is similar to a greedy matching algorithm on a sequence where we can remember seen numbers.
    // We can construct the sequence by deciding the order of discarding pairs.
    // Each pair of same number will be discarded in some turn. The turn when they are discarded depends on when their second occurrence is flipped relative to other cards.
    // Let's denote the two copies of number i as L_i and R_i (left and right).
    // The algorithm will flip L_i at some point. If at that time, the other copy R_i is not yet flipped, L_i becomes open. Later, when R_i is flipped, if L_i is still open, they match immediately (good turn). If L_i was already matched with something else? No, L_i can only be matched with R_i. So a number is open from the time its first copy is flipped until its second copy is flipped.
    // However, there is the rule: if two open cards have the same number, they are matched immediately. But since each number appears exactly twice, this can only happen if both copies are open? That's impossible because they are the same number; if both are open, they would be matched immediately when the second becomes open. So actually, the "pair in S" rule only applies if there are two different numbers that each have both copies open? No, S contains cards that have been flipped once and not yet matched. Since each number has exactly two copies, a number can appear in S at most once (because if both copies were in S, they would be the same number, and the algorithm would have matched them immediately when the second was flipped). Wait, could both copies be in S? Suppose we flip the first copy, it goes to S. Later we flip the second copy. According to the algorithm: "Afterwards, if there is another card that you have flipped previously and has the number x, flip that card." So when we flip the second copy, if the first copy is in S, we immediately flip the first copy as the second card of the turn, and they match. So the second copy never goes into S; it's matched immediately. So S never contains two copies of the same number. So the "pair in S" rule (two cards in S with the same number) is actually impossible! Because S only contains at most one copy of each number. So that rule is never triggered? Let's check the sample 1: n=2, sequence 2 1 2 1.
    // Turn 1: flip 2, flip 1 -> different, both become seen. S={2,1}.
    // Turn 2: start. Are there two cards in S with same number? No. Flip first unseen: position 3 (value 2). 2 is in S, so we flip the seen 2 (position 1). They match, discarded. S={1}.
    // Turn 3: S={1}. No pair. Flip first unseen: position 4 (value 1). 1 in S, flip seen 1. Match.
    // So the "pair in S" rule never triggered. Is it ever possible? Suppose S has two copies of the same number. That would require that we flipped both copies without matching them. But the algorithm always matches a copy with its seen counterpart immediately when the second is flipped. So it's impossible. Therefore, the rule "If there are two cards that you have flipped previously and have the same number, flip those two cards" is actually never used! Because you can never have two cards of the same number in the "flipped previously" set without them having been matched already. Wait, could it happen if we flip two cards in one turn that are the same number? That would discard them immediately, so they don't go to S. So indeed, S never contains duplicates. So the algorithm simplifies:
    // - At each turn, flip the first unseen card. Let its number be x.
    // - If x is in S, flip the card in S with number x. They match, discard both, remove x from S.
    // - Else, flip the next unseen card. Let its number be y.
    //   - If x == y, discard both.
    //   - Else, add x and y to S.
    // This is exactly the process! The initial check for pair in S is redundant.
    // So the algorithm is: we scan left to right. We maintain a set S of open numbers. When we encounter a number, if it's in S, we close it (good turn). If not, we look at the next number. If they match, we discard both (good turn). If not, we open both (bad turn).
    // This is equivalent to: we process the sequence in chunks of 2 cards? Not exactly, because after a good turn (matching with S), we don't consume the next card; we just matched with an already open card. The "first unseen card" pointer moves to the next card after the one we just flipped? Actually, when we match with an open card, we flip two cards: the current unseen card and the open card. The open card was already flipped previously, so it's not "unseen". The next turn, the first unseen card is the next card after the one we just flipped. So the pointer only advances by 1 after a match with S? Let's trace:
    // Sequence: a1, a2, a3, ...
    // Turn 1: flip a1. If a1 in S? S empty. So flip a2. If a1==a2, discard both, pointer moves to a3. Else, S={a1,a2}, pointer moves to a3.
    // Turn 2: flip a3. If a3 in S, match with the open a3, discard both, S removes a3, pointer moves to a4. Else, flip a4. If a3==a4, discard, pointer to a5. Else, S adds a3,a4, pointer to a5.
    // So the pointer advances by 2 after a turn that flips two unseen cards (either matching or not). But if we match with an open card, we only advance the pointer by 1 (since we flipped one new card and one old card). So the number of turns is not simply n + b? Let's recalculate.
    // Let U be the number of turns where we flip two unseen cards (both new). In these turns, we consume 2 new cards. If they match, good turn; if not, bad turn.
    // Let V be the number of turns where we flip one new card and match it with an open card. In these turns, we consume 1 new card.
    // Total new cards = 2n. So 2*U + 1*V = 2n.
    // Total turns = U + V.
    // We want total turns = k.
    // Also, each bad turn happens only in U when the two new cards don't match. Let B be number of bad turns. Then B <= U.
    // In a bad turn, we add two new numbers to S. In a good turn of type U (two new cards match), we add 0 to S. In a good turn of type V, we remove 1 from S.
    // Initially S empty, finally S empty. So total additions = total removals.
    // Additions: each bad turn adds 2. Each good turn of type U adds 0. Each good turn of type V adds 0 (it removes 1). So total additions = 2B.
    // Removals: each good turn of type V removes 1. Also, when we have a good turn of type U (two new cards match), we don't remove from S. But wait, what about matching two open cards? That never happens. So removals only happen in type V turns. So total removals = V.
    // Thus 2B = V.
    // From 2U + V = 2n, and V = 2B, we get 2U + 2B = 2n => U + B = n => U = n - B.
    // Total turns = U + V = (n - B) + 2B = n + B.
    // So total turns = n + B, where B is the number of bad turns. So k = n + B => B = k - n. This matches our earlier b.
    // Also, U = n - B = 2n - k.
    // V = 2B = 2(k - n).
    // Since U >= 0 and V >= 0, we need B <= n, and B >= 0. Also U must be integer, so B <= n. And V = 2B <= 2n? Actually V can be up to 2n, but since U >= 0, B <= n. So k <= 2n. But wait, sample 1: n=2, k=3 => B=1, U=1, V=2. Total turns=3. Works.
    // Sample 4: n=3, k=5 => B=2, U=1, V=4. Total turns=5. Works.
    // So the algorithm's behavior is completely determined by B, U, V.
    // Now, how to construct a sequence with given B?
    // We need exactly B bad turns. Each bad turn consumes two new cards that are different and neither is in S. After a bad turn, both go to S.
    // Good turns of type V consume one new card that matches an open card in S.
    // Good turns of type U consume two new cards that match each other.
    // We can design the sequence by interleaving these operations.
    // Let's think of the sequence of operations as a string of actions:
    // - 'B': bad turn (flip two new different cards, both go to S)
    // - 'U': good turn type U (flip two new same cards, discard)
    // - 'V': good turn type V (flip one new card that matches an open card, discard both)
    // The order of these actions must be valid: a V turn requires that the number is currently in S. S gets additions from B turns and removals from V turns.
    // We can think of S as a set of numbers waiting for their second copy.
    // Each number appears exactly twice. In a B turn, we introduce two numbers (first copies). In a V turn, we close one number (second copy matches first). In a U turn, we introduce and close a number in the same turn (both copies at once).
    // So we have n pairs. Some pairs are "split": their first copy appears in a B turn, second copy in a V turn. Some pairs are "together": both copies appear in a U turn.
    // Let s be the number of split pairs. Then each split pair contributes one first copy (added in some B turn) and one second copy (matched in some V turn). Since each B turn adds 2 first copies, we have 2B first copies total, so s = 2B? Wait, each B turn adds two first copies, so total first copies from B turns = 2B. Each such first copy belongs to a split pair. So the number of split pairs is 2B? But each split pair has exactly one first copy and one second copy. So 2B first copies means 2B split pairs. But total pairs = n. The remaining pairs are together (U turns). Each U turn consumes one together pair. Number of U turns = U = n - B. So together pairs = n - B. Split pairs = n - (n - B) = B? Contradiction: 2B vs B.
    // Let's recalc: In a B turn, we flip two new cards, both different, neither in S. These are first copies of two different numbers. So each B turn introduces two new numbers into S. So the number of first copies introduced in B turns is 2B. Each of these numbers must later be matched in a V turn (second copy). So there are 2B V turns? But V = 2B. Yes! Each V turn matches one number. So the number of split pairs is exactly the number of V turns, which is 2B. But wait, a split pair has one first copy and one second copy. The first copy is introduced in a B turn. The second copy is matched in a V turn. So the number of split pairs equals the number of V turns = 2B. But total pairs = n. Together pairs = n - 2B. But U = n - B. Each U turn consumes one together pair. So together pairs = U = n - B. Contradiction: n - 2B vs n - B.
    // Where is the error? Let's re-evaluate.
    // In a B turn, we flip two new cards, both go to S. These are first copies. So we add 2 to S.
    // In a V turn, we flip one new card that matches an open card in S. This new card is the second copy of some number. We remove 1 from S.
    // In a U turn, we flip two new cards that match each other. They are both first and second copies of the same number? Actually, if they match, they are the two copies of the same number. So they are both copies of that number. They are discarded immediately, so they never enter S. So a U turn consumes one pair completely.
    // Now, count pairs: Let x be the number of pairs that are split (one copy in B, one in V). Let y be the number of pairs that are together (both copies in U).
    // Total pairs: x + y = n.
    // Total first copies in B turns: each split pair contributes 1 first copy. So total first copies in B = x. But each B turn adds 2 first copies, so 2B = x.
    // Total V turns: each split pair contributes 1 second copy matched in a V turn. So V = x.
    // But we also have V = 2B from earlier equation (2B = V). So x = 2B and V = 2B, consistent.
    // Total U turns: each together pair contributes 1 U turn. So U = y.
    // We had U = n - B. So y = n - B.
    // Then x + y = 2B + n - B = n + B, but should be n. Contradiction!
    // So my equation U = n - B must be wrong. Let's re-derive U and V from the process.
    // Let:
    // B = number of bad turns (two new cards, different, both go to S)
    // U = number of good turns where two new cards match (discard immediately)
    // V = number of good turns where one new card matches an open card in S.
    // Total turns = B + U + V = k.
    // Total new cards consumed = 2B + 2U + 1V = 2n.
    // Also, S changes: each B adds 2, each V removes 1. Initially 0, finally 0. So 2B - V = 0 => V = 2B.
    // Substitute V: 2B + 2U + 2B = 2n => 4B + 2U = 2n => 2B + U = n => U = n - 2B.
    // Total turns k = B + U + V = B + (n - 2B) + 2B = n + B.
    // So k = n + B => B = k - n. This matches.
    // Then U = n - 2B = n - 2(k - n) = 3n - 2k.
    // V = 2B = 2(k - n).
    // Now check constraints: U >= 0 => 3n - 2k >= 0 => k <= 1.5n. But sample 4: n=3, k=5 => 1.5n = 4.5, k=5 > 4.5, so U would be negative! Let's compute: n=3, k=5 => B=2, U = 3 - 4 = -1. Impossible. But the sample output is YES. So my model is still wrong.
    // Let's re-simulate sample 4 with the new definitions.
    // Sequence: 1 2 3 1 2 3
    // Turn 1: flip 1, flip 2 -> different, bad. B=1. S={1,2}. (2 new cards consumed)
    // Turn 2: flip 3 (new), flip 1 (old? Wait, 1 was already flipped, so it's not new. But according to my types, V turn is one new card and one old card. Here we flipped 3 (new) and 1 (old). But they are different! So this turn does not discard. So it's a bad turn? But earlier I said bad turn is when two flipped cards are different. So turn 2 is also a bad turn! Because the two cards flipped are 3 and 1, different. So B=2. But wait, in turn 2, the second card was old (already in S). So it's not a "two new cards" turn. My definition of B as "two new cards" is wrong. A bad turn is ANY turn where the two flipped cards are different, regardless of whether they are new or old.
    // So let's redefine:
    // A turn is good if the two flipped cards have the SAME number. Bad otherwise.
    // In a good turn, the two cards could be:
    // - Both new (U): they are the two copies of the same number, flipped together.
    // - One new, one old (V): the new card matches an old card in S.
    // - Two old cards? This would be the "pair in S" rule, but we argued it's impossible. So no.
    // In a bad turn, the two cards could be:
    // - Both new (let's call this type B1): they are different, both go to S.
    // - One new, one old (type B2): the new card is different from the old card. What happens to S? The old card was already in S. The new card is different, so it goes to S. The old card remains in S? Or does it get removed? The turn ends with both cards flipped back. They are both "flipped previously". So the old card was already in S, the new card is added to S. So S gains one new number. But wait, the old card was already in S, so its status doesn't change. So S size increases by 1.
    // - Two old cards? Could we flip two old cards that are different? The algorithm never does that because it always flips the first unseen card as the first card. The second card could be old only if the first card's number is in S (then we flip that old card). So we never flip two old cards in one turn.
    // So bad turns can be of two subtypes: B1 (two new) and B2 (one new, one old).
    // Let:
    // B1 = number of bad turns with two new cards.
    // B2 = number of bad turns with one new, one old.
    // U = number of good turns with two new cards (matching).
    // V = number of good turns with one new, one old (matching).
    // Total turns = B1 + B2 + U + V = k.
    // Total new cards consumed = 2*B1 + 1*B2 + 2*U + 1*V = 2n.
    // S dynamics: each B1 adds 2 to S. each B2 adds 1 to S (the new card; the old card is already in S). each V removes 1 from S. U adds 0. Initially 0, finally 0.
    // So 2*B1 + B2 - V = 0 => V = 2*B1 + B2.
    // Also, total bad turns B = B1 + B2 = k - n. (since total good turns = n)
    // We have U + V = n (total good turns = n).
    // So V = n - U.
    // From V = 2*B1 + B2.
    // Also B1 + B2 = B.
    // We need to find nonnegative integers B1, B2, U, V satisfying these.
    // From total new cards: 2B1 + B2 + 2U + V = 2n.
    // Substitute V = n - U: 2B1 + B2 + 2U + n - U = 2n => 2B1 + B2 + U = n.
    // But V = n - U, and V = 2B1 + B2, so 2B1 + B2 = n - U, which matches.
    // So we have:
    // B1 + B2 = B = k - n
    // 2B1 + B2 + U = n
    // U >= 0, V = n - U >= 0 => 0 <= U <= n.
    // Also B1, B2 >= 0.
    // We can choose any B1, B2, U that satisfy these. Then we can construct a sequence.
    // Let's check sample 4: n=3, k=5 => B=2.
    // We need B1+B2=2, 2B1+B2+U=3.
    // Possible solutions:
    // B1=0, B2=2 => 0+2+U=3 => U=1. V = n-U =
