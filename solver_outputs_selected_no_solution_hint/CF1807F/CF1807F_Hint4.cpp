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
        bool reached = false;

        // Maximum number of states: 4 * n * m
        // But sum n*m <= 5e4, so we can just simulate with visited states
        // State: (i, j, di, dj)
        set<tuple<int, int, int, int>> visited;

        while (true) {
            if (ci == i2 && cj == j2) {
                reached = true;
                break;
            }

            auto state = make_tuple(ci, cj, di, dj);
            if (visited.count(state)) {
                break; // cycle detected, will never reach
            }
            visited.insert(state);

            // Check if next step hits a wall
            int ni = ci + di;
            int nj = cj + dj;

            bool hit_i = false, hit_j = false;
            if (ni < 1 || ni > n) {
                di = -di;
                hit_i = true;
            }
            if (nj < 1 || nj > m) {
                dj = -dj;
                hit_j = true;
            }

            if (hit_i || hit_j) {
                bounces++;
            }

            ci = ci + di;
            cj = cj + dj;
        }

        if (reached) {
            cout << bounces << '\n';
        } else {
            cout << -1 << '\n';
        }
    }

    return 0;
}
