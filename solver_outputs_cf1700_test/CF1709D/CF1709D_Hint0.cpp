// Hint0
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

        // The robot can move up to the highest possible row in columns between ys and yf
        // The highest reachable row is limited by the blocked cells in those columns.
        // The robot can step up to n, but must avoid blocked cells.
        // Since moves are in steps of k, the robot can only be at rows congruent to xs mod k.
        // The maximum row it can reach in column y is: n - ((n - (a[y] + 1)) % k) but careful.
        // Actually, from xs, the robot can go up to the largest row <= n that is ≡ xs (mod k).
        // But it must also be > a[y] for all y in the path.
        // So the maximum safe row in column y is the largest r ≤ n such that r ≡ xs (mod k) and r > a[y].
        // If no such r exists, then column y is impassable.
        // We need to check if there exists a column y between ys and yf where the max safe row < xs,
        // or if the max safe row in the start or finish column is less than the respective start/finish row.
        // Actually, we just need to ensure that for all columns between min(ys,yf) and max(ys,yf),
        // the maximum reachable row is at least max(xs, xf) (since we can go up and down along the way).
        // But we also need to be able to reach xf in column yf.
        // Since we can move vertically only in steps of k, the reachable rows in any column are those ≡ xs (mod k).
        // So the condition is: for all y in [min(ys, yf), max(ys, yf)], the largest r ≤ n with r ≡ xs (mod k) and r > a[y] must be ≥ max(xs, xf).
        // However, we can also move horizontally at any reachable row. So we just need that there is some row R ≥ max(xs, xf) that is reachable in all columns between.
        // That is equivalent to: the minimum over y in [min(ys, yf), max(ys, yf)] of (max reachable row in column y) ≥ max(xs, xf).
        // Compute max reachable row in column y: let r = n - ((n - xs) % k + k) % k? Actually, the largest ≤ n congruent to xs mod k is: n - ((n - xs) % k + k) % k.
        // But we need it to be > a[y]. So if that largest ≤ n is ≤ a[y], then no reachable row exists in that column (or the max reachable is < xs, which fails).
        // So we can precompute for each column the max reachable row given a starting row xs mod k. But xs varies per query.
        // Notice that the condition only depends on xs mod k. Let rem = xs % k.
        // For a column y, the largest row ≤ n with row % k == rem is: n - ((n - rem) % k + k) % k. Actually simpler: ((n - rem) / k) * k + rem, but careful with negative.
        // Let max_row = ((n - rem) / k) * k + rem. If max_row > n, subtract k. But since rem < k, it's fine.
        // If max_row <= a[y], then no reachable row > a[y] exists. Otherwise, the max reachable row > a[y] is max_row.
        // We need min over y in range of max_row > a[y] ? max_row : -inf.
        // But we also need to consider that we can start at xs and end at xf. So we need max(xs, xf) ≤ that min max_row.
        // Since xs and xf are given and are > a[ys], a[yf] respectively, they are valid.
        // So for each query, we need to check if for all y in [min(ys, yf), max(ys, yf)], the condition max_row(y) >= max(xs, xf) holds.
        // max_row(y) depends on rem = xs % k. So we can precompute for each possible rem? k up to 1e9, so no.
        // But we can answer queries offline? Or we can use a segment tree that stores a[y] and we query the maximum a[y] in the range.
        // Because max_row(y) >= max(xs, xf) is equivalent to: the largest row ≡ rem (mod k) that is > a[y] is at least max(xs, xf).
        // This is equivalent to: a[y] < max(xs, xf) and also the step condition? Actually, if a[y] >= max(xs, xf), then even if max_row(y) >= max(xs, xf), it might be that max_row(y) is the only reachable row but it's ≤ a[y]? No, max_row(y) is the largest reachable row. If a[y] >= max(xs, xf), then any reachable row > a[y] must be > a[y] >= max(xs, xf). So if there is any reachable row > a[y], it will be ≥ max(xs, xf) automatically because the next reachable row after a[y] is at least a[y]+1, but we need it to be ≡ rem mod k. It could be that the smallest reachable row > a[y] is > max(xs, xf). That's fine. The problem is if there is NO reachable row > a[y] at all. That happens when max_row(y) ≤ a[y].
        // So condition: for all y in range, we need max_row(y) > a[y] AND max_row(y) >= max(xs, xf)? Actually, if max_row(y) > a[y], then the maximum reachable row is max_row(y). We need that max_row(y) >= max(xs, xf). But if max_row(y) > a[y] and max_row(y) < max(xs, xf), then we cannot go high enough. So we need max_row(y) >= max(xs, xf).
        // However, if a[y] is very high, max_row(y) might be ≤ a[y], meaning no reachable row exists. So we need max_row(y) > a[y].
        // So condition: max_row(y) > a[y] and max_row(y) >= max(xs, xf).
        // But max_row(y) is the largest row ≡ rem mod k. It is either > a[y] or ≤ a[y]. If it's > a[y], then it's the maximum reachable. If it's ≤ a[y], then there is no reachable row > a[y] because any row > a[y] would be at least a[y]+1, and the next congruent row is max_row(y) + k, which is > n (since max_row(y) is the largest ≤ n). So indeed, if max_row(y) ≤ a[y], column y is impassable.
        // So we need: max_row(y) > a[y] and max_row(y) >= max(xs, xf).
        // max_row(y) = n - ((n - rem) % k). Actually, n - ((n - rem) % k) gives the largest ≤ n with that remainder? Let's test: n=11, rem=1, k=3. n - rem = 10, 10 % 3 = 1, n - 1 = 10. 10 % 3 = 1, yes. If n=11, rem=2, k=3: n-rem=9, 9%3=0, n-0=11, 11%3=2, yes. If n=11, rem=0, k=3: n-rem=11, 11%3=2, n-2=9, 9%3=0, yes. So max_row = n - ((n - rem) % k). But if ((n - rem) % k) == k? No, modulo k gives 0..k-1. So it's fine.
        // So max_row(y) depends only on rem and k, not on y. It's a constant for the query! Let M = n - ((n - rem) % k). This is the global maximum reachable row in any column (if unblocked).
        // Then for column y, the maximum reachable row is M if M > a[y], else there is no reachable row (or the maximum reachable is ≤ a[y]).
        // So the condition simplifies: For all y in [min(ys, yf), max(ys, yf)], we need M > a[y] and M >= max(xs, xf).
        // But wait: what if M <= a[y]? Then no reachable row exists in that column. So we need M > a[y] for all y in the range.
        // Also, we need M >= max(xs, xf). But note that xs and xf are already > a[ys] and > a[yf] respectively, and they are ≡ rem mod k. So xs <= M and xf <= M automatically because M is the largest ≡ rem mod k. So M >= max(xs, xf) is automatically true if xs and xf are valid. Actually, xs could be less than M, but M is the maximum, so M >= xs and M >= xf. So that condition is redundant.
        // So the only condition is: for all y between ys and yf inclusive, a[y] < M.
        // But is that sufficient? Let's test with sample.
        // Sample 1: n=11, m=10, a = [9,0,0,10,3,4,8,11,10,8] (1-indexed)
        // Query 1: xs=1, ys=2, xf=1, yf=3, k=1. rem=0. M = 11 - ((11-0)%1) = 11 - 0 = 11. Range y=2..3, a[2]=0, a[3]=0, both < 11. YES. Correct.
        // Query 2: xs=1, ys=2, xf=1, yf=3, k=2. rem=1. M = 11 - ((11-1)%2) = 11 - (10%2)=11-0=11. Range a[2]=0, a[3]=0 < 11. But output is NO. Why? Because with k=2, from row 1, moving up 2 steps goes to row 3, then 5,7,9,11. But start is row 1, finish is row 1. We need to go from (1,2) to (1,3). We can move right? Right moves 2 columns at a time. From column 2, moving right once goes to column 4. We cannot reach column 3 directly. So horizontal moves also step by k. So we need to be able to move horizontally in steps of k. That means the column difference must be a multiple of k. We already checked that. But also, when moving horizontally, we must stay on a row that is unblocked in all intermediate columns. In query 2, we need to go from column 2 to column 3. Difference is 1, not multiple of 2. So we already output NO because of column difference mod k != 0. So that's covered.
        // Query 3: xs=4, ys=3, xf=4, yf=5, k=2. rem=0. M=11. Range y=3..5, a[3]=0, a[4]=10, a[5]=3. a[4]=10 < 11? Yes, 10 < 11. So M > a[y] for all. But output is NO. Why? Let's see: start (4,3), finish (4,5). k=2. Column diff = 2, multiple of 2. Row diff = 0, multiple of 2. So alignment is fine. But can we move? From (4,3), we can move right by 2 columns to (4,5). But we need to check if the intermediate column 4 is unblocked at row 4? a[4]=10, so row 4 is blocked! Because a[4]=10 means rows 1..10 are blocked. So row 4 is blocked in column 4. So we cannot pass through column 4 at row 4. We could move up first? From (4,3), move up 2: to (6,3). Then move right 2: to (6,5). Then move down 2: to (4,5). Check column 4 at row 6: a[4]=10, row 6 is blocked! So still blocked. Move up more: to (8,3), then right to (8,5), then down to (4,5). At row 8, column 4: a[4]=10, row 8 is blocked. Row 10 is blocked. Row 12 is out of grid (n=11). So we cannot ever be in column 4 at an unblocked row that is ≡ 4 mod 2? Actually, start row 4, so reachable rows are even rows: 4,6,8,10. All these are ≤10, and a[4]=10 blocks rows 1..10. So all even rows up to 10 are blocked. The only even row >10 is 12, out of grid. So column 4 is impassable. Our condition M > a[y] gave M=11 > 10, so it said column 4 is passable. But it's not, because the reachable rows in column 4 are only those ≡ rem mod k. rem = xs % k = 4 % 2 = 0. So reachable rows are even rows. The largest even row ≤11 is 10. But 10 is blocked (a[4]=10 blocks row 10). So the maximum reachable row in column 4 is actually 10, but it's blocked. The next lower even row is 8, also blocked, down to 2, all blocked. So there is no reachable row > a[4] in column 4. Our M was 11, but 11 is odd, not reachable because rem=0. So M must be computed with the correct remainder, but also must be > a[y]. However, M=11 is not reachable because 11 % 2 = 1 ≠ 0. So my formula for M was wrong: M = n - ((n - rem) % k) gives the largest number ≤ n with remainder rem? Let's check: n=11, rem=0, k=2. n - rem = 11. 11 % 2 = 1. n - 1 = 10. 10 % 2 = 0. So M=10. I earlier computed M=11 incorrectly because I did (11-0)%2 = 1, 11-1=10. Yes, M=10. So M=10. Then a[4]=10, so M > a[4] is false (10 > 10 is false). So column 4 fails. So condition M > a[y] is correct. My manual calculation of M was wrong. So the condition is: for all y in range, M > a[y], where M = n - ((n - rem) % k) is the maximum reachable row in the entire grid for that remainder. But wait: what if M <= a[y]? Then no reachable row exists in that column. So we need M > a[y] for all y in the range.
        // But is that sufficient? Consider a case where M > a[y] for all y, but the start row xs is not M. We can always move up to M, then move horizontally, then move down to xf. Since M is reachable in all columns (because M > a[y] and M ≡ rem mod k), we can go up from xs to M (since xs ≡ rem, and we can move up in steps of k until M), then move horizontally in steps of k (since column difference is multiple of k, we can move exactly to the target column by repeated right/left moves of k columns, staying at row M which is unblocked in all intermediate columns because M > a[y]), then move down from M to xf (since xf ≡ rem, we can move down in steps of k). So yes, it's sufficient.
        // But wait: what if we don't need to go all the way up to M? We just need some row R ≥ max(xs, xf) that is reachable in all columns between. The maximum such row is M, but if M works, we're good. If M doesn't work (i.e., M ≤ a[y] for some y), could there be a lower row that works? Suppose M ≤ a[y] for some y. That means the largest row ≡ rem is ≤ a[y]. Then any row ≡ rem is ≤ M ≤ a[y], so all rows with that remainder are blocked in that column. So no reachable row exists in that column. So it's impossible. So the condition is exactly M > max_{y in range} a[y].
        // So for each query, we need to check:
        // 1. abs(ys - yf) % k == 0
        // 2. abs(xs - xf) % k == 0
        // 3. Let rem = xs % k. M = n - ((n - rem) % k). (If M > n, M -= k? Actually, the formula gives ≤ n. If rem > n, then M would be negative? But xs ≤ n, so rem ≤ n. If rem > n, but rem < k, and k can be > n. If k > n, then rem = xs. Then n - rem might be negative. The formula n - ((n - rem) % k) works with C++ modulo? In C++, % for negative numbers is implementation-defined before C++11, but in C++11 it's defined as remainder with sign of dividend. So (n - rem) % k could be negative. Better to compute M safely.)
        // Actually, we can compute M as: if (rem > n) then no reachable row? But xs ≤ n, so rem ≤ n. If k > n, then the only reachable row is xs itself (since moving up k steps goes out of grid). So M = xs. But the formula: n - ((n - rem) % k). If k > n, then (n - rem) % k = n - rem (since n - rem < k). Then M = n - (n - rem) = rem = xs. Works if n - rem >= 0. If n - rem < 0, then rem > n, impossible. So it's fine. But to avoid negative modulo, we can do: int max_row = n - ((n - rem) % k + k) % k; Actually, ((n - rem) % k + k) % k gives non-negative remainder. So M = n - ((n - rem) % k + k) % k. But if n - rem is negative, then (n - rem) % k is negative, adding k makes it positive. So M = n - positive. That could be less than n. But if rem > n, then n - rem < 0, and we want M < 0? But rem > n never happens. So it's safe.
        // Alternatively, we can just find the largest r ≤ n with r % k == rem: r = n - ((n - rem) % k + k) % k. But if r < rem, then r += k? Actually, if n < rem, then r would be negative. But n >= xs >= rem? xs could be > n? No, xs ≤ n. So rem ≤ n. So n - rem >= 0. So (n - rem) % k is fine. So we can just do M = n - (n - rem) % k. In C++17, % with non-negative left operand is non-negative. So it's safe.
        // So condition 3: M > max_a in range [min(ys, yf), max(ys, yf)].
        // We can precompute a segment tree or sparse table for range maximum queries on a.
        // Since m up to 2e5, q up to 2e5, we can use a segment tree or a sparse table. Sparse table O(m log m) build, O(1) query. Good.

        // Let's test query 3 with correct M: xs=4, k=2, rem=0. n=11. M = 11 - (11-0)%2 = 11 - 1 = 10. max_a in range 3..5 = max(0,10,3) = 10. M > 10? 10 > 10 false. So NO. Correct.
        // Query 4: xs=5, ys=3, xf=11, yf=5, k=3. rem=2. M = 11 - (11-2)%3 = 11 - 9%3 = 11 - 0 = 11. Range 3..5 max_a = 10. 11 > 10 true. Also column diff = 2, not multiple of 3. So NO. Correct.
        // Query 5: xs=5, ys=3, xf=11, yf=5, k=2. rem=1. M = 11 - (11-1)%2 = 11 - 10%2 = 11 - 0 = 11. Range max_a = 10. 11 > 10 true. Column diff = 2, multiple of 2. Row diff = 6, multiple of 2. So YES. Correct.
        // Query 6: xs=11, ys=9, xf=9, yf=10, k=1. rem=0. M=11. Range 9..10 max_a = max(10,8)=10. 11>10 true. Diffs: col 1, row 2, both multiples of 1. YES. Correct.

        // So algorithm:
        // Build sparse table for max in a[1..m].
        // For each query:
        //   if (abs(ys - yf) % k != 0) NO
        //   if (abs(xs - xf) % k != 0) NO
        //   rem = xs % k;
        //   M = n - (n - rem) % k;
        //   if (M <= max_a_in_range(min(ys, yf), max(ys, yf))) NO
        //   else YES
        // But wait: what if M < xs? That can't happen because xs ≡ rem mod k and xs ≤ n, so M ≥ xs. Similarly M ≥ xf.
        // So it's fine.

        // Edge case: k can be large, but modulo operations are fine.
        // Sparse table:
        int LOG = 0;
        while ((1 << LOG) <= m) LOG++;
        vector<vector<int>> st(LOG, vector<int>(m + 1));
        for (int i = 1; i <= m; ++i) st[0][i] = a[i];
        for (int j = 1; j < LOG; ++j) {
            for (int i = 1; i + (1 << j) - 1 <= m; ++i) {
                st[j][i] = max(st[j-1][i], st[j-1][i + (1 << (j-1))]);
            }
        }
        auto range_max = [&](int l, int r) {
            int len = r - l + 1;
            int j = 31 - __builtin_clz(len);
            return max(st[j][l], st[j][r - (1 << j) + 1]);
        };

        // Process queries
        while (q--) {
            int xs, ys, xf, yf, k;
            cin >> xs >> ys >> xf >> yf >> k;
            if (abs(ys - yf) % k != 0) {
                cout << "NO\n";
                continue;
            }
            if (abs(xs - xf) % k != 0) {
                cout << "NO\n";
                continue;
            }
            int rem = xs % k;
            int M = n - (n - rem) % k;
            int l = min(ys, yf), r = max(ys, yf);
            int max_a = range_max(l, r);
            if (M <= max_a) {
                cout << "NO\n";
            } else {
                cout << "YES\n";
            }
        }
    }
    return 0;
}
