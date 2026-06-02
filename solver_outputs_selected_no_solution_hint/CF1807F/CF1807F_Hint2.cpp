#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, m, i1, j1, i2, j2;
    string d;
    cin >> n >> m >> i1 >> j1 >> i2 >> j2 >> d;
    
    int di = 0, dj = 0;
    if (d[0] == 'D') di = 1;
    else di = -1;
    if (d[1] == 'R') dj = 1;
    else dj = -1;
    
    int ci = i1, cj = j1;
    int bounces = 0;
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
        
        bool bounced = false;
        int ni = ci + di;
        int nj = cj + dj;
        
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
        }
        
        ci += di;
        cj += dj;
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
