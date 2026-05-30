// Hint10
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<vector<int>> adj(n + 1);
        for (int i = 0; i < n - 1; i++) {
            int u, v;
            cin >> u >> v;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
        string s;
        cin >> s;
        s = " " + s; // 1-indexed
        
        vector<int> leaf(n + 1, 0);
        // root is 1, never leaf
        for (int i = 2; i <= n; i++) {
            if (adj[i].size() == 1) leaf[i] = 1;
        }
        
        int cnt0 = 0, cnt1 = 0, cntQ_leaf = 0;
        for (int i = 2; i <= n; i++) {
            if (leaf[i]) {
                if (s[i] == '0') cnt0++;
                else if (s[i] == '1') cnt1++;
                else cntQ_leaf++;
            }
        }
        
        int root_val = -1; // -1 unknown, 0 or 1
        if (s[1] == '0') root_val = 0;
        else if (s[1] == '1') root_val = 1;
        
        int ans = 0;
        if (root_val != -1) {
            // root is fixed
            if (root_val == 0) {
                // Iris wants leaves different from root -> 1
                // Dora wants leaves same as root -> 0
                // Iris goes first
                // Iris will pick a ? leaf and set to 1, Dora will set to 0
                // The number of leaves set to 1 will be ceil(cntQ_leaf / 2)
                int iris_gets = (cntQ_leaf + 1) / 2;
                ans = cnt1 + iris_gets;
            } else {
                // root_val == 1
                // Iris wants leaves 0
                int iris_gets = (cntQ_leaf + 1) / 2;
                ans = cnt0 + iris_gets;
            }
        } else {
            // root is ?
            // count unimportant nodes (non-root, non-leaf) that are ?
            int cntQ_unimportant = 0;
            for (int i = 2; i <= n; i++) {
                if (!leaf[i] && s[i] == '?') cntQ_unimportant++;
            }
            
            // If there is an odd number of ? in unimportant nodes, Dora will be forced to color the root
            // after all unimportant ? are filled (Iris starts, so if odd, Iris fills last unimportant, Dora has to move)
            // Actually: Iris and Dora alternate. If cntQ_unimportant is odd, Iris fills the last unimportant ?,
            // then Dora must move and will color the root (since it's the only ? left if no leaf ? left, but there are leaf ?).
            // Wait, the game: they can choose any ? vertex. The hints say: if odd number of ? in unimportant nodes,
            // Dora will have to colour the root (after filling the ? in unimportant nodes one by one) which will cause Iris to colour the leaves first.
            // Let's simulate optimal play.
            
            // If cntQ_unimportant % 2 == 1:
            // They will fill all unimportant ? first (optimal). Since Iris starts, Iris fills 1st, Dora 2nd, ..., Iris fills last (odd-th).
            // Then Dora's turn: only root and leaf ? remain. Dora must color root (to minimize score).
            // After Dora colors root, Iris gets to color leaves first.
            // If Dora sets root to 0, Iris wants leaves 1; if Dora sets root to 1, Iris wants leaves 0.
            // Dora will choose root value to minimize the final score.
            // Dora can see the leaf counts: cnt0, cnt1, cntQ_leaf.
            // If Dora sets root=0, then Iris will get ceil(cntQ_leaf/2) leaves of value 1, plus existing cnt1.
            // If Dora sets root=1, Iris gets ceil(cntQ_leaf/2) leaves of value 0, plus existing cnt0.
            // Dora will choose min of these two.
            
            // If cntQ_unimportant % 2 == 0:
            // They fill all unimportant ? (Iris starts, Dora fills last). Then Iris must color root.
            // Iris will choose root value to maximize score.
            // Iris will choose max of the two options.
            
            // But wait: what if there are no unimportant ? and cntQ_unimportant = 0? Then it's even.
            // Also, what if there are leaf ? but no unimportant ? ? The same logic.
            
            // However, there is a twist: if there are no leaf ? at all, then the game is just about root and maybe unimportant.
            // But the hints suggest this logic.
            
            // Let's compute the two possible outcomes based on who colors root.
            // Option A: root becomes 0, then Iris (if she moves first on leaves) gets ceil(cntQ_leaf/2) of 1s.
            // Option B: root becomes 1, then Iris gets ceil(cntQ_leaf/2) of 0s.
            
            auto calc = [&](int root_val, int first_on_leaves) -> int {
                // first_on_leaves: 0 for Iris, 1 for Dora? Actually, after root is set, the remaining ? are only leaf ?.
                // The player whose turn it is will start coloring leaf ?.
                // If Iris starts, she gets ceil(cntQ_leaf/2) of her desired value.
                // If Dora starts, Dora gets ceil(cntQ_leaf/2) of her desired value, so Iris gets floor(cntQ_leaf/2).
                // But we need to know who starts on leaf ?.
                // After root is colored, the turn passes to the other player.
                // So if Dora colored root, Iris starts on leaves.
                // If Iris colored root, Dora starts on leaves.
                // So:
                // If Iris starts on leaves: Iris gets ceil(cntQ_leaf/2) of her desired value.
                // If Dora starts on leaves: Iris gets floor(cntQ_leaf/2) of her desired value.
                // Desired value for Iris: if root=0 -> 1; if root=1 -> 0.
                // Existing leaves of desired value are already counted.
                // So:
                if (root_val == 0) {
                    int desired = 1;
                    int existing = cnt1;
                    if (first_on_leaves == 0) { // Iris starts
                        return existing + (cntQ_leaf + 1) / 2;
                    } else { // Dora starts
                        return existing + cntQ_leaf / 2;
                    }
                } else {
                    int desired = 0;
                    int existing = cnt0;
                    if (first_on_leaves == 0) {
                        return existing + (cntQ_leaf + 1) / 2;
                    } else {
                        return existing + cntQ_leaf / 2;
                    }
                }
            };
            
            if (cntQ_unimportant % 2 == 1) {
                // Dora will color root, then Iris starts on leaves.
                int score0 = calc(0, 0); // root=0, Iris starts
                int score1 = calc(1, 0); // root=1, Iris starts
                ans = min(score0, score1);
            } else {
                // Iris will color root, then Dora starts on leaves.
                int score0 = calc(0, 1); // root=0, Dora starts
                int score1 = calc(1, 1); // root=1, Dora starts
                ans = max(score0, score1);
            }
        }
        
        cout << ans << '\n';
    }
    
    return 0;
}
