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

    // Sparse table for range maximum queries on a[1..m]
    int LOG = 0;
    while ((1 << LOG) <= m) ++LOG;
    vector<vector<int>> st(LOG, vector<int>(m + 1));
    for (int i = 1; i <= m; ++i) {
        st[0][i] = a[i];
    }
    for (int j = 1; j < LOG; ++j) {
        for (int i = 1; i + (1 << j) - 1 <= m; ++i) {
            st[j][i] = max(st[j-1][i], st[j-1][i + (1 << (j-1))]);
        }
    }
    auto range_max = [&](int l, int r) {
        if (l > r) swap(l, r);
        int len = r - l + 1;
        int j = 31 - __builtin_clz(len);
        return max(st[j][l], st[j][r - (1 << j) + 1]);
    };

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

        // The highest row the robot can reach in columns between ys and yf
        // It can go up to the maximum possible row that is not blocked.
        // The robot starts at xs, and can move vertically in steps of k.
        // The maximum reachable row in any column is n - (n - xs) % k? Actually,
        // from xs, the robot can reach rows: xs, xs+k, xs+2k, ... up to n.
        // But it must avoid blocked cells. The blocked cells are rows 1..a_i.
        // So the robot can only be in rows > a_i.
        // The highest row it can be in column i is: the largest r <= n such that r > a_i and r ≡ xs (mod k).
        // That is: let max_row = n - ((n - xs) % k + k) % k? Actually simpler:
        // The reachable rows are xs + t*k for integer t >= 0, up to n.
        // The maximum reachable row in column i is the largest such row that is > a_i.
        // If xs <= a_i, then the robot cannot even be in that column? But start cell is unblocked, so xs > a_ys.
        // For other columns, we need to check if there exists a reachable row > a_i.
        // The condition for column i: the largest reachable row <= n must be > a_i.
        // Largest reachable row overall: R_max = xs + ((n - xs) / k) * k.
        // For column i, if R_max <= a_i, then no reachable row exists (since all reachable rows are <= R_max and if R_max <= a_i, all are <= a_i, so blocked).
        // So we need R_max > a_i for all columns between ys and yf.
        // But wait: the robot can also move down? It can move down, so reachable rows also include xs - t*k for t>0, down to 1.
        // However, moving down into blocked cells is not allowed. But if it moves down, it might go into blocked cells.
        // Actually, the robot can move up and down. So reachable rows are all rows r such that r ≡ xs (mod k) and 1 <= r <= n.
        // But it must not enter blocked cells. So for a column i, the robot can be in any row r with r ≡ xs (mod k) and r > a_i.
        // The maximum such row is the largest r <= n with r ≡ xs (mod k) and r > a_i.
        // If no such r exists, the robot cannot enter column i at all.
        // For the robot to move from column ys to yf, it must be able to pass through all intermediate columns.
        // It can change rows while moving horizontally? No, horizontal moves are left/right, which keep the same row.
        // So the robot must find a row r such that:
        // - r ≡ xs (mod k) (since vertical moves change row by multiples of k)
        // - r > a_i for all i between ys and yf inclusive (so it can move horizontally without hitting blocked cells)
        // - r is reachable from xs (which is automatic if r ≡ xs mod k and 1 <= r <= n)
        // - r is reachable from xf? Actually, the robot ends at xf. So we also need r ≡ xf (mod k) which is already true because xs ≡ xf (mod k) from earlier check.
        // So we just need to find if there exists a row r with r ≡ xs (mod k), r > max_{i in [ys, yf]} a_i, and r <= n.
        // The maximum possible such r is R_max = xs + ((n - xs) / k) * k.
        // If R_max > max_a, then we can choose r = R_max (or any other reachable row > max_a).
        // But wait: is it always possible to reach that row r from xs and then go to xf?
        // From xs, we can go up to r (since r >= xs and r ≡ xs mod k). Then move horizontally to the target column at row r.
        // Then from row r go vertically to xf. Since r ≡ xf (mod k), we can move down/up to xf as long as we don't hit blocked cells.
        // But when moving vertically in the target column, we must ensure that all intermediate rows between r and xf are unblocked in that column.
        // However, the target column has blocked cells only at rows 1..a_yf. Since xf > a_yf, and r > max_a >= a_yf, the path from r down to xf (if r > xf) or up to xf (if r < xf) stays in rows > a_yf? Not necessarily: if r > xf, we go down through rows r, r-k, r-2k, ..., xf. All these rows are >= xf > a_yf, so they are unblocked. If r < xf, we go up through rows r, r+k, ..., xf, all > a_yf. So it's safe.
        // But what about the start column? We start at xs, go up to r. The path goes through rows xs, xs+k, ..., r. All these are > a_ys? Since xs > a_ys and r >= xs, all intermediate rows are >= xs > a_ys, so safe.
        // So the only obstacle is the horizontal movement: we need a row r that is unblocked in all columns between ys and yf.
        // Thus the condition is: max_a = max_{i in [ys, yf]} a_i < R_max, where R_max is the largest row <= n with R_max ≡ xs (mod k).
        // But wait: what if R_max <= max_a? Could we use a smaller reachable row? The largest reachable row is R_max. If R_max <= max_a, then all reachable rows are <= max_a, so none are > max_a. So impossible.
        // So condition: R_max > max_a.
        // However, there is a catch: the robot can also move down initially? It starts at xs. If xs is already > max_a, we could just move horizontally at row xs without any vertical movement. But xs might not be ≡ xf (mod k)? We already checked xs ≡ xf (mod k). So if xs > max_a, we can just go horizontally at row xs. But wait: xs might not be reachable from xf? Actually, if xs > max_a, we can go horizontally to yf at row xs, then vertically to xf. But vertical movement from xs to xf requires that all intermediate rows in column yf are unblocked. Since xs > max_a >= a_yf, and xf > a_yf, the path is safe. So if xs > max_a, it's always possible? But we also need xs ≡ xf (mod k), which we have. So if xs > max_a, answer is YES.
        // But our condition R_max > max_a covers this: if xs > max_a, then R_max >= xs > max_a, so condition holds.
        // What if xs <= max_a? Then we must move up to a row > max_a. The smallest such row is the smallest r > max_a with r ≡ xs (mod k). That r must be <= n. The largest such row is R_max. If R_max > max_a, then such an r exists. So condition is exactly R_max > max_a.
        // But wait: is it possible that we need to move down first? Moving down would go to rows < xs, which are even smaller, so they would be <= max_a if xs <= max_a. So no benefit.
        // So the condition seems correct.

        int max_a = range_max(ys, yf);
        // Compute R_max: largest r <= n with r ≡ xs (mod k)
        // r = xs + t*k <= n  => t = (n - xs) / k
        // R_max = xs + ((n - xs) / k) * k
        int R_max = xs + ((n - xs) / k) * k;
        if (R_max > max_a) {
            cout << "YES\n";
        } else {
            cout << "NO\n";
        }
    }

    return 0;
}
