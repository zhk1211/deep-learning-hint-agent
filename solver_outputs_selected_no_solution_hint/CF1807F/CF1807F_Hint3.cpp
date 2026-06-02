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
        int steps = 0;
        const int MAX_STEPS = 4 * n * m + 5;
        
        while (steps < MAX_STEPS) {
            if (ci == i2 && cj == j2) {
                found = true;
                break;
            }
            
            int ni = ci + di;
            int nj = cj + dj;
            
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
            }
            
            ci += di;
            cj += dj;
            steps++;
        }
        
        if (found) {
            cout << bounces << '\n';
        } else {
            cout << -1 << '\n';
        }
    }
    
    return 0;
}
