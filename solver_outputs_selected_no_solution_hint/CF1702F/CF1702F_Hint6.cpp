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
        vector<int> a(n), b(n);
        for (int i = 0; i < n; ++i) cin >> a[i];
        for (int i = 0; i < n; ++i) cin >> b[i];
        
        multiset<int> ma(a.begin(), a.end());
        multiset<int> mb(b.begin(), b.end());
        
        while (!ma.empty()) {
            int mx = *ma.rbegin();
            int bx = *mb.rbegin();
            
            if (mx == bx) {
                ma.erase(prev(ma.end()));
                mb.erase(prev(mb.end()));
            } else if (bx > mx) {
                mb.erase(prev(mb.end()));
                mb.insert(bx / 2);
            } else {
                if (mx % 2 != 0) {
                    break;
                }
                ma.erase(prev(ma.end()));
                ma.insert(mx / 2);
            }
        }
        
        cout << (ma.empty() ? "YES" : "NO") << '\n';
    }
    
    return 0;
}
