// Hint0
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
        vector<int> p(n + 1);
        vector<bool> used(n + 1, false);
        
        p[1] = 1;
        used[1] = true;
        
        for (int i = 2; i <= n; i++) {
            if (!used[i]) {
                vector<int> cycle;
                int cur = i;
                while (cur <= n) {
                    cycle.push_back(cur);
                    used[cur] = true;
                    if (cur * 2 > n) break;
                    cur *= 2;
                }
                int sz = cycle.size();
                for (int j = 0; j < sz; j++) {
                    p[cycle[j]] = cycle[(j + 1) % sz];
                }
            }
        }
        
        for (int i = 1; i <= n; i++) {
            cout << p[i] << " \n"[i == n];
        }
    }
    return 0;
}
