// Hint0
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

        set<Cell> free_cells;
        set<Cell> free_tables; // completely unoccupied tables

        auto add_table = [&](int X, int Y) {
            vector<Cell> cells = {{3*X+1, 3*Y+1}, {3*X+1, 3*Y+2}, {3*X+2, 3*Y+1}, {3*X+2, 3*Y+2}};
            for (auto c : cells) free_cells.insert(c);
            free_tables.insert({3*X+1, 3*Y+1}); // representative
        };

        // Pre-generate enough tables. Max distance needed: n up to 50000, each guest moves at most ~500 steps? 
        // Actually distance can be up to ~ 2 * sqrt(n) * 3? Let's generate generously.
        // We'll generate tables on the fly when needed.
        // But to avoid complexity, we can generate all tables up to some bound.
        // Since sum n <= 50000, max distance is roughly O(sqrt(n)*3) ~ 700.
        // We'll generate tables with x+y <= 200 (in table coordinates) which is more than enough.
        int max_table_coord = 200;
        for (int X = 0; X <= max_table_coord; ++X) {
            for (int Y = 0; Y <= max_table_coord; ++Y) {
                add_table(X, Y);
            }
        }

        // We also need to keep track of which tables are completely free.
        // free_tables stores a representative cell of each completely free table.
        // When a cell is taken, we remove it from free_cells, and also remove the table from free_tables if it was there.
        // But we need to know if a table becomes partially occupied.
        // We'll maintain a map from table representative to count of occupied cells.
        map<Cell, int> occupied_count; // representative -> number of occupied cells in that table

        for (int i = 0; i < n; ++i) {
            Cell chosen;
            if (t[i] == 0) {
                // nearest completely unoccupied table
                // find the best table representative in free_tables
                // But we need distance to the nearest cell of that table? Actually guest goes to nearest table cell of that table.
                // The problem says: "nearest table cell that belongs to a completely unoccupied table."
                // So we need to find the nearest cell among all cells of completely unoccupied tables.
                // We can just iterate over free_tables and compute distance to each of its 4 cells, but that's slow.
                // Better: maintain a set of all cells belonging to completely unoccupied tables.
                // We'll maintain free_table_cells set.
                set<Cell> free_table_cells;
                // We'll update it when tables become partially occupied.
                // Actually we can just maintain it: initially all cells of all tables are in free_table_cells.
                // When a cell is taken, if its table was completely free, we remove all 4 cells from free_table_cells.
                // When a table becomes completely free again? Never, because guests never leave.
                // So free_table_cells only shrinks.
                // Let's build it.
                // But we already have free_cells. We can just filter free_cells by checking if the table is completely free.
                // However, we need to find the minimum distance cell among those.
                // Since n is up to 50000, we can afford O(log N) per query using a set ordered by distance, then x, then y.
                // We'll maintain two sets: one for all free cells (free_cells), and one for free cells of completely unoccupied tables (free_table_cells).
                // Both ordered by (distance, x, y).
                // When a cell is taken, we remove it from free_cells, and if its table was completely free, we remove all its 4 cells from free_table_cells.
                // To do this efficiently, we need to know for each cell its table representative.
                // We can compute representative: for cell (x,y), table rep is (3*((x-1)/3)+1, 3*((y-1)/3)+1).
                // Let's implement this.
            }
        }

        // Let's restart with a cleaner approach.
    }
    return 0;
}
