// Hint7
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];

    vector<int> stk;
    stk.reserve(n);
    vector<ll> dp(n + 1, 0);
    vector<ll> sum_dp(n + 1, 0);
    ll ans = 0;

    for (int i = 0; i < n; ++i) {
        while (!stk.empty() && a[stk.back()] <= a[i]) {
            stk.pop_back();
        }
        int j = stk.empty() ? -1 : stk.back();
        stk.push_back(i);

        if (j == -1) {
            dp[i] = 1;
        } else {
            dp[i] = dp[j] + 1;
        }
        sum_dp[i + 1] = sum_dp[i] + dp[i];
        ans += sum_dp[i + 1];
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
