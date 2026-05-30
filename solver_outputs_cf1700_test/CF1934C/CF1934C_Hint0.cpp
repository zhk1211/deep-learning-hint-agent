// Hint0
#include <bits/stdc++.h>
using namespace std;

int query(int x, int y) {
    cout << "? " << x << " " << y << endl;
    int d;
    cin >> d;
    return d;
}

void answer(int x, int y) {
    cout << "! " << x << " " << y << endl;
}

void solve() {
    int n, m;
    cin >> n >> m;
    
    int d1 = query(1, 1);
    int d2 = query(1, m);
    int d3 = query(n, 1);
    
    // Intersection of lines from (1,1) and (1,m)
    // x + y = d1 + 2
    // x - y = d2 - m + 1
    int x1 = (d1 + d2 - m + 3) / 2;
    int y1 = d1 + 2 - x1;
    
    // Intersection of lines from (1,1) and (n,1)
    // x + y = d1 + 2
    // -x + y = d3 - n + 1
    int y2 = (d1 + d3 - n + 3) / 2;
    int x2 = d1 + 2 - y2;
    
    if (x1 >= 1 && x1 <= n && y1 >= 1 && y1 <= m) {
        int d4 = query(x1, y1);
        if (d4 == 0) {
            answer(x1, y1);
        } else {
            answer(x2, y2);
        }
    } else {
        answer(x2, y2);
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
