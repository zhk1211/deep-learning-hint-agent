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
        vector<int> a(n + 1);
        set<int> ones;
        int total = 0;
        for (int i = 1; i <= n; ++i) {
            cin >> a[i];
            total += a[i];
            if (a[i] == 1) ones.insert(i);
        }

        while (q--) {
            int op;
            cin >> op;
            if (op == 1) {
                int s;
                cin >> s;
                if (s > total) {
                    cout << "NO\n";
                    continue;
                }
                if (s % 2 == total % 2) {
                    cout << "YES\n";
                } else {
                    if (ones.empty()) {
                        cout << "NO\n";
                    } else {
                        int x = *ones.begin();
                        int y = *ones.rbegin();
                        int sum_pref = 2 * (x - 1) + 1;      // sum a[1..x]
                        int sum_suff = 1 + 2 * (n - y);      // sum a[y..n]
                        int max_opp = max(total - sum_pref, total - sum_suff);
                        if (max_opp >= s) cout << "YES\n";
                        else cout << "NO\n";
                    }
                }
            } else {
                int i, v;
                cin >> i >> v;
                if (a[i] == v) continue;
                if (a[i] == 1) ones.erase(i);
                else ones.insert(i);
                total += v - a[i];
                a[i] = v;
            }
        }
    }
    return 0;
}
