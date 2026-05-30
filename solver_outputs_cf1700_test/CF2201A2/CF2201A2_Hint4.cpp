// Hint4
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];

    vector<int> nxt(n, n);
    vector<int> st;
    for (int i = 0; i < n; ++i) {
        while (!st.empty() && a[st.back()] + 1 != a[i]) {
            nxt[st.back()] = i;
            st.pop_back();
        }
        st.push_back(i);
    }

    vector<ll> dp(n + 1, 0);
    ll ans = 0;
    for (int i = n - 1; i >= 0; --i) {
        int j = nxt[i];
        if (j == n) dp[i] = n - i;
        else dp[i] = (j - i) + dp[j];
        ans += dp[i];
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
