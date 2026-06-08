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
        vector<int> r(n);
        for (int i = 0; i < n; ++i) {
            cin >> r[i];
            --r[i];
        }
        
        vector<int> indeg(n, 0);
        for (int i = 0; i < n; ++i) {
            indeg[r[i]]++;
        }
        
        vector<int> q;
        for (int i = 0; i < n; ++i) {
            if (indeg[i] == 0) q.push_back(i);
        }
        
        vector<int> dist(n, 0);
        for (int i = 0; i < (int)q.size(); ++i) {
            int u = q[i];
            int v = r[u];
            dist[v] = max(dist[v], dist[u] + 1);
            if (--indeg[v] == 0) {
                q.push_back(v);
            }
        }
        
        int ans = 2;
        for (int i = 0; i < n; ++i) {
            if (indeg[i] > 0) {
                int len = 1;
                int cur = r[i];
                while (cur != i) {
                    len++;
                    cur = r[cur];
                }
                ans = max(ans, len + 1);
                for (int j = 0; j < len; ++j) {
                    indeg[cur] = 0;
                    cur = r[cur];
                }
            }
        }
        
        for (int i = 0; i < n; ++i) {
            if (dist[i] > 0) {
                ans = max(ans, dist[i] + 2);
            }
        }
        
        cout << ans << '\n';
    }
    return 0;
}
