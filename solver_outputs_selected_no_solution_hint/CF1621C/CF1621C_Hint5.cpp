#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> p(n + 1, 0);
    vector<bool> vis(n + 1, false);
    
    for (int i = 1; i <= n; ++i) {
        if (vis[i]) continue;
        vector<int> cycle;
        int cur = i;
        while (true) {
            cout << "? " << i << endl;
            int val;
            cin >> val;
            if (val == 0) exit(0);
            if (!cycle.empty() && val == cycle[0]) break;
            cycle.push_back(val);
        }
        int sz = cycle.size();
        for (int j = 0; j < sz; ++j) {
            int from = cycle[j];
            int to = cycle[(j + 1) % sz];
            p[from] = to;
            vis[from] = true;
        }
    }
    
    cout << "!";
    for (int i = 1; i <= n; ++i) {
        cout << " " << p[i];
    }
    cout << endl;
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
