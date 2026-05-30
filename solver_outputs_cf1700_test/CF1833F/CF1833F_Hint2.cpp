// Hint2
#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

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
        vector<pair<int, int>> freq;
        for (int x : a) {
            if (freq.empty() || freq.back().first != x) {
                freq.push_back({x, 1});
            } else {
                freq.back().second++;
            }
        }
        int k = freq.size();
        vector<long long> pref(k + 1, 1);
        for (int i = 0; i < k; ++i) {
            pref[i + 1] = (pref[i] * freq[i].second) % MOD;
        }
        long long ans = 0;
        for (int i = 0; i + m <= k; ++i) {
            if (freq[i + m - 1].first - freq[i].first < m) {
                long long ways = pref[i + m] * modinv(pref[i], MOD) % MOD;
                ans = (ans + ways) % MOD;
            }
        }
        cout << ans << '\n';
    }
    return 0;
}
