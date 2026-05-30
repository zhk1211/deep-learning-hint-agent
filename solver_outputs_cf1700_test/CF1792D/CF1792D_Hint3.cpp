// Hint3
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n, m;
        cin >> n >> m;
        vector<vector<int>> a(n, vector<int>(m));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                cin >> a[i][j];
                --a[i][j]; // 0-indexed
            }
        }
        
        // For each permutation, compute its inverse
        vector<vector<int>> inv(n, vector<int>(m));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                inv[i][a[i][j]] = j;
            }
        }
        
        // Build a trie over the inverses
        // Each node stores children indices (0..m-1) and a flag if it's an end of some inverse
        vector<array<int, 10>> trie;
        vector<bool> is_end;
        trie.push_back({});
        is_end.push_back(false);
        
        for (int i = 0; i < n; ++i) {
            int node = 0;
            for (int j = 0; j < m; ++j) {
                int val = inv[i][j];
                if (trie[node][val] == 0) {
                    trie[node][val] = trie.size();
                    trie.push_back({});
                    is_end.push_back(false);
                }
                node = trie[node][val];
            }
            is_end[node] = true;
        }
        
        // For each a_i, we want max k such that there exists j with a_i * a_j having prefix 0..k-1
        // Equivalent: a_i * a_j has prefix 0..k-1 iff for all x in [0, k-1], (a_i * a_j)(x) = x
        // (a_i * a_j)(x) = a_j[a_i[x]] = x => a_j[a_i[x]] = x => a_i[x] = inv_j[x]
        // So we need inv_j[x] = a_i[x] for x = 0..k-1.
        // That means the inverse of a_j must match a_i on the first k positions.
        // So we just walk the trie with the sequence a_i[0], a_i[1], ... and find the longest prefix present.
        
        vector<int> ans(n);
        for (int i = 0; i < n; ++i) {
            int node = 0;
            int k = 0;
            for (int j = 0; j < m; ++j) {
                int val = a[i][j];
                if (trie[node][val] == 0) break;
                node = trie[node][val];
                ++k;
            }
            ans[i] = k;
        }
        
        for (int i = 0; i < n; ++i) {
            cout << ans[i] << " \n"[i == n - 1];
        }
    }
    return 0;
}
