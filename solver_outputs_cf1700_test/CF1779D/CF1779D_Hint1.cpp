// Hint1
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n), b(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    for (int i = 0; i < n; i++) cin >> b[i];
    int m;
    cin >> m;
    vector<int> x(m);
    for (int i = 0; i < m; i++) cin >> x[i];

    // Check if b[i] > a[i] anywhere -> impossible
    for (int i = 0; i < n; i++) {
        if (b[i] > a[i]) {
            cout << "NO\n";
            return;
        }
    }

    // Count available razors
    map<int, int> razor_count;
    for (int val : x) razor_count[val]++;

    // We need to process segments where b[i] != a[i] and b[i] is the target value.
    // Use a stack to handle nested segments.
    stack<int> st;
    for (int i = 0; i < n; i++) {
        // Remove from stack values that are less than current b[i]
        while (!st.empty() && st.top() < b[i]) {
            st.pop();
        }
        // If current b[i] is already in stack with same value, no new razor needed for this segment
        if (a[i] != b[i]) {
            if (st.empty() || st.top() != b[i]) {
                // Need a razor of size b[i]
                if (razor_count[b[i]] == 0) {
                    cout << "NO\n";
                    return;
                }
                razor_count[b[i]]--;
                st.push(b[i]);
            }
        }
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
