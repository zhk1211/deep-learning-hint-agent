// Hint2
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

        // Find leaves (excluding root 1)
        vector<int> leaves;
        for (int i = 2; i <= n; i++) {
            if (adj[i].size() == 1) {
                leaves.push_back(i);
            }
        }

        // Count question marks
        int total_q = 0;
        int leaf_q = 0;
        int nonleaf_q = 0;
        for (int i = 1; i <= n; i++) {
            if (s[i] == '?') {
                total_q++;
                if (adj[i].size() == 1 && i != 1) {
                    leaf_q++;
                } else {
                    nonleaf_q++;
                }
            }
        }

        // If root is fixed
        if (s[1] != '?') {
            // Count fixed leaves with non-zero weight
            int fixed_nonzero = 0;
            for (int leaf : leaves) {
                if (s[leaf] != '?') {
                    if (s[leaf] != s[1]) {
                        fixed_nonzero++;
                    }
                }
            }
            // The game: Iris wants to maximize, Dora minimize
            // Each leaf ? can be set to match root (weight 0) or opposite (non-zero)
            // Iris will try to make them opposite, Dora will try to match
            // Since Iris goes first, she can secure ceil(leaf_q/2) of them as opposite
            int iris_gets = (leaf_q + 1) / 2;
            cout << fixed_nonzero + iris_gets << "\n";
        } else {
            // Root is ?
            // First move: Iris will set root to something
            // After that, the game reduces to the fixed root case with total_q - 1 remaining
            // But Iris can choose root value to maximize the final score
            // Let's compute for both possible root values
            auto calc = [&](char root_val) -> int {
                int fixed_nonzero = 0;
                for (int leaf : leaves) {
                    if (s[leaf] != '?') {
                        if (s[leaf] != root_val) {
                            fixed_nonzero++;
                        }
                    }
                }
                // Remaining question marks: total_q - 1 (root is set)
                // Among them, leaf_q are leaf question marks
                // Iris will get ceil(leaf_q/2) of them as opposite
                int iris_gets = (leaf_q + 1) / 2;
                return fixed_nonzero + iris_gets;
            };
            int ans0 = calc('0');
            int ans1 = calc('1');
            // Iris chooses the maximum
            cout << max(ans0, ans1) << "\n";
        }
    }
    return 0;
}
