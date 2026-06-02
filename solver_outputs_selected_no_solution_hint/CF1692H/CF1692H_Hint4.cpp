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
        for (int i = 0; i < n; i++) {
            cin >> x[i];
        }
        
        int best_a = x[0], best_l = 1, best_r = 1;
        int max_profit = 0; // maximum net gain in terms of exponent
        
        int i = 0;
        while (i < n) {
            int j = i;
            while (j < n && x[j] == x[i]) j++;
            // block of identical values from i to j-1
            int val = x[i];
            // We'll find the best subsegment within this block or crossing boundaries
            // Actually, we can treat the whole array as sequence of +1 (if x==a) and -1 (if x!=a)
            // For a fixed a, we want max subarray sum of transformed array.
            // Since a must be one of the values in the optimal segment (otherwise all -1, sum negative),
            // we can try each distinct value as candidate a.
            // But we can do it efficiently by processing blocks of equal values.
            
            // For current value val, we want max subarray sum where +1 for val, -1 otherwise.
            // We can compute this using Kadane on the fly, but we need to reset for each val.
            // Since sum of n over test cases <= 2e5, we can afford O(n) per test case if we do it smartly.
            // We'll process each distinct value separately using two-pointer over blocks.
            
            // We'll collect all blocks of this value and compute max subarray sum using prefix sums.
            // But simpler: we can just do Kadane for each value by scanning the whole array once per value?
            // That would be O(n * distinct_values) worst-case O(n^2). Not good.
            
            // Better: For each value, we can find its occurrences and treat gaps as negative.
            // We can compute max subarray sum for each value by considering only its occurrences and the gaps between them.
            // Let positions of val be p0, p1, ..., pk-1.
            // The transformed array has +1 at these positions, -1 elsewhere.
            // The sum over a segment [L,R] is: (number of val in [L,R]) - (R-L+1 - number of val) = 2*cnt - (R-L+1).
            // We want to maximize this.
            // For a fixed val, we can consider segments that start at some occurrence and end at some occurrence.
            // It's always optimal to start and end at occurrences of val (or just after/before), but we can adjust.
            // Actually, we can think of it as: we want to maximize (2*cnt - len).
            // If we fix the number of val occurrences in the segment, we want the segment to be as short as possible.
            // So we only need to consider segments that start at an occurrence and end at an occurrence.
            // Then len = pj - pi + 1, cnt = j - i + 1.
            // Profit = 2*(j-i+1) - (pj - pi + 1) = (2*(j-i+1) - (pj - pi + 1)).
            // We want max over i<=j.
            // This can be rewritten as: for each j, we want max over i<=j of (2*(j-i+1) - (pj - pi + 1))
            // = 2j + 2 - pj - 1 + max_{i<=j} ( -2i + pi )
            // = 2j - pj + 1 + max_{i<=j} (pi - 2i)
            // So we can compute this in O(k) for each value.
            // Total sum of k over all distinct values is n, so overall O(n) per test case.
            
            // But we need to do this for each distinct value. We can group positions by value using a map.
            // Since sum n <= 2e5, map is fine.
            
            // However, we can also just do it in one pass without map by processing blocks of identical values,
            // but we need to handle multiple blocks of the same value that are separated by other values.
            // The above formula works for any set of positions.
            
            // Let's implement the map approach.
            i = j; // move to next block
        }
        
        // Actually, let's do map approach from scratch.
        unordered_map<int, vector<int>> pos;
        for (int i = 0; i < n; i++) {
            pos[x[i]].push_back(i);
        }
        
        for (auto& [val, p] : pos) {
            int k = p.size();
            int max_val = -1e9, best_i = -1;
            for (int j = 0; j < k; j++) {
                int pi = p[j];
                int cur = pi - 2*j;
                if (j == 0) {
                    max_val = cur;
                    best_i = j;
                } else {
                    if (cur > max_val) {
                        max_val = cur;
                        best_i = j;
                    }
                }
                int profit = 2*j - pi + 1 + max_val;
                if (profit > max_profit) {
                    max_profit = profit;
                    best_a = val;
                    best_l = p[best_i] + 1;
                    best_r = p[j] + 1;
                }
            }
        }
        
        cout << best_a << " " << best_l << " " << best_r << "\n";
    }
    return 0;
}
