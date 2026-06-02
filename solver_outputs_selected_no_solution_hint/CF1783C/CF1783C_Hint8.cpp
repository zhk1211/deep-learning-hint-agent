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
        // We can choose any subset of opponents to beat, sum of a_i <= m
        // But also we need to consider the effect on our place
        
        // Let's sort a copy to easily find how many we can beat if we pick smallest
        vector<int> sorted_a = a;
        sort(sorted_a.begin(), sorted_a.end());
        
        // prefix sums of sorted a
        vector<long long> pref(n + 1, 0);
        for (int i = 0; i < n; ++i) {
            pref[i + 1] = pref[i] + sorted_a[i];
        }
        
        // max wins we can get by picking any subset
        int max_wins = 0;
        while (max_wins < n && pref[max_wins + 1] <= m) {
            ++max_wins;
        }
        
        // If we can beat all opponents, place is 1
        if (max_wins == n) {
            cout << 1 << '\n';
            continue;
        }
        
        // Now we need to find the minimum place.
        // Our place = 1 + number of opponents with strictly more wins than us.
        // Opponent i has base wins = i - 1 (since beats all j < i)
        // If we beat opponent i, his wins remain i-1, else he gets +1 win (against us) -> total i wins.
        // So if we beat opponent i, his wins = i-1; else his wins = i.
        
        // We want to minimize our place = 1 + count of opponents with wins > our_wins.
        // Our wins = number of opponents we beat.
        // Let w = our wins.
        // For opponent i:
        //   if we beat him: his wins = i-1. He has more wins than us if i-1 > w => i > w+1.
        //   if we lose to him: his wins = i. He has more wins than us if i > w.
        // So if we beat opponent i, we reduce his wins by 1, potentially making him not exceed our wins.
        
        // We can choose which opponents to beat, sum a_i <= m.
        // We want to maximize our wins w, but also we might want to beat specific opponents to reduce their wins.
        // The key insight: to minimize place, we should try to achieve w = max_wins or w = max_wins + 1?
        // Actually, we can't get more than max_wins wins because of m constraint.
        // But we might be able to beat opponent (max_wins + 1) specifically, even if we can't beat max_wins+1 opponents in total.
        // Because if we beat opponent (max_wins + 1), his wins become max_wins, which equals our max_wins, so he doesn't have strictly more wins.
        // This could reduce our place by 1 compared to not beating him.
        
        // Let's check if we can beat opponent with index = max_wins + 1 (1-based).
        // If max_wins + 1 <= n, opponent index = max_wins + 1.
        // To beat him, we need a_{max_wins} (0-based) time.
        // We also need to beat max_wins other opponents to achieve w = max_wins + 1? No, we can have w = max_wins and also beat this specific opponent.
        // But if we beat him, we must include him in our set of beaten opponents. So total beaten = some set of size <= max_wins + 1? Actually we can beat at most max_wins opponents if we don't include him, or we can beat a set of size max_wins that includes him? Wait.
        // If we beat opponent (max_wins+1), we need to spend a_{max_wins}. We can still beat at most max_wins opponents total because m constraint.
        // So we can beat a set of size max_wins that includes this opponent, or we can beat a set of size max_wins that does not include him.
        // If we beat him, our wins = max_wins (since we beat max_wins opponents including him). His wins = max_wins. So he doesn't have > wins.
        // If we don't beat him, our wins = max_wins, his wins = max_wins + 1, so he has > wins.
        // So by beating him, we reduce the count of opponents with > wins by 1.
        // Can we always do that? Only if we can find a set of max_wins opponents that includes him and sum <= m.
        // The optimal set of max_wins opponents is the one with smallest sum. If we force include him, we might need to replace the largest element in the smallest max_wins set with him.
        // So we check: if we take the smallest max_wins elements, their sum is pref[max_wins].
        // If we want to include opponent (max_wins+1), we can remove the largest among those max_wins (which is sorted_a[max_wins-1]) and add a_{max_wins}.
        // So new sum = pref[max_wins] - sorted_a[max_wins-1] + a[max_wins] (0-based index max_wins).
        // If this sum <= m, then we can achieve w = max_wins and also beat opponent max_wins+1, reducing our place by 1.
        
        int place;
        if (max_wins < n) {
            int idx = max_wins; // 0-based index of opponent max_wins+1
            long long sum_with_him = pref[max_wins] - sorted_a[max_wins - 1] + a[idx];
            if (sum_with_him <= m) {
                // We can beat opponent max_wins+1 and still have max_wins wins.
                // Then our place = 1 + (number of opponents with wins > max_wins).
                // Opponents with wins > max_wins: those with i > max_wins + 1? Let's compute.
                // If we beat opponent max_wins+1, his wins = max_wins.
                // For i > max_wins+1: we lose to them (since we only beat max_wins opponents, and we included max_wins+1, so we didn't beat them). Their wins = i.
                // They have > max_wins if i > max_wins => i >= max_wins+1. But i starts from max_wins+2? Actually i = max_wins+2, max_wins+3, ..., n.
                // For i = max_wins+2: wins = max_wins+2 > max_wins.
                // So number of opponents with > max_wins = n - (max_wins + 1).
                // Plus maybe some with i <= max_wins? For i <= max_wins, we beat them? Not necessarily. We beat some set of max_wins opponents. It could include some i <= max_wins and exclude others.
                // But if we beat opponent i (i <= max_wins), his wins = i-1 <= max_wins-1 < max_wins, so not > max_wins.
                // If we lose to opponent i (i <= max_wins), his wins = i <= max_wins, so not > max_wins (equal or less). Actually if i = max_wins and we lose, his wins = max_wins, not strictly greater.
                // So only opponents with i > max_wins+1 have strictly more wins.
                // Wait, opponent max_wins+1 we beat, so his wins = max_wins, not >.
                // So count = n - (max_wins + 1).
                place = 1 + (n - (max_wins + 1));
            } else {
                // Cannot beat opponent max_wins+1 while keeping max_wins wins.
                // Then our wins = max_wins.
                // Opponents with > max_wins: opponent max_wins+1 (wins = max_wins+1) and all i > max_wins+1 (wins = i > max_wins+1 > max_wins).
                // So count = 1 + (n - (max_wins + 1)) = n - max_wins.
                place = 1 + (n - max_wins);
            }
        } else {
            // max_wins == n already handled
            place = 1;
        }
        
        cout << place << '\n';
    }
    
    return 0;
}
