#include <bits/stdc++.h>
using namespace std;

int query(int i) {
    cout << "? " << i << endl;
    int x;
    cin >> x;
    if (x == 0) exit(0);
    return x;
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
        vis[cur] = true;
        while (true) {
            int nxt = query(i);
            if (nxt == cycle[0]) break;
            cycle.push_back(nxt);
            vis[nxt] = true;
        }
        int sz = cycle.size();
        for (int j = 0; j < sz; ++j) {
            p[cycle[j]] = cycle[(j + 1) % sz];
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
