// Hint0
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

        vector<int> peaks;
        for (int i = 1; i + 1 < n; ++i) {
            if (a[i] > a[i - 1] && a[i] > a[i + 1]) {
                peaks.push_back(i);
            }
        }

        int ans = 0;
        int m = peaks.size();
        for (int i = 0; i < m; ) {
            int j = i;
            while (j + 1 < m && peaks[j + 1] == peaks[j] + 2) {
                ++j;
            }
            int len = j - i + 1;
            ans += (len + 1) / 2;
            i = j + 1;
        }

        cout << ans << '\n';
    }

    return 0;
}
