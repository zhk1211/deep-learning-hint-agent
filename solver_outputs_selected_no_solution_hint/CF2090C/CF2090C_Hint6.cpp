#include <bits/stdc++.h>
using namespace std;

struct Cell {
    int x, y;
    bool operator<(const Cell& o) const {
        if (x != o.x) return x < o.x;
        return y < o.y;
    }
    bool operator==(const Cell& o) const {
        return x == o.x && y == o.y;
    }
};

int dist(int x, int y) {
    return x + y;
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

        // tables: each table is represented by its top-left cell (3x+1, 3y+1)
        // table cells: (3x+1,3y+1), (3x+1,3y+2), (3x+2,3y+1), (3x+2,3y+2)
        // We maintain sets of available table cells for type 0 and type 1.
        // For type 0: need completely unoccupied table -> all 4 cells free.
        // For type 1: any free table cell.

        // We'll maintain a set of free table cells (for type 1) and a set of free tables (for type 0).
        // A table is identified by (x,y) where its top-left is (3x+1,3y+1).
        // We'll generate candidates on the fly using BFS-like expansion from (0,0).
        // Since n <= 50000 total, we can precompute enough tables.

        // We'll maintain a priority queue of candidate table cells for type 1,
        // and a priority queue of candidate tables for type 0.
        // Distance to a table cell (cx,cy) is cx+cy.
        // For type 0, distance to a table is min distance among its 4 cells.
        // But the guest chooses the specific cell with smallest x then y among those at min distance.
        // Actually, for a completely unoccupied table, the guest will sit at one of its cells.
        // The problem says: "walk to the nearest table cell that belongs to a completely unoccupied table."
        // So they choose a table cell that is part of a completely unoccupied table.
        // The distance is to that cell. They choose the cell with smallest distance, then smallest x, then smallest y.
        // So we can treat each table as providing 4 candidate cells, but only if the table is completely free.
        // For type 1, any free table cell.

        // We'll maintain:
        // - set of free table cells (type 1 candidates)
        // - set of free tables (type 0 candidates) where each table has all 4 cells free.
        // When a guest sits, we remove the chosen cell from free cells, and if it was part of a free table, that table is no longer completely free.

        // To generate candidates efficiently, we can precompute tables in order of distance.
        // Distance to a table cell (3x+1,3y+1) is 3x+3y+2.
        // The minimum distance among the 4 cells of table (x,y) is 3x+3y+2 (top-left) or 3x+3y+3? Let's check:
        // (3x+1,3y+1) -> sum = 3x+3y+2
        // (3x+1,3y+2) -> sum = 3x+3y+3
        // (3x+2,3y+1) -> sum = 3x+3y+3
        // (3x+2,3y+2) -> sum = 3x+3y+4
        // So min distance is 3x+3y+2.
        // We can generate tables in increasing order of (x+y) and then x.

        // We'll maintain an index of the next table to consider for expansion.
        // Since n is up to 50000, we might need up to 50000 tables? Actually each guest takes one cell, so at most 50000 cells.
        // A table has 4 cells, so at most 12500 tables fully occupied. But type 0 might require new tables.
        // We'll generate tables lazily.

        // Let's keep a vector of tables generated so far.
        struct Table {
            int x, y; // table coordinates (3x+1,3y+1) top-left
            bool fully_free = true;
            int occupied_mask = 0; // bitmask of occupied cells: 0: (1,1), 1: (1,2), 2: (2,1), 3: (2,2)
        };
        vector<Table> tables;
        // map from table index to its index in tables vector
        map<pair<int,int>, int> table_idx;

        // free cells set: we can use a set of cells ordered by (dist, x, y)
        // For type 1, we need to pick the cell with smallest dist, then x, then y.
        // We'll maintain a set of free cells.
        struct CellInfo {
            int x, y;
            int table_id;
            int cell_type; // 0..3
            bool operator<(const CellInfo& o) const {
                int d1 = x+y, d2 = o.x+o.y;
                if (d1 != d2) return d1 < d2;
                if (x != o.x) return x < o.x;
                return y < o.y;
            }
        };
        set<CellInfo> free_cells;

        // For type 0, we need to pick a cell from a completely free table.
        // We'll maintain a set of completely free tables, ordered by the best cell they can offer.
        // The best cell of a table is the one with smallest distance, then x, then y.
        // For table (x,y), the cells are:
        // (3x+1,3y+1) d=3x+3y+2
        // (3x+1,3y+2) d=3x+3y+3
        // (3x+2,3y+1) d=3x+3y+3
        // (3x+2,3y+2) d=3x+3y+4
        // The best is (3x+1,3y+1) because smallest distance, then x=3x+1, y=3y+1.
        // So we can just use that cell as representative.
        struct FreeTable {
            int table_id;
            Cell best_cell;
            bool operator<(const FreeTable& o) const {
                int d1 = best_cell.x + best_cell.y;
                int d2 = o.best_cell.x + o.best_cell.y;
                if (d1 != d2) return d1 < d2;
                if (best_cell.x != o.best_cell.x) return best_cell.x < o.best_cell.x;
                return best_cell.y < o.best_cell.y;
            }
        };
        set<FreeTable> free_tables;

        // Function to add a new table if not already added
        auto add_table = [&](int tx, int ty) {
            if (table_idx.count({tx, ty})) return;
            int id = tables.size();
            tables.push_back({tx, ty, true, 0});
            table_idx[{tx, ty}] = id;
            // add its 4 cells to free_cells
            vector<Cell> cells = {
                {3*tx+1, 3*ty+1},
                {3*tx+1, 3*ty+2},
                {3*tx+2, 3*ty+1},
                {3*tx+2, 3*ty+2}
            };
            for (int i = 0; i < 4; ++i) {
                free_cells.insert({cells[i].x, cells[i].y, id, i});
            }
            // add to free_tables
            free_tables.insert({id, cells[0]});
        };

        // We need to generate tables in order of distance of their best cell.
        // We'll maintain a queue of table coordinates to add, sorted by (x+y, x).
        // Actually we can just generate them on demand: when the best candidate in free_cells or free_tables
        // has distance D, we should ensure all tables with best cell distance <= D are added.
        // But it's simpler to pre-generate enough tables. Max distance needed?
        // Each step distance increases by at most 1? Actually distances can go up to around 3*sqrt(n) maybe.
        // Since n <= 50000, max distance is around 3*50000? No, cells are sparse.
        // The number of tables within distance D is about O(D^2/9). For D=1000, ~111k tables, too many.
        // We need lazy generation.

        // We'll maintain a priority queue of tables to add, ordered by the distance of their best cell.
        // Table (x,y) best cell distance = 3x+3y+2.
        // We start with table (0,0).
        // When we need a candidate, we expand tables until the best candidate's distance is <= the next table's distance?
        // Actually we can just expand tables one by one as needed.

        // We'll use a BFS-like expansion on table coordinates (x,y).
        // Since distance = 3x+3y+2, increasing x+y increases distance.
        // We can generate tables in increasing order of (x+y), and for same sum, increasing x.
        // We'll maintain a queue of table coordinates to add.
        queue<pair<int,int>> table_queue;
        set<pair<int,int>> visited_tables;
        table_queue.push({0,0});
        visited_tables.insert({0,0});

        // We'll also need to know when to stop expanding. We'll expand until we have enough candidates.
        // For each guest, we'll ensure that the best candidate in free_cells (for type 1) or free_tables (for type 0)
        // is available. If not, we expand more tables.

        auto expand = [&]() {
            while (!table_queue.empty()) {
                auto [x, y] = table_queue.front();
                // Check if this table's best cell distance is less than or equal to the current best candidate's distance?
                // We'll just add it.
                add_table(x, y);
                table_queue.pop();
                // Add neighbors: (x+1,y) and (x,y+1)
                if (!visited_tables.count({x+1, y})) {
                    visited_tables.insert({x+1, y});
                    table_queue.push({x+1, y});
                }
                if (!visited_tables.count({x, y+1})) {
                    visited_tables.insert({x, y+1});
                    table_queue.push({x, y+1});
                }
                // We only need to add one table at a time? Actually we might need many.
                // But we can break after adding one and let the loop continue if needed.
                break;
            }
        };

        // Ensure at least one table is added
        expand();

        for (int i = 0; i < n; ++i) {
            if (t[i] == 1) {
                // type 1: nearest vacant table cell
                // Ensure free_cells is not empty
                while (free_cells.empty()) {
                    expand();
                }
                auto it = free_cells.begin();
                CellInfo chosen = *it;
                free_cells.erase(it);
                // update table
                int tid = chosen.table_id;
                Table& tb = tables[tid];
                tb.occupied_mask |= (1 << chosen.cell_type);
                if (tb.fully_free) {
                    tb.fully_free = false;
                    // remove from free_tables
                    FreeTable ft;
                    ft.table_id = tid;
                    ft.best_cell = {3*tb.x+1, 3*tb.y+1};
                    free_tables.erase(ft);
                }
                cout << chosen.x << " " << chosen.y << "\n";
            } else {
                // type 0: nearest table cell that belongs to a completely unoccupied table
                // Ensure free_tables is not empty
                while (free_tables.empty()) {
                    expand();
                }
                auto it = free_tables.begin();
                FreeTable ft = *it;
                free_tables.erase(it);
                int tid = ft.table_id;
                Table& tb = tables[tid];
                // The guest will sit at the best cell of this table: (3x+1, 3y+1)
                Cell best = {3*tb.x+1, 3*tb.y+1};
                // Remove this cell from free_cells
                CellInfo to_remove;
                to_remove.x = best.x;
                to_remove.y = best.y;
                to_remove.table_id = tid;
                to_remove.cell_type = 0;
                free_cells.erase(to_remove);
                tb.occupied_mask |= 1;
                tb.fully_free = false;
                cout << best.x << " " << best.y << "\n";
            }
        }
    }
    return 0;
}
