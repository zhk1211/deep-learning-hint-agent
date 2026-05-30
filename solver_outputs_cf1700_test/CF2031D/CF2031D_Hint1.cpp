// Hint1
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    vector<int> left(n), right(n);
    stack<int> st;

    // nearest greater to left
    for (int i = 0; i < n; ++i) {
        while (!st.empty() && a[st.top()] <= a[i]) st.pop();
        left[i] = st.empty() ? -1 : st.top();
        st.push(i);
    }
    while (!st.empty()) st.pop();

    // nearest greater to right
    for (int i = n - 1; i >= 0; --i) {
        while (!st.empty() && a[st.top()] <= a[i]) st.pop();
        right[i] = st.empty() ? n : st.top();
        st.push(i);
    }
    while (!st.empty()) st.pop();

    vector<int> ans(n);
    vector<int> dp(n, -1);

    function<int(int)> dfs = [&](int u) -> int {
        if (dp[u] != -1) return dp[u];
        int mx = a[u];
        if (left[u] != -1) mx = max(mx, dfs(left[u]));
        if (right[u] != n) mx = max(mx, dfs(right[u]));
        return dp[u] = mx;
    };

    for (int i = 0; i < n; ++i) {
        ans[i] = dfs(i);
    }

    for (int i = 0; i < n; ++i) {
        cout << ans[i] << " \n"[i == n - 1];
    }
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
