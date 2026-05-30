#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        int a, b, c;
        cin >> a >> b >> c;
        if (c != a + 1) {
            cout << -1 << '\n';
            continue;
        }
        int cur = 1, next = 0, height = 0;
        while (a > 0 || b > 0) {
            for (int i = 0; i < cur; ++i) {
                if (a > 0) {
                    --a;
                    next += 2;
                } else if (b > 0) {
                    --b;
                    next += 1;
                }
            }
            cur = next;
            next = 0;
            ++height;
        }
        cout << height << '\n';
    }
    return 0;
}
