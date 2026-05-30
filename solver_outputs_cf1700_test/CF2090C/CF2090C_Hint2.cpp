// Hint2
#include <bits/stdc++.h>
using namespace std;

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
        // We'll map table cells to table index (x,y)
        // occupied[x][y] = bitset of 4 cells: bit0:(1,1), bit1:(1,2), bit2:(2,1), bit3:(2,2)
        // We'll store occupied tables in a map for sparse usage.
        unordered_map<int, unordered_map<int, int>> occupied; // occupied[x][y] bitmask
        
        auto get_mask = [&](int x, int y) -> int& {
            return occupied[x][y];
        };
        
        auto is_table_fully_free = [&](int x, int y) -> bool {
            auto it = occupied.find(x);
            if (it == occupied.end()) return true;
            auto it2 = it->second.find(y);
            if (it2 == it->second.end()) return true;
            return it2->second == 0;
        };
        
        auto is_cell_occupied = [&](int x, int y, int dx, int dy) -> bool {
            // dx,dy in {1,2}
            int bit = (dx == 1 ? 0 : 2) + (dy == 1 ? 0 : 1);
            auto it = occupied.find(x);
            if (it == occupied.end()) return false;
            auto it2 = it->second.find(y);
            if (it2 == it->second.end()) return false;
            return (it2->second >> bit) & 1;
        };
        
        auto set_cell_occupied = [&](int x, int y, int dx, int dy) {
            int bit = (dx == 1 ? 0 : 2) + (dy == 1 ? 0 : 1);
            get_mask(x, y) |= (1 << bit);
        };
        
        // We'll generate candidate cells in BFS order from (0,0) but only corridor cells.
        // Actually we need to find nearest table cell satisfying condition.
        // Distance to table cell (3x+dx, 3y+dy) is (3x+dx) + (3y+dy) because all moves are along corridors
        // and corridors are all cells not in tables. The shortest path from (0,0) to any cell in this grid
        // is Manhattan distance because there are no obstacles? Wait, tables block movement?
        // "In one step, they can move to any neighboring by side corridor cell"
        // So they cannot step on table cells except the final step. So tables are obstacles.
        // But the grid is infinite and tables are 2x2 blocks separated by corridors.
        // The distance is the shortest path avoiding table cells.
        // However, note that (0,0) is a corridor? (0,0) is not in any table because tables start at x>=1,y>=1.
        // The distance to a table cell (X,Y) is X+Y? Let's check: from (0,0) to (1,1) distance 2.
        // (1,1) is table cell. Path: (0,0)->(0,1)->(1,1) or (1,0)->(1,1). Both length 2. Manhattan distance = 2.
        // To (1,2): distance 3. Manhattan = 3.
        // To (2,1): distance 3.
        // To (1,4): distance 5. Manhattan = 5.
        // So distance = X+Y. Is that always true? Since corridors are all cells not in tables, and tables are 2x2 blocks,
        // the grid is essentially a lattice where you can walk around tables. The shortest path to a table cell
        // from (0,0) is indeed Manhattan distance because you can always go along axes without entering tables?
        // Let's verify: to reach (1,4) = (3*0+1, 3*1+1). Path: (0,0)->(0,1)->(0,2)->(0,3)->(0,4)->(1,4) length 5.
        // (0,2) is corridor? (0,2): x=0, not in any table. (0,3): x=0, not in table. (0,4): x=0, not in table.
        // So yes, you can go along x=0 up to y=4 then step right to (1,4). So distance = X+Y.
        // But wait: what if there is a table blocking? Tables are at (3x+1,3y+1) etc. The corridor cells include all
        // cells with x=0 or y=0, and cells with x%3==0 or y%3==0? Actually tables occupy cells where both x and y are not divisible by 3?
        // Table cells: (3x+1,3y+1), (3x+1,3y+2), (3x+2,3y+1), (3x+2,3y+2). So cells with x%3==0 or y%3==0 are corridors.
        // So from (0,0) you can always move along x=0 (all y are corridors) and then step into table cell.
        // So distance = X+Y. This holds for any table cell because you can go along axes.
        // Therefore, distance to table cell (X,Y) is X+Y.
        // The problem says: "The distance is defined as the smallest number of steps needed to reach the table cell."
        // So we can just use X+Y.
        
        // For t=1: nearest vacant table cell (any cell of a table, not necessarily whole table free).
        // For t=0: nearest table cell that belongs to a completely unoccupied table (all 4 cells free).
        
        // We need to find the cell with smallest distance (X+Y), then smallest X, then smallest Y.
        // Since distance = X+Y, we can iterate over possible distances d from 2 upwards.
        // For each d, we consider table cells (X,Y) with X+Y = d, X,Y >=1, and (X-1)%3 < 2, (Y-1)%3 < 2? Actually table cells are exactly those where X%3 != 0 and Y%3 != 0? Let's check: (1,1): 1%3=1,1%3=1 -> table. (1,2): 1%3=1,2%3=2 -> table. (2,1): 2%3=2,1%3=1 -> table. (2,2): 2%3=2,2%3=2 -> table. (1,3): 1%3=1,3%3=0 -> corridor. So table cells are those with X%3 != 0 and Y%3 != 0.
        // But we also need to map to table index (x,y) = ((X-1)/3, (Y-1)/3) for checking occupancy.
        
        // Since n <= 50000 total, we can precompute or generate candidates on the fly.
        // We'll maintain a set of candidate cells for t=0 and t=1? Actually we can just iterate d from 2 upwards,
        // and for each d, generate all table cells with X+Y = d, X,Y>=1, X%3!=0, Y%3!=0.
        // But d can be up to? With n=50000, maximum distance might be around? Each table has 4 cells. So 50000 guests occupy 50000 cells, which is 12500 tables. The maximum distance might be a few hundred? Let's estimate: number of table cells with distance <= D is roughly number of pairs (X,Y) with X+Y<=D, X,Y>=1, X%3!=0, Y%3!=0. That's about (2/3)^2 * (D^2/2) = 2/9 D^2. For 50000 cells, D ~ sqrt(50000*9/2) ~ sqrt(225000) ~ 474. So D up to ~500. So we can just iterate d from 2 to something like 1000, and for each d, generate all valid (X,Y) with X+Y=d, check condition, and if found, break.
        // But we need to do this for each guest. 50000 * 500 = 25e6 operations, which is fine in 1 sec in C++.
        // However, we must be careful to generate cells in order: smallest X+Y, then smallest X, then smallest Y.
        // For a fixed d, X from 1 to d-1, Y = d - X. We need X%3!=0 and Y%3!=0.
        // We can just loop X from 1 to d-1, and if both conditions hold, check occupancy.
        
        // For t=0: we need a cell from a completely free table. That means for the table index (x,y) = ((X-1)/3, (Y-1)/3), all 4 cells are unoccupied.
        // For t=1: we need a cell that is unoccupied.
        
        // We'll process guests one by one.
        for (int i = 0; i < n; ++i) {
            int type = t[i];
            Cell best = {-1, -1};
            // We'll search d from 2 upwards until we find a cell.
            for (int d = 2; ; ++d) {
                bool found = false;
                // X from 1 to d-1
                for (int X = 1; X < d; ++X) {
                    int Y = d - X;
                    if (X % 3 == 0 || Y % 3 == 0) continue;
                    // table cell
                    int tx = (X - 1) / 3;
                    int ty = (Y - 1) / 3;
                    int dx = X - 3*tx; // 1 or 2
                    int dy = Y - 3*ty; // 1 or 2
                    if (type == 0) {
                        // need completely unoccupied table
                        if (is_table_fully_free(tx, ty)) {
                            best = {X, Y};
                            found = true;
                            break;
                        }
                    } else {
                        // need unoccupied cell
                        if (!is_cell_occupied(tx, ty, dx, dy)) {
                            best = {X, Y};
                            found = true;
                            break;
                        }
                    }
                }
                if (found) break;
            }
            // occupy the cell
            int X = best.x, Y = best.y;
            int tx = (X - 1) / 3;
            int ty = (Y - 1) / 3;
            int dx = X - 3*tx;
            int dy = Y - 3*ty;
            set_cell_occupied(tx, ty, dx, dy);
            cout << X << " " << Y << "\n";
        }
    }
    return 0;
}
