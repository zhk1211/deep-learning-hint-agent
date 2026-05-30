// Hint2
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        int n;
        cin >> n;
        vector<int> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }

        map<int, int> freq;
        for (int x : a) freq[x]++;

        int max_freq = 0;
        for (auto &p : freq) {
            max_freq = max(max_freq, p.second);
        }

        int cnt_max = 0;
        for (auto &p : freq) {
            if (p.second == max_freq) cnt_max++;
        }

        // The maximum possible minimum distance is:
        // We can place the most frequent elements as spacers.
        // The answer = (n - cnt_max) / (max_freq - 1) - 1? Let's derive properly.
        // Actually, the known formula: answer = (n - cnt_max) / (max_freq - 1) - 1? No.
        // Let's think: we want to maximize the minimum distance between same fillings.
        // The bottleneck is the most frequent filling. Suppose it appears max_freq times.
        // We need to place them with at least d other elements between them.
        // So we need at least (max_freq - 1) * d other elements.
        // The total other elements available = n - max_freq.
        // So (max_freq - 1) * d <= n - max_freq.
        // But if there are multiple fillings with max_freq, they can be interleaved.
        // Actually, if there are cnt_max fillings with max_freq, we can place them in a cycle.
        // The maximum d such that we can arrange is: d = (n - cnt_max) / (max_freq - 1) - 1? Wait.
        // Let's use binary search as hinted.

        auto can = [&](int d) -> bool {
            // d is the minimum distance between same fillings.
            // For the most frequent filling(s), we need to place them with at least d gaps.
            // If max_freq == 1, then d can be anything? But problem guarantees at least two same.
            // So max_freq >= 2.
            // We need to check if we can arrange all elements such that distance >= d.
            // Equivalent: we can place the most frequent elements first.
            // The required number of other elements to separate max_freq items is (max_freq - 1) * d.
            // But if there are multiple max_freq fillings, they can share the gaps? Actually, they can be placed in the gaps of each other.
            // A known greedy: we can always arrange if we have enough "other" elements.
            // More precisely, we can think of placing all elements in a sequence.
            // The maximum possible minimum distance is floor((n - cnt_max) / (max_freq - 1)) - 1? Let's test with examples.
            // Example 1: n=7, freq: 1:2, 4:2, 6:2, 7:1. max_freq=2, cnt_max=3. (7-3)/(2-1)=4. Answer is 3. So 4-1=3.
            // Example 2: n=8, freq: 1:2, 4:3, 6:2, 7:1. max_freq=3, cnt_max=1. (8-1)/(3-1)=7/2=3. Answer is 2. So 3-1=2.
            // Example 3: n=3, freq: 3:3. max_freq=3, cnt_max=1. (3-1)/(3-1)=2/2=1. Answer is 0. So 1-1=0.
            // Example 4: n=6, freq: 2:2, 5:1, 3:1, 1:1, 4:1. max_freq=2, cnt_max=1. (6-1)/(2-1)=5. Answer is 4. So 5-1=4.
            // So formula: answer = (n - cnt_max) / (max_freq - 1) - 1.
            // But wait, is it always true? Let's test with a case: n=5, a = [1,1,1,2,2]. max_freq=3, cnt_max=1. (5-1)/(3-1)=4/2=2. 2-1=1. Can we achieve distance 1? Yes: 1,2,1,2,1 -> distances: between 1's: 1 and 1. So min distance = 1. Can we achieve 2? Need (3-1)*2=4 other elements, but we have only 2. So max is 1. Formula gives 1. Correct.
            // Another: n=6, a=[1,1,1,2,2,2]. max_freq=3, cnt_max=2. (6-2)/(3-1)=4/2=2. 2-1=1. Can we achieve 1? Yes: 1,2,1,2,1,2 -> distances 1. Can we achieve 2? Need to place three 1's with distance >=2: 1 _ _ 1 _ _ 1 -> requires 4 other elements, but we have 3 other (the 2's). Not enough. So max=1. Formula gives 1.
            // So the formula seems correct. But let's implement binary search as hinted, just to be safe and follow hints.
            // Binary search for d from 0 to n.
            // Check function: can we arrange with min distance >= d?
            // We can use a greedy: place the most frequent elements first, then fill gaps.
            // Actually, a known check: we need (max_freq - 1) * d + cnt_max <= n? Not exactly.
            // Let's derive: We have cnt_max elements of the most frequent type(s). We can place them in a cycle.
            // The total number of elements is n. If we place the max_freq elements with d gaps, the total length needed is max_freq + (max_freq - 1) * d = max_freq * (d+1) - d.
            // But we have other elements that can be placed in the gaps. The condition for feasibility is that the total number of other elements is at least (max_freq - 1) * d.
            // However, if there are multiple max_freq fillings, they can be placed in the gaps of each other, so the required other elements is reduced by (cnt_max - 1) because the first of each max_freq filling can be placed without needing extra gaps? Actually, the standard formula is: we need at least (max_freq - 1) * d + cnt_max <= n? Let's test.
            // Example 1: max_freq=2, cnt_max=3, d=3. (2-1)*3 + 3 = 1*3+3=6 <=7. True. d=4: (1)*4+3=7 <=7. True. But answer is 3. So d=4 would be true by this condition, but actual answer is 3. So condition is not sufficient.
            // Let's re-evaluate. The correct condition: We can always arrange if we have enough "other" elements to separate the most frequent ones. But if we have multiple max_freq, they can be interleaved. The maximum d is floor((n - max_freq) / (max_freq - 1))? No.
            // Let's stick to the derived formula: answer = (n - cnt_max) / (max_freq - 1) - 1.
            // Let's test with a case where max_freq is not the only bottleneck. Suppose n=10, a: 1:4, 2:3, 3:3. max_freq=4, cnt_max=1. (10-1)/(4-1)=9/3=3. 3-1=2. Can we achieve 2? We need to place four 1's with distance >=2: 1 _ _ 1 _ _ 1 _ _ 1 -> requires 3 gaps of size 2 = 6 other elements. We have 6 other elements (three 2's and three 3's). So yes, we can fill exactly. Can we achieve 3? Need 3 gaps of size 3 = 9 other elements, but we have only 6. So max=2. Formula gives 2.
            // What if we have multiple max_freq? n=10, a: 1:4, 2:4, 3:2. max_freq=4, cnt_max=2. (10-2)/(4-1)=8/3=2. 2-1=1. Can we achieve 1? Yes. Can we achieve 2? We have two fillings with 4 each. We can interleave them: 1,2,1,2,1,2,1,2,3,3 -> distances between 1's: 1,1,1 (min=1). To achieve distance 2, we need to place four 1's with gaps of 2: 1 _ _ 1 _ _ 1 _ _ 1. That's 10 positions. We have four 2's and two 3's. We can place 2's in some gaps, but we need to place four 2's as well with distance >=2. They would require similar gaps. It's impossible to have both sets of four with distance 2 in 10 slots. So max=1. Formula gives 1.
            // So the formula seems robust. But let's implement binary search as per hint, using a check function that is correct.
            // The check function: can we arrange with min distance >= d?
            // We can use a greedy: sort frequencies descending. Try to place elements one by one in a sequence of length n, maintaining the last position of each filling.
            // But that's O(n) per check, and binary search O(log n), total O(n log n) per test case, sum n <= 1e5, so fine.
            // However, we can also just compute the formula directly in O(n) total. But hints say binary search and greedy. Let's do binary search with a simple check.
            // Check function: We want to see if we can schedule all tasks with cooldown d. This is equivalent to task scheduler problem: given frequencies, can we arrange with distance >= d between same tasks?
            // The known condition: let max_freq = max frequency. The minimum number of slots required is (max_freq - 1) * (d + 1) + count_max, where count_max is number of tasks with max_freq. If this <= n, then possible? But we saw earlier that for d=4 in example 1, (2-1)*(4+1)+3 = 1*5+3=8 > 7, so false. For d=3: (1)*(3+1)+3=4+3=7 <=7, true. So that condition works for example 1. Let's test example 2: max_freq=3, cnt_max=1. d=2: (3-1)*(2+1)+1 = 2*3+1=7 <=8, true. d=3: 2*4+1=9 >8, false. So max d=2. Example 3: max_freq=3, cnt_max=1. d=0: (2)*(0+1)+1=2+1=3 <=3, true. d=1: 2*2+1=5 >3, false. So max d=0. Example 4: max_freq=2, cnt_max=1. d=4: (1)*(4+1)+1=5+1=6 <=6, true. d=5: 1*6+1=7 >6, false. So max d=4.
            // This condition matches all examples! And it's the standard task scheduler formula.
            // So check(d): return (max_freq - 1) * (d + 1) + cnt_max <= n;
            // Then binary search for max d in [0, n].
            // Let's verify with the earlier case n=10, a: 1:4,2:4,3:2. max_freq=4, cnt_max=2. d=1: (4-1)*(1+1)+2 = 3*2+2=8 <=10, true. d=2: 3*3+2=11 >10, false. So max d=1. Correct.
            // So this check is correct and simple.
            // Why does it work? Because we can always arrange the most frequent tasks with gaps of size d, and then insert the remaining tasks into the gaps or at the ends. The formula gives the minimum length of a schedule that satisfies the distance constraint for the most frequent tasks. If that minimum length <= n, we can always pad the extra slots with the remaining tasks without violating the constraint (since they have lower frequency, they won't exceed the gaps). This is a known result.
            // So we can binary search d.

            int low = 0, high = n, ans = 0;
            while (low <= high) {
                int mid = (low + high) / 2;
                long long needed = 1LL * (max_freq - 1) * (mid + 1) + cnt_max;
                if (needed <= n) {
                    ans = mid;
                    low = mid + 1;
                } else {
                    high = mid - 1;
                }
            }
            cout << ans << '\n';
        }
    }
    return 0;
}
