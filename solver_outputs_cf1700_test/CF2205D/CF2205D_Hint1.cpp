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
        cin >> n;
        vector<int> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }
        
        vector<int> pos(n + 1);
        for (int i = 0; i < n; ++i) {
            pos[a[i]] = i;
        }
        
        int l = pos[1], r = pos[1];
        int ans = 0;
        for (int v = 2; v <= n; ++v) {
            int p = pos[v];
            if (p < l) {
                l = p;
            } else if (p > r) {
                r = p;
            } else {
                ans++;
            }
        }
        cout << ans << '\n';
    }
    return 0;
}
