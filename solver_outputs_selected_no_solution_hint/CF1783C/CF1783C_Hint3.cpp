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
        
        // We want to find max wins we can get
        // Sort a copy to pick smallest times
        vector<int> b = a;
        sort(b.begin(), b.end());
        
        int max_wins = 0;
        long long sum = 0;
        for (int i = 0; i < n; ++i) {
            if (sum + b[i] <= m) {
                sum += b[i];
                max_wins++;
            } else {
                break;
            }
        }
        
        // If we can win against all, place is 1
        if (max_wins == n) {
            cout << 1 << "\n";
            continue;
        }
        
        // Now we need to check if we can beat the opponent at position max_wins
        // (0-indexed: opponent max_wins has index max_wins in original order)
        // If we can beat them, we can achieve max_wins wins and place = n - max_wins
        // Otherwise, we might need to replace one win to beat them, getting max_wins wins but better place?
        // Actually, if we can't beat opponent max_wins, we might still get max_wins wins but place = n - max_wins + 1?
        // Let's think: The opponent at position max_wins (0-indexed) will have at least max_wins wins (since they beat all lower indices).
        // If we have max_wins wins, we tie with them if we beat them, else they have more wins.
        // So we want to see if we can select a set of max_wins opponents that includes opponent max_wins.
        
        // We already have the smallest max_wins times sum. If opponent max_wins is among them, we are good.
        // If not, we can try to replace the largest in our set with opponent max_wins, if total <= m.
        
        int opponent_idx = max_wins; // 0-indexed, this opponent has index max_wins
        int opponent_time = a[opponent_idx];
        
        // Check if opponent is already in the smallest max_wins
        // We can check by seeing if opponent_time <= b[max_wins-1] (since b is sorted)
        // But careful: if there are duplicates, we need to know if we picked this specific opponent.
        // Better: compute sum of smallest max_wins, and also sum of smallest max_wins-1 + opponent_time.
        
        long long sum_without_max = 0;
        for (int i = 0; i < max_wins - 1; ++i) {
            sum_without_max += b[i];
        }
        
        bool can_beat_opponent = false;
        if (max_wins == 0) {
            // If max_wins == 0, we can't beat anyone, place is n+1? But max_wins=0 means we can't win any.
            // Opponent at index 0 has at least 0 wins? Actually they have at least 0 wins, but we have 0 wins.
            // Place would be n+1 if all have >=1 win, but if some have 0 wins, we tie.
            // However, max_wins=0 means we can't win any, so we have 0 wins.
            // Opponent 0 has 0 wins? They beat no one (since indices >0 only). So they have 0 wins.
            // We tie with them. So place = n - 0 + 1? Let's handle separately.
            // Actually, if max_wins=0, we can't beat opponent 0, but we tie with them.
            // So our place is n - 0 + 1? Wait, if we have 0 wins, and opponent 0 has 0 wins, we tie.
            // Number of people with strictly more wins: those with >=1 win. There are n - 0? No.
            // Let's just use the general logic: we want to see if we can get max_wins wins and beat opponent max_wins.
            // If max_wins=0, opponent_idx=0. We can't beat them because we have 0 wins. So we don't beat them.
            // Then our place is n - max_wins + 1? Let's test with sample 3: n=5, m=0, a=[1,1,1,1,1]. max_wins=0.
            // Opponent 0 has 0 wins? Actually opponent 1 (index 0) beats no one, so 0 wins. We have 0 wins.
            // People with strictly more wins: opponents 2,3,4,5 have at least 1 win (they beat opponent 1). So 4 people.
            // Our place = 4 + 1 = 5? But sample output says 6. Wait, sample 3 output is 6.
            // Let's recalc: n=5 opponents, total participants = 6. Opponents: 1..5.
            // Opponent 1 (index 0) has 0 wins. Opponent 2 has 1 win (beats 1). Opponent 3 has 2 wins. Opponent 4 has 3 wins. Opponent 5 has 4 wins.
            // We have 0 wins. So wins: us=0, opp1=0, opp2=1, opp3=2, opp4=3, opp5=4.
            // Sorted: opp5(4), opp4(3), opp3(2), opp2(1), us(0), opp1(0).
            // Places: opp5=1, opp4=2, opp3=3, opp2=4, us and opp1 tie for 5th place? Actually place = number of people with strictly more wins + 1.
            // For us: people with >0 wins: opp2, opp3, opp4, opp5 -> 4 people. So place = 5.
            // But sample says 6. Why? Let's read problem: "The final place of each contestant is equal to the number of contestants with strictly more wins + 1."
            // If 3 contestants have 5 wins, 1 has 3 wins, 2 have 1 win: first 3 get 1st, fourth gets 4th, two last get 5th.
            // In our case: wins: 4,3,2,1,0,0. Number of people with strictly more than 0: 4. So place = 5. But sample says 6.
            // Wait, sample 3: n=5, m=0, a=[1,1,1,1,1]. Output is 6. That means we got last place (6th). How?
            // Let's recalc opponent wins: Opponent i beats opponent j if i > j.
            // Opponent 1 (i=1): beats no one (since no j < 1). Wins = 0.
            // Opponent 2: beats opponent 1. Wins = 1.
            // Opponent 3: beats 1 and 2. Wins = 2.
            // Opponent 4: beats 1,2,3. Wins = 3.
            // Opponent 5: beats 1,2,3,4. Wins = 4.
            // We lose to all because m=0 and a_i >=1. So we have 0 wins.
            // So wins: [4,3,2,1,0,0]. Sorted: 4,3,2,1,0,0.
            // Places: 4 wins -> 1st, 3 wins -> 2nd, 2 wins -> 3rd, 1 win -> 4th, 0 wins -> 5th? But there are two 0 wins.
            // According to rule: number of contestants with strictly more wins + 1.
            // For a person with 0 wins: contestants with >0 wins: those with 4,3,2,1 -> 4 people. So place = 5.
            // So both 0-win people get 5th place. But sample output says 6. Contradiction.
            // Let's re-read sample 3: "5 0" then "1 1 1 1 1". Output "6". 
            // Maybe I miscounted participants: n+1 participants: you and n opponents. So total 6 participants.
            // If we have 0 wins, and opponent 1 has 0 wins, then there are 4 people with >0 wins. So place = 5. But output is 6.
            // Unless opponent 1 actually has 1 win? No, i > j condition: opponent 1 has index 1, no j < 1, so 0 wins.
            // Wait, maybe the opponents are numbered from 1 to n, and when they play each other, the one with larger number wins.
            // So opponent 1 loses to everyone (2,3,4,5). So 0 wins.
            // Opponent 2 beats 1, loses to 3,4,5 -> 1 win.
            // Opponent 3 beats 1,2, loses to 4,5 -> 2 wins.
            // Opponent 4 beats 1,2,3, loses to 5 -> 3 wins.
            // Opponent 5 beats 1,2,3,4 -> 4 wins.
            // We lose to all -> 0 wins.
            // So wins: 4,3,2,1,0,0. Places: 1,2,3,4,5,5. So we should get 5, not 6.
            // But sample says 6. Let's check sample note: "In the third test case, you have no time to prepare at all, so you'll lose all games. Since each opponent has at least 1 win, you'll take the last place (place 6)."
            // "each opponent has at least 1 win" - that means opponent 1 must have at least 1 win. How?
            // Ah! I misunderstood. "If the opponent i plays against the opponent j, he wins if and only if i > j." 
            // This means when i and j play, the one with larger index wins. So opponent 1 (i=1) plays against opponent 2 (j=2): since 1 < 2, opponent 2 wins. So opponent 1 loses.
            // But opponent 1 also plays against opponent 3,4,5 and loses. So opponent 1 has 0 wins.
            // Why does the note say each opponent has at least 1 win? 
            // Wait, maybe the opponents are numbered from 1 to n, but you are not numbered. The condition "i > j" applies only when two opponents play. 
            // But opponent 1 plays against you. If they beat you, they get 1 win. In sample 3, m=0, you lose to all opponents. So opponent 1 beats you, getting 1 win.
            // Ah! I forgot the matches against you. Each opponent plays against you. If you don't prepare enough, they win. So opponent 1 gets a win against you. So opponent 1 has 1 win (against you) + 0 wins against other opponents = 1 win.
            // Similarly, opponent 2 beats you and opponent 1 -> 2 wins. Opponent 3 beats you, 1,2 -> 3 wins. Opponent 4 -> 4 wins. Opponent 5 -> 5 wins.
            // You have 0 wins. So wins: 5,4,3,2,1,0. Sorted: 5,4,3,2,1,0. Places: 1,2,3,4,5,6. So you get 6th place.
            // That makes sense. So we must include the win against you for each opponent.
            // So opponent i's total wins = (number of opponents j with j < i) + (1 if they beat you else 0).
            // They beat you if you don't prepare enough (i.e., you don't select them).
            // So if you select a set S of opponents to beat, then for each i in S, you beat them, so they don't get the win against you.
            // For i not in S, they get the win against you.
            // So opponent i's wins = (i-1) + (1 if i not in S else 0) = i - (1 if i in S else 0).
            // Your wins = |S|.
            // We want to minimize your place = 1 + number of participants with strictly more wins than you.
            // A participant is either you or an opponent.
            // For an opponent i, they have more wins than you if i - (i in S) > |S|.
            // We can choose S with sum a_i <= m to minimize this count.
            
            // This changes everything. Let's re-derive.
        }
        
        // Let's re-solve with correct understanding.
        // We have n opponents, each has base wins = i-1 (0-indexed: opponent at index i has i wins from opponents with smaller index).
        // Additionally, they get +1 win if we don't beat them.
        // So opponent i's wins = i + (1 if we don't beat them else 0). (using 0-indexed i, so i from 0 to n-1, base wins = i)
        // Our wins = k = number of opponents we beat.
        // We want to minimize place = 1 + count of participants with wins > k.
        // Participants: us (wins = k) and opponents (wins as above).
        // Opponent i has wins > k iff i + (1 if we don't beat them) > k.
        // If we beat them: condition i > k.
        // If we don't beat them: condition i + 1 > k, i.e., i >= k.
        // So opponent i has strictly more wins than us if:
        // - if we beat them: i > k
        // - if we don't beat them: i >= k
        // We want to choose a set S of size k (with sum a_i <= m) to minimize the number of opponents satisfying the above.
        // Let's analyze for a fixed k.
        // For i < k: if we beat them, condition i > k is false (since i < k). If we don't beat them, condition i >= k is false (since i < k). So they never have >k wins. They have at most k wins? Actually if we don't beat them, wins = i+1 <= k, so they tie or have less. So they don't contribute to strictly more.
        // For i = k: if we beat them, condition i > k is false (k > k false). If we don't beat them, condition i >= k is true (k >= k). So they have >k wins only if we don't beat them.
        // For i > k: if we beat them, condition i > k is true. If we don't beat them, condition i >= k is true (since i > k implies i >= k). So they always have >k wins regardless.
        // Therefore, for a given k, the number of opponents with >k wins is:
        // (number of i > k) + (1 if we don't beat opponent k, else 0).
        // Number of i > k is n - 1 - k (since indices 0..n-1, i > k means i from k+1 to n-1, count = n-1-k).
        // So place = 1 + (n - 1 - k) + (1 if we don't beat opponent k else 0) = n - k + (1 if we don't beat opponent k else 0).
        // Wait, check: if we beat opponent k, place = n - k. If we don't, place = n - k + 1.
        // Let's test with sample 2: n=3, m=2, a=[1,2,3]. We can beat opponent 2 (index 1, a=2) -> k=1. Opponent k = opponent 1 (index 1). We beat him? Yes, we selected him. So place = n - k = 3 - 1 = 2. Output is 2. Correct.
        // Sample 1: n=4, m=401, a=[100,100,200,1]. We can beat all -> k=4. Opponent k = opponent 4 (index 3). We beat him. Place = n - k = 4 - 4 = 0? But place can't be 0. Wait, n=4, k=4, place = 4 - 4 = 0? That's wrong. Let's recalc: n=4 opponents, total participants = 5. If we beat all, our wins = 4. Opponents' wins: opponent 0: base 0, we beat him -> 0 wins. Opponent 1: base 1, we beat -> 1 win. Opponent 2: base 2, we beat -> 2 wins. Opponent 3: base 3, we beat -> 3 wins. So wins: us=4, opp0=0, opp1=1, opp2=2, opp3=3. Number of participants with >4 wins: 0. Place = 1. So place = 1. My formula gave n - k = 0. So formula is off by 1? Let's re-derive.
        // Participants: us + n opponents. Total = n+1.
        // Our wins = k.
        // Opponent i wins = i + (1 if not beaten).
        // We want count of participants with wins > k.
        // For i < k: max wins = (k-1) + 1 = k if not beaten, else k-1. So never > k.
        // For i = k: wins = k + (1 if not beaten). So > k iff not beaten.
        // For i > k: wins >= k+1 (since base i >= k+1, even if beaten, wins = i >= k+1 > k). So always > k.
        // So count = (number of i > k) + (1 if not beaten opponent k).
        // Number of i > k: indices k+1 to n-1, count = n - 1 - k.
        // So count = n - 1 - k + (not beaten k).
        // Place = count + 1 = n - k + (not beaten k).
        // For n=4, k=4: i > k means i > 4, none. So count = 0 + (not beaten 4)? But opponent 4 doesn't exist (indices 0..3). So we consider opponent k only if k < n. If k = n, there is no opponent k. Then count = n - 1 - n = -1? That's wrong.
        // If k = n, we beat all opponents. Then for all i, i < n = k. So no opponent has > k wins. Count = 0. Place = 1.
        // So formula should handle k = n separately: place = 1.
        // For k < n: opponent k exists. Count = (n - 1 - k) + (1 if not beaten opponent k). Place = n - k + (not beaten k).
        // Check n=3, k=1: place = 3 - 1 + (not beaten 1). If beaten, place=2. Correct.
        // Check n=4, k=3: suppose we beat 3 opponents but not opponent 3 (index 3). Then place = 4 - 3 + 1 = 2. Let's verify: we beat 3 opponents, say we beat 0,1,2. Our wins=3. Opponent 3: base 3, not beaten -> 4 wins >3. Opponents 0,1,2: beaten -> wins 0,1,2 <=3. Opponent 4 doesn't exist. So only opponent 3 has >3 wins. Count=1, place=2. Correct.
        // If we beat opponent 3 instead of someone else, k=3, beaten opponent 3 -> place = 4 - 3 = 1. Our wins=3, opponent 3 beaten -> wins=3 (tie). No one has >3. Place=1. Correct.
        // So formula: if k == n: place = 1. else: place = n - k + (1 if we don't beat opponent k else 0).
        // We want to minimize place. Since place decreases as k increases, we want maximum possible k.
        // But we also have the option to not beat opponent k even if we could, if it allows a larger k? No, k is the number of wins. If we don't beat opponent k, we might beat someone else to get same k, but place is worse by 1. So for a given k, we prefer to beat opponent k if possible.
        // However, maybe we can achieve a larger k by not beating opponent k? No, k is just the count. To achieve k wins, we need to pick k opponents. If we can pick a set of size k that includes opponent k, we get better place (n-k). If we can only pick a set of size k that does NOT include opponent k, we get place n-k+1.
        // But maybe we can achieve k+1 wins by dropping opponent k and picking someone else? That would increase k, so place might be n-(k+1) = n-k-1, which is better than n-k. So we always want maximum k.
        // So the strategy: find the maximum k such that there exists a set of k opponents with sum <= m.
        // Then, among all sets of size k, we want to know if we can include opponent k. If yes, place = n - k (or 1 if k=n). If no, place = n - k + 1.
        // But wait: what if the maximum k is not unique? We just need to know if there exists a set of size max_k that includes opponent max_k.
        // If max_k = n, place = 1.
        // Else, we need to check if we can select max_k opponents including opponent max_k with sum <= m.
        // How to check? We can compute the sum of the smallest max_k opponents. If opponent max_k is among them, then yes.
        // If not, we can try to replace the largest of those smallest max_k with opponent max_k. If the new sum <= m, then yes.
        // Because any optimal set of size max_k will consist of the smallest max_k times, except possibly swapping one to include opponent max_k.
        // So algorithm:
        // 1. Find max_k: sort a, take smallest until sum > m. max_k = count.
        // 2. If max_k == n: output 1.
        // 3. Else: let opp_time = a[max_k] (0-indexed). Compute sum_smallest = sum of first max_k in sorted a.
        //    If opp_time <= sorted_a[max_k-1] (i.e., it is among the smallest max_k), then we can include it naturally -> place = n - max_k.
        //    Else: we need to replace the largest in the smallest max_k (which is sorted_a[max_k-1]) with opp_time.
        //    New sum = sum_smallest - sorted_a[max_k-1] + opp_time.
        //    If new sum <= m, then we can include opponent max_k -> place = n - max_k.
        //    Else: we cannot include opponent max_k in any set of size max_k -> place = n - max_k + 1.
        // But wait: what if max_k = 0? Then opponent max_k = opponent 0. We need to check if we can beat opponent 0. If we can (a[0] <= m), then max_k would be at least 1. So max_k=0 implies a[0] > m. So we cannot beat opponent 0. Then place = n - 0 + 1 = n+1? But total participants = n+1, so place n+1 is last. Correct.
        // Let's test with sample 4: n=4, m=0, a=[0,1,1,1]. Sorted a: [0,1,1,1]. m=0. Smallest sum: take 0 -> sum=0 <=0, max_k=1. Next 1 -> sum=1 >0, stop. max_k=1.
        // max_k=1 < n. opp_time = a[1] = 1. Sorted_a[0] = 0. opp_time (1) > sorted_a[0] (0). So not in smallest. New sum = 0 - 0 + 1 = 1 > m=0. So cannot include opponent 1. Place = n - max_k + 1 = 4 - 1 + 1 = 4. Output 4. Correct.
        // Sample 5: n=4, m=4, a=[1,2,2,1]. Sorted: [1,1,2,2]. m=4. Sum: 1 (sum=1), 1 (sum=2), 2 (sum=4), next 2 would be 6>4. max_k=3.
        // max_k=3 < n=4. opp_time = a[3] = 1. Sorted_a[2] = 2. opp_time=1 <=2, so it is among smallest? Actually sorted_a[0..2] = [1,1,2]. opp_time=1 is <=2, but is it exactly one of them? We need to check if we can pick a set of size 3 that includes opponent 3 (time 1). Since there are two 1s in sorted, we can pick opponent 3 as one of the 1s. So yes. Place = n - max_k = 4 - 3 = 1. Output 1. Correct.
        // Sample 2: n=3, m=2, a=[1,2,3]. Sorted: [1,2,3]. m=2. Sum: 1 (sum=1), 2 (sum=3>2) stop. max_k=1.
        // max_k=1 < n. opp_time = a[1] = 2. Sorted_a[0] = 1. opp_time=2 >1. New sum = 1 - 1 + 2 = 2 <= m=2. So can include opponent 1. Place = n - max_k = 3 - 1 = 2. Correct.
        // Sample 3: n=5, m=0, a=[1,1,1,1,1]. Sorted: [1,1,1,1,1]. m=0. Sum: first 1 >0, so max_k=0.
        // max_k=0 < n. opp_time = a[0] = 1. Since max_k=0, we have no set. We cannot beat opponent 0. Place = n - 0 + 1 = 6. Correct.
        // Sample 1: n=4, m=401, a=[100,100,200,1]. Sorted: [1,100,100,200]. Sum all = 401 <=401. max_k=4 = n. Place=1. Correct.
        
        // So algorithm works.
        
        // Re-implement correctly:
        vector<int> sorted_a = a;
        sort(sorted_a.begin(), sorted_a.end());
        int k = 0;
        long long sum_smallest = 0;
        for (int i = 0; i < n; ++i) {
            if (sum_smallest + sorted_a[i] <= m) {
                sum_smallest += sorted_a[i];
                k++;
            } else {
                break;
            }
        }
        
        if (k == n) {
            cout << 1 << "\n";
        } else {
            int opp_time = a[k]; // opponent at index k (0-indexed)
            // Check if we can include opponent k in a set of size k
            bool can_include = false;
            if (k == 0) {
                // k=0: we cannot include any opponent, so cannot include opponent 0
                can_include = false;
            } else {
                // sum_smallest is sum of first k elements in sorted_a
                // If opp_time <= sorted_a[k-1], it might be among them, but we need to be careful with duplicates.
                // Actually, if opp_time < sorted_a[k-1], then it's definitely smaller than the largest in our set, so we could swap it in and sum would be <= sum_smallest? Wait, if opp_time < sorted_a[k-1], then replacing the largest with opp_time gives a smaller sum, so definitely <= m. But we need to check if we can actually pick a set of size k that includes opponent k. Since opp_time is smaller than the largest we picked, we can just replace that largest with opponent k, and the sum decreases, so it's valid. So if opp_time <= sorted_a[k-1], we can include it? Not exactly: if opp_time == sorted_a[k-1], it might be that opponent k is exactly the one we already picked, or there are multiple with same value. But if opp_time <= sorted_a[k-1], then the sum of the smallest k elements including opponent k is at most sum_smallest (by replacing the largest with opp_time). So it's <= m. So we can include it.
                // So condition: opp_time <= sorted_a[k-1] OR (sum_smallest - sorted_a[k-1] + opp_time <= m).
                // Actually, if opp_time <= sorted_a[k-1], then sum_smallest - sorted_a[k-1] + opp_time <= sum_smallest <= m, so it's automatically true.
                // So we can just check: sum_smallest - sorted_a[k-1] + opp_time <= m.
                long long new_sum = sum_smallest - sorted_a[k-1] + opp_time;
                if (new_sum <= m) {
                    can_include = true;
                }
            }
            
            int place;
            if (can_include) {
                place = n - k;
            } else {
                place = n - k + 1;
            }
            cout << place << "\n";
        }
    }
    
    return 0;
}
