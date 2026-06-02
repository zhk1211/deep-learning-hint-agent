#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, k;
    cin >> n >> k;
    vector<int> friends(k);
    for (int i = 0; i < k; ++i) {
        cin >> friends[i];
    }
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    vector<int> dist_vlad(n + 1, -1);
    queue<int> q;
    dist_vlad[1] = 0;
    q.push(1);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (dist_vlad[v] == -1) {
                dist_vlad[v] = dist_vlad[u] + 1;
                q.push(v);
            }
        }
    }
    
    vector<int> dist_friend(n + 1, -1);
    for (int x : friends) {
        dist_friend[x] = 0;
        q.push(x);
    }
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (dist_friend[v] == -1) {
                dist_friend[v] = dist_friend[u] + 1;
                q.push(v);
            }
        }
    }
    
    bool can_win = false;
    for (int u = 2; u <= n; ++u) {
        if (adj[u].size() == 1) { // leaf
            if (dist_vlad[u] < dist_friend[u]) {
                can_win = true;
                break;
            }
        }
    }
    
    cout << (can_win ? "YES" : "NO") << "\n";
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
