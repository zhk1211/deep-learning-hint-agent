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
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
            while (a[i] % 2 == 0) a[i] /= 2;
        }
        for (int i = 0; i < n; ++i) {
            cin >> b[i];
        }

        multiset<int> ma(a.begin(), a.end());
        sort(b.begin(), b.end(), greater<int>());

        bool ok = true;
        for (int x : b) {
            while (x > 0) {
                if (ma.count(x)) {
                    ma.erase(ma.find(x));
                    break;
                }
                if (x == 1) {
                    ok = false;
                    break;
                }
                x /= 2;
            }
            if (!ok) break;
        }

        cout << (ok ? "YES" : "NO") << '\n';
    }
    return 0;
}
