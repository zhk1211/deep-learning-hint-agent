// Hint0
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n, m;
        cin >> n >> m;
        vector<int> a(n), b(m);
        for (int i = 0; i < n; ++i) cin >> a[i];
        for (int i = 0; i < m; ++i) cin >> b[i];

        sort(b.begin(), b.end(), greater<int>());

        vector<int> c;
        c.reserve(n + m);
        int i = 0, j = 0;
        while (i < n && j < m) {
            if (a[i] >= b[j]) {
                c.push_back(a[i++]);
            } else {
                c.push_back(b[j++]);
            }
        }
        while (i < n) c.push_back(a[i++]);
        while (j < m) c.push_back(b[j++]);

        for (int k = 0; k < n + m; ++k) {
            cout << c[k] << " \n"[k == n + m - 1];
        }
    }
    return 0;
}
