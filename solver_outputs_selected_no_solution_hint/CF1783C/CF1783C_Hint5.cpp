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
        vector<int> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }

        // We want to maximize our wins, but also consider the effect on our rank.
        // If we can win against x opponents, our wins = x.
        // Opponent i has base wins = i (since beats 0..i-1) plus possibly a win against us if we don't prepare for i.
        // Actually opponent i wins against all j < i, and against us if we don't prepare for i.
        // So opponent i's wins = i + (we don't prepare for i ? 1 : 0).
        // Our rank = 1 + number of opponents with strictly more wins than us.

        // We can choose a set S of opponents to prepare against, sum a_i <= m.
        // Our wins = |S|.
        // For opponent i:
        //   if i in S: wins_i = i
        //   else: wins_i = i + 1
        // We want to minimize rank = 1 + count(i: wins_i > |S|).

        // Let's sort opponents by a_i to maximize |S| greedily? But we also care about which opponents we skip.
        // Notice that if we skip opponent i, their wins become i+1, which might exceed our wins.
        // If we beat opponent i, their wins = i.
        // So beating an opponent reduces their wins by 1.
        // Our rank depends on how many opponents end up with wins > our wins.

        // Let's consider we decide to win x games. We need to pick x opponents with smallest a_i.
        // But we also might want to specifically beat certain opponents to reduce their wins.
        // However, note that opponent i's wins without our interference is i+1.
        // If we beat them, it becomes i.
        // So beating opponent i reduces their wins by 1.
        // To minimize rank, we want to reduce wins of those who would otherwise have > x wins.
        // Opponents with i+1 > x, i.e., i >= x, would have > x wins if we don't beat them.
        // If we beat them, their wins become i, which might still be > x if i > x, or == x if i == x.
        // Actually if i > x, even after beating them, wins_i = i > x, so they still have more wins.
        // If i == x, beating them makes wins_i = x, which is not strictly more than x, so they don't outrank us.
        // If i < x, even without beating them, wins_i = i+1 <= x, so they don't outrank us (unless i+1 > x, i.e., i >= x, but i < x so i <= x-1, i+1 <= x).
        // So the only opponents who can potentially outrank us are those with i >= x.
        // Among them, those with i > x will always have > x wins regardless of whether we beat them.
        // Those with i == x will have > x wins if we don't beat them, but == x wins if we beat them.
        // Therefore, to minimize the number of opponents with > x wins, we must beat opponent x (1-indexed) if we want to have x wins.
        // Because opponent x is the only one whose win count can be reduced from x+1 to x, which is not > x.
        // All opponents i > x will always have > x wins.
        // So if we achieve x wins, the number of opponents with > x wins is at least (n - x) (those i > x) plus possibly 1 if we don't beat opponent x.
        // Thus rank = 1 + (n - x) + (we didn't beat opponent x ? 1 : 0) = n - x + 1 + (didn't beat x).
        // Wait: if we beat opponent x, rank = n - x + 1. If we don't, rank = n - x + 2.
        // But is it always possible to beat opponent x while achieving x wins? We need to select x opponents including opponent x, with sum a_i <= m.
        // To maximize chance, we should pick the x smallest a_i, but we must include opponent x.
        // So we can try all possible x from 0 to n, and see if we can select x opponents including opponent x (if x>0) with sum <= m.
        // Actually x can be 0: then we have 0 wins. Opponents with i >= 0? All i >= 1 have i+1 >= 2 > 0, so all n opponents have >0 wins. Rank = n+1.
        // For x > 0: we need to pick x opponents, must include opponent x. The other x-1 should be the smallest among the rest.
        // We can pre-sort opponents by a_i, but we need to know the original index to identify opponent x.
        // Let's keep original indices. We can sort pairs (a_i, i). Then for each x, we need to select x opponents including the one with original index x.
        // We can find the smallest x-1 a_i's among all except the one with index x. Then check sum <= m.
        // Since n up to 5e5, we can do this efficiently by precomputing prefix sums of sorted a_i, and for each x, we locate the position of opponent x in the sorted array, and compute the sum of the smallest x elements that include it.
        // Specifically, let sorted array be b[0..n-1] with values and original indices.
        // For a given x (1..n), we want the sum of the x smallest a_i that include the element with original index x.
        // Let pos be the index in b of the element with original index x.
        // If we take the first x elements of b, if pos < x, then opponent x is already included, sum = prefix[x-1].
        // If pos >= x, then the first x elements do not include opponent x. We need to replace the largest among the first x (which is b[x-1]) with b[pos]? Actually we want the smallest possible sum of x elements including b[pos]. The optimal set is the first x-1 elements of b plus b[pos], but if b[pos] is already among the first x-1? No, pos >= x, so b[pos] is not in first x. So the sum = prefix[x-2] + b[pos].value (if x>=2). If x=1, sum = b[pos].value.
        // But wait: what if there are multiple elements with the same a_i? The original index matters. We need to ensure we correctly identify the element with original index x. Since we sort by a_i, and then by original index to break ties, we can find its position.
        // So algorithm:
        // For each test case:
        //   read n, m, a[1..n].
        //   create vector of pairs (a_i, i) for i=1..n.
        //   sort by a_i, then by i.
        //   compute prefix sums of sorted a_i.
        //   ans = n + 1 (worst rank, x=0)
        //   for x = 1 to n:
        //       find position of opponent x in sorted array.
        //       compute sum:
        //         if pos < x: sum = prefix[x-1]
        //         else: sum = (x>=2 ? prefix[x-2] : 0) + a[x-1] (since a is 0-indexed, opponent x has value a[x-1])
        //       if sum <= m:
        //           rank = n - x + 1
        //           ans = min(ans, rank)
        //   print ans

        // But wait: is it always optimal to beat opponent x if we want x wins? What if we beat opponent x but we could achieve x wins without beating them, and instead beat someone else? But if we don't beat opponent x, their wins = x+1 > x, so they outrank us. So we must beat them to avoid that. So yes, we must include opponent x.
        // However, what if we achieve x wins but we beat opponent x and also some others, and opponent x's wins become x, which ties with us. That's fine, they don't strictly outrank us.
        // So the condition is correct.

        // Edge case: x = n. Then we beat all opponents. Opponent n has wins = n (since we beat them), we have n wins. No one has > n wins. Rank = 1. Formula gives n - n + 1 = 1. Correct.

        // Let's test with sample.
        // Sample 1: n=4, m=401, a=[100,100,200,1]
        // Sorted: (1,4), (100,1), (100,2), (200,3)
        // prefix: [1, 101, 201, 401]
        // x=1: opponent 1 at pos 1 (0-indexed). pos=1, x=1 -> pos >= x, sum = a[0]=100 <=401. rank=4-1+1=4. ans=4.
        // x=2: opponent 2 at pos 2. pos=2 >=2, sum = prefix[0] + a[1] = 1 + 100 = 101 <=401. rank=4-2+1=3. ans=3.
        // x=3: opponent 3 at pos 3. pos=3 >=3, sum = prefix[1] + a[2] = 101 + 200 = 301 <=401. rank=4-3+1=2. ans=2.
        // x=4: opponent 4 at pos 0. pos=0 <4, sum = prefix[3] = 401 <=401. rank=4-4+1=1. ans=1.
        // Output 1. Matches.

        // Sample 2: n=3, m=2, a=[1,2,3]
        // Sorted: (1,1), (2,2), (3,3)
        // prefix: [1,3,6]
        // x=1: opp 1 at pos 0 <1, sum=prefix[0]=1 <=2. rank=3-1+1=3. ans=3.
        // x=2: opp 2 at pos 1 <2, sum=prefix[1]=3 >2. cannot.
        // x=3: opp 3 at pos 2 <3, sum=prefix[2]=6 >2. cannot.
        // ans=3? But sample output is 2. Wait, my formula gave rank=3 for x=1, but sample says place 2. Let's recalc.
        // If we beat opponent 2 (a=2), we have 1 win. Opponent 1: wins = 1 (beats 0? Actually opponent 1 beats no one except maybe us? Wait, opponent 1 plays opponent 2: 1<2 so opponent 2 wins. Opponent 1 plays opponent 3: 1<3 so opponent 3 wins. Opponent 1 plays us: we didn't prepare for 1, so opponent 1 wins. So opponent 1 wins: against us? Yes, we didn't prepare. So opponent 1 wins = 1 (against us). Opponent 2: we prepared, so we win against 2. Opponent 2 beats 1, loses to 3. So opponent 2 wins = 1 (against 1). Opponent 3: beats 1 and 2, and beats us (we didn't prepare). So opponent 3 wins = 3. Our wins = 1 (against 2). So wins: opponent 3 has 3, we have 1, opponent 1 has 1, opponent 2 has 1. Number of people with strictly more wins than us: only opponent 3. So rank = 1+1=2. My formula for x=1: rank = n - x + 1 = 3 - 1 + 1 = 3. But we didn't beat opponent x=1. According to my logic, if we have 1 win and don't beat opponent 1, opponent 1's wins = 1+1=2 > 1, so they outrank us. But in reality, opponent 1's wins = 1? Let's check: opponent 1's wins: against 2? loses. against 3? loses. against us? wins (we didn't prepare). So total 1 win. Why did I think opponent 1's wins = i+1 = 2? Because i=1, so i+1=2. But opponent 1 only gets 1 win from us, and 0 from others because they are higher indices? Wait, opponent i beats all j < i. For i=1, there are no j < 1. So opponent 1 beats nobody among other opponents. So opponent 1's base wins = 0. Then plus possibly 1 from us. So wins = 0 + (we don't prepare? 1 : 0) = 1. My earlier formula: opponent i's wins = i + (we don't prepare? 1 : 0). But i is 1-indexed. For i=1, i = 1, but base wins should be 0, not 1. Ah! Mistake: opponent i beats opponents 1..i-1, so number of wins against other opponents is i-1, not i. Because they beat those with smaller index. So opponent i's base wins = i-1. Then against us: +1 if we don't prepare. So total wins = (i-1) + (we don't prepare? 1 : 0) = i - 1 + (1 - prepared_i) = i - prepared_i. Where prepared_i = 1 if we prepare, else 0.
        // So opponent i's wins = i - 1 + (1 - prepared_i) = i - prepared_i.
        // Our wins = sum prepared_i = x.
        // We want to count opponents with wins > x.
        // Opponent i's wins > x  <=>  i - prepared_i > x  <=>  i > x + prepared_i.
        // Since prepared_i is 0 or 1, this means:
        // If we prepare for i (prepared_i=1): wins = i-1. Condition: i-1 > x => i > x+1.
        // If we don't prepare for i (prepared_i=0): wins = i. Condition: i > x.
        // So opponents with i > x+1 always have > x wins regardless.
        // Opponents with i = x+1: if we don't prepare, wins = x+1 > x, outrank us. If we prepare, wins = x, not > x.
        // Opponents with i = x: if we don't prepare, wins = x, not > x. If we prepare, wins = x-1, not > x.
        // Opponents with i < x: wins <= x, never > x.
        // Therefore, to minimize outrankers, we must prepare for opponent x+1 if we want to have x wins. Because opponent x+1 is the only one whose win count can be reduced from x+1 to x.
        // Wait, what about opponent x? If we don't prepare, wins = x, which is not > x. So they don't outrank us. So we don't need to prepare for opponent x.
        // But if we prepare for opponent x+1, their wins become x, tying us. So they don't outrank us.
        // So the critical opponent is x+1, not x.
        // Let's re-evaluate:
        // Number of opponents with wins > x:
        // For i > x+1: always > x. Count = n - (x+1) = n - x - 1.
        // For i = x+1: > x if not prepared, = x if prepared.
        // For i <= x: never > x.
        // So total outrankers = (n - x - 1) + (prepared_{x+1} ? 0 : 1).
        // Rank = 1 + outrankers = n - x - 1 + 1 + (prepared_{x+1} ? 0 : 1) = n - x + (prepared_{x+1} ? 0 : 1).
        // If we prepare for x+1, rank = n - x. If not, rank = n - x + 1.
        // But wait, if x = n, then x+1 = n+1 doesn't exist. Then outrankers = 0, rank = 1. Formula: n - n = 0? Actually n - x = 0, but rank should be 1. So rank = n - x + 1? Let's check: for x=n, n - n = 0, but rank is 1. So rank = n - x + 1? But earlier we had n - x + (0 or 1). For x=n, n - n = 0, so rank = 0 or 1. We need rank = 1. So if we prepare for x+1 (which doesn't exist), we consider it as prepared? Actually if x=n, there is no opponent n+1, so the condition "prepared_{x+1}" is vacuously true? We can treat it as prepared. Then rank = n - x = 0? That's wrong. Let's recalc carefully.

        // Let's do general:
        // Our wins = x.
        // Opponent i wins = (i-1) + (1 - prepared_i) = i - prepared_i.
        // We want count of i such that i - prepared_i > x.
        // For i from 1 to n:
        // If prepared_i = 1: condition i - 1 > x => i > x+1.
        // If prepared_i = 0: condition i > x.
        // So the set of i with > x wins is:
        // {i | i > x+1} U {i | i = x+1 and prepared_i = 0}.
        // So count = (n - (x+1)) + (1 if x+1 <= n and prepared_{x+1} == 0 else 0).
        // Rank = 1 + count.
        // If x+1 > n (i.e., x >= n), then count = 0, rank = 1.
        // If x+1 <= n, rank = 1 + (n - x - 1) + (prepared_{x+1} ? 0 : 1) = n - x + (prepared_{x+1} ? 0 : 1).
        // So rank = n - x + 1 if we don't prepare for x+1, and rank = n - x if we do prepare for x+1.
        // For x=n: x+1 = n+1 > n, so rank = 1. Formula n - x + 1 = 1, n - x = 0. So we should use n - x + 1 when x+1 > n? Actually if x=n, we don't need to prepare for anyone else, rank is 1. So the formula rank = n - x + 1 works for x=n if we consider that we "prepare" for x+1? No, it's just a special case.
        // Let's unify: rank = n - x + 1 - (prepared_{x+1} ? 1 : 0) for x < n, and rank = 1 for x = n.
        // But note that if we prepare for x+1, rank = n - x. If we don't, rank = n - x + 1.
        // So to minimize rank for a given x, we want to prepare for x+1 if x < n.
        // Therefore, to achieve x wins, we must select a set of x opponents to prepare, and if x < n, we must include opponent x+1 in that set. If x = n, we must include all opponents (which includes opponent n, but there's no n+1).
        // So the condition for achieving x wins with optimal rank is:
        // - If x = 0: rank = n + 1 (since we have 0 wins, all opponents have at least 1 win? Actually opponent 1 has wins = 1 - prepared_1. If we prepare for no one, opponent 1 wins = 1 > 0, opponent 2 wins = 2 > 0, etc. So all n opponents have >0 wins. Rank = n+1.)
        // - If 1 <= x < n: we need to select x opponents including opponent x+1, with sum a_i <= m. Then rank = n - x.
        // - If x = n: we need to select all n opponents, sum <= m. Rank = 1.
        // But wait, is it always possible that by including opponent x+1 we get rank n - x? What if we include opponent x+1 but then some other opponent i > x+1? They already have > x wins regardless. So rank is exactly n - x.
        // So we just need to check for each x from 0 to n, if we can achieve x wins with the required inclusion, and take the minimum rank.
        // For x=0: always possible (sum=0 <= m), rank = n+1.
        // For x=n: need sum of all a_i <= m, rank = 1.
        // For 1 <= x < n: need to pick x opponents including opponent x+1, sum <= m, rank = n - x.

        // Now, how to check efficiently? Same as before but with opponent x+1 instead of x.
        // We can sort opponents by a_i, keep original indices.
        // For each x from 1 to n-1:
        //   required opponent = x+1.
        //   find its position in sorted array.
        //   compute min sum of x elements including it.
        //   if sum <= m, candidate rank = n - x.
        // Also check x=n: sum all <= m -> rank 1.
        // Take min over all valid x.

        // Let's test sample 2 with this corrected logic.
        // n=3, m=2, a=[1,2,3]
        // Sorted: (1,1), (2,2), (3,3)
        // prefix: [1,3,6]
        // x=0: rank=4.
        // x=1: need to include opponent 2 (value 2). pos of opp 2 is 1. x=1, pos=1 >=1, sum = a[1]=2 <=2. Valid. rank = n - x = 3 - 1 = 2. ans=2.
        // x=2: need to include opponent 3 (value 3). pos=2. x=2, pos=2 >=2, sum = prefix[0] + a[2] = 1+3=4 >2. Invalid.
        // x=3: sum all=6 >2. Invalid.
        // min rank=2. Matches sample.

        // Sample 3: n=5, m=0, a=[1,1,1,1,1]
        // x=0: rank=6.
        // x=1: need opp 2, a=1. sum=1 >0. invalid.
        // ...
        // ans=6. Matches.

        // Sample 4: n=4, m=0, a=[0,1,1,1]
        // Sorted: (0,1), (1,2), (1,3), (1,4)
        // x=0: rank=5.
        // x=1: need opp 2 (value 1). sum=1 >0. invalid.
        // But wait, sample output is 4. How? In sample, we have 0 time, but we can still win against opponent 1 because a_1=0. So we get 1 win. Opponent 1 has 0 wins? Let's check: opponent 1 base wins = 0. We prepared for 1, so we win, opponent 1 loses to us. So opponent 1 wins = 0. We have 1 win. Opponent 2: base wins = 1 (beats 1), plus beats us? We didn't prepare for 2, so opponent 2 wins against us. Total wins = 2. Opponent 3: beats 1,2, and us -> 3 wins. Opponent 4: beats 1,2,3, and us -> 4 wins. So wins: opp1=0, us=1, opp2=2, opp3=3, opp4=4. Number of people with strictly more wins than us: opp2, opp3, opp4 -> 3 people. Rank = 4. So we achieved x=1 win, and rank=4. But according to my formula, for x=1, we need to include opponent x+1=2 to get rank n-x = 3? Wait, n=4, x=1, n-x=3. But actual rank is 4. Why? Because we didn't prepare for opponent 2. If we had prepared for opponent 2, we would have 2 wins? No, we only have 0 time, we can only prepare for opponent 1 (cost 0). We cannot prepare for opponent 2 (cost 1). So we have x=1 win, but we did not prepare for opponent 2. According to my earlier deduction, if we don't prepare for opponent x+1, rank = n - x + 1 = 4 - 1 + 1 = 4. That matches! So my formula for rank when we don't prepare for x+1 is n - x + 1. And when we do prepare, it's n - x. So we need to consider both possibilities: for a given x, we might not be able to include opponent x+1, but we can still achieve x wins without including them, resulting in a slightly worse rank. So we should not force inclusion of x+1; rather, we should compute the best rank achievable for each possible x, which depends on whether we can include x+1 or not.

        // So general approach:
        // For each possible number of wins x (0 to n):
        //   We need to select x opponents to beat, sum a_i <= m.
        //   If we can select a set that includes opponent x+1 (if x < n), then rank = n - x.
        //   Else, if we can select any x opponents (without requiring x+1), then rank = n - x + 1 (if x < n) or rank = 1 (if x = n? Actually if x=n, we must include all, so rank=1).
        //   But wait: if x = n, there is no x+1, so rank = 1 always if possible.
        //   For x = 0: rank = n + 1.
        // So we want to find the minimum rank over all achievable x, considering both inclusion and non-inclusion of the critical opponent.

        // However, we can simplify: The best rank for a given x is either n - x (if we can include x+1) or n - x + 1 (if we can't include x+1 but can still get x wins). But note that if we can get x wins without including x+1, we might also be able to get x wins with including x+1 by swapping? Not necessarily, because x+1 might have a large a_i. So we need to check both.

        // But we can just compute for each x the minimum sum of x elements, and also the minimum sum of x elements that include opponent x+1. Then determine possible ranks.

        // Actually, we can think of it as: We want to maximize our "effective" score. Our rank is determined by our wins and whether we beat the opponent at position x+1.
        // Another perspective: The rank is n - x + 1 if we don't beat opponent x+1, and n - x if we do. So beating opponent x+1 is worth 1 rank improvement.
        // So we can try to see the maximum x we can achieve, and also whether we can achieve x while beating x+1.

        // Let's formalize:
        // Let S be the set of opponents we beat. |S| = x.
        // Rank = 1 + number of i with i - I(i in S) > x.
        // As derived, this equals:
        // If x < n:
        //   If x+1 in S: rank = n - x
        //   Else: rank = n - x + 1
        // If x = n: rank = 1.
        // If x = 0: rank = n + 1.

        // So we want to minimize rank. Since n - x is decreasing with x, we generally want larger x. But including x+1 gives a bonus of 1 rank.
        // So we can iterate over possible x from 0 to n, and for each x, we check if we can achieve it, and if so, what's the best rank we can get at that x. Then take min.

        // To check if we can achieve x wins:
        // We can always just take the x smallest a_i. That gives the minimum sum for x wins. If that sum <= m, then we can achieve x wins (without necessarily including x+1). The rank would be n - x + 1 (if x < n) or 1 (if x = n).
        // Additionally, if we can achieve x wins while including x+1, we get better rank n - x. The minimum sum to achieve x wins including x+1 is: if x+1 is among the x smallest, then the sum is the same as the x smallest sum. Otherwise, it's the sum of the smallest x-1 plus a_{x+1}. If that sum <= m, we get the better rank.

        // So we can precompute the sorted array and prefix sums. Then for each x from 1 to n-1:
        //   sum_x = prefix[x-1]  // sum of x smallest
        //   if sum_x <= m: candidate rank = n - x + 1.
        //   // now check inclusion of x+1
        //   find pos of opponent x+1 in sorted array.
        //   if pos < x: sum_inc = sum_x  // already included
        //   else: sum_inc = (x>=2 ? prefix[x-2] : 0) + a[x]  // a is 0-indexed, opponent x+1 is at index x in 0-indexed a? Wait, opponent indices are 1..n. So opponent x+1 has value a[x] if a is 0-indexed.
        //   if sum_inc <= m: candidate rank = n - x.
        // Also check x=n: sum_n = prefix[n-1] <= m -> rank 1.
        // x=0: rank n+1.

        // Take min rank.

        // Let's test sample 4 with this:
        // n=4, m=0, a=[0,1,1,1]
        // Sorted: (0,1), (1,2), (1,3), (1,4)
        // prefix: [0,1,2,3]
        // x=0: rank=5.
        // x=1: sum_x = prefix[0] = 0 <=0 -> candidate rank = n - x + 1 = 4 - 1 + 1 = 4.
        //      inclusion: opponent 2 at pos 1 (0-indexed). pos=1 >=1, sum_inc = a[1] = 1 >0 -> no better rank.
        // x=2: sum_x = prefix[1] = 1 >0 -> invalid.
        // x=3: sum_x = 2 >0.
        // x=4: sum_x = 3 >0.
        // min rank = min(5,4) = 4. Matches sample.

        // Sample 1: n=4, m=401, a=[100,100,200,1]
        // Sorted: (1,4), (100,1), (100,2), (200,3)
        // prefix: [1,101,201,401]
        // x=0: rank=5.
        // x=1: sum_x=1 <=401 -> rank=4-1+1=4. inclusion: opp2 at pos2, sum_inc = a[1]=100 <=401 -> rank=4-1=3. min rank so far=3.
        // x=2: sum_x=101 <=401 -> rank=4-2+1=3. inclusion: opp3 at pos3, sum_inc = prefix[0]+a[2]=1+200=201 <=401 -> rank=4-2=2. min=2.
        // x=3: sum_x=201 <=401 -> rank=4-3+1=2. inclusion: opp4 at pos0 <3, sum_inc = sum_x=201 <=401 -> rank=4-3=1. min=1.
        // x=4: sum_x=401 <=401 -> rank=1. min=1.
        // Output 1. Matches.

        // Sample 5: n=4, m=4, a=[1,2,2,1]
        // Sorted: (1,1), (1,4), (2,2), (2,3)
        // prefix: [1,2,4,6]
        // x=0: rank=5.
        // x=1: sum_x=1 <=4 -> rank=4. inclusion: opp2 at pos2 (value 2), sum_inc = a[1]=2 <=4 -> rank=3. min=3.
        // x=2: sum_x=2 <=4 -> rank=3. inclusion: opp3 at pos3 (value 2), sum_inc = prefix[0]+a[2]=1+2=3 <=4 -> rank=2. min=2.
        // x=3: sum_x=4 <=4 -> rank=2. inclusion: opp4 at pos1 <3, sum_inc = sum_x=4 <=4 -> rank=1. min=1.
        // x=4: sum_x=6 >4.
        // Output 1. Matches sample.

        // So this logic works.

        // Implementation details:
        // We have up to 5e5 total n. Sorting each test case O(n log n) is fine.
        // We need to find the position of each opponent in the sorted array. We can create an array pos[1..n] where pos[i] is the index in the sorted array of opponent i.
        // Since we sort pairs (a_i, i), we can just record pos after sorting.

        // Let's write code.

        vector<int> a(n);
        for (int i = 0; i < n; ++i) cin >> a[i];

        vector<pair<int, int>> b(n);
        for (int i = 0; i < n; ++i) b[i] = {a[i], i};
        sort(b.begin(), b.end());

        vector<int> pos(n);
        for (int i = 0; i < n; ++i) pos[b[i].second] = i;

        vector<long long> pref(n);
        pref[0] = b[0].first;
        for (int i = 1; i < n; ++i) pref[i] = pref[i-1] + b[i].first;

        int ans = n + 1; // x=0

        // x from 1 to n-1
        for (int x = 1; x < n; ++x) {
            long long sum_x = pref[x-1];
            if (sum_x <= m) {
                ans = min(ans, n - x + 1);
            }
            // inclusion of opponent x+1 (0-indexed: opponent index is x)
            int opp = x; // 0-indexed opponent index for x+1
            int p = pos[opp];
            long long sum_inc;
            if (p < x) {
                sum_inc = sum_x;
            } else {
                sum_inc = (x >= 2 ? pref[x-2] : 0) + a[opp];
            }
            if (sum_inc <= m) {
                ans = min(ans, n - x);
            }
        }

        // x
