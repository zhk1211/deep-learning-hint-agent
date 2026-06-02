#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }
    
    vector<int> dp(3, 0);
    // dp[0] - min coins if current element is painted by coin (or from left)
    // dp[1] - min coins if current element is painted from left with value 1
    // dp[2] - min coins if current element is painted from left with value 2
    
    for (int i = 0; i < n; ++i) {
        vector<int> ndp(3, INT_MAX);
        if (a[i] == 0) {
            // must use coin
            ndp[0] = min({dp[0], dp[1], dp[2]}) + 1;
        } else if (a[i] == 1) {
            // option 1: use coin
            ndp[0] = min({dp[0], dp[1], dp[2]}) + 1;
            // option 2: get from left with value 1
            ndp[1] = min(dp[0], dp[1]);
        } else { // a[i] == 2
            // option 1: use coin
            ndp[0] = min({dp[0], dp[1], dp[2]}) + 1;
            // option 2: get from left with value 1
            ndp[1] = min(dp[0], dp[1]);
            // option 3: get from left with value 2
            ndp[2] = min(dp[0], dp[2]);
        }
        dp = ndp;
    }
    
    cout << min({dp[0], dp[1], dp[2]}) << '\n';
    return 0;
}
