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
    
    // dp[i][j] = min coins to paint first i elements (0-indexed up to i-1)
    // with j indicating the state of the "active" red element that can still decrease:
    // j=0: no active red element (or it's 0)
    // j=1: active red element with value 1
    // j=2: active red element with value 2
    // We process elements left to right.
    
    const int INF = 1e9;
    vector<int> dp(3, INF);
    dp[0] = 0; // initially no active red
    
    for (int i = 0; i < n; ++i) {
        vector<int> ndp(3, INF);
        int x = a[i];
        
        for (int cur = 0; cur < 3; ++cur) {
            if (dp[cur] == INF) continue;
            
            // Option 1: pay coin to paint this element red
            // After paying, the new active red is this element (value x)
            // But if x == 0, it cannot be used to paint others, so state becomes 0.
            int new_state = (x > 0 ? x : 0);
            ndp[new_state] = min(ndp[new_state], dp[cur] + 1);
            
            // Option 2: use the current active red element (if any) to paint this blue element
            if (cur > 0) {
                // We decrease the active red by 1, so new active value is cur - 1
                // If cur - 1 == 0, state becomes 0.
                int next_cur = cur - 1;
                ndp[next_cur] = min(ndp[next_cur], dp[cur]);
            }
        }
        dp = move(ndp);
    }
    
    // Answer is min over states after processing all elements
    int ans = min({dp[0], dp[1], dp[2]});
    cout << ans << '\n';
    
    return 0;
}
