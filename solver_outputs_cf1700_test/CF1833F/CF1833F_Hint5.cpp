// Hint5
#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

int mod_pow(int a, int e) {
    int res = 1;
    while (e) {
        if (e & 1) res = 1LL * res * a % MOD;
        a = 1LL * a * a % MOD;
        e >>= 1;
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n, m;
        cin >> n >> m;
        vector<int> a(n);
        for (int i = 0; i < n; ++i) cin >> a[i];

        sort(a.begin(), a.end());
        vector<pair<int, int>> comp;
        for (int x : a) {
            if (comp.empty() || comp.back().first != x)
                comp.push_back({x, 1});
            else
                comp.back().second++;
        }

        int k = comp.size();
        vector<int> pref(k + 1, 1);
        for (int i = 0; i < k; ++i) {
            pref[i + 1] = 1LL * pref[i] * comp[i].second % MOD;
        }

        auto range_prod = [&](int l, int r) -> int {
            if (l > r) return 1;
            return 1LL * pref[r + 1] * mod_pow(pref[l], MOD - 2) % MOD;
        };

        int ans = 0;
        int j = 0;
        for (int i = 0; i < k; ++i) {
            while (j < k && comp[j].first - comp[i].first < m) ++j;
            if (j - i == m) {
                ans = (ans + range_prod(i, j - 1)) % MOD;
            }
        }
        cout << ans << '\n';
    }
    return 0;
}
