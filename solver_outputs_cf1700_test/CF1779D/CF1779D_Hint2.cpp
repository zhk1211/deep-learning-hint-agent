// Hint2
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

    // Check if any b[i] > a[i] -> impossible
    for (int i = 0; i < n; ++i) {
        if (b[i] > a[i]) {
            cout << "NO\n";
            return;
        }
    }

    // Count available razors
    map<int, int> razors;
    for (int v : x) razors[v]++;

    // We need to process segments where b[i] != a[i] and we need a razor of size b[i]
    // Use a stack to find the "maximal" needed cuts
    vector<int> needed;
    stack<int> st;
    for (int i = 0; i < n; ++i) {
        // We only care about positions where a[i] != b[i]
        // But also we need to consider that a cut of size b[i] can cover a segment
        // The idea: we process left to right, maintaining a stack of b[i] values that are "active"
        // When we see a new b[i], we pop from stack all values > b[i] (they can't extend past a smaller value)
        // If the stack is empty or top != b[i], we need a new razor of size b[i] (if a[i] != b[i])
        while (!st.empty() && st.top() > b[i]) {
            st.pop();
        }
        if (a[i] != b[i]) {
            if (st.empty() || st.top() != b[i]) {
                needed.push_back(b[i]);
                st.push(b[i]);
            }
        } else {
            // If a[i] == b[i], we still need to maintain stack for future cuts
            // But we don't need a razor for this position
            if (!st.empty() && st.top() == b[i]) {
                // already have this size on stack, do nothing
            } else {
                // push b[i] if it's not there? Actually we only push when we need a cut.
                // For a[i]==b[i], we don't need a cut, but the value b[i] can still be used as a "wall"
                // So we push it to stack to block larger values from extending.
                // But careful: if we push it, we might later pop it incorrectly.
                // Standard approach: push b[i] onto stack if it's not already top.
                if (st.empty() || st.top() != b[i]) {
                    st.push(b[i]);
                }
            }
        }
    }

    // Check if we have enough razors for needed sizes
    for (int v : needed) {
        if (razors[v] > 0) {
            razors[v]--;
        } else {
            cout << "NO\n";
            return;
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
