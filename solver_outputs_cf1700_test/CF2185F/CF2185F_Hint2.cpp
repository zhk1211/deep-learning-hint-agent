// Hint2
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

        // Precompute for each position the opponent XOR at each round
        // opp[i][k] = XOR of all elements in the opponent subtree at round k for position i
        // We'll compute this bottom-up.
        vector<vector<int>> opp(m, vector<int>(n + 1, 0));
        vector<int> cur = a;
        for (int k = 0; k < n; ++k) {
            int step = 1 << k;
            for (int i = 0; i < m; i += 2 * step) {
                // left block [i, i+step-1], right block [i+step, i+2*step-1]
                int left_xor = 0, right_xor = 0;
                for (int j = 0; j < step; ++j) {
                    left_xor ^= cur[i + j];
                    right_xor ^= cur[i + step + j];
                }
                for (int j = 0; j < step; ++j) {
                    opp[i + j][k] = right_xor;
                    opp[i + step + j][k] = left_xor;
                }
            }
            // update cur for next round
            vector<int> nxt(m / 2);
            for (int i = 0; i < m; i += 2 * step) {
                int left_xor = 0, right_xor = 0;
                for (int j = 0; j < step; ++j) {
                    left_xor ^= cur[i + j];
                    right_xor ^= cur[i + step + j];
                }
                // winner's new value = left_xor ^ right_xor
                int win_val = left_xor ^ right_xor;
                // winner is left if left_xor >= right_xor? Actually left wins if left_xor >= right_xor (tie left wins)
                // But for XOR sum of the new stack, it's always left_xor ^ right_xor regardless of winner.
                // However, the position of the winner matters for future rounds.
                // We need to track which original positions end up where.
                // This approach is getting complicated. Let's think differently.
            }
        }

        // Actually, we need a different approach.
        // Let's simulate the tournament as a binary tree.
        // Each node represents a segment of original positions.
        // We can precompute for each node the XOR sum of its segment.
        // Then for a query (pos, new_val), we can walk up the tree and compute the result.
        // The number of cows above the queried cow in the final stack is the number of cows that end up above it.
        // This is equal to the sum of sizes of subtrees that lose to the path from leaf to root.
        // We can compute this by walking up and comparing XOR sums.

        // Let's build segment tree over original array.
        // Each node stores XOR of its segment.
        vector<int> seg(2 * m);
        for (int i = 0; i < m; ++i) {
            seg[m + i] = a[i];
        }
        for (int i = m - 1; i > 0; --i) {
            seg[i] = seg[2 * i] ^ seg[2 * i + 1];
        }

        // For each query, we temporarily change the value at pos, then walk up.
        // We need to know for each node on the path, which child wins.
        // The winner's XOR becomes the XOR of both children.
        // The loser's subtree size is added to the answer.
        // We can compute this on the fly.

        while (q--) {
            int pos, new_val;
            cin >> pos >> new_val;
            --pos; // 0-indexed
            int old_val = a[pos];
            // We'll simulate the tournament with the modified value.
            // We only need to follow the path from leaf to root.
            int idx = m + pos;
            int cur_xor = new_val;
            int ans = 0;
            // We need to know the XOR of the sibling subtree at each step.
            // We can compute it using the segment tree, but we must account for the modified value.
            // Since only one leaf changes, we can compute sibling XOR by taking the node's XOR and removing the changed leaf's contribution if it's in that subtree.
            // Actually, we can just recompute the XOR of the sibling using the original seg tree and the fact that only pos changed.
            // Let's walk up level by level.
            int node = idx;
            int node_xor = new_val;
            while (node > 1) {
                int parent = node / 2;
                int sibling = node ^ 1;
                // Original XOR of sibling subtree
                int sib_xor = seg[sibling];
                // If the sibling subtree contains the modified position, we need to adjust.
                // But the modified position is only in our path, so sibling never contains it.
                // So sib_xor is correct.
                // Determine winner
                // Left child is even index, right child is odd.
                // In the tournament, left stack is at odd position (1st, 3rd, ...) and fights right stack.
                // In our tree, left child corresponds to the left stack, right child to right stack.
                // So left child is node with smaller index? Actually, if we number leaves 0..m-1, then left child is even index in the tree array.
                // The rule: left stack wins if its skill >= right stack's skill.
                int left_xor, right_xor;
                if (node % 2 == 0) {
                    // node is left child
                    left_xor = node_xor;
                    right_xor = sib_xor;
                } else {
                    left_xor = sib_xor;
                    right_xor = node_xor;
                }
                if (left_xor >= right_xor) {
                    // left wins
                    if (node % 2 == 0) {
                        // we are left, we win, sibling loses, add sibling size
                        // size of sibling subtree = number of leaves in it
                        // We can compute size based on level.
                        // Level: distance from leaf. We can track depth.
                        // Actually, we can compute size as 2^(n - depth).
                        // Let's track depth from bottom.
                    } else {
                        // we are right, we lose, add our own size? No, we want cows above us.
                        // If we lose, all cows in our current stack end up below the winner? Wait.
                        // The winning stack jumps on top of the losing stack.
                        // So the losing stack ends up at the bottom, winning stack on top.
                        // Therefore, cows in the losing stack will be below the winning stack.
                        // For a cow in the losing stack, the number of cows above it increases by the size of the winning stack.
                        // For a cow in the winning stack, the number of cows above it doesn't change from this fight.
                        // We are tracking the queried cow. If we lose, we add the size of the winning stack to ans.
                        // If we win, we add 0.
                    }
                } else {
                    // right wins
                    if (node % 2 == 0) {
                        // we are left, we lose
                    } else {
                        // we are right, we win
                    }
                }
                // Update node_xor for parent
                node_xor = left_xor ^ right_xor;
                node = parent;
            }
            cout << ans << '\n';
        }
    }
    return 0;
}
