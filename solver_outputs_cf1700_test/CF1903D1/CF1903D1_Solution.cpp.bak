// Solution
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    while (q--) {
        long long k;
        cin >> k;
        vector<long long> cur = a;
        long long ans = 0;
        for (int b = 60; b >= 0; --b) {
            long long bit = 1LL << b;
            long long need = 0;
            for (int i = 0; i < n; ++i) {
                if (!(cur[i] & bit)) {
                    long long r = cur[i] & (bit - 1);
                    need += bit - r;
                    if (need > k) break;
                }
            }
            if (need <= k) {
                k -= need;
                ans |= bit;
                for (int i = 0; i < n; ++i) {
                    if (!(cur[i] & bit)) {
                        long long r = cur[i] & (bit - 1);
                        cur[i] += bit - r;
                    }
                }
            }
        }
        cout << ans << '\n';
    }

    return 0;
}
