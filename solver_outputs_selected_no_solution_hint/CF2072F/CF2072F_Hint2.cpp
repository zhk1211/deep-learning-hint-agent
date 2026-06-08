#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n;
        unsigned k;
        cin >> n >> k;
        vector<unsigned> ans(n);
        for (int i = 0; i < n; ++i) {
            // Check if (n-1 choose i) is odd
            if (((n - 1) & i) == i) {
                ans[i] = k;
            } else {
                ans[i] = 0;
            }
        }
        for (int i = 0; i < n; ++i) {
            cout << ans[i] << " \n"[i == n - 1];
        }
    }
    return 0;
}
