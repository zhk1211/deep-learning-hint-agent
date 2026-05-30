// Hint0
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
        int sum = 0;
        set<int> ones;
        for (int i = 1; i <= n; ++i) {
            cin >> a[i];
            sum += a[i];
            if (a[i] == 1) ones.insert(i);
        }

        while (q--) {
            int op;
            cin >> op;
            if (op == 1) {
                int s;
                cin >> s;
                if (s > sum) {
                    cout << "NO\n";
                    continue;
                }
                if ((sum - s) % 2 == 0) {
                    cout << "YES\n";
                } else {
                    if (ones.empty()) {
                        cout << "NO\n";
                    } else {
                        int first = *ones.begin();
                        int last = *ones.rbegin();
                        int max_possible = sum - 2 * min(first - 1, n - last) - 1;
                        if (s <= max_possible) {
                            cout << "YES\n";
                        } else {
                            cout << "NO\n";
                        }
                    }
                }
            } else {
                int i, v;
                cin >> i >> v;
                if (a[i] == v) continue;
                if (a[i] == 1) {
                    ones.erase(i);
                    sum -= 1;
                } else {
                    sum -= 2;
                }
                a[i] = v;
                if (v == 1) {
                    ones.insert(i);
                    sum += 1;
                } else {
                    sum += 2;
                }
            }
        }
    }
    return 0;
}
