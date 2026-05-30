// Hint1
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> a(2 * n);
    for (int i = 0; i < 2 * n; i++) cin >> a[i];
    sort(a.begin(), a.end());

    // Try each possible partner for the largest element
    for (int p = 0; p < 2 * n - 1; p++) {
        int x = a.back() + a[p];
        multiset<int> ms(a.begin(), a.end());
        vector<pair<int, int>> ops;
        bool ok = true;

        for (int step = 0; step < n; step++) {
            auto it = prev(ms.end());
            int mx = *it;
            ms.erase(it);
            int need = x - mx;
            auto it2 = ms.find(need);
            if (it2 == ms.end()) {
                ok = false;
                break;
            }
            ms.erase(it2);
            ops.push_back({mx, need});
            x = mx;
        }

        if (ok) {
            cout << "YES\n";
            cout << a.back() + a[p] << "\n";
            for (auto &pr : ops) {
                cout << pr.first << " " << pr.second << "\n";
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
