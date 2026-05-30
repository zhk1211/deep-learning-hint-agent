// Hint0
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n, m, i1, j1, i2, j2;
        string d;
        cin >> n >> m >> i1 >> j1 >> i2 >> j2 >> d;

        int di = (d[0] == 'D' ? 1 : -1);
        int dj = (d[1] == 'R' ? 1 : -1);

        int ci = i1, cj = j1;
        int bounces = 0;
        bool found = false;

        // We'll simulate until we revisit a state (position + direction)
        // Since n,m <= 25000, total states <= 4 * n * m, but sum n*m <= 5e4, so fine.
        // Use a set of tuples (i, j, di, dj)
        set<tuple<int,int,int,int>> visited;

        while (true) {
            if (ci == i2 && cj == j2) {
                found = true;
                break;
            }
            auto state = make_tuple(ci, cj, di, dj);
            if (visited.count(state)) {
                break; // cycle detected, never reach
            }
            visited.insert(state);

            // Check if next step would hit a wall
            int ni = ci + di;
            int nj = cj + dj;
            bool hit_i = (ni < 1 || ni > n);
            bool hit_j = (nj < 1 || nj > m);

            if (hit_i && hit_j) {
                // corner bounce
                di = -di;
                dj = -dj;
                bounces++;
                // position stays same for this step? Actually the ball first moves then bounces.
                // According to statement: "After each step, the ball maintains its direction unless it hits a wall...
                // Note that the ball first goes in a cell and only after that bounces if it needs to."
                // So we should move first, then if that move would go out, we bounce.
                // But here we are checking before moving. Let's adjust:
                // We'll move, then if out of bounds, we bounce and adjust position back?
                // Actually typical simulation: move, if out, bounce and adjust.
                // Let's do: move to ni,nj, then if out, bounce and set position to the wall cell.
                // But corner case: if both out, we bounce both and set to corner.
                // Let's redo simulation properly.
            }
        }

        // Let's redo simulation properly below.
    }

    return 0;
}
