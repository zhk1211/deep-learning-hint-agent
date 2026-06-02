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
        vector<int> p(n);
        for (int i = 0; i < n; ++i) {
            cin >> p[i];
            --p[i];
        }
        
        vector<bool> visited(n, false);
        long long ans = 1;
        
        for (int i = 0; i < n; ++i) {
            if (!visited[i]) {
                vector<int> cycle;
                int cur = i;
                while (!visited[cur]) {
                    visited[cur] = true;
                    cycle.push_back(cur);
                    cur = p[cur];
                }
                
                int len = cycle.size();
                string orig, rotated;
                for (int idx : cycle) orig += s[idx];
                rotated = orig;
                
                int period = 1;
                do {
                    char first = rotated[0];
                    rotated.erase(0, 1);
                    rotated.push_back(first);
                    if (rotated == orig) break;
                    ++period;
                } while (true);
                
                ans = lcm(ans, (long long)period);
            }
        }
        
        cout << ans << '\n';
    }
    
    return 0;
}
