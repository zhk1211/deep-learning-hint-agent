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
        // Sort opponents by preparation time
        vector<pair<int, int>> b(n);
        for (int i = 0; i < n; ++i) {
            b[i] = {a[i], i};
        }
        sort(b.begin(), b.end());

        int wins = 0;
        long long sum = 0;
        vector<bool> used(n, false);
        for (int i = 0; i < n; ++i) {
            if (sum + b[i].first <= m) {
                sum += b[i].first;
                used[b[i].second] = true;
                ++wins;
            } else {
                break;
            }
        }

        // If we can win against all, place is 1
        if (wins == n) {
            cout << 1 << '\n';
            continue;
        }

        // Check if we can replace one chosen opponent with the (wins+1)-th opponent
        // to get wins+1 wins, which might improve place
        bool can_extra = false;
        if (wins < n) {
            // Try to include opponent with index wins (0-based) which is the one we haven't beaten yet
            // that would give us wins+1 wins
            int target_idx = wins; // 0-based index in original order
            // We need to beat this opponent
            // We have sum time used, we need to free up at least a[target_idx] - (m - sum)
            // Actually we need total sum + a[target_idx] <= m, but we already used sum.
            // So we need to remove some already chosen opponent with time >= a[target_idx] - (m - sum)
            // Or simpler: we can try to see if we can achieve wins+1 wins by selecting
            // the first wins+1 smallest a_i, but we must include the target opponent.
            // Since we sorted by a_i, the first wins+1 smallest might not include target.
            // We can check if we can pick target and the smallest wins others.
            
            // Let's compute sum of smallest wins elements excluding target, plus a[target_idx]
            long long new_sum = a[target_idx];
            int cnt = 1;
            for (int i = 0; i < n && cnt <= wins; ++i) {
                if (b[i].second != target_idx) {
                    new_sum += b[i].first;
                    ++cnt;
                }
            }
            if (new_sum <= m) {
                can_extra = true;
            }
        }

        int my_wins = wins;
        if (can_extra) {
            my_wins = wins + 1;
        }

        // Now compute place
        // Opponent i has at least i wins (since beats all j < i)
        // Actually opponent i (1-indexed) beats opponents 1..i-1, so has at least i-1 wins.
        // Plus possibly beats you if you didn't prepare for them.
        // So wins_i = (i-1) + (you didn't beat i ? 1 : 0)
        // We need number of opponents with strictly more wins than my_wins.
        // Opponent i has more wins if:
        // If you beat i: wins_i = i-1, so i-1 > my_wins => i > my_wins + 1
        // If you didn't beat i: wins_i = i, so i > my_wins => i > my_wins
        // So opponents with index > my_wins (1-indexed) always have more wins.
        // Opponent with index = my_wins + 1: if you beat him, his wins = my_wins, not more.
        // If you didn't beat him, his wins = my_wins + 1 > my_wins, so he has more.
        // Opponents with index <= my_wins: max wins is my_wins (if you didn't beat them) or my_wins-1 (if you beat them), so not more.
        
        int place = my_wins + 1; // base: number of opponents with index > my_wins is n - my_wins, so place = (n - my_wins) + 1? Wait.
        // Let's count strictly more wins.
        int more_wins = n - my_wins; // opponents with index > my_wins (1-indexed) always have > my_wins wins.
        // Check opponent with index = my_wins + 1 (if exists)
        if (my_wins < n) {
            int idx = my_wins; // 0-based index of opponent number my_wins+1
            // Did we beat him?
            bool beat = used[idx] || (can_extra && idx == wins); // if we got extra win by beating him
            if (!beat) {
                // He has my_wins + 1 wins > my_wins
                more_wins++;
            }
        }
        place = more_wins + 1;
        cout << place << '\n';
    }
    return 0;
}
