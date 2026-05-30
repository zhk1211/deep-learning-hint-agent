// Hint2
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
        
        int l = 1, r = n + 1;
        while (r - l > 1) {
            int m = (l + r) / 2;
            if (p[m] <= x) l = m;
            else r = m;
        }
        
        if (p[l] == x) {
            cout << "0\n";
            continue;
        }
        
        // We need to make p[l] == x after at most 2 swaps.
        // First, swap x to position l.
        // But we must ensure the binary search path remains the same,
        // or we can do a second swap to fix it.
        // Actually, we can just swap pos_x and l.
        // Then simulate again to see if it works.
        // If not, we can do one more swap to fix the "pivot" that caused the wrong path.
        
        vector<pair<int,int>> ops;
        ops.push_back({pos_x, l});
        swap(p[pos_x], p[l]);
        
        // Simulate again
        int l2 = 1, r2 = n + 1;
        while (r2 - l2 > 1) {
            int m = (l2 + r2) / 2;
            if (p[m] <= x) l2 = m;
            else r2 = m;
        }
        
        if (p[l2] == x) {
            cout << "1\n";
            cout << ops[0].first << " " << ops[0].second << "\n";
            continue;
        }
        
        // Need second swap.
        // The problem is that some pivot m gave wrong direction.
        // We can swap the element at the first wrong pivot with something that fixes it.
        // Actually, we can just swap the element at the position that should have been the correct l
        // with the element that is currently at that position.
        // But simpler: we know that after first swap, x is at l (original l).
        // The binary search might have gone wrong because some pivot had value <= x but should have been > x, or vice versa.
        // We can find the first pivot where the condition p[m] <= x gave the wrong result.
        // But we can also just swap the element at the final l2 with x (which is at original l).
        // However, x is already at original l. If we swap l2 and original l, we might fix it.
        // Let's try: swap p[l2] and p[original l].
        // But careful: original l is ops[0].second.
        int orig_l = ops[0].second;
        ops.push_back({l2, orig_l});
        swap(p[l2], p[orig_l]);
        
        // Simulate third time (not necessary for output, but for verification)
        // We can just output 2 and the ops.
        cout << "2\n";
        cout << ops[0].first << " " << ops[0].second << "\n";
        cout << ops[1].first << " " << ops[1].second << "\n";
    }
    return 0;
}
