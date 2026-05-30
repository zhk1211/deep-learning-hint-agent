// Hint3
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n, k;
        cin >> n >> k;
        vector<long long> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }
        
        // We will choose k traps to jump over.
        // If we jump over a trap at index i, we save a[i] damage but we also
        // avoid the bonus damage that would have been added to all later traps.
        // Actually, jumping over trap i means we don't take a[i] + (number of jumps before i).
        // But the number of jumps before i depends on which traps we jump.
        // Alternative perspective: total damage = sum of all a[i] + sum over i of (number of jumps before i).
        // If we jump over a set S of size <= k, then for each i not in S, we take a[i] + (number of jumps in S before i).
        // Total damage = sum_{i not in S} a[i] + sum_{i not in S} (number of jumps before i).
        // The second term is equal to sum_{j in S} (number of traps after j that are not jumped).
        // Let's denote the number of traps after j that are not jumped = (n - j) - (number of jumps after j).
        // This is complicated.
        
        // Better approach: think backwards. If we decide to jump the last trap, we save a[n-1] + (number of jumps before it).
        // But the number of jumps before it is exactly the number of jumps we made among the first n-1 traps.
        // This suggests a greedy strategy: we should jump traps with largest a[i] + (something)?
        
        // Let's derive: Suppose we choose a set S of indices to jump, |S| <= k.
        // Total damage = sum_{i not in S} (a[i] + number of j in S with j < i).
        // = sum_{i=1..n} a[i] - sum_{i in S} a[i] + sum_{i not in S} (number of j in S with j < i).
        // The last term: for each j in S, it contributes to all i > j, i not in S.
        // So it contributes (number of i > j, i not in S) = (n - j) - (number of jumps after j).
        // So total damage = sum a[i] - sum_{j in S} a[j] + sum_{j in S} [ (n - j) - (number of jumps after j) ].
        // = sum a[i] - sum_{j in S} [ a[j] + j - n + (number of jumps after j) ].
        // This still depends on the order.
        
        // Alternative: think about the bonus damage. If we jump over a trap, all subsequent traps get +1.
        // This is equivalent to: each trap i we go through takes a[i] + (number of jumps before i).
        // If we decide to jump k traps, we want to choose which ones to jump to minimize total damage.
        // Consider the contribution of each trap to the total damage if we go through it.
        // If we go through trap i, we pay a[i] + (number of jumps before i).
        // The number of jumps before i is exactly the number of chosen jumps among indices < i.
        // So if we decide to jump some set S, the total damage is sum_{i not in S} a[i] + sum_{i not in S} (number of j in S with j < i).
        // The second sum can be rewritten as sum_{j in S} (number of i > j, i not in S).
        // Let's denote x_j = 1 if we jump j, else 0.
        // Total damage = sum_i a_i (1 - x_i) + sum_{j} x_j * (number of i > j with x_i = 0).
        // = sum_i a_i - sum_i a_i x_i + sum_{j} x_j * ( (n - j) - sum_{i > j} x_i ).
        // = sum_i a_i - sum_i a_i x_i + sum_j x_j (n - j) - sum_j x_j * (sum_{i > j} x_i).
        // The last term: sum_{j} x_j * (sum_{i > j} x_i) = number of pairs (j, i) with j < i and both jumped.
        // This is exactly C(|S|, 2) = s(s-1)/2 where s = |S|.
        // So total damage = sum a_i - sum_{i in S} a_i + sum_{i in S} (n - i) - s(s-1)/2.
        // = sum a_i - sum_{i in S} (a_i + i - n) - s(s-1)/2.
        // Wait, check: sum_{i in S} (n - i) = s*n - sum_{i in S} i.
        // So total damage = sum a_i - sum_{i in S} a_i + s*n - sum_{i in S} i - s(s-1)/2.
        // = sum a_i + s*n - sum_{i in S} (a_i + i) - s(s-1)/2.
        // This is independent of the order of jumps! It only depends on the set S.
        // And we want to minimize total damage, which is equivalent to maximizing sum_{i in S} (a_i + i) for a given s = |S|, because sum a_i, s*n, and s(s-1)/2 are fixed for a given s.
        // But we can choose any s <= k. So we should consider all possible s from 0 to k, and for each s, pick the s indices with the largest (a_i + i) to maximize the subtracted term.
        // Then compute total damage = sum a_i + s*n - sum_{top s} (a_i + i) - s(s-1)/2.
        // Take the minimum over s.
        
        // Let's verify with the example.
        // Example 2: n=4, k=1, a=[5,10,11,5]
        // sum a_i = 31.
        // s=0: damage = 31 + 0 - 0 - 0 = 31.
        // s=1: compute a_i + i (1-indexed): 
        // i=1: 5+1=6
        // i=2: 10+2=12
        // i=3: 11+3=14
        // i=4: 5+4=9
        // Max is 14 (i=3). damage = 31 + 1*4 - 14 - 0 = 31+4-14=21. Correct.
        // Example 3: n=7, k=5, a=[8,2,5,15,11,2,8]
        // sum a = 8+2+5+15+11+2+8 = 51.
        // a_i + i:
        // 1:9, 2:4, 3:8, 4:19, 5:16, 6:8, 7:15.
        // We can choose up to 5. Let's try s=5: pick top 5: 19,16,15,9,8 (indices 4,5,7,1,3 or 6). Sum = 19+16+15+9+8 = 67.
        // damage = 51 + 5*7 - 67 - 5*4/2 = 51+35-67-10 = 9. Correct.
        // Example 4: n=6, k=3, a=[1,2,3,4,5,6]
        // sum a = 21.
        // a_i+i: 2,4,6,8,10,12.
        // s=3: top 3 sum = 12+10+8=30.
        // damage = 21 + 3*6 - 30 - 3*2/2 = 21+18-30-3 = 6. Correct.
        // Example 5: n=1, k=1, a=[7]
        // sum a = 7.
        // a_i+i: 8.
        // s=1: damage = 7 + 1*1 - 8 - 0 = 0. Correct.
        
        // So the solution is:
        // For each test case:
        // 1. Compute total_sum = sum(a).
        // 2. Create array b[i] = a[i] + (i+1)  (using 1-indexed i).
        // 3. Sort b in descending order.
        // 4. Compute prefix sums of sorted b.
        // 5. For s from 0 to k:
        //    damage = total_sum + s * n - prefix_sum[s] - s*(s-1)/2
        //    ans = min(ans, damage)
        // Output ans.
        
        long long total_sum = accumulate(a.begin(), a.end(), 0LL);
        vector<long long> b(n);
        for (int i = 0; i < n; ++i) {
            b[i] = a[i] + (i + 1);
        }
        sort(b.begin(), b.end(), greater<long long>());
        vector<long long> pref(n + 1, 0);
        for (int i = 0; i < n; ++i) {
            pref[i+1] = pref[i] + b[i];
        }
        long long ans = total_sum; // s=0
        for (int s = 1; s <= k; ++s) {
            long long damage = total_sum + 1LL * s * n - pref[s] - 1LL * s * (s - 1) / 2;
            if (damage < ans) ans = damage;
        }
        cout << ans << '\n';
    }
    return 0;
}
