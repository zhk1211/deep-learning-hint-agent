// Hint1
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    string s;
    cin >> n >> s;
    vector<int> L(n + 1, 0), R(n + 1, 0);
    
    for (int i = 0; i < n; ++i) {
        if (s[i] == 'L') {
            L[i + 1] = R[i] + 1;
        } else {
            R[i + 1] = L[i] + 1;
        }
    }
    
    vector<int> revL(n + 1, 0), revR(n + 1, 0);
    for (int i = n - 1; i >= 0; --i) {
        if (s[i] == 'L') {
            revR[i] = revL[i + 1] + 1;
        } else {
            revL[i] = revR[i + 1] + 1;
        }
    }
    
    for (int i = 0; i <= n; ++i) {
        int ans = 1 + L[i] + revR[i];
        cout << ans << " \n"[i == n];
    }
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
