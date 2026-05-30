// Hint2
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<long long> k(n), h(n);
        for (int i = 0; i < n; ++i) cin >> k[i];
        for (int i = 0; i < n; ++i) cin >> h[i];
        
        vector<pair<long long, long long>> intervals;
        for (int i = 0; i < n; ++i) {
            long long L = k[i] - h[i];
            long long R = k[i];
            intervals.emplace_back(L, R);
        }
        
        sort(intervals.begin(), intervals.end());
        
        long long ans = 0;
        long long curL = intervals[0].first, curR = intervals[0].second;
        for (int i = 1; i < n; ++i) {
            long long L = intervals[i].first, R = intervals[i].second;
            if (L < curR) {
                curR = max(curR, R);
            } else {
                long long len = curR - curL;
                ans += len * (len + 1) / 2;
                curL = L;
                curR = R;
            }
        }
        long long len = curR - curL;
        ans += len * (len + 1) / 2;
        
        cout << ans << '\n';
    }
    return 0;
}
