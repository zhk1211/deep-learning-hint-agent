// Hint1
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;
    k--;

    vector<int> x(m), y(m);
    for (int i = 0; i < m; i++) {
        cin >> x[i] >> y[i];
        x[i]--; y[i]--;
    }

    const int INF = 1e9;
    vector<int> dp(n, INF);
    dp[k] = 0;

    for (int i = 0; i < m; i++) {
        int a = x[i], b = y[i];
        int da = dp[a], db = dp[b];
        dp[a] = min(da + 1, db);
        dp[b] = min(db + 1, da);
    }

    for (int i = 0; i < n; i++) {
        if (dp[i] >= INF) cout << "-1 ";
        else cout << dp[i] << " ";
    }
    cout << "\n";

    return 0;
}
