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
        vector<int> a(n), b(n);
        for (int i = 0; i < n; ++i) cin >> a[i];
        for (int i = 0; i < n; ++i) cin >> b[i];

        int i = n - 1, j = n - 1;
        multiset<int> reserve;
        bool ok = true;

        while (i >= 0) {
            while (j > 0 && b[j] == b[j - 1]) {
                reserve.insert(b[j]);
                --j;
            }
            if (j >= 0 && a[i] == b[j]) {
                --i;
                --j;
            } else {
                auto it = reserve.find(a[i]);
                if (it != reserve.end()) {
                    reserve.erase(it);
                    --i;
                } else {
                    ok = false;
                    break;
                }
            }
        }

        cout << (ok ? "YES" : "NO") << '\n';
    }
    return 0;
}
