#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, k;
    cin >> n >> k;
    
    // The algorithm always takes at least n turns (each pair discarded separately)
    // and at most 2n-1 turns (when we always flip a new second card until the end)
    if (k < n || k > 2 * n - 1) {
        cout << "NO\n";
        return;
    }
    
    // We need to construct a sequence of length 2n with each number 1..n exactly twice
    // such that the greedy algorithm takes exactly k turns.
    
    // The greedy algorithm works as follows:
    // - It maintains a set of "seen" numbers (flipped previously but not matched).
    // - If there's a pair in seen, it matches them immediately (1 turn).
    // - Otherwise, it flips the first unseen card (call it x).
    //   - If x is in seen, it flips the matching seen card (1 turn, discards both).
    //   - Otherwise, it flips the next unseen card as second.
    //     - If they match, they're discarded (1 turn).
    //     - If not, both are added to seen (still 1 turn).
    
    // Key observation: The number of turns equals n + (number of times we flip two new cards that don't match).
    // Because each pair must be discarded eventually, and each discard takes 1 turn.
    // When we flip two new cards that match, that's 1 turn for that pair.
    // When we flip a new card and match it with a seen card, that's 1 turn.
    // When we flip two new cards that don't match, we spend 1 turn but both become seen,
    // and later they'll be matched with their partners, each taking 1 turn.
    // So total turns = n + (number of "mismatch" turns).
    
    // Let m = number of mismatch turns. Then k = n + m.
    // So m = k - n.
    // We need 0 <= m <= n-1.
    // m = 0 means we always match immediately (sequence like 1,1,2,2,...,n,n).
    // m = n-1 is maximum (sequence like 1,2,3,...,n,1,2,3,...,n).
    
    int m = k - n;
    // m must be between 0 and n-1 inclusive.
    // We already checked k bounds, so m is valid.
    
    vector<int> a;
    a.reserve(2 * n);
    
    if (m == 0) {
        // No mismatches: just pair them consecutively
        for (int i = 1; i <= n; i++) {
            a.push_back(i);
            a.push_back(i);
        }
    } else {
        // We want exactly m mismatches.
        // Construction: place first (m+1) distinct numbers, then their partners,
        // but arranged so that the first m pairs are mismatches, and the last one matches.
        // Actually, we can do: 
        // First part: 1, 2, 3, ..., m+1
        // Then we need to place the second copies.
        // To create m mismatches, we can place the second copy of 1 at position after some new numbers.
        // A known construction: 
        // a = [1, 2, 3, ..., m+1, 1, m+2, 2, m+3, 3, ..., 2m+1?] 
        // Let's think simpler:
        // We want the first m turns to be mismatches (flip two new different cards).
        // Then after that, all remaining cards are matched immediately.
        
        // Construction:
        // Place numbers 1..(m+1) in order.
        for (int i = 1; i <= m + 1; i++) {
            a.push_back(i);
        }
        // Now we have m+1 numbers placed once each.
        // We need to place their second copies such that:
        // - The first m times we flip a new card, the second card we flip is also new and different.
        // - The (m+1)-th time, we flip the last new card and its partner is already seen? 
        // Actually, let's simulate:
        // Initially seen = empty.
        // Turn 1: flip first unseen -> 1. seen doesn't have 1. flip next unseen -> 2. mismatch. seen = {1,2}
        // Turn 2: flip first unseen -> 3. seen doesn't have 3. flip next unseen -> 4. mismatch. seen = {1,2,3,4}
        // ...
        // Turn m: flip first unseen -> m. seen doesn't have m. flip next unseen -> m+1. mismatch. seen = {1,2,...,m+1}
        // Now we have flipped all first copies. seen has all numbers 1..m+1.
        // Next, we need to place the second copies.
        // If we place the second copy of 1 next, then:
        // Turn m+1: first unseen is second copy of 1. seen has 1, so we flip the seen 1. match! (1 turn)
        // Then seen becomes {2,3,...,m+1}.
        // Next, if we place second copy of 2, match, etc.
        // This would give total turns = (m mismatches) + (m+1 matches) = 2m+1? But we have n pairs total.
        // Wait, we only placed m+1 pairs so far. We still have n - (m+1) pairs left.
        // For the remaining pairs, we can place them as consecutive pairs (i,i) and they will be matched immediately when flipped.
        // But careful: The algorithm always flips the first unseen card. After we discard all seen cards, the remaining cards are all unseen.
        // If we place them as consecutive pairs, the algorithm will flip the first one, then the second one (since no seen cards), they match, discard.
        // So each such pair takes 1 turn.
        // Total turns = m (mismatches) + (m+1) (matches for the first m+1 pairs) + (n - (m+1)) (matches for the rest) = n + m = k. Perfect!
        
        // So the construction:
        // First part: 1, 2, 3, ..., m+1
        // Second part: 1, 2, 3, ..., m+1 (the second copies in the same order)
        // Third part: remaining numbers (m+2 to n) each repeated twice consecutively.
        
        // But wait: Let's simulate this construction for n=3, m=1 (k=4):
        // a = [1, 2, 1, 2, 3, 3]
        // Turn 1: flip 1, then flip 2 (mismatch). seen={1,2}
        // Turn 2: first unseen is 1. seen has 1, so flip the seen 1. match. seen={2}
        // Turn 3: first unseen is 2. seen has 2, match. seen={}
        // Turn 4: first unseen is 3, flip next unseen 3, match. done. 4 turns. Correct.
        
        // But wait, the problem example 4: n=3, k=5 -> m=2.
        // Our construction: m=2 -> first part: 1,2,3; second part: 1,2,3; third part: none (since m+1=3=n).
        // a = [1,2,3,1,2,3]
        // Simulate:
        // Turn 1: 1,2 mismatch. seen={1,2}
        // Turn 2: first unseen 3, flip next unseen 1? Wait, after turn 1, the next unseen is 3. 
        // The algorithm: flip first unseen -> 3. seen doesn't have 3. flip next unseen -> 1 (the second copy of 1). 
        // But 1 is in seen! So it would match 1? No, the algorithm says: 
        // "Afterwards, if there is another card that you have flipped previously and has the number x, flip that card."
        // Here x=3, and we haven't flipped 3 previously. So we go to: "Otherwise, flip the first card that you have never flipped so far as the second one."
        // The next unseen is the second copy of 1. So we flip 1. They are different (3 vs 1), so mismatch. seen={1,2,3,1?} Wait, seen contains numbers, not positions. 
        // Actually, seen is a set of numbers that have been flipped and are currently face-up? No, the algorithm: "If there are two cards that you have flipped previously and have the same number, flip those two cards."
        // This means we remember which numbers we have seen exactly once (i.e., flipped but not yet matched). 
        // When we flip a card, if its number is in the "seen once" set, we match it with the previously seen one. Otherwise, we add it to the set.
        // So after turn 1: seen_once = {1,2} (each seen once).
        // Turn 2: flip first unseen -> 3. 3 not in seen_once. flip next unseen -> 1. 1 IS in seen_once. So we match 1 with the previously seen 1? 
        // Wait, the algorithm says: "Afterwards, if there is another card that you have flipped previously and has the number x, flip that card." Here x is the first card's number, which is 3. 
        // It does NOT say "if the second card's number is in seen". It says: after flipping the first card (which is 3), if there is a previously flipped card with number 3, flip it. 
        // Since there isn't, we flip the next unseen card as second. That second card is 1. 
        // Then we compare the two flipped cards: 3 and 1. They are different, so we flip them back. Both 3 and 1 are now "flipped previously". 
        // But 1 was already flipped previously! So now we have two cards with number 1 that are flipped previously? 
        // Actually, the first 1 was flipped in turn 1 and is still face-up? No, in turn 1 we flipped 1 and 2, they were different, so we flipped them back (they are face-down again). 
        // But the algorithm remembers that we have "flipped previously" those cards. The state is: we have seen numbers 1 and 2 once each. 
        // In turn 2, we flip 3 (new) and 1 (which was seen once). After flipping them, we have flipped 3 (now seen once) and 1 (now seen twice? Actually, we flipped the second copy of 1). 
        // Since we flipped two cards with different numbers, we flip them back. Now we have seen: 1 (twice? but they are different cards), 2 (once), 3 (once). 
        // The rule "If there are two cards that you have flipped previously and have the same number, flip those two cards." Now we have two cards with number 1 that have been flipped previously! 
        // So on the next turn, we would flip those two 1s and discard them. 
        // Let's continue simulation properly:
        // Initial: all face-down, no memory.
        // Turn 1: flip first unseen -> pos1 (1). x=1. No previously flipped 1. flip next unseen -> pos2 (2). Compare 1 and 2: different. Flip both back. Memory: we have flipped pos1 (1) and pos2 (2). So numbers seen once: {1,2}.
        // Turn 2: check if any number seen twice? No. flip first unseen -> pos3 (3). x=3. No previously flipped 3. flip next unseen -> pos4 (1). Compare 3 and 1: different. Flip both back. Memory: we have flipped pos1(1), pos2(2), pos3(3), pos4(1). Now number 1 has been flipped twice (two different cards). So we have a pair!
        // Turn 3: there are two cards with same number flipped previously (the two 1s). So we flip those two cards (pos1 and pos4). They match, discard. Memory: we forget about those cards. Remaining flipped: pos2(2), pos3(3). Numbers seen once: {2,3}.
        // Turn 4: check pairs? No. flip first unseen -> pos5 (2). x=2. There is a previously flipped 2 (pos2). So we flip that card (pos2). They match, discard. Memory: remaining flipped: pos3(3). Seen once: {3}.
        // Turn 5: flip first unseen -> pos6 (3). x=3. Previously flipped 3 exists (pos3). Flip it, match, discard. Done. Total 5 turns. Matches example.
        
        // So our construction [1,2,3,1,2,3] works for m=2.
        // Let's test m=1: [1,2,1,2,3,3] for n=3, k=4.
        // Turn 1: flip 1,2 mismatch. seen={1,2}
        // Turn 2: first unseen 1. x=1. previously flipped 1 exists? Yes, the first 1. So we flip that first 1. They match, discard. seen={2}
        // Turn 3: first unseen 2. x=2. previously flipped 2 exists. Flip it, match, discard. seen={}
        // Turn 4: first unseen 3. x=3. no previous 3. flip next unseen 3. match, discard. Total 4. Works.
        
        // So the construction is:
        // For i = 1..m+1: a.push_back(i)
        // For i = 1..m+1: a.push_back(i)  // second copies
        // For i = m+2..n: a.push_back(i), a.push_back(i)
        
        // But wait, is this always valid? Let's check n=2, k=3 (m=1):
        // a = [1,2,1,2] (since m+1=2, no remaining)
        // Turn 1: 1,2 mismatch. seen={1,2}
        // Turn 2: first unseen 1. x=1, previous 1 exists, match. seen={2}
        // Turn 3: first unseen 2. x=2, previous 2 exists, match. done. 3 turns. Works.
        
        // n=2, k=2 (m=0): handled by m=0 case: [1,1,2,2] -> 2 turns.
        // n=2, k=4? max k=2n-1=3, so invalid.
        
        // What about n=4, m=2 (k=6):
        // a = [1,2,3, 1,2,3, 4,4]
        // Simulate:
        // T1: 1,2 mismatch. seen={1,2}
        // T2: first unseen 3. x=3, no prev 3. flip next unseen 1. mismatch (3 vs 1). seen={1,2,3,1} -> pair of 1s!
        // T3: flip two 1s, discard. seen={2,3}
        // T4: first unseen 2. x=2, prev 2 exists, match. seen={3}
        // T5: first unseen 3. x=3, prev 3 exists, match. seen={}
        // T6: first unseen 4. x=4, no prev 4. flip next unseen 4. match. done. 6 turns. Works.
        
        // So the construction seems correct.
        
        // But wait, the problem statement says: "If there are two cards that you have flipped previously and have the same number, flip those two cards."
        // In our simulation, after turn 2 in the last example, we had two 1s flipped previously. The algorithm will flip them on turn 3. That's correct.
        
        // However, we must ensure that the algorithm's behavior matches our simulation exactly.
        // Let's double-check the algorithm's priority:
        // 1. If there are two cards that you have flipped previously and have the same number, flip those two cards.
        // 2. Otherwise, flip the first card that you have never flipped so far as the first one.
        // 3. Afterwards, if there is another card that you have flipped previously and has the number x, flip that card.
        // 4. Otherwise, flip the first card that you have never flipped so far (including in this turn) as the second one.
        
        // In our construction, after the first m turns, we have m+1 numbers each seen once? Actually, after m turns, we have flipped 2m cards: the first m numbers and the numbers 2..m+1? Let's trace carefully.
        
        // For general m:
        // a = [1, 2, 3, ..., m+1, 1, 2, 3, ..., m+1, (m+2,m+2), ...]
        // Turn 1: flip 1,2 -> mismatch. seen_once = {1,2}
        // Turn 2: flip first unseen = 3. x=3. no prev 3. flip next unseen = 4? Wait, next unseen is 4 if m+1 >=4. But if m=2, m+1=3, next unseen is 1 (second copy). So it depends on m.
        // Actually, the sequence of unseen cards: first m+1 cards are 1,2,...,m+1. Then the next m+1 cards are again 1,2,...,m+1.
        // So on turn t (1 <= t <= m), we flip the t-th card (number t) as first, and the (t+1)-th card? No, the algorithm flips the first unseen card. Initially all unseen.
        // Turn 1: first unseen = pos1 (1). second = next unseen = pos2 (2). mismatch. seen = {1,2}.
        // Turn 2: first unseen = pos3 (3). second = next unseen = pos4 (4). mismatch. seen = {1,2,3,4}.
        // ...
        // Turn m: first unseen = pos(2m-1) = number m. second = pos(2m) = number m+1. mismatch. seen = {1,2,...,m+1}.
        // So after m turns, we have flipped the first 2m cards, which are 1,2,...,m+1? Wait, 2m cards: positions 1..2m.
        // The first m+1 positions are 1..m+1. The next m+1 positions are 1..m+1 again.
        // So positions 1..2m: if 2m <= m+1? That would mean m <= 1. For m>=2, 2m > m+1, so we have flipped some second copies.
        // Let's list positions for m=3: a = [1,2,3,4, 1,2,3,4]
        // Turn 1: pos1(1), pos2(2) -> seen={1,2}
        // Turn 2: pos3(3), pos4(4) -> seen={1,2,3,4}
        // Turn 3: pos5(1), pos6(2)? Wait, after turn 2, the next unseen is pos5 (1). 
        // Turn 3: first unseen = pos5 (1). x=1. Is there a previously flipped 1? Yes, pos1. So we would flip pos1! That's a match, not a mismatch.
        // But we wanted m=3 mismatches. So this construction fails for m>=2? Let's re-evaluate.
        
        // Ah! In my earlier simulation for m=2, I had a = [1,2,3,1,2,3]. 
        // Turn 1: pos1(1), pos2(2) -> seen={1,2}
        // Turn 2: first unseen = pos3(3). x=3. no prev 3. next unseen = pos4(1). mismatch (3 vs 1). seen={1,2,3,1} -> now two 1s!
        // So turn 2 was a mismatch, but it involved the second copy of 1. 
        // In the general construction I proposed: a = [1..m+1] + [1..m+1] + pairs.
        // For m=3: a = [1,2,3,4,1,2,3,4]
        // Turn 1: 1,2 mismatch. seen={1,2}
        // Turn 2: first unseen = 3. x=3. no prev 3. next unseen = 4. mismatch (3 vs 4). seen={1,2,3,4}
        // Turn 3: first unseen = 1 (second copy). x=1. prev 1 exists! So we flip the first 1. match! Not a mismatch.
        // So we only got 2 mismatches, not 3.
        
        // So the simple concatenation only works for m=1 and m=2? Let's check m=2: [1,2,3,1,2,3] gave 2 mismatches? Actually we got mismatches on turn 1 (1,2) and turn 2 (3,1). That's 2 mismatches. m=2. Works.
        // For m=3, we need 3 mismatches. How to get 3 mismatches?
        // We need the first 3 turns to be mismatches. That means on turn 3, the first unseen card should not have its partner already seen, and the second card should also be new and different.
        // In [1,2,3,4,1,2,3,4], on turn 3, first unseen is 1 (second copy), which has its partner seen. So it matches.
        // To avoid that, we need to delay the second copies of the early numbers.
        // Idea: interleave the second copies such that the first m turns always flip two new numbers.
        // We can place the second copies in a different order.
        // Let's think: We want the first m turns to flip 2m distinct numbers? But we only have m+1 distinct numbers in the first part. So we must reuse some numbers? No, we have n pairs total. We can use more numbers.
        // Actually, we can use numbers 1..2m? But we only have n numbers. If m is large, we might not have enough numbers. But m <= n-1, so 2m could be up to 2n-2, which is > n. So we can't have 2m distinct numbers if m > n/2.
        // So the mismatches must involve flipping some numbers that are already seen? No, a mismatch turn is defined as flipping two cards that are different. They could be both new, or one new and one seen but different from the first? Wait, the algorithm: if the first card's number is in seen, we flip the matching seen card -> that's a match, not a mismatch. So a mismatch only happens when the first card is new (not in seen), and the second card is either new (and different) or seen but different? But if the second card is seen, its number is in seen. But the first card is new, so its number is not in seen. They are different, so it's a mismatch. However, if the second card is seen, then after flipping it, we now have two copies of that seen number flipped? Actually, the seen set contains numbers that have been flipped exactly once. If we flip a second card that is in seen, we are flipping the second copy of that number. Then we would have two copies of that number flipped, so they would be matched on the next turn. But the current turn is still a mismatch (since the two cards flipped are different). So a mismatch turn can involve a seen card as the second card, as long as the first card is new and different.
        // In our m=2 example: turn 2: first=3 (new), second=1 (seen). mismatch. Then next turn matches the two 1s.
        // So to get m mismatches, we can have some turns where the second card is a seen card.
        // Let's design a general construction.
        
        // We want exactly m mismatch turns.
        // Let the sequence be divided into blocks.
        // A known solution from editorial: 
        // If m == 0: 1,1,2,2,...,n,n
        // Else: 
        // We can construct as follows:
        // Let the first m+1 numbers be 1,2,...,m+1.
        // Then we place the second copy of 1.
        // Then we place the remaining numbers in a way that creates the mismatches.
        // Actually, the editorial suggests:
        // a = [1, 2, ..., m+1, 1, m+2, 2, m+3, 3, ..., 2m+1?] 
        // Let's derive properly.
        
        // We want the first m turns to be mismatches.
        // Turn 1: flip 1 and 2 (mismatch). seen={1,2}
        // Turn 2: flip 3 and ?. We want mismatch. If we flip 3 and 1 (seen), mismatch. seen becomes {1,2,3,1} -> pair of 1s.
        // Turn 3: we must match the two 1s (that's a match turn, not a mismatch). So after turn 2, we have a pair, which will be matched on turn 3. That turn is a match, not a mismatch. So we only got 2 mismatches (turn 1 and 2). To get m=3, we need turn 3 to also be a mismatch. But turn 3 will be forced to match the pair of 1s because the algorithm prioritizes matching existing pairs. So we cannot have a mismatch on turn 3 if there is a pending pair.
        // Therefore, to have m mismatches, we must ensure that no pair is formed until after the m-th turn? But in m=2 example, a pair was formed after turn 2, and turn 3 was a match. That's fine because m=2 means exactly 2 mismatches, and the rest are matches. So the mismatches don't have to be consecutive; they can be interleaved with matches? Wait, the total turns = n + m. The m mismatches are the extra turns beyond n. In the m=2 example, total turns = 5, n=3, so m=2. The turns were: T1 mismatch, T2 mismatch, T3 match, T4 match, T5 match. So mismatches were T1 and T2. They were consecutive. Could they be non-consecutive? Suppose we have a match early on. That would reduce the number of seen cards, making it harder to get mismatches later. It's probably optimal to have all mismatches first.
        
        // So we want the first m turns to be mismatches, and then all remaining turns are matches.
        // After m mismatches, we will have some seen cards. Then we need to match them all in the remaining turns.
        // Each mismatch turn adds two cards to the seen set (if both are new) or one new and one seen (which creates a pair that will be matched next turn). 
        // Let's analyze the state after t mismatches.
        // We can design the sequence such that after m mismatches, the seen set contains exactly some numbers, and the remaining cards are arranged to match them efficiently.
        
        // Consider the following construction from the editorial:
        // Let m = k - n.
        // If m == 0: 1,1,2,2,...,n,n
        // Else:
        // We output: 1, 2, ..., m+1, then 1, then m+2, 2, m+3, 3, ... up to 2m+1? Wait, we need to place all numbers.
        // Let's look at the sample: n=6, k=10 -> m=4.
        // Sample output: 2 1 3 4 5 4 1 2 6 5 6 3
        // Let's check if this follows a pattern.
        // n=6, m=4.
        // Sequence: 2, 1, 3, 4, 5, 4, 1, 2, 6, 5, 6, 3
        // Let's simulate to see mismatches:
        // I'll trust the sample.
        
        // Another approach: We can construct by placing the first m+1 numbers as: 1,2,...,m+1.
        // Then we need to place the second copies such that the first m turns are mismatches.
        // Let's try to make the first m turns flip: 
        // T1: 1 and 2 (mismatch)
        // T2: 3 and 1 (mismatch, using second copy of 1)
        // T3: match the two 1s? That would be a match. But we want T3 to be a mismatch. So we cannot have the second copy of 1 appear too early.
        // To delay the pair of 1s, we can place the second copy of 1 later. 
        // Suppose we place the second copies in reverse order? 
        // a = [1,2,3,4, 4,3,2,1] for m=3? Let's test n=4, m=3 (k=7).
        // a = [1,2,3,4,4,3,2,1]
        // T1: 1,2 mismatch. seen={1,2}
        // T2: first unseen 3. x=3. no prev 3. next unseen 4. mismatch (3,4). seen={1,2,3,4}
        // T3: first unseen 4 (second copy). x=4. prev 4 exists! match. So only 2 mismatches.
        
        // We need T3 to be a mismatch. So the first unseen on T3 must be a new number (not in seen). But all numbers 1..4 are already seen. So we must introduce a new number? But we only have n=4 numbers. So we can't have a new number. Therefore, for n=4, m cannot be 3? But k max is 2n-1=7, so m max is n-1=3. So m=3 should be possible. How?
        // If all numbers are seen by T3, then T3 must be a match. So m=3 would mean 3 mismatches, but we only have 4 numbers. Let's think: total turns = n+m = 7. Mismatches = 3, matches = 4. Each match discards a pair. Each mismatch adds cards to seen. 
        // Let's try to construct for n=4, m=3 manually.
        // We need 3 mismatches and 4 matches.
        // Let the pairs be (1,2), (3,4) etc.
        // One known construction for maximum k (m=n-1) is: 1,2,3,...,n,1,2,3,...,n. For n=4, that's [1,2,3,4,1,2,3,4]. Let's simulate:
        // T1: 1,2 mismatch. seen={1,2}
        // T2: 3,1 mismatch (3 new, 1 seen). seen={1,2,3,1} -> pair 1s.
        // T3: match 1s. seen={2,3}
        // T4: first unseen 2 (second copy). x=2. prev 2 exists. match. seen={3}
        // T5: first unseen 3 (second copy). x=3. prev 3 exists. match. seen={}
        // T6: first unseen 4 (second copy). x=4. no prev 4. next unseen? none? Actually, after T5, we have discarded 1s, 2s, 3s. Remaining: the two 4s? Wait, we had [1,2,3,4,1,2,3,4]. After T1: flipped 1,2. T2: flipped 3,1 (second 1). T3: matched 1s (first and second 1). T4: first unseen is 2 (second copy). Match with first 2. T5: first unseen is 3 (second copy). Match with first 3. T6: first unseen is 4 (first copy). x=4. no prev 4. next unseen is second 4. match. Total 6 turns? That's n+m = 4+2=6? Wait, m=2? Let's count mismatches: T1 mismatch, T2 mismatch. That's 2 mismatches. T3-T6 matches. Total turns=6. So m=2, not 3. So [1,2,3,4,1,2,3,4] gives m=2 for n=4.
        // How to get m=3 for n=4?
        // We need 7 turns. Let's try to design:
        // We need 3 mismatches. Each mismatch consumes at least one turn and doesn't discard a pair. 
        // Let's try: a = [1,2,3,1,4,2,4,3]? 
        // Simulate:
        // T1: 1,2 mismatch. seen={1,2}
        // T2: first unseen 3. x=3. no prev 3. next unseen 1 (second copy). mismatch (3,1). seen={1,2,3,1} -> pair 1s.
        // T3: match 1s. seen={2,3}
        // T4: first unseen 4. x=4. no prev 4. next unseen 2 (second copy). mismatch (4,2). seen={2,3,4,2} -> pair 2s.
        // T5: match 2s. seen={3,4}
        // T6: first unseen 4 (second copy). x=4. prev 4 exists. match. seen={3}
        // T7: first unseen 3 (second copy). x=3. prev 3 exists. match. done. 7 turns! Mismatches: T1, T2, T4 -> 3 mismatches. Total turns=7. Works!
        // So the pattern: we interleave the second copies to create mismatches.
        
        // Let's generalize: We want m mismatches. We can create a mismatch whenever we flip a new card and then a seen card (different from the new one). This creates a pair of the seen card, which will be matched next turn. So each such mismatch is followed by a match of that pair. This pair of turns (mismatch then match) discards one pair and leaves the new card in the seen set. 
        // Let's formalize: Start with seen empty.
        // We can have a sequence of "mismatch+match" blocks that each consume one new pair and one seen card? Actually, let's think recursively.
        
        // Another known construction from editorial:
        // If m == 0: 1,1,2,2,...,n,n
        // Else if m == n-1: 1,2,3,...,n,1,2,3,...,n (but we saw this gives m=n-1? For n=3, m=2, it gave 5 turns, m=2=n-1. For n=4, it gave 6 turns, m=2, but n-1=3. So it doesn't give m=n-1 for n>=4. Wait, sample n=3, k=5 -> m=2, output was 1,2,3,1,2,3. That's the simple concatenation. For n=4, k=7 (m=3), the simple concatenation gives k=6. So it's not maximal for n>=4.
        // The maximal k is achieved by a different sequence. The editorial says the maximum k is 2n-1, and it's achieved
