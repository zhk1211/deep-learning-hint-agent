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

        vector<int> vals(n);
        for (int i = 0; i < n; ++i) {
            vals[i] = a[i] + i + 1;
        }

        sort(vals.begin(), vals.end(), greater<int>());

        vector<int> b;
        b.push_back(vals[0]);
        for (int i = 1; i < n; ++i) {
            int cur = vals[i];
            int prev = b.back();
            if (cur >= prev) {
                cur = prev - 1;
            }
            if (cur > 0) {
                b.push_back(cur);
            }
        }

        for (int i = 0; i < (int)b.size(); ++i) {
            if (i > 0) cout << ' ';
            cout << b[i];
        }
        cout << '\n';
    }

    return 0;
}
