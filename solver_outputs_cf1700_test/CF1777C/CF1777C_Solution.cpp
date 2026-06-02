#include <bits/stdc++.h>
using namespace std;

const int MAXA = 100000;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<vector<int>> divisors(MAXA + 1);
    for (int i = 1; i <= MAXA; ++i) {
        for (int j = i; j <= MAXA; j += i) {
            divisors[j].push_back(i);
        }
    }

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

        vector<int> freq(m + 1, 0);
        int covered = 0;
        int ans = INT_MAX;

        int r = 0;
        for (int l = 0; l < n; ++l) {
            while (r < n && covered < m) {
                int val = a[r];
                for (int d : divisors[val]) {
                    if (d > m) break;
                    if (freq[d] == 0) {
                        ++covered;
                    }
                    ++freq[d];
                }
                ++r;
            }
            if (covered == m) {
                ans = min(ans, a[r - 1] - a[l]);
            }
            int val = a[l];
            for (int d : divisors[val]) {
                if (d > m) break;
                --freq[d];
                if (freq[d] == 0) {
                    --covered;
                }
            }
        }

        if (ans == INT_MAX) {
            cout << -1 << '\n';
        } else {
            cout << ans << '\n';
        }
    }
    return 0;
}
