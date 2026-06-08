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

        // Check necessary condition: total vertices = a + b + c
        // In a rooted tree, sum of children = a*2 + b*1 + c*0 = 2a + b
        // This must equal total vertices - 1 (since root has no parent)
        // So 2a + b = a + b + c - 1  => a + 1 = c
        if (c != a + 1) {
            cout << "-1\n";
            continue;
        }

        // Special case: only root
        if (a == 0 && b == 0 && c == 1) {
            cout << "0\n";
            continue;
        }

        // We build the tree level by level.
        // Start with root (level 0). It can have up to 2 children.
        // We'll simulate BFS-like construction.
        // At each level, we have some number of nodes that need children.
        // We first use nodes with 2 children (a), then nodes with 1 child (b).
        // We want to minimize height, so we fill levels as much as possible.

        // We'll track how many nodes at current level can have children.
        // Initially, root can have up to 2 children.
        int height = 0;
        int nodes_with_2 = 1; // root can take up to 2 children
        int nodes_with_1 = 0; // no nodes with 1 child yet
        int remaining_a = a;
        int remaining_b = b;
        int remaining_c = c - 1; // root already counted

        while (remaining_a > 0 || remaining_b > 0 || remaining_c > 0) {
            // Number of nodes at next level that will need children
            int next_nodes_with_2 = 0;
            int next_nodes_with_1 = 0;

            // Process nodes that can have 2 children
            while (nodes_with_2 > 0 && (remaining_a > 0 || remaining_b > 0 || remaining_c > 0)) {
                if (remaining_a > 0) {
                    // use one a node: it takes 2 children spots
                    remaining_a--;
                    next_nodes_with_2++;
                    nodes_with_2--;
                } else if (remaining_b > 0) {
                    // use one b node: it takes 1 child spot
                    remaining_b--;
                    next_nodes_with_1++;
                    nodes_with_2--;
                } else {
                    // only c nodes left: they take 0 children spots
                    // but we still need to place them as children
                    remaining_c--;
                    nodes_with_2--;
                }
            }

            // Process nodes that can have 1 child
            while (nodes_with_1 > 0 && (remaining_a > 0 || remaining_b > 0 || remaining_c > 0)) {
                if (remaining_a > 0) {
                    remaining_a--;
                    next_nodes_with_2++;
                    nodes_with_1--;
                } else if (remaining_b > 0) {
                    remaining_b--;
                    next_nodes_with_1++;
                    nodes_with_1--;
                } else {
                    remaining_c--;
                    nodes_with_1--;
                }
            }

            // If we still have nodes that can have children but no remaining nodes to place,
            // that's fine, they just don't get children.
            // Move to next level
            nodes_with_2 = next_nodes_with_2;
            nodes_with_1 = next_nodes_with_1;
            height++;

            // If no nodes at next level can have children, but we still have nodes to place,
            // we need to continue placing them as children of nodes that can't have children?
            // Actually, if we have remaining nodes but no nodes that can take children,
            // it's impossible (should not happen if c = a+1).
            if (nodes_with_2 == 0 && nodes_with_1 == 0 && (remaining_a > 0 || remaining_b > 0 || remaining_c > 0)) {
                // This would mean we have nodes left but no parent slots.
                // But with c = a+1, this shouldn't occur.
                height = -1;
                break;
            }
        }

        cout << height << "\n";
    }

    return 0;
}
