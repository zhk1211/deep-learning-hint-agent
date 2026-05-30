// Hint6
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<string> grid(n);
        for (int i = 0; i < n; ++i) {
            cin >> grid[i];
        }
        
        // We'll process row by row from top to bottom.
        // For each cell, we need to know the current parity due to operations
        // from previous rows that affect this cell.
        // The operation at (i, j) affects (x, y) if x > i and x - i >= |y - j|.
        // This is equivalent to: for a fixed row r > i, the affected columns are
        // from j - (r - i) to j + (r - i).
        // So the effect on row r is a range update with +1 (mod 2) on an interval.
        // We can maintain difference arrays for the "triangular" effects.
        
        // We maintain three arrays for the current row:
        // diag1: effect from operations that propagate down-right (slope 1)
        // diag2: effect from operations that propagate down-left (slope -1)
        // straight: effect from operations that propagate straight down? Actually
        // the operation's effect is symmetric: it's a combination of both diagonals.
        // But we can decompose it into two prefix sums.
        
        // Let's use the approach from editorial: maintain two arrays L and R
        // representing the effect of operations from above that affect current row
        // as a range. We can use difference arrays for the current row.
        
        vector<int> diag1(2 * n + 5, 0); // slope 1: r + c
        vector<int> diag2(2 * n + 5, 0); // slope -1: r - c + n
        vector<int> col_effect(n + 2, 0); // for current row's column-wise effect
        
        int ans = 0;
        for (int r = 0; r < n; ++r) {
            // compute prefix sum of col_effect to get current parity for each column
            int cur = 0;
            for (int c = 0; c < n; ++c) {
                cur += col_effect[c];
                // also add effects from diagonals
                int val = (grid[r][c] - '0') ^ (cur & 1) ^ ((diag1[r + c] + diag2[r - c + n]) & 1);
                if (val == 1) {
                    // need to perform operation at (r, c)
                    ++ans;
                    // update col_effect: operation at (r, c) affects rows below
                    // For row r+1, it affects columns from c-1 to c+1
                    // For row r+2, from c-2 to c+2, etc.
                    // This can be done by updating diag1 and diag2.
                    // diag1: slope 1, affects cells with same r+c
                    // diag2: slope -1, affects cells with same r-c
                    // But careful: the operation affects all cells (x, y) with x > r and x - r >= |y - c|.
                    // This is exactly the union of two "cones": one going down-left and one down-right.
                    // We can represent this by adding to diag1 and diag2, but we must avoid double counting the straight down part?
                    // Actually the operation's effect is exactly: for each row x > r, columns from c - (x - r) to c + (x - r).
                    // This is equivalent to adding 1 to diag1 at (r, c) and diag2 at (r, c), but then the intersection (the column c) would be added twice.
                    // So we need to subtract the straight down effect.
                    // Alternatively, we can maintain a separate straight down effect array.
                    // Let's use three arrays: diag1, diag2, and straight.
                    // But we can combine them into col_effect updates for future rows using difference arrays over rows.
                    // Simpler: maintain an array "effect" for each row that tells the range of columns affected.
                    // Since n <= 3000, we can do O(n^2) per test case, total sum n^2 <= 9e6, so O(n^2) is fine.
                    // We can just simulate the effect by updating a 2D difference array? That would be O(n^3) if naive.
                    // But we can do it row by row with prefix sums.
                    
                    // Let's use the method from hints: maintain prefix sums of operations from previous rows.
                    // We can maintain an array "op" of size n for the current row's operations.
                    // But we need to know for each cell the cumulative effect from above.
                    // We can maintain three running sums: one for straight down, one for diagonal down-right, one for diagonal down-left.
                    // Actually, the operation at (i, j) affects cell (r, c) if r > i and |c - j| <= r - i.
                    // This is equivalent to: i + j <= r + c and i - j <= r - c and i < r.
                    // Not exactly.
                    
                    // Let's use the approach with difference arrays on diagonals.
                    // We'll maintain:
                    // diag1[d] = number of operations that affect diagonal d (r + c) from above.
                    // diag2[d] = number of operations that affect diagonal d (r - c + n) from above.
                    // But an operation affects a range of diagonals? No, it affects all cells in a "cone".
                    // Actually, if we perform operation at (i, j), then for any row r > i, the affected columns are from j - (r - i) to j + (r - i).
                    // This means that on row r, the operation adds 1 to an interval of columns.
                    // We can propagate this interval to the next row: if on row r the interval is [L, R], then on row r+1 it becomes [L-1, R+1].
                    // So we can maintain for each row the list of active intervals.
                    // Since we process row by row, we can maintain the current intervals from all operations above.
                    // Each operation at (i, j) starts an interval at row i+1: [j, j] (length 1), then expands.
                    // We can maintain a difference array for the current row's column effects, and also maintain how many active intervals we have that are expanding.
                    // Specifically, we can keep track of the "left expanding" and "right expanding" effects.
                    
                    // Let's use the solution from editorial:
                    // Maintain arrays:
                    // col_effect: difference array for the current row's column parity from operations that are "active".
                    // diag1: effect that shifts left each row (slope -1? Actually if interval expands left, the left boundary moves left by 1 each row).
                    // diag2: effect that shifts right each row.
                    // We can maintain the number of operations that are currently affecting the row as a range, and update the difference array for the next row.
                    
                    // We'll do it as follows:
                    // We have an array "cur_ops" that tells for each column the number of operations that affect this column in the current row.
                    // But we need to update it for the next row.
                    // Let L[c] be the number of operations that have their left boundary at column c (i.e., they started at some (i, j) and on current row their interval is [c, something]).
                    // Actually, it's easier to maintain two arrays: left_expand and right_expand.
                    // left_expand[c] = number of operations whose interval's left end is at column c.
                    // right_expand[c] = number of operations whose interval's right end is at column c.
                    // Then for the next row, the left ends all decrease by 1, right ends increase by 1.
                    // So we can shift these arrays.
                    
                    // Let's implement:
                    // We maintain arrays L and R of size n+2 (1-indexed for convenience).
                    // L[c] = number of active operations whose left boundary is exactly c.
                    // R[c] = number of active operations whose right boundary is exactly c.
                    // Also we maintain a difference array D for the current row's column parity.
                    // Initially all zero.
                    // For each row r from 1 to n:
                    //   Compute prefix sum of D to get current parity for each column.
                    //   For each column c:
                    //      val = grid[r][c] ^ parity[c]
                    //      if val == 1:
                    //         ans++
                    //         // start a new operation at (r, c)
                    //         // This operation will affect row r+1: columns [c-1, c+1]
                    //         // So for row r+1, we need to add 1 to interval [c-1, c+1].
                    //         // We can do this by updating L and R for the next row.
                    //         // But we are currently processing row r, so we need to apply updates for row r+1.
                    //         // We can maintain L_next and R_next.
                    //   After processing row r, we prepare for row r+1:
                    //      new D for row r+1 is computed from L and R of current row.
                    //      Actually, the effect on row r+1 from operations that were active on row r is:
                    //      each operation with interval [L, R] on row r becomes [L-1, R+1] on row r+1.
                    //      So the difference array for row r+1 can be built by taking L and R from row r, shifting them.
                    
                    // Let's define:
                    // For current row, we have an array "add" which is the difference array of the parity effect from all operations above.
                    // We can compute it on the fly.
                    
                    // Alternative simpler O(n^2) method:
                    // We can maintain a 2D array "affected" but that's too large.
                    // We can maintain for each row the list of intervals, but total intervals could be O(n^2).
                    
                    // Let's use the method with three arrays: diag1, diag2, and straight.
                    // diag1[d] = number of operations that affect the diagonal d = r + c.
                    // diag2[d] = number of operations that affect the diagonal d = r - c + n.
                    // But an operation affects a range of diagonals? Actually, an operation at (i, j) affects cell (r, c) iff r > i and |c - j| <= r - i.
                    // This condition is equivalent to: i + j <= r + c and i - j <= r - c and i < r? Not exactly.
                    // Let's derive: |c - j| <= r - i  =>  - (r - i) <= c - j <= r - i
                    // => j - (r - i) <= c <= j + (r - i)
                    // => i + j <= r + c  and  i - j <= r - c.
                    // So (r, c) is affected iff r > i, r + c >= i + j, and r - c >= i - j.
                    // So if we define for each operation its "start" diagonals: d1 = i + j, d2 = i - j.
                    // Then for a cell (r, c), the number of operations affecting it is the number of operations with i < r, d1 <= r + c, d2 <= r - c.
                    // This is a 2D range query. We can process row by row, maintaining a 2D BIT? Too slow.
                    
                    // Let's go back to the interval expansion idea.
                    // We can maintain for each row the difference array of column effects.
                    // Let diff[c] be the difference array for the current row.
                    // When we move to the next row, the intervals expand: each operation's interval [L, R] becomes [L-1, R+1].
                    // This means the difference array for the next row can be obtained by:
                    // new_diff[c] = old_diff[c] + (something from expansions).
                    // Actually, if we have an operation that on current row has interval [L, R], its contribution to diff is +1 at L, -1 at R+1.
                    // On next row, interval is [L-1, R+1], so contribution is +1 at L-1, -1 at R+2.
                    // So the difference array shifts: the +1 moves left by 1, the -1 moves right by 1.
                    // We can maintain two arrays: one for the "left endpoints" and one for the "right endpoints".
                    // Let left_end[c] = number of operations whose interval's left endpoint is c.
                    // Let right_end[c] = number of operations whose interval's right endpoint is c.
                    // Then the difference array for current row is:
                    // diff[c] = left_end[c] - right_end[c-1]? Not exactly.
                    // Actually, the effect on column c is the number of intervals covering c.
                    // If we have left_end and right_end, the number of intervals covering c is:
                    // sum_{i <= c} left_end[i] - sum_{i < c} right_end[i].
                    // So diff[c] = left_end[c] - right_end[c-1] (with right_end[-1]=0).
                    // Then prefix sum of diff gives the coverage.
                    // For the next row, each left endpoint moves left by 1, each right endpoint moves right by 1.
                    // So new_left_end[c] = old_left_end[c+1] (since left endpoint at c+1 moves to c).
                    // new_right_end[c] = old_right_end[c-1] (since right endpoint at c-1 moves to c).
                    // Also, new operations started at current row (r, c) will have interval on next row: [c-1, c+1].
                    // So they contribute +1 to left_end[c-1] and +1 to right_end[c+1] for the next row.
                    // This gives an O(n^2) algorithm!
                    
                    // Let's formalize:
                    // We process rows from 0 to n-1.
                    // Maintain arrays L[0..n-1] and R[0..n-1] for the current row's left and right endpoints.
                    // Initially all zero.
                    // For each row r:
                    //   Compute prefix sums to get current parity:
                    //   cur = 0, cov = 0
                    //   for c in 0..n-1:
                    //       cov += L[c] - (c > 0 ? R[c-1] : 0)
                    //       parity = cov % 2
                    //       if grid[r][c] ^ parity:
                    //           ans++
                    //           // start new operation at (r, c)
                    //           // It will affect next row: left endpoint at c-1, right endpoint at c+1
                    //           // We need to update L and R for the next row.
                    //           // But we are currently using L and R for this row. We can accumulate updates for next row in separate arrays L_next, R_next.
                    //   After processing row, prepare L and R for next row:
                    //      L_next[c] = L[c+1] + (new left endpoints at c from operations started this row)
                    //      R_next[c] = R[c-1] + (new right endpoints at c from operations started this row)
                    //   Then swap or assign.
                    
                    // Let's implement this.
                }
            }
        }
        
        // Re-implement cleanly:
        // We'll use 0-indexed columns.
        // L and R size n+2 to avoid bounds checking.
        vector<int> L(n + 2, 0), R(n + 2, 0);
        // We'll process row by row.
        for (int r = 0; r < n; ++r) {
            vector<int> L_next(n + 2, 0), R_next(n + 2, 0);
            int cov = 0;
            for (int c = 0; c < n; ++c) {
                cov += L[c] - (c > 0 ? R[c-1] : 0);
                int val = (grid[r][c] - '0') ^ (cov & 1);
                if (val) {
                    ++ans;
                    // new operation at (r, c)
                    // next row: left endpoint at c-1, right endpoint at c+1
                    if (c - 1 >= 0) L_next[c - 1]++;
                    else L_next[0]++; // actually if c-1 < 0, the interval starts at column 0? Wait, the interval on next row is [c-1, c+1]. If c-1 < 0, it means the left bound is 0? But the operation's effect: for row r+1, columns from c-1 to c+1. If c-1 < 0, then columns < 0 don't exist, so the interval is effectively [0, c+1]. So the left endpoint is 0.
                    // Similarly, if c+1 >= n, right endpoint is n-1.
                    // So we should clamp.
                    int left = max(0, c - 1);
                    int right = min(n - 1, c + 1);
                    L_next[left]++;
                    R_next[right]++;
                }
            }
            // Also, existing operations expand:
            // For each left endpoint at column c in current row, it moves to c-1 in next row.
            // For each right endpoint at column c in current row, it moves to c+1 in next row.
            // We need to add these to L_next and R_next.
            for (int c = 0; c < n; ++c) {
                if (L[c]) {
                    int new_left = max(0, c - 1);
                    L_next[new_left] += L[c];
                }
                if (R[c]) {
                    int new_right = min(n - 1, c + 1);
                    R_next[new_right] += R[c];
                }
            }
            L = move(L_next);
            R = move(R_next);
        }
        cout << ans << "\n";
    }
    return 0;
}
