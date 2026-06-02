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
            long long start = k[i] - h[i] + 1;
            long long end = k[i];
            intervals.emplace_back(start, end);
        }
        
        sort(intervals.begin(), intervals.end());
        
        long long ans = 0;
        long long cur_start = intervals[0].first;
        long long cur_end = intervals[0].second;
        
        for (int i = 1; i < n; ++i) {
            long long s = intervals[i].first;
            long long e = intervals[i].second;
            if (s <= cur_end) {
                cur_end = max(cur_end, e);
            } else {
                long long len = cur_end - cur_start + 1;
                ans += len * (len + 1) / 2;
                cur_start = s;
                cur_end = e;
            }
        }
        long long len = cur_end - cur_start + 1;
        ans += len * (len + 1) / 2;
        
        cout << ans << '\n';
    }
    return 0;
}
