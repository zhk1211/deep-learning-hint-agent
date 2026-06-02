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
        int total_sum = 0;
        set<int> ones;
        for (int i = 1; i <= n; ++i) {
            cin >> a[i];
            total_sum += a[i];
            if (a[i] == 1) ones.insert(i);
        }

        while (q--) {
            int op;
            cin >> op;
            if (op == 1) {
                int s;
                cin >> s;
                if (s > total_sum) {
                    cout << "NO\n";
                    continue;
                }
                if ((total_sum - s) % 2 == 0) {
                    cout << "YES\n";
                } else {
                    if (ones.empty()) {
                        cout << "NO\n";
                    } else {
                        int first_one = *ones.begin();
                        int last_one = *ones.rbegin();
                        int max_sum = max(total_sum - 2 * (first_one - 1) - 1, 
                                          total_sum - 2 * (n - last_one) - 1);
                        if (s <= max_sum) {
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
                    total_sum -= 1;
                } else {
                    total_sum -= 2;
                }
                a[i] = v;
                if (v == 1) {
                    ones.insert(i);
                    total_sum += 1;
                } else {
                    total_sum += 2;
                }
            }
        }
    }
    return 0;
}
