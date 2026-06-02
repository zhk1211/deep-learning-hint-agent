#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<string> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }
    
    // We'll maintain the effect of operations from above rows.
    // For each cell, we need to know if it's currently flipped.
    // We can process row by row from top to bottom.
    // For each row, we maintain an array of prefix sums of operations
    // that affect the current row.
    
    vector<int> op(n, 0); // op[j] = 1 if we perform operation at (current_row, j)
    // But we need to know the cumulative effect from previous rows.
    // The operation at (r, c) affects (i, j) if i > r and i - r >= |j - c|.
    // This is like a "cone" shape downwards.
    // We can maintain two diagonal difference arrays for the left and right diagonals.
    
    // Let's process rows from 0 to n-1.
    // We'll maintain:
    // diag1[k] = number of operations that affect the current cell via left diagonal (j - i)
    // diag2[k] = number of operations that affect the current cell via right diagonal (j + i)
    // But the condition is i - r >= |j - c|, which means the operation affects a range of columns
    // in row i: from c - (i - r) to c + (i - r).
    // So for a fixed row i, the effect from an operation at (r, c) is a range [c - (i - r), c + (i - r)].
    // This is equivalent to adding 1 to a range in row i.
    // We can maintain the effect using difference arrays for each row, but we need to do it efficiently.
    
    // Alternative: process row by row, and maintain the "active" operations from previous rows.
    // When we move from row i-1 to row i, the range of an operation at (r, c) expands by 1 on both sides.
    // So we can maintain a difference array for the current row that is updated from the previous row's difference array.
    
    // Let diff[j] be the difference array for the current row's flip state.
    // Initially, for row 0, diff is all zeros.
    // When we process row i, we first update diff from the previous row's diff:
    // The effect from previous operations expands: if in row i-1 the effect was on range [L, R], in row i it becomes [L-1, R+1].
    // So we can shift the difference array: new_diff[j] = old_diff[j+1] (for left boundary) and old_diff[j-1] (for right boundary)?
    // Actually, we can maintain two arrays: left_diff and right_diff.
    // Let's think: An operation at (r, c) adds 1 to range [c - (i - r), c + (i - r)] in row i.
    // The difference array for this range: add 1 at L, subtract 1 at R+1.
    // As i increases, L decreases by 1, R increases by 1.
    // So if we have a difference array d for row i-1, then for row i, the left boundaries shift left by 1, right boundaries shift right by 1.
    // This means d[j] in row i-1 corresponds to d[j+1] for the left part? Not exactly.
    
    // Better: maintain two arrays: add_left[i][j] and add_right[i][j]? Too much memory.
    // We can maintain a 1D array for the current row's difference, and update it when moving to next row.
    // Let cur_diff be the difference array for the current row's flip state (before considering new operations on this row).
    // When we move to next row, the effect from all previous operations expands:
    // For each operation, the left boundary moves left by 1, right boundary moves right by 1.
    // So the difference array for the next row can be obtained by:
    // new_diff[j] = cur_diff[j] + (something from expansion).
    // Actually, if in row i the effect is a range [L, R], the difference array has +1 at L, -1 at R+1.
    // In row i+1, the range is [L-1, R+1], so difference array has +1 at L-1, -1 at R+2.
    // So the +1 moves left by 1, the -1 moves right by 1.
    // Therefore, we can maintain two separate difference arrays: one for the +1s (left boundaries) and one for the -1s (right boundaries).
    // Let left_diff[j] = number of operations whose left boundary is at j in the current row.
    // Let right_diff[j] = number of operations whose right boundary is at j in the current row.
    // Then the flip state at column j is prefix sum of (left_diff - right_diff) up to j.
    // When moving to next row, left_diff shifts left by 1: new_left_diff[j] = left_diff[j+1] (with j+1 <= n-1, and new left boundaries can appear at j=0 from operations that had left boundary at 0? Actually, if left boundary was at 0, it becomes -1, which is out of bounds, but the range just extends to the left edge, so we can clamp at 0. However, the operation's effect is only within the matrix, so if L-1 < 0, it just starts at 0. So we need to accumulate left boundaries that go off the left edge? They just stay at 0? Actually, if L becomes negative, the range starts at 0, so the +1 should be at 0. So we need to add the count of left boundaries that were at 0 to the new left_diff[0] as well? Wait, if left boundary was at 0 in row i, in row i+1 it would be at -1, but since we clamp, the range starts at 0, so the +1 should still be at 0. So left_diff[0] in row i+1 gets contributions from left_diff[1] (shifted) plus left_diff[0] (clamped). Similarly, right_diff: if right boundary was at n-1, it becomes n, which is out of bounds, so the -1 should be at n (which is beyond the last column, so we can ignore it or keep it at n). Actually, the range ends at min(n-1, R+1). So if R+1 >= n, the -1 is not needed because the range goes to the end. So we can maintain right_diff up to n, and when shifting right, right_diff[j] = right_diff[j-1] for j>=1, and right_diff[n] accumulates right_diff[n] + right_diff[n-1]? Let's formalize.
    
    // Let L[i][j] = number of operations from rows < i whose left boundary in row i is exactly j.
    // Let R[i][j] = number of operations from rows < i whose right boundary in row i is exactly j.
    // Then the flip state at (i, j) is sum_{k=0}^{j} (L[i][k] - R[i][k]).
    // When moving to row i+1:
    // For each operation, left boundary moves left by 1, but clamped to 0.
    // So L[i+1][j] = L[i][j+1] for j >= 0, except j=0 also gets L[i][0] (since those would move to -1 but stay at 0).
    // So L[i+1][0] = L[i][0] + L[i][1], and for j > 0: L[i+1][j] = L[i][j+1].
    // Similarly, right boundary moves right by 1, clamped to n (meaning if it goes beyond n-1, the -1 is at n, which doesn't affect columns 0..n-1).
    // So R[i+1][j] = R[i][j-1] for j >= 1, and R[i+1][n] gets R[i][n] + R[i][n-1] (but we only care up to n-1, so we can just maintain up to n).
    // Actually, we only need R up to n-1 for the prefix sum, but the -1 at n doesn't affect columns 0..n-1. So we can just drop R[i][n] and when shifting, R[i+1][j] = R[i][j-1] for j>=1, and R[i+1][n] accumulates the ones that go beyond.
    // But we can simplify: we don't need to store L and R for all rows, just the current row's L and R arrays, and update them.
    
    // Additionally, we can perform new operations on the current row. An operation at (i, c) adds 1 to the range [c, c] in row i? Wait, the operation at (i, j) inverts (i, j) and all cells (x, y) with x > i and x - i >= |y - j|. So for row i itself, it only inverts (i, j). For rows below, it inverts a range. So if we perform an operation at (i, c), it adds 1 to the flip state at (i, c) (and also affects future rows). In terms of our L and R for future rows, this operation will have left boundary = c - (x - i) and right boundary = c + (x - i) for row x > i. So for the next row i+1, its left boundary is c-1, right boundary is c+1. So we can add this operation to L and R for the next row: L[i+1][c-1] += 1 (if c-1 >= 0 else L[i+1][0] += 1), and R[i+1][c+1] += 1 (if c+1 < n else R[i+1][n] += 1). But we are processing row by row, so when we are at row i, we first compute the current flip state using L and R from previous rows, then we decide whether to perform an operation at each cell to make it 0. Then we update L and R for the next row based on the operations we just performed, and also shift the existing L and R from previous rows.
    
    // Let's maintain:
    // vector<int> L(n, 0); // left boundaries for current row
    // vector<int> R(n+1, 0); // right boundaries for current row (index n is for the -1 that goes beyond)
    // Actually, we need R up to n because when we shift, R[j] = R[j-1], and R[n] accumulates.
    
    // Algorithm:
    // Initialize L and R as zeros.
    // For each row i from 0 to n-1:
    //   Compute current flip state for each column j:
    //     We can maintain a running sum: cur = 0.
    //     For j from 0 to n-1:
    //       cur += L[j] - R[j];
    //       flip = cur % 2;
    //       desired = a[i][j] - '0';
    //       if (flip ^ desired) == 1: // need to flip this cell
    //         perform operation at (i, j):
    //           ans++;
    //           // This operation affects current row: it flips (i, j). But we are already processing it, so we can just record that we did it.
    //           // For future rows, it will add to L and R.
    //           // For the next row, left boundary = j-1 (clamped to 0), right boundary = j+1 (clamped to n).
    //           // We can update L and R for the next row immediately, but we need to be careful not to affect the current row's computation.
    //           // So we can store the updates in a temporary array for the next row.
    //   After finishing the row, we update L and R for the next row:
    //     new_L[0] = L[0] + L[1] + (operations we added that have left boundary at 0 for next row)
    //     new_L[j] = L[j+1] + (operations with left boundary j)
    //     new_R[j] = R[j-1] + (operations with right boundary j) for j>=1
    //     new_R[n] = R[n] + R[n-1] + (operations with right boundary n)
    //   Then set L = new_L, R = new_R.
    
    // We can do this with O(n^2) total time.
    
    vector<int> L(n, 0), R(n+1, 0);
    int ans = 0;
    
    for (int i = 0; i < n; ++i) {
        vector<int> new_L(n, 0), new_R(n+1, 0);
        
        // First, shift the existing L and R to the next row (they come from operations in rows < i)
        // new_L[0] gets L[0] + L[1] (since L[0] stays at 0, L[1] moves to 0)
        if (n > 0) {
            new_L[0] += L[0];
            if (n > 1) new_L[0] += L[1];
        }
        for (int j = 1; j < n; ++j) {
            if (j+1 < n) new_L[j] += L[j+1];
        }
        // new_R: R[j] moves to R[j+1]? Actually, right boundary moves right by 1: R[i][j] becomes R[i+1][j+1].
        // So new_R[j] = R[j-1] for j>=1.
        for (int j = 1; j <= n; ++j) {
            new_R[j] += R[j-1];
        }
        // new_R[n] also gets R[n] (which stays at n? Actually R[n] means -1 at n, moving right stays at n)
        new_R[n] += R[n];
        
        // Now compute current row's flip state and decide operations
        int cur = 0;
        for (int j = 0; j < n; ++j) {
            cur += L[j] - R[j];
            int flip = cur & 1;
            int val = a[i][j] - '0';
            if (flip != val) {
                // need to perform operation at (i, j)
                ans++;
                // This operation affects next row:
                // left boundary = j-1 (clamped to 0)
                int left = max(0, j-1);
                new_L[left]++;
                // right boundary = j+1 (clamped to n)
                int right = min(n, j+1);
                new_R[right]++;
                // Also, this operation flips the current cell, so we can update cur manually? 
                // Actually, we are processing left to right, and this operation flips (i, j). 
                // But our flip state already includes the effect of previous operations. 
                // The operation at (i, j) does NOT affect other cells in the same row. 
                // So we don't need to update cur for subsequent columns in the same row.
                // However, we must update the current cell's flip state? We already decided to flip it, so it becomes 0. 
                // For the next columns, the operation at (i, j) does nothing to them. So cur remains unchanged.
            }
        }
        
        L = move(new_L);
        R = move(new_R);
    }
    
    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}
