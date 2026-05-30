// Hint3
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
        int j = 0; // pointer to a block with size > 1 (or any block if none)
        
        for (int i = 0; i < m; ++i) {
            if (blocks[i] == 0) continue;
            // advance j to a block that can provide an extra deletion
            while (j < m && (j < i || blocks[j] <= 1)) {
                ++j;
            }
            if (j < m && blocks[j] > 1) {
                // use one character from block j
                --blocks[j];
                ++ans;
            } else {
                // no block with size > 1, we must skip the next block entirely
                ++ans;
                ++i; // skip the next block (which will be deleted by the prefix removal)
            }
        }
        
        cout << ans << '\n';
    }
    
    return 0;
}
