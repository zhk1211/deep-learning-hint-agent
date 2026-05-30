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
        vector<int> a(2 * n);
        for (int i = 0; i < 2 * n; ++i) cin >> a[i];
        sort(a.begin(), a.end());
        bool found = false;
        int start_x = -1;
        vector<pair<int, int>> ops;
        for (int i = 0; i < 2 * n - 1; ++i) {
            int x = a.back() + a[i];
            multiset<int> s(a.begin(), a.end());
            vector<pair<int, int>> cur_ops;
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
                cur_ops.emplace_back(mx, need);
                x = mx;
            }
            if (ok) {
                found = true;
                start_x = a.back() + a[i];
                ops = move(cur_ops);
                break;
            }
        }
        if (!found) {
            cout << "NO\n";
        } else {
            cout << "YES\n";
            cout << start_x << '\n';
            for (auto &p : ops) {
                cout << p.first << ' ' << p.second << '\n';
            }
        }
    }
    return 0;
}
