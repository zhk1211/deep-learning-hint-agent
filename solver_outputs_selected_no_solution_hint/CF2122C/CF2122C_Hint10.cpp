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
        vector<tuple<int, int, int>> pts(n);
        for (int i = 0; i < n; ++i) {
            int x, y;
            cin >> x >> y;
            pts[i] = {x, y, i + 1};
        }
        sort(pts.begin(), pts.end(), [](const auto& a, const auto& b) {
            return get<0>(a) < get<0>(b);
        });
        for (int i = 0; i < n / 2; ++i) {
            cout << get<2>(pts[i]) << ' ' << get<2>(pts[n - 1 - i]) << '\n';
        }
    }
    return 0;
}
