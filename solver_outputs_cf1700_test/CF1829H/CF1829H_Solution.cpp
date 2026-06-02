#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

void solve() {
    int n, k;
    cin >> n >> k;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }
    
    // dp[j] = number of subsequences with AND value j
    vector<long long> dp(64, 0);
    
    for (int x : a) {
        vector<long long> new_dp = dp; // case: don't use current element
        for (int j = 0; j < 64; ++j) {
            if (dp[j] > 0) {
                int and_val = j & x;
                new_dp[and_val] = (new_dp[and_val] + dp[j]) % MOD;
            }
        }
        new_dp[x] = (new_dp[x] + 1) % MOD; // start new subsequence with just x
        dp = move(new_dp);
    }
    
    long long ans = 0;
    for (int j = 0; j < 64; ++j) {
        if (__builtin_popcount(j) == k) {
            ans = (ans + dp[j]) % MOD;
        }
    }
    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}
