// Hint0
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
            cout << -1 << "\n";
            continue;
        }
        
        // If no vertices with children, height is 0
        if (a == 0 && b == 0) {
            cout << 0 << "\n";
            continue;
        }
        
        // Calculate height
        // First, build the perfect binary tree part with a internal nodes
        // Height of perfect binary tree with a internal nodes (all have 2 children)
        // Number of leaves in perfect binary tree = a + 1
        // Height h of perfect binary tree: 2^{h+1} - 1 >= a? Actually we need to find min height
        // where we can place a nodes with 2 children.
        // We can compute the height level by level.
        
        int height = 0;
        int nodes_at_level = 1; // root level
        int remaining_a = a;
        int remaining_b = b;
        
        // We'll simulate level by level
        while (remaining_a > 0 || remaining_b > 0) {
            if (nodes_at_level == 0) {
                // Should not happen if c == a+1
                break;
            }
            
            // First use nodes_at_level to place a-nodes (2 children)
            int use_for_a = min(remaining_a, nodes_at_level);
            remaining_a -= use_for_a;
            int nodes_left = nodes_at_level - use_for_a;
            
            // Next use remaining nodes at this level for b-nodes (1 child)
            int use_for_b = min(remaining_b, nodes_left);
            remaining_b -= use_for_b;
            nodes_left -= use_for_b;
            
            // The children of a-nodes (2 each) and b-nodes (1 each) will be at next level
            nodes_at_level = use_for_a * 2 + use_for_b;
            
            height++;
        }
        
        cout << height << "\n";
    }
    
    return 0;
}
