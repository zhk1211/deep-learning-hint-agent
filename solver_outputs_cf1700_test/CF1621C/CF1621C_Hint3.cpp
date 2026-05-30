// Hint3
#include <bits/stdc++.h>
using namespace std;

int query(int i) {
    cout << "? " << i << endl;
    int res;
    cin >> res;
    if (res == 0) exit(0);
    return res;
}

void solve() {
    int n;
    cin >> n;
    vector<int> p(n + 1, 0);
    vector<bool> vis(n + 1, false);
    
    for (int i = 1; i <= n; ++i) {
        if (vis[i]) continue;
        vector<int> cycle;
        int cur = query(i);
        cycle.push_back(cur);
        while (true) {
            int nxt = query(i);
            cycle.push_back(nxt);
            if (nxt == cycle[0]) break;
        }
        int sz = cycle.size() - 1;
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
