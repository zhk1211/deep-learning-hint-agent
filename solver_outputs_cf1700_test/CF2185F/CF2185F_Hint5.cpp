// Hint5
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
        // For each node, store the winner index and the XOR sum of the segment
        // We'll build a segment tree over the array
        int size = 1;
        while (size < m) size <<= 1;
        vector<int> seg_winner(2 * size, -1);
        vector<int> seg_xor(2 * size, 0);
        for (int i = 0; i < m; ++i) {
            seg_winner[size + i] = i;
            seg_xor[size + i] = a[i];
        }
        for (int i = size - 1; i > 0; --i) {
            int left = seg_winner[2 * i];
            int right = seg_winner[2 * i + 1];
            if (left == -1) {
                seg_winner[i] = right;
                seg_xor[i] = seg_xor[2 * i + 1];
            } else if (right == -1) {
                seg_winner[i] = left;
                seg_xor[i] = seg_xor[2 * i];
            } else {
                int left_xor = seg_xor[2 * i];
                int right_xor = seg_xor[2 * i + 1];
                if (left_xor > right_xor || (left_xor == right_xor && left < right)) {
                    seg_winner[i] = left;
                    seg_xor[i] = left_xor ^ right_xor;
                } else {
                    seg_winner[i] = right;
                    seg_xor[i] = left_xor ^ right_xor;
                }
            }
        }

        // For each query, we need to simulate the tournament with the modified skill
        // We can do this by walking up the tree from the leaf of the modified cow
        // and recomputing the winner at each level, keeping track of the stack
        // that the modified cow ends up in.
        // However, we need to know how many cows are above the modified cow in the final stack.
        // The final stack is the winner of the root.
        // If the modified cow is not the root winner, then it lost at some level,
        // and the number of cows above it is the size of the stack that beat it
        // at the level it lost, plus the cows that were above it in its own stack
        // when it lost.
        // Actually, the problem asks: "how many cows are above the cow that was given the potion in the final stack."
        // The final stack is the stack of the overall winner.
        // If the modified cow is the overall winner, then the cows above it are those
        // that were in its stack and above it. Since it is the winner, its stack
        // is the final stack, and the cows above it are exactly the cows that were
        // in the losing stacks that it absorbed, in the order they were absorbed.
        // But we need the count, not the order.
        // Let's think differently.
        // We can simulate the tournament for each query by walking up the tree.
        // At each node, we know the two children. We can compute the winner and the XOR sum
        // of the node given the current skill of the modified cow.
        // We start at the leaf of the modified cow, with its new skill.
        // We maintain the current stack's XOR sum and the index of the top cow (the winner of that stack).
        // Initially, the stack is just the modified cow, so XOR sum = new skill, top = modified cow index.
        // We also maintain the number of cows in the current stack.
        // As we go up, we meet the opponent stack. The opponent stack's XOR sum and winner
        // can be obtained from the precomputed segment tree if the opponent stack does not contain
        // the modified cow. If the opponent stack contains the modified cow, then we are merging
        // two parts of the same original segment? Actually, the modified cow is only in one leaf,
        // so at each level, exactly one child contains the modified cow, and the other child
        // is a normal precomputed stack.
        // So we can compare the current stack's XOR sum with the opponent's precomputed XOR sum.
        // The winner's stack will have XOR sum = current_xor ^ opponent_xor.
        // If current stack wins, then the opponent stack is placed below the current stack,
        // so the cows above the modified cow do not change (the opponent goes below).
        // If the opponent stack wins, then the opponent stack is placed on top of the current stack,
        // so all cows in the opponent stack end up above the modified cow.
        // In that case, the modified cow's stack becomes part of the opponent's stack,
        // and the new top cow is the opponent's winner.
        // We continue until we reach the root.
        // At the end, if the modified cow is the overall winner, the number of cows above it
        // is the total number of cows in the final stack minus 1 (since it is at the top? Wait, no.
        // The final stack has the winner on top. If the modified cow is the winner, it is on top,
        // so there are 0 cows above it? But the sample says for the first query, answer is 1.
        // Let's re-read the sample.
        // In the first test case, first query: potion to cow 1, new skill 1 (same as original).
        // Final stack: cows 3,4,1,2 from bottom to top? The note says: "final stack has cows 3,4,1,2 in that order"
        // and "its skill level is now 2^2=0". The winner is the first stack (cows 1,2) because tie and leftmost wins.
        // The first stack originally had cow 1 at bottom, cow 2 at top? Wait: "Cows 1 and 2 fight. Since cow 2's skill level is higher than cow 1's skill level, cow 2 wins the fight, so its stack has cows 1,2 in that order"
        // So stack has bottom cow 1, top cow 2. Then it fights with stack (3,4) where cow 4 won, so stack (3,4) with bottom 3, top 4.
        // First stack wins, so it jumps on top of the losing stack. The losing stack is (3,4). The winning stack (1,2) jumps on top.
        // So final stack: bottom 3,4, then 1,2 on top. So from bottom to top: 3,4,1,2.
        // Cow 1 is in the final stack. How many cows above cow 1? Cow 2 is above cow 1. So answer 1.
        // Cow 1 is not the overall winner (cow 2 is the winner). So if the modified cow is not the winner,
        // the cows above it are those that were in its own stack above it, plus all cows in stacks that beat it.
        // In this case, cow 1 lost to cow 2 in the first round, so cow 2 is above it. Then the stack (1,2) won the next round,
        // so no more cows above cow 1. Total above = 1.
        // If cow 4 is modified (second query), new skill 8. Cow 4 wins against cow 3, stack (3,4) with top 4.
        // Then stack (3,4) beats stack (1,2). So final stack: bottom 1,2, then 3,4 on top? Wait: "the second stack wins the fight, so the final stack has cows 1,2,3,4 in that order"
        // Second stack is (3,4). It jumps on top of first stack (1,2). So bottom 1,2, top 3,4. Cow 4 is the top, so 0 above. Answer 0.
        // So our logic: if the modified cow's stack loses at some level, the opponent stack is placed on top of it,
        // so all cows in the opponent stack are above the modified cow. Then the modified cow's stack becomes part of the opponent's stack,
        // but the modified cow is now below all those opponent cows. If later this combined stack wins, the new opponent goes below,
        // so no additional cows above. If it loses again, another opponent stack goes on top, adding more cows above.
        // So the total cows above = sum of sizes of all opponent stacks that beat the stack containing the modified cow,
        // plus the number of cows that were above the modified cow in its own stack at the time it lost?
        // Actually, when the modified cow is in a stack, there might be cows above it from previous wins.
        // But note: the modified cow starts at the bottom of its own stack (since it's a single cow).
        // When its stack wins, the opponent stack is placed below, so the modified cow remains at the same position
        // relative to the top? Wait: "The winning stack will jump on top of the losing stack".
        // So if the modified cow's stack wins, the losing stack goes below the entire winning stack.
        // So the modified cow's position from the top does not change; the new cows are added below.
        // If the modified cow's stack loses, the winning stack jumps on top of it, so all cows in the winning stack
        // are now above the modified cow. Then the modified cow's stack is now the combined stack, with the new top being the opponent's top.
        // So the cows above the modified cow increase by the size of the opponent stack.
        // Also, if the modified cow was not at the bottom of its stack (i.e., there were cows below it from previous wins),
        // those cows remain below it. But we only care about cows above.
        // So we can simulate by maintaining:
        // - current_xor: XOR sum of the stack containing the modified cow.
        // - current_winner: the index of the top cow of that stack (not needed for count, but needed to determine tie-breaker? Actually tie-breaker is based on the leftmost stack, which is determined by the original positions. The leftmost stack is the one that comes from the left child in the segment tree. So we need to know which child is left and which is right.)
        // - above: number of cows above the modified cow in the current stack.
        // - current_size: total number of cows in the current stack.
        // Initially: current_xor = new_skill, current_winner = b_i (0-indexed), above = 0, current_size = 1.
        // We walk up from the leaf to the root. At each step, we are at a segment tree node that represents a segment.
        // The current stack is the result of merging some children. We need to know the opponent stack.
        // The opponent stack is the other child of the current node in the segment tree.
        // We can get its precomputed XOR sum and winner (if it doesn't contain the modified cow).
        // But wait: the segment tree we built is for the original skills. If the opponent stack does not contain the modified cow,
        // its XOR sum and winner are exactly as precomputed. If it does contain the modified cow, then we are at a node
        // where both children are part of the modified cow's stack? That can't happen because the modified cow is a single leaf.
        // At each level, exactly one child contains the modified cow. The other child is a "pure" precomputed stack.
        // So we can use the precomputed values for the opponent.
        // We compare current_xor vs opponent_xor. Tie goes to the left stack.
        // We need to know which child is left. In the segment tree, left child is 2*i, right child is 2*i+1.
        // The leaf index of the modified cow is pos = size + b_i (0-indexed).
        // We can determine at each parent whether the current stack comes from the left or right child.
        // Let's do a bottom-up traversal:
        int pos = size + b_i - 1; // if b_i is 1-indexed
        int cur_xor = c_i;
        int cur_winner = b_i - 1;
        int above = 0;
        int cur_size = 1;
        // We'll go up until we reach the root (node 1)
        int node = pos;
        while (node > 1) {
            int parent = node / 2;
            int sibling = (node % 2 == 0) ? node + 1 : node - 1;
            // Determine if current stack is left or right child
            bool cur_is_left = (node % 2 == 0);
            // Get opponent's precomputed values
            int opp_xor = seg_xor[sibling];
            int opp_winner = seg_winner[sibling];
            int opp_size = 1 << (n - (int)log2(parent)); // Actually, the size of the segment at this level.
            // The level of the parent: root is level 0? Let's compute the size of the segment represented by parent.
            // The segment tree leaves are at depth n. The size of a node at depth d (root depth 0) is 2^(n-d).
            // We can compute the size of the sibling's segment by finding the number of leaves in its subtree.
            // Since the tree is perfect, the sibling's subtree size is the same as the current node's subtree size before merging.
            // At this point, cur_size is the size of the current stack (which is the result of merging some children below).
            // The sibling's size is the size of the segment it represents, which is exactly the number of leaves in that subtree.
            // We can precompute sizes or just compute it as the distance to leaf.
            // Actually, the size of the sibling's segment is 2^(level), where level is the distance from leaf to this node.
            // Let's compute the size of the sibling's segment: it's the number of leaves in that subtree.
            // Since the tree is perfect, the size is 1 << (__builtin_ctz(sibling)?) Better: we can just use the fact that
            // the total number of leaves in the parent's segment is 2 * cur_size? No, cur_size is the size of the current stack,
            // which is exactly the number of leaves in the current node's subtree. Because we started with size 1 and doubled
            // each time we won? Wait, cur_size is the size of the stack containing the modified cow. When we merge,
            // if we win, the opponent stack is added below, so cur_size increases by opp_size.
            // If we lose, the opponent stack is added above, so the new stack size is cur_size + opp_size, but the modified cow
            // is now in a larger stack, and its new size is that sum. However, we need opp_size for the opponent.
            // The opponent's size is exactly the number of leaves in the sibling's subtree, which is the same as the number
            // of leaves in the current node's subtree before this merge? Actually, at this level, both children have the same
            // number of leaves, which is exactly the size of the current stack before this merge? Not necessarily, because
            // the current stack might have already absorbed some stacks from lower levels. But the segment tree is built such
            // that each node represents a contiguous segment of the original array. The size of the segment is fixed.
            // The current stack's size is exactly the size of the segment represented by the current node (node).
            // Because we started at a leaf (size 1) and as we go up, we are combining with the sibling to form the parent segment.
            // So at node `node`, the segment size is the number of leaves in its subtree. That is exactly cur_size before we
            // merge with sibling? Actually, cur_size is the size of the stack after all merges below this node.
            // But the stack after merges below this node is exactly the result of the tournament within the segment of `node`.
            // That stack contains all cows from that segment, so its size is exactly the segment size.
            // So cur_size is the segment size of `node`. Similarly, the sibling's segment size is the same.
            // So opp_size = cur_size.
            opp_size = cur_size;
            // Now compare
            bool cur_wins;
            if (cur_xor > opp_xor) {
                cur_wins = true;
            } else if (cur_xor < opp_xor) {
                cur_wins = false;
            } else {
                // tie: leftmost wins. Leftmost is the left child.
                cur_wins = cur_is_left;
            }
            if (cur_wins) {
                // current stack wins, opponent goes below
                // cows above modified cow unchanged
                // new XOR = cur_xor ^ opp_xor
                cur_xor ^= opp_xor;
                // new size = cur_size + opp_size
                cur_size += opp_size;
                // winner remains cur_winner (top of current stack)
                // node becomes parent
            } else {
                // opponent wins, opponent jumps on top
                // all cows in opponent stack are now above the modified cow
                above += opp_size;
                // the new stack's XOR = cur_xor ^ opp_xor
                cur_xor ^= opp_xor;
                // the new top is the opponent's winner
                cur_winner = opp_winner;
                // new size = cur_size + opp_size
                cur_size += opp_size;
            }
            node = parent;
        }
        // After reaching root, the modified cow is somewhere in the final stack.
        // The number of cows above it is `above`.
        cout << above << '\n';
    }
    return 0;
}
