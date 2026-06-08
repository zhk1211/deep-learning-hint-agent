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
        
        // A tree with a+b+c vertices must have a+b+c-1 edges.
        // Also, sum of children = 2*a + 1*b + 0*c = 2a + b.
        // In any rooted tree, sum of children = number of edges = total vertices - 1.
        // So we must have 2a + b = a + b + c - 1  =>  a = c - 1.
        if (a != c - 1) {
            cout << -1 << "\n";
            continue;
        }
        
        // Now we need to construct the tree with minimum height.
        // We have a nodes with 2 children, b nodes with 1 child, c leaves.
        // The total number of nodes is a+b+c.
        // We can think of building the tree level by level.
        // At each level, we have some number of "available slots" for children.
        // Initially, at level 0, we have the root (1 slot).
        // We want to minimize height, so we should fill levels as much as possible.
        // We process nodes with 2 children first (they give more slots) to keep the tree shallow.
        // Then we use nodes with 1 child.
        // Leaves (0 children) just consume slots without creating new ones.
        
        int height = 0;
        int slots = 1; // number of nodes we can place at the current level
        int remaining_a = a;
        int remaining_b = b;
        int remaining_c = c;
        
        // We'll simulate level by level.
        while (slots > 0 && (remaining_a > 0 || remaining_b > 0 || remaining_c > 0)) {
            // Number of nodes we will place at this level is limited by slots.
            int nodes_this_level = slots;
            int next_slots = 0;
            
            // First use nodes with 2 children as much as possible.
            int use_a = min(remaining_a, nodes_this_level);
            nodes_this_level -= use_a;
            remaining_a -= use_a;
            next_slots += use_a * 2;
            
            // Then use nodes with 1 child.
            int use_b = min(remaining_b, nodes_this_level);
            nodes_this_level -= use_b;
            remaining_b -= use_b;
            next_slots += use_b * 1;
            
            // The rest must be leaves (0 children).
            int use_c = min(remaining_c, nodes_this_level);
            nodes_this_level -= use_c;
            remaining_c -= use_c;
            // Leaves add 0 to next_slots.
            
            // If we couldn't fill all slots, that means we ran out of nodes, but that's fine.
            // Move to next level.
            slots = next_slots;
            height++;
        }
        
        // If we still have nodes left, something went wrong (shouldn't happen if a = c-1).
        if (remaining_a > 0 || remaining_b > 0 || remaining_c > 0) {
            cout << -1 << "\n";
        } else {
            cout << height << "\n";
        }
    }
    
    return 0;
}
