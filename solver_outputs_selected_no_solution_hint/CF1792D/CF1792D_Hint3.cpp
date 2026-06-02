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

        vector<vector<int>> trie(1, vector<int>(m, -1));
        vector<int> best(1, 0);
        for (int i = 0; i < n; ++i) {
            int node = 0;
            for (int val = 0; val < m; ++val) {
                int p = pos[i][val];
                if (trie[node][p] == -1) {
                    trie[node][p] = trie.size();
                    trie.emplace_back(m, -1);
                    best.push_back(val + 1);
                }
                node = trie[node][p];
            }
        }

        for (int i = 0; i < n; ++i) {
            int node = 0;
            int ans = 0;
            for (int val = 0; val < m; ++val) {
                int p = a[i][val];
                if (trie[node][p] == -1) break;
                node = trie[node][p];
                ans = best[node];
            }
            cout << ans << " \n"[i == n - 1];
        }
    }
    return 0;
}
