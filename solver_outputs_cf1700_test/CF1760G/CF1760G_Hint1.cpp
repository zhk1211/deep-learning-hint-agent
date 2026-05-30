// Hint1
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, a, b;
    cin >> n >> a >> b;
    vector<vector<pair<int,int>>> adj(n+1);
    for (int i = 0; i < n-1; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }
    
    // BFS/DFS from b to collect XOR values from b to all other nodes (excluding a)
    // We'll use DFS
    vector<int> xor_from_b(n+1, -1);
    queue<int> q;
    xor_from_b[b] = 0;
    q.push(b);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (auto &[v, w] : adj[u]) {
            if (xor_from_b[v] == -1 && v != a) {
                xor_from_b[v] = xor_from_b[u] ^ w;
                q.push(v);
            }
        }
    }
    
    // Now DFS from a to collect XOR values from a to all nodes (excluding b)
    vector<int> xor_from_a(n+1, -1);
    queue<int> q2;
    xor_from_a[a] = 0;
    q2.push(a);
    while (!q2.empty()) {
        int u = q2.front(); q2.pop();
        if (u == b) continue; // don't go past b
        for (auto &[v, w] : adj[u]) {
            if (xor_from_a[v] == -1 && v != b) {
                xor_from_a[v] = xor_from_a[u] ^ w;
                q2.push(v);
            }
        }
    }
    
    // Check if there's a node reachable from a (excluding b) with XOR equal to some node reachable from b (excluding a)
    // Also check if a can directly reach b with XOR 0
    // Actually, we need to see if there exists a node u (u != b) reachable from a, and a node v (v != a) reachable from b,
    // such that xor_from_a[u] == xor_from_b[v]. If yes, then we can go a->u, teleport to v, then v->b.
    // Also, if xor_from_a[b] == 0 (i.e., we can reach b directly without teleport), then YES.
    
    // Check direct path
    if (xor_from_a[b] == 0) {
        cout << "YES\n";
        return;
    }
    
    // Collect all XORs from a side (excluding b)
    unordered_set<int> a_xors;
    for (int i = 1; i <= n; i++) {
        if (xor_from_a[i] != -1 && i != b) {
            a_xors.insert(xor_from_a[i]);
        }
    }
    
    // Check if any XOR from b side (excluding a) matches
    for (int i = 1; i <= n; i++) {
        if (xor_from_b[i] != -1 && i != a) {
            if (a_xors.count(xor_from_b[i])) {
                cout << "YES\n";
                return;
            }
        }
    }
    
    cout << "NO\n";
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
