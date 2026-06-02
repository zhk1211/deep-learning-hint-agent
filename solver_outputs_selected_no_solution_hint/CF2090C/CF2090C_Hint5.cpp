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

// table id = (x, y) where table occupies cells:
// (3x+1,3y+1), (3x+1,3y+2), (3x+2,3y+1), (3x+2,3y+2)
// distance from (0,0) to a table cell (r,c) is r+c
// For a table (x,y), the four cells have distances:
// (3x+1)+(3y+1)=3(x+y)+2
// (3x+1)+(3y+2)=3(x+y)+3
// (3x+2)+(3y+1)=3(x+y)+3
// (3x+2)+(3y+2)=3(x+y)+4
// The minimum distance to the table is 3(x+y)+2, achieved at (3x+1,3y+1)
// So distance to table = 3*(x+y)+2

// For type 0: nearest completely unoccupied table
// For type 1: nearest vacant table cell (any cell of a table)

// We maintain sets of available tables and cells.

struct Table {
    int x, y;
    int dist; // 3*(x+y)+2
    bool operator<(const Table& o) const {
        if (dist != o.dist) return dist < o.dist;
        if (x != o.x) return x < o.x;
        return y < o.y;
    }
};

struct TableCell {
    int x, y; // actual coordinates
    int dist; // x+y
    bool operator<(const TableCell& o) const {
        if (dist != o.dist) return dist < o.dist;
        if (x != o.x) return x < o.x;
        return y < o.y;
    }
};

void solve() {
    int n;
    cin >> n;
    vector<int> t(n);
    for (int i = 0; i < n; ++i) cin >> t[i];

    // We'll generate tables and cells on demand.
    // Keep track of next table index to consider for type 0.
    // For type 1, we need to pick the nearest free cell among all tables.
    // Since tables are infinite, we can generate candidates lazily.

    // For type 0: completely free tables.
    set<Table> free_tables;
    // For type 1: free cells from partially occupied tables.
    set<TableCell> free_cells;

    // We also need to know which tables are completely free.
    // When a table gets its first occupant, it's no longer completely free.
    // We'll maintain occupancy count per table.
    map<pair<int,int>, int> table_occ; // (x,y) -> number of occupied cells (0..4)
    // Also maintain which cells of a table are free.
    // Since n <= 50000, we can generate tables up to some bound.
    // Maximum distance needed: worst case n=50000, each guest takes a new table or cell.
    // Distance grows roughly as sqrt(n)*something. We can generate tables up to x+y <= 200 maybe.
    // Let's generate all tables with distance <= some large bound, say 3*200+2 = 602.
    // Actually, we can generate on the fly: maintain a candidate table index (s = x+y) and generate all tables with that s.

    // We'll maintain a queue of tables to add to free_tables and free_cells.
    int cur_s = 0; // current x+y for generating tables
    // We'll generate tables in increasing order of distance (which is 3*s+2).
    // For a given s, tables are (x, s-x) for x=0..s.
    // Order by x (which matches distance tie-breaking: smaller x first).

    auto generate_tables_up_to = [&](int max_dist) {
        // max_dist is the distance of table = 3*s+2
        while (true) {
            int dist = 3 * cur_s + 2;
            if (dist > max_dist) break;
            for (int x = 0; x <= cur_s; ++x) {
                int y = cur_s - x;
                Table tb{x, y, dist};
                free_tables.insert(tb);
                // add its four cells
                vector<Cell> cells = {
                    {3*x+1, 3*y+1},
                    {3*x+1, 3*y+2},
                    {3*x+2, 3*y+1},
                    {3*x+2, 3*y+2}
                };
                for (auto c : cells) {
                    free_cells.insert({c.x, c.y, c.x + c.y});
                }
                table_occ[{x, y}] = 0;
            }
            ++cur_s;
        }
    };

    // We need to answer queries. For type 0: pick the best free table.
    // For type 1: pick the best free cell.
    // After picking, update structures.

    // To avoid generating too many tables at once, we can generate enough to satisfy the next query.
    // For type 0: we need at least one free table. If none, generate more.
    // For type 1: we need at least one free cell. If none, generate more.

    for (int i = 0; i < n; ++i) {
        if (t[i] == 0) {
            // need a completely free table
            if (free_tables.empty()) {
                // generate more tables; we don't know max_dist, so generate next s
                generate_tables_up_to(3 * cur_s + 2 + 100); // generate a bunch
            }
            // pick the best
            Table best = *free_tables.begin();
            free_tables.erase(free_tables.begin());
            // occupy one cell of this table? Actually guest occupies a table cell.
            // The problem says: "they must move to a neighboring by side a free table cell. They occupy the chosen table cell"
            // For type 0, they choose a table that is completely unoccupied, and then they sit at some cell of that table.
            // Which cell? The distance to the table is defined as smallest number of steps to reach a table cell.
            // The nearest cell of that table is (3x+1,3y+1). They will sit there because it's the closest.
            // But wait: the problem says "they walk to the nearest table cell that belongs to a completely unoccupied table."
            // The distance is defined as smallest number of steps to reach the table cell. So they will pick the cell with minimal distance.
            // For a completely free table, the minimal distance cell is (3x+1,3y+1).
            // So they occupy that cell.
            Cell chosen = {3*best.x+1, 3*best.y+1};
            cout << chosen.x << " " << chosen.y << "\n";
            // update occupancy
            table_occ[{best.x, best.y}]++;
            // remove this cell from free_cells
            free_cells.erase({chosen.x, chosen.y, chosen.x + chosen.y});
            // table is no longer completely free, but we already removed it from free_tables.
            // other cells of this table remain in free_cells for type 1 guests.
        } else {
            // type 1: nearest vacant table cell
            if (free_cells.empty()) {
                generate_tables_up_to(3 * cur_s + 2 + 100);
            }
            TableCell best = *free_cells.begin();
            free_cells.erase(free_cells.begin());
            cout << best.x << " " << best.y << "\n";
            // determine which table this cell belongs to
            // cell (r,c) belongs to table ( (r-1)/3, (c-1)/3 ) because table cells are 3x+1,3x+2 etc.
            int tx = (best.x - 1) / 3;
            int ty = (best.y - 1) / 3;
            table_occ[{tx, ty}]++;
            // if this was the first occupant of this table, the table is no longer completely free.
            // remove from free_tables if present.
            Table tb{tx, ty, 3*(tx+ty)+2};
            auto it = free_tables.find(tb);
            if (it != free_tables.end()) {
                free_tables.erase(it);
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int q;
    cin >> q;
    while (q--) {
        solve();
    }
    return 0;
}
