// Hint3
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];

    vector<int> dp(n);
    vector<int> st;
    ll ans = 0;
    ll sum_dp = 0;

    for (int i = 0; i < n; ++i) {
        while (!st.empty() && a[st.back()] + (i - st.back()) <= a[i]) {
            int j = st.back();
            st.pop_back();
            int prev = st.empty() ? -1 : st.back();
            sum_dp -= dp[j];
            if (prev != -1) {
                dp[prev] = dp[prev] + (j - prev) - 1 + dp[j];
                sum_dp += dp[prev];
            }
        }
        if (st.empty()) {
            dp[i] = i;
        } else {
            int j = st.back();
            dp[i] = dp[j] + (i - j) - 1;
        }
        sum_dp += dp[i];
        st.push_back(i);
        ans += sum_dp;
    }
    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) solve();
    return 0;
}
