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

        // Precompute for each permutation its "inverse" mapping: value -> position
        vector<vector<int>> pos(n, vector<int>(m));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                pos[i][a[i][j]] = j;
            }
        }

        // For each permutation, compute the sequence of positions of 0,1,...,m-1
        // Then build a trie over these sequences.
        // Each node in trie corresponds to a prefix of positions.
        // We'll store the maximum depth (beauty) reachable from that node.
        // Since m <= 10, we can just use a map or array of size m for children.
        struct Node {
            array<int, 10> next; // m <= 10
            int max_depth;
            Node() {
                fill(next.begin(), next.end(), -1);
                max_depth = 0;
            }
        };
        vector<Node> trie(1);

        for (int i = 0; i < n; ++i) {
            int node = 0;
            for (int val = 0; val < m; ++val) {
                int p = pos[i][val];
                if (trie[node].next[p] == -1) {
                    trie[node].next[p] = trie.size();
                    trie.emplace_back();
                }
                node = trie[node].next[p];
                trie[node].max_depth = val + 1; // depth = number of matched elements
            }
        }

        // For each a_i, we want max beauty of a_i * a_j over j.
        // beauty = largest k such that (a_i * a_j)_x = x for x=1..k.
        // (a_i * a_j)_x = a_j[a_i[x-1]] (1-indexed). We want this = x.
        // So a_j[a_i[x-1]] = x, i.e., a_j[pos] = x where pos = a_i[x-1].
        // In 0-indexed: a_j[ a_i[x] ] = x+1? Let's derive carefully.
        // Let p = a_i, q = a_j. Product r = p * q means r_x = q_{p_x} (1-indexed).
        // We want r_1=1, r_2=2, ..., r_k=k.
        // So q_{p_1} = 1, q_{p_2} = 2, ..., q_{p_k} = k.
        // In 0-indexed: p_1 is a_i[0], etc. We want q[ a_i[0] ] = 0, q[ a_i[1] ] = 1, ..., q[ a_i[k-1] ] = k-1.
        // That means for x from 0 to k-1, the position of value x in q is exactly a_i[x].
        // So the sequence of positions of 0,1,...,k-1 in q must be exactly a_i[0], a_i[1], ..., a_i[k-1].
        // We have a trie of these position sequences for all given permutations q = a_j.
        // For a fixed a_i, we just walk the trie following the sequence a_i[0], a_i[1], ... as long as possible.
        // The max depth we can reach is the answer for i.

        vector<int> ans(n);
        for (int i = 0; i < n; ++i) {
            int node = 0;
            int beauty = 0;
            for (int k = 0; k < m; ++k) {
                int p = a[i][k];
                if (trie[node].next[p] != -1) {
                    node = trie[node].next[p];
                    beauty = trie[node].max_depth; // this is k+1
                } else {
                    break;
                }
            }
            ans[i] = beauty;
        }

        for (int i = 0; i < n; ++i) {
            cout << ans[i] << " \n"[i == n - 1];
        }
    }
    return 0;
}
