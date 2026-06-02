#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

struct Table {
    int x, y;
    array<pair<int,int>, 4> cells() const {
        return {make_pair(3*x+1, 3*y+1),
                make_pair(3*x+1, 3*y+2),
                make_pair(3*x+2, 3*y+1),
                make_pair(3*x+2, 3*y+2)};
    }
};

int dist_to_table_cell(int x, int y, int tx, int ty) {
    // distance from (0,0) to a table cell (tx,ty) of table (x,y)
    // moving only through corridors, last step into table cell
    // Actually distance from (0,0) to (tx,ty) with last step into table cell
    // The table cells are at (3x+1,3y+1), (3x+1,3y+2), (3x+2,3y+1), (3x+2,3y+2)
    // The distance is simply Manhattan distance because we can move freely in corridors
    // and the last step is just a step into the table cell.
    return abs(tx) + abs(ty);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int q;
    cin >> q;
    while (q--) {
        int n;
        cin >> n;
        vector<int> t(n);
        for (int i = 0; i < n; ++i) cin >> t[i];

        // We'll maintain a set of candidate tables.
        // For t=0: guest wants a completely free table.
        // For t=1: guest wants any free table cell.
        // We'll simulate using a priority queue of tables sorted by distance, then x, then y.
        // But we need to handle the fact that tables have 4 cells, and t=1 can occupy individual cells.
        // The hints suggest a simpler approach: the first guest determines everything.
        // Let's follow the hints.

        // The problem reduces to: there are only two possible first tables: (0,0) and (0,1) or (1,0)?
        // Actually, the first guest (t=0 or t=1) will choose the nearest table cell.
        // Nearest table cells: (1,1) from table (0,0) distance 2.
        // Next nearest: (1,2) and (2,1) from tables (0,0) and (0,0)? Wait, table (0,0) has cells (1,1),(1,2),(2,1),(2,2).
        // So the first guest always picks (1,1) because it's the unique minimum distance 2.
        // But the sample shows first guest picks (1,1) for t=0.
        // What about t=1? Also (1,1) because it's the nearest vacant cell.
        // So first guest always picks (1,1). Then the problem becomes: after some cells are occupied,
        // the distances to other tables change.
        // The hints say: if first guest goes to first candidate, distance to other candidates increases by 1.
        // Otherwise decreases by 1.
        // This suggests a pattern: the set of candidate tables is always the same, and their distances shift by ±1.
        // Let's analyze.

        // The tables are at (x,y). The distance to a table cell is the Manhattan distance from (0,0) to that cell.
        // For table (x,y), the four cells have distances:
        // (3x+1,3y+1): 3x+3y+2
        // (3x+1,3y+2): 3x+3y+3
        // (3x+2,3y+1): 3x+3y+3
        // (3x+2,3y+2): 3x+3y+4
        // The minimum distance to a free cell of table (x,y) is 3x+3y+2 if (1,1) cell is free, else 3x+3y+3 if one of the two side cells is free, else 3x+3y+4 if only the far cell is free.
        // But the hints talk about "candidates". What are candidates?
        // Probably the set of tables that are "closest" in some sense.
        // Let's think: the first guest picks (1,1). Then the table (0,0) has 3 free cells left.
        // The next nearest free cells are (1,2) and (2,1) at distance 3.
        // If t=1, guest picks nearest vacant cell, which could be (1,2) or (2,1) depending on tie-breaking.
        // If t=0, guest wants a completely unoccupied table. The nearest completely unoccupied table is (0,1) or (1,0)?
        // Table (0,1) has cells (1,4),(1,5),(2,4),(2,5). Minimum distance is to (1,4) = 5.
        // Table (1,0) has cells (4,1),(4,2),(5,1),(5,2). Minimum distance to (4,1) = 5.
        // So for t=0, the fourth guest picks (1,4) or (4,1) at distance 5.
        // This matches sample.

        // The hints suggest that the answer depends only on t_i and the order, and there is a simple pattern.
        // Let's try to derive the pattern from the hints.

        // Hint 2: Consider the first guest. How many candidates can there be?
        // Answer: 1? The first guest always picks (1,1). So only one candidate.

        // Hint 3: If the first guest goes to the first candidate, then the distance from the second guest to the other candidates increases by 1. Otherwise, it decreases by 1.
        // This implies there are multiple candidates for the first guest? But we just said there's only one.
        // Maybe "candidates" refers to the set of tables that will be chosen by guests, not just the first guest.
        // Perhaps the set of tables that are "active" is always the same, and their distances shift.

        // Let's look at the sample:
        // Test case 1: t = 0 1 1 0 0 1
        // Output: (1,1), (1,2), (2,1), (1,4), (4,1), (1,5)
        // Tables used: (0,0) cells (1,1),(1,2),(2,1); (0,1) cell (1,4); (1,0) cell (4,1); (0,1) cell (1,5).
        // Notice that tables (0,0), (0,1), (1,0) are used. Table (0,0) is partially filled, (0,1) partially, (1,0) partially.
        // The distances: (1,1)=2, (1,2)=3, (2,1)=3, (1,4)=5, (4,1)=5, (1,5)=6.
        // If we consider the "candidates" as the set of tables that are "next" to be used, maybe they are (0,0), (0,1), (1,0), (1,1)? But (1,1) has cells (4,4) etc., distance 8.

        // Another perspective: The problem is equivalent to guests occupying cells in a specific order determined by a BFS from (0,0) on the grid of table cells, but with the twist that t=0 requires a completely free table.
        // However, the hints strongly suggest a greedy simulation with a set of "candidates" that changes in a predictable way.

        // Let's re-read hints carefully:
        // Hint 3: If the first guest goes to the first candidate, then the distance from the second guest to the other candidates increases by 1. Otherwise, it decreases by 1.
        // Hint 4: This means that there is a strategy of choosing candidates for the first guest, such that if we apply it to the other guests, their distances will not change.
        // Hint 5: What happens if the first guest goes to a different candidate?
        // Hint 6: How much will the distance from the other guests change?
        // Hint 7: The answer depends on the value of t_1, but the answer is the same for both cases.

        // This sounds like the problem can be reduced to: there is a sequence of "candidate tables" that are considered in order. The first guest's choice determines whether the distances to the remaining candidates increase or decrease by 1, but the relative order and choices of subsequent guests remain the same regardless of t_1? That seems contradictory because t_1=0 vs t_1=1 give different first choices? But hint 7 says the answer is the same for both cases? That can't be right because sample test case 1 has t1=0 and picks (1,1); test case 2 has t1=1 and picks (1,1) as well. So maybe the first guest always picks (1,1) regardless of t? Let's check: if t1=1, nearest vacant cell is (1,1). If t1=0, nearest completely unoccupied table is table (0,0) which has cell (1,1). So in both cases, first guest picks (1,1). So indeed, the first guest's choice is the same for both t=0 and t=1. So hint 7 is trivially true.

        // What about subsequent guests? The hints suggest that the set of candidates is always the same, and their distances shift by ±1 depending on the first guest's choice. But since the first guest's choice is fixed, the distances shift in a fixed way.

        // Let's try to simulate the process efficiently. n up to 50000, sum n 50000. We can simulate each guest in O(log n) or similar.

        // We need to maintain the set of free table cells and completely free tables.
        // Tables are infinite, but we only care about those within distance ~ n.
        // The maximum distance a guest might travel is O(n). Since n <= 50000, the maximum coordinate is around 50000. So we can limit x,y to about 20000 (since 3x ~ 60000).

        // Let's design a simulation:
        // We'll maintain:
        // - For t=1: a set of all free table cells, sorted by distance, then x, then y.
        // - For t=0: a set of completely free tables, represented by their "best" cell (the one with smallest distance, i.e., (3x+1,3y+1)), sorted by distance, then x, then y.
        // When a guest arrives:
        //   if t=1: pop the best free cell, mark it occupied. If its table becomes completely occupied? No, t=1 doesn't care about table occupancy.
        //   if t=0: pop the best completely free table, occupy its best cell (3x+1,3y+1). That table is no longer completely free. Its remaining cells become available for t=1.
        // But wait: for t=0, the guest occupies one cell of that table. The table is no longer completely unoccupied. The other cells of that table are still free and can be used by t=1 guests.
        // Also, when a t=1 guest occupies a cell, if that cell was part of a completely free table, that table is no longer completely free.
        // So we need to maintain both structures and update them accordingly.

        // However, the number of tables we need to consider is large. But we can generate tables on the fly.
        // The distance to a table cell (3x+1,3y+1) is 3x+3y+2. So tables are ordered by x+y, then x.
        // We can precompute all tables up to a certain sum. Max distance for n=50000 guests: each guest increases the occupied cells. The maximum distance a guest will travel is roughly the distance to the n-th nearest cell. The number of table cells within distance D is about O(D^2). For D ~ 50000, D^2 is too large. But we only have 50000 guests, so we only need the first 50000 cells. The cells are generated by tables with x+y <= something. The number of tables with x+y <= S is (S+1)(S+2)/2. For S=400, that's ~80000 tables, each with 4 cells = 320k cells. That's manageable. 50000 guests will likely stay within S=400. Let's check: sum of x+y for table (x,y) gives distance ~3(x+y). For distance 50000, x+y ~ 16666. That's too large. But wait, the distance to a cell is 3x+3y+2,3,4. So if a guest goes to distance D, x+y is about D/3. For D=50000, x+y=16666, number of tables is ~140 million, too many. But do we really need to go that far? The guests occupy cells, so the distance increases as guests arrive. The 50000th guest might have to go to a cell with distance around 50000? Actually, the number of cells within distance D is roughly the number of integer points with x+y <= D/3, times 4. That's about 4*(D/3)^2 /2 = (2/9)D^2. For D=50000, that's ~5.5e8 cells, far more than 50000. So the 50000th guest will have distance much less than 50000. Let's estimate: we need 50000 cells. Solve (2/9)D^2 = 50000 => D^2 = 225000 => D ≈ 474. So maximum distance is around 500. That means x+y <= 500/3 ≈ 166. Number of tables with x+y <= 166 is about 166^2/2 ≈ 14000. That's very small! So we can just precompute all tables up to x+y = 200, which gives ~20000 tables, 80000 cells. That's well within limits.

        // Let's verify: The cells are at (3x+1,3y+1) etc. The Manhattan distance from (0,0) is (3x+1)+(3y+1) = 3(x+y)+2. So distance D corresponds to x+y = (D-2)/3. For D=500, x+y=166. So indeed, we only need tables with x+y up to ~200 to cover 50000 guests. So we can pre-generate all tables up to x+y = 200 (or maybe 300 to be safe). Let's set MAX_SUM = 300. Number of tables = (301*302)/2 ≈ 45000. Cells = 180k. That's fine.

        // Now, we need to simulate the process. We'll maintain:
        // - A set of free cells for t=1. Each cell has coordinates (cx, cy), distance = cx+cy, and we sort by distance, then cx, then cy.
        // - A set of completely free tables for t=0. Each table is represented by its best cell (3x+1,3y+1). We sort by distance, then cx, then cy.
        // When a t=1 guest takes a cell, we remove it from the free cells. If that cell belonged to a table that was completely free, we must remove that table from the t=0 set. Also, if the table still has other free cells, they remain in the t=1 set.
        // When a t=0 guest takes a table, we remove its best cell from the t=1 set, and remove the table from the t=0 set. The other 3 cells of that table remain in the t=1 set (they were already there? Actually, when a table is completely free, all its 4 cells are in the t=1 set. So we just remove the best cell from t=1, and the table from t=0. The other 3 cells stay in t=1.)
        // But wait: The t=0 guest occupies the best cell (3x+1,3y+1). So that cell is no longer free. The other cells are still free. So we just need to remove that one cell from t=1, and remove the table from t=0.

        // We need to efficiently find and remove cells/tables. We can use std::set with custom comparators. But we need to remove specific cells. We can assign each cell an ID or use a set of tuples.

        // Let's define a Cell struct: x, y. Distance = x+y.
        // For t=1, we want the cell with smallest distance, then smallest x, then smallest y.
        // For t=0, we want the table with smallest distance to its (3x+1,3y+1) cell, then smallest x, then smallest y. The table can be represented by its (x,y) coordinates.

        // We'll precompute all tables with x>=0, y>=0, x+y <= MAX_SUM. For each table, we generate its 4 cells. We'll add all cells to the t=1 set, and add the table to the t=0 set (using its best cell's distance and coordinates for ordering).

        // But we must be careful: The t=0 set should only contain tables that are *completely* unoccupied. Initially all are unoccupied. When a cell is occupied, if it's the first occupation of that table, the table is no longer completely unoccupied, so we remove it from t=0. We can track the number of occupied cells per table.

        // Since we only generate tables up to a limit, what if a guest needs a cell beyond that limit? We can dynamically add more tables if needed. But as argued, 50000 guests will fit within x+y <= 200. Let's set MAX_SUM = 250 to be safe. That gives (251*252)/2 = 31626 tables, 126504 cells. That's plenty.

        // Let's implement:

        // We'll have a map from table (x,y) to a struct with: occupied count (0..4), and maybe an iterator to the t=0 set for O(1) removal.
        // Actually, we can just store tables in a set for t=0, and when a cell is occupied, we check if its table was completely free (occupied count becomes 1 from 0). If so, we remove it from t=0 set. We can find it in the set by its key (distance, x, y). Since we know the table's (x,y), we can compute its best cell's distance = 3x+3y+2, and then do a set.erase() with that key. That's O(log N).

        // For t=1, we need to remove the chosen cell from the set. We can store cells in a set with key (distance, x, y). When we pop the smallest, we remove it. If a table becomes partially occupied, its other cells remain in the set. That's fine.

        // One issue: When a t=0 guest picks a table, we need to occupy its best cell (3x+1,3y+1). That cell is in the t=1 set. We must remove it from there as well. So we need to erase that specific cell from the t=1 set. We can do that by its key.

        // So the algorithm per test case:
        // 1. Precompute all tables with x+y <= MAX_SUM. For each, insert all 4 cells into free_cells set. Insert table into free_tables set.
        // 2. Initialize occupied count for each table to 0.
        // 3. For each guest:
        //    if t == 1:
        //        cell = *free_cells.begin()
        //        free_cells.erase(free_cells.begin())
        //        output cell.x, cell.y
        //        determine table (x,y) from cell: x = (cell.x - 1)/3, y = (cell.y - 1)/3 (since cell.x = 3x+1 or 3x+2, but (cell.x-1)/3 gives x for both because integer division truncates)
        //        Actually: if cell.x % 3 == 1 or 2, (cell.x-1)/3 gives x. Similarly for y.
        //        increment occupied[table]
        //        if occupied[table] == 1:
        //            // table was completely free, remove from free_tables
        //            free_tables.erase({3*x+3*y+2, 3*x+1, 3*y+1}) // key is (distance, cell.x, cell.y) of best cell
        //    else: // t == 0
        //        table_key = *free_tables.begin() // (distance, best_cell.x, best_cell.y)
        //        free_tables.erase(free_tables.begin())
        //        x = (table_key.x - 1)/3, y = (table_key.y - 1)/3
        //        // occupy the best cell
        //        free_cells.erase({table_key.distance, table_key.x, table_key.y})
        //        occupied[{x,y}] = 1 (or increment to 1)
        //        output table_key.x, table_key.y
        //        // Note: the other 3 cells of this table remain in free_cells.

        // But wait: What if the best cell of a table was already occupied by a previous t=1 guest? That can't happen because the table was in free_tables, meaning it was completely unoccupied. So its best cell is definitely free.

        // What about the case when a t=1 guest occupies a cell that is not the best cell of a completely free table? Then occupied count becomes 1, and we remove the table from free_tables. That's handled.

        // What if a t=1 guest occupies a cell of a table that is already partially occupied? Then occupied count goes from 1 to 2, etc. free_tables is not affected.

        // This seems correct.

        // Now, we need to ensure we have enough tables. If during the process, free_cells or free_tables becomes empty, we need to add more tables with higher x+y. We can do this dynamically: keep track of the current max_sum, and when we run out, increase max_sum and add new tables. But as argued, 50000 guests will not exhaust tables with x+y <= 250. Let's verify: Number of cells with x+y <= S is sum_{s=0}^S (s+1)*4? Actually, number of tables with x+y = s is s+1. Each table has 4 cells. So total cells = 4 * sum_{s=0}^S (s+1) = 4 * (S+1)(S+2)/2 = 2*(S+1)(S+2). For S=250, that's 2*251*252 = 126504 cells. 50000 guests will use at most 50000 cells. So we are safe. We can just precompute up to S=250. To be extra safe, we can set S=300.

        // Let's set MAX_SUM = 300.

        // Now, the sets: We need to order by distance, then x, then y. Distance for a cell is x+y. For a table, distance is 3x+3y+2, and the best cell is (3x+1,3y+1). So we can use a tuple (distance, x, y) for both, where for cells x and y are the cell coordinates, for tables x and y are the best cell coordinates.

        // We'll use std::set<tuple<int,int,int>> for both.

        // Let's code.

        // One detail: The tie-breaking: smallest x, then smallest y. Our tuple (distance, x, y) naturally sorts by distance first, then x, then y. That's correct.

        // For t=0, the guest wants the nearest table cell that belongs to a completely unoccupied table. The distance to that table is the distance to its nearest cell, which is (3x+1,3y+1) with distance 3x+3y+2. So using that cell as the representative is correct. The guest will occupy that cell. So output is (3x+1, 3y+1).

        // Let's test with sample.

        // Sample 1:
        // 6 guests: 0 1 1 0 0 1
        // Precompute tables up to sum 300.
        // Guest 1 t=0: free_tables smallest is table (0,0) with best cell (1,1) dist 2. Occupy (1,1). Remove table from free_tables. Remove cell (1,1) from free_cells. Output 1 1.
        // Guest 2 t=1: free_cells smallest: (1,2) dist 3, (2,1) dist 3. Smallest x is 1, so (1,2). Output 1 2. Remove from free_cells. Table (0,0) occupied count becomes 2. (was 1, now 2). free_tables unchanged.
        // Guest 3 t=1: free_cells smallest: (2,1) dist 3. Output 2 1. Remove. Table (0,0) occupied count 3.
        // Guest 4 t=0: free_tables smallest: table (0,1) best cell (1,4) dist 5, table (1,0) best cell (4,1) dist 5. Smallest x is 1, so (1,4). Output 1 4. Remove table from free_tables, remove cell (1,4) from free_cells. Table (0,1) occupied count=1.
        // Guest 5 t=0: free_tables smallest: table (1,0) best cell (4,1) dist 5. Output 4 1. Remove table, remove cell (4,1). Table (1,0) occupied=1.
        // Guest 6 t=1: free_cells smallest: (1,5) dist 6, (2,2) dist 4? Wait, (2,2) is from table (0,0) and is still free? Table (0,0) had cells (1,1),(1,2),(2,1),(2,2). We occupied (1,1),(1,2),(2,1). So (2,2) is free with dist 4. But sample says guest 6 picks (1,5) dist 6. Why not (2,2)? Because (2,2) has distance 4, which is smaller than 6. But sample output says (1,5). Let's check sample explanation: "The distance from the sixth guest to the cell (1,5) is 6, as is the distance to the cell (2,2), but since the first coordinate is smaller, he will choose the first option." Wait, distance to (2,2) is 4, not 6. Let's recalc: (2,2) distance = 2+2=4. (1,5) distance = 1+5=6. So (2,2) is closer. Why did they say distance to (2,2) is 6? Let's re-read the problem statement carefully.

        // "The distance is defined as the smallest number of steps needed to reach the table cell."
        // Steps: move to neighboring by side corridor cell, and in their last step, they must move to a neighboring by side a free table cell.
        // So the path must be entirely in corridors except the last step into the table cell.
        // The table cells are (3x+1,3y+1), (3x+1,3y+2), (3x+2,3y+1), (3x+2,3y+2).
        // Corridors are all other cells.
        // So to reach (2,2), is (2,2) a corridor or a table cell? (2,2) is not of the form (3x+1,3y+1) etc. because 2 mod 3 = 2, but we need 3x+1 or 3x+2. For x=0, 3*0+1=1, 3*0+2=2. So (2,2) could be (3*0+2, 3*0+2) which is a table cell of table (0,0). Yes, (2,2) is a table cell. So distance to (2,2) is 4? But wait, the path must go through corridors. The cell (0,0) is a corridor? (0,0) is not a table cell because table cells have x,y >=1. So (0,0) is a corridor. From (0,0), we can move to (1,0) [corridor], (2,0) [corridor? (2,0) is not a table cell because y=0, table cells have y>=1. So (2,0) is corridor], then (2,1) [table cell? (2,1) is table cell of table (0,0). But we cannot step into (2,1) because that's a table cell and we must only step into a table cell on the last step. So we must avoid table cells until the last step. So the path must only use corridor cells. Are (1,0) and (2,0) corridors? Yes. Then from (2,0) we can step to (2,1) which is a table cell. That's distance 3: (0,0)->(1,0)->(2,0)->(2,1). But (2,1) is occupied by guest 3. So we can't go there. What about (2,2)? To reach (2,2), we could go (0,0)->(1,0)->(2,0)->(2,1) but (2,1) is occupied, so we can't step there. We could go (0,0)->(0,1)->(0,2)->(1,2) but (1,2) is occupied. So we need a path that avoids occupied table cells. The distance is the shortest path in the grid where we can only traverse corridor cells, and the final step enters an unoccupied table cell. Occupied table cells are obstacles. So the distance is not simply Manhattan distance; it's the shortest path avoiding obstacles (occupied table cells) and other table cells? Wait, can we step on unoccupied table cells before the last step? The problem says: "In one step, they can move to any neighboring by side corridor cell, and in their last step, they must move to a neighboring by side a free table cell." This implies that during the walk, they can only move to corridor cells. They cannot step on table cells (even free ones) until the very last step. So all table cells are obstacles except the destination. So the distance is the length of the shortest path from (0,0) to a free table cell, where the path consists entirely of corridor cells except the final step into the table cell. This is equivalent to: the distance is the Manhattan distance to the table cell if there is a corridor path of that length, but we must ensure the path doesn't go through other table cells. Since table cells are scattered, the shortest path might be blocked by other table cells.

        // This changes everything! The hints about distances increasing/decreasing by 1 now make sense in the context of obstacles.

        // Let's re-evaluate. The grid has table cells at specific positions. All other cells are corridors. The guests start at (0,0) which is a corridor. They can move up/right? Actually, they can move to any neighboring side cell (up, down, left, right). But since all table cells are at positive coordinates, and (0,0) is at the bottom-left? The problem says x,y are non-negative integers. So the grid is the first quadrant. The corridors are all cells not of the form (3x+1,3y+1), (3x+1,3y+2), (3x+2,3y+1), (3x+2,3y+2). So the table cells form 2x2 blocks at coordinates with x,y ≡ 1,2 mod 3. The corridors are the rest.

        // The distance from (0,0) to a table cell is the shortest path using only corridor cells, plus one final step into the table cell. This is equivalent to the shortest path in the grid where table cells are walls (cannot be entered except the destination). But wait, the destination is a table cell, and we can enter it on the last step. So we can think of the distance as the shortest path from (0,0) to a cell adjacent to the destination table cell, plus 1. The adjacent cells must be corridors.

        // This is a classic BFS problem on an infinite grid with obstacles. But the obstacles are regular (2x2 blocks every 3 cells). The hints suggest a pattern.

        // Let's analyze the geometry. The table cells are at:
        // (1,1), (1,2), (2,1), (2,2) for table (0,0)
        // (1,4), (1,5), (2,4), (2,5) for table (0,1)
        // (4,1), (4,2), (5,1), (5,2) for table (1,0)
        // (4,4), (4,5), (5,4), (5,5) for table (1,1)
        // etc.
        // The corridors are all other cells. Notice that the corridors form a grid of "streets" of width 1 between the 2x2 blocks. Specifically, the lines x=0, y=0 are corridors. The lines x=3k, y=3k are corridors? Let's check: x=3, y=3: (3,3) is not a table cell because 3 mod 3 = 0. So (3,3) is corridor. The cells with x=3k or y=3k are corridors. Also cells with x=3k+1, y=3k+3? Actually, the pattern is: every third row and column is entirely corridor. The other rows/columns have alternating table cells and corridors.

        // The distance from (0,0) to a table cell is the Manhattan distance if there were no obstacles, but because the 2x2 blocks are obstacles, you might have to go around them. However, since the blocks are separated by corridors, you can always go around. The shortest path will essentially go along the corridors.

        // Let's compute distances to some cells:
        // (1,1): path: (0,0)->(1,0)->(1,1) [distance 2]. (1,0) is corridor? x=1,y=0: 1 mod 3 =1, 0 mod 3=0 -> not a table cell because y=0. So yes, corridor. So distance 2.
        // (1,2): (0,0)->(1,0)->(2,0)->(2,1)->(1,1)? No, (2,1) is table cell, can't step there. Alternative: (0,0)->(0,1)->(0,2)->(1,2) [distance 3]. (0,1) is corridor? x=0, y=1: 0 mod 3=0, 1 mod 3=1 -> not table cell. (0,2): 0,2 -> corridor. (1,2): table cell. So distance 3.
        // (2,1): symmetric, distance 3.
        // (2,2): (0,0)->(1,0)->(2,0)->(3,0)->(3,1)->(3,2)->(2,2)? Let's find shortest. (2,2) is table cell. Adjacent corridors: (1,2) is table cell, (2,1) table cell, (3,2) corridor? (3,2): 3 mod 3=0, 2 mod 3=2 -> corridor. (2,3): corridor. So we can reach (3,2) or (2,3). Path to (3,2): (0,0)->(1,0)->(2,0)->(3,0)->(3,1)->(3,2) -> step to (2,2). That's 6 steps? Let's count: (0,0) to (1
