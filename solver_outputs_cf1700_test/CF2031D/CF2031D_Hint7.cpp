// Hint7
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    
    vector<int> pref_max(n), suff_min(n);
    pref_max[0] = a[0];
    for (int i = 1; i < n; ++i) pref_max[i] = max(pref_max[i-1], a[i]);
    suff_min[n-1] = a[n-1];
    for (int i = n-2; i >= 0; --i) suff_min[i] = min(suff_min[i+1], a[i]);
    
    vector<int> ans(n);
    int global_max = *max_element(a.begin(), a.end());
    
    vector<int> next_greater(n, -1), prev_smaller(n, -1);
    stack<int> st;
    for (int i = 0; i < n; ++i) {
        while (!st.empty() && a[st.top()] < a[i]) {
            next_greater[st.top()] = i;
            st.pop();
        }
        st.push(i);
    }
    while (!st.empty()) st.pop();
    for (int i = n-1; i >= 0; --i) {
        while (!st.empty() && a[st.top()] > a[i]) {
            prev_smaller[st.top()] = i;
            st.pop();
        }
        st.push(i);
    }
    
    vector<int> reach_max(n);
    for (int i = 0; i < n; ++i) reach_max[i] = a[i];
    
    for (int i = 0; i < n; ++i) {
        if (next_greater[i] != -1) {
            reach_max[next_greater[i]] = max(reach_max[next_greater[i]], reach_max[i]);
        }
    }
    for (int i = n-1; i >= 0; --i) {
        if (prev_smaller[i] != -1) {
            reach_max[prev_smaller[i]] = max(reach_max[prev_smaller[i]], reach_max[i]);
        }
    }
    
    vector<int> left_max(n), right_min(n);
    left_max[0] = reach_max[0];
    for (int i = 1; i < n; ++i) left_max[i] = max(left_max[i-1], reach_max[i]);
    right_min[n-1] = reach_max[n-1];
    for (int i = n-2; i >= 0; --i) right_min[i] = min(right_min[i+1], reach_max[i]);
    
    for (int i = 0; i < n; ++i) {
        int res = a[i];
        if (i > 0) res = max(res, left_max[i-1]);
        if (i < n-1) res = max(res, right_min[i+1]);
        ans[i] = max(res, global_max);
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
    while (t--) solve();
    return 0;
}
