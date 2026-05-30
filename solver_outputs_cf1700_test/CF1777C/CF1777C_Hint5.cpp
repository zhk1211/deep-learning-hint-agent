// Hint5
#include <bits/stdc++.h>
using namespace std;

const int MAXV = 100000;

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

        vector<vector<int>> factors(MAXV + 1);
        for (int i = 1; i <= MAXV; ++i) {
            for (int j = i; j <= MAXV; j += i) {
                factors[j].push_back(i);
            }
        }

        vector<int> cnt(m + 1, 0);
        int covered = 0;
        int ans = INT_MAX;
        int l = 0;
        for (int r = 0; r < n; ++r) {
            for (int f : factors[a[r]]) {
                if (f > m) break;
                if (cnt[f] == 0) covered++;
                cnt[f]++;
            }
            while (covered == m) {
                ans = min(ans, a[r] - a[l]);
                for (int f : factors[a[l]]) {
                    if (f > m) break;
                    cnt[f]--;
                    if (cnt[f] == 0) covered--;
                }
                l++;
            }
        }
        if (ans == INT_MAX) cout << -1 << '\n';
        else cout << ans << '\n';
    }
    return 0;
}
