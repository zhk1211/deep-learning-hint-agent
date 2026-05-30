// Solution
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        string s;
        cin >> s;
        
        int V = 2 * (n + 1);
        vector<int> parent(V), sz(V, 1);
        iota(parent.begin(), parent.end(), 0);
        
        function<int(int)> find = [&](int x) {
            return parent[x] == x ? x : parent[x] = find(parent[x]);
        };
        auto unite = [&](int u, int v) {
            u = find(u); v = find(v);
            if (u != v) {
                if (sz[u] < sz[v]) swap(u, v);
                parent[v] = u;
                sz[u] += sz[v];
            }
        };
        
        for (int i = 1; i <= n; ++i) {
            if (s[i - 1] == 'R') {
                int u = 2 * (i - 1);       // (i-1, 0)
                int v = 2 * i + 1;         // (i, 1)
                unite(u, v);
            } else {
                int u = 2 * i
