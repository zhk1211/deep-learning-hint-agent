// Hint6
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
            if (s[i] == s[i-1]) {
                ++cnt;
            } else {
                blocks.push_back(cnt);
                cnt = 1;
            }
        }
        blocks.push_back(cnt);
        
        int m = blocks.size();
        int ans = 0;
        int j = 0; // pointer to a block with size > 1 that we can use to "skip" deletions
        
        for (int i = 0; i < m; ++i) {
            if (blocks[i] == 0) continue;
            ++ans;
            // advance j to a block with size > 1, starting from max(j, i)
            while (j < m && (j < i || blocks[j] <= 1)) {
                ++j;
            }
            if (j < m) {
                // use one character from block j to "delete" without reducing number of blocks
                --blocks[j];
            } else {
                // no block with size > 1, we have to skip the next block entirely
                ++i;
            }
        }
        cout << ans << '\n';
    }
    return 0;
}
