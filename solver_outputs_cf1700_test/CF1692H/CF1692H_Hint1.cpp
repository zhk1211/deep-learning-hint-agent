// Hint1
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
        
        // We'll find the maximum subarray sum of transformed values
        // where value = +1 if x[i] == a, else -1.
        // We need to find a, l, r maximizing sum.
        // Since a must be one of the values in the array (otherwise all -1, sum negative, better pick any single element),
        // we can process each distinct value separately.
        
        // Group indices by value
        unordered_map<int, vector<int>> pos;
        for (int i = 0; i < n; ++i) {
            pos[x[i]].push_back(i);
        }
        
        int best_a = x[0], best_l = 1, best_r = 1;
        int max_gain = 1; // at least one element gives gain 1 (if we pick l=r, a=x[l])
        
        for (auto& [val, indices] : pos) {
            // We want to find maximum subarray sum in the transformed array for this val.
            // But we can do it efficiently by considering only indices where val occurs,
            // and the gaps between them.
            // Let transformed array b[i] = 1 if x[i]==val else -1.
            // We can compute prefix sums and find max difference.
            // Since n up to 2e5 total, we can just scan the whole array for each val? That would be O(n * distinct) worst-case O(n^2).
            // Instead, we can process each val by scanning its occurrences and using the fact that between occurrences,
            // the prefix sum decreases by 1 per step.
            // We can compute prefix sums at occurrence indices and also consider the minimum prefix sum before each occurrence.
            
            // Let’s compute prefix sums at each index, but we only need to consider positions around occurrences.
            // Actually, we can just compute the maximum subarray sum for this val using a single pass over the array,
            // but we need to do it for each distinct val. Sum of lengths of subarrays? No.
            // However, total n over test cases is 2e5, and number of distinct values could be up to n.
            // If we do O(n) per distinct value, worst-case O(n^2) if all distinct.
            // We need a better approach.
            
            // Alternative: For each val, we can find the maximum subarray sum by considering the array of +1/-1.
            // This is equivalent to: find max (count of val - count of not val) in a subarray.
            // We can compute this by scanning and keeping track of the best difference.
            // But we can do it in O(size of indices) if we use the fact that between occurrences, the value is -1.
            // Let the occurrences be at indices p0, p1, ..., pk-1.
            // The transformed array has 1 at these positions, -1 elsewhere.
            // The maximum subarray sum can be found by considering subarrays that start and end at occurrences? Not necessarily.
            // But we can compute the maximum subarray sum using a standard DP:
            // dp[i] = max(0, dp[i-1] + (x[i]==val ? 1 : -1))
            // We can compute this in O(n) for each val, but that's too slow.
            
            // Instead, we can use the fact that we only care about the maximum gain.
            // We can transform the problem: For a fixed val, we want max over l<=r of (2 * count(val in [l,r]) - (r-l+1)).
            // = max over l<=r of (2 * count - length).
            // This is equivalent to: for each occurrence, we can think of it as +2, and each step costs -1.
            // Let’s define a score array where we start at some l, and we want to maximize the sum.
            // We can use a method similar to maximum subarray sum but with "reset" points.
            
            // Actually, we can compute the maximum subarray sum for each val in O(number of occurrences) by considering the gaps.
            // Let’s define an array of "events": at each index i, the value is +1 if x[i]==val else -1.
            // The prefix sum S[i] = S[i-1] + (x[i]==val ? 1 : -1).
            // We want max_{i<j} (S[j] - S[i-1]).
            // We can compute this by scanning and keeping the minimum S seen so far.
            // To do it per val efficiently, we can process the array once, but we need to separate by val.
            // We can use a map from val to the minimum prefix sum seen so far for that val.
            // But the prefix sum S[i] depends on the val: S[i] for val = number of val up to i - (i - number of val up to i) = 2 * count_val(i) - i.
            // So for each val, we want max over i<j of ( (2*cnt[j] - j) - (2*cnt[i-1] - (i-1)) ).
            // Let f(val, i) = 2 * count_val(i) - i.
            // We want max over i<j of f(val, j) - f(val, i-1).
            // We can compute this by iterating i from 1 to n, and for each val = x[i], we update its count, compute f, and we want to subtract the minimum f seen so far for that val.
            // But we need to consider subarrays that don't necessarily start at an occurrence of val? Actually, the minimum f before i can be at any index, not necessarily where val occurs.
            // However, we can maintain for each val the minimum f seen so far (over all indices from 0 to i-1).
            // Then when we are at index i, for val = x[i], we can compute current f, and candidate = f - min_f[val].
            // But we also need to consider subarrays that end at an index where val does not occur? The maximum subarray sum for a fixed val will always end at an occurrence of val, because if it ends at a -1, we can drop it and increase sum. Similarly, it will start right after a -1 or at an occurrence. So we only need to evaluate candidates when we are at an occurrence of val.
            // So we can do: iterate i from 1 to n, maintain for each val the minimum f seen so far (over all indices 0..i-1). When we process i, we first update min_f for all vals? No, we only need to update min_f for all vals using the current f? That would be O(n * distinct) again.
            // But we can update min_f for all vals by just keeping track of the global minimum of f? No, f is different per val.
            // We need a way to update min_f[val] for all vals at each step. f(val, i) = 2*cnt[val] - i. As i increases, for vals that are not x[i], cnt doesn't change, so f(val, i) = f(val, i-1) - 1. So min_f[val] could decrease by 1 if we consider the new f? Actually, min_f[val] is the minimum over all indices 0..k of f(val, k). When we move to i, the new f(val, i) for val != x[i] is f(val, i-1) - 1. This could be a new minimum. So min_f[val] = min(min_f[val], f(val, i)). But we can't update all vals.
            // However, we can use a trick: instead of maintaining min_f for each val, we can maintain the maximum subarray sum directly using a DP that resets. For each val, we can compute the maximum subarray sum by scanning its occurrences and the gaps.
            // Let's go back to processing each val separately using its occurrences.
            // For a given val, let the occurrences be at indices p[0], p[1], ..., p[m-1].
            // The transformed array b has 1 at p[i], -1 elsewhere.
            // We want max subarray sum.
            // We can compute it by considering subarrays that start at some p[i] or after a gap, and end at some p[j] or before a gap.
            // Actually, we can use the standard maximum subarray sum algorithm on the sequence of gaps and occurrences.
            // Let’s define a running sum. We can start at the first occurrence, but we might want to include some -1s before it if they are compensated by later +1s? No, because including -1s before the first +1 only decreases the sum. So the optimal subarray will start at a +1 (or right after a -1 if we skip it). So we can start our sum at the first occurrence.
            // Then we iterate through the array, but we can skip the -1s between occurrences by subtracting the gap length.
            // Specifically, we can process the occurrences in order:
            // current_sum = 1 (for the first occurrence)
            // max_sum = 1
            // For i from 1 to m-1:
            //   gap = p[i] - p[i-1] - 1
            //   current_sum = current_sum - gap + 1
            //   if current_sum < 1, we can reset to 1 (start new subarray at p[i])
            //   max_sum = max(max_sum, current_sum)
            // But is this correct? Consider that between p[i-1] and p[i], we have gap -1s. So the sum decreases by gap, then we add +1 at p[i]. So net change is -gap + 1.
            // If current_sum becomes <= 0, it's better to start a new subarray at p[i] with sum 1.
            // However, what if the optimal subarray starts at some -1 before p[i-1]? That would only decrease the sum, so not optimal.
            // What if the optimal subarray ends at some -1 after p[i]? That would also decrease the sum, so not optimal.
            // So the maximum subarray sum for this val is indeed achieved by a subarray that starts and ends at occurrences of val.
            // But wait: consider val=4, array: 4, -1, -1, 4. The sum of subarray from first 4 to second 4 is 1 -1 -1 +1 = 0. But if we take just the first 4, sum=1. If we take just the second 4, sum=1. So max is 1. Our method: start at first 4, sum=1. gap=2, current_sum = 1 -2 +1 = 0. max_sum remains 1. Then we reset? If current_sum < 1, we reset to 1. So we would reset to 1 at the second 4. That gives max_sum=1. Correct.
            // What about subarray that includes some -1s after the last occurrence? Not optimal.
            // So this O(m) per val works.
            
            // But we also need to consider the possibility that the best subarray is a single element not equal to val? No, because we are iterating over all vals, and for each val we find the best subarray for that val. The overall best will be the max over all vals. If the best subarray has no occurrences of val, then its sum would be negative (all -1), which is worse than picking a single element equal to some val (sum=1). So we only need to consider vals that appear.
            
            // So we can compute for each val the maximum subarray sum using its occurrences.
            // We also need to keep track of the actual l and r that achieve this max sum.
            // We can do that by storing the start index of the current subarray.
            
            const auto& p = indices;
            int m = p.size();
            int cur_sum = 1;
            int max_sum = 1;
            int start_idx = p[0]; // the start of the current subarray (index of the first occurrence in the subarray)
            int best_start = p[0], best_end = p[0];
            
            for (int i = 1; i < m; ++i) {
                int gap = p[i] - p[i-1] - 1;
                cur_sum = cur_sum - gap + 1;
                if (cur_sum < 1) {
                    cur_sum = 1;
                    start_idx = p[i];
                }
                if (cur_sum > max_sum) {
                    max_sum = cur_sum;
                    best_start = start_idx;
                    best_end = p[i];
                }
            }
            
            // Now we have max_sum for this val. But wait: is max_sum the actual sum of +1/-1? Yes.
            // The money gained is 2^{max_sum}? Actually, starting with 1 dollar, each correct guess doubles, each incorrect halves.
            // So after a subarray with sum S (where S = (#correct) - (#incorrect)), the money is 2^S.
            // We want to maximize S.
            // So we just need the maximum S over all vals and subarrays.
            // Our max_sum is exactly that S.
            
            if (max_sum > max_gain) {
                max_gain = max_sum;
                best_a = val;
                best_l = best_start + 1; // 1-indexed
                best_r = best_end + 1;
            }
        }
        
        cout << best_a << " " << best_l << " " << best_r << "\n";
    }
    
    return 0;
}
