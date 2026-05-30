// Hint1
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n;
        string s;
        cin >> n >> s;

        vector<int> blocks;
        int cnt = 1;
        for (int i = 1; i < n; ++i) {
            if (s[i] == s[i - 1]) {
                ++cnt;
            } else {
                blocks.push_back(cnt);
                cnt = 1;
            }
        }
        blocks.push_back(cnt);

        int m = blocks.size();
        int ops = 0;
        int j = 0; // pointer to a block with size > 1 (or any block if none)
        for (int i = 0; i < m; ++i) {
            if (blocks[i] == 0) continue;
            // find next block with size > 1 to steal from
            while (j < m && (j < i || blocks[j] <= 1)) {
                ++j;
            }
            if (j < m) {
                // steal one from block j
                --blocks[j];
                ++ops;
            } else {
                // no block with size > 1, must delete two blocks (current and next)
                ++ops;
                ++i; // skip next block
            }
        }
        cout << ops << '\n';
    }
    return 0;
}
