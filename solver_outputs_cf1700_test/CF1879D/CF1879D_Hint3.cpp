// Hint3
#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    long long ans = 0;

    for (int bit = 0; bit < 30; ++bit) {
        int cnt0 = 1, cnt1 = 0;
        long long sum0 = 0, sum1 = 0;
        int pref = 0;

        for (int r = 0; r < n; ++r) {
            int val = (a[r] >> bit) & 1;
            pref ^= val;

            if (pref == 0) {
                long long add = (1LL * cnt1 * (r + 1) - sum1) % MOD;
                ans = (ans + (add << bit)) % MOD;
                cnt0++;
                sum0 += r + 1;
            } else {
                long long add = (1LL * cnt0 * (r + 1) - sum0) % MOD;
                ans = (ans + (add << bit)) % MOD;
                cnt1++;
                sum1 += r + 1;
            }
        }
    }

    cout << (ans % MOD + MOD) % MOD << '\n';
    return 0;
}
