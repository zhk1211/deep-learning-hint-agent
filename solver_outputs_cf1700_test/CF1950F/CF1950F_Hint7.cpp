// Hint7
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int a, b, c;
        cin >> a >> b >> c;
        
        // Check necessary condition: c must be a + 1
        if (c != a + 1) {
            cout << -1 << '\n';
            continue;
        }
        
        // If no vertices with children, height is 0
        if (a == 0 && b == 0) {
            cout << 0 << '\n';
            continue;
        }
        
        // Build tree level by level
        // Start with root (level 0)
        int height = 0;
        int leaves = 1; // current number of leaves at this level
        
        // First use all 'a' vertices (with 2 children) to expand leaves
        // Each such vertex consumes one leaf and creates two new leaves
        while (a > 0) {
            if (leaves == 0) {
                height++;
                // No leaves to expand, but we still have 'a' vertices - impossible?
                // Actually this shouldn't happen if c = a+1, but handle safely
                break;
            }
            int use = min(leaves, a);
            a -= use;
            leaves -= use;
            leaves += 2 * use;
            height++;
        }
        
        // Now use 'b' vertices (with 1 child)
        // Each consumes one leaf and creates one new leaf
        while (b > 0) {
            if (leaves == 0) {
                height++;
                break;
            }
            int use = min(leaves, b);
            b -= use;
            leaves -= use;
            leaves += use;
            height++;
        }
        
        // After processing all a and b, we should have exactly c leaves
        // The height is the number of levels we expanded
        cout << height << '\n';
    }
    
    return 0;
}
