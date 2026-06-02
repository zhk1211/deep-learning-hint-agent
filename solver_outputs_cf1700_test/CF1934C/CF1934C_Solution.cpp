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
    // line: x + y = d1 + 2
    
    // find intersection of line with grid boundaries
    // left/bottom boundary: x = min(n, d1 + 1), y = d1 + 2 - x
    int x1 = min(n, d1 + 1);
    int y1 = d1 + 2 - x1;
    // top/right boundary: y = min(m, d1 + 1), x = d1 + 2 - y
    int y2 = min(m, d1 + 1);
    int x2 = d1 + 2 - y2;
    
    int d2 = query(x1, y1);
    int d3 = query(x2, y2);
    
    // candidate 1 from first endpoint
    int cx1 = x1 - d2 / 2;
    int cy1 = y1 + d2 / 2;
    // candidate 2 from second endpoint
    int cx2 = x2 + d3 / 2;
    int cy2 = y2 - d3 / 2;
    
    // query one candidate
    if (cx1 >= 1 && cx1 <= n && cy1 >= 1 && cy1 <= m) {
        int d4 = query(cx1, cy1);
        if (d4 == 0) {
            answer(cx1, cy1);
        } else {
            answer(cx2, cy2);
        }
    } else {
        answer(cx2, cy2);
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
