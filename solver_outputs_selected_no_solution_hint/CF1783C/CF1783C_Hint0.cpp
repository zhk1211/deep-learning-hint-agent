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
        
        // We want to find the maximum number of wins we can get
        // by choosing a subset of opponents with sum of a_i <= m.
        // Since a_i <= 1000, we can use a greedy approach: sort and take smallest.
        vector<int> sorted_a = a;
        sort(sorted_a.begin(), sorted_a.end());
        int wins = 0;
        long long sum = 0;
        for (int x : sorted_a) {
            if (sum + x <= m) {
                sum += x;
                ++wins;
            } else {
                break;
            }
        }
        
        // If we can win against all opponents, place is 1.
        if (wins == n) {
            cout << 1 << "\n";
            continue;
        }
        
        // Now we need to determine our place.
        // Our wins = wins.
        // Opponent i has base wins = i (since beats all j < i).
        // Plus if we beat opponent i, they lose that match, so their total wins = i - 1.
        // If we lose to opponent i, their total wins = i.
        // We want to count how many opponents have strictly more wins than us.
        // Let's consider opponent i (1-indexed). Their wins = i - (we beat them ? 1 : 0).
        // We need to decide which opponents we beat to maximize our place (minimize number).
        // We already know the maximum possible wins we can achieve is 'wins'.
        // To minimize our place, we want to beat opponents with as large index as possible,
        // because beating a higher-index opponent reduces their wins more significantly.
        // However, we must respect the sum constraint.
        // We can try to see if we can achieve a configuration where we beat exactly 'wins' opponents
        // and among them we include opponent (wins+1) if possible.
        // Because if we beat opponent (wins+1), their wins become (wins+1 - 1) = wins,
        // so they tie with us instead of having wins+1 > wins.
        // This can improve our place by 1.
        // So we check if we can select a set of 'wins' opponents with sum <= m that includes opponent (wins+1).
        // If yes, our place is n - wins + 1? Let's derive.
        // Actually, place = 1 + number of opponents with strictly more wins.
        // Opponents with index > wins+1 have at least wins+2 wins (if we don't beat them) or wins+1 (if we beat them).
        // But we can only beat 'wins' opponents, so we cannot beat all of them.
        // Let's compute place directly.
        // If we can beat opponent (wins+1), then opponent (wins+1) has wins = wins.
        // Opponents 1..wins: if we beat some of them, their wins = i-1 <= wins-1, so they have <= wins.
        // Opponents wins+2..n: their wins >= wins+1 (since we can't beat them all, at least some have wins+1 or more).
        // Actually, opponent i for i > wins+1: base wins = i. If we beat them, wins = i-1.
        // Since i-1 >= wins+1, they have > wins. So all opponents with index > wins+1 have > wins.
        // Opponent wins+1: if we beat them, wins = wins (tie). If not, wins = wins+1 (> wins).
        // So number of opponents with strictly more wins = (n - (wins+1)) + (we don't beat wins+1 ? 1 : 0).
        // Thus place = 1 + (n - wins - 1) + (we don't beat wins+1 ? 1 : 0) = n - wins + (we don't beat wins+1 ? 1 : 0).
        // If we beat wins+1, place = n - wins. Else place = n - wins + 1.
        // But wait, what if wins = 0? Then wins+1 = 1. If we beat opponent 1, place = n - 0 = n? But sample 3: n=5, wins=0, place=6. n - wins + 1 = 6. So formula works: if we don't beat opponent 1 (since wins=0, we can't beat anyone), place = n - 0 + 1 = n+1. That matches sample 3.
        // Sample 4: n=4, a=[0,1,1,1], m=0. wins = 1 (beat opponent 1 with a1=0). wins+1 = 2. Can we beat opponent 2? a2=1 > m=0, so no. Place = n - wins + 1 = 4 - 1 + 1 = 4. Matches sample 4.
        // Sample 2: n=3, a=[1,2,3], m=2. wins = 1 (beat opponent 1 or 2? Actually sorted: 1,2,3. sum=1<=2, sum=1+2=3>2, so wins=1). wins+1=2. Can we beat opponent 2? We need to select 1 opponent with sum<=2 that includes opponent 2. Opponent 2 has a2=2 <=2, so yes, we can beat opponent 2 instead of opponent 1. Then place = n - wins = 3 - 1 = 2. Matches sample 2.
        // Sample 1: n=4, m=401, a=[100,100,200,1]. Sorted: 1,100,100,200. sum=1+100+100+200=401<=401, wins=4. wins=n => place=1.
        // Sample 5: n=4, m=4, a=[1,2,2,1]. Sorted: 1,1,2,2. sum=1+1+2=4<=4, wins=3. wins+1=4. Can we beat opponent 4? We need to select 3 opponents with sum<=4 including opponent 4 (a4=1). We can pick opponents with a: 1 (opponent 4), 1 (opponent 1), and 2 (opponent 2 or 3). Sum=4. So yes. Place = n - wins = 4 - 3 = 1. Matches sample 5.
        
        // So the key is to check if we can achieve 'wins' wins while including opponent (wins+1).
        // Note: opponent indices are 1-based. a is 0-indexed. opponent (wins+1) corresponds to a[wins] (since wins is 0-indexed count, wins+1 is index wins in 0-indexed if we consider 1-indexed opponents? Actually opponent i has index i-1 in array a. So opponent (wins+1) is at index wins in a (0-indexed).)
        int idx = wins; // 0-indexed index of opponent (wins+1)
        // We need to check if there exists a subset of size 'wins' with sum <= m that includes a[idx].
        // Since we already know we can pick 'wins' smallest elements, we can try to replace one of them with a[idx] if a[idx] is not already included.
        // The optimal strategy: take the smallest 'wins' elements. If a[idx] is among them, then we already beat opponent (wins+1). So place = n - wins.
        // If not, we can try to remove the largest element from the selected set and add a[idx], and check if the new sum <= m.
        // Because we want to keep the sum as small as possible while including a[idx].
        // So we compute sum of smallest 'wins' elements. Let that be S.
        // If a[idx] <= sorted_a[wins-1] (the largest in the selected set), then a[idx] is already in the set? Not necessarily, because sorted_a may have duplicates. But if a[idx] is less than or equal to the maximum of the selected set, we can swap it in without increasing the sum? Actually, if we remove the maximum and add a[idx], the new sum is S - max_selected + a[idx]. We need this <= m.
        // But we also need to ensure we pick exactly 'wins' opponents. So we just check if S - sorted_a[wins-1] + a[idx] <= m.
        // However, if wins == 0, there is no selected set, so we cannot include anyone. Then place = n - 0 + 1 = n+1. But wait, if wins=0, opponent 1 is idx=0. We can't beat anyone, so place = n+1. That matches.
        // If wins == n, we already handled.
        bool can_beat_wins_plus_1 = false;
        if (wins > 0 && wins < n) {
            long long sum_selected = 0;
            for (int i = 0; i < wins; ++i) {
                sum_selected += sorted_a[i];
            }
            // Check if a[idx] is already in the selected set? We can just check if sum_selected - sorted_a[wins-1] + a[idx] <= m.
            // But if a[idx] is already in the selected set, then sum_selected already includes it, and this swap might not be necessary. However, if a[idx] is already in the set, then sum_selected <= m, and we don't need to swap. The condition sum_selected - sorted_a[wins-1] + a[idx] <= m will also hold because a[idx] <= sorted_a[wins-1], so the new sum <= sum_selected <= m. So it's fine.
            if (sum_selected - sorted_a[wins-1] + a[idx] <= m) {
                can_beat_wins_plus_1 = true;
            }
        }
        
        int place;
        if (wins == n) {
            place = 1;
        } else if (can_beat_wins_plus_1) {
            place = n - wins;
        } else {
            place = n - wins + 1;
        }
        cout << place << "\n";
    }
    return 0;
}
