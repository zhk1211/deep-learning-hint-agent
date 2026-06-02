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
        vector<int> sorted_a = a;
        sort(sorted_a.begin(), sorted_a.end());
        
        // prefix sums of sorted array
        vector<long long> pref(n + 1, 0);
        for (int i = 0; i < n; ++i) {
            pref[i + 1] = pref[i] + sorted_a[i];
        }
        
        // max_wins = max k such that pref[k] <= m
        int max_wins = 0;
        while (max_wins < n && pref[max_wins + 1] <= m) {
            ++max_wins;
        }
        
        // If we can win against all, place is 1
        if (max_wins == n) {
            cout << 1 << '\n';
            continue;
        }
        
        // Now we consider if we can beat the opponent at position max_wins
        // (0-indexed in sorted order, but we need original index max_wins)
        // Actually we want to see if we can achieve at least max_wins+1 wins
        // by possibly replacing one opponent with the (max_wins+1)-th opponent.
        // The opponent we need to beat to get place <= max_wins+1 is the one
        // who would have max_wins wins if we don't beat them.
        // In sorted order, the opponent at index max_wins (0-based) has the 
        // (max_wins+1)-th smallest a_i. If we can beat them, we get max_wins+1 wins.
        // But we might need to drop one of the smallest max_wins opponents to fit.
        
        // The opponent we want to beat is the one with a_i = sorted_a[max_wins].
        // But there could be multiple with same value. We need to check if we can
        // select max_wins opponents including this specific one, with sum <= m.
        // We can try to take the smallest max_wins opponents, and if the specific
        // opponent is not among them, we replace the largest among them with it.
        
        // Find the original index of the opponent we want to beat.
        // Actually we just need its a_i value. Let target = sorted_a[max_wins].
        // We'll try to form a set of max_wins+1 opponents that includes this target.
        // The best sum is: sum of smallest max_wins opponents + target - 
        // (if target is among smallest max_wins, we just take smallest max_wins+1)
        // else we take smallest max_wins, remove the largest, add target.
        
        long long sum_needed;
        if (a[max_wins] <= sorted_a[max_wins - 1]) {
            // target is among the smallest max_wins (or equal to the largest of them)
            // Actually we need to check if target is <= sorted_a[max_wins-1]
            // If yes, then the smallest max_wins+1 opponents include target.
            sum_needed = pref[max_wins + 1];
        } else {
            // target is larger than the largest of the smallest max_wins.
            // We take smallest max_wins, remove the largest (sorted_a[max_wins-1]),
            // and add target.
            sum_needed = pref[max_wins] - sorted_a[max_wins - 1] + a[max_wins];
        }
        
        if (sum_needed <= m) {
            // We can achieve max_wins+1 wins, so place is n - (max_wins+1) + 1 = n - max_wins
            cout << n - max_wins << '\n';
        } else {
            // We can only achieve max_wins wins, so place is n - max_wins + 1
            cout << n - max_wins + 1 << '\n';
        }
    }
    return 0;
}
