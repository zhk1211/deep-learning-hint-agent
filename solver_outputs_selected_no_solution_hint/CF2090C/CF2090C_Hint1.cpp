#include <bits/stdc++.h>
using namespace std;

struct Cell {
    int x, y;
    bool operator<(const Cell& o) const {
        int d1 = x + y, d2 = o.x + o.y;
        if (d1 != d2) return d1 < d2;
        if (x != o.x) return x < o.x;
        return y < o.y;
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
        
        // tables are indexed by (x,y) where table occupies:
        // (3x+1,3y+1), (3x+1,3y+2), (3x+2,3y+1), (3x+2,3y+2)
        // distance from (0,0) to a cell (a,b) is a+b
        // For a table at (x,y), the four cells have distances:
        // (3x+1)+(3y+1)=3x+3y+2, (3x+1)+(3y+2)=3x+3y+3,
        // (3x+2)+(3y+1)=3x+3y+3, (3x+2)+(3y+2)=3x+3y+4
        // The minimum distance to reach that table is 3x+3y+2 (to cell (3x+1,3y+1))
        // and the last step must be from a corridor to a table cell.
        // Actually distance is number of steps: from (0,0) to (3x+1,3y+1) is (3x+1)+(3y+1)=3x+3y+2.
        // So distance to table (x,y) is 3x+3y+2.
        
        // We need to maintain available tables.
        // For t=0: nearest completely unoccupied table.
        // For t=1: nearest table with at least one free cell.
        
        // Since n <= 50000 total, we can simulate.
        // We'll keep a set of tables that are completely free (all 4 cells free).
        // And a set of tables that have at least one free cell.
        // But we need to find the nearest according to distance, then x, then y.
        // Distance to table (x,y) = 3x+3y+2.
        // So ordering by distance is same as ordering by (x+y), then x, then y.
        
        // We'll maintain two priority queues or sets.
        // Since we need to extract min and also possibly update (when a table becomes partially occupied),
        // we can use a set with custom comparator, and lazy deletion.
        
        struct Table {
            int x, y;
            int free_cells; // 4,3,2,1,0
        };
        
        map<pair<int,int>, int> free_cnt; // (x,y) -> number of free cells (0..4)
        auto get_dist = [](int x, int y) { return 3*x + 3*y + 2; };
        
        // comparator for completely free tables (t=0)
        auto cmp0 = [&](const pair<int,int>& a, const pair<int,int>& b) {
            int da = get_dist(a.first, a.second);
            int db = get_dist(b.first, b.second);
            if (da != db) return da < db;
            if (a.first != b.first) return a.first < b.first;
            return a.second < b.second;
        };
        set<pair<int,int>, decltype(cmp0)> free_tables(cmp0);
        
        // comparator for tables with at least one free cell (t=1)
        auto cmp1 = [&](const pair<int,int>& a, const pair<int,int>& b) {
            int da = get_dist(a.first, a.second);
            int db = get_dist(b.first, b.second);
            if (da != db) return da < db;
            if (a.first != b.first) return a.first < b.first;
            return a.second < b.second;
        };
        set<pair<int,int>, decltype(cmp1)> avail_tables(cmp1);
        
        // We also need to know which specific cell the guest takes.
        // For a table (x,y), the cells are:
        // (3x+1,3y+1), (3x+1,3y+2), (3x+2,3y+1), (3x+2,3y+2)
        // The guest will choose the nearest free cell of that table.
        // Among free cells, they choose the one with smallest distance, then smallest x, then smallest y.
        // Distances: (1,1): 2, (1,2): 3, (2,1): 3, (2,2): 4.
        // So order of preference: (1,1), (1,2), (2,1), (2,2) relative to table.
        // We can maintain for each table which cells are occupied.
        // Since n is small, we can just store a bitset or set of occupied cells per table.
        
        map<pair<int,int>, set<pair<int,int>>> occupied; // table -> set of occupied cells (local coords? or global)
        // Actually we can store global coordinates of occupied cells.
        // But we need to know which of the 4 cells are free.
        // We'll store a set of free cells per table, or occupied.
        // Since each table has only 4 cells, we can just use an integer mask.
        map<pair<int,int>, int> mask; // 0..15, bit 0: (1,1), bit1: (1,2), bit2: (2,1), bit3: (2,2)
        
        // To add a new table if needed, we can dynamically generate tables as they are requested.
        // But we need to know which tables exist. Since distance increases, we can generate on the fly.
        // The maximum distance needed: n up to 50000, each table provides up to 4 cells.
        // So at most about n tables. We can pre-generate or generate lazily.
        // We'll generate tables in increasing distance order as needed.
        // We'll maintain a "next table" candidate.
        
        // We'll use a BFS-like generation: tables are at (x,y). Distance = 3x+3y+2.
        // We can generate tables in order of (x+y), then x.
        // We'll keep a queue of tables to add.
        
        // But simpler: since n is only 50000, we can just iterate x and y up to some bound.
        // Max distance: each guest takes one cell. Worst case distance grows.
        // Distance = 3x+3y+2. For n=50000, max distance ~ O(sqrt(n)*3)? Actually number of tables with sum <= S is about (S+1)(S+2)/2.
        // We need at most n tables (since each table has 4 cells, but guests might spread). Actually worst case: all t=0, each guest takes a new completely free table, so they occupy one cell per table, so they use n tables. So we need at most n tables.
        // Sum of x+y = S. Number of tables with sum <= S is (S+1)(S+2)/2 >= n => S ~ sqrt(2n) ~ 316 for n=50000.
        // So max x+y ~ 316. Distance max ~ 3*316+2 ~ 950.
        // So we can pre-generate all tables up to sum 400.
        
        // Let's pre-generate tables in order.
        vector<pair<int,int>> tables;
        for (int s = 0; ; ++s) {
            for (int x = 0; x <= s; ++x) {
                int y = s - x;
                tables.emplace_back(x, y);
                if (tables.size() >= n + 5) break;
            }
            if (tables.size() >= n + 5) break;
        }
        // Now we have enough tables.
        
        // Initialize structures.
        free_tables.clear();
        avail_tables.clear();
        free_cnt.clear();
        mask.clear();
        occupied.clear();
        
        // We'll add tables lazily: we maintain an index into tables vector.
        int table_idx = 0;
        auto add_table = [&](int x, int y) {
            free_cnt[{x,y}] = 4;
            mask[{x,y}] = 0; // all free
            free_tables.insert({x,y});
            avail_tables.insert({x,y});
        };
        
        // We need to ensure that when we query, the nearest table is available.
        // We'll add tables as needed: before each query, we add tables until the nearest available table is within the generated set.
        // But since we pre-generated enough tables, we can just add all tables up to some point? Actually we can add all pre-generated tables at start.
        for (auto [x,y] : tables) {
            add_table(x, y);
        }
        
        // Now process guests.
        for (int i = 0; i < n; ++i) {
            int type = t[i];
            pair<int,int> chosen_table;
            if (type == 0) {
                // completely free table
                chosen_table = *free_tables.begin();
            } else {
                // any table with at least one free cell
                chosen_table = *avail_tables.begin();
            }
            int tx = chosen_table.first, ty = chosen_table.second;
            
            // Now choose the specific cell in this table.
            // Order of preference: (1,1), (1,2), (2,1), (2,2) relative to table.
            // Global coordinates:
            vector<pair<int,int>> cells = {
                {3*tx+1, 3*ty+1},
                {3*tx+1, 3*ty+2},
                {3*tx+2, 3*ty+1},
                {3*tx+2, 3*ty+2}
            };
            // bits: 0 for (1,1), 1 for (1,2), 2 for (2,1), 3 for (2,2)
            int m = mask[{tx,ty}];
            int chosen_bit = -1;
            for (int b = 0; b < 4; ++b) {
                if (!(m & (1 << b))) {
                    chosen_bit = b;
                    break;
                }
            }
            // Update mask
            mask[{tx,ty}] |= (1 << chosen_bit);
            int new_free = 4 - __builtin_popcount(mask[{tx,ty}]);
            free_cnt[{tx,ty}] = new_free;
            
            // Update sets
            if (new_free == 0) {
                avail_tables.erase({tx,ty});
                free_tables.erase({tx,ty});
            } else if (new_free < 4) {
                // no longer completely free
                free_tables.erase({tx,ty});
                // still in avail_tables (it was there, and we didn't remove it)
                // but we need to reinsert? No, it's still there. However, if we use a set, the key is the same, so it's fine.
                // But if we had removed and reinserted, it would be same. So no action needed.
            }
            // If new_free == 4, it remains in both (but we just occupied one, so it won't be 4).
            
            // Output chosen cell
            cout << cells[chosen_bit].first << " " << cells[chosen_bit].second << "\n";
        }
    }
    return 0;
}
