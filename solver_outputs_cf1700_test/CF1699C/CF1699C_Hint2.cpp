// Hint2
#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    vector<int> pos(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
        pos[a[i]] = i;
    }
    
    long long ans = 1;
    int l = pos[0], r = pos[0];
    for (int mex = 1; mex < n; mex++) {
        int p = pos[mex];
        if (p < l) {
            l = p;
        } else if (p > r) {
            r = p;
        } else {
            // p is inside [l, r]
            int free_positions = (r - l + 1) - mex;
            ans = (ans * free_positions) % MOD;
        }
    }
    cout << ans << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}
