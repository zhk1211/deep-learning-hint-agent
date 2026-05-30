#include <iostream>
#include <algorithm>
using namespace std;

int f(int x, int a) {
    if (a > x) return x + 1;
    if (a == x) return x;
    return x - 1;
}

void solve() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        const int INF = 1e9;
        int dp0 = 0, dp1 = -INF, dp2 = -INF;
        for (int i = 0; i < n; ++i) {
            int a;
            cin >> a;
            int ndp0 = f(dp0, a);
            int ndp1 = max(dp1, dp0);
            int ndp2 = max(f(dp1, a), f(dp2, a));
            dp0 = ndp0;
            dp1 = ndp1;
            dp2 = ndp2;
        }
        cout << max(dp1, dp2) << '\n';
    }
}

int main() {
    solve();
    return 0;
}
