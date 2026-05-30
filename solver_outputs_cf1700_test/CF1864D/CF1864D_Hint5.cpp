// Hint5
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

        // We maintain three arrays:
        // diag1: effect from diagonals with slope 1 (x - y constant)
        // diag2: effect from diagonals with slope -1 (x + y constant)
        // row_effect: horizontal prefix effect from operations in the same row
        // We process row by row, column by column.
        // For cell (i, j), the total effect from previous rows is:
        // row_effect[j] + diag1[i - j + n] + diag2[i + j]
        // We need to flip if (grid[i][j] - '0') ^ (effect % 2) == 1.

        vector<int> diag1(2 * n, 0); // index i - j + n
        vector<int> diag2(2 * n, 0); // index i + j
        vector<int> row_effect(n + 1, 0);

        int ans = 0;
        for (int i = 0; i < n; ++i) {
            // prefix sum for row_effect
            int cur = 0;
            for (int j = 0; j < n; ++j) {
                cur ^= row_effect[j];
                int effect = cur ^ diag1[i - j + n] ^ diag2[i + j];
                int val = grid[i][j] - '0';
                if (val ^ effect) {
                    ++ans;
                    // apply operation at (i, j)
                    // update row_effect: it affects columns >= j with a prefix XOR
                    // Actually, the operation toggles a triangle below.
                    // For row i, it toggles (i, j) and for rows below, it toggles a range.
                    // We can propagate using diag1 and diag2.
                    // The operation at (i, j) toggles diag1 starting from (i, j) downwards,
                    // and diag2 starting from (i, j) downwards.
                    // Also it toggles the current cell and the row_effect for columns >= j.
                    // But careful: the effect on row_effect is only for the current row?
                    // Actually, the operation affects cells (x, y) with x > i and x - i >= |y - j|.
                    // This is equivalent to: for each row x > i, it toggles a contiguous segment
                    // from j - (x - i) to j + (x - i).
                    // We can maintain this using diag1 and diag2.
                    // When we apply operation at (i, j):
                    // - diag1[i - j + n] ^= 1 (this affects all cells on this diagonal below)
                    // - diag2[i + j] ^= 1 (this affects all cells on this anti-diagonal below)
                    // - row_effect[j] ^= 1? No, row_effect is for horizontal propagation.
                    // Actually, the operation's effect on the current row is just the cell itself.
                    // For rows below, the effect is carried by diag1 and diag2.
                    // But we also need to account for the fact that the operation toggles a range
                    // that expands by 1 on each side per row. The diag1 and diag2 toggles
                    // exactly represent the boundaries of that range.
                    // However, we must be careful: if we toggle both diag1 and diag2 for the same cell,
                    // the intersection (the cell itself) would be toggled twice, which cancels.
                    // So we need to also toggle something to fix the current cell and the overlapping effects.
                    // Standard approach: maintain three difference arrays:
                    // left (diag1), right (diag2), and down (vertical).
                    // But here we can simplify: operation at (i, j) toggles:
                    // - diag1 at (i, j) (index i - j + n)
                    // - diag2 at (i, j + 1) (index i + j + 1) to avoid double counting the cell?
                    // Let's derive properly.

                    // The operation toggles all (x, y) with x >= i, y in [j - (x - i), j + (x - i)].
                    // This is a triangle. We can represent it using two diagonal difference arrays
                    // and one vertical difference array.
                    // For each row x, the range is [L, R] where L = j - (x - i), R = j + (x - i).
                    // Notice L = (i - j) + x, R = (i + j) - x? No.
                    // L = j - x + i = (i + j) - x? Actually L = j - (x - i) = j - x + i = (i + j) - x.
                    // R = j + x - i = (j - i) + x.
                    // So the left boundary is on diagonal with constant (i + j), right boundary on diagonal with constant (j - i).
                    // But we need to toggle the interior. We can use:
                    // - add 1 to diag1 at (i, j) with index (i - j + n) -> this toggles all cells on that diagonal (where x - y = i - j) for x >= i.
                    // - add 1 to diag2 at (i, j+1) with index (i + j + 1) -> this toggles all cells on that anti-diagonal (where x + y = i + j + 1) for x >= i.
                    // - add 1 to a vertical effect at column j? Actually, the intersection of these two diagonals toggles the cell twice.
                    // Let's test: For a cell (x, y) in the triangle, we have x >= i, and j - (x - i) <= y <= j + (x - i).
                    // This is equivalent to: x - y <= i - j? No.
                    // Let's use the standard method from similar problems (e.g., "triangular updates").
                    // We can maintain:
                    // diag1[d] = effect from operations whose left boundary is on diagonal d.
                    // diag2[d] = effect from operations whose right boundary is on anti-diagonal d.
                    // row_effect[j] = effect from operations that affect column j vertically? 
                    // Actually, we can process row by row and maintain the current effect on each cell.
                    // The effect from previous rows on cell (i, j) comes from:
                    // - operations that started at some (r, c) with r < i and c <= j and the triangle covers (i, j).
                    // The condition for (r, c) to affect (i, j) is: i - r >= |j - c|.
                    // This is equivalent to: c - (i - r) <= j <= c + (i - r).
                    // So j >= c - (i - r) and j <= c + (i - r).
                    // Rearranging: c - j <= i - r and j - c <= i - r.
                    // So i - r >= max(c - j, j - c) = |j - c|.
                    // This is symmetric.
                    // We can think of it as: an operation at (r, c) adds 1 to all cells (i, j) with i >= r and |j - c| <= i - r.
                    // This is a cone. We can maintain two arrays:
                    // add_left[d] = number of operations that affect the left diagonal d (where x - y = d).
                    // add_right[d] = number of operations that affect the right diagonal d (where x + y = d).
                    // But we need to subtract the overlap.
                    // Actually, the effect on cell (i, j) is the sum of operations whose cone covers it.
                    // We can compute it by maintaining a 2D difference array, but that's too slow.
                    // Instead, we can maintain for each row the effect using prefix sums of diag1 and diag2.
                    // Let's define:
                    // diag1[d] = XOR sum of operations that started on diagonal d (x - y = d) at some row <= current.
                    // diag2[d] = XOR sum of operations that started on anti-diagonal d (x + y = d) at some row <= current.
                    // But an operation at (r, c) affects cell (i, j) if i >= r and |j - c| <= i - r.
                    // This is equivalent to: (i - j) >= (r - c) and (i + j) >= (r + c)? No.
                    // Let's check: j >= c - (i - r) => i + j >= r + c.
                    // And j <= c + (i - r) => i - j >= r - c.
                    // So the condition is: i + j >= r + c AND i - j >= r - c.
                    // Also i >= r.
                    // So if we process rows in increasing order, for a cell (i, j), the operations that affect it are those with r <= i, r + c <= i + j, r - c <= i - j.
                    // This is a 3D condition. But we can maintain two Fenwick trees? Not needed.
                    // Notice that if we process row by row, and for each row we process columns from left to right,
                    // we can maintain the effect using two running XORs: one for diag1 and one for diag2.
                    // Let's define:
                    // diag1[d] = XOR of operations that started on diagonal d and whose effect has reached the current row.
                    // But the effect on a cell (i, j) from an operation at (r, c) is active if i >= r and i - r >= |j - c|.
                    // This means that for a fixed operation, as we go down rows, the range of columns it affects expands by 1 each row.
                    // So we can maintain an array `left` and `right` that represent the boundaries of active operations.
                    // Specifically, when we apply an operation at (i, j), we can:
                    // - toggle a `left` effect at diagonal (i - j) starting from row i.
                    // - toggle a `right` effect at diagonal (i + j) starting from row i.
                    // Then for a cell (i, j), the total effect is the XOR of `left` on diagonal (i - j) and `right` on diagonal (i + j), but we must be careful about double counting.
                    // Actually, the operation toggles a triangle. If we just XOR `left` and `right`, the cell at the intersection of the two boundaries (the tip) gets toggled twice, which cancels. But the tip is the starting cell, which should be toggled once. So we need an additional vertical effect to fix the tip and the interior.
                    // Standard solution from similar problems (e.g., CF 1732C2? No, this is a known problem: "AquaMoon and the Matrix" or similar).
                    // Let's search memory: There is a problem "AquaMoon and the Matrix" where you need to make all zeros using operations that invert a triangle below a cell. The solution uses two diagonal prefix sums and one vertical prefix sum.
                    // Specifically, we maintain:
                    // - diag1: effect from left diagonals (x - y)
                    // - diag2: effect from right diagonals (x + y)
                    // - col_effect: effect that goes straight down a column.
                    // When we apply operation at (i, j):
                    //   diag1[i - j + n] ^= 1
                    //   diag2[i + j + 1] ^= 1   // note: +1 to shift the right boundary
                    //   col_effect[j] ^= 1? No, we need to fix the double count.
                    // Let's derive:
                    // Suppose we want to add 1 to all cells (x, y) with x >= i, j - (x - i) <= y <= j + (x - i).
                    // We can do:
                    //   add 1 to diag1 at (i, j) -> this adds 1 to all cells with x - y = i - j and x >= i.
                    //   add 1 to diag2 at (i, j+1) -> this adds 1 to all cells with x + y = i + j + 1 and x >= i.
                    //   subtract 1 from col_effect at (i, j)? Not exactly.
                    // Consider a cell (x, y) in the triangle. It satisfies x - y >= i - j? Actually, from j - (x - i) <= y, we get x - y <= i - j + 2(x - i)? No.
                    // Let's test with a small example.
                    // Operation at (0, 0) on a 3x3 grid:
                    // Cells affected: (0,0), (1,0), (1,1), (2,0), (2,1), (2,2).
                    // If we toggle diag1 at (0,0) (x-y=0): toggles (0,0), (1,1), (2,2).
                    // Toggle diag2 at (0,1) (x+y=1): toggles (0,1), (1,0), (2,-1) invalid.
                    // We want (0,0) toggled once, but here it's toggled only by diag1. (1,0) toggled by diag2. (1,1) toggled by diag1. (2,0) not toggled? Wait, (2,0) is in triangle but x+y=2, not 1. So diag2 at (0,1) doesn't reach (2,0). So this doesn't work.
                    // We need a different approach.

                    // Let's use the method from the hints: "The effect of the preceding rows on the current row can be expressed as prefix sums."
                    // "What effect do the lines with slope 1 and -1 from the preceding rows have on the current row?"
                    // "How can you quickly calculate the effect of the preceding rows on the current row?"
                    // This suggests we can maintain for each row the net effect from all previous operations using two arrays that we update as we go down.
                    // Let's define:
                    // For each row i, we want to know the XOR effect on each cell (i, j) from all operations performed on rows < i.
                    // An operation at (r, c) affects cell (i, j) iff i - r >= |j - c|.
                    // This is equivalent to: c - (i - r) <= j <= c + (i - r).
                    // So for a fixed operation, as we go down one row, the range of affected columns expands by 1 on both sides.
                    // We can maintain an array `effect` of size n, which is the XOR effect on the current row's cells from all previous operations.
                    // When we move from row i-1 to row i, the effect on row i from an operation at (r, c) is the effect on row i-1, but expanded left and right by 1.
                    // So we can update `effect` by shifting left and right? But we also need to add new operations from row i-1.
                    // Actually, we process row by row. For each row i, we first compute the effect from all operations above. Then we decide if we need to perform operations on this row to fix 1s. Then we update the effect for the next row.
                    // Let `cur_effect[j]` be the XOR effect on cell (i, j) from operations in rows < i.
                    // Initially, for row 0, cur_effect is all 0.
                    // When we move to row i+1, the effect from an operation at (r, c) on row i+1 is the same as on row i, but expanded: it affects columns from c - (i+1 - r) to c + (i+1 - r). This is exactly the effect on row i expanded left and right by 1.
                    // So if we have the effect array for row i, we can compute the effect for row i+1 by taking the effect array for row i, and for each j, the new effect at j is the XOR of effect[i][j-1], effect[i][j], effect[i][j+1]? Not exactly, because the expansion is not a simple blur; it's a range expansion.
                    // Actually, if an operation affects a contiguous segment [L, R] on row i, on row i+1 it affects [L-1, R+1]. So the effect on row i+1 is the effect on row i, but shifted left and right and OR'd? Since it's XOR, we can't just shift.
                    // But we can maintain the effect using two arrays: `diag1` and `diag2` that represent the boundaries of the affected segments.
                    // Consider the effect on row i as a binary array. The operations are such that the affected segments are always of the form [c - d, c + d] where d = i - r. These segments are symmetric around c.
                    // The union (XOR) of such segments can be represented by toggling the start and end+1 of each segment. But since segments expand, we can maintain the boundaries dynamically.
                    // Let's think of the effect on row i as being built from two types of contributions: left boundaries and right boundaries.
                    // For each operation at (r, c), on row i it contributes a segment [c - (i-r), c + (i-r)].
                    // The left boundary is L = c - (i-r) = (c - r) + i? No: L = c - i + r = (r + c) - i.
                    // The right boundary is R = c + i - r = (c - r) + i.
                    // So L depends on (r + c) and i, R depends on (c - r) and i.
                    // Notice that L = (r + c) - i, R = (c - r) + i.
                    // So for a fixed operation, as i increases, L decreases by 1 each row, R increases by 1 each row.
                    // This means the left boundary moves left along the anti-diagonal (r + c) constant? Actually, L = (r + c) - i => i + L = r + c. So the left boundary lies on the anti-diagonal i + j = r + c.
                    // The right boundary R = (c - r) + i => R - i = c - r => i - R = r - c. So the right boundary lies on the diagonal i - j = r - c.
                    // Therefore, the operation toggles all cells between these two boundaries on each row.
                    // So the effect on row i is the XOR of all operations whose left boundary is <= j and right boundary is >= j.
                    // This is equivalent to: for each cell (i, j), the effect is the XOR of:
                    // - all operations with r + c <= i + j (left boundary condition)
                    // - all operations with r - c <= i - j (right boundary condition)
                    // But we must be careful: an operation affects (i, j) iff BOTH conditions hold (and also r <= i, but that's implied if both hold? Not necessarily: if r > i, then r + c > i + j? Not always. We need r <= i.)
                    // Actually, the condition is: i >= r, i + j >= r + c, i - j >= r - c.
                    // If i >= r, then i + j >= r + c and i - j >= r - c are exactly the segment conditions.
                    // So we can process row by row, and maintain two Fenwick trees or prefix sums over the diagonals.
                    // Since we only need XOR, we can maintain two arrays:
                    // `diag1[x]` = XOR of operations with r - c = x.
                    // `diag2[x]` = XOR of operations with r + c = x.
                    // But we need to only include operations with r <= i.
                    // As we process row i, we can add operations from row i to these arrays.
                    // Then for cell (i, j), the effect from operations with r <= i is:
                    // XOR of diag1[x] for x <= i - j? No, condition is r - c <= i - j. But diag1[x] stores operations with r - c = x. However, we also need r <= i. If r - c <= i - j, it's possible that r > i? For example, r = i+1, c = j, then r - c = i+1 - j > i - j, so it's not included. If r - c <= i - j, could r be > i? Suppose r = i+1, then r - c <= i - j => i+1 - c <= i - j => c >= j+1. Then r + c >= i+1 + j+1 = i+j+2 > i+j, so the other condition fails. So if both conditions hold, r <= i is automatically satisfied? Let's check: i + j >= r + c and i - j >= r - c. Adding: 2i >= 2r => i >= r. Yes! So the two conditions imply i >= r. Therefore, we don't need to separately check r <= i.
                    // So the effect on (i, j) is the XOR of all operations with r + c <= i + j and r - c <= i - j.
                    // This is a 2D range query on (r+c, r-c). But we can process row by row and maintain prefix XORs on the diagonals.
                    // However, we need to compute this for each cell efficiently.
                    // Notice that as we move across row i from left to right, i + j increases, i - j decreases.
                    // We can maintain two running XORs: one for diag1 (r - c) and one for diag2 (r + c).
                    // Let's define:
                    // For a fixed row i, we process columns j from 0 to n-1.
                    // We maintain a variable `cur` that represents the XOR effect on the current cell from all operations with r <= i.
                    // How does `cur` change when we move from j to j+1?
                    // The condition for (i, j+1) vs (i, j):
                    // - i + (j+1) > i + j, so we need to include operations with r + c = i + j + 1.
                    // - i - (j+1) < i - j, so we need to exclude operations with r - c = i - j? Actually, condition is r - c <= i - (j+1). So operations with r - c = i - j no longer satisfy the condition. So we need to XOR them out.
                    // Therefore, we can maintain `cur` as we move j:
                    // cur ^= diag2[i + j + 1] ^ diag1[i - j]? Wait, let's derive carefully.

                    // Let S be the set of operations with r <= i. For cell (i, j), effect = XOR of op in S with r+c <= i+j and r-c <= i-j.
                    // Let's define two arrays:
                    // `right_sum[x]` = XOR of operations with r+c = x.
                    // `left_sum[x]` = XOR of operations with r-c = x.
                    // But we need to combine them with AND condition. This is not simply the XOR of both, because an operation must satisfy both. If we just XOR right_sum and left_sum, we might double count or include operations that satisfy only one.
                    // Actually, the condition is a conjunction. The set of operations that affect (i, j) is the intersection of two sets: {op | r+c <= i+j} and {op | r-c <= i-j}.
                    // The XOR of the intersection is not easily computed from the XOR of the individual sets unless we have more information.
                    // However, we can use the fact that we process row by row and maintain the effect using difference arrays on the current row.
                    // Let's go back to the idea of maintaining the effect array for the current row and updating it for the next row.
                    // Suppose we have an array `effect` of length n, initially all 0.
                    // When we perform an operation at (i, j), we want to update `effect` for future rows.
                    // The operation at (i, j) will affect row i+1: columns j-1 to j+1 (if valid).
                    // Row i+2: columns j-2 to j+2, etc.
                    // So we can maintain two arrays `add_left` and `add_right` that represent the boundaries of the expanding segments.
                    // Specifically, we can maintain:
                    // `diag1[d]` = XOR of operations that started on diagonal d (i - j = d). For each such operation, on row x, it contributes a right boundary at column j + (x - i) = (x) - (i - j) = x - d. So the right boundary is on column x - d.
                    // `diag2[d]` = XOR of operations that started on anti-diagonal d (i + j = d). For each such operation, on row x, it contributes a left boundary at column j - (x - i) = (i + j) - x = d - x.
                    // So on row x, the effect from all operations is the XOR of:
                    // - for each diag1[d], it toggles all columns >= x - d? Actually, the right boundary means the segment ends at x - d. So it toggles columns <= x - d? Wait: the segment is [L, R]. R = j + (x - i) = x - (i - j) = x - d. So the operation affects columns up to x - d. So it toggles all columns from some L to x - d.
                    // Similarly, the left boundary L = j - (x - i) = (i + j) - x = d - x. So it toggles columns from d - x to R.
                    // So the effect on column y of row x is the XOR of all operations with d - x <= y <= x - d'? This is getting messy.

                    // Let's use the standard solution from the hints: dynamic programming / prefix sums.
                    // We can maintain three arrays for the current row:
                    // `col_effect[j]` = vertical effect from operations that started in column j.
                    // `diag1_effect[j]` = effect from left diagonals.
                    // `diag2_effect[j]` = effect from right diagonals.
                    // Actually, many solutions for this problem (CF 1730C? No, this is Codeforces Round #1730? Let's think: problem name might be "AquaMoon and the Matrix" from a recent contest.)
                    // I recall a solution: we process from top to bottom. We maintain three arrays: `left`, `right`, `down`. 
                    // `left[i]` and `right[i]` are for diagonals, `down[i]` is for columns.
                    // When we see a 1 at (i, j) that needs flipping, we do:
                    //   ans++
                    //   down[j] ^= 1
                    //   left[i - j + n] ^= 1
                    //   right[i + j] ^= 1
                    // Then for the next row, we update the effects:
                    //   new_down[j] = down[j] ^ left[i - j + n] ^ right[i + j]? No.
                    // Let's derive the effect on cell (i, j) from all previous operations.
                    // Suppose we have operations at various (r, c). The effect on (i, j) is the XOR of all operations with r <= i and |j - c| <= i - r.
                    // We can rewrite this as: there exists an operation at (r, c) such that r - c >= i - j? No.
                    // Let's use the idea of "difference arrays on the grid".
                    // Consider the operation as adding 1 to a triangle. We can use a 2D difference array, but we need to do it efficiently.
                    // The triangle can be represented by:
                    // - add 1 to cell (i, j)
                    // - for the next row, we want to add 1 to (i+1, j-1) and (i+1, j+1) and subtract 1 from (i+1, j)? Not exactly.
                    // Actually, if we have a triangle starting at (i, j), we can propagate it downwards by:
                    //   effect[i+1][j-1] ^= 1
                    //   effect[i+1][j+1] ^= 1
                    //   effect[i+1][j] ^= 1? No, that would be for a different shape.
                    // Let's test: operation at (0,0). We want to affect (1,0) and (1,1). If we set effect[1][-1] (ignored), effect[1][1], and effect[1][0]? That doesn't match.
                    // Another way: The triangle is the set of cells (x, y) with x >= i, y >= j - (x - i), y <= j + (x - i).
                    // This is equivalent to: y - x >= j - i and y + x <= j + i? No: y >= j - x + i => y + x >= i + j. And y <= j + x - i => y - x <= j - i.
                    // So the conditions are: y + x >= i + j and y - x <= j - i.
                    // Also x >= i.
                    // So the triangle is the intersection of three half-planes: x >= i, y + x >= i + j, y - x <= j - i.
                    // This is a cone. We can maintain the effect using three 2D Fenwick trees? Too slow.
                    // But we only need to process row by row. For a fixed row x, the conditions become:
                    // y >= i + j - x, y <= j - i + x, and x >= i (already true if we process rows in order).
                    // So on row x, the affected columns are from L = i + j - x to R = j - i + x.
                    // Notice L = (i + j) - x, R = (j - i) + x.
                    // So L depends only on (i+j) and x, R depends only on (j-i) and x.
                    // Therefore, if we maintain two arrays:
                    // `add_left[d]` = XOR of operations with i + j = d.
                    // `add_right[d]` = XOR of operations with j - i = d.
                    // Then on row x, the effect on column y is the XOR of:
                    // - all add_left[d] with d <= x + y? Because L = d - x <= y => d <= x + y.
                    // - all add_right[d] with d >= y - x? Because R = d + x >= y => d >= y - x.
                    // But again, we need the intersection: an operation must satisfy BOTH.
                    // However, notice that if an operation satisfies both, it contributes to both conditions. If we just XOR the two contributions, we might double count the operation? Actually, if we define the effect as the XOR of add_left and add_right, an operation that satisfies both would be XORed twice, which cancels out. So we need to add it back? Or we can use a different representation.

                    // Let's look for a known solution. I remember a problem "Matrix and Shifts" or "AquaMoon and the Matrix" from Codeforces Round #1730 (Div. 2) problem D? No, that's different.
                    // Searching memory: There is a problem "AquaMoon and the Matrix" where you have to make all zeros using operations that invert a triangle below a cell. The solution uses three arrays: `l`, `r`, `c` (left diagonal, right diagonal, column). 
                    // For each row i from 0 to n-1:
                    //   maintain `cur` array of size n, initially 0.
                    //   for j from 0 to n-1:
                    //     cur[j] ^= l[j] ^ r[j] ^ c[j]? No.
                    // Let's derive from scratch with the hint: "The first row is fixed, the second row is determined by the first row, the third row is determined by the first two rows, and so on."
                    // This suggests a greedy approach: process rows from top to bottom. For each cell, if it's 1 after considering the effects from above, we must perform an operation here. Because any operation performed later (on lower rows) cannot affect this cell. So the solution is unique: we must perform an operation exactly on those cells that are 1 after applying the effects from all operations above.
                    // So the problem reduces to: given the operations performed on rows above, compute the effect on the current cell, and if it's 1, perform an operation here and update the effect for future rows.
                    // So we need an efficient way to compute the effect on (i, j) from all operations performed on rows < i.
                    // Let's define the state after processing row i-1. We have performed some operations. We need to know for each column j, what is the XOR effect from those operations on cell (i, j).
                    // Let `effect[j]` be this XOR effect for row i.
                    // How do we compute `effect` for row i from the operations?
                    // An operation at (r, c) affects row i on columns [c - (i-r), c + (i-r)].
                    // So if we know the operations, we could add their contributions. But we need to do it in O(n) per row.
                    // Notice that the effect on row i can be derived from the effect on row i-1 and the operations performed on row i-1.
                    // Let `prev_effect` be the effect on row i-1 from operations on rows < i-1.
                    // Then the effect on row i from those same operations is the "expansion" of `prev_effect`: for each operation, the segment expands by 1 on both sides.
                    // So if we have `prev_effect` as a binary array, the new effect `cur_effect` from old operations is:
                    //   cur_effect[j] = prev_effect[j-1] ^ prev_effect[j] ^ prev_effect[j+1]? No, that would be if the effect was a single cell. But the effect is a segment. The expansion of a segment [L, R] to [L-1, R+1] means that the new effect is the old effect shifted left OR shifted right? Actually, if a cell was affected, its neighbors might now be affected. But the XOR sum of segments doesn't simply expand by OR.
                    // However, we can represent the effect array as the XOR of several segments. The expansion of each segment can be handled by maintaining the boundaries.
                    // Let's maintain two arrays: `left_bound` and `right_bound` that represent the boundaries of the active segments.
                    // For each operation at (r, c), we can think of it as adding a left boundary at (r, c) that moves left one column per row, and a right boundary at (r, c) that moves right one column per row.
                    // The effect on a cell (i, j) is the XOR of the number of left boundaries that have passed j from the left, and right boundaries that have passed j from the right? More precisely, a segment [L, R] is active on row i. The cell j is inside iff L <= j <= R.
                    // This is equivalent to: the number of left boundaries <= j minus the number of right boundaries < j? But since it's XOR, we can't just count.
                    // However, we can use the fact that the operations are applied in a greedy manner. We can maintain the effect using three running XORs:
                    // - `diag1`: effect from left boundaries (which move left along anti-diagonals)
                    // - `diag2`: effect from right boundaries (which move right along diagonals)
                    // - `col`: effect that goes straight down? 
                    // Let's try to simulate the expansion.
