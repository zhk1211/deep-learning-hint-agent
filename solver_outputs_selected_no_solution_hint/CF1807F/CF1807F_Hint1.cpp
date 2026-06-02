#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, m, i1, j1, i2, j2;
    string d;
    cin >> n >> m >> i1 >> j1 >> i2 >> j2 >> d;
    
    int di = (d[0] == 'D' ? 1 : -1);
    int dj = (d[1] == 'R' ? 1 : -1);
    
    int ci = i1, cj = j1;
    int bounces = 0;
    
    // To detect cycles, we store visited states: (i, j, di, dj)
    // Since n,m <= 25000, total states <= 4*n*m, but sum n*m <= 5e4, so fine.
    // We'll use a set of tuple<int,int,int,int> or a 4D boolean array if small.
    // Since sum n*m <= 5e4, we can use a set.
    set<tuple<int,int,int,int>> visited;
    
    while (true) {
        if (ci == i2 && cj == j2) {
            cout << bounces << "\n";
            return;
        }
        
        auto state = make_tuple(ci, cj, di, dj);
        if (visited.count(state)) {
            cout << -1 << "\n";
            return;
        }
        visited.insert(state);
        
        // Compute next position
        int ni = ci + di;
        int nj = cj + dj;
        
        // Check if bounce needed
        bool bounce = false;
        if (ni < 1 || ni > n) {
            di = -di;
            bounce = true;
        }
        if (nj < 1 || nj > m) {
            dj = -dj;
            bounce = true;
        }
        
        if (bounce) {
            bounces++;
            // Recompute next position after bounce
            ni = ci + di;
            nj = cj + dj;
        }
        
        ci = ni;
        cj = nj;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}
