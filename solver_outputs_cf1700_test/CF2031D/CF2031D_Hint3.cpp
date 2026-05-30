// Hint3
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    
    vector<int> L(n), R(n);
    vector<int> st;
    for (int i = 0; i < n; ++i) {
        while (!st.empty() && a[st.back()] <= a[i]) st.pop_back();
        L[i] = st.empty() ? -1 : st.back();
        st.push_back(i);
    }
    st.clear();
    for (int i = n - 1; i >= 0; --i) {
        while (!st.empty() && a[st.back()] <= a[i]) st.pop_back();
        R[i] = st.empty() ? n : st.back();
        st.push_back(i);
    }
    
    vector<int> ans(n);
    vector<int> pref_max(n), suff_max(n);
    pref_max[0] = a[0];
    for (int i = 1; i < n; ++i) pref_max[i] = max(pref_max[i-1], a[i]);
    suff_max[n-1] = a[n-1];
    for (int i = n-2; i >= 0; --i) suff_max[i] = max(suff_max[i+1], a[i]);
    
    for (int i = 0; i < n; ++i) {
        int l = L[i], r = R[i];
        int cur = a[i];
        if (l != -1) cur = max(cur, pref_max[l]);
        if (r != n) cur = max(cur, suff_max[r]);
        ans[i] = cur;
    }
    
    for (int i = 0; i < n; ++i) cout << ans[i] << " \n"[i == n-1];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) solve();
    return 0;
}
