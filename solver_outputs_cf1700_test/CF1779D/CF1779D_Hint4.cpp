// Hint4
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
    map<int, int> razor_count;
    for (int v : x) razor_count[v]++;

    // We need to process segments where b[i] != a[i] and b[i] is the target value.
    // Use a stack to find the "maximal" needed cuts.
    // We iterate left to right, maintaining a stack of needed b[i] values that are not yet covered.
    // When we see a b[i] that is larger than the top of stack, we might need a new razor for it.
    // If b[i] is smaller, we pop until stack top <= b[i] or empty.
    // Actually, we need to assign razors to the "peaks" of b in the segments where a[i] != b[i].
    // Standard approach: for each distinct value in b that appears where a[i] != b[i],
    // we need to cover its contiguous segments with razors of that size.
    // We can use a stack to simulate the process of cutting from left to right.
    
    stack<int> st;
    map<int, int> needed; // how many razors of each size are needed
    for (int i = 0; i < n; ++i) {
        // We only care about positions where a[i] != b[i]
        if (a[i] == b[i]) {
            // If we have a pending cut, we might need to finalize it when we leave the segment?
            // Actually, if a[i] == b[i], it acts as a barrier: cuts cannot cross it.
            // So we clear the stack.
            while (!st.empty()) st.pop();
            continue;
        }
        // Now a[i] > b[i] (since we already checked b[i] <= a[i])
        int val = b[i];
        // Remove from stack any values greater than val, because a cut of size val cannot cover them.
        while (!st.empty() && st.top() > val) {
            st.pop();
        }
        // If stack is empty or top < val, we need a new razor of size val.
        if (st.empty() || st.top() < val) {
            needed[val]++;
            st.push(val);
        }
        // If top == val, it's already covered by an ongoing cut.
    }

    // Check if we have enough razors
    for (auto &p : needed) {
        int size = p.first;
        int req = p.second;
        if (razor_count[size] < req) {
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
