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
                --a[i][j];
            }
        }

        vector<vector<int>> pos(n, vector<int>(m));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                pos[i][a[i][j]] = j;
            }
        }

        vector<int> max_beauty(n, 0);
        vector<int> trie(1, -1);
        vector<int> next_node(1, -1);
        vector<int> beauty_val(1, 0);
        int node_cnt = 1;

        auto insert = [&](const vector<int>& p) {
            int cur = 0;
            for (int k = 0; k < m; ++k) {
                int val = p[k];
                bool found = false;
                for (int idx = trie[cur]; idx != -1; idx = next_node[idx]) {
                    if (beauty_val[idx] == val) {
                        cur = idx;
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    trie.push_back(-1);
                    next_node.push_back(-1);
                    beauty_val.push_back(val);
                    int new_node = node_cnt++;
                    next_node[new_node - 1] = trie[cur];
                    trie[cur] = new_node;
                    cur = new_node;
                }
            }
        };

        for (int i = 0; i < n; ++i) {
            vector<int> inv(m);
            for (int j = 0; j < m; ++j) {
                inv[a[i][j]] = j;
            }
            insert(inv);
        }

        for (int i = 0; i < n; ++i) {
            int cur = 0;
            int beauty = 0;
            for (int k = 0; k < m; ++k) {
                int val = a[i][k];
                bool found = false;
                for (int idx = trie[cur]; idx != -1; idx = next_node[idx]) {
                    if (beauty_val[idx] == val) {
                        cur = idx;
                        found = true;
                        break;
                    }
                }
                if (!found) break;
                ++beauty;
            }
            max_beauty[i] = beauty;
        }

        for (int i = 0; i < n; ++i) {
            cout << max_beauty[i] << " \n"[i == n - 1];
        }
    }
    return 0;
}
