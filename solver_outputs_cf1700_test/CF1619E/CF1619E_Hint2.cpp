// Hint2
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
        vector<int> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }

        vector<int> cnt(n + 1, 0);
        for (int x : a) {
            if (x <= n) cnt[x]++;
        }

        vector<long long> ans(n + 1, -1);
        long long ops = 0;
        vector<int> extra;
        bool possible = true;

        for (int i = 0; i <= n; ++i) {
            if (!possible) {
                ans[i] = -1;
                continue;
            }
            ans[i] = ops + cnt[i];
            if (cnt[i] == 0) {
                if (extra.empty()) {
                    possible = false;
                } else {
                    int j = extra.back();
                    extra.pop_back();
                    ops += i - j;
                }
            } else {
                for (int k = 1; k < cnt[i]; ++k) {
                    extra.push_back(i);
                }
            }
        }

        for (int i = 0; i <= n; ++i) {
            cout << ans[i] << (i == n ? '\n' : ' ');
        }
    }
    return 0;
}
