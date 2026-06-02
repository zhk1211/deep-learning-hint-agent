#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    string s;
    cin >> n >> s;
    
    int ans = 0;
    for (int i = 0; i < n; ++i) {
        int bal = 0, cnt = 0;
        for (int j = i; j < n; ++j) {
            if (s[j] == '+') {
                bal++;
            } else {
                cnt++;
                bal--;
            }
            if (cnt >= 2 && bal < 0) {
                int need = (-bal + 1) / 2;
                if (need * 2 <= cnt) {
                    ans++;
                }
            } else if (bal >= 0 && (cnt % 2 == 0)) {
                ans++;
            }
        }
    }
    cout << ans << '\n';
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
