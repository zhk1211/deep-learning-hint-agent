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
        if (!vis[i]) {
            vector<int> cycle;
            int start = query(i);
            int cur = start;
            do {
                cur = query(i);
                cycle.push_back(cur);
            } while (cur != start);
            
            for (size_t j = 0; j < cycle.size(); ++j) {
                int from = cycle[j];
                int to = cycle[(j + 1) % cycle.size()];
                p[from] = to;
                vis[from] = true;
            }
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
