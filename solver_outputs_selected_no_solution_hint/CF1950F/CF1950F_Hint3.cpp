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
        
        // Check if total vertices condition holds: a + b + c must be 1 + 2*a + 1*b
        // Because sum of children = 2*a + 1*b + 0*c = total vertices - 1 (except root)
        if (a + b + c != 2 * a + b + 1) {
            cout << -1 << '\n';
            continue;
        }
        
        // Special case: only one vertex
        if (a == 0 && b == 0 && c == 1) {
            cout << 0 << '\n';
            continue;
        }
        
        // If no vertices with 2 children, tree is a path: height = b + c - 1? Actually b + c vertices, height = b
        // But we need to handle general case.
        // We'll simulate building the tree level by level.
        // At each level we have some number of "slots" (nodes that can have children).
        // Initially, root is one slot.
        // We want to minimize height, so we fill slots with 2-children nodes first (they create more slots for next level).
        // Then fill with 1-child nodes, then leaves.
        
        int height = 0;
        int slots = 1; // nodes at current level that need children
        
        while (slots > 0) {
            // If no more nodes to place, break
            if (a == 0 && b == 0 && c == 0) break;
            
            // We will fill current slots with available nodes
            int use_a = min(slots, a);
            a -= use_a;
            slots -= use_a;
            int new_slots = use_a * 2;
            
            int use_b = min(slots, b);
            b -= use_b;
            slots -= use_b;
            new_slots += use_b;
            
            int use_c = min(slots, c);
            c -= use_c;
            slots -= use_c;
            // leaves add no new slots
            
            // If we still have slots left but no nodes to fill them, impossible
            if (slots > 0 && a == 0 && b == 0 && c == 0) {
                height = -1;
                break;
            }
            
            slots = new_slots;
            height++;
        }
        
        // If there are remaining nodes, impossible
        if (a > 0 || b > 0 || c > 0) height = -1;
        
        cout << height << '\n';
    }
    
    return 0;
}
