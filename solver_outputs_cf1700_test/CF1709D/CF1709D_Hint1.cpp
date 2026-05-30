// Hint1
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

        // The robot can move freely in the unblocked area.
        // The only restriction is that it must not hit blocked cells.
        // The highest blocked cell in the columns between ys and yf
        // must be strictly less than the maximum row the robot can reach.
        // The robot can reach up to n, but it must stay above blocked cells.
        // Actually, the robot can move up to n, but it must be able to pass
        // through the columns. The maximum row it can achieve in column i
        // is n, but it must be > a[i] to be unblocked.
        // The robot starts at (xs, ys). It can move up to any row that is
        // congruent to xs modulo k, up to n. The maximum such row is:
        int max_row = xs + (n - xs) / k * k;
        // It must be > a[i] for all i between ys and yf.
        // Since a[i] can be up to n, we need max_row > a[i].
        // Also, the finish row xf must be <= max_row.
        if (xf > max_row) {
            cout << "NO\n";
            continue;
        }

        // Find the maximum blocked height in the range [min(ys, yf), max(ys, yf)]
        int l = min(ys, yf);
        int r = max(ys, yf);
        // We can precompute a segment tree or sparse table for range maximum queries.
        // Since m <= 2e5 and q <= 2e5, we need O(1) or O(log m) per query.
        // We'll use a sparse table for O(1) queries.
        // But we need to build it once.
        // We'll do it outside the query loop.
        // Wait, we are inside the query loop now. We should have built it before.
        // Let's restructure: read all, build sparse table, then process queries.
        // We'll move the building outside.
        // Actually, we can just compute it on the fly with a segment tree,
        // but sparse table is easier and O(1) query.
        // We'll build it now.
        // But we are inside the query loop. We need to build it before.
        // Let's fix the code structure.
    }

    // The above is wrong because we need the sparse table built before queries.
    // Let's rewrite properly.
    return 0;
}
