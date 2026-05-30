// Hint1
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

        multiset<int> ma(a.begin(), a.end());
        multiset<int> mb(b.begin(), b.end());

        while (!ma.empty()) {
            int x = *ma.rbegin();
            int y = *mb.rbegin();

            if (x == y) {
                ma.erase(prev(ma.end()));
                mb.erase(prev(mb.end()));
            } else if (x > y) {
                if (x % 2 == 0) {
                    int nx = x / 2;
                    ma.erase(prev(ma.end()));
                    ma.insert(nx);
                } else {
                    break;
                }
            } else {
                if (y % 2 == 0) {
                    int ny = y / 2;
                    mb.erase(prev(mb.end()));
                    mb.insert(ny);
                } else {
                    int ny = y / 2;
                    mb.erase(prev(mb.end()));
                    mb.insert(ny);
                }
            }
        }

        cout << (ma.empty() ? "YES" : "NO") << '\n';
    }
    return 0;
}
