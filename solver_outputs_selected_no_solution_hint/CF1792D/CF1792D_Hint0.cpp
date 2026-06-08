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
            }
        }
        
        // Precompute position arrays for each permutation
        vector<vector<int>> pos(n, vector<int>(m + 1));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                pos[i][a[i][j]] = j + 1; // 1-indexed positions
            }
        }
        
        // For each permutation, compute the sequence of positions of 1..m
        // and store in a trie (or just a set of vectors)
        // Since m <= 10, we can use a set of vectors or a trie with arrays.
        // We'll build a trie where each node has children for values 1..m
        // representing the position of the next number.
        struct Node {
            array<int, 11> child; // 1..10, index 0 unused
            Node() {
                fill(child.begin(), child.end(), -1);
            }
        };
        vector<Node> trie(1);
        
        for (int i = 0; i < n; ++i) {
            int node = 0;
            for (int val = 1; val <= m; ++val) {
                int p = pos[i][val];
                if (trie[node].child[p] == -1) {
                    trie[node].child[p] = trie.size();
                    trie.emplace_back();
                }
                node = trie[node].child[p];
            }
        }
        
        // For each i, we want max beauty of a_i * a_j over j.
        // beauty = largest k such that (a_i * a_j)_1 = 1, ..., (a_i * a_j)_k = k.
        // (a_i * a_j)_x = a_j[a_i[x]].
        // So we need a_j[a_i[1]] = 1, a_j[a_i[2]] = 2, ..., a_j[a_i[k]] = k.
        // This means for each x from 1..k, position of x in a_j must be a_i[x].
        // So we need to find the longest prefix of the sequence (a_i[1], a_i[2], ..., a_i[m])
        // that matches the position sequence of some a_j.
        // We can query the trie with the sequence a_i[1], a_i[2], ...
        for (int i = 0; i < n; ++i) {
            int node = 0;
            int beauty = 0;
            for (int k = 1; k <= m; ++k) {
                int needed_pos = a[i][k - 1]; // because a_i is 0-indexed in our storage
                if (trie[node].child[needed_pos] != -1) {
                    node = trie[node].child[needed_pos];
                    beauty = k;
                } else {
                    break;
                }
            }
            cout << beauty << (i + 1 == n ? '\n' : ' ');
        }
    }
    return 0;
}
