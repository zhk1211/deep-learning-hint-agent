#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<int> x(n);
        for (int i = 0; i < n; ++i) {
            cin >> x[i];
        }

        // We want to maximize the final money.
        // Money starts at 1, doubles on correct guess, halves on wrong.
        // So final money = 2^{correct - wrong} = 2^{2*correct - (r-l+1)}.
        // Maximizing money is equivalent to maximizing (correct - wrong) = 2*correct - len.
        // For a fixed a, we can transform the array: +1 if x[i]==a, -1 if x[i]!=a.
        // Then we need maximum subarray sum in this transformed array.
        // Since a must be one of the values present in the array (otherwise all -1, subarray sum <=0, worse than picking a present value with at least one +1),
        // we can process each distinct value separately.
        // To do this efficiently, we can group indices by value.
        // For each value, we run Kadane's algorithm on the sequence of indices, but we must account for the -1s between them.
        // Specifically, if we have indices i1 < i2 < ... < ik for value v,
        // the contribution of a segment from index p to q (inclusive) in the original array is:
        // number of v in [p,q] minus number of non-v in [p,q] = 2*cnt_v - (q-p+1).
        // This can be computed by considering the gaps.
        // Let positions be pos[0..m-1]. For a subarray starting at pos[i] and ending at pos[j],
        // length = pos[j] - pos[i] + 1, cnt_v = j - i + 1.
        // Score = 2*(j-i+1) - (pos[j] - pos[i] + 1) = 2j - 2i + 2 - pos[j] + pos[i] - 1 = (2j - pos[j]) - (2i - pos[i]) + 1.
        // So for a fixed start i, we want to maximize (2j - pos[j]) for j >= i.
        // We can iterate from right to left, keeping track of the maximum (2j - pos[j]) seen so far.
        // Then for each i, best score = max_val - (2i - pos[i]) + 1.
        // We also need to consider subarrays that don't start exactly at a v position? 
        // Actually, any optimal subarray will start and end at positions where x[i]==v, because extending into -1s only decreases the sum.
        // So we only need to consider subarrays starting and ending at occurrences of v.
        // We'll keep track of the best score, and the corresponding l and r.
        // l = pos[i], r = pos[best_j] where best_j is the j that gave max (2j - pos[j]).
        // We need to handle the case where the best subarray is a single element (score = 1).
        // Also, we need to output a, l, r (1-indexed).

        int best_a = x[0], best_l = 1, best_r = 1;
        int best_score = 1; // at least 1 if we pick a single correct element

        unordered_map<int, vector<int>> pos_map;
        for (int i = 0; i < n; ++i) {
            pos_map[x[i]].push_back(i);
        }

        for (auto& [val, pos] : pos_map) {
            int m = pos.size();
            // We'll iterate from right to left.
            int max_val = INT_MIN;
            int best_j = -1;
            // We need to find for each i, the max of (2j - pos[j]) for j >= i.
            // We'll compute suffix maximums.
            vector<int> suffix_max(m);
            vector<int> suffix_best_j(m);
            for (int i = m - 1; i >= 0; --i) {
                int cur = 2 * i - pos[i];
                if (i == m - 1 || cur > suffix_max[i + 1]) {
                    suffix_max[i] = cur;
                    suffix_best_j[i] = i;
                } else {
                    suffix_max[i] = suffix_max[i + 1];
                    suffix_best_j[i] = suffix_best_j[i + 1];
                }
            }
            for (int i = 0; i < m; ++i) {
                int j = suffix_best_j[i];
                int score = (2 * j - pos[j]) - (2 * i - pos[i]) + 1;
                if (score > best_score) {
                    best_score = score;
                    best_a = val;
                    best_l = pos[i] + 1;
                    best_r = pos[j] + 1;
                }
            }
        }

        cout << best_a << " " << best_l << " " << best_r << "\n";
    }
    return 0;
}
