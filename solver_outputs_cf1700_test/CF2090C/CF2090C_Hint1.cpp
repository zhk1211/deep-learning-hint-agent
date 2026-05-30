// Hint1
#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

struct Cell {
    int x, y;
    bool operator<(const Cell& other) const {
        int d1 = x + y, d2 = other.x + other.y;
        if (d1 != d2) return d1 < d2;
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
};

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
        
        // Tables are indexed by (x, y) where table occupies:
        // (3x+1,3y+1), (3x+1,3y+2), (3x+2,3y+1), (3x+2,3y+2)
        // Distance from (0,0) to a cell (a,b) is a+b.
        // For a table at (x,y), the four cells have distances:
        // (3x+1)+(3y+1)=3(x+y)+2
        // (3x+1)+(3y+2)=3(x+y)+3
        // (3x+2)+(3y+1)=3(x+y)+3
        // (3x+2)+(3y+2)=3(x+y)+4
        // The minimum distance to reach that table is 3(x+y)+2 (to cell (3x+1,3y+1))
        // and then one step into the table cell.
        // Actually guest starts at (0,0), moves through corridors, last step into table cell.
        // Distance to table cell = distance to that cell from (0,0) via corridors.
        // Since corridors are all non-table cells, and (0,0) is corridor, distance is just Manhattan distance.
        // So distance to cell (a,b) is a+b.
        // For a table (x,y), the four cells have distances as above.
        // The guest will choose the nearest table cell, which is the one with smallest distance.
        // Among the four cells of a table, the one with smallest distance is (3x+1,3y+1) with distance 3(x+y)+2.
        // So effectively each table has a "representative" cell (3x+1,3y+1) with distance d = 3(x+y)+2.
        // But wait: the guest can choose any of the four cells, and they are all part of the same table.
        // The problem says: "they must move to a neighboring by side a free table cell."
        // So they move to a table cell, not necessarily the closest one of that table? Actually they choose the table cell that minimizes distance.
        // Since all four cells belong to the same table, the minimum distance to that table is the minimum distance among its four cells.
        // That minimum is 3(x+y)+2, achieved at (3x+1,3y+1).
        // So we can think of each table as offering its best cell (3x+1,3y+1) with distance d = 3(x+y)+2.
        // However, if that cell is occupied, the guest might choose another cell of the same table? 
        // The problem: "they occupy the chosen table cell, and no other guest can move there."
        // So if the best cell is taken, they can still choose another cell of the same table, but that would have larger distance.
        // But wait: the distance is defined as smallest number of steps to reach the table cell.
        // If the best cell is occupied, the distance to that table via another cell might be larger.
        // However, the guest chooses the nearest vacant table cell overall, not per table.
        // So if a table has its best cell occupied, its other cells are still available but with higher distance.
        // So we need to consider all individual table cells, not just the best one per table.
        // But note: the four cells of a table are at distances: d, d+1, d+1, d+2 where d = 3(x+y)+2.
        // So they are very close. The ordering by distance, then x, then y will prioritize the best cell first.
        // For t_i = 0: guest wants a completely unoccupied table. That means all four cells of that table are free.
        // So they will only consider tables where none of its four cells are occupied.
        // For t_i = 1: guest wants any vacant table cell.
        
        // We need to efficiently find the next cell according to the rules.
        // Since sum n <= 50000, we can simulate with a priority queue or set.
        // The number of tables that could be used is limited because distances grow.
        // Max distance needed: for n=50000, we might need up to distance around maybe a few hundred.
        // We can generate candidate cells on the fly.
        
        // We'll maintain a set of available cells for type 1, and a set of available tables for type 0.
        // But careful: for type 0, the table must be completely unoccupied. Once a cell of a table is taken, that table is no longer completely unoccupied.
        // However, later if all cells become free again? No, cells are never freed. So once a table is partially occupied, it's never completely unoccupied again.
        // So we can maintain a set of completely free tables.
        // For type 1, we need to consider all individual cells. When a cell is taken, we remove it from available cells.
        // But also, if a cell is taken, its table is no longer completely free, so we remove that table from the type 0 set.
        
