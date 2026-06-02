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
        if ((ys - yf) % k != 0) {
            cout << "NO\n";
            continue;
        }

        // Check row alignment modulo k
        if ((xs - xf) % k != 0) {
            cout << "NO\n";
            continue;
        }

        // The highest row the robot can reach in the start column
        // It can move up to the top or until blocked
        // Actually, the robot can move up to any row > a[ys] that is congruent to xs mod k
        // The maximum reachable row in column ys is n - ((n - xs) % k)
        // But we need to consider the blocked cells: the robot cannot go below or at a[ys]
        // So the maximum row reachable in column ys is the largest row <= n with row % k == xs % k and row > a[ys]
        int max_row_start = xs + ((n - xs) / k) * k;
        // Similarly for finish column
        int max_row_finish = xf + ((n - xf) / k) * k;

        // The robot can move horizontally only at rows that are reachable from both columns
        // The horizontal move must be at a row that is > max(a[ys], a[yf]) and reachable from both
        // The reachable rows in column ys are those > a[ys] with row % k == xs % k
        // The reachable rows in column yf are those > a[yf] with row % k == xf % k
        // Since xs % k == xf % k (from the check above), the congruence condition is the same
        // So we need a row r such that r > max(a[ys], a[yf]) and r % k == xs % k and r <= min(max_row_start, max_row_finish)
        int max_blocked = max(a[ys], a[yf]);
        int min_max_row = min(max_row_start, max_row_finish);
        // Find the largest row <= min_max_row with the correct remainder
        int r = min_max_row - ((min_max_row - xs) % k + k) % k;
        if (r > max_blocked) {
            cout << "YES\n";
        } else {
            cout << "NO\n";
        }
    }

    return 0;
}
