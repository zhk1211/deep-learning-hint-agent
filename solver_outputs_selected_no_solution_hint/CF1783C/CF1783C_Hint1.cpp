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
        // by choosing a subset of opponents with sum <= m.
        // Sort a copy to pick smallest times.
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

        // If we can win against all opponents, place is 1.
        if (max_wins == n) {
            cout << 1 << "\n";
            continue;
        }

        // Now we need to determine our final place.
        // Our wins = max_wins.
        // Opponent i has base wins = i (since beats opponents 1..i-1).
        // Plus possibly a win against us if we don't prepare for them.
        // We want to see if we can beat the opponent with index max_wins (0-based).
        // That opponent has base wins = max_wins.
        // If we can beat them, we can take their place or better.
        // To beat them, we need to have strictly more wins than them,
        // or equal wins but we need to be placed better? 
        // Place is determined by number of contestants with strictly more wins + 1.
        // So if we have same wins as someone, we share the same place.
        // To get place <= max_wins+1, we need to have wins >= max_wins.
        // Actually, if we have max_wins wins, and opponent max_wins (0-based) has max_wins wins,
        // then we tie with them. But there might be others with more wins.
        // Let's think: The opponents with index > max_wins have base wins > max_wins.
        // They will have at least max_wins+1 wins. So there are n - max_wins opponents with more wins.
        // If we have max_wins wins, our place is (n - max_wins) + 1? Not exactly.
        // Wait, opponent i has base wins = i (0-based: beats 0..i-1). So opponent i has i wins from other opponents.
        // If we don't prepare for i, they get a win against us, so total wins = i+1.
        // If we prepare for i, they get 0 wins against us, so total wins = i.
        // We want to minimize our place. Our place = 1 + number of participants with strictly more wins.
        // Participants are us and n opponents.
        // Let our wins = W.
        // For each opponent i, their wins = i + (1 if we don't prepare for i else 0).
        // We need to count how many have wins > W.
        // We can choose which opponents to prepare for (subset sum <= m) to maximize W and also affect their wins.
        // But we already fixed max_wins as the maximum possible W.
        // Now we need to see if we can achieve a place better than n - max_wins + 1?
        // Let's analyze: If we have W wins, then opponents with base wins > W (i >= W+1) will have at least W+1 wins
        // (since even if we beat them, they have i >= W+1 wins from others). So they definitely have > W wins.
        // There are n - (W+1) + 1 = n - W such opponents.
        // Opponents with base wins = W (i = W) have W wins from others. If we beat them, they have W wins, tie with us.
        // If we lose to them, they have W+1 wins, beat us.
        // Opponents with base wins < W have at most W-1 wins from others, so even if they beat us, they have at most W wins,
        // so they cannot have > W wins.
        // So the number of participants with strictly more wins than us is at least n - W (those with i >= W+1).
        // It could be n - W + 1 if opponent i = W beats us (then they have W+1 > W).
        // So our place is either (n - W + 1) or (n - W + 2)? Let's recalc:
        // Place = 1 + count(strictly more wins).
        // If opponent W beats us, count = (n - W) [from i>=W+1] + 1 [from i=W] = n - W + 1. Place = n - W + 2.
        // If we beat opponent W, count = n - W. Place = n - W + 1.
        // So to get place n - W + 1, we need to beat opponent W (i = W).
        // But we already have W wins. To beat opponent W, we must include a_W in our preparation set.
        // However, our max_wins was computed by taking the smallest W elements from sorted a.
        // It might not include a_W if a_W is large.
        // We can try to see if we can achieve W wins while also beating opponent W.
        // If we can, place = n - W + 1. Otherwise, place = n - W + 2.
        // But wait, what if we can achieve W+1 wins? Then max_wins would be W+1, contradiction.
        // So max_wins is the maximum possible wins.
        // Now, can we achieve max_wins wins while also beating opponent max_wins?
        // Opponent max_wins has index max_wins (0-based). We need to include a[max_wins] in our chosen set of size max_wins.
        // If we can find a set of max_wins opponents with sum <= m that includes a[max_wins], then we can tie/beat them.
        // Since we already know the smallest max_wins sum is S = sum of smallest max_wins elements.
        // If a[max_wins] is among those smallest max_wins, then we can just use that set.
        // If not, we can try to replace the largest element in the smallest max_wins set with a[max_wins] and check sum.
        // Actually, we need any set of size max_wins with sum <= m that includes a[max_wins].
        // The minimal sum of a set of size max_wins containing a[max_wins] is:
        // Take a[max_wins] and the smallest max_wins-1 elements from the rest.
        // We can compute that sum and check if <= m.
        // If yes, place = n - max_wins + 1.
        // Else place = n - max_wins + 2.
        // But careful: if max_wins == 0, opponent 0 has base wins 0. If we have 0 wins, we lose to everyone.
        // Then opponent 0 has 1 win (beats us), so they have >0 wins. Count = n (all opponents have >=1 win? Actually opponent 0 has 1 win, others have at least 1 win from beating opponent 0? Wait, opponent 1 beats opponent 0, so has 1 win, etc. All opponents have at least 1 win except possibly? Let's check: opponent 0 has 0 wins from others, but beats us -> 1 win. Opponent 1 beats opponent 0 -> 1 win, plus beats us if we don't prepare. Since we have 0 wins, we don't prepare for anyone, so all opponents beat us. So all opponents have at least 1 win. So count of >0 wins = n. Place = n+1. That matches formula: n - 0 + 2 = n+2? Wait, n - 0 + 2 = n+2, but actual place is n+1. So formula needs adjustment for W=0.
        // Let's re-derive carefully.

        // Let W be our wins.
        // For opponent i (0-indexed):
        // base_wins = i (wins against opponents 0..i-1).
        // against_us = 1 if we don't prepare, else 0.
        // total_wins_i = i + (1 if we don't prepare else 0).
        // We want to count participants with total_wins > W.
        // Participants: opponents 0..n-1 and us.
        // Us: we have W wins, so not > W.
        // Opponents with i >= W+1: base_wins >= W+1. Even if we beat them, total_wins >= W+1 > W. So they count.
        // Number of such opponents: n - (W+1) = n - W - 1.
        // Opponent with i = W: base_wins = W. If we beat them, total_wins = W (not > W). If we lose, total_wins = W+1 > W.
        // Opponents with i < W: base_wins <= W-1. If we lose to them, total_wins <= W. So they cannot have > W.
        // So count = (n - W - 1) + (1 if we lose to opponent W else 0).
        // Place = 1 + count = n - W - 1 + 1 + (lose to W?) = n - W + (lose to W? 1 : 0).
        // If we beat opponent W: place = n - W.
        // If we lose to opponent W: place = n - W + 1.
        // Check W=0: n - 0 = n. If we lose to opponent 0 (which we do, since W=0 means we didn't prepare for anyone), place = n + 1? Wait, n - 0 + 1 = n+1. But earlier we thought place = n+1. Yes, n+1. So formula gives n+1. Correct.
        // If we beat opponent 0 (impossible if W=0 because to beat opponent 0 we need to prepare, which would give us at least 1 win, so W>=1). So for W=0, place = n+1.
        // For W=n: n - n = 0? But place cannot be 0. If W=n, we beat everyone. Opponent n doesn't exist. The count of i >= n is 0. Opponent n-1 has base wins n-1. If we beat them, total = n-1 < n. So count = 0. Place = 1. Formula: n - n = 0? That would be 0, but we need 1. So formula fails for W=n. But we already handled W=n separately (place=1). So fine.

        // So for 0 <= W < n:
        // place = n - W + (we lose to opponent W ? 1 : 0).
        // We want to minimize place, so we want to beat opponent W if possible.
        // Opponent W is the one with index W (0-based). To beat them, we must include a[W] in our preparation set.
        // We already have max_wins = W_max.
        // We need to check if there exists a set of size W_max with sum <= m that includes a[W_max].
        // If yes, place = n - W_max.
        // Else, place = n - W_max + 1.

        int W = max_wins;
        int place;
        if (W == n) {
            place = 1;
        } else {
            // Check if we can achieve W wins including a[W].
            // We need to select W opponents including opponent W, with minimal sum.
            // The minimal sum is a[W] + sum of smallest W-1 elements from all other opponents.
            // We can compute the sum of smallest W elements from sorted_a, call it S.
            // If a[W] is among the smallest W, then S already includes a[W], so we can just use that set.
            // If a[W] is not among the smallest W, then the smallest W elements are the first W in sorted_a.
            // The largest among them is sorted_a[W-1] (0-indexed). We can replace it with a[W] to get a candidate sum.
            // But we need the minimal sum of any set of size W containing a[W].
            // That is: take a[W], and take the smallest W-1 elements from the rest.
            // The rest consists of all elements except a[W]. The smallest W-1 from the rest are:
            // If a[W] is not among the smallest W, then the smallest W elements are exactly the first W in sorted_a.
            // The smallest W-1 from the rest (excluding a[W]) are the first W-1 in sorted_a (since a[W] is >= sorted_a[W-1]).
            // So sum = sum of first W-1 in sorted_a + a[W].
            // If a[W] is among the smallest W, then the smallest W elements include a[W]. The smallest W-1 from the rest would be the first W elements except a[W]. But we can just use the first W elements (which already include a[W]) and their sum is S. So sum = S.
            // So we can compute:
            long long sum_with_W;
            if (W == 0) {
                // W=0: we don't need to include anyone. But we already have W=0, and we lose to opponent 0.
                // Actually, if W=0, we cannot beat opponent 0 because that would require preparing for them, giving us 1 win.
                // So we always lose to opponent 0. Place = n - 0 + 1 = n+1.
                place = n + 1;
            } else {
                // W >= 1
                // Compute sum of smallest W elements.
                long long S = 0;
                for (int i = 0; i < W; ++i) {
                    S += sorted_a[i];
                }
                // Check if a[W] is among the smallest W.
                // Since sorted_a is sorted, the smallest W elements are sorted_a[0..W-1].
                // a[W] is the time for opponent W. We need to see if a[W] <= sorted_a[W-1].
                // Actually, if a[W] <= sorted_a[W-1], then it is among the smallest W.
                // But careful: there could be duplicates. If a[W] == sorted_a[W-1], it might be included or not.
                // However, the minimal sum of a set of size W containing a[W] is:
                // If a[W] <= sorted_a[W-1], then we can just take the first W elements (which include a[W] if we choose appropriately? Not necessarily. The first W elements are the smallest W values. If a[W] is one of the smallest W values, then there exists a multiset of size W with sum S that includes a[W]. Because we can just pick a[W] and the other W-1 smallest from the rest. The sum would be <= S? Actually, if a[W] is among the smallest W, then the sum of the smallest W elements is S. If we force include a[W], we might need to drop the largest of the smallest W, which is sorted_a[W-1]. If a[W] == sorted_a[W-1], then dropping one and including a[W] gives the same sum S. If a[W] < sorted_a[W-1], then dropping sorted_a[W-1] and including a[W] gives sum < S. But we can also just take the first W elements as they are, but they might not contain a[W] if a[W] is not exactly one of those indices. Wait, the first W elements of sorted_a are values, not indices. We don't care which opponents they correspond to, as long as we can pick that set. If a[W] is <= sorted_a[W-1], then the value a[W] is <= the W-th smallest value. That means there are at least W values <= sorted_a[W-1], and a[W] is one of them. So we can form a set of size W with sum <= S that includes a[W]? Actually, the minimal sum of a set of size W containing a specific element is: take that element, and take the smallest W-1 elements from the rest. The sum is a[W] + sum of smallest W-1 elements excluding a[W]. If a[W] is among the smallest W, then the smallest W-1 elements excluding a[W] are exactly the first W elements of sorted_a with one copy of a[W] removed. Their sum is S - a[W] + (something)? Let's compute properly.

                // Let sorted_a[0..n-1] be sorted values.
                // We want min sum of W distinct opponents including opponent W.
                // Let val = a[W].
                // We need to pick W-1 other opponents with minimal sum.
                // The minimal sum of any W-1 opponents from the rest is the sum of the smallest W-1 values in the array after removing one occurrence of val.
                // We can find the position of val in sorted_a. Since there might be duplicates, we can just consider:
                // If val < sorted_a[W-1], then val is strictly smaller than the W-th smallest. So the smallest W elements include val. The smallest W-1 from the rest (excluding val) are the first W elements except val. Their sum is S - val + sorted_a[W]? No, S is sum of first W elements. If val is among them, then S includes val. The sum of the other W-1 elements among the first W is S - val. But we also have the element sorted_a[W] (the (W+1)-th smallest) available. If we take val and the smallest W-1 from the rest, we would take val and the first W elements except one copy of val. That sum is S. Because we just replace the missing copy with val itself. So sum = S.
                // If val == sorted_a[W-1], then val is equal to the W-th smallest. The first W elements are all <= val. If we take val and the smallest W-1 from the rest, we can take the first W-1 elements (which are <= val) and val. Their sum is sum of first W-1 + val. Since sorted_a[W-1] = val, sum of first W-1 + val = S. So again sum = S.
                // If val > sorted_a[W-1], then val is larger than the W-th smallest. The smallest W elements do not include val. The smallest W-1 from the rest (excluding val) are just the first W-1 elements of sorted_a. So sum = sum of first W-1 + val.
                // So in all cases, the minimal sum with val is:
                // if val <= sorted_a[W-1]: sum = S
                // else: sum = S - sorted_a[W-1] + val
                // But wait, if val <= sorted_a[W-1], S already includes val? Not necessarily. S is sum of first W elements. If val is among the first W elements, then S includes val. But if val is equal to sorted_a[W-1] but there are multiple copies, the first W elements might not include the specific opponent W's value if we just take the first W indices? No, we don't care about indices, we just need a set of opponents. If val <= sorted_a[W-1], then there exists a set of W opponents with sum S that includes opponent W? Not automatically, because S is the sum of the W smallest values. If val is one of the W smallest values, we can just pick opponent W and the other W-1 opponents corresponding to the other W-1 smallest values. That set has sum S. So yes, we can achieve sum S.
                // So the condition for being able to include opponent W in a winning set of size W is: min_sum_with_W <= m.
                // min_sum_with_W = S if a[W] <= sorted_a[W-1] else (S - sorted_a[W-1] + a[W]).

                long long min_sum_with_W;
                if (a[W] <= sorted_a[W-1]) {
                    min_sum_with_W = S;
                } else {
                    min_sum_with_W = S - sorted_a[W-1] + a[W];
                }

                if (min_sum_with_W <= m) {
                    place = n - W;
                } else {
                    place = n - W + 1;
                }
            }
        }
        cout << place << "\n";
    }
    return 0;
}
