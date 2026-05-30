// Hint1
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
                --a[i][j]; // 0-index
            }
        }

        // For each permutation, compute the inverse
        vector<vector<int>> inv(n, vector<int>(m));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                inv[i][a[i][j]] = j;
            }
        }

        // Build a trie over all possible sequences of the form:
        // inv[j][0], inv[j][1], ..., inv[j][k-1] for all j and k
        // Actually we store the longest prefix of 0,1,2,... that can be matched
        // by some a_j.
        // We'll insert for each j the sequence inv[j][0], inv[j][1], ... 
        // but we only care about prefixes that are consecutive from 0.
        // So we insert the whole sequence of length m, but we will query
        // for each i the longest prefix of a_i that matches some inv[j].
        
        // Trie node
        struct Node {
            array<int, 10> next; // m <= 10
            Node() {
                fill(next.begin(), next.end(), -1);
            }
        };
        vector<Node> trie(1);
        
        for (int j = 0; j < n; ++j) {
            int node = 0;
            for (int k = 0; k < m; ++k) {
                int val = inv[j][k];
                if (trie[node].next[val] == -1) {
                    trie[node].next[val] = trie.size();
                    trie.emplace_back();
                }
                node = trie[node].next[val];
            }
        }
        
        // For each i, we want max beauty = max k such that 
        // a_i[0]=0, a_i[1]=1, ..., a_i[k-1]=k-1 and there exists j with
        // inv[j][0]=a_i[0], inv[j][1]=a_i[1], ..., inv[j][k-1]=a_i[k-1].
        // So we traverse the trie with a_i[0], a_i[1], ... and find the depth.
        for (int i = 0; i < n; ++i) {
            int node = 0;
            int beauty = 0;
            for (int k = 0; k < m; ++k) {
                if (a[i][k] != k) break; // not consecutive from 0
                int val = a[i][k];
                if (trie[node].next[val] != -1) {
                    node = trie[node].next[val];
                    beauty = k + 1;
                } else {
                    break;
                }
            }
            cout << beauty << (i + 1 == n ? '\n' : ' ');
        }
    }
    return 0;
}
