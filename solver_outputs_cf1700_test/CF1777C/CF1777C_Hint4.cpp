// Hint4
#include <bits/stdc++.h>
using namespace std;

const int MAXA = 100000;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n, m;
        cin >> n >> m;
        vector<int> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }
        sort(a.begin(), a.end());
        a.erase(unique(a.begin(), a.end()), a.end());
        n = (int)a.size();

        vector<vector<int>> factors(MAXA + 1);
        for (int i = 1; i <= MAXA; ++i) {
            for (int j = i; j <= MAXA; j += i) {
                factors[j].push_back(i);
            }
        }

        vector<int> cnt(m + 1, 0);
        int covered = 0;
        int ans = INT_MAX;
        int r = 0;

        for (int l = 0; l < n; ++l) {
            while (r < n && covered < m) {
                int val = a[r];
                for (int f : factors[val]) {
                    if (f <= m) {
                        if (cnt[f] == 0) covered++;
                        cnt[f]++;
                    }
                }
                r++;
            }
            if (covered == m) {
                ans = min(ans, a[r - 1] - a[l]);
            }
            int val = a[l];
            for (int f : factors[val]) {
                if (f <= m) {
                    cnt[f]--;
                    if (cnt[f] == 0) covered--;
                }
            }
        }

        if (ans == INT_MAX) cout << -1 << '\n';
        else cout << ans << '\n';
    }
    return 0;
}
