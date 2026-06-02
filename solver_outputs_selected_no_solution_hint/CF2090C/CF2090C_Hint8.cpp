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
        set<Cell> free_tables;
        
        auto add_table = [&](int x, int y) {
            free_cells.insert({x, y});
            free_cells.insert({x, y+1});
            free_cells.insert({x+1, y});
            free_cells.insert({x+1, y+1});
            free_tables.insert({x, y});
        };
        
        add_table(0, 0);
        
        for (int i = 0; i < n; ++i) {
            Cell chosen;
            if (t[i] == 1) {
                chosen = *free_cells.begin();
            } else {
                // find a cell from a completely unoccupied table
                for (auto it = free_cells.begin(); it != free_cells.end(); ++it) {
                    int x = it->x, y = it->y;
                    int tx = (x - 1) / 3, ty = (y - 1) / 3;
                    if (x % 3 == 1 && y % 3 == 1) {
                        tx = (x - 1) / 3;
                        ty = (y - 1) / 3;
                    } else if (x % 3 == 1 && y % 3 == 2) {
                        tx = (x - 1) / 3;
                        ty = (y - 2) / 3;
                    } else if (x % 3 == 2 && y % 3 == 1) {
                        tx = (x - 2) / 3;
                        ty = (y - 1) / 3;
                    } else if (x % 3 == 2 && y % 3 == 2) {
                        tx = (x - 2) / 3;
                        ty = (y - 2) / 3;
                    }
                    if (free_tables.count({tx, ty})) {
                        chosen = *it;
                        break;
                    }
                }
            }
            
            cout << chosen.x << " " << chosen.y << "\n";
            
            // remove chosen cell
            free_cells.erase(chosen);
            
            // if the table becomes partially occupied, remove it from free_tables
            int x = chosen.x, y = chosen.y;
            int tx, ty;
            if (x % 3 == 1 && y % 3 == 1) {
                tx = (x - 1) / 3;
                ty = (y - 1) / 3;
            } else if (x % 3 == 1 && y % 3 == 2) {
                tx = (x - 1) / 3;
                ty = (y - 2) / 3;
            } else if (x % 3 == 2 && y % 3 == 1) {
                tx = (x - 2) / 3;
                ty = (y - 1) / 3;
            } else {
                tx = (x - 2) / 3;
                ty = (y - 2) / 3;
            }
            free_tables.erase({tx, ty});
            
            // add neighboring tables if they become reachable
            // a table is reachable if at least one of its cells is adjacent to an occupied cell or (0,0)
            // but simpler: we can just add tables that are adjacent to the chosen cell
            // Actually, we need to maintain the set of all free cells that are reachable.
            // Since the hall is infinite, we can't precompute all. We need to generate on the fly.
            // The distance from (0,0) to a table cell (x,y) is x+y.
            // When a cell is occupied, its neighbors might become reachable if they weren't before.
            // But we can just add all tables that are at Manhattan distance <= current max distance + something.
            // However, n <= 50000, so we can afford to add tables lazily.
            // We'll add tables that are adjacent (by side) to the chosen cell.
            vector<pair<int,int>> dirs = {{0,1},{0,-1},{1,0},{-1,0}};
            for (auto [dx, dy] : dirs) {
                int nx = chosen.x + dx, ny = chosen.y + dy;
                if (nx < 0 || ny < 0) continue;
                // check if (nx, ny) belongs to a table
                if (nx % 3 == 0 || ny % 3 == 0) continue; // corridor
                // determine table coordinates
                int ttx, tty;
                if (nx % 3 == 1 && ny % 3 == 1) {
                    ttx = (nx - 1) / 3;
                    tty = (ny - 1) / 3;
                } else if (nx % 3 == 1 && ny % 3 == 2) {
                    ttx = (nx - 1) / 3;
                    tty = (ny - 2) / 3;
                } else if (nx % 3 == 2 && ny % 3 == 1) {
                    ttx = (nx - 2) / 3;
                    tty = (ny - 1) / 3;
                } else {
                    ttx = (nx - 2) / 3;
                    tty = (ny - 2) / 3;
                }
                // if this table is not yet added, add it
                if (free_cells.find({ttx*3+1, tty*3+1}) == free_cells.end() &&
                    free_cells.find({ttx*3+1, tty*3+2}) == free_cells.end() &&
                    free_cells.find({ttx*3+2, tty*3+1}) == free_cells.end() &&
                    free_cells.find({ttx*3+2, tty*3+2}) == free_cells.end()) {
                    // not added yet, but we need to check if it's reachable
                    // it's reachable because we just stepped into a neighbor
                    add_table(ttx, tty);
                }
            }
        }
    }
    return 0;
}
