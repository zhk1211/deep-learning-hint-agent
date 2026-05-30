// Hint0
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

        vector<int> pos_in_a_i(m);
        vector<int> max_beauty(n, 0);
        vector<int> trie;
        trie.reserve(n * m + 1);
        trie.push_back(-1);
        vector<int> nxt;
        nxt.reserve(n * m + 1);
        vector<int> val;
        val.reserve(n * m + 1);
        nxt.push_back(0);
        val.push_back(0);

        auto add_to_trie = [&](const vector<int>& perm) {
            int node = 0;
            for (int x : perm) {
                if (nxt[node] == 0) {
                    nxt[node] = trie.size();
                    trie.push_back(-1);
                    nxt.push_back(0);
                    val.push_back(0);
                }
                node = nxt[node];
                if (trie[node] == -1) {
                    trie[node] = x;
                }
            }
            val[node] = max(val[node], m);
        };

        for (int i = 0; i < n; ++i) {
            vector<int> inv(m);
            for (int j = 0; j < m; ++j) {
                inv[a[i][j]] = j;
            }
            add_to_trie(inv);
        }

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                pos_in_a_i[a[i][j]] = j;
            }
            int node = 0;
            int beauty = 0;
            for (int k = 0; k < m; ++k) {
                int target = pos_in_a_i[k];
                if (nxt[node] == 0) break;
                node = nxt[node];
                if (trie[node] != target) break;
                beauty = k + 1;
            }
            max_beauty[i] = max(max_beauty[i], beauty);
        }

        for (int i = 0; i < n; ++i) {
            cout << max_beauty[i] << " \n"[i == n - 1];
        }
    }
    return 0;
}
