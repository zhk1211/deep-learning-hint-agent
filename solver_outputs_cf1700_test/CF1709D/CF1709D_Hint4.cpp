// Hint4
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

        if (abs(ys - yf) % k != 0 || abs(xs - xf) % k != 0) {
            cout << "NO\n";
            continue;
        }

        // The highest row the robot can reach in columns between ys and yf
        // is limited by the blocked cells. The robot must stay strictly above a[i].
        // The robot moves in steps of k, so its row modulo k is fixed.
        // It can reach any row >= a[i]+1 that has the same remainder as xs mod k,
        // provided it doesn't exceed n.
        // The maximum possible row in column i with the correct remainder is:
        // Let r = xs % k. The largest row <= n with row % k == r is: n - ((n - r) % k + k) % k.
        // But we also need row > a[i]. So the max reachable row in column i is the largest
        // number <= n with remainder r that is > a[i]. If no such number exists, it's impossible.
        // Actually, we just need to check if there exists a path. The robot can move up/down
        // freely as long as it doesn't hit blocked cells. Since k divides the difference,
        // the robot can adjust its row by multiples of k. The only obstacle is the blocked cells.
        // The robot must be able to go from (xs, ys) to (xf, yf) without entering a blocked cell.
        // Since horizontal moves don't change row, the robot must be able to move vertically
        // in each column it passes through. The critical part is the maximum blocked cell
        // in the columns between ys and yf. The robot's row must be > max(a[i]) for all i between
        // min(ys, yf) and max(ys, yf). But the robot can also move up before moving horizontally.
        // So the robot can first move up to some row R (multiple of k steps from xs) such that
        // R > max(a[i]) for the columns in the range, then move horizontally, then move down to xf.
        // The condition is: there exists an integer t >= 0 such that xs + t*k <= n and
        // xs + t*k > max_{i in [min(ys, yf), max(ys, yf)]} a[i].
        // Then from that row, it can move horizontally (since row doesn't change) and then down to xf.
        // Since xf has the same remainder as xs mod k, and xf > a[yf], it's possible.
        // So we just need to check if the maximum blocked cell in the column range is less than
        // the maximum row reachable from xs with the same remainder.
        // The maximum row reachable from xs with remainder r = xs % k is:
        // max_row = n - ((n - r) % k + k) % k; but if xs > max_row? Actually xs itself is reachable.
        // The set of reachable rows is { xs + t*k | t integer, 1 <= xs + t*k <= n }.
        // The maximum in this set is: r + k * floor((n - r) / k). But careful: if xs > n? xs <= n by input.
        // So max_reachable = xs + k * ((n - xs) / k).
        // We need max_reachable > max_blocked.
        // But wait: the robot can also move down first? No, moving down might hit blocked cells.
        // So it must move up first if necessary.
        // So condition: max_reachable > max_blocked_in_range.
        // Also, the finish cell must be reachable: xf must be in the reachable set, which is true
        // because xf % k == xs % k and xf <= n and xf > a[yf] (given). So that's fine.

        int l = min(ys, yf);
        int r = max(ys, yf);
        // We need max a[i] for i in [l, r]. Since m up to 2e5, we can precompute a segment tree or sparse table.
        // But we didn't precompute! We need O(1) or O(log m) per query. q up to 2e5, so O(m) per query is too slow.
        // We must precompute a RMQ structure. Sparse table is O(m log m) build, O(1) query.
        // Let's build a sparse table for range maximum query on a[1..m].
        // But we are inside main, we can build it before queries.
        // Wait, we read m and a, then q. So we can build sparse table after reading a, before q.
        // Let's do that.
    }

    return 0;
}
