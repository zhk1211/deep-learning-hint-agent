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
        
        // We'll simulate until we either reach the target or detect a loop.
        // The state is (i, j, di, dj). There are at most 4*n*m states.
        // Since sum of n*m <= 5e4, we can use a visited array per test case.
        // We'll use a 4D boolean array or a set of tuples.
        // To be efficient, we can use a 2D array of bitsets or a vector of vector of array<bool,4>.
        // We'll map direction to 0..3: (di,dj) pairs: (1,1)->0, (1,-1)->1, (-1,1)->2, (-1,-1)->3.
        
        auto dir_to_idx = [&](int di, int dj) -> int {
            if (di == 1 && dj == 1) return 0;
            if (di == 1 && dj == -1) return 1;
            if (di == -1 && dj == 1) return 2;
            return 3;
        };
        
        // visited[i][j][dir] = true if we've been in this state
        vector<vector<array<bool, 4>>> visited(n+1, vector<array<bool, 4>>(m+1));
        
        while (true) {
            if (ci == i2 && cj == j2) {
                reached = true;
                break;
            }
            
            int dir_idx = dir_to_idx(di, dj);
            if (visited[ci][cj][dir_idx]) {
                // loop detected, will never reach
                break;
            }
            visited[ci][cj][dir_idx] = true;
            
            // Check if next step hits a wall
            int ni = ci + di;
            int nj = cj + dj;
            bool hit_i = false, hit_j = false;
            if (ni < 1 || ni > n) {
                hit_i = true;
                di = -di;
            }
            if (nj < 1 || nj > m) {
                hit_j = true;
                dj = -dj;
            }
            
            if (hit_i || hit_j) {
                bounces++;
                // After bounce, we stay in the same cell and direction is updated.
                // The problem says: "the ball first goes in a cell and only after that bounces if it needs to."
                // So we don't move this step, just change direction.
                // But careful: if we hit a corner, both flip, which we already did.
                // Next iteration will move with new direction.
            } else {
                // No bounce, just move
                ci = ni;
                cj = nj;
            }
        }
        
        if (reached) {
            cout << bounces << '\n';
        } else {
            cout << -1 << '\n';
        }
    }
    
    return 0;
}
