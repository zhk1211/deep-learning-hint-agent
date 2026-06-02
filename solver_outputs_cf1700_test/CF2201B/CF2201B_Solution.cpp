#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, k;
    cin >> n >> k;
    if (k < n || k > 2 * n - 1) {
        cout << "NO\n";
        return;
    }
    cout << "YES\n";
    if (k == n) {
        for (int i = 1; i <= n; ++i) {
            cout << i << ' ' << i << (i == n ? '\n' : ' ');
        }
        return;
    }
    int x = k - n + 1;
    vector<int> res;
    if (x == 1) {
        for (int i = 1; i <= n; ++i) {
            res.push_back(i);
            res.push_back(i);
        }
    } else {
        // construct for (x, 2x-1)
        vector<int> part1;
        part1.push_back(1);
        for (int i = 2; i <= x; ++i) {
            part1.push_back(i);
        }
        part1.push_back(1);
        for (int i = 2; i <= x - 1; ++i) {
            part1.push_back(i);
        }
        part1.push_back(x);
        // construct for (n-x, n-x)
        int rem = n - x;
        for (int i = 1; i <= rem; ++i) {
            part1.push_back(x + i);
            part1.push_back(x + i);
        }
        res = part1;
    }
    for (int i = 0; i < 2 * n; ++i) {
        cout << res[i] << (i + 1 == 2 * n ? '\n' : ' ');
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
