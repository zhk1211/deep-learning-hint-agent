#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

long long modpow(long long a, long long e) {
    long long res = 1;
    while (e) {
        if (e & 1) res = res * a % MOD;
        a = a * a % MOD;
        e >>= 1;
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n, m;
        cin >> n >> m;
        vector<int> a(n);
        for (int i = 0; i < n; ++i) cin >> a[i];
        sort(a.begin(), a.end());

        vector<pair<int, int>> freq;
        for (int x : a) {
            if (freq.empty() || freq.back().first != x)
                freq.push_back({x, 1});
            else
                freq.back().second++;
        }

        int k = freq.size();
        vector<long long> pref(k + 1, 1);
        for (int i = 0; i < k; ++i) {
            pref[i + 1] = pref[i] * freq[i].second % MOD;
        }

        auto range_product = [&](int l, int r) -> long long {
            if (l > r) return 1;
            return pref[r + 1] * modpow(pref[l], MOD - 2) % MOD;
        };

        long long ans = 0;
        int j = 0;
        for (int i = 0; i < k; ++i) {
            while (j < k && freq[j].first - freq[i].first < m) {
                ++j;
            }
            // window [i, j-1]
            int len = j - i;
            if (len >= m) {
                // we need to choose m distinct values from this window
                // but we must pick exactly one student from each chosen value
                // So we sum over all combinations of m distinct values in the window
                // product of their frequencies
                // This can be done with a sliding window and prefix products
                // Actually we can just compute for each i the contribution of the window ending at j-1
                // But we need to avoid double counting. Better: for each i, consider the window starting at i
                // and we need to pick m values from it. The number of ways is sum over all subsets of size m
                // of product of frequencies. This is exactly the coefficient of x^m in product (1 + f_i x)
                // which can be computed with DP, but n is up to 2e5, so we need O(n) per test.
                // Alternative: Since we only need to pick m distinct values, and the window is contiguous in sorted order,
                // we can fix the smallest value in the chosen set, say at index i, then we need to choose m-1 values from
                // the next len-1 values. But the product of frequencies is not simply combin(len-1, m-1) because frequencies differ.
                // However, we can use the fact that we are iterating i, and we can maintain a sliding window of size m.
                // Actually, the condition is that the max difference in the chosen set is < m. So if we sort and pick m distinct values,
                // the condition is that the difference between the maximum and minimum chosen values is < m.
                // So if we fix the minimum value at index i, the maximum value can be at most freq[i].first + m - 1.
                // So the allowed values are those in the window [i, j-1] where j is the first index with freq[j].first - freq[i].first >= m.
                // We need to choose m distinct values from this window, including the one at i.
                // The number of ways is: freq[i].second * (sum over subsets of size m-1 from indices i+1 to j-1 of product of frequencies).
                // This is exactly the coefficient of x^{m-1} in product_{t=i+1}^{j-1} (1 + freq[t].second * x).
                // We can compute this using prefix products of polynomials? But m is up to n, so we need a faster way.
                // Notice that the window size len = j - i. If len < m, contribution is 0.
                // If len >= m, we can use a sliding window and maintain the product of (1 + f_t x) using a segment tree? Too heavy.
                // But observe that we only need the answer for each i, and we can move i and j. However, the product changes.
                // Another approach: Since we only need to pick exactly m distinct values, and the values are sorted, we can use two pointers
                // to maintain a window of size exactly m? No, the window size can be larger than m.
                // Let's think differently: The condition "absolute difference strictly less than m" for every pair is equivalent to
                // max - min < m. So if we sort the distinct values, any subset of size m with max - min < m is valid.
                // So we can iterate over all possible windows of distinct values where the difference between first and last is < m,
                // and for each window, we need to sum the product of frequencies of any m distinct values in it.
                // This is a classic problem: given an array of frequencies, for each contiguous subarray of length L, compute
                // the sum of products of all m-element subsets. This can be done with a sliding window and maintaining the elementary symmetric sums.
                // But m is not fixed across testcases? m is given per testcase, up to n.
                // However, we can use the fact that we only need the sum for windows where the difference condition holds.
                // We can use two pointers to find for each left index i, the rightmost index j such that freq[j].first - freq[i].first < m.
                // Then we need to compute for the subarray freq[i..j-1] the sum of products of all m-element subsets.
                // Let this subarray have length L = j - i. We need S(m, i, j-1) = sum_{subset of size m} prod_{t in subset} freq[t].second.
                // We can compute this using DP if we process left to right and maintain a polynomial, but L can be up to n.
                // Notice that the total sum of n over testcases is 2e5, so O(n * m) is too slow if m is large.
                // We need an O(n) or O(n log n) per testcase.
                // Alternative insight: Since we only need to pick m distinct values, and the condition is max - min < m,
                // the values themselves are integers. If we have a window of distinct values where max - min < m,
                // then the number of distinct values in that window is at most m (since they are integers and distinct).
                // Wait! Is that true? If we have distinct integers, and max - min < m, then the number of distinct integers
                // in that range is at most m. Because if min = x, max = y, and y - x < m, then the integers in [x, y] are
                // x, x+1, ..., y, which is y - x + 1 <= m. So there are at most m distinct integers in any window satisfying the condition.
                // But our array freq contains only the values that actually appear in the input. So the window [i, j-1] consists of
                // some of the integers in [freq[i].first, freq[i].first + m - 1]. The number of such integers present in the input
                // could be less than m, but it cannot exceed m because the range length is m and they are distinct integers.
                // Therefore, for any valid window, the number of distinct values L = j - i is <= m.
                // And we need to choose exactly m students from these L distinct values, with pairwise distinct levels.
                // But we only have L distinct values available. If L < m, we cannot choose m distinct values, so contribution is 0.
                // If L == m, then we must choose exactly one student from each of the L distinct values. The number of ways is
                // the product of their frequencies.
                // So the problem reduces to: For each i, find the window of distinct values starting at i such that
                // freq[j-1].first - freq[i].first < m and freq[j].first - freq[i].first >= m (or j == k).
                // If the number of distinct values in this window is exactly m, then add the product of their frequencies to the answer.
                // Is it possible that L > m? As argued, no, because the values are distinct integers and the range length is < m,
                // so the number of integers in that range is at most m. Since our freq array only contains values that appear,
                // L <= m. So L can only be <= m. If L < m, we can't form a dance. If L == m, we must take all of them.
                // Therefore, we just need to find all windows of exactly m distinct values where max - min < m,
                // and multiply their frequencies.
                // But wait: Could there be a case where we have a window with L = m, but we could also choose a subset of size m
                // from a larger window? But we just proved L cannot be > m. So the only way to get m distinct values is if the window
                // has exactly m distinct values. So we just need to consider windows of exactly m consecutive distinct values
                // (in sorted order) and check if the difference between the last and first is < m. If yes, add the product of frequencies.
                // Let's test this logic with the sample.
                // Sample 1: n=7, m=4, a = [8,10,10,9,6,11,7]
                // Sorted a: 6,7,8,9,10,10,11
                // Distinct values with frequencies:
                // 6:1, 7:1, 8:1, 9:1, 10:2, 11:1
                // m=4. We need windows of 4 consecutive distinct values.
                // Windows:
                // [6,7,8,9]: max-min = 3 < 4 -> product = 1*1*1*1 = 1
                // [7,8,9,10]: max-min = 3 < 4 -> product = 1*1*1*2 = 2
                // [8,9,10,11]: max-min = 3 < 4 -> product = 1*1*2*1 = 2
                // Total = 5. Matches sample output 5.
                // Sample 2: n=5, m=3, a = [4,2,2,3,6]
                // Sorted: 2,2,3,4,6
                // Distinct: 2:2, 3:1, 4:1, 6:1
                // m=3. Windows of 3:
                // [2,3,4]: max-min = 2 < 3 -> product = 2*1*1 = 2
                // [3,4,6]: max-min = 3 not < 3 -> skip
                // Total = 2. Matches sample.
                // Sample 3: n=8, m=2, a = [1,5,2,2,3,1,3,3]
                // Sorted: 1,1,2,2,3,3,3,5
                // Distinct: 1:2, 2:2, 3:3, 5:1
                // m=2. Windows of 2:
                // [1,2]: diff=1<2 -> 2*2=4
                // [2,3]: diff=1<2 -> 2*3=6
                // [3,5]: diff=2 not <2 -> skip
                // Total = 10. Matches sample.
                // Sample 4: n=3, m=3, a = [3,3,3]
                // Distinct: 3:3. Only one distinct value, cannot form window of 3. Output 0. Matches.
                // Sample 5: n=5, m=1, a = [3,4,3,10,7]
                // Distinct: 3:2, 4:1, 7:1, 10:1
                // m=1. Windows of 1: any single distinct value. Condition max-min < 1 is always true (0<1).
                // So sum of frequencies = 2+1+1+1 = 5. Matches sample.
                // Sample 6: n=12, m=3, a = [5,2,1,1,4,3,5,5,5,2,7,5]
                // Sorted: 1,1,2,2,3,4,5,5,5,5,5,7
                // Distinct: 1:2, 2:2, 3:1, 4:1, 5:5, 7:1
                // m=3. Windows of 3:
                // [1,2,3]: diff=2<3 -> 2*2*1=4
                // [2,3,4]: diff=2<3 -> 2*1*1=2
                // [3,4,5]: diff=2<3 -> 1*1*5=5
                // [4,5,7]: diff=3 not <3 -> skip
                // Total = 11. Matches sample.
                // Sample 7: n=1, m=1, a=[1] -> distinct: 1:1 -> window of 1: product=1. Matches.
                // Sample 8: n=3, m=2, a=[1,2,3] -> distinct: 1:1,2:1,3:1
                // Windows of 2: [1,2]: diff=1<2 -> 1; [2,3]: diff=1<2 -> 1; total=2. Matches.
                // Sample 9: n=2, m=2, a=[1,2] -> distinct: 1:1,2:1 -> window [1,2]: diff=1<2 -> 1. Matches.
                // So the logic holds perfectly!
                // Therefore, the algorithm is:
                // 1. Sort the array a.
                // 2. Compress to distinct values and their frequencies.
                // 3. If m == 1, answer is sum of all frequencies (since any single student forms a dance of size 1, and condition max-min=0 < 1 holds).
                //    Actually, the window of size 1 always satisfies max-min=0 < 1, so we just sum frequencies.
                // 4. For m > 1, iterate over all windows of exactly m consecutive distinct values.
                //    Check if the difference between the last and first value in the window is < m.
                //    If yes, add the product of their frequencies to the answer.
                //    Use a sliding window of size m over the distinct values array.
                //    We can maintain the product modulo MOD. Since we only move the window by one step, we can multiply by the new element and divide by the old element using modular inverse.
                //    Since MOD is prime and frequencies are up to n <= 2e5, they are not multiples of MOD, so inverse exists.
                // Time complexity: O(n log n) for sorting, O(n) for the rest. Sum of n <= 2e5, so fine.
                
                // Wait: The window size is exactly m distinct values. But what if there are multiple windows that overlap?
                // We just slide a window of length m over the distinct values array. Each window gives a set of m distinct values.
                // Since the values are sorted, any set of m distinct values that satisfies max-min < m must be consecutive in the sorted distinct array.
                // Proof: Suppose we have a set S of m distinct integers with max-min < m. Since they are integers, the number of integers in [min, max] is max-min+1 <= m.
                // Since |S| = m, S must be exactly all integers in [min, max]. Therefore, in the sorted distinct array, they appear consecutively.
                // So sliding a window of size m over the distinct array covers all possible valid sets.
                // So the algorithm is correct.
            }
        }

        // Implementation based on the above insight:
        if (m == 1) {
            long long sum = 0;
            for (auto &p : freq) sum = (sum + p.second) % MOD;
            cout << sum << '\n';
            continue;
        }

        long long ans = 0;
        int k = freq.size();
        if (k >= m) {
            long long prod = 1;
            for (int i = 0; i < m; ++i) {
                prod = prod * freq[i].second % MOD;
            }
            if (freq[m-1].first - freq[0].first < m) {
                ans = (ans + prod) % MOD;
            }
            for (int i = m; i < k; ++i) {
                // remove freq[i-m].second, add freq[i].second
                prod = prod * modpow(freq[i-m].second, MOD - 2) % MOD;
                prod = prod * freq[i].second % MOD;
                if (freq[i].first - freq[i-m+1].first < m) {
                    ans = (ans + prod) % MOD;
                }
            }
        }
        cout << ans << '\n';
    }
    return 0;
}
