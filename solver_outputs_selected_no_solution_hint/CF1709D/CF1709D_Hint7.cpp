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
        if (abs(ys - yf) % k != 0) {
            cout << "NO\n";
            continue;
        }

        // Check row alignment modulo k
        if (abs(xs - xf) % k != 0) {
            cout << "NO\n";
            continue;
        }

        // The robot can move up to the highest possible row in columns between ys and yf.
        // It can reach row x if x <= n - max_blocked_in_range.
        // The maximum row it can reach is n - max_a, but it must also be able to step up by k.
        // Actually, the robot can move up to any row that is congruent to xs mod k and <= n - max_a.
        // The highest reachable row is xs + k * floor((n - max_a - xs) / k) if xs <= n - max_a.
        // But we need to check if xf is reachable: xf must be <= n - max_a and xf % k == xs % k.
        // Also, the robot must not explode when moving horizontally: it must stay above blocked cells.
        // The horizontal move happens at some row r. The robot can choose r as any row that is
        // reachable and >= max_blocked_in_columns + 1.
        // Since we can move vertically first, we can go up to the highest possible row before moving horizontally.
        // The highest row we can reach is: let max_a = max_{i between ys and yf} a[i].
        // Then the highest row is n - max_a, but we must also respect the step size k.
        // The maximum row reachable from xs with step k and not exceeding n - max_a is:
        // highest = xs + ((n - max_a - xs) / k) * k, provided xs <= n - max_a.
        // If xs > n - max_a, we cannot even start moving horizontally without hitting a blocked cell.
        
        int max_a = 0;
        if (ys <= yf) {
            for (int i = ys; i <= yf; ++i) max_a = max(max_a, a[i]);
        } else {
            for (int i = yf; i <= ys; ++i) max_a = max(max_a, a[i]);
        }

        if (xs <= n - max_a) {
            int highest = xs + ((n - max_a - xs) / k) * k;
            if (xf <= highest) {
                cout << "YES\n";
                continue;
            }
        }
        cout << "NO\n";
    }

    return 0;
}
