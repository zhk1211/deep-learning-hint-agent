// Hint0
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n, q;
        cin >> n >> q;
        int m = 1 << n;
        vector<int> a(m);
        for (int i = 0; i < m; ++i) {
            cin >> a[i];
        }

        // Precompute the tournament tree
        // tree[node] = {winner_index, xor_sum}
        // winner_index is the index of the cow that ends up on top of the stack represented by this node
        // We'll store the entire stack order implicitly via the tree structure
        vector<pair<int, int>> tree(2 * m);
        for (int i = 0; i < m; ++i) {
            tree[m + i] = {i, a[i]};
        }
        for (int i = m - 1; i >= 1; --i) {
            auto &left = tree[2 * i];
            auto &right = tree[2 * i + 1];
            if (left.second > right.second || (left.second == right.second && left.first < right.first)) {
                tree[i] = {left.first, left.second ^ right.second};
            } else {
                tree[i] = {right.first, left.second ^ right.second};
            }
        }

        // For each query, we need to simulate the tournament with the modified skill
        // We can do this by updating the leaf and recomputing the path to the root
        // Since q can be up to 2e5 and n <= 18, O(q * n) is acceptable
        while (q--) {
            int b, c;
            cin >> b >> c;
            --b; // 0-indexed

            // Temporary update
            int pos = m + b;
            int old_val = tree[pos].second;
            tree[pos].second = c;

            // Recompute up the tree
            int cur = pos / 2;
            while (cur >= 1) {
                auto &left = tree[2 * cur];
                auto &right = tree[2 * cur + 1];
                if (left.second > right.second || (left.second == right.second && left.first < right.first)) {
                    tree[cur] = {left.first, left.second ^ right.second};
                } else {
                    tree[cur] = {right.first, left.second ^ right.second};
                }
                cur /= 2;
            }

            // Now we need to find how many cows are above cow b in the final stack
            // The final stack is represented by the root node tree[1]
            // The winner (top of stack) is tree[1].first
            // We need to count the number of cows above b in the stack order
            // The stack order is determined by the sequence of wins:
            // When two stacks merge, the winning stack goes on top of the losing stack.
            // So the final stack is a concatenation of the losing stacks in reverse order of their defeat,
            // with the ultimate winner on top.
            // We can compute the position of b by traversing from leaf to root and counting how many cows
            // are above it based on which side it was on and who won.
            
            // Alternative: We can compute the depth of b in the final stack by looking at the path from leaf to root.
            // At each node, if b's stack was the loser, then all cows in the winner's stack are above b.
            // If b's stack was the winner, then no cows from the loser's stack are above b (they are below).
            // We need to accumulate the sizes of the winner's stacks when b's stack loses.
            
            int ans = 0;
            int node = pos;
            while (node > 1) {
                int parent = node / 2;
                int sibling = (node % 2 == 0) ? node + 1 : node - 1;
                // Determine if the stack containing b won or lost at this parent
                auto &left = tree[2 * parent];
                auto &right = tree[2 * parent + 1];
                int winner_idx;
                if (left.second > right.second || (left.second == right.second && left.first < right.first)) {
                    winner_idx = left.first;
                } else {
                    winner_idx = right.first;
                }
                // The stack containing b is the one with winner_idx == b? Not exactly, because b might not be the top of its stack.
                // Actually, the tree stores the top cow of the stack at each node. So if tree[node].first == b, then b is on top of its stack.
                // But after updates, b might not be on top if it lost earlier? Wait, the tree is recomputed from scratch for each query,
                // so the tree always reflects the current state. At leaf, tree[pos].first = b. As we go up, if b's stack wins, tree[parent].first = b.
                // If b's stack loses, tree[parent].first = sibling's top. So we can check if tree[parent].first == b.
                // If yes, b's stack won, so no cows from sibling are above b.
                // If no, b's stack lost, so all cows in sibling's stack are above b.
                // The size of sibling's stack is 2^{depth} where depth is the level from bottom? 
                // Actually, the number of leaves in the subtree of sibling is the number of cows in that stack.
                // We can compute the size of sibling's subtree: it's a perfect binary tree of some height.
                // Height from leaf: node is at some level. The subtree size of sibling is 2^{h} where h is the height of sibling's subtree.
                // We can compute the height by finding the number of leaves in sibling's subtree.
                // Since the tree is a complete binary tree with leaves at level 0 (leaf), the size of subtree of a node at depth d (root depth n) is 2^{n-d}.
                // Let's compute depth of sibling: we can find the level by counting bits.
                int level = 0;
                int temp = sibling;
                while (temp < m) {
                    temp *= 2;
                    level++;
                }
                // Actually, easier: the number of leaves in sibling's subtree is 1 << (n - (int)log2(sibling)).
                // We can precompute log2 or use __builtin_clz.
                int sibling_leaves = 1;
                {
                    int x = sibling;
                    while (x < m) {
                        x *= 2;
                    }
                    // x is now >= m, the number of leaves is x / m? No.
                    // The subtree size in terms of leaves is m / (1 << (31 - __builtin_clz(sibling)))?
                    // Let's just compute the height from leaf: the number of times we can divide sibling by 2 until we reach a leaf? No.
                    // The subtree of sibling has size = number of leaves in that subtree.
                    // Since the tree is perfect, the number of leaves in subtree of node i is m / (1 << (int)log2(i)).
                    // Because node 1 has m leaves, node 2 has m/2, node 4 has m/4, etc.
                    // So sibling_leaves = m / (1 << (31 - __builtin_clz(sibling)));
                }
                // Actually, we can just use the fact that the subtree size is 1 << (n - depth_of_node).
                // depth_of_node = floor(log2(node)).
                int depth_node = 31 - __builtin_clz(node);
                int depth_sibling = 31 - __builtin_clz(sibling);
                int sibling_size = 1 << (n - depth_sibling);
                
                if (tree[parent].first != b) {
                    ans += sibling_size;
                }
                node = parent;
            }
            cout << ans << '\n';

            // Restore the original value
            tree[pos].second = old_val;
            cur = pos / 2;
            while (cur >= 1) {
                auto &left = tree[2 * cur];
                auto &right = tree[2 * cur + 1];
                if (left.second > right.second || (left.second == right.second && left.first < right.first)) {
                    tree[cur] = {left.first, left.second ^ right.second};
                } else {
                    tree[cur] = {right.first, left.second ^ right.second};
                }
                cur /= 2;
            }
        }
    }
    return 0;
}