        // We need to generate cells as needed. Since distances increase, we can pre-generate a sufficient number of tables.
        // Let's estimate max distance. For n=50000, the number of cells within distance D is roughly O(D^2). 
        // Distance D corresponds to x+y = D. Number of table cells with distance <= D: each table contributes up to 4 cells, but tables are spaced by 3.
        // Roughly, number of tables with 3(x+y)+2 <= D => x+y <= (D-2)/3. Number of such tables is about ((D-2)/3)^2 / 2? Actually x,y >=0, so number of pairs (x,y) with x+y <= K is (K+1)(K+2)/2.
        // So for D around 600, K ~ 200, number of tables ~ 20000, cells ~ 80000. That should be enough for n=50000.
        // We can pre-generate all tables up to some limit, say x+y <= 200 (which gives max distance 3*200+4=604). That's about 20000 tables, 80000 cells. That's fine.
        // But we need to generate them in order of distance, then x, then y.
        // We can just iterate over all possible tables, and for each table, generate its four cells in order of distance (which is also the tie-breaking order: smaller distance first, then smaller x, then smaller y).
        // The four cells sorted by (distance, x, y):
        // (3x+1,3y+1): dist = 3(x+y)+2
        // (3x+1,3y+2): dist = 3(x+y)+3, x same, y larger
        // (3x+2,3y+1): dist = 3(x+y)+3, x larger, y smaller
        // (3x+2,3y+2): dist = 3(x+y)+4
        // So the order is exactly: (1,1), (1,2), (2,1), (2,2) for the offsets.
        // So we can generate all cells in the correct global order by iterating over sum s = x+y from 0 upwards, and for each s, iterate x from 0 to s, y = s-x.
        // For each table (x,y), push the four cells in order.
        // This will generate cells in increasing distance, and for same distance, increasing x, then increasing y? Let's check:
        // For two tables with same s = x+y, the distances of their cells:
        // The first cell of table (x,y) has distance 3s+2. So all first cells of tables with same s have same distance.
        // Among those, we compare x of the cell: 3x+1. So smaller table x gives smaller cell x. So if we iterate x from 0 to s, we get increasing cell x.
        // For same table x, cell y = 3y+1, and since y = s-x, larger x means smaller y. But we iterate x increasing, so y decreasing. However, tie-breaking: after distance, we compare x, then y. Since x is increasing, we will correctly order by x first. For same x? Different tables with same x but different y: but if x is same, then y = s-x is same, so only one table. So no tie in x among first cells of same s.
        // What about second cells (1,2) and (2,1)? They have distance 3s+3. For a given table, (1,2) has x=3x+1, y=3y+2; (2,1) has x=3x+2, y=3y+1. Compare (1,2) of table (x1,y1) and (2,1) of table (x2,y2). They could have same distance if s1=s2. We need to ensure global order. Our generation order: for each table (x,y) in increasing x (for fixed s), we push its four cells in the order: (1,1), (1,2), (2,1), (2,2). This will interleave cells from different tables. Is the global order correct?
        // Let's test with s=0: table (0,0). Cells: (1,1) dist 2; (1,2) dist 3; (2,1) dist 3; (2,2) dist 4.
        // s=1: tables (0,1) and (1,0).
        // Table (0,1): (1,4) dist 5; (1,5) dist 6; (2,4) dist 6; (2,5) dist 7.
        // Table (1,0): (4,1) dist 5; (4,2) dist 6; (5,1) dist 6; (5,2) dist 7.
        // Global order by distance, then x, then y:
        // dist 2: (1,1)
        // dist 3: (1,2), (2,1)
        // dist 4: (2,2)
        // dist 5: (1,4), (4,1) -> compare x: 1<4, so (1,4) then (4,1)
        // dist 6: (1,5) [x=1], (2,4) [x=2], (4,2) [x=4], (5,1) [x=5] -> order: (1,5), (2,4), (4,2), (5,1)
        // dist 7: (2,5), (5,2)
        // Our generation: s=0: (1,1), (1,2), (2,1), (2,2)
        // s=1: x=0: (1,4), (1,5), (2,4), (2,5); x=1: (4,1), (4,2), (5,1), (5,2)
        // Concatenating: (1,1), (1,2), (2,1), (2,2), (1,4), (1,5), (2,4), (2,5), (4,1), (4,2), (5,1), (5,2)
        // This is not correct: (4,1) should come before (1,5) because dist 5 < 6. But in our list, (1,5) comes before (4,1). So the simple concatenation does not yield correct global order because we are grouping by table, and within same s, we process all cells of table x=0 before table x=1. That breaks distance order because (1,5) has dist 6 while (4,1) has dist 5.
        // So we need to generate all cells sorted globally by (distance, x, y). We can just generate all possible cells up to some limit, put them in a vector, and sort them once. Since total cells ~80000, sorting is fine.
        // But we need to know the mapping from cell to table to handle type 0. We can assign a table ID to each cell.
        // Let's define table ID as a unique integer for each (x,y). We can use a map or just encode as x*1000+y (since x,y up to ~200).
        // We'll maintain:
        // - occupied_cells: set of cells that are taken.
        // - completely_free_tables: set of table IDs that have 0 occupied cells.
        // - available_cells: a priority queue or sorted list of cells that are not occupied. But we need to efficiently get the minimum according to the criteria. Since we only add cells as we need them, we can maintain a pointer to the next cell in the globally sorted list, and skip occupied ones.
        // Actually, we can pre-sort all cells. Then we maintain an index pointer for type 1 queries: the next available cell is the first cell in the sorted list that is not occupied. Since we only take cells, the pointer only moves forward. We can just iterate through the sorted list and skip occupied cells.
        // For type 0 queries: we need the minimum distance table that is completely free. We can also pre-sort tables by the distance of their best cell (3x+3y+2), and then by x, then y. Actually the tie-breaking for tables: "nearest table cell that belongs to a completely unoccupied table." The distance is to the table cell. So they will choose the best cell of that table (the one with smallest distance among its four cells). Since the table is completely unoccupied, all its cells are free, so they will naturally pick the best cell (3x+1,3y+1) because it has the smallest distance. So for a completely free table, the guest will always sit at (3x+1,3y+1). So we can think of the table as offering that cell. The tie-breaking among tables: if multiple tables have the same distance to their best cell, we compare the coordinates of that best cell (x, y). So we can sort tables by (distance_to_best_cell, best_cell_x, best_cell_y). Then for type 0, we pick the first table in that sorted list that is still completely free. When a cell is occupied, we mark its table as not completely free (if it was). We can maintain a set of completely free tables, and for type 0, we need the minimum table according to the order. We can use a priority queue or just a sorted list and a pointer, similar to type 1, but we need to remove tables that become partially occupied. Since a table becomes partially occupied only when one of its cells is taken, we can just check if the table is still completely free when we consider it. We can maintain a boolean array for tables: completely_free[table_id]. Initially all true. When a cell is taken, we set completely_free[table_id] = false. For type 0, we iterate through the sorted tables list, skip those with completely_free false, and take the first one. Since we only take one table per type 0 query, and tables are only removed from consideration, the pointer moves forward.
        // However, note: a type 0 guest occupies a cell of that table. That cell is the best cell (3x+1,3y+1). That cell might also be the next available cell for type 1. So we need to mark that specific cell as occupied, and also remove it from the type 1 available cells. Our type 1 pointer will naturally skip it if it's occupied.
        // So algorithm:
        // 1. Precompute all tables up to some limit. Determine limit: we need enough tables to accommodate n guests. Since each guest takes one cell, and there are up to 50000 guests, we need at least 50000 cells. But type 0 guests take a whole table (but only occupy one cell initially, though the table is no longer completely free). Actually a type 0 guest only occupies one cell, but the table becomes partially occupied. Other guests can still take the remaining cells. So the total number of cells needed is n. So we need at least n cells. With max distance around 600, we have ~80000 cells, which is >50000. So we can set limit such that we generate enough cells. We can dynamically generate until we have enough cells, but pre-generating a fixed large number is easier. Let's set max_sum = 200 (x+y <= 200). Number of tables = (200+1)*(200+2)/2 = 20301. Cells = 4 * 20301 = 81204. That's safe.
        // 2. Generate all cells: for x from 0 to max_sum, for y from 0 to max_sum - x? Actually we want all tables with x+y <= max_sum. So for s from 0 to max_sum, for x from 0 to s, y = s-x. For each, generate the four cells in order: (3x+1,3y+1), (3x+1,3y+2), (3x+2,3y+1), (3x+2,3y+2). Store them in a vector with their table_id (we can encode table_id = x * (max_sum+1) + y or just use a map from cell to table_id). Also store the table's best cell for sorting tables.
        // 3. Sort all cells by (distance, x, y). Distance = x+y.
        // 4. Create a vector of tables sorted by (distance of best cell, best cell x, best cell y). The best cell is (3x+1,3y+1). Distance = 3(x+y)+2.
        // 5. Initialize:
        //    - occupied = vector<bool>(max_cells? Actually we can use a set or a 2D array. Since coordinates up to 3*200+2=602, we can use a 2D boolean array of size 605x605. That's small.
        //    - completely_free = vector<bool>(num_tables, true). We need to map table (x,y) to an index. We can assign index = x * (max_sum+1) + y, but max_sum+1=201, so max index ~ 200*201+200 = 40400. That's fine.
        //    - ptr1 = 0 for type 1 cells.
        //    - ptr0 = 0 for type 0 tables.
        // 6. For each guest:
        //    if t_i == 1:
        //        while occupied[ cells[ptr1] ] is true, ptr1++.
        //        cell = cells[ptr1].
        //        mark cell occupied.
        //        table_id = table_of[cell].
        //        completely_free[table_id] = false.
        //        output cell.x, cell.y.
        //    else (t_i == 0):
        //        while not completely_free[ tables[ptr0].id ], ptr0++.
        //        table = tables[ptr0].
        //        cell = table.best_cell.
        //        mark cell occupied.
        //        completely_free[table.id] = false.
        //        output cell.x, cell.y.
        //        Note: we don't advance ptr0 because we might have skipped some tables that are not free, but we only advance when the current table is not free. Actually we do advance ptr0 in the while loop. After taking the table, ptr0 stays at that table, but it's no longer free, so next type 0 will skip it. So we don't need to increment ptr0 after taking.
        //    However, we must ensure that the cell we take for type 0 is not already occupied (it shouldn't be, because the table was completely free). But just in case, we can assert.
        // 7. That's it.
        
