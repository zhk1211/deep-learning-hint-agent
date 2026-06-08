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

        if (k >= n) {
            long long sum = accumulate(a.begin(), a.end(), 0LL);
            cout << sum << '\n';
            continue;
        }

        // dp[i][j] = min sum for prefix up to i, with j operations used,
        // and the last element is a[i] (we don't change it further after i)
        // Actually we need to consider that operations can change elements to neighbor values.
        // The key insight: after operations, the array consists of blocks of equal values.
        // Each operation can extend a block by one element.
        // We can think of choosing some elements to be "leaders" and others to be changed to match a neighbor.
        // Equivalent to: we can select up to k elements to NOT be changed (or changed fewer times?).
        // Better: we can perform at most k operations. Each operation reduces sum by (a[i] - min(neighbor)).
        // But the problem is more subtle: operations can cascade.
        // Known solution: dp[i][j] = minimum sum for prefix i with j operations, where we consider the last element fixed.
        // Let dp[i][j] be min sum for first i elements using j operations, and a[i] is unchanged.
        // Then we can transition from some previous p < i, where we change all elements between p+1 and i-1 to a[p] or a[i].
        // The number of operations needed is (i - p - 1).
        // So dp[i][j] = min over p < i, j >= (i-p-1) of dp[p][j - (i-p-1)] + (i-p-1)*min(a[p], a[i]) + a[i].
        // Base: dp[0][0] = 0, dp[0][>0] = INF.
        // Answer: min over i, j <= k of dp[i][j] + (n-i)*a[i] (change suffix to a[i]).
        // But careful: we can also change prefix before first fixed element.
        // So we can add a dummy element at index 0 with value a[0]? Better: handle prefix separately.
        // Actually we can consider dp[i][j] as min sum for prefix ending at i with a[i] fixed, and we have used j operations.
        // Then for the prefix before the first fixed element, we can change all to that fixed element.
        // So we can initialize dp[i][i-1] = i * a[i] (change all previous to a[i]).
        // But k <= 10, n up to 3e5, so O(n*k^2) is too slow? k is small, but n is large.
        // We need O(n*k) or O(n*k*log) maybe.
        // Since k <= 10, we can do O(n*k^2) = 3e5 * 100 = 3e7, which is acceptable in C++ with optimization.
        // But we need to be careful with transitions: for each i, we can look back up to k steps because j <= k.
        // So we only need to consider p such that i-p-1 <= k, i.e., p >= i-1-k.
        // So inner loop over p is at most k+1.
        // Then total complexity O(n*k^2) = 3e5 * 100 = 3e7, fine.

        const long long INF = 1e18;
        // dp[i][j] for i from 0 to n, j from 0 to k.
        // We'll use 1-indexed for convenience: dp[i][j] for first i elements, last fixed at i.
        vector<vector<long long>> dp(n + 1, vector<long long>(k + 1, INF));
        // Base: dp[0][0] = 0? But we need to start with some fixed element.
        // Instead, we can initialize dp[i][i-1] = prefix sum changed to a[i]? But i-1 might exceed k.
        // Better: iterate i from 1 to n, and for each i, consider it as the first fixed element.
        // Then operations used = i-1 (change all before to a[i]).
        // So if i-1 <= k, dp[i][i-1] = i * a[i-1] (0-indexed a).
        for (int i = 1; i <= n; ++i) {
            int ops = i - 1;
            if (ops <= k) {
                dp[i][ops] = (long long)i * a[i-1];
            }
        }

        // Now transitions
        for (int i = 1; i <= n; ++i) {
            for (int j = 0; j <= k; ++j) {
                if (dp[i][j] == INF) continue;
                // try to extend to next fixed element p > i
                for (int p = i + 1; p <= n; ++p) {
                    int ops_needed = p - i - 1;
                    if (j + ops_needed > k) break; // since ops_needed increases with p, we can break early
                    long long add = (long long)ops_needed * min(a[i-1], a[p-1]) + a[p-1];
                    if (dp[i][j] + add < dp[p][j + ops_needed]) {
                        dp[p][j + ops_needed] = dp[i][j] + add;
                    }
                }
            }
        }

        long long ans = INF;
        // After last fixed element at i, we can change the suffix (n-i elements) to a[i-1]
        for (int i = 1; i <= n; ++i) {
            for (int j = 0; j <= k; ++j) {
                if (dp[i][j] == INF) continue;
                int rem_ops = k - j;
                int suffix_len = n - i;
                int ops_used_suffix = min(rem_ops, suffix_len);
                // We can change up to ops_used_suffix elements in the suffix to a[i-1]
                // Actually we can change all suffix elements to a[i-1] if we have enough operations.
                // But we might not need to change all; we can leave some unchanged if we run out of ops.
                // The sum of suffix: we change ops_used_suffix elements to a[i-1], the rest remain as original.
                // But wait: the operation allows changing an element to its neighbor's value.
                // To change a suffix element to a[i-1], we need to propagate the value.
                // If we have a block of fixed value at i, we can extend it to the right by using one operation per element.
                // So we can change up to rem_ops elements to the right to a[i-1].
                // The remaining suffix elements (if any) stay as original.
                // So we need to compute suffix sum efficiently.
                // We can precompute suffix sums of original array.
                // Let suffix_sum[i] = sum of a[i..n-1] (0-indexed).
                // Then for fixed i (1-indexed), the original suffix sum is suffix_sum[i] (where suffix_sum[i] = sum from i to n-1).
                // If we change x = min(rem_ops, n-i) elements to a[i-1], the new suffix sum = x * a[i-1] + (original suffix sum of the rest).
                // The rest starts at index i + x (1-indexed) = i + x (0-indexed: i-1 + x + 1? Let's be careful).
                // Let's use 0-indexed for array a.
                // i is 1-indexed fixed position. a[i-1] is the value.
                // Suffix original indices: i to n-1 (0-indexed). Length = n - i.
                // We can change first x elements of this suffix to a[i-1].
                // The remaining suffix is from index i + x to n-1.
                // So we need sum of a from i+x to n-1.
                // We can precompute suffix sums: suff[idx] = sum from idx to n-1.
                // Then total = dp[i][j] + x * a[i-1] + (suff[i+x] if i+x < n else 0).
                // But wait: we might also not change all possible; we can choose to change fewer? Since we want min sum, we should change as many as possible if a[i-1] is smaller than the original values. But if a[i-1] is larger, we might not want to change. However, we can always choose to not use all operations. So we should consider x from 0 to min(rem_ops, n-i). But since we want minimum, we can just take x = min(rem_ops, n-i) if a[i-1] <= original values? Actually we can just compute for all possible x? But x can be up to n, too slow.
                // Observation: Since we want to minimize sum, we would only change an element if a[i-1] < original value. So we should change as many as possible as long as a[i-1] < original. But the original values might not be sorted. However, we can only change a contiguous suffix starting from i. So we should change a prefix of the suffix. The optimal x is the maximum possible (up to rem_ops) because if we change an element, we replace it with a[i-1]. If a[i-1] is smaller, it's beneficial; if larger, we wouldn't change it. But we can't skip an element and change a later one because operations propagate contiguously. So we must change a contiguous block from i onward. So we should change as many as we have operations for, but only if it reduces sum. Actually, we can just compute the sum for x = min(rem_ops, n-i) and also consider x=0? But if a[i-1] is larger than some later element, changing it might increase sum. However, we are not forced to use all operations. So we should take the minimum over x from 0 to min(rem_ops, n-i). But n can be large, k is small. min(rem_ops, n-i) <= k <= 10. So we can just loop x from 0 to min(rem_ops, n-i) and compute. That's O(k) per state, total O(n*k^2) still fine.
                // Actually we already have O(n*k^2) from transitions, adding O(k) here is okay.
                int max_x = min(rem_ops, n - i);
                // We need suffix sums. Let's precompute suffix sums array.
                // We'll do it outside the loop.
                // For now, assume we have suff array.
                // We'll compute inside.
            }
        }

        // Let's implement properly with suffix sums.
        vector<long long> suff(n + 1, 0);
        for (int i = n - 1; i >= 0; --i) {
            suff[i] = suff[i + 1] + a[i];
        }

        // Recompute answer
        ans = INF;
        for (int i = 1; i <= n; ++i) {
            for (int j = 0; j <= k; ++j) {
                if (dp[i][j] == INF) continue;
                int rem_ops = k - j;
                int max_x = min(rem_ops, n - i);
                // Try all x from 0 to max_x
                for (int x = 0; x <= max_x; ++x) {
                    long long cur = dp[i][j] + (long long)x * a[i-1] + suff[i + x]; // i is 1-indexed, so i+x in 0-indexed is (i-1)+x+1? Wait: suff array is 0-indexed. i is 1-indexed fixed position. The suffix original starts at index i (0-indexed). If we change x elements, the remaining suffix starts at index i + x (0-indexed). So suff[i + x] is correct if suff is defined on 0-indexed with size n+1, suff[idx] = sum from idx to n-1. So i+x is 0-indexed index. Since i is 1-indexed, i+x as 0-indexed is (i-1)+x+1? Let's check: i=1 means first element index 0. Suffix starts at index 1? No, if we fix first element, the suffix is indices 1..n-1. So i=1 -> suffix start index = 1. So i+x with i=1 gives 1+x. So we need to use i as the start index? Actually i is 1-indexed, so the fixed element is at index i-1. The suffix after it starts at index i (0-indexed). So if we change x elements, the remaining suffix starts at index i + x. So suff[i + x] is correct if we treat i as 0-indexed? Let's redefine: let i be 0-indexed position of fixed element. Then dp[i][j] is min sum for prefix up to i with a[i] fixed. Then suffix starts at i+1. If we change x elements, remaining suffix starts at i+1+x. So we need suff[i+1+x]. That's easier. Let's change to 0-indexed for dp.
        }

        // Let's redo with 0-indexed to avoid confusion.
        // dp[i][j] = min sum for prefix 0..i with a[i] fixed, using j operations.
        // i from 0 to n-1.
        // Initialize: dp[i][i] = (i+1)*a[i] (change all before to a[i]), if i <= k.
        // Transitions: from i to p > i, ops = p - i - 1, add = ops * min(a[i], a[p]) + a[p].
        // Answer: min over i, j of dp[i][j] + x*a[i] + suff[i+1+x] where x <= k-j, x <= n-1-i.
        // suff[idx] = sum from idx to n-1.

        int N = n;
        vector<vector<long long>> dp2(N, vector<long long>(k + 1, INF));
        for (int i = 0; i < N; ++i) {
            if (i <= k) {
                dp2[i][i] = (long long)(i + 1) * a[i];
            }
        }

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j <= k; ++j) {
                if (dp2[i][j] == INF) continue;
                for (int p = i + 1; p < N; ++p) {
                    int ops = p - i - 1;
                    if (j + ops > k) break;
                    long long add = (long long)ops * min(a[i], a[p]) + a[p];
                    if (dp2[i][j] + add < dp2[p][j + ops]) {
                        dp2[p][j + ops] = dp2[i][j] + add;
                    }
                }
            }
        }

        suff.assign(N + 1, 0);
        for (int i = N - 1; i >= 0; --i) {
            suff[i] = suff[i + 1] + a[i];
        }

        long long answer = INF;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j <= k; ++j) {
                if (dp2[i][j] == INF) continue;
                int rem = k - j;
                int max_x = min(rem, N - 1 - i);
                for (int x = 0; x <= max_x; ++x) {
                    long long cur = dp2[i][j] + (long long)x * a[i] + suff[i + 1 + x];
                    if (cur < answer) answer = cur;
                }
            }
        }

        // Also consider the case where we don't fix any element? That's covered by fixing some element and changing everything else? Actually if we fix no element, we can't do anything? But we can change all elements to one value? That would require at least n-1 operations. If k >= n-1, we can change all to min element. But our dp with fixed element covers that: if we fix the minimum element and change all others to it, that uses n-1 operations. Our dp initialization allows fixing an element and changing all before it. Then suffix we change all after it. So it's covered.
        // But wait: what if we change all elements to some value that is not originally in the array? The operation only allows replacing with neighbor's value, so final values must be from the original array. So fixing some original element is necessary.

        // Edge case: k=0. Then dp2[i][0] is only valid if i=0? Actually dp2[i][i] with i=0 gives dp2[0][0] = a[0]. Then suffix x=0 gives a[0] + suff[1] = total sum. That's correct. For i>0, dp2[i][0] is INF because i>0 requires i operations. So answer will be total sum. Good.

        cout << answer << '\n';
    }
    return 0;
}
