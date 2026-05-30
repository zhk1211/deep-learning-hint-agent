// Hint11
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<pair<int, int>> ops;
    int cur = n;
    while (cur > 2) {
        int sq = (int)sqrt(cur);
        if (sq * sq < cur) sq++;
        for (int i = sq + 1; i < cur; i++) {
            ops.push_back({i, cur});
        }
        ops.push_back({cur, sq});
        ops.push_back({cur, sq});
        cur = sq;
    }
    cout << ops.size() << "\n";
    for (auto [x, y] : ops) {
        cout << x << " " << y << "\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}
