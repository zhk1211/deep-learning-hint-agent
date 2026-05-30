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
        vector<pair<int, int>> ops;
        int x = n;
        while (x > 2) {
            // find y = ceil(sqrt(x))
            int y = 1;
            while (y * y < x) {
                ++y;
            }
            // reduce all numbers in (y, x) to 1 using x
            for (int z = y + 1; z < x; ++z) {
                ops.emplace_back(z, x);
            }
            // reduce x to 1 using y (takes exactly two operations)
            ops.emplace_back(x, y);
            ops.emplace_back(x, y);
            x = y;
        }
        cout << ops.size() << '\n';
        for (auto &p : ops) {
            cout << p.first << ' ' << p.second << '\n';
        }
    }
    return 0;
}
