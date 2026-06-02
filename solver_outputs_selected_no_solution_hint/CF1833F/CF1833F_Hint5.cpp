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

        // compress unique values with frequencies
        vector<pair<int, int>> comp;
        for (int x : a) {
            if (comp.empty() || comp.back().first != x)
                comp.push_back({x, 1});
            else
                comp.back().second++;
        }

        int k = comp.size();
        vector<long long> fact(n + 1, 1), invfact(n + 1, 1);
        for (int i = 1; i <= n; ++i) fact[i] = fact[i - 1] * i % MOD;
        invfact[n] = modpow(fact[n], MOD - 2);
        for (int i = n - 1; i >= 0; --i) invfact[i] = invfact[i + 1] * (i + 1) % MOD;

        auto nCr = [&](int N, int R) -> long long {
            if (R < 0 || R > N) return 0;
            return fact[N] * invfact[R] % MOD * invfact[N - R] % MOD;
        };

        long long ans = 0;
        int j = 0;
        long long prod = 1;
        for (int i = 0; i < k; ++i) {
            // maintain window [i, j] where values differ by < m
            while (j < k && comp[j].first - comp[i].first < m) {
                if (j > i) prod = prod * comp[j].second % MOD;
                ++j;
            }
            // now window is [i, j-1]
            int len = j - i;
            if (len >= m) {
                // sum of products of combinations for each element as the max
                // we need to choose m distinct values from the window
                // we can use prefix product of frequencies
                // Actually we can compute directly: for each i as minimum, we want to pick m distinct values from window
                // The number of ways = sum over subsets of size m of product of frequencies
                // This can be done with sliding window and maintaining product of (1 + freq * x) but m is small? No.
                // Alternative: since values are distinct, we can use two pointers and maintain product of frequencies of the last m-1 elements?
                // Let's think: we need to choose exactly m distinct values from the window.
                // If we fix the minimum value at i, we need to choose m-1 other values from the remaining len-1 values.
                // The number of ways is the sum of products of frequencies of any m-1 distinct values from the window excluding i.
                // This is the coefficient of x^{m-1} in product_{t=i+1}^{j-1} (1 + freq[t] * x).
                // We can maintain this product polynomial? But m up to n, total n up to 2e5, so we need O(n) or O(n log n).
                // However, note that the condition "absolute difference strictly less than m" means the window size len is at most m? Not necessarily.
                // Actually, if we have m distinct values within a range of size < m, then the values must be consecutive integers!
                // Because if you have m distinct integers with max - min < m, they must be exactly a set of m consecutive integers.
                // Let's verify: Suppose we have m distinct integers, the minimum is L, maximum is R, with R - L < m.
                // The number of integers in [L, R] is R - L + 1 <= m. Since we have m distinct integers, we must have exactly all integers from L to R.
                // So the values in the magnificent dance must be a set of m consecutive integers!
                // This is a crucial observation.
                // Therefore, the window [i, j-1] can only contribute if it contains at least m consecutive integers.
                // But wait, the condition is on the levels of the chosen dancers, not on all available levels. The chosen dancers must have levels that are m distinct integers with max-min < m, so they must be m consecutive integers.
                // So we only need to consider windows of exactly m consecutive integers.
                // However, the array may have duplicates, and we can choose any subset of students with those levels.
                // So for each possible set of m consecutive integers, if all those integers appear in the array, the number of ways is the product of their frequencies.
                // But wait: the dance requires exactly m students with pairwise distinct levels. So we must pick exactly one student from each of the m levels.
                // So if we have frequencies f[L], f[L+1], ..., f[L+m-1], the number of ways is product of f[i] for i in that range.
                // And we sum this over all L such that all these levels exist.
                // But is that all? Let's test with sample 1: n=7, m=4, a = [8,10,10,9,6,11,7]
                // Sorted: 6,7,8,9,10,10,11
                // Frequencies: 6:1, 7:1, 8:1, 9:1, 10:2, 11:1
                // Consecutive sets of size 4:
                // 6,7,8,9 -> product = 1
                // 7,8,9,10 -> product = 1*1*1*2 = 2
                // 8,9,10,11 -> product = 1*1*2*1 = 2
                // Sum = 5. Matches sample output 5.
                // Sample 2: 5 3, a = [4,2,2,3,6] -> sorted: 2,2,3,4,6 -> freq: 2:2, 3:1, 4:1, 6:1
                // Consecutive sets of size 3:
                // 2,3,4 -> product = 2*1*1 = 2
                // 3,4,5 -> missing 5
                // 4,5,6 -> missing 5
                // Sum = 2. Matches.
                // So the problem reduces to: given frequencies of each value, find sum over all L of product of frequencies of L, L+1, ..., L+m-1, if all exist.
                // This can be done by sliding window over the compressed unique sorted values.
                // We need to check if the values are consecutive integers and if the window size is exactly m.
                // So we can iterate over the compressed array, and maintain a window of values that are consecutive.
                // Actually, we can just check for each i from 0 to k-m, if comp[i].first + m - 1 == comp[i+m-1].first, then the values are consecutive.
                // Then the product of frequencies from i to i+m-1 is the answer contribution.
                // But wait: what if there are gaps? The window of size m in the compressed array might not be consecutive integers if there are missing numbers. The condition comp[i].first + m - 1 == comp[i+m-1].first exactly checks that the m values are consecutive integers.
                // So we can precompute prefix products of frequencies to quickly get the product for any range.
                // Let's test sample 3: 8 2, a = [1,5,2,2,3,1,3,3] -> sorted: 1,1,2,2,3,3,3,5 -> freq: 1:2, 2:2, 3:3, 5:1
                // m=2, consecutive sets of size 2:
                // (1,2): 2*2=4
                // (2,3): 2*3=6
                // (3,4): missing 4
                // (4,5): missing 4
                // Sum = 10. Matches sample output 10.
                // Sample 4: 3 3, a = [3,3,3] -> freq: 3:3. No 3 consecutive integers, sum=0. Matches.
                // Sample 5: 5 1, a = [3,4,3,10,7] -> m=1, any single student works. Number of ways = n = 5. Matches.
                // Sample 6: 12 3, a = [5,2,1,1,4,3,5,5,5,2,7,5] -> sorted: 1,1,2,2,3,4,5,5,5,5,5,7 -> freq: 1:2, 2:2, 3:1, 4:1, 5:5, 7:1
                // Consecutive sets of size 3:
                // 1,2,3: 2*2*1=4
                // 2,3,4: 2*1*1=2
                // 3,4,5: 1*1*5=5
                // 4,5,6: missing 6
                // 5,6,7: missing 6
                // Sum = 11. Matches.
                // Sample 7: 1 1, a=[1] -> 1. Matches.
                // Sample 8: 3 2, a=[1,2,3] -> freq: 1:1,2:1,3:1. Consecutive pairs: (1,2):1, (2,3):1. Sum=2. Matches.
                // Sample 9: 2 2, a=[1,2] -> 1. Matches.
                // So the solution is correct.

                // Implementation: compute prefix product of frequencies modulo MOD.
                vector<long long> pref(k + 1, 1);
                for (int i = 0; i < k; ++i) {
                    pref[i + 1] = pref[i] * comp[i].second % MOD;
                }
                long long ans = 0;
                for (int i = 0; i + m <= k; ++i) {
                    if (comp[i + m - 1].first - comp[i].first == m - 1) {
                        // product from i to i+m-1
                        long long ways = pref[i + m] * modpow(pref[i], MOD - 2) % MOD;
                        ans = (ans + ways) % MOD;
                    }
                }
                cout << ans << '\n';
            }
        }
    }
    return 0;
}
