// Hint3
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

    // Check if b[i] > a[i] anywhere -> impossible
    for (int i = 0; i < n; ++i) {
        if (b[i] > a[i]) {
            cout << "NO\n";
            return;
        }
    }

    // Count available razors
    map<int, int> razor_count;
    for (int val : x) razor_count[val]++;

    // We need to process cuts for each distinct b[i] value
    // We'll use a stack to maintain the "active" cuts
    // and a map to count required razors of each size.
    map<int, int> needed;
    stack<int> st; // stores b[i] values in increasing order? Actually we need to handle cuts.

    // We iterate left to right, maintaining a monotonic stack of b[i] values
    // that represent the current "open" cuts.
    for (int i = 0; i < n; ++i) {
        // Remove cuts that are too small (they can't cover this position because they'd be overridden)
        while (!st.empty() && st.top() < b[i]) {
            st.pop();
        }
        // If we need a cut of size b[i] and it's not already covered by a larger cut
        if (a[i] != b[i]) {
            if (st.empty() || st.top() != b[i]) {
                // We need to start a new cut of size b[i]
                needed[b[i]]++;
                st.push(b[i]);
            }
        }
        // If a[i] == b[i], no cut needed here, but existing cuts might still be active.
        // However, if b[i] is smaller than some active cut, that active cut would have been popped.
        // Actually, if a[i] == b[i], we don't need to start a cut, but we must ensure that
        // any active cut larger than b[i] is still valid? No, because if a[i] == b[i],
        // the hair is already correct, and we don't need to apply any cut here.
        // But if there's an active cut with size > b[i], it would set a[i] to min(a[i], size) = b[i] (since b[i] < size),
        // which is fine. However, if active cut size == b[i], it's also fine.
        // The only issue is if active cut size < b[i], but we popped those.
    }

    // Check if we have enough razors for each needed size
    for (auto& p : needed) {
        int size = p.first;
        int cnt = p.second;
        if (razor_count[size] < cnt) {
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
