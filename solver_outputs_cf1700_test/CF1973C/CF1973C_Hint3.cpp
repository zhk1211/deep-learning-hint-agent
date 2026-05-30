// Hint3
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
        vector<int> p(n);
        for (int i = 0; i < n; ++i) {
            cin >> p[i];
        }

        vector<int> q(n);
        vector<int> pos(n + 1);
        for (int i = 0; i < n; ++i) {
            pos[p[i]] = i;
        }

        vector<int> big, small;
        for (int i = 1; i <= n; ++i) {
            if (i > n / 2) {
                big.push_back(i);
            } else {
                small.push_back(i);
            }
        }

        vector<int> order;
        for (int i = 0; i < n; ++i) {
            order.push_back(i);
        }
        sort(order.begin(), order.end(), [&](int i, int j) {
            return p[i] < p[j];
        });

        vector<int> ans(n);
        for (int i = 0; i < n / 2; ++i) {
            int idx = order[i];
            ans[idx] = big[i];
        }
        for (int i = n / 2; i < n; ++i) {
            int idx = order[i];
            ans[idx] = small[i - n / 2];
        }

        for (int i = 0; i < n; ++i) {
            cout << ans[i] << " \n"[i == n - 1];
        }
    }
    return 0;
}
