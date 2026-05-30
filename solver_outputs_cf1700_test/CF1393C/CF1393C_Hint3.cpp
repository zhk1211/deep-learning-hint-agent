// Hint3
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
        
        // count frequencies
        unordered_map<int, int> freq;
        for (int x : a) freq[x]++;
        
        // find max frequency and count how many have that frequency
        int max_freq = 0;
        int cnt_max = 0;
        for (auto &p : freq) {
            if (p.second > max_freq) {
                max_freq = p.second;
                cnt_max = 1;
            } else if (p.second == max_freq) {
                cnt_max++;
            }
        }
        
        // The answer is floor((n - cnt_max) / (max_freq - 1)) - 1? Let's derive.
        // Actually, we can place the most frequent elements as "separators".
        // The maximum possible minimum distance is (n - cnt_max) / (max_freq - 1) - 1?
        // Let's think: we have max_freq copies of the most frequent element(s).
        // We need to place them with gaps. The number of gaps between max_freq items is max_freq - 1.
        // In each gap we can put other elements. The total other elements = n - cnt_max.
        // The minimum gap size (in terms of number of other elements between two same most frequent) 
        // will be floor((n - cnt_max) / (max_freq - 1)).
        // But distance is number of eaten patty-cakes strictly between them, which equals gap size.
        // So answer = floor((n - cnt_max) / (max_freq - 1)).
        // Wait, check sample: n=7, freq: 1:2, 7:1, 6:2, 4:2 -> max_freq=2, cnt_max=3 (1,6,4 all have 2).
        // (7-3)/(2-1)=4/1=4. But sample output is 3. So something off.
        // Let's re-evaluate: The most frequent elements are not necessarily the only ones that need spacing.
        // Actually, the problem is about minimum distance between *any* same filling.
        // The bottleneck is the filling with maximum frequency. But we can interleave them.
        // The known result: answer = (n - max_freq) / (max_freq - 1) + something? Let's derive properly.
        
        // Let's use binary search as hinted.
        auto can = [&](int d) -> bool {
            // d is the minimum distance we want to achieve between same fillings.
            // We need to check if we can arrange items such that any two same fillings have at least d items between them.
            // Equivalent to: for each filling with frequency f, we need to place them with at least d gaps.
            // The total length needed for one filling with frequency f is f + (f-1)*d = f*(d+1) - d.
            // But we can interleave different fillings. The necessary and sufficient condition for existence of such arrangement
            // is that the sum over all fillings of min(f, something)? Not exactly.
            // Greedy: place the most frequent elements first, filling slots.
            // Actually, a known greedy: we can always achieve distance d if the number of items with frequency = max_freq
            // is not too large. Let's think.
            // We have slots: we can think of placing the most frequent elements as "anchors" with d spaces between them.
            // The maximum number of items we can place with distance d is: if we have k items of the most frequent type,
            // they occupy k + (k-1)*d = k*(d+1) - d positions. The remaining n - k items can be placed in the (k-1)*d gaps.
            // But we also have other frequent items. The condition is: for the most frequent item with frequency F,
            // we need (F-1)*d + F <= n? Not sufficient because other items also have frequencies.
            // Let's use the standard approach: we want to check if we can arrange such that no two same are within d distance.
            // This is equivalent to: can we schedule tasks with cooldown d? Yes! It's exactly the task scheduler problem.
            // The condition: let max_freq = F, and number of items with frequency F is cnt_max.
            // Then the minimum length needed is (F-1)*(d+1) + cnt_max. We need this <= n.
            // Because we can arrange the most frequent ones with d gaps, and then insert the rest into the gaps or at the end.
            // So can(d) is true iff (F-1)*(d+1) + cnt_max <= n.
            // Let's test with sample 1: F=2, cnt_max=3, n=7. For d=3: (2-1)*(3+1)+3 = 1*4+3=7 <=7 true. For d=4: (1)*(5)+3=8>7 false. So max d=3. Correct.
            // Sample 2: n=8, frequencies: 1:2, 4:3, 6:2, 7:1 -> F=3, cnt_max=1. d=2: (3-1)*(2+1)+1=2*3+1=7<=8 true. d=3: (2)*(4)+1=9>8 false. Output 2. Correct.
            // Sample 3: 3 3 3 -> F=3, cnt_max=1, n=3. d=0: (2)*(1)+1=3<=3 true. d=1: (2)*(2)+1=5>3 false. Output 0. Correct.
            // Sample 4: 2 5 2 3 1 4 -> frequencies: 2:2, others:1. F=2, cnt_max=1. n=6. d=4: (1)*(5)+1=6<=6 true. d=5: (1)*(6)+1=7>6 false. Output 4. Correct.
            // So formula works!
            long long needed = (max_freq - 1LL) * (d + 1LL) + cnt_max;
            return needed <= n;
        };
        
        int lo = 0, hi = n - 2; // maximum possible distance is n-2 (when only two same items at ends)
        int ans = 0;
        while (lo <= hi) {
            int mid = (lo + hi) / 2;
            if (can(mid)) {
                ans = mid;
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
        cout << ans << '\n';
    }
    return 0;
}
