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
        
        // visited states: (i, j, di, dj)
        // Since n,m <= 25000, we can't store all states directly.
        // But we can detect cycles by tracking when we hit a corner with same direction,
        // or use a set with hashing. Since sum n*m <= 5e4, we can use a 4D boolean array
        // but dimensions are up to 25000 each. Instead, we can use a set of tuples.
        // However, the number of distinct states is bounded by 4 * (n+m) because
        // the ball only changes direction at boundaries.
        // We can just simulate until we either find target or detect a cycle.
        // To detect cycle, we can store states as (i, j, di, dj) in a set.
        // Since sum n*m <= 5e4, total states across all test cases is manageable.
        
        set<tuple<int,int,int,int>> visited;
        
        while (true) {
            if (ci == i2 && cj == j2) {
                found = true;
                break;
            }
            
            auto state = make_tuple(ci, cj, di, dj);
            if (visited.count(state)) {
                break; // cycle detected, never reaches target
            }
            visited.insert(state);
            
            // compute next position
            int ni = ci + di;
            int nj = cj + dj;
            
            bool bounced = false;
            if (ni < 1 || ni > n) {
                di = -di;
                bounced = true;
            }
            if (nj < 1 || nj > m) {
                dj = -dj;
                bounced = true;
            }
            
            if (bounced) {
                bounces++;
                // recalculate next position after bounce
                ni = ci + di;
                nj = cj + dj;
            }
            
            ci = ni;
            cj = nj;
        }
        
        if (found) {
            cout << bounces << '\n';
        } else {
            cout << -1 << '\n';
        }
    }
    
    return 0;
}
