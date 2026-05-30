#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    const int MOD = 998244353;
    long long ans = 0;
    long long p2 = 1; // 2^b mod MOD

    // Process each bit independently (up to 30 bits, 1e9 < 2^30)
    for (int b = 0; b <= 30; ++b) {
        int cnt0 = 1, cnt1 = 0;            // number of left prefixes with parity 0/1
        long long sumL0 = 1, sumL1 = 0;    // sum of left boundaries l for those prefixes
        int pref = 0;                      // prefix parity up to current position
        long long total_bit = 0;           // total contribution of this bit

        for (int i = 0; i < n; ++i) {
            int bit = (a[i] >> b) & 1;
            pref ^= bit;
            int r = i + 1;                 // 1-based right boundary

            // We need left boundaries with opposite parity
            long long term;
            if (pref == 0) {
                // opposite parity is 1
                term = ((r + 1LL) * cnt1 - sumL1) % MOD;
            } else {
                // opposite parity is 0
                term = ((r + 1LL) * cnt0 - sumL0) % MOD;
            }
            if (term < 0) term += MOD;
            total_bit = (total_bit + term) % MOD;

            // Add current prefix as a future left boundary
            if (pref == 0) {
                ++cnt0;
                sumL0 = (sumL0 + r + 1) % MOD;
            } else {
                ++cnt1;
                sumL1 = (sumL1 + r + 1) % MOD;
            }
        }

        ans = (ans + total_bit * p2) % MOD;
        p2 = p2 * 2 % MOD;
    }

    cout << ans << '\n';
    return 0;
}
