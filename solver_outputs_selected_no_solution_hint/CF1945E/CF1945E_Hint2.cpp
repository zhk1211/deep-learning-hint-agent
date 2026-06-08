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
        for (int i = 1; i <= n; i++) {
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
        
        // p[l] != x, we need at most 2 swaps
        // First swap: put x at position l
        cout << "1\n";
        cout << pos_x << " " << l << "\n";
        swap(p[pos_x], p[l]);
        pos_x = l;
        
        // Now run binary search again to see if it works
        l = 1, r = n + 1;
        while (r - l > 1) {
            int m = (l + r) / 2;
            if (p[m] <= x) l = m;
            else r = m;
        }
        
        if (p[l] == x) {
            continue;
        }
        
        // Need second swap: find where x should be to make binary search find it
        // We know x is at pos_x, and binary search ended at l with p[l] != x
        // We need to swap x with the element that binary search would compare at the step where it goes wrong
        // Actually simpler: just swap x with the element at the position that binary search should have checked
        // to make it go to x. We can find the correct position by simulating binary search with target x
        // and swapping x with the element at the position where the search path diverges.
        
        // Find the position that binary search would check that leads to x
        int target_pos = -1;
        int low = 1, high = n + 1;
        while (high - low > 1) {
            int mid = (low + high) / 2;
            if (mid == pos_x) {
                // This is the step where we want binary search to go towards x
                // We need p[mid] <= x to be true, which it is since p[pos_x] = x
                // But we need to ensure the search continues correctly
                low = mid;
            } else if (p[mid] <= x) {
                low = mid;
            } else {
                high = mid;
            }
        }
        // Actually, we need to find the position that is currently preventing binary search from finding x
        // Let's find the position that binary search would check that is on the path to x
        // We can just swap x with the element at position l (where binary search ended)
        // But we already did that? Wait, after first swap, x is at l, but binary search still didn't find it?
        // That means the binary search path changed because we swapped.
        
        // Let's re-simulate: after first swap, x is at l. Binary search might not find it because
        // the comparisons at earlier steps might have changed.
        // We need to find a position to swap with x such that binary search finds x.
        // The correct approach: find the position that binary search would check that is on the path to x,
        // and swap x with the element there.
        
        // Let's find the sequence of m values that binary search checks when searching for x in a sorted array.
        // Actually, we can just find the position where x should be to be found.
        // We know x is at pos_x. We want binary search to end at pos_x.
        // So we need to ensure that for every step, if m < pos_x, p[m] <= x, and if m > pos_x, p[m] > x.
        // We can find a position that violates this and swap x with it.
        
        // Find the first m in the binary search path that is on the wrong side.
        int wrong_pos = -1;
        low = 1, high = n + 1;
        while (high - low > 1) {
            int mid = (low + high) / 2;
            if (mid == pos_x) {
                low = mid;
            } else if (mid < pos_x) {
                if (p[mid] > x) {
                    wrong_pos = mid;
                    break;
                }
                low = mid;
            } else {
                if (p[mid] <= x) {
                    wrong_pos = mid;
                    break;
                }
                high = mid;
            }
        }
        
        if (wrong_pos != -1) {
            cout << "1\n";
            cout << pos_x << " " << wrong_pos << "\n";
        } else {
            // If no wrong position found, but binary search still didn't find x,
            // it means the issue is that x itself is not at the final l.
            // This can happen if x is at a position that binary search never reaches as l.
            // In that case, we can just swap x with the element at the final l.
            // But we already did that? Wait, we need to output the second swap.
            // Actually, the problem guarantees 2 operations are enough.
            // Let's just swap x with the element at the position that binary search ends at.
            // But we need to output it.
            // Since we already did one swap, we need one more.
            // Let's find the final l after the first swap and swap x with it.
            // But we already computed l after first swap, and p[l] != x.
            // So we swap x (at pos_x) with p[l].
            cout << "1\n";
            cout << pos_x << " " << l << "\n";
        }
    }
    return 0;
}
