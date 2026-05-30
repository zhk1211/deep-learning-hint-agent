// Hint8
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
        stack<int> extra;
        
        for (int i = 0; i <= n; ++i) {
            if (i > 0) {
                if (cnt[i - 1] == 0) {
                    if (extra.empty()) {
                        break;
                    }
                    int val = extra.top();
                    extra.pop();
                    ops += (i - 1 - val);
                    cnt[i - 1] = 1;
                }
            }
            ans[i] = ops + cnt[i];
            for (int j = 0; j < cnt[i] - 1; ++j) {
                extra.push(i);
            }
        }
        
        for (int i = 0; i <= n; ++i) {
            cout << ans[i] << (i == n ? '\n' : ' ');
        }
    }
    return 0;
}
