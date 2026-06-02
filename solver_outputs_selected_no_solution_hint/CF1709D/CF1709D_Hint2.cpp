#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<int> a(m + 1);
    for (int i = 1; i <= m; ++i) {
        cin >> a[i];
    }

    int q;
    cin >> q;
    while (q--) {
        int xs, ys, xf, yf, k;
        cin >> xs >> ys >> xf >> yf >> k;

        // Check column alignment modulo k
        if (abs(ys - yf) % k != 0 || abs(xs - xf) % k != 0) {
            cout << "NO\n";
            continue;
        }

        // If same column, just check if we can move vertically without hitting blocked cells
        if (ys == yf) {
            cout << "YES\n";
            continue;
        }

        // Maximum row we can reach in the start column
        // We can go up to the highest multiple of k that is <= n
        // Actually we can go up to n - ((n - xs) % k) but we need to be above blocked cells in intermediate columns
        // The key is to reach a row that is > max blocked in columns between ys and yf
        int max_blocked = 0;
        int l = min(ys, yf), r = max(ys, yf);
        for (int col = l; col <= r; ++col) {
            max_blocked = max(max_blocked, a[col]);
        }

        // Highest row we can reach from start column
        int highest_reachable = xs + ((n - xs) / k) * k;
        if (highest_reachable <= max_blocked) {
            cout << "NO\n";
        } else {
            cout << "YES\n";
        }
    }

    return 0;
}