        // But wait: what if a type 0 guest takes a cell that is not the next in the type 1 order? That's fine, type 1 will just skip it.
        // What if a type 1 guest takes a cell that belongs to a table that was previously completely free? Then that table becomes partially occupied, so we set completely_free false. That's handled.
        // What if a type 1 guest takes a cell that is the best cell of a table, but the table was already partially occupied? Then completely_free is already false, no change.
        // This seems correct.
        
        // Let's implement.
        
        const int MAX_SUM = 200;
        // Precompute tables
        struct Table {
            int id; // index
            int best_x, best_y;
            int dist; // distance of best cell
        };
        vector<Cell> all_cells;
        vector<Table> all_tables;
        // We'll assign table id = x * (MAX_SUM+1) + y
        int table_id_bound = (MAX_SUM+1)*(MAX_SUM+1);
        vector<int> cell_to_table; // will be sized after we know number of cells, or we can use a map from pair to table id. Since coordinates up to 606, we can use 2D array: table_of[x][y].
        // Actually we can just compute table id from cell coordinates: given cell (cx, cy), we can find which table it belongs to.
        // The table cells are of the form (3x+1,3y+1), (3x+1,3y+2), (3x+2,3y+1), (3x+2,3y+2).
        // So given (cx, cy), we can find x = (cx-1)/3, y = (cy-1)/3. But careful: if cx-1 is not divisible by 3? Actually all table cells satisfy cx%3 == 1 or 2, and similarly for cy. So (cx-1)/3 or (cx-2)/3? Let's derive: For (3x+1,3y+1): x = (cx-1)/3, y = (cy-1)/3. For (3x+1,3y+2): x = (cx-1)/3, y = (cy-2)/3. For (3x+2,3y+1): x = (cx-2)/3, y = (cy-1)/3. For (3x+2,3y+2): x = (cx-2)/3, y = (cy-2)/3.
        // In all cases, x = (cx-1)/3 (integer division) if cx%3==1, else (cx-2)/3 if cx%3==2. Actually we can just do x = (cx-1)/3 for cx%3==1, and (cx-2)/3 for cx%3==2. But since we only generate these cells, we can just store table id in a 2D array of size 610x610, initialized to -1.
        vector<vector<int>> table_of(610, vector<int>(610, -1));
        
