// Hint4
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    
    vector<int> ans(n);
    vector<int> stk;
    vector<int> max_suffix(n);
    max_suffix[n-1] = a[n-1];
    for (int i = n-2; i >= 0; --i) {
        max_suffix[i] = max(a[i], max_suffix[i+1]);
    }
    
    int global_max = *max_element(a.begin(), a.end());
    
    for (int i = n-1; i >= 0; --i) {
        while (!stk.empty() && a[stk.back()] <= a[i]) {
            stk.pop_back();
        }
        if (stk.empty()) {
            ans[i] = global_max;
        } else {
            ans[i] = max(a[i], max_suffix[stk.back()]);
        }
        stk.push_back(i);
    }
    
    for (int i = 0; i < n; ++i) {
        cout << ans[i] << " \n"[i == n-1];
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
