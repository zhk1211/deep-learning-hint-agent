// Hint2
#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<int> a(n);
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }
        
        // Check basic necessary condition: sum of a_i must be n
        long long sum_a = 0;
        for (int x : a) sum_a += x;
        if (sum_a != n) {
            cout << "0\n";
            continue;
        }
        
        // We'll build the answer by considering the two diagonals
        // Let's compute the positions of black cells on the two "max" conditions
        // Condition 2: max(x_i, y_i) = k for each k from 1..n
        // Condition 3: max(x_i, n+1-y_i) = k for each k from 1..n
        
        // We can think of placing black cells step by step from k=n down to 1
        // At step k, we decide the cell that satisfies max(x,y)=k and max(x,n+1-y)=k
        
        // We'll maintain available rows counts
        vector<int> row_left = a;
        
        // We'll process k from n down to 1
        // At each k, we have two "slots" to fill: one on the main diagonal (x=y) or its surroundings,
        // and one on the anti-diagonal (x+y=n+1) or its surroundings.
        // Actually, the conditions uniquely determine the cells if we think carefully.
        
        // Let's simulate the process:
        // For k from n down to 1:
        // We need to place a cell with max(x,y)=k. This cell must be in row k or column k.
        // Similarly, we need a cell with max(x, n+1-y)=k. This cell must be in row k or column n+1-k.
        // But these two cells could be the same cell if it lies on both diagonals (i.e., x=y=k or x=k, y=n+1-k etc.)
        
        // We can think of it as: at each step k, we have two "requirements": one for the main diagonal and one for the anti-diagonal.
        // They correspond to cells that are "maximal" in some sense.
        
        // Let's define arrays for rows and columns used.
        // Actually, we can use a greedy approach: 
        // For each k from n down to 1, we try to satisfy the conditions by picking cells.
        // The cell for max(x,y)=k must be either (k, y) with y <= k, or (x, k) with x <= k.
        // The cell for max(x, n+1-y)=k must be either (k, y) with n+1-y <= k => y >= n+1-k, or (x, n+1-k) with x <= k.
        
        // We can combine these: at step k, we have two "candidates" rows: row k (for both conditions potentially) and maybe other rows.
        // But we must place exactly a_i cells in row i.
        
        // Let's think differently: The conditions imply that the set of black cells is exactly the set of cells (x,y) such that
        // x <= something? Actually, from the examples and hints, we can deduce that the black cells form a "staircase" shape.
        
        // Let's try to construct the grid row by row from bottom to top.
        // We know that for each k, there is exactly one cell with max(x,y)=k. This means that if we consider the cells sorted by max(x,y),
        // they appear exactly once for each k.
        // Similarly for the other condition.
        
        // This is a known problem: the conditions essentially say that the black cells are exactly the cells of a permutation matrix
        // with some additional properties? No, because multiple cells per row are allowed.
        
        // Let's analyze the hints:
        // Hint 1: What can be said about the position of the black cell (1,1)?
        // If there is a black cell at (1,1), then max(1,1)=1, so it satisfies condition 2 for k=1.
        // Also max(1, n+1-1)=max(1,n)=n, so it would satisfy condition 3 for k=n.
        // So (1,1) is special: it covers the smallest k for condition 2 and the largest k for condition 3.
        
        // Hint 2: If you repeat the same process starting from (1,n), what can you say about the position of the black cell (1,n)?
        // (1,n) has max(1,n)=n, so it covers k=n for condition 2.
        // And max(1, n+1-n)=max(1,1)=1, so it covers k=1 for condition 3.
        
        // So the corners are paired: (1,1) and (1,n) are like "opposites" in terms of the two conditions.
        
        // This suggests that we can pair up the conditions: for each k, the cell that satisfies max(x,y)=k and the cell that satisfies max(x, n+1-y)=n+1-k might be related.
        
        // Let's define for each cell its "diagonal" value d1 = max(x,y) and "anti-diagonal" value d2 = max(x, n+1-y).
        // The conditions say that the multiset of d1 over black cells is {1,2,...,n} and similarly for d2.
        
        // Now, consider the transformation: reflect the grid horizontally? Actually, if we take a cell (x,y), its d2 is max(x, n+1-y).
        // If we replace y by n+1-y, then d2 becomes max(x, y') where y' = n+1-y. So d2 is just the d1 of the reflected cell.
        // So the conditions are symmetric under horizontal reflection.
        
        // This suggests that the set of black cells must be symmetric in some sense? Not necessarily, but the multiset of d1 and d2 are the same.
        
        // Let's try to construct the grid by placing cells in order of k from 1 to n.
        // At step k, we need to place a cell with d1 = k. This cell must be in row k or column k, and its row and column indices are <= k.
        // Similarly, we need to place a cell with d2 = k. This cell must be in row k or column n+1-k, with row <= k and column >= n+1-k.
        
        // These two cells could be the same if k = n+1-k (i.e., n is odd and k = (n+1)/2) and the cell is (k,k).
        // Otherwise, they are distinct.
        
        // Now, we also have row constraints: row i must have exactly a_i black cells.
        
        // Let's think about the process from bottom-right? Actually, consider the cell with d1 = n. It must be in row n or column n, and its coordinates <= n.
        // So it could be (n, y) for any y, or (x, n) for any x. But since max(x,y)=n, if it's (n,y), then y <= n; if it's (x,n), then x <= n.
        // Similarly, the cell with d2 = n must be in row n or column 1, with row <= n and column >= 1. So it could be (n, y) with y >= 1, or (x, 1) with x <= n.
        
        // Notice that row n appears in both conditions for k=n. So row n might contain one or two of these cells.
        
        // Let's try to determine the number of black cells in each row from the conditions.
        // For each row i, how many cells can have d1 = something? A cell in row i has d1 = max(i, y). So if y <= i, d1 = i; if y > i, d1 = y.
        // So the d1 value is at least i. Similarly, d2 = max(i, n+1-y). If n+1-y <= i => y >= n+1-i, then d2 = i; else d2 = n+1-y.
        // So d2 is at least i.
        
        // This means that for row i, the black cells in that row can only contribute to d1 values >= i and d2 values >= i.
        // Since we need exactly one cell for each d1 value from 1 to n, the cells in row i can only cover d1 values >= i.
        // Therefore, the number of cells in rows 1..i must be at most i? Actually, the cells with d1 <= i must be placed in rows <= i or columns <= i.
        // But if a cell is in row > i and column > i, its d1 > i. So all cells with d1 <= i must have row <= i or column <= i.
        // This is a known necessary condition: for each i, the number of cells in rows 1..i plus the number of cells in columns 1..i must be at least i? Not exactly.
        
        // Let's use a different approach: The conditions uniquely determine the set of black cells if we consider the "profile" of the grid.
        // Look at the example: a = [2,2,1,0,0], n=5. The black cells are (1,1), (2,2), (3,3), (2,4), (1,5).
        // Notice that these cells form a "path" from (1,1) to (1,5) going right and down? Actually, they are (1,1), (2,2), (3,3), (2,4), (1,5).
        // If we sort by row: row1: (1,1), (1,5); row2: (2,2), (2,4); row3: (3,3); row4: none; row5: none.
        // The columns used are 1,2,3,4,5. Each column has exactly one black cell? Column1: (1,1); column2: (2,2); column3: (3,3); column4: (2,4); column5: (1,5).
        // So it's a permutation matrix! Indeed, each row and each column has exactly one black cell? No, row1 has two cells, column1 has one, etc.
        // Wait, check columns: column1 has (1,1); column2 has (2,2); column3 has (3,3); column4 has (2,4); column5 has (1,5). So each column has exactly one black cell.
        // And rows: row1 has 2, row2 has 2, row3 has 1, row4 0, row5 0. So columns are a permutation, rows are given by a.
        // Is it always true that each column has exactly one black cell? Let's check condition 2: for each k, exactly one cell with max(x,y)=k.
        // If we consider columns, a cell in column j has d1 = max(x,j). So d1 >= j. For d1 to take all values 1..n exactly once, the columns must also be a permutation?
        // Not necessarily, because multiple cells can be in the same column? If two cells are in the same column j, then their d1 values are max(x1,j) and max(x2,j).
        // Since x1 and x2 are distinct rows, these max values could be different. But could they cover all 1..n? Possibly, but then some other column would have to miss some values.
        // Let's test with a different a. Suppose a = [1,1,1] for n=3. Can we have a valid grid? Let's try to construct.
        // We need cells with d1=1,2,3 and d2=1,2,3.
        // d1=1: must be (1,1) because max(x,y)=1 => x=1,y=1.
        // d2=1: max(x, 4-y)=1 => x=1, 4-y=1 => y=3. So (1,3).
        // d1=2: could be (2,1), (2,2), (1,2). But row1 already has 1 cell? a1=1, so row1 is full. So (1,2) not allowed. So (2,1) or (2,2).
        // d2=2: max(x, 4-y)=2. Options: (2,2) because max(2,2)=2; (2,1)? max(2,3)=3 not 2; (1,2)? max(1,2)=2 but row1 full. So (2,2) is forced for d2=2.
        // Then d1=2 must be (2,1) because (2,2) taken. So (2,1) for d1=2.
        // Now d1=3: remaining cell must have max(x,y)=3. Options: (3,1), (3,2), (3,3), (2,3), (1,3) but row1 full, row2 has a2=1? a2=1, so row2 full (we placed (2,1) and (2,2)? Wait, a2=1, so row2 can only have 1 cell! But we placed two cells in row2: (2,1) and (2,2). That's invalid.
        // So a=[1,1,1] is impossible. The sample says for n=2, a=[1,1] output 0. So indeed, columns must also have exactly one black cell? Let's check the sample with n=2, a=[2,0] output 1. The grid is {(1,1),(1,2)}. Columns: column1 has (1,1), column2 has (1,2). So each column has exactly one cell. Row1 has 2, row2 has 0.
        // Another sample: n=4, a=[3,1,0,0] output 2. Let's try to construct. We need 3 cells in row1, 1 in row2. Total 4 cells. Columns? Likely each column has exactly one cell.
        // So it seems that in any valid grid, each column must contain exactly one black cell. Let's prove this.
        // Consider the condition: for each k, exactly one cell with max(x,y)=k.
        // Let c_j be the number of black cells in column j.
        // For a cell in column j, its d1 = max(x,j). So the d1 values of cells in column j are at least j.
        // The multiset of d1 values is {1,2,...,n}. So the number of cells with d1 <= j is j.
        // These cells must be in columns <= j or rows <= j. But if a cell is in column > j and row > j, its d1 > j.
        // So all cells with d1 <= j are in the set of cells with column <= j or row <= j.
        // Let R_j be the number of cells in rows 1..j, C_j be the number of cells in columns 1..j.
        // Then the number of cells with d1 <= j is at most R_j + C_j, but we have to be careful about double-counting cells that are in both.
        // Actually, the cells with d1 <= j are exactly those with row <= j and column <= j? No, a cell with row <= j and column > j has d1 = column > j, so not <= j.
        // Wait: if row <= j and column > j, then max(row, column) = column > j. So d1 > j.
        // If row > j and column <= j, then max = row > j, so d1 > j.
        // So the only cells with d1 <= j are those with both row <= j and column <= j.
        // Therefore, the number of cells with d1 <= j is exactly the number of black cells in the subgrid [1..j] x [1..j].
        // But we need exactly j cells with d1 <= j (since d1 takes values 1..j exactly once).
        // So the number of black cells in the top-left j x j subgrid must be exactly j.
        // This is a known condition for permutation matrices: if we consider the cells as 1s in a matrix, the number of 1s in the top-left j x j subgrid is j.
        // This implies that the matrix is a permutation matrix if we also have exactly one 1 per row? Not necessarily: we could have multiple 1s in a row, but then the top-left subgrid condition would force something.
        // Let's check: if we have multiple 1s in a row, say row i has two cells, then for j >= i, the top-left j x j subgrid includes row i. If row i has two cells, they could both be in columns <= j, so the count in top-left j x j could exceed j? But we need exactly j.
        // In the example n=5, a=[2,2,1,0,0], the top-left 1x1 has 1 cell (1,1). 2x2 has cells (1,1), (2,2) -> 2 cells. 3x3 has (1,1),(2,2),(3,3) -> 3 cells. 4x4 has those plus? (2,4) is in row2 col4, so not in 4x4? Actually 4x4 includes columns 1..4, so (2,4) is in 4x4. So 4x4 has 4 cells: (1,1),(2,2),(3,3),(2,4). 5x5 has all 5 cells. So indeed, each top-left j x j has exactly j cells.
        // This condition is exactly the condition for a "Ferrers diagram" or a "staircase" shape? Actually, if we have a set of cells such that each top-left j x j has exactly j cells, then the set of cells forms a "permutation" in the sense that if we sort by row, the columns are non-decreasing? Not exactly.
        // But note that this condition implies that each column has at most 1 cell? Let's check: Suppose column j has two cells, at rows r1 < r2. Then for j' = j, the top-left j x j includes column j. If both rows are <= j, then the count in top-left j x j would be at least 2 from column j alone, but we need exactly j cells total. If j is large enough, it might still be possible if other columns have fewer cells. But let's test: can we have a column with 2 cells? Suppose n=3, a=[2,0,1]? Sum=3. Try to construct: we need 2 cells in row1, 1 in row3. Top-left 1x1 must have 1 cell -> (1,1). Top-left 2x2 must have 2 cells. We already have (1,1). We need one more in 2x2. It could be (1,2) or (2,1) or (2,2). But row2 has a2=0, so no cells in row2. So it must be (1,2). Then top-left 3x3 must have 3 cells. We have (1,1),(1,2). We need one more. It must be in row3 (since row1 full, row2 empty). It could be (3,1),(3,2),(3,3). But then column1 would have (1,1) and possibly (3,1) -> two cells in column1. Is that allowed? Let's check conditions: d1 values: (1,1)->1, (1,2)->2, (3,1)->3. So d1=1,2,3 covered. d2: max(x,4-y). (1,1): max(1,3)=3. (1,2): max(1,2)=2. (3,1): max(3,3)=3. But we need d2=1,2,3 exactly once. Here d2=3 appears twice, d2=1 missing. So invalid. So maybe column1 having two cells breaks d2 condition.
        // Let's analyze d2 condition similarly. For d2 = max(x, n+1-y) <= j, we need x <= j and n+1-y <= j => y >= n+1-j. So the cell must be in row <= j and column >= n+1-j. So the number of cells with d2 <= j is the number of cells in the top-right j x j subgrid (rows 1..j, columns n+1-j..n). And we need exactly j cells there.
        // So we have two conditions: for each j, the number of cells in top-left j x j is j, and the number of cells in top-right j x j is j.
        // These two conditions together might force each column to have exactly one cell. Let's see: Consider the top-left j x j and top-right j x j. Their intersection is rows 1..j, columns from n+1-j to j? That is only non-empty if n+1-j <= j => j >= (n+1)/2. For j < (n+1)/2, the two subgrids are disjoint in columns. So the total cells in rows 1..j is the sum of cells in top-left j x j and top-right j x j, which is 2j. But rows 1..j can have at most sum_{i=1}^j a_i cells. So we need sum_{i=1}^j a_i >= 2j for j < (n+1)/2? In the example n=5, j=1: sum a1=2, 2j=2. j=2: sum a1+a2=4, 2j=4. j=3: sum a1+a2+a3=5, 2j=6? But for j=3, (n+1)/2 = 3, so j=3 is the boundary. For j=3, the two subgrids overlap in columns 3..3? Actually top-left 3x3: cols 1-3. top-right 3x3: cols 3-5. Intersection is col 3. So they are not disjoint. So the condition changes.
        // This suggests that the array a must satisfy certain inequalities.
        // Let's derive the necessary and sufficient conditions on a.
        // From top-left condition: for each j, the number of cells in rows 1..j and columns 1..j is j.
        // Let b_j be the number of cells in row j that are in columns 1..j? Not exactly.
        // We can think of the cells as forming a "path" from (1,1) to (1,n) or something.
        // Actually, the conditions are exactly saying that the black cells form a "permutation" when we consider the two diagonals.
        // There is a known result: The number of such grids is 2^{something} if a is a "mountain" shape? Let's look at the sample n=4, a=[3,1,0,0] output 2. What are the two grids?
        // We need 3 cells in row1, 1 in row2. Total 4 cells. Top-left 1x1: 1 cell -> (1,1). Top-left 2x2: 2 cells -> we have (1,1), need one more in 2x2. It could be (1,2) or (2,1) or (2,2). But row2 has a2=1, so if we put (2,1) or (2,2), row2 gets its cell. If we put (1,2), row2 still needs a cell later. Let's try both.
        // Also top-right 1x1: cols 4..4, rows 1..1 -> 1 cell. That must be (1,4) because row1 must have 3 cells total, and we already have (1,1) and maybe (1,2) or not. Actually, top-right 1x1 must have exactly 1 cell, so (1,4) must be black. So row1 gets (1,4) for sure.
        // Now row1 needs 3 cells, so it needs two more besides (1,4). They must be in columns 1,2,3. We already have (1,1) from top-left 1x1. So we need one more in row1 among cols 2,3.
        // Now top-left 2x2: must have 2 cells. We have (1,1). The other could be (1,2) or (2,1) or (2,2). If we put (1,2), then row1 has (1,1),(1,2),(1,4) -> 3 cells, done. Row2 still needs 1 cell. Top-left 3x3 must have 3 cells. We have (1,1),(1,2) in top-left 3x3. Need one more. It must be in row2 or row3, cols 1-3. Row2 needs 1 cell, so it could be (2,3) or (2,1) or (2,2). But also top-right conditions: top-right 2x2: rows 1-2, cols 3-4. Must have 2 cells. We already have (1,4) in top-right 2x2. Need one more in rows 1-2, cols 3-4. It could be (1,3) or (2,3) or (2,4). But row1 already has 3 cells, so (1,3) not allowed. So it must be (2,3) or (2,4). If we put (2,3), then row2 gets its cell, and top-right 2x2 gets (2,3) and (1,4) -> 2 cells. Then top-left 3x3: we have (1,1),(1,2),(2,3) -> 3 cells. Good. Top-right 3x3: rows 1-3, cols 2-4? Actually top-right 3x3 is rows 1-3, cols 4-3+1=2 to 4? Wait: n=4, top-right j x j: rows 1..j, cols n+1-j..n. For j=3: rows 1-3, cols 4+1-3=2 to 4. So cols 2,3,4. Must have 3 cells. We have (1,4), (2,3). Need one more in rows 1-3, cols 2-4. It could be (1,2) already in col2, so that counts. So we have (1,2),(2,3),(1,4) -> 3 cells. Good. Top-left 4x4: must have 4 cells. We have (1,1),(1,2),(2,3) -> 3 cells. Need one more. It must be in row4? But a4=0, so no cells in row4. So we need a cell in row3? a3=0, so no. So this fails. So (1,2) and (2,3) doesn't work because we need 4 cells total but only placed 3? Wait, we placed (1,1),(1,2),(1,4),(2,3) -> 4 cells. Top-left 4x4 is the whole grid, must have 4 cells. We have 4 cells, so that's fine. But wait, top-left 4x4 condition is for j=4: number of cells in rows 1-4, cols 1-4 is 4. Our cells are (1,1) in col1, (1,2) in col2, (2,3) in col3, (1,4) in col4. All are in cols 1-4, so 4 cells. Good. But we also need d2 conditions? We already checked top-right conditions. What about d2=4? max(x,5-y)=4. We need exactly one cell with d2=4. Our cells: (1,1): max(1,4)=4; (1,2): max(1,3)=3; (2,3): max(2,2)=2; (1,4): max(1,1)=1. So d2 values: 4,3,2,1. Perfect. d1 values: (1,1):1; (1,2):2; (2,3):3; (1,4):4. Perfect. So this grid is valid! Let's check row counts: row1: (1,1),(1,2),(1,4) -> 3 cells, row2: (2,3) -> 1 cell, row3:0, row4:0. So a=[3,1,0,0] works with this grid.
        // Now what is the other grid? We could have chosen (2,2) instead of (1,2) and (2,3)? Let's try: (1,1), (1,4) forced. Row1 needs one more in cols 2,3. Suppose we put (1,3). Then row1: (1,1),(1,3),(1,4). Top-left 2x2: must have 2 cells. We have (1,1). Need one more in 2x2. It could be (2,1) or (2,2). Row2 needs 1 cell. If we put (2,2), then top-left 2x2 has (1,1),(2,2) -> 2 cells. Top-right 2x2: rows 1-2, cols 3-4. Must have 2 cells. We have (1,3),(1,4) -> 2 cells. Good. Top-left 3x3: rows 1-3, cols 1-3. Must have 3 cells. We have (1,1),(2,2),(1,3) -> 3 cells. Top-right 3x3: rows 1-3, cols 2-4. Must have 3 cells. We have (1,3),(1,4) and need one more. Could be (2,2)? But (2,2) is in col2, which is in cols 2-4, so yes. So (1,3),(1,4),(2,2) -> 3 cells. Top-left 4x4: 4 cells total. We have (1,1),(1,3),(1,4),(2,2) -> 4 cells. d1: (1,1):1, (2,2):2, (1,3):3, (1,4):4. d2: (1,1):4, (2,2):3? max(2,3)=3; (1,3): max(1,2)=2; (1,4):1. So d2: 4,3,2,1. Valid! So this is the second grid.
        // Are there other possibilities? What if we put (2,1) for row2? Then top-left 2x2: (1,1),(2,1) -> 2 cells. Row1 still needs one more in cols 2,3. Could be (1,2) or (1,3). If (1,2): then row1: (1,1),(1,2),(1,4). Top-right 2x2: rows 1-2, cols 3-4. Must have 2 cells. We have (1,4). Need one more. Could be (1,3) but row1 full. Could be (2,3) or (2,4). But row2 already has (2,1), and a2=1, so row2 full. So no. If (1,3): row1: (1,1),(1,3),(1,4). Top-right 2x2: (1,3),(1,4) -> 2 cells. Good. Top-left 3x3: rows 1-3, cols 1-3. Must have 3 cells. We have (1,1),(2,1),(1,3) -> 3 cells. Top-right 3x3: rows 1-3, cols 2-4. Must have 3 cells. We have (1,3),(1,4) and need one more. Could be (2,1)? col1 not in 2-4. So no. Could be (2,2) but row2 full. Could be (3,?) but a3=0. So fails. So only two grids.
        // So the valid grids correspond to choosing for each row i (from 1 to n) whether the "extra" cells (beyond the forced diagonal ones) go to the left or right? Actually, in both valid grids, row1 has cells in col1, col4, and one of col2 or col3. Row2 has cell in col3 or col2 respectively. So it's like a "path" that goes from (1,1) to (1,n) by moving right and down? Notice that the cells are (1,1), then either (1,2) and (2,3) or (1,3) and (2,2), then (1,4). So the cells form a sequence where each step either increases row or increases column? Actually, if we sort by d1: (1,1) d1=1, (1,2) d1=2, (2,3) d1=3, (1,4) d1=4. The coordinates: (1,1) -> (1,2) (right), (1,2) -> (2,3) (down-right), (2,3) -> (1,4) (up-right). Not monotonic.
        // But notice that the columns are exactly a permutation of 1..n. In both grids, the columns used are 1,2,3,4 for the first grid? First grid: (1,1) col1, (1,2) col2, (2,3) col3, (1,4) col4 -> columns 1,2,3,4. Second grid: (1,1) col1, (1,3) col3, (2,2) col2, (1,4) col4 -> columns 1,3,2,4. So the columns form a permutation. And the rows have the given counts a_i.
        // So the problem reduces to: We need to place n cells in the grid such that each column has exactly one cell, the row counts are a_i, and the two diagonal conditions hold.
        // The diagonal conditions are equivalent to: if we sort the cells by column, the row indices form a permutation? Actually, if each column has exactly one cell, let the cell in column j be at row p_j. Then p is a permutation of 1..n? Not necessarily, because multiple cells can be in the same row. But wait, if each column has exactly one cell, then there are n cells total. The row counts a_i sum to n. So the multiset of rows is exactly the multiset where row i appears a_i times. So p is not a permutation but a sequence of length n with row i appearing a_i times.
        // Now, what are the conditions on p?
        // Condition 2: for each k, exactly one cell with max(x,y)=k. Here x = p_j, y = j. So max(p_j, j) = k. Since we need each k from 1 to n exactly once, the multiset {max(p_j, j)} must be {1,2,...,n}.
        // Condition 3: for each k, exactly one cell with max(x, n+1-y)=k. Here max(p_j, n+1-j) = k. So the multiset {max(p_j, n+1-j)} must be {1,2,...,n}.
        // So we need to find the number of sequences p_1,...,p_n (where p_j is the row of the cell in column j) such that:
        // - The number of j with p_j = i is a_i.
        // - {max(p_j, j)} = {1..n}
        // - {
