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

        // We maintain three 2D difference arrays to track the effect of operations
        // on lower rows. Since we process row by row from top to bottom, we only need
        // to keep track of the current row's state.
        // We use vectors of size n+2 to avoid boundary checks.
        vector<int> diag1(n + 2, 0); // main diagonal effect (x - y constant)
        vector<int> diag2(n + 2, 0); // anti diagonal effect (x + y constant)
        vector<int> col(n + 2, 0);   // vertical effect

        int ans = 0;
        for (int i = 0; i < n; ++i) {
            // current row's flip state from previous operations
            vector<int> cur(n + 2, 0);
            // propagate effects from previous rows to current row
            // We'll compute prefix sums of effects along the row.
            int running = 0;
            for (int j = 0; j < n; ++j) {
                // add effects from diag1, diag2, col at this cell
                running += diag1[j] + diag2[j] + col[j];
                cur[j] = running;
                // shift diag1 and diag2 for next column
                // diag1 moves to j+1 (since x-y constant, as j increases, diag1 index increases)
                // diag2 moves to j-1 (since x+y constant, as j increases, diag2 index decreases)
                // We'll handle this by updating the arrays for the next row.
            }

            // Now decide operations on this row
            for (int j = 0; j < n; ++j) {
                int val = (grid[i][j] - '0') ^ (cur[j] & 1);
                if (val == 1) {
                    ++ans;
                    // apply operation at (i, j)
                    // This affects cells (x, y) with x > i and x - i >= |y - j|
                    // Equivalent to: for all rows below, a range of columns is flipped.
                    // We can represent this with three difference arrays:
                    // col[j] adds to all rows below at column j
                    // diag1 adds to cells with x - y = i - j, starting from row i+1
                    // diag2 adds to cells with x + y = i + j, starting from row i+1
                    // But careful: the operation flips a triangular region.
                    // The standard way: we maintain three effects:
                    // - a vertical effect that flips column j for all rows below
                    // - a diagonal effect (x - y = i - j) that flips from row i+1, column j+1 onwards
                    // - a diagonal effect (x + y = i + j) that flips from row i+1, column j-1 onwards
                    // However, the region is x - i >= |y - j|, which is a cone.
                    // The difference arrays need to be updated such that when we move to next row,
                    // the effects spread left and right by one.
                    // We can do this by updating diag1 at j+1, diag2 at j-1, and col at j.
                    // But we also need to cancel the double counting at the center.
                    // Actually, the standard solution uses:
                    // col[j] ^= 1;
                    // diag1[j+1] ^= 1;
                    // diag2[j-1] ^= 1;
                    // But we must be careful with indices.
                    // Let's use 1-indexed for difference arrays to avoid negative indices.
                }
            }

            // After processing row i, we need to update the difference arrays for the next row.
            // The effects propagate: diag1 shifts right, diag2 shifts left, col stays.
            // But we also need to combine them correctly.
            // We'll do this update after deciding operations, but before moving to next row.
            // Actually, the operations we just applied affect rows > i, so we update the arrays now.
            // We'll maintain the arrays for the next row's state.
            // To avoid complex logic, we can recompute the arrays for the next row based on current row's operations.
        }

        // The above is incomplete. Let's implement the known efficient solution.
        // We'll reset and do it properly.
    }
    return 0;
}
