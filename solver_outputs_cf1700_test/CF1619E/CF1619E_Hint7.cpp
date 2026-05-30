// Hint7
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
        vector<int> cnt(n + 1, 0);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
            if (a[i] <= n) cnt[a[i]]++;
        }
        
        vector<long long> ans(n + 1, -1);
        long long ops = 0;
        vector<int> extra; // stack of extra elements less than current i
        
        for (int i = 0; i <= n; ++i) {
            if (i > 0) {
                if (cnt[i - 1] == 0) {
                    if (extra.empty()) {
                        // cannot proceed further
                        break;
                    }
                    int use = extra.back();
                    extra.pop_back();
                    ops += (i - 1 - use);
                    // now we have one copy of i-1
                    cnt[i - 1] = 1; // conceptually we have it now
                }
            }
            ans[i] = ops + cnt[i]; // need to increment all occurrences of i to > i
            // add extra copies of i to stack
            if (cnt[i] > 1) {
                for (int j = 1; j < cnt[i]; ++j) {
                    extra.push_back(i);
                }
            }
        }
        
        for (int i = 0; i <= n; ++i) {
            cout << ans[i] << (i == n ? '\n' : ' ');
        }
    }
    return 0;
}
