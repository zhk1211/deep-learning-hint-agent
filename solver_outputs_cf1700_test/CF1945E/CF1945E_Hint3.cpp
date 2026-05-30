// Hint3
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n, x;
        cin >> n >> x;
        vector<int> p(n + 1);
        int pos_x = -1;
        for (int i = 1; i <= n; ++i) {
            cin >> p[i];
            if (p[i] == x) pos_x = i;
        }

        // Simulate binary search to find the final l
        int l = 1, r = n + 1;
        while (r - l > 1) {
            int m = (l + r) / 2;
            if (p[m] <= x) l = m;
            else r = m;
        }

        // If p[l] == x, no swaps needed
        if (p[l] == x) {
            cout << "0\n";
            continue;
        }

        // We need at most 2 swaps.
        // First, swap x to position l.
        // But we must ensure that after swap, the binary search still ends at l.
        // Actually, we can just swap x to position l, then simulate again.
        // If still not correct, we can swap x with the element that is at the position
        // where binary search would go wrong. But simpler: we can just swap x to l,
        // then if needed, swap the element that is now at l with the element that
        // binary search would compare incorrectly.
        // However, the problem guarantees 2 operations are enough.
        // We can do: swap pos_x and l. Then simulate again. If p[l] == x, done with 1 swap.
        // Else, we need a second swap. The second swap should fix the binary search.
        // After first swap, x is at l. The binary search might still not end at l
        // because some earlier step might have taken the wrong branch.
        // We can find the position where binary search goes wrong and swap x with that element.
        // But a known trick: swap pos_x with l, then find the new l after simulation.
        // If new l != pos_x (original pos_x), swap new l with pos_x.
        // This always works in at most 2 swaps.

        vector<pair<int, int>> ops;
        swap(p[pos_x], p[l]);
        ops.push_back({pos_x, l});

        // Simulate again
        int l2 = 1, r2 = n + 1;
        while (r2 - l2 > 1) {
            int m = (l2 + r2) / 2;
            if (p[m] <= x) l2 = m;
            else r2 = m;
        }

        if (p[l2] != x) {
            // Need second swap: swap l2 with pos_x (which is now at l)
            // Actually after first swap, x is at l. So we swap l2 and l.
            swap(p[l2], p[l]);
            ops.push_back({l2, l});
        }

        cout << ops.size() << "\n";
        for (auto [i, j] : ops) {
            cout << i << " " << j << "\n";
        }
    }
    return 0;
}
