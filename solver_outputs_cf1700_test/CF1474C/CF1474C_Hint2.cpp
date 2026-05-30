// Hint2
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> a(2 * n);
    for (int i = 0; i < 2 * n; i++) cin >> a[i];
    sort(a.begin(), a.end());
    
    for (int i = 0; i < 2 * n - 1; i++) {
        int x = a[i] + a.back();
        multiset<int> ms(a.begin(), a.end());
        vector<pair<int, int>> ops;
        bool ok = true;
        for (int j = 0; j < n; j++) {
            auto it = ms.end();
            --it;
            int mx = *it;
            ms.erase(it);
            int need = x - mx;
            auto it2 = ms.find(need);
            if (it2 == ms.end()) {
                ok = false;
                break;
            }
            ms.erase(it2);
            ops.emplace_back(mx, need);
            x = mx;
        }
        if (ok) {
            cout << "YES\n";
            cout << a[i] + a.back() << "\n";
            for (auto &p : ops) cout << p.first << " " << p.second << "\n";
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
    while (t--) solve();
    return 0;
}
