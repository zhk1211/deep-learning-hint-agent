// Hint3
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n, m;
        cin >> n >> m;
        vector<int> a(n), b(m);
        for (int i = 0; i < n; ++i) cin >> a[i];
        for (int i = 0; i < m; ++i) cin >> b[i];

        // Compute LIS of a
        vector<int> dp;
        for (int x : a) {
            auto it = lower_bound(dp.begin(), dp.end(), x);
            if (it == dp.end()) dp.push_back(x);
            else *it = x;
        }
        int lis_a = (int)dp.size();

        // We want to insert b elements without increasing LIS beyond lis_a.
        // Strategy: sort b descending, then for each element of a, output all b elements >= a[i] (descending),
        // then output a[i]. After a, output remaining b.
        sort(b.begin(), b.end(), greater<int>());
        int ptr = 0;
        vector<int> res;
        res.reserve(n + m);
        for (int i = 0; i < n; ++i) {
            while (ptr < m && b[ptr] >= a[i]) {
                res.push_back(b[ptr++]);
            }
            res.push_back(a[i]);
        }
        while (ptr < m) {
            res.push_back(b[ptr++]);
        }

        for (int i = 0; i < n + m; ++i) {
            cout << res[i] << " \n"[i == n + m - 1];
        }
    }
    return 0;
}
