// Solution
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 50005;
const int MAX_CELLS = 200000; // 4 * n, n <= 50000

struct Cell {
    int x, y, dist;
    bool operator<(const Cell& other) const {
        if (dist != other.dist) return dist < other.dist;
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
};

int dist(int x, int y) {
    int add = (x % 3 == 2 && y % 3 == 2) ? 2 : 0;
    return x + y + add;
}

bool is_table_cell(int x, int y) {
    int mx = x % 3;
    int my = y % 3;
    return (mx == 1 || mx == 2) && (my == 1 || my == 2);
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

        // Generate first 4*n table cells sorted by distance, then x, then y
        vector<Cell> cells;
        cells.reserve(MAX_CELLS);
        // BFS-like generation up to needed distance
        // We'll generate all table cells with distance <= some limit
        // Since max n=50000, 4*n=200000 cells are enough
        // We can just iterate over possible distances
        int limit = 4 * n;
        int cur_dist = 0;
        while ((int)cells.size() < limit) {
            // Generate all table cells with distance == cur_dist
            // x from 0 to cur_dist
            for (int x = 0; x <= cur_dist; ++x) {
                int y = cur_dist - x;
                // adjust for the +2 case
                // Actually dist(x,y) = x+y+2 if x%3==2 && y%3==2 else x+y
                // So we need to find x,y such that dist(x,y) == cur_dist
                // Case 1: no extra +2
                if (x + y == cur_dist) {
                    if (is_table_cell(x, y)) {
                        cells.push_back({x, y, cur_dist});
                    }
                }
                // Case 2: with extra +2
                if (x + y + 2 == cur_dist) {
                    if (x % 3 == 2 && y % 3 == 2 && is_table_cell(x, y)) {
                        cells.push_back({x, y, cur_dist});
                    }
                }
            }
            ++cur_dist;
        }
        // Sort the generated cells to ensure order
        sort(cells.begin(), cells.begin() + limit);

        // occupancy tracking
        // table index: each table occupies 4 cells, we can map cell to table id
        // table id = (x/3)*something + (y/3), but we can just use map or array
        // Since x,y up to maybe ~6000, we can use a map from pair to int
        // Or we can use a 2D array with offset, but easier: map<pair<int,int>, int> table_id
        // Actually we only need to know if a table is completely unoccupied.
        // We can track occupied count per table.
        // Table coordinate: (X,Y) where X = (x-1)/3, Y = (y-1)/3 for cells with x%3 in {1,2}
        // Let's define table index: for cell (x,y), table_x = (x-1)/3, table_y = (y-1)/3
        // We'll use a map from pair to int for occupied count.
        map<pair<int,int>, int> table_occ; // number of occupied cells in this table
        // Also need to know if a cell is occupied
        map<pair<int,int>, bool> cell_occ;

        // For type 0: nearest table cell that belongs to a completely unoccupied table
        // For type 1: nearest vacant table cell

        // We'll maintain pointers for type 0 and type 1
        int ptr0 = 0, ptr1 = 0;

        for (int i = 0; i < n; ++i) {
            int type = t[i];
            Cell chosen;
            if (type == 0) {
                while (ptr0 < limit) {
                    Cell c = cells[ptr0];
                    pair<int,int> table = {(c.x-1)/3, (c.y-1)/3};
                    if (table_occ[table] == 0) {
                        chosen = c;
                        break;
                    }
                    ++ptr0;
                }
            } else {
                while (ptr1 < limit) {
                    Cell c = cells[ptr1];
                    if (!cell_occ[{c.x, c.y}]) {
                        chosen = c;
                        break;
                    }
                    ++ptr1;
                }
            }
            // update structures
            cell_occ[{chosen.x, chosen.y}] = true;
            pair<int,int> table = {(chosen.x-1)/3, (chosen.y-1)/3};
            table_occ[table]++;

            cout << chosen.x << " " << chosen.y << "\n";
        }
    }
    return 0;
}
