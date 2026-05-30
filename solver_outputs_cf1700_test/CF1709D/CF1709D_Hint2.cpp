// Hint2
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

        // The robot can move freely if it can reach a row above all blocked cells in the columns it passes.
        // The highest blocked cell in the columns between ys and yf (inclusive) must be less than the maximum row
        // the robot can reach. The robot can reach up to n - ((n - xs) % k) from xs, but we need to ensure
        // it can go high enough to avoid obstacles.
        // Actually, the robot can move up as much as it wants, but it must stay within grid.
        // The maximum row it can reach from xs is: xs + k * t <= n  => max_row = n - ((n - xs) % k).
        // But it can also move down, so it can reach any row with same modulo as xs.
        // The critical part: to move horizontally between columns, the robot must be at a row > max_blocked in those columns.
        // Since it can move vertically first, it can go up to max_row, then move horizontally, then go down.
        // So we need max_blocked_in_range < max_row.
        // But wait: the robot can also move down first? No, moving down might hit blocked cells.
        // So the only safe way is to go up to a row that clears all obstacles in the path, then move horizontally, then go down.
        // So we need: max(a[i]) for i between ys and yf (inclusive) < max_row.
        // max_row = n - ((n - xs) % k). But also we must ensure that from that high row, we can go down to xf.
        // Since xf and xs have same modulo k, it's possible if max_row >= xf? Actually, we can go down from max_row to xf
        // as long as xf <= max_row and (max_row - xf) % k == 0. Since xs and xf have same modulo k, and max_row has same modulo as xs,
        // it's fine. But we also need to ensure we don't hit obstacles when going down in the final column? The final column's blocked cells
        // are up to a[yf], and xf > a[yf] by input, so it's safe.
        // So condition: max_blocked < max_row.
        // But we also need to consider that we might not need to go all the way up if the direct horizontal path is clear.
        // However, the robot moves in steps of k, so if xs > max_blocked, we can move horizontally directly? But we move k cells at a time,
        // so we might need to adjust vertically to align columns. Actually, we can move horizontally only if we are at a row > max_blocked.
        // If xs > max_blocked, we can move horizontally from xs? But we move k steps horizontally, so we might overshoot and need to come back.
        // Since we can move left/right, we can always adjust as long as we stay in the same row > max_blocked.
        // So if xs > max_blocked, we can just move horizontally at row xs. But wait: we need to change columns by a multiple of k.
        // If we move horizontally at row xs, we need to ensure that for every step, the cells we pass are unblocked.
        // Since we move k cells at a time, we might land in a column with blocked cells up to a_i. If xs <= a_i, we explode.
        // So we need xs > max_blocked for all intermediate columns.
        // If xs <= max_blocked, we must go up first. The maximum row we can reach is max_row = n - ((n - xs) % k).
        // So condition: max_blocked < max_row.
        // But what if we go up, but max_row is still <= max_blocked? Then impossible.
        // Also, we need to ensure that from max_row we can go down to xf. Since max_row >= xs and xf <= n, and modulo matches, it's possible.
        // However, there is a catch: when going up, we move k cells at a time. We start at xs. We can go up to max_row.
        // But what if there is a blocked cell in the start column above xs? The blocked cells are only at the bottom, so above a[ys] it's clear.
        // Since xs > a[ys], going up is safe.
        // So the condition is simply: max_blocked < max_row, where max_blocked = max(a[i]) for i in [min(ys, yf), max(ys, yf)].

        int l = min(ys, yf);
        int r = max(ys, yf);
        // We need range maximum query on array a. Since m up to 2e5 and q up to 2e5, we can use a segment tree or sparse table.
        // But we can also precompute a segment tree. Let's build a segment tree for range max.
        // However, we are reading queries online, we can build a segment tree before queries.
        // Since we don't have the segment tree built yet in this scope, we need to build it once.
        // But the code structure: we read n, m, then a, then q. We can build a segment tree after reading a.
        // Wait, we are inside main, we can build it before the query loop.
        // But the current code is inside the query loop without the tree. We need to restructure.
        // Let's move the query loop after building the segment tree.
    }

    return 0;
}
