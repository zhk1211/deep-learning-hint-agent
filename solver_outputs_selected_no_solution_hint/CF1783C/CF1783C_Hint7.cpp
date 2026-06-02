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
        // by choosing some subset of opponents to beat.
        // Sort a copy to easily pick smallest times.
        vector<int> sorted_a = a;
        sort(sorted_a.begin(), sorted_a.end());
        
        // prefix sums of sorted times
        vector<long long> pref(n + 1, 0);
        for (int i = 0; i < n; ++i) {
            pref[i + 1] = pref[i] + sorted_a[i];
        }
        
        // max_wins = max k such that sum of smallest k a_i <= m
        int max_wins = 0;
        while (max_wins < n && pref[max_wins + 1] <= m) {
            ++max_wins;
        }
        
        // If we can beat all opponents, place is 1
        if (max_wins == n) {
            cout << 1 << "\n";
            continue;
        }
        
        // Now we need to find the best place.
        // The place is determined by number of opponents with strictly more wins than us.
        // Opponent i has wins = (number of opponents j with j < i) + (1 if we beat i else 0).
        // Actually opponent i beats all j < i, so base wins = i - 1.
        // If we beat opponent i, they get i - 1 wins; if we lose, they get i wins.
        // Our wins = number of opponents we beat.
        // We want to minimize our place = 1 + number of opponents with wins > our wins.
        
        // Consider we can beat max_wins opponents. Can we achieve a better place by beating max_wins+1?
        // To beat max_wins+1 opponents, we need to include opponent max_wins+1 (1-indexed in sorted order? No, original indices matter).
        // The key idea: if we can beat opponent (max_wins+1) in original order (i.e., the one with index max_wins in 0-indexed? Wait, opponent numbers are 1..n, and base wins = i-1.
        // Opponent i has base wins i-1. If we beat them, they stay at i-1 wins; if we lose, they get i wins.
        // Our wins = k. Opponents with wins > k are those with base wins > k (if we lose to them) or base wins >= k+1? Let's analyze.
        // For opponent i:
        // - if we beat i: their wins = i-1.
        // - if we lose to i: their wins = i.
        // We want count of opponents with wins > our_wins.
        // Suppose we beat a set S of size k. Our wins = k.
        // For i not in S: wins = i. Condition i > k => i >= k+1.
        // For i in S: wins = i-1. Condition i-1 > k => i > k+1 => i >= k+2.
        // So opponents with wins > k are:
        // - all i >= k+1 not in S (since i >= k+1 => i > k)
        // - all i >= k+2 in S (since i-1 > k => i > k+1)
        // Total = (n - (k+1) + 1) - (number of i in S with i >= k+1) + (number of i in S with i >= k+2)
        // = (n - k) - (number in S with i >= k+1) + (number in S with i >= k+2)
        // = (n - k) - (number in S with i = k+1).
        // So place = 1 + (n - k) - (1 if (k+1) in S else 0) = n - k + 1 - I((k+1) in S).
        // Thus place = n - k + 1 if we don't beat opponent k+1, and n - k if we beat opponent k+1.
        // We want to minimize place, so we want to maximize k, and if possible, include opponent k+1 in S.
        
        // We already know max_wins = maximum k such that we can beat k opponents (any subset).
        // Can we achieve k = max_wins and also include opponent max_wins+1 in S?
        // If we can, place = n - max_wins.
        // Otherwise, place = n - max_wins + 1.
        // But wait: what if we can achieve k = max_wins+1 by some other selection? max_wins is the maximum possible k overall.
        // So max_wins is the absolute maximum wins we can get. So k cannot be max_wins+1.
        // So the best place is either n - max_wins (if we can include opponent max_wins+1 in a set of size max_wins) or n - max_wins + 1.
        
        // How to check if we can include opponent max_wins+1 in a set of size max_wins?
        // Opponent max_wins+1 is the opponent with index = max_wins + 1 (1-indexed). In 0-indexed array, it's a[max_wins].
        // We need to select max_wins opponents including this one, with total time <= m.
        // The optimal way to select max_wins opponents including a specific one is to take that one plus the smallest max_wins-1 among the others.
        // So we can just check: sum of a[max_wins] + sum of smallest max_wins-1 from all other opponents <= m?
        // But careful: max_wins could be 0. If max_wins = 0, then we can't beat anyone. Place = n + 1? Let's check formula: if k=0, place = n - 0 + 1 - I(1 in S). Since k=0, S empty, so I=0, place = n+1. That matches: we have 0 wins, all n opponents have at least 1 win (since each beats those with smaller index, opponent 1 has 0 wins? Wait opponent 1 base wins = 0. If we lose to opponent 1, they have 1 win. So all opponents have >=1 win except possibly opponent 1 if we beat them. But if k=0, we lose to all, so opponent 1 has 1 win. So all n opponents have >=1 win, we have 0, so place = n+1. Correct.)
        // If max_wins = n, we already handled place = 1.
        
        int place;
        if (max_wins == 0) {
            // Can we beat opponent 1? That would be k=1, but max_wins=0 means we can't even beat one opponent (sum of smallest > m). So no.
            place = n + 1;
        } else {
            // Check if we can include opponent max_wins+1 (index max_wins in 0-indexed) in a set of size max_wins.
            int target_idx = max_wins; // 0-indexed, corresponds to opponent number max_wins+1
            int target_time = a[target_idx];
            // We need to pick max_wins-1 other opponents with smallest times, excluding target_idx.
            // We can use the sorted array but need to exclude target_time once.
            // Since sorted_a contains all times, we can find the sum of smallest max_wins times from sorted_a.
            // If target_time is among those smallest max_wins, then the sum of smallest max_wins includes it.
            // We want sum of smallest max_wins-1 excluding target_time.
            // Let sum_smallest_max_wins = pref[max_wins].
            // If target_time <= sorted_a[max_wins-1] (the largest in the smallest max_wins), then target_time is in the smallest max_wins.
            // Then sum of others = sum_smallest_max_wins - target_time + sorted_a[max_wins] (the next smallest).
            // Otherwise, target_time is not in the smallest max_wins, so sum of others = sum_smallest_max_wins - sorted_a[max_wins-1]? Wait, we need to pick max_wins-1 smallest excluding target. If target is not in the smallest max_wins, then the smallest max_wins-1 are just the first max_wins-1 elements of sorted_a. So sum = pref[max_wins-1].
            // But careful: if target_time is exactly equal to some elements, we need to remove one occurrence.
            // Simpler: just compute the sum of the smallest max_wins elements, then if target_time is among them, we replace it with the next element.
            long long sum_with_target;
            if (target_time <= sorted_a[max_wins-1]) {
                sum_with_target = pref[max_wins] - target_time + sorted_a[max_wins];
            } else {
                sum_with_target = pref[max_wins-1] + target_time;
            }
            if (sum_with_target <= m) {
                place = n - max_wins;
            } else {
                place = n - max_wins + 1;
            }
        }
        cout << place << "\n";
    }
    return 0;
}
