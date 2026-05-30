// Hint6
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

        long long total_base = 0;
        for (long long x : a) total_base += x;

        // We will choose up to k traps to jump over.
        // For each trap i we consider jumping over it.
        // The damage saved by jumping over trap i (compared to taking it)
        // is: (a[i] + number_of_jumps_before_i) - 0 = a[i] + jumps_before.
        // But jumps_before depends on which traps we jump.
        // However, if we decide to jump over a set S of traps,
        // the total damage taken = sum_{i not in S} (a[i] + number of jumps before i).
        // This equals sum_{i not in S} a[i] + sum_{i not in S} (number of jumps before i).
        // The second sum counts for each jump j in S, the number of traps after j that are not jumped.
        // Equivalently, total damage = sum_{i=1..n} a[i] - sum_{i in S} a[i] + sum_{j in S} (number of traps after j that are not jumped).
        // Let's sort the chosen jumps by position.
        // If we jump over traps at positions p1 < p2 < ... < pm (m <= k),
        // then for a jump at pj, the number of traps after it that are not jumped is (n - pj) - (m - j).
        // So total damage = total_base - sum_{j=1..m} a[pj] + sum_{j=1..m} (n - pj - (m - j)).
        // = total_base - sum a[pj] + sum (n - pj) - sum_{j=1..m} (m - j)
        // = total_base - sum a[pj] + sum (n - pj) - m*(m-1)/2.
        // We want to minimize this, which is equivalent to maximizing:
        // sum a[pj] - sum (n - pj) = sum (a[pj] - (n - pj)).
        // Let value[i] = a[i] - (n - i)   (using 0-indexed i from 0 to n-1, so n - i - 1? Let's adjust.)
        // If we use 1-indexed positions: position i (1..n). Then n - i is number of traps after i.
        // So value[i] = a[i] - (n - i).
        // We want to pick up to k indices with the largest value[i], but careful: the formula derived assumed we subtract m*(m-1)/2.
        // Actually, total damage = total_base - sum_{chosen} a[i] + sum_{chosen} (n - i) - m*(m-1)/2.
        // = total_base - sum_{chosen} (a[i] - (n - i)) - m*(m-1)/2.
        // So damage = total_base - (sum of values of chosen) - m*(m-1)/2.
        // To minimize damage, we want to maximize (sum of values + m*(m-1)/2).
        // But m is the number of chosen traps. We can choose any m from 0 to k.
        // For a fixed m, the best is to pick the m traps with the largest value[i].
        // Then we compute damage = total_base - sum_of_top_m_values - m*(m-1)/2.
        // We take the minimum over m=0..k.

        vector<long long> values(n);
        for (int i = 0; i < n; ++i) {
            // i is 0-indexed, position = i+1
            values[i] = a[i] - (n - (i + 1));
        }
        sort(values.rbegin(), values.rend());

        long long ans = total_base; // m=0 case
        long long sum_vals = 0;
        for (int m = 1; m <= k; ++m) {
            sum_vals += values[m - 1];
            long long damage = total_base - sum_vals - 1LL * m * (m - 1) / 2;
            if (damage < ans) ans = damage;
        }
        cout << ans << '\n';
    }
    return 0;
}
