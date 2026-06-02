#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;
const int MAXN = 1000000;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> dp(n + 1);
    vector<int> divisors(n + 1, 0);

    // Precompute number of divisors for all numbers up to n
    for (int i = 1; i <= n; ++i) {
        for (int j = i; j <= n; j += i) {
            divisors[j]++;
        }
    }

    dp[0] = 1;
    dp[1] = 1;
    long long sum_dp = dp[0] + dp[1]; // sum of dp[0]..dp[i-1] for current i

    for (int i = 2; i <= n; ++i) {
        dp[i] = (divisors[i] + sum_dp) % MOD;
        sum_dp = (sum_dp + dp[i]) % MOD;
    }

    cout << dp[n] << "\n";
    return 0;
}
