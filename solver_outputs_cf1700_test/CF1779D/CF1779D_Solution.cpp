#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n), b(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    for (int i = 0; i < n; ++i) cin >> b[i];
    int m;
    cin >> m;
    vector<int> x(m);
    for (int i = 0; i < m; ++i) cin >> x[i];

    // Check impossible case: a[i] < b[i]
    for (int i = 0; i < n; ++i) {
        if (a[i] < b[i]) {
            cout << "NO\n";
            return;
        }
    }

    // Collect required cuts
    vector<int> needed;
    stack<int> st;
    for (int i = 0; i < n; ++i) {
        while (!st.empty() && st.top() < b[i]) {
            st.pop();
        }
        if (a[i] > b[i]) {
            if (st.empty() || st.top() != b[i]) {
                st.push(b[i]);
                needed.push_back(b[i]);
            }
        }
    }

    // Check if razors are sufficient
    sort(needed.begin(), needed.end());
    sort(x.begin(), x.end());
    int j = 0;
    for (int val : needed) {
        while (j < m && x[j] < val) ++j;
        if (j == m || x[j] != val) {
            cout << "NO\n";
            return;
        }
        ++j; // use this razor
    }
    cout << "YES\n";
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
