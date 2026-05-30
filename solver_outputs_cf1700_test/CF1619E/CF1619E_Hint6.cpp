// Hint6
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
        vector<int> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }
        
        vector<int> cnt(n + 1, 0);
        for (int x : a) {
            if (x <= n) cnt[x]++;
        }
        
        vector<long long> ans(n + 1, -1);
        long long ops = 0;
        vector<int> extra; // stack of extra indices < i
        
        for (int i = 0; i <= n; ++i) {
            if (i > 0) {
                if (cnt[i - 1] == 0) {
                    if (extra.empty()) {
                        // cannot proceed further
                        break;
                    }
                    int j = extra.back();
                    extra.pop_back();
                    ops += (i - 1 - j);
                    // now we have one occurrence of i-1
                } else {
                    // we already have cnt[i-1] >= 1
                    // extra occurrences can be used later
                    for (int k = 1; k < cnt[i - 1]; ++k) {
                        extra.push_back(i - 1);
                    }
                }
            }
            // now we can achieve MEX = i
            ans[i] = ops + cnt[i];
        }
        
        for (int i = 0; i <= n; ++i) {
            cout << ans[i] << (i == n ? '\n' : ' ');
        }
    }
    return 0;
}
