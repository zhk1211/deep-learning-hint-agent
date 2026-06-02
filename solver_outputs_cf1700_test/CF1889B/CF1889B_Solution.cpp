#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n;
        long long c;
        cin >> n >> c;
        vector<long long> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }
        
        // We need to connect all nodes to node 1 (0-indexed: node 0)
        // Condition to connect node i (i>0) to component of 1:
        // sum of a in component of 1 + a[i] >= (i+1) * 1 * c
        // i.e., sum1 + a[i] >= (i+1)*c
        // Initially sum1 = a[0]
        // We can connect nodes in order of decreasing (a[i] - (i+1)*c)
        // because larger value means easier to satisfy.
        
        vector<pair<long long, int>> nodes;
        for (int i = 1; i < n; ++i) {
            nodes.emplace_back(a[i] - (long long)(i + 1) * c, i);
        }
        sort(nodes.begin(), nodes.end(), greater<pair<long long, int>>());
        
        long long sum1 = a[0];
        bool ok = true;
        for (auto &p : nodes) {
            int i = p.second;
            if (sum1 + a[i] >= (long long)(i + 1) * c) {
                sum1 += a[i];
            } else {
                ok = false;
                break;
            }
        }
        cout << (ok ? "YES" : "NO") << '\n';
    }
    return 0;
}
