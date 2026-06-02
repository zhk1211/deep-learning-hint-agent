#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> a(2 * n);
    for (int i = 0; i < 2 * n; ++i) {
        cin >> a[i];
    }
    sort(a.begin(), a.end());
    
    for (int i = 0; i < 2 * n - 1; ++i) {
        int x = a.back() + a[i];
        multiset<int> s(a.begin(), a.end());
        vector<pair<int, int>> ops;
        bool ok = true;
        
        while (!s.empty()) {
            auto it = prev(s.end());
            int mx = *it;
            s.erase(it);
            int need = x - mx;
            auto it2 = s.find(need);
            if (it2 == s.end()) {
                ok = false;
                break;
            }
            s.erase(it2);
            ops.emplace_back(mx, need);
            x = mx;
        }
        
        if (ok) {
            cout << "YES\n";
            cout << a.back() + a[i] << "\n";
            for (auto &p : ops) {
                cout << p.first << " " << p.second << "\n";
            }
            return;
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
