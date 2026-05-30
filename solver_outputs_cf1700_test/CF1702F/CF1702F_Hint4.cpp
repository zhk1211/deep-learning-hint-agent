// Hint4
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

        multiset<int> ma;
        for (int x : a) {
            while (x % 2 == 0) x /= 2;
            ma.insert(x);
        }

        multiset<int> mb(b.begin(), b.end());

        bool ok = true;
        while (!mb.empty()) {
            int val = *mb.rbegin();
            mb.erase(prev(mb.end()));

            auto it = ma.find(val);
            if (it != ma.end()) {
                ma.erase(it);
            } else {
                if (val == 1) {
                    ok = false;
                    break;
                }
                mb.insert(val / 2);
            }
        }

        cout << (ok ? "YES" : "NO") << '\n';
    }

    return 0;
}
