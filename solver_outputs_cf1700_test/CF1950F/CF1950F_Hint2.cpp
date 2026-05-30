// Hint2
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
        
        // Check if tree is possible: c must be a + 1
        if (c != a + 1) {
            cout << -1 << '\n';
            continue;
        }
        
        if (a == 0) {
            // Only b nodes with 1 child and c leaves
            // Structure: root with 1 child, chain of b nodes, then c leaves
            // Actually if a=0, c=1, so we have b nodes with 1 child and 1 leaf
            // Height = b
            cout << b << '\n';
            continue;
        }
        
        // General case: a > 0, c = a + 1
        // We want to minimize height by building a tree level by level
        // Start with root (level 0)
        // We have a nodes with 2 children, b nodes with 1 child
        // We'll place nodes level by level, using 2-child nodes first to grow width
        
        // Calculate the height based on how many levels we need to place all a nodes
        // Each level i (0-indexed) can have at most 2^i nodes with 2 children
        // We need to find the minimum height h such that we can place all a nodes
        // and then place b nodes on the last level(s)
        
        int height = 0;
        int nodes_this_level = 1; // root
        int total_a_placed = 0;
        
        // First, place all a nodes using full binary tree structure as much as possible
        while (total_a_placed + nodes_this_level <= a) {
            total_a_placed += nodes_this_level;
            nodes_this_level *= 2;
            height++;
        }
        
        // Now we have some remaining a nodes to place on current level
        int remaining_a = a - total_a_placed;
        
        // The current level has 'nodes_this_level' positions, we use 'remaining_a' of them for 2-child nodes
        // The rest (nodes_this_level - remaining_a) will be for 1-child nodes or leaves
        int slots_for_b_on_this_level = nodes_this_level - remaining_a;
        
        // The 2-child nodes on this level will produce 2*remaining_a children on next level
        // The 1-child nodes on this level will produce slots_for_b_on_this_level children on next level
        // But we also need to place b nodes somewhere
        
        // Actually, we need to place b nodes with 1 child. They can be placed on any level.
        // To minimize height, we should place them as high as possible, but we already filled higher levels with 2-child nodes.
        // After placing all a nodes, we have some structure. The remaining b nodes can be attached to the last level.
        
        // Let's recalculate: after placing all a nodes, what is the number of nodes on the last level?
        // The last level (where we placed the last a nodes) has:
        // - remaining_a nodes with 2 children
        // - some nodes with 1 child (if we put b nodes there)
        // - some leaves (if we put c nodes there, but c is fixed)
        
        // Better approach: The height is determined by the level where we place the last node.
        // We can think of building the tree level by level, using 2-child nodes first.
        // After placing all a nodes, we have a certain number of "slots" on the next level.
        // These slots will be filled by children of the last level's nodes.
        // The children can be: 2-child nodes (but we have no more a), 1-child nodes (b), or leaves (c).
        // Since c = a+1, and total nodes = a+b+c, we can compute the number of nodes on each level.
        
        // Let's simulate level by level:
        // Level 0: 1 node (root)
        // We'll distribute a and b to minimize height.
        
        // Actually, we can compute the height directly:
        // If we have a nodes with 2 children, the maximum number of leaves is a+1.
        // The tree can be seen as a binary tree where some internal nodes have 1 child.
        // The height is the length of the longest path from root to leaf.
        // To minimize height, we want to make the tree as "bushy" as possible, i.e., use 2-child nodes at higher levels.
        
        // Let's compute the number of nodes at each level if we use all 2-child nodes first.
        // We already found the level where we place the last 2-child node.
        // Let's call that level L (0-indexed). At level L, we placed 'remaining_a' 2-child nodes.
        // The remaining positions on level L are filled with either 1-child nodes or leaves.
        // But we also have b 1-child nodes to place. They can be placed on level L or higher.
        // Actually, we can place 1-child nodes on any level except the last one (since they need a child).
        
        // Let's compute the total number of nodes we can place up to each level.
        // We'll find the minimum height such that we can accommodate all a, b, c.
        
        // Alternative: The height is the number of edges on the longest path.
        // We can think of it as: we need to place a+b+c nodes.
        // The number of nodes at level i is at most 2^i, but limited by a and b.
        // We can greedily assign 2-child nodes to earlier levels.
        
        // Let's compute the level of the last leaf.
        // We know c = a+1 leaves. All leaves are at the bottom.
        // The height is the maximum depth of any leaf.
        
        // Let's compute the depth of leaves when we arrange optimally.
        // We have a 2-child nodes. They create a binary tree structure.
        // If we ignore 1-child nodes, the height of a full binary tree with a internal nodes is ceil(log2(a+1)).
        // But we also have b 1-child nodes which can extend some paths.
        
        // Let's compute the minimum height H such that we can place all nodes.
        // At each level i from 0 to H-1, we can have some number of nodes.
        // The total number of nodes at levels 0..H-1 is at most something.
        // Actually, we can compute the maximum number of leaves at depth <= H.
        
        // Let's use a different approach: The height is the maximum depth.
        // We can binary search on height, but constraints are small enough to compute directly.
        
        // Let's compute the number of nodes we can have at each depth if we minimize height.
        // We'll build the tree top-down: at each level, we have some number of "available slots" for children.
        // Initially, level 0 has 1 node (root). This node can be either 2-child, 1-child, or leaf.
        // We want to use 2-child nodes as early as possible to increase width.
        
        // Let's simulate:
        int curr_level_nodes = 1; // nodes at current level that need to be assigned a type
        int a_left = a, b_left = b;
        int h = 0;
        
        while (a_left > 0 || b_left > 0 || curr_level_nodes > 0) {
            // We'll process current level
            // The number of nodes at this level is curr_level_nodes.
            // We can assign some of them as 2-child (using a_left), some as 1-child (using b_left), rest as leaves.
            // But we want to minimize height, so we should use 2-child nodes first.
            
            if (a_left >= curr_level_nodes) {
                // All nodes on this level become 2-child
                a_left -= curr_level_nodes;
                // They produce 2*curr_level_nodes children for next level
                curr_level_nodes *= 2;
            } else {
                // We use 'a_left' nodes as 2-child, and the rest we need to fill with 1-child or leaves.
                int use_a = a_left;
                a_left = 0;
                int remaining_slots = curr_level_nodes - use_a;
                
                // We can use some of remaining_slots for 1-child nodes (b_left)
                int use_b = min(b_left, remaining_slots);
                b_left -= use_b;
                remaining_slots -= use_b;
                
                // The rest remaining_slots become leaves (c is fixed, but we don't track c here)
                // The next level will have children from 2-child and 1-child nodes:
                // 2*use_a + use_b
                curr_level_nodes = 2 * use_a + use_b;
                
                // If we still have b_left > 0, we can attach them as 1-child nodes on deeper levels.
                // But we already used all slots on this level. The remaining b_left will have to be placed on next levels.
                // However, we can also attach 1-child nodes as children of leaves? No, leaves have 0 children.
                // So b_left must be placed on nodes that are not leaves. We'll handle them in subsequent iterations.
            }
            
            h++;
            
            // If no nodes for next level and nothing left, break
            if (curr_level_nodes == 0 && a_left == 0 && b_left == 0) break;
        }
        
        // The height is h-1 (since we incremented h after processing the last level that had nodes)
        // But wait, if the last level only had leaves, the height is the number of edges from root to those leaves.
        // In our simulation, h counts the number of levels processed. The root is level 0.
        // If we process level 0, h becomes 1. The leaves on level 0 would mean height 0.
        // Let's adjust: height = h - 1.
        
        cout << h - 1 << '\n';
    }
    
    return 0;
}
