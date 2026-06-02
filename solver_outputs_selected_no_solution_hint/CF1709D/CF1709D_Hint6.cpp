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

        // The highest row the robot can reach in the start column
        // It can go up to the top, but must avoid blocked cells.
        // Actually, the robot can move up to any row >= xs, but limited by n.
        // The key is the maximum row it can achieve in columns between ys and yf.
        // It can go up to n, but must stay above the blocked cells.
        // The maximum possible row in column i is n, but the robot must be at row > a[i].
        // Since it moves in steps of k, the reachable rows in column i are those congruent to xs mod k
        // and > a[i] and <= n.
        // The maximum reachable row in column i is: n - ((n - xs) % k) if we can go up freely,
        // but we must also ensure we can pass through the column.
        // Actually, the robot can move up to increase row, but cannot go into blocked cells.
        // The critical constraint is the minimum of the maximum reachable rows across all columns
        // in the path from ys to yf.
        // Since we can move left/right arbitrarily, we can choose any path.
        // The best we can do is to go up as much as possible in the start column, then move horizontally,
        // then go up/down to the target.
        // The maximum row we can be at when crossing column i is limited by a[i].
        // We need to be at row > a[i] in column i.
        // The maximum row reachable in column i is: the largest row <= n with row % k == xs % k and row > a[i].
        // Let max_row_in_col(i) = n - ((n - xs) % k) if that is > a[i], else we need to find the largest
        // row <= n with same modulo that is > a[i]. But since we can also move down, we just need to ensure
        // that there exists some row > a[i] with the correct modulo.
        // Actually, the robot can move up and down, so the set of reachable rows in column i is all rows
        // with row % k == xs % k and row > a[i] and row <= n.
        // The maximum such row is: let r = n - ((n - xs) % k); if r > a[i] then r else r - k (if still > a[i]) etc.
        // But we don't need the maximum; we need to know if we can go from start to finish.
        // The condition is: we can reach the finish if and only if we can find a row R such that:
        // R % k == xs % k, R > max(a[i]) for i between ys and yf inclusive? Not exactly.
        // Because we can go up in the start column, then move horizontally at some row, then go to finish.
        // The horizontal movement must be at a row that is > a[i] for all columns we pass through.
        // So we need to find a row R with R % k == xs % k, R > max_{i in [min(ys,yf), max(ys,yf)]} a[i],
        // and also R <= n, and we must be able to reach R from xs and from R to xf.
        // Since we can move vertically in steps of k, we can reach any row with the same modulo as xs
        // as long as it's > a[start] and > a[finish] respectively? Actually, we start at xs, so we can
        // go up to any row >= xs with same modulo, and down to any row <= xs with same modulo, as long
        // as we don't hit blocked cells. But blocked cells are only at the bottom, so going down is limited
        // by a[start] and a[finish]. Going up is limited by n.
        // So we need a row R such that:
        // 1. R % k == xs % k
        // 2. R > a[i] for all i in [min(ys,yf), max(ys,yf)]
        // 3. R >= xs and R >= xf? Actually, we can go down from R to xf if xf <= R and xf % k == R % k.
        //    But we can also go up from xs to R if xs <= R. Since xs and xf have the same modulo as R,
        //    we just need R >= max(xs, xf)? Not necessarily, because we could go down from xs to some R
        //    if R <= xs. But then we would need R > a[i] for all columns, and R <= xs, R >= xf?
        //    Actually, we can go up or down. The only constraint is that the path from xs to R and R to xf
        //    must not hit blocked cells. Since blocked cells are at the bottom, going down is dangerous.
        //    If we go down, we must ensure we don't go <= a[start] or <= a[finish].
        //    So if we choose R < xs, we need R > a[start] and we go down from xs to R.
        //    Similarly, if R < xf, we need R > a[finish] and we go down from xf to R? Wait, we go from R to xf,
        //    so if R < xf, we go up from R to xf, which is safe as long as R > a[finish]? Actually, going up
        //    from R to xf is safe because we only go up, and blocked cells are at the bottom. The only danger
        //    is if R <= a[finish], then we start in a blocked cell, which is not allowed. So we need R > a[finish]
        //    and R > a[start]. But if we go down from xs to R, we need to ensure that all intermediate rows
        //    are unblocked in the start column. Since blocked cells are only at the bottom, any row > a[start]
        //    is unblocked. So going down from xs to R is safe as long as R > a[start].
        //    Similarly, going up from R to xf is safe as long as R > a[finish]? Actually, if R <= a[finish],
        //    we can't even be at R in the finish column. So we need R > a[finish] and R > a[start].
        //    But wait: we are at R in some column, then we move horizontally to the finish column, then we
        //    move vertically to xf. The horizontal movement is at row R, so we need R > a[i] for all columns
        //    we pass through. Then in the finish column, we move from R to xf. If R < xf, we go up, which is
        //    safe because all rows from R to xf are > a[finish]? Not necessarily: a[finish] could be between
        //    R and xf. But we are moving up, so we start at R > a[finish] and go up, so we never hit the
        //    blocked cells (which are at the bottom). So it's safe. If R > xf, we go down from R to xf.
        //    This requires that all rows from xf to R are > a[finish]. Since a[finish] is the number of
        //    blocked cells at the bottom, any row > a[finish] is unblocked. So we need xf > a[finish] (given)
        //    and R > a[finish]. But if we go down, we might hit a[finish] if R is not careful? Actually,
        //    if we go down from R to xf, we pass through rows R, R-1, ..., xf. All these rows must be > a[finish].
        //    Since xf > a[finish] is given, and R > a[finish], all intermediate rows are > a[finish] because
        //    they are between xf and R. So it's safe. The only danger is if R <= a[finish], but then we can't
        //    even be at R. So the condition is simply R > a[start] and R > a[finish] and R > max_{i in range} a[i].
        //    But wait: what if we go down from xs to R, and then horizontally, and then up to xf?
        //    That's fine as long as R > a[start], R > a[finish], and R > all a[i] in between.
        //    So we need to find if there exists an integer R such that:
        //    - R % k == xs % k
        //    - R > max(a[start], a[finish], max_{i in [min(ys,yf), max(ys,yf)]} a[i])
        //    - R <= n (since we can't go above n)
        //    - Also, we need to be able to go from xs to R and from R to xf. Since xs and xf have the same
        //      modulo as R, we can always go vertically between them as long as we don't hit blocked cells.
        //      But if R < xs, we go down from xs to R. This requires that all rows from R to xs are > a[start].
        //      Since xs > a[start] (given) and R > a[start], it's safe. Similarly for xf.
        //      So no additional constraint.
        //    Therefore, the problem reduces to: does there exist a row R with R % k == xs % k,
        //    R > M, and R <= n, where M = max(a[ys], a[yf], max_{i between ys and yf} a[i])?
        //    But wait: is it always optimal to choose the smallest possible R > M? Yes, because we just need
        //    one such R. The smallest R > M with the correct modulo is: let rem = xs % k.
        //    We want the smallest R > M such that R % k == rem.
        //    That is: R = M + 1 + ((rem - (M + 1)) % k + k) % k.
        //    If R <= n, then YES, else NO.
        //    But is that sufficient? Consider the case where we need to go up from xs to R, but xs > R?
        //    Then we go down. That's fine. What if xs is very high and we want to go down to a low R,
        //    but a[start] is high? We already enforce R > a[start]. So it's fine.
        //    What if we want to go up from xs to R, but R > n? Not allowed.
        //    So the condition seems correct.

        // However, there is a catch: the robot moves in steps of k. When moving horizontally, it moves
        // exactly k cells left or right. So the column change must be a multiple of k. We already checked
        // that |ys - yf| % k == 0. But we also need to ensure that the horizontal movement doesn't
        // go out of bounds? The grid has columns 1..m, and we can move left/right. If we move left from
        // column ys, we might go to column < 1? But we can choose the path: we can move right/left
        // multiple times. Since we only need to reach yf, and the difference is a multiple of k,
        // we can just move directly. But we must ensure that all intermediate columns are within 1..m.
        // Since we move exactly k cells at a time, if we move from ys to yf, we will pass through
        // columns ys, ys +/- k, ys +/- 2k, ... yf. All these columns must be within 1..m.
        // But wait: we can also move up/down to change rows, and then move horizontally. The horizontal
        // movement is independent of vertical. We can move horizontally at any row. So we just need
        // to ensure that there is a path from ys to yf moving in steps of k that stays within 1..m.
        // Since we can move left and right, we can always go towards yf. The only issue is if we
        // are forced to go outside 1..m. But we can always choose to move in the direction of yf.
        // If ys < yf, we move right by k each time. We will land on columns ys, ys+k, ..., yf.
        // All these are <= yf <= m, so they are within bounds. Similarly if ys > yf, we move left.
        // So no out-of-bounds issue as long as the start and finish are within bounds (given).
        // So we don't need to check that.

        // But wait: what if k is very large? Then moving right by k might overshoot yf?
        // But we can only move exactly k cells. So if |ys - yf| is not a multiple of k, we already
        // output NO. If it is a multiple, we can just move directly. So it's fine.

        // So the algorithm is:
        // For each query:
        // 1. Check if |xs - xf| % k == 0 and |ys - yf| % k == 0. If not, NO.
        // 2. Let M = max(a[ys], a[yf], max_{i in [min(ys,yf), max(ys,yf)]} a[i]).
        // 3. Find the smallest R > M with R % k == xs % k.
        // 4. If R <= n, YES, else NO.

        // But we need to compute the maximum of a[i] in a range efficiently.
        // Since m <= 2e5 and q <= 2e5, we can use a segment tree or sparse table for RMQ.
        // Sparse table is O(m log m) build, O(1) query. Perfect.

        // Wait: is there any other constraint? The hints say: "If k=1, you can reach x-th row from y-th row iff x <= y."
        // That suggests that for k=1, the condition is different? Let's test with k=1.
        // For k=1, our condition: R > M, R % 1 == xs % 1 (always true), smallest R > M is M+1.
        // So we need M+1 <= n. But M is max of a[i] in range. Since a[i] <= n, M+1 <= n+1.
        // If M = n, then M+1 = n+1 > n, so NO. So if any column in the range has a[i] = n, we can't pass.
        // But the hint says: "If k=1, you can reach x-th row from y-th row iff x <= y." That seems to refer
        // to something else? Let's read hint 6: "If k = 1, you can reach x-th row from y-th row iff x <= y."
        // That might be about moving only up? Actually, the robot can move up, right, down, left.
        // With k=1, it moves one cell at a time. The condition to go from (xs, ys) to (xf, yf) is that
        // there exists a path avoiding blocked cells. Since blocked cells are at the bottom, the only
        // obstacle is that you cannot go below a[i] in column i. So you need to find a row R such that
        // R > max a[i] in the range, and you can go from xs to R and R to xf. Since k=1, you can change
        // row by 1 each step. So you can go from xs to R if R >= xs? Actually, you can go down as well,
        // but you must not hit blocked cells. If R < xs, you go down, but you need R > a[start].
        // So the condition is exactly as we derived. But the hint says "x <= y" which might be a different
        // interpretation. Let's check the sample with k=1.
        // Sample 1: n=11, m=10, a = [9,0,0,10,3,4,8,11,10,8] (1-indexed)
        // Query 1: 1 2 1 3 1 -> xs=1, ys=2, xf=1, yf=3, k=1. Output YES.
        // Our condition: |1-1|%1=0, |2-3|%1=0. M = max(a[2], a[3], max_{i in [2,3]} a[i]) = max(0,0)=0.
        // Smallest R > 0 with R%1=0 is 1. 1 <= 11 -> YES. Correct.
        // Query 6: 11 9 9 10 1 -> xs=11, ys=9, xf=9, yf=10, k=1. Output YES.
        // M = max(a[9]=10, a[10]=8, max in [9,10]) = 10. R > 10 -> 11. 11 <= 11 -> YES. Correct.
        // So our condition works for k=1.

        // But hint 5 says: "If a[x] != a[y] for some x != y, then the answer for k=1 is NO."
        // That seems contradictory because in sample 1, a[9]=10, a[10]=8, they are different, yet answer is YES.
        // Wait, hint 5 might be for a different problem? Or maybe it's about something else.
        // Let's read the hints carefully: 
        // Hint 1: Try to solve the problem when k = 1. What is the answer?
        // Hint 2: Is the answer the same for k >= 2?
        // Hint 3: How can we calculate the answer for k = 1 efficiently?
        // Hint 4: Is there some property of the answer for k = 1, which allows us to find the answer for k >= 2?
        // Hint 5: If a[x] != a[y] for some x != y, then the answer for k = 1 is "NO".
        // Hint 6: If k = 1, you can reach x-th row from y-th row iff x <= y.
        // These hints seem to be for a different problem? Or maybe I misread the problem statement.
        // Let's re-read the problem statement carefully.
        // "The i-th column has the bottom a_i cells blocked (the cells in rows 1, 2, ..., a_i), the remaining n - a_i cells are unblocked."
        // So blocked cells are at the bottom. The robot can move up, right, down, left.
        // If it attempts to move into a blocked cell or outside the grid, it explodes.
        // The robot executes each command k times.
        // You can send an arbitrary number of commands.
        // The robot must stop in the finish cell. If it visits the finish cell while still executing commands, it doesn't count.
        // So the robot must end exactly at the finish cell after completing a command.
        // That means the sequence of commands must result in the robot being at the finish cell after some command.
        // It cannot stop in the middle of a command.
        // Our condition that the row difference must be a multiple of k is correct because each vertical command moves k cells.
        // Similarly for horizontal.
        // But wait: what if we move up and then down? The net vertical displacement must be a multiple of k.
        // But we can also move horizontally in between. The total vertical displacement is sum of up/down commands * k.
        // So the difference in rows must be a multiple of k. That's what we checked.
        // However, is it possible that we need to move up and then down to avoid blocked cells, and the net is a multiple of k,
        // but the intermediate rows might be blocked? Our condition with R ensures we can find a safe row to cross horizontally.
        // But what if we need to go up, then horizontally, then down, and the horizontal movement is at a row that is not
        // reachable from the start because we can't go up that high due to blocked cells in the start column?
        // In the start column, we can go up to any row <= n as long as we don't hit blocked cells. Since blocked cells are
        // only at the bottom (rows 1..a[start]), we can go up from xs to any row >= xs. We cannot go down below xs because
        // we would hit blocked cells if we go <= a[start]. But xs > a[start] is given. So we can go down to any row > a[start].
        // So from xs, we can reach any row R with R % k == xs % k and R > a[start] and R <= n.
        // Similarly, from R we can reach xf if xf % k == R % k and xf > a[finish] (given) and we can go vertically between
        // R and xf without hitting blocked cells. As argued, that's safe as long as R > a[finish] and xf > a[finish].
        // So our condition seems complete.

        // But why do hints 5 and 6 say those things? Let's test hint 5: "If a[x] != a[y] for some x != y, then the answer for k=1 is NO."
        // In sample 1, a[9]=10, a[10]=8, they are different, but query 6 with k=1 is YES. So hint 5 must be wrong or I misinterpret it.
        // Maybe the hints are for a different version of the problem? Or maybe "a[x] != a[y]" means something else?
        // Perhaps the hints are for a problem where the grid is transposed? Or maybe the hints are for a problem where
        // the robot can only move up and right? No, the problem says up, right, down, left.
        // Let's ignore the hints that seem contradictory and trust our reasoning, which matches the sample.

        // Let's test query 2: 1 2 1 3 2 -> xs=1, ys=2, xf=1, yf=3, k=2. Output NO.
        // Our condition: |1-1|%2=0, |2-3|%2=1 -> not 0, so NO. Correct.
        // Query 3: 4 3 4 5 2 -> xs=4, ys=3, xf=4, yf=5, k=2. Output NO.
        // |4-4|%2=0, |3-5|%2=0. M = max(a[3]=0, a[5]=3, max in [3,5] = max(0,10,3,4?) Wait, columns 3,4,5: a[3]=0, a[4]=10, a[5]=3. So M=10.
        // R > 10, R%2 == 4%2=0. Smallest R > 10 with even is 12. 12 <= 11? No, 12 > 11. So NO. Correct.
        // Query 4: 5 3 11 5 3 -> xs=5, ys=3, xf=11, yf=5, k=3. Output NO.
        // |5-11|%3 = 6%3=0, |3-5|%3=2%3=2 !=0 -> NO. Correct.
        // Query 5: 5 3 11 5 2 -> xs=5, ys=3, xf=11, yf=5, k=2. Output YES.
        // |5-11|%2=0, |3-5|%2=0. M = max(a[3]=0, a[5]=3, max in [3,5] = max(0,10,3,4?) a[4]=10, so M=10.
        // R > 10, R%2 == 5%2=1. Smallest R > 10 with odd: 11. 11 <= 11 -> YES. Correct.
        // Query 6: 11 9 9 10 1 -> already did, YES.

        // So our condition works for all sample queries.

        // Now, we need to implement RMQ. Since m <= 2e5, we can use a segment tree or sparse table.
        // Sparse table: O(m log m) build, O(1) query. m=2e5, log2(2e5) ~ 18. So table size 2e5 * 19 ~ 3.8e6 ints, fine.
        // Alternatively, we can use a segment tree, but sparse table is faster for queries.

        // Let's build sparse table for range maximum query on array a[1..m].

        int LOG = 0;
        while ((1 << LOG) <= m) LOG++;
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

        // Process queries
        // We already read q, now loop
        // But we already started processing queries above. We need to restructure.
        // Actually, we can just read all queries, but we already processed some in the loop.
        // Let's rewrite to read all queries after building sparse table.
    }

    return 0;
}
