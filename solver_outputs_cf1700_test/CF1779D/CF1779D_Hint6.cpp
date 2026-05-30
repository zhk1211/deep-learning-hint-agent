// Hint6
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

    // Check if any b[i] > a[i] -> impossible
    for (int i = 0; i < n; i++) {
        if (b[i] > a[i]) {
            cout << "NO\n";
            return;
        }
    }

    // Count available razors
    map<int, int> razor_count;
    for (int val : x) razor_count[val]++;

    // We need to process segments where b[i] != a[i] and we need a razor of size b[i]
    // Use a stack to handle the "largest required razor" for each segment
    stack<int> st;
    for (int i = 0; i < n; i++) {
        // We only care about positions where a[i] != b[i]
        // For those, we need a razor of size b[i] to cut down to b[i]
        // The razor must be applied on a segment that covers this position.
        // The segment's left bound can extend left as long as b[j] <= b[i] and we haven't hit a position where b[j] > b[i] (which would require a larger razor).
        // We process left to right, maintaining a stack of needed razor sizes.
        // When we see a new b[i] that is larger than the top of stack, we need a new razor.
        // If it's equal, it can be covered by the same segment.
        // If it's smaller, we pop until stack top <= b[i], then if stack top != b[i] we push b[i] (need new razor).
        while (!st.empty() && st.top() > b[i]) {
            st.pop();
        }
        if (a[i] != b[i]) {
            if (st.empty() || st.top() != b[i]) {
                st.push(b[i]);
                if (razor_count[b[i]] == 0) {
                    cout << "NO\n";
                    return;
                }
                razor_count[b[i]]--;
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
