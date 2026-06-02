// Solution
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n, q;
        cin >> n >> q;
        vector<int> p(n + 1);
        for (int i = 1; i <= n; ++i) {
            cin >> p[i];
        }
        string s;
        cin >> s;
        s = " " + s; // 1-indexed
        
        // pos[x] = position of value x in p
        vector<int> pos(n + 1);
        for (int i = 1; i <= n; ++i) {
            pos[p[i]] = i;
        }
        
        // difference array for intervals
        vector<int> diff(n + 2, 0);
        for (int x = 1; x <= n; ++x) {
            int l = min(x, pos[x]);
            int r = max(x, pos[x]);
            if (l < r) {
                diff[l]++;
                diff[r]--;
            }
        }
        
        // d[i] = number of intervals covering position i
        vector<int> d(n + 1, 0);
        int cur = 0;
        for (int i = 1; i <= n; ++i) {
            cur += diff[i];
            d[i] = cur;
        }
        
        // set of bad indices i (1 <= i <= n-1) where s[i]=='L' && s[i+1]=='R' and d[i] > 0
        set<int> bad;
        auto is_bad = [&](int i) -> bool {
            if (i < 1 || i >= n) return false;
            return (s[i] == 'L' && s[i+1] == 'R' && d[i] > 0);
        };
        
        for (int i = 1; i < n; ++i) {
            if (is_bad(i)) bad.insert(i);
        }
        
        // process queries
        while (q--) {
            int idx;
            cin >> idx;
            // flip s[idx]
            s[idx] = (s[idx] == 'L' ? 'R' : 'L');
            
            // indices that could be affected: idx-1 and idx
            for (int cand : {idx-1, idx}) {
                if (cand >= 1 && cand < n) {
                    if (is_bad(cand)) bad.insert(cand);
                    else bad.erase(cand);
                }
            }
            
            cout << (bad.empty() ? "YES" : "NO") << "\n";
        }
    }
    return 0;
}
