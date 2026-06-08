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

        // We want to maximize our wins.
        // Sort opponents by preparation time.
        vector<int> sorted_a = a;
        sort(sorted_a.begin(), sorted_a.end());

        int max_wins = 0;
        long long sum = 0;
        for (int i = 0; i < n; ++i) {
            if (sum + sorted_a[i] <= m) {
                sum += sorted_a[i];
                max_wins++;
            } else {
                break;
            }
        }

        // If we can win against everyone, place is 1.
        if (max_wins == n) {
            cout << 1 << "\n";
            continue;
        }

        // Check if we can win against the opponent with index max_wins.
        // Opponents are 0-indexed in array, but their "strength" is based on index.
        // Opponent i (0-indexed) has i wins against other opponents (since i > j means win).
        // So opponent i has exactly i wins if they lose to us, or i+1 wins if they beat us.
        // Our wins = max_wins.
        // We need to see if we can replace one of the wins with the opponent at position max_wins.
        // The opponent at position max_wins (0-indexed) has index max_wins.
        // If we beat them, they have max_wins wins, we have max_wins+1 wins -> we are ahead.
        // If we don't beat them, they have max_wins+1 wins, we have max_wins wins -> they are ahead.
        // So we want to beat them if possible.
        bool can_beat_key = false;
        if (a[max_wins] <= m) {
            // We can directly prepare for them if we have enough total time.
            // But we already used sum time for max_wins opponents.
            // We need to see if we can fit a[max_wins] by possibly dropping one.
            // Actually, we just need to check if there exists a set of max_wins opponents including this one.
            // Since we sorted, we can try to take the first max_wins-1 smallest and this one.
            long long new_sum = 0;
            int taken = 0;
            bool used_key = false;
            for (int i = 0; i < n && taken < max_wins; ++i) {
                if (!used_key && sorted_a[i] == a[max_wins]) {
                    // We need to be careful with duplicates. Better approach:
                    // Just compute sum of smallest max_wins-1, then add a[max_wins].
                }
            }
            // Simpler: We can just check if we can select max_wins opponents including the one at index max_wins.
            // We can do this by taking the smallest max_wins-1 opponents, and then the key opponent.
            // But we must ensure we don't double count if the key opponent is among the smallest.
            // Let's just compute the sum of the smallest max_wins opponents that include a[max_wins].
            // We can do this by taking the first max_wins elements from sorted_a, and if a[max_wins] is not among them,
            // we replace the largest of those with a[max_wins].
            long long sum_with_key = 0;
            if (max_wins > 0) {
                // Take first max_wins-1 smallest
                for (int i = 0; i < max_wins - 1; ++i) {
                    sum_with_key += sorted_a[i];
                }
                // Add the key opponent
                sum_with_key += a[max_wins];
                // But if a[max_wins] is already among the first max_wins-1, we might have taken it twice?
                // Actually, we are taking first max_wins-1 from sorted_a, then adding a[max_wins].
                // If a[max_wins] is among those first max_wins-1, then we are effectively taking max_wins opponents
                // but one of them is counted twice? No, we are just selecting a set of max_wins opponents.
                // The set consists of the first max_wins-1 smallest and a[max_wins].
                // If a[max_wins] is already in the first max_wins-1, then the set size is max_wins-1? No, because we add it again.
                // We need to ensure we don't double count. Better: we want to select max_wins opponents including a[max_wins].
                // We can just take the smallest max_wins elements from the array, but if a[max_wins] is not included,
                // we swap it with the largest among them.
                // Let's do it properly:
                vector<int> selected;
                for (int i = 0; i < n; ++i) {
                    if (i == max_wins) continue;
                    selected.push_back(a[i]);
                }
                sort(selected.begin(), selected.end());
                sum_with_key = a[max_wins];
                for (int i = 0; i < max_wins - 1; ++i) {
                    sum_with_key += selected[i];
                }
            } else {
                // max_wins == 0, we can't beat anyone. But we already handled max_wins == n.
                // If max_wins == 0, we can't beat the key opponent unless a[max_wins] <= m.
                sum_with_key = a[max_wins];
            }
            if (sum_with_key <= m) {
                can_beat_key = true;
            }
        }

        if (can_beat_key) {
            // We have max_wins + 1 wins? Wait, if we beat the key opponent, we have max_wins wins?
            // Let's recalc: max_wins is the maximum number of wins we can get without considering the key opponent.
            // If we can beat the key opponent, we can have max_wins wins that include beating the key opponent.
            // Then our wins = max_wins. The key opponent (index max_wins) will have max_wins wins (since they lose to us).
            // Other opponents with index < max_wins have at most max_wins wins (if they beat us) or max_wins-1 (if we beat them).
            // Opponents with index > max_wins have at least max_wins+1 wins.
            // So our place is n - max_wins + 1? Let's think.
            // Number of people with strictly more wins than us: those with index > max_wins (they have at least max_wins+1 wins).
            // There are n - max_wins - 1 such people.
            // Also, the key opponent has exactly max_wins wins, same as us, so not strictly more.
            // So place = (n - max_wins - 1) + 1 = n - max_wins.
            // But wait, if we can beat the key opponent, we have max_wins wins. The key opponent has max_wins wins.
            // Opponents with index < max_wins: if we beat them, they have index wins; if we lose, they have index+1 wins.
            // Since index < max_wins, index+1 <= max_wins. So they have at most max_wins wins.
            // So no one with index < max_wins can have > max_wins wins.
            // So only those with index > max_wins have > max_wins wins. There are n - max_wins - 1 of them.
            // So place = (n - max_wins - 1) + 1 = n - max_wins.
            cout << n - max_wins << "\n";
        } else {
            // We cannot beat the key opponent. Then our wins = max_wins.
            // The key opponent has max_wins + 1 wins (since they beat us).
            // Opponents with index > max_wins have at least max_wins+1 wins.
            // Opponents with index < max_wins have at most max_wins wins.
            // So people with strictly more wins: key opponent + those with index > max_wins = 1 + (n - max_wins - 1) = n - max_wins.
            // Place = (n - max_wins) + 1 = n - max_wins + 1.
            cout << n - max_wins + 1 << "\n";
        }
    }
    return 0;
}
