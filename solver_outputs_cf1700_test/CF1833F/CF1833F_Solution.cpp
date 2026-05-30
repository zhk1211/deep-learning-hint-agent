// Solution
#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1000000007;

long long mod_pow(long long a, long long b) {
    long long res = 1;
    while (b) {
        if (b & 1) res = res * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int t;
    cin >> t;
    while (t--) {
        int n, m;
        cin >> n >> m;
        map<int, int> freq;
        for (int i = 0; i < n; ++i) {
            int x;
            cin >> x;
            ++freq[x];
        }
        vector<int> b;
        vector<long long> c;
        for (auto &p : freq) {
            b.push_back(p.first);
            c.push_back(p.second);
        }
        int k = b.size();
        if (m > k) {
            cout << 0 << '\n';
            continue;
        }
        vector<long long> pref(k + 1);
        pref[0] = 1;
        for (int i = 0; i < k; ++i) {
            pref[i + 1] = pref[i] * c[i] % MOD;
        }
        vector<long long> inv_pref(k + 1);
        inv_pref[k] = mod_pow(pref[k], MOD - 2);
        for (int i = k; i >= 1; --i) {
            inv_pref[i - 1] = inv_pref[i] * c[i - 1] % MOD;
        }
        long long ans = 0;
        for (int i = 0; i + m <= k; ++i) {
            if (b[i + m - 1] - b[i] == m - 1) {
                long long prod = pref[i + m] * inv_pref[i] % MOD;
                ans = (ans + prod) % MOD;
            }
        }
        cout << ans << '\n';
    }
    return 0;
}