        for (int s = 0; s <= MAX_SUM; ++s) {
            for (int x = 0; x <= s; ++x) {
                int y = s - x;
                int cx = 3*x + 1, cy = 3*y + 1;
                // best cell
                int best_x = cx, best_y = cy;
                int dist_best = 3*s + 2;
                int tid = x * (MAX_SUM+1) + y;
                all_tables.push_back({tid, best_x, best_y, dist_best});
                // four cells
                all_cells.push_back({cx, cy});
                table_of[cx][cy] = tid;
                all_cells.push_back({cx, cy+1});
                table_of[cx][cy+1] = tid;
                all_cells.push_back({cx+1, cy});
                table_of[cx+1][cy] = tid;
                all_cells.push_back({cx+1, cy+1});
                table_of[cx+1][cy+1] = tid;
            }
        }
        
        // Sort all_cells by (distance, x, y)
        sort(all_cells.begin(), all_cells.end(), [](const Cell& a, const Cell& b) {
            int da = a.x + a.y, db = b.x + b.y;
            if (da != db) return da < db;
            if (a.x != b.x) return a.x < b.x;
            return a.y < b.y;
        });
        
        // Sort all_tables by (dist_best, best_x, best_y)
        sort(all_tables.begin(), all_tables.end(), [](const Table& a, const Table& b) {
            if (a.dist != b.dist) return a.dist < b.dist;
            if (a.best_x != b.best_x) return a.best_x < b.best_x;
            return a.best_y < b.best_y;
        });
        
