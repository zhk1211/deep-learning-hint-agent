// Hint2
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    string s;
    cin >> n >> s;
    int ans = 0;
    for (int i = 0; i < n; ++i) {
        int bal = 0;
        for (int j = i; j < n; ++j) {
            bal += (s[j] == '+' ? 1 : -1);
            if (bal <= 0 && bal % 3 == 0) {
                ++ans;
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
