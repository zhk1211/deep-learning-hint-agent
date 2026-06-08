#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;
const int MAXN = 500000;

vector<long long> fact, inv_fact;

long long mod_pow(long long a, long long e) {
    long long res = 1;
    while (e) {
        if (e & 1) res = res * a % MOD;
        a = a * a % MOD;
        e >>= 1;
    }
    return res;
}

void precompute_factorials(int n) {
    fact.resize(n + 1);
    inv_fact.resize(n + 1);
    fact[0] = 1;
    for (int i = 1; i <= n; i++) {
        fact[i] = fact[i - 1] * i % MOD;
    }
    inv_fact[n] = mod_pow(fact[n], MOD - 2);
    for (int i = n - 1; i >= 0; i--) {
        inv_fact[i] = inv_fact[i + 1] * (i + 1) % MOD;
    }
}

long long nCr(int n, int r) {
    if (r < 0 || r > n) return 0;
    return fact[n] * inv_fact[r] % MOD * inv_fact[n - r] % MOD;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    vector<vector<int>> tests(t, vector<int>(26));
    int total_sum = 0;
    for (int i = 0; i < t; i++) {
        for (int j = 0; j < 26; j++) {
            cin >> tests[i][j];
            total_sum += tests[i][j];
        }
    }

    precompute_factorials(total_sum);

    for (auto &c : tests) {
        int sum = 0;
        int odd_count = 0;
        int even_count = 0;
        int odd_sum = 0;
        int even_sum = 0;

        for (int i = 0; i < 26; i++) {
            sum += c[i];
            if (c[i] % 2 == 1) {
                odd_count++;
                odd_sum += c[i];
            } else {
                even_count++;
                even_sum += c[i];
            }
        }

        if (odd_count > 1) {
            cout << 0 << '\n';
            continue;
        }

        int n = sum;
        int k = (n + 1) / 2; // number of odd positions (1-indexed)
        int m = n / 2;       // number of even positions

        long long ans = 1;

        if (odd_count == 1) {
            // The odd-count letter must occupy one odd position and the rest even positions.
            // Actually, it must occupy all odd positions? Wait, condition: |i-j| mod 2 = 0 if s_i = s_j.
            // This means all occurrences of a letter must be in positions of the same parity.
            // So if a letter has odd count, it must occupy all odd positions? No, it can occupy some odd and some even?
            // Let's analyze: If a letter appears in both odd and even positions, then there exist i (odd) and j (even) with |i-j| odd -> violation.
            // So each letter must be entirely in odd positions or entirely in even positions.
            // Since total odd positions = ceil(n/2), total even = floor(n/2).
            // If a letter has odd count, it cannot be split between odd and even because sum of counts in odd + even = odd, so one part odd, one even -> impossible to have both parts non-zero? Actually if count is odd, it could be all in odd or all in even. But if all in even, count must be <= even positions. But odd count means it's odd number. If all in even, count is even? No, count is odd, but even positions count is floor(n/2). If floor(n/2) is even? No, count is odd, but the number of even positions is an integer. The sum of counts in even positions must equal total even positions. If a letter with odd count is placed entirely in even positions, its count is odd, but the total even positions sum is m. That's fine. However, if there is a letter with odd count, can it be placed in even positions? Let's check: If we put it in even positions, then the remaining letters must fill odd positions and the rest of even positions. But the remaining letters all have even counts. The sum of even counts is even. The total odd positions k must be filled by letters with even counts. Sum of even counts is even, but k could be odd or even. If k is odd, sum of even counts cannot be odd because sum of even numbers is even. So k must be even. Similarly, if the odd-count letter is placed in odd positions, then the remaining even-count letters fill the rest of odd positions (k - odd_count) and all even positions (m). Since odd_count is odd, k - odd_count has the same parity as k - odd. The sum of even counts is even, so k - odd_count must be even. So k and odd_count must have the same parity. Since odd_count is odd, k must be odd. So if k is even, the odd-count letter cannot be in odd positions; it must be in even positions. But then m must be odd? Let's derive properly.

            // Actually, the condition is: each letter occupies positions of a single parity.
            // Let O be the set of letters assigned to odd positions, E to even positions.
            // Sum_{x in O} c_x = k, Sum_{x in E} c_x = m.
            // Since all c_x for x != the odd-count letter are even, the parity of the sum for O is determined by whether the odd-count letter is in O.
            // If odd-count letter in O: sum = odd + even = odd. So k must be odd.
            // If odd-count letter in E: sum = even (since all in E have even counts? Wait, the odd-count letter has odd count, so if it's in E, sum for E = odd + even = odd. So m must be odd.
            // So exactly one of k or m is odd. Since k + m = n, k and m have opposite parity. So one is odd, one is even.
            // Thus, the odd-count letter must be assigned to the parity that has odd size.
            // So the assignment of letters to parities is forced: the odd-count letter goes to the odd-sized parity, and the even-count letters are distributed to fill the remaining spots.

            // So we just need to choose which even-count letters go to odd positions, and which to even positions, such that the sum of their counts matches the required remaining spots.
            // Let target_odd = k, target_even = m.
            // If odd_count == 1:
            //   Let odd_letter be the one with odd count.
            //   If k % 2 == 1: odd_letter goes to odd positions. Then remaining odd spots = k - c[odd_letter]. Remaining even spots = m.
            //   Else: odd_letter goes to even positions. Remaining odd spots = k. Remaining even spots = m - c[odd_letter].
            //   The remaining letters all have even counts. We need to partition them into two sets with sums exactly equal to the remaining spots.
            //   This is a subset sum problem with even numbers. Since total remaining sum is even, and we need to achieve specific sums, it's equivalent to: we have items with values c_i/2, and we need to choose a subset summing to (remaining_odd)/2.
            //   Because all c_i are even, we can divide everything by 2.
            //   So we need to count the number of ways to choose a subset of the even-count letters (each can be used fully in odd or fully in even) such that sum of (c_i/2) for those assigned to odd equals R = remaining_odd / 2.
            //   Then the number of ways is the number of subsets with sum R. Since each letter is distinct, the number of ways is the number of subsets of indices (from the even-count letters) that sum to R.
            //   Then, for each such assignment, the number of distinct strings is: (ways to arrange letters in odd positions) * (ways to arrange letters in even positions).
            //   Arrangement in odd positions: multinomial coefficient: k! / (product over letters in O of c_x!).
            //   Similarly for even positions: m! / (product over letters in E of c_x!).
            //   The product of these two gives: k! * m! / (product over all letters of c_x!).
            //   Notice that this product does NOT depend on which letters are assigned to which parity! It only depends on the total counts. Because the denominator is the product of all factorials of counts, which is fixed. The numerator is k! * m!, which is also fixed.
            //   So the number of distinct strings for a given valid assignment of letters to parities is exactly: k! * m! / (prod c_i!).
            //   Therefore, the total answer is: (number of valid assignments) * (k! * m! / prod c_i!) mod MOD.
            //   So we just need to count the number of valid assignments (i.e., number of subsets of even-count letters that sum to the required remaining odd spots).
            //   This is a subset sum count problem. Since total sum of c_i over all test cases <= 5e5, we can do DP per test case? But sum over all test cases is 5e5, so total sum of all c_i across all test cases is 5e5. We can do a DP for each test case with O(sum * number of even-count letters) which might be too slow if we do naive DP per test case. However, sum of c_i per test case is at most 5e5, and total sum across test cases is 5e5. We can use DP with bitset or optimized knapsack? But we need to count number of subsets modulo MOD. Bitset gives boolean, not count. We need counts.
            //   Since total sum is 5e5, we can do a DP array of size up to 5e5 per test case, but the sum of sizes across test cases is 5e5. So we can allocate a DP array for each test case and do a knapsack. The number of items (even-count letters) is at most 26. So we can do a simple DP: dp[s] = number of ways to get sum s. Complexity per test case: O(number of even-count letters * remaining_sum). Since remaining_sum <= total_sum, and total sum over test cases <= 5e5, the overall complexity is O(26 * 5e5) = 1.3e7, which is fine.
            //   But wait: we need to divide by 2. So we work with half-values.
            //   Let's formalize.

            int odd_letter = -1;
            for (int i = 0; i < 26; i++) if (c[i] % 2 == 1) odd_letter = i;

            int target_odd = k;
            int target_even = m;
            int rem_odd, rem_even;
            if (k % 2 == 1) {
                // odd_letter goes to odd
                rem_odd = k - c[odd_letter];
                rem_even = m;
            } else {
                // odd_letter goes to even
                rem_odd = k;
                rem_even = m - c[odd_letter];
            }

            if (rem_odd < 0 || rem_even < 0) {
                cout << 0 << '\n';
                continue;
            }

            // Now we have even-count letters. We need to choose a subset to assign to odd positions such that sum of their counts = rem_odd.
            // Since all these counts are even, rem_odd must be even. If not, answer 0.
            if (rem_odd % 2 != 0) {
                cout << 0 << '\n';
                continue;
            }

            int half_rem_odd = rem_odd / 2;
            vector<int> items;
            for (int i = 0; i < 26; i++) {
                if (i == odd_letter) continue;
                if (c[i] > 0) {
                    items.push_back(c[i] / 2);
                }
            }

            // DP to count subsets summing to half_rem_odd
            vector<long long> dp(half_rem_odd + 1, 0);
            dp[0] = 1;
            for (int val : items) {
                for (int s = half_rem_odd; s >= val; s--) {
                    dp[s] = (dp[s] + dp[s - val]) % MOD;
                }
            }

            long long ways = dp[half_rem_odd];
            if (ways == 0) {
                cout << 0 << '\n';
                continue;
            }

            // Compute k! * m! / prod c_i!
            long long num = fact[k] * fact[m] % MOD;
            long long den = 1;
            for (int i = 0; i < 26; i++) {
                den = den * fact[c[i]] % MOD;
            }
            long long inv_den = mod_pow(den, MOD - 2);
            ans = num * inv_den % MOD;
            ans = ans * ways % MOD;
            cout << ans << '\n';

        } else { // odd_count == 0
            // All counts are even.
            // We need to assign each letter entirely to odd or entirely to even positions.
            // Sum of counts in odd = k, in even = m.
            // Since all counts are even, k and m must both be even. If not, answer 0.
            if (k % 2 != 0 || m % 2 != 0) {
                cout << 0 << '\n';
                continue;
            }
            // Again, we need to choose a subset of letters to go to odd positions such that sum = k.
            // Since all counts even, we can divide by 2.
            int half_k = k / 2;
            vector<int> items;
            for (int i = 0; i < 26; i++) {
                if (c[i] > 0) {
                    items.push_back(c[i] / 2);
                }
            }
            vector<long long> dp(half_k + 1, 0);
            dp[0] = 1;
            for (int val : items) {
                for (int s = half_k; s >= val; s--) {
                    dp[s] = (dp[s] + dp[s - val]) % MOD;
                }
            }
            long long ways = dp[half_k];
            if (ways == 0) {
                cout << 0 << '\n';
                continue;
            }
            long long num = fact[k] * fact[m] % MOD;
            long long den = 1;
            for (int i = 0; i < 26; i++) {
                den = den * fact[c[i]] % MOD;
            }
            long long inv_den = mod_pow(den, MOD - 2);
            ans = num * inv_den % MOD;
            ans = ans * ways % MOD;
            cout << ans << '\n';
        }
    }

    return 0;
}
