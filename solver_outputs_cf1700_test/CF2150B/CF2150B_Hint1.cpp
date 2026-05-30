// Hint1
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
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }

        // Check necessary condition: sum of a_i must be n
        if (accumulate(a.begin(), a.end(), 0LL) != n) {
            cout << "0\n";
            continue;
        }

        // We'll build the answer by processing rows from n down to 1
        // We maintain the number of "free" columns that can still receive black cells
        // in the current and lower rows.
        // Initially, no columns are used.
        // We also need to satisfy the two max conditions.
        // The key observation: the two max conditions force a unique structure:
        // For each k from 1 to n, there is exactly one black cell with max(x, y) = k
        // and exactly one with max(x, n+1-y) = k.
        // This implies that the set of black cells is exactly a permutation-like structure
        // where each row i has a_i cells, and the columns are determined by the two diagonals.
        // Actually, it's known that the conditions imply that the black cells form a set
        // where each row i has a_i cells, and the columns are exactly the set of values
        // such that the two "max" conditions hold. It turns out that the number of ways
        // is product over i from 1 to n of something depending on a_i and the number of
        // available columns.
        // Let's derive:
        // Let c_i be the number of black cells in row i (given as a_i).
        // The condition max(x, y) = k means that on the diagonal x=y, we have exactly one
        // cell per k, but not necessarily on the diagonal. Actually, max(x,y)=k means the cell
        // is on the boundary of the kxk square (row k or column k). Similarly for the other.
        // This is equivalent to: the black cells form a set where each row i has a_i cells,
        // and the columns are a permutation of 1..n? Not exactly.
        // Let's think differently: The two conditions together imply that the black cells
        // are exactly the cells (i, p_i) for some permutation p? No, because a_i can be >1.
        // Wait, the example: a = [2,2,1,0,0], cells: (1,1),(2,2),(3,3),(2,4),(1,5).
        // Row 1 has 2 cells: (1,1) and (1,5). Row 2 has 2: (2,2),(2,4). Row 3 has 1: (3,3).
        // Check max(x,y)=k: k=1: max(1,1)=1 -> (1,1). k=2: max(2,2)=2 -> (2,2). k=3: max(3,3)=3 -> (3,3). k=4: max(2,4)=4 -> (2,4). k=5: max(1,5)=5 -> (1,5). Works.
        // Check max(x, n+1-y)=k: n=5. k=1: max(1,5)=1? Wait n+1-y: for (1,1): n+1-1=5, max(1,5)=5, not 1. So (1,1) gives max=5. Let's list:
        // (1,1): max(1,5)=5 -> k=5.
        // (2,2): max(2,4)=4 -> k=4.
        // (3,3): max(3,3)=3 -> k=3.
        // (2,4): max(2,2)=2 -> k=2.
        // (1,5): max(1,1)=1 -> k=1.
        // So the second condition is satisfied with k=1..5.
        // Notice that the cells are exactly those on two diagonals? Actually, they are on the main diagonal and the anti-diagonal? (1,1) main, (2,2) main, (3,3) main, (2,4) is not on anti-diagonal (anti-diagonal would be (2,4)? n=5, anti-diagonal: x+y=6 => (2,4) yes! (1,5) anti-diagonal. So the black cells are exactly the union of the main diagonal and the anti-diagonal, but with some cells possibly overlapping? Here (3,3) is on both? 3+3=6, yes it's on anti-diagonal too. So the set is the union of the two diagonals. And a_i counts how many of these are in row i: row 1: (1,1) and (1,5) -> 2. row 2: (2,2) and (2,4) -> 2. row 3: (3,3) -> 1. row 4: none -> 0. row 5: none -> 0.
        // So the conditions force the black cells to be exactly the cells on the two diagonals? Let's test another case: n=2, a=[2,0]. Valid grid: (1,1),(1,2). Diagonals: main: (1,1),(2,2); anti: (1,2),(2,1). Union: all four cells. But a=[2,0] means row1 has 2, row2 has 0. That is exactly the anti-diagonal? Actually (1,1) and (1,2) are in row1: (1,1) is main, (1,2) is anti. So it's the union of the two diagonals restricted to row1? But row2 has 0, so we didn't take (2,1) or (2,2). So the set is not the full union; it's a subset of the union that satisfies the max conditions.
        // Let's analyze the max conditions more systematically.
        // Condition 1: For each k, exactly one cell with max(x,y)=k.
        // This means that if we consider the cells sorted by max(x,y), we get exactly one per k.
        // Condition 2: For each k, exactly one cell with max(x, n+1-y)=k.
        // This is symmetric after reflecting y.
        // Let's define for each cell its "rank" in the first condition: r1 = max(x,y).
        // And for the second: r2 = max(x, n+1-y).
        // The conditions say that the mapping from cells to r1 is a bijection to {1..n}, and similarly for r2.
        // So each cell has a pair (r1, r2). Since there are exactly n cells (because sum a_i = n? Wait, sum a_i is not necessarily n? The problem says: for each row k, there exists exactly a_k indices i such that x_i = k. So total black cells = sum a_i. But the conditions "for each k, exactly one index i such that max(x_i, y_i)=k" implies there are exactly n black cells, because k ranges 1..n. So sum a_i must be n. That's a necessary condition. We already checked that.
        // So there are exactly n black cells.
        // Now, each cell gets a unique r1 and a unique r2. So the cells correspond to a permutation? Actually, we have n cells, each has a distinct r1 in 1..n and a distinct r2 in 1..n. So the set of cells is exactly a set of n pairs (x,y) such that the values max(x,y) are a permutation of 1..n, and max(x, n+1-y) are a permutation of 1..n.
        // Moreover, the row counts are given by a_i.
        // Let's see what pairs (x,y) can appear. For a given cell, r1 = max(x,y), r2 = max(x, n+1-y).
        // We can think of the grid as having coordinates. Let's define u = x, v = y, and also v' = n+1-y.
        // Then r1 = max(u, v), r2 = max(u, v').
        // Note that v + v' = n+1.
        // We want to select n cells such that the r1 values are 1..n each exactly once, and r2 values are 1..n each exactly once.
        // This is reminiscent of the problem of placing rooks on a board with some forbidden positions? Actually, it's known that the conditions force the cells to be exactly the "diagonal" cells of a permutation matrix? Let's try to characterize.
        // Consider the transformation: For each cell, we can map it to (r1, r2). What is the range of (r1, r2) for valid cells?
        // Given u,v in [1,n], we have r1 = max(u,v), r2 = max(u, n+1-v).
        // Notice that r1 >= u, r1 >= v; r2 >= u, r2 >= n+1-v.
        // Also, r1 and r2 are at least something.
        // Let's see the possible pairs (r1, r2) for a fixed u. For a given row u, what columns v are possible?
        // We need r1 = max(u,v) and r2 = max(u, n+1-v).
        // Since r1 and r2 must be distinct for different cells, and we need exactly one cell per r1 and per r2, this is like matching rows to columns with constraints.
        // Actually, we can think of it as: we need to choose for each row u, a set of columns (with total size a_u) such that the multiset of r1 values is exactly {1..n} and r2 values exactly {1..n}.
        // But since each cell gives one r1 and one r2, and there are n cells, it's a bijection between cells and r1, and between cells and r2. So we can think of the cells as a permutation if we order them by r1? Not necessarily, because multiple cells can have the same row.
        // Let's try to construct the cells row by row from bottom to top? The hints say: "What can be said about the position of the black cell (1,1)?" So maybe we can determine the cells uniquely from the a_i array?
        // Let's analyze the example: a = [2,2,1,0,0]. The cells are (1,1),(1,5),(2,2),(2,4),(3,3).
        // Notice that the cells are exactly those on the two diagonals, but only up to row 3? Actually, row 4 and 5 have 0, so no cells there. But the max conditions require r1=4 and r1=5 to be present. They are present: (2,4) has r1=4, (1,5) has r1=5. So even though rows 4 and 5 have no black cells, the r1 values 4 and 5 are achieved by cells in lower rows (row 2 and 1). Similarly, r2=4 is (2,2) because max(2, 5+1-2=4)=4; r2=5 is (1,1) because max(1,5)=5.
        // So the cells can be in rows lower than their r1 or r2.
        // Let's think about the constraints on a_i. For each row i, we have a_i cells. Their r1 values must be >= i (since r1 = max(i, y) >= i). Also, their r2 values must be >= i (since r2 = max(i, n+1-y) >= i). So all cells in row i have r1 >= i and r2 >= i.
        // Moreover, the r1 values of cells in rows > i are all >= row > i, so they are > i. Therefore, the r1 values that are <= i must come from rows <= i. Since there are exactly i values of r1 that are <= i (namely 1..i), and each row j <= i contributes a_j cells, all with r1 >= j, but we need to cover 1..i. This gives a necessary condition: for each i, the number of cells in rows 1..i must be at least i? Actually, the sum of a_j for j=1..i must be >= i, because we need to assign i distinct r1 values <= i, and each cell can only provide one r1 value. Since there are sum_{j=1}^i a_j cells in rows 1..i, we must have sum_{j=1}^i a_j >= i. Similarly, for r2, we get the same condition? Let's check: r2 values <= i must come from rows <= i? r2 = max(x, n+1-y). If x <= i, then r2 >= x, but could be > i if n+1-y > i. So a cell in row x <= i could have r2 > i. So the condition for r2 is not simply sum a_j >= i. However, there is a symmetry: if we reverse the columns, the problem is symmetric. So maybe the condition is that for all i, sum_{j=1}^i a_j >= i and sum_{j=1}^i a_{n+1-j} >= i? Not sure.
        // Let's look at the known solution for this problem (I recall it's a Codeforces problem "HyperioxX" or similar? Actually, it's from Codeforces Round 2150? The problem might be "Grid" or something). I think the solution involves checking that a_i is non-increasing? No.
        // Let's try to derive the number of ways.
        // Consider the process of assigning cells to satisfy the max conditions. We can think of building the set of black cells by considering k from n down to 1. For each k, we need to place a cell with r1 = k and a cell with r2 = k. But these could be the same cell if it satisfies both? In the example, for k=3, the cell (3,3) has r1=3 and r2=3. So one cell can serve both.
        // Let's define for each cell its "type": it can be on the main diagonal (x=y), on the anti-diagonal (x+y=n+1), or both (center if n odd), or neither? In the example, (2,4) is on anti-diagonal but not main diagonal; its r1=4, r2=2. (1,5) is on anti-diagonal, r1=5, r2=1. (1,1) is on main diagonal, r1=1, r2=5. (2,2) main, r1=2, r2=4. (3,3) both, r1=3, r2=3.
        // So all cells are on either the main diagonal or the anti-diagonal. Is that always true?
        // Suppose a cell is not on either diagonal. Then x != y and x+y != n+1. Then r1 = max(x,y) and r2 = max(x, n+1-y). Can such a cell exist while satisfying the bijection? Let's test n=3. Suppose we try to put a cell at (1,2). Then r1 = max(1,2)=2, r2 = max(1, 4-2=2)=2. So it would have r1=r2=2. Then we need another cell with r1=1 and r2=1? But (1,1) gives r1=1, r2=3. (1,3) gives r1=3, r2=1. (2,1) gives r1=2, r2=3? max(2,2)=2, max(2,4-1=3)=3. So (2,1) gives r1=2, r2=3. (2,3) gives r1=3, r2=2. (3,2) gives r1=3, r2=2? max(3,2)=3, max(3,4-2=2)=3 -> r1=3, r2=3. So (3,2) gives 3,3. It seems many cells give r1=r2. But we need distinct r1 and distinct r2. If we have a cell with r1=r2=k, then it uses up both the k-th r1 and k-th r2. So we can have at most one such cell per k? Actually, if we have two cells with r1=r2=k, they would both have r1=k, violating distinctness. So at most one cell can have r1=r2=k. And if we have a cell with r1 != r2, then it uses an r1 value and a different r2 value.
        // In fact, the conditions imply that the set of black cells is exactly a set of n cells that form a permutation matrix if we map (r1, r2)? Actually, since r1 and r2 are both permutations of 1..n, the pairs (r1, r2) form a permutation of 1..n: each r1 appears exactly once, each r2 appears exactly once. So the mapping from r1 to r2 is a permutation! That is, if we list the cells sorted by r1, their r2 values form a permutation of 1..n. So the set of cells corresponds to a permutation π where the cell with r1 = k has r2 = π(k). And the cell's coordinates (x,y) must satisfy max(x,y)=k and max(x, n+1-y)=π(k).
        // So the problem reduces to: count the number of permutations π of 1..n such that there exists a set of cells (x_i, y_i) with row counts a_i, and for each k, there is a cell with r1=k, r2=π(k). And we need to assign these cells to rows such that row i gets exactly a_i cells.
        // But wait: the row counts a_i are given. So we need to count the number of ways to choose the cells (x,y) satisfying the conditions and matching the row counts.
        // Let's analyze the constraints on (x,y) given r1=k and r2=π(k). We have:
        // max(x,y) = k
        // max(x, n+1-y) = π(k)
        // Also 1 <= x,y <= n.
        // We need to find all pairs (x,y) satisfying these for given k and π(k). Then we need to assign each k to a specific (x,y) such that the row counts are exactly a.
        // Let's solve for (x,y) given k and p = π(k).
        // We have x <= k, y <= k, and at least one of x,y equals k.
        // Also x <= p, n+1-y <= p, and at least one of x, n+1-y equals p.
        // Let's consider cases based on which attains the max.
        // Case 1: x = k. Then y <= k. The second condition: max(k, n+1-y) = p.
        // Subcase 1a: k >= n+1-y, so max = k. Then we need k = p. So if p = k, then we need k >= n+1-y => y >= n+1-k. Also y <= k. So y can be any integer in [max(1, n+1-k), k]. But we also have the condition that the cell is not necessarily on the diagonal? Actually, we just need some y. But we also have the row count: the cell is in row x = k. So this cell contributes to row k.
        // Subcase 1b: n+1-y > k, so max = n+1-y = p. Then we need p > k, and y = n+1-p. Also we need y <= k (since x=k and max(x,y)=k implies y <= k). So n+1-p <= k => p >= n+1-k. Also y >= 1 => p <= n. So conditions: p > k, p >= n+1-k, p <= n. And then x=k, y=n+1-p. This cell is in row k.
        // Case 2: y = k, x < k. Then first condition max(x,k)=k holds. Second: max(x, n+1-k) = p.
        // Subcase 2a: x >= n+1-k, so max = x = p. Then p = x < k. Also we need x >= n+1-k => p >= n+1-k. And x >= 1 => p >= 1. So conditions: p < k, p >= n+1-k. Cell is (p, k) in row p.
        // Subcase 2b: n+1-k > x, so max = n+1-k = p. Then p = n+1-k. And we need x < n+1-k = p, so x < p. Also x < k (already). And x >= 1. So conditions: p = n+1-k, and we can choose any x in [1, min(k-1, p-1)]? But wait, we also need the cell to be valid: max(x,k)=k is automatic. But we need to assign exactly one cell for this k. However, if there are multiple possible x, we have choices. But note that the row of this cell is x, so it contributes to row x. And we have a_i given. So the choices of x will affect row counts.
        // So for each k, given p = π(k), there may be multiple possible cells (x,y) that satisfy the conditions. We need to select exactly one cell for each k such that the row counts match a_i.
        // Moreover, the selected cells must be distinct (since they are different k, but could they coincide? If two different k give the same (x,y), that would be a problem because a cell can only be black once. But since each k gives a distinct r1, and r1 = max(x,y), if two cells had the same (x,y), they would have the same r1, contradiction. So cells for different k are automatically distinct.)
        // So the problem reduces to: Given a permutation π, we can determine for each k the set of possible cells (x,y) that satisfy the conditions for that k and p=π(k). Then we need to count the number of ways to choose one cell per k such that the total number of cells in row i is exactly a_i.
        // But we also need to sum over all valid permutations π? Or is π uniquely determined by a? Let's check the example: a = [2,2,1,0,0]. The cells we had: (1,1) r1=1,r2=5; (2,2) r1=2,r2=4; (3,3) r1=3,r2=3; (2,4) r1=4,r2=2; (1,5) r1=5,r2=1. So π(1)=5, π(2)=4, π(3)=3, π(4)=2, π(5)=1. This is the permutation that reverses 1..5. Is that forced? Let's see if we could have a different permutation. Suppose we try to change something. For instance, could we have a cell with r1=4, r2=4? That would be (4,4) or (4,?) or (?,4). But row 4 has a_4=0, so we cannot put any cell in row 4. So (4,4) is forbidden. What about (2,4) we already have. Could we have (1,4)? r1=4, r2=max(1,2)=2? Actually n=5, n+1-4=2, max(1,2)=2. So (1,4) gives r1=4, r2=2. That's the same r2 as (2,4)? But r2 must be distinct. So if we used (1,4) for r1=4, then r2=2 is used. Then we need another cell for r2=4? But r2=4 could be from (2,2) as before. So maybe we could swap? Let's try to construct an alternative set with the same a. We need row 1: 2 cells, row 2: 2 cells, row 3: 1 cell. The r1 values must be 1,2,3,4,5. The cells in row 1 can have r1=1 or r1=5? Actually, if a cell is in row 1, its r1 = max(1,y) = y if y>=1. So r1 can be any y from 1 to n. But if y=1, r1=1; y=5, r1=5. So row 1 can provide r1=1 or 5 (or others if y is something else). Row 2 can provide r1=2,3,4,5? max(2,y): if y=2 ->2; y=3->3; y=4->4; y=5->5. Row 3: max(3,y): y=3->3; y=4->4; y=5->5. So to get r1=4, we could use row 2 with y=4, or row 3 with y=4, or row 4 with y=4 (but row 4 has 0 cells). To get r1=5, we could use row 1 with y=5, row 2 with y=5, row 3 with y=5, etc. But we also have the r2 condition. Let's list all possible cells and their (r1,r2):
        // For n=5:
        // (1,1): (1,5)
        // (1,2): (2,4)
        // (1,3): (3,3)
        // (1,4): (4,2)
        // (1,5): (5,1)
        // (2,1): (2,5) because max(2,1)=2, max(2,5)=5
        // (2,2): (2,4)
        // (2,3): (3,3)
        // (2,4): (4,2)
        // (2,5): (5,1)
        // (3,1): (3,5)
        // (3,2): (3,4)
        // (3,3): (3,3)
        // (3,4): (4,2)
        // (3,5): (5,1)
        // (4,1): (4,5)
        // (4,2): (4,4)
        // (4,3): (4,3)
        // (4,4): (4,2)
        // (4,5): (5,1)
        // (5,1): (5,5)
        // (5,2): (5,4)
        // (5,3): (5,3)
        // (5,4): (5,2)
        // (5,5): (5,1)
        // Wait, I need to compute r2 = max(x, n+1-y) = max(x, 6-y).
        // Let's recompute carefully:
        // (1,1): max(1,5)=5 -> (1,5)
        // (1,2): max(1,4)=4 -> (2,4)
        // (1,3): max(1,3)=3 -> (3,3)
        // (1,4): max(1,2)=2 -> (4,2)
        // (1,5): max(1,1)=1 -> (5,1)
        // (2,1): max(2,5)=5 -> (2,5)
        // (2,2): max(2,4)=4 -> (2,4)
        // (2,3): max(2,3)=3 -> (3,3)
        // (2,4): max(2,2)=2 -> (4,2)
        // (2,5): max(2,1)=2? Wait max(2,1)=2, but r1 = max(2,5)=5? Actually r1 = max(x,y) = max(2,5)=5. So (2,5) gives r1=5. r2 = max(2, 6-5=1) = max(2,1)=2. So (5,2)? No, the pair is (r1, r2) = (5,2). I wrote (5,1) incorrectly. Let's do systematically:
        // For each cell, r1 = max(x,y), r2 = max(x, n+1-y).
        // Let's make a table for n=5:
        // x=1:
        // y=1: r1=1, r2=max(1,5)=5 -> (1,5)
        // y=2: r1=2, r2=max(1,4)=4 -> (2,4)
        // y=3: r1=3, r2=max(1,3)=3 -> (3,3)
        // y=4: r1=4, r2=max(1,2)=2 -> (4,2)
        // y=5: r1=5, r2=max(1,1)=1 -> (5,1)
        // x=2:
        // y=1: r1=2, r2=max(2,5)=5 -> (2,5)
        // y=2: r1=2, r2=max(2,4)=4 -> (2,4)
        // y=3: r1=3, r2=max(2,3)=3 -> (3,3)
        // y=4: r1=4, r2=max(2,2)=2 -> (4,2)
        // y=5: r1=5, r2=max(2,1)=2 -> (5,2)
        // x=3:
        // y=1: r1=3, r2=max(3,5)=5 -> (3,5)
        // y=2: r1=3, r2=max(3,4)=4 -> (3,4)
        // y=3: r1=3, r2=max(3,3)=3 -> (3,3)
        // y=4: r1=4, r2=max(3,2)=3 -> (4,3)
        // y=5: r1=5, r2=max(3,1)=3 -> (5,3)
        // x=4:
        // y=1: r1=4, r2=max(4,5)=5 -> (4,5)
        // y=2: r1=4, r2=max(4,4)=4 -> (4,4)
        // y=3: r1=4, r2=max(4,3)=4 -> (4,4)? Wait max(4,3)=4, so (4,4)
        // y=4: r1=4, r2=max(4,2)=4 -> (4,4)
        // y=5: r1=5, r2=max(4,1)=4 -> (5,4)
        // x=5:
        // y=1: r1=5, r2=max(5,5)=5 -> (5,5)
        // y=2: r1=5, r2=max(5,4)=5 -> (5,5)
        // y=3: r1=5, r2=max(5,3)=5 -> (5,5)
        // y=4: r1=5, r2=max(5,2)=5 -> (5,5)
        // y=5: r1=5, r2=max(5,1)=5 -> (5,5)
        // So the possible (r1,r2) pairs are:
        // (1,5) from (1,1)
        // (2,4) from (1,2), (2,2)
        // (3,3) from (1,3), (2,3), (3,3)
        // (4,2) from (1,4), (2,4), (3,4)? Wait (3,4) gave (4,3) not (4,2). Let's check (3,4): r1=4, r2=max(3,2)=3 -> (4,3). So (4,2) comes from (1,4), (2,4). Also (4,4) gave (4,4). (4,2) also from? (4,?) no.
        // (5,1) from (1,5)
        // (2,5) from (2,1)
        // (5,2) from (2,5)
        // (3,5) from (3,1)
        // (3,4) from (3,2)
        // (4,3) from (3,4)
        // (5,3) from (3,5)
        // (4,5) from (4,1)
        // (4,4) from (4,2),(4,3),(4,4)
        // (5,4) from (4,5)
        // (5,5) from (5,1)..(5,5)
        // So many cells give the same (r1,r2). But we need to select exactly one cell for each r1 from 1 to 5, and they must have distinct r2. So we are selecting a permutation π such that for each k, we pick a cell with r1=k and r2=π(k). And the cell must exist (i.e., there is at least one (x,y) with those r1,r2). Then we also need to assign which specific (x,y) we use for that k, and the row counts must match a.
        // Notice that for a given (r1,r2) pair, there may be multiple cells (x,y) that produce it. For example, (2,4) can be from (1,2) or (2,2). If we choose (1,2), it adds to row 1; if (2,2), adds to row 2. So the choice of which cell to realize a given (r1,r2) pair affects the row counts.
        // Moreover, the permutation π is not fixed; we can choose any permutation π such that for each k, the pair (k, π(k)) is realizable by some cell. Then we also choose the specific cell.
        // But wait: the problem asks for the number of grids (sets of black cells) satisfying the conditions. So we are directly choosing the set of cells. The conditions are exactly that the set of cells has size n, row counts a_i, and the two max conditions hold. So we don't need to think about π explicitly; we can just try to construct the set of cells directly.
        // Let's think about the structure of valid sets. From the example, the black cells are exactly the cells on the two diagonals that are in rows with a_i > 0? But in the example, row 1 has a_1=2, and the cells are (1,1) and (1,5) — these are the main diagonal and anti-diagonal in row 1. Row 2 has a_2=2: (2,2) and (2,4) — main and anti. Row 3 has a_3=1: (3,3) — both main and anti (the center). Row 4 and 5 have 0. So it seems that in each row i, the black cells are exactly the intersection of the row with the two diagonals, but we only take those that are "active"? But how do we decide which rows get which?
        // Let's look at the second sample: n=2, a=[2,0]. Valid grid: (1,1),(1,2). These are the main diagonal (1,1) and anti-diagonal (1,2) in row 1. Row 2 has 0. So again, the black cells are exactly the cells on the two diagonals in the rows that have a_i > 0? But row 1 has a_1=2, so it takes both diagonals. Row 2 has a_2=0, takes