        // Occupancy
        vector<vector<bool>> occ(610, vector<bool>(610, false));
        vector<bool> comp_free(table_id_bound, true); // initially all tables completely free
        
        int ptr1 = 0, ptr0 = 0;
        int num_cells = all_cells.size();
        int num_tables = all_tables.size();
        
        for (int i = 0; i < n; ++i) {
            if (t[i] == 1) {
                while (ptr1 < num_cells && occ[all_cells[ptr1].x][all_cells[ptr1].y]) {
                    ptr1++;
                }
                // assert(ptr1 < num_cells);
                Cell c = all_cells[ptr1];
                occ[c.x][c.y] = true;
                int tid = table_of[c.x][c.y];
                comp_free[tid] = false;
                cout << c.x << " " << c.y << "\n";
                ptr1++; // move to next for future
            } else {
                while (ptr0 < num_tables && !comp_free[all_tables[ptr0].id]) {
                    ptr0++;
                }
                // assert(ptr0 < num_tables);
                Table tb = all_tables[ptr0];
                Cell c = {tb.best_x, tb.best_y};
                occ[c.x][c.y] = true;
                comp_free[tb.id] = false;
                cout << c.x << " " << c.y << "\n";
                // ptr0 remains, because this table is no longer free, next type 0 will skip it.
            }
        }
    }
    return 0;
}
