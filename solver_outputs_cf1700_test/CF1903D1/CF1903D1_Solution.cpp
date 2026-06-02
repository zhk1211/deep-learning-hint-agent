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
        for (int bit = 60; bit >= 0; --bit) {
            long long need = 0;
            long long mask = (1LL << bit);
            vector<long long> tmp = cur;
            for (int i = 0; i < n; ++i) {
                if (!(cur[i] & mask)) {
                    long long cost = mask - (cur[i] % mask);
                    need += cost;
                    tmp[i] += cost;
                    if (need > k) break;
                }
            }
            if (need <= k) {
                k -= need;
                ans |= mask;
                cur = move(tmp);
            }
        }
        cout << ans << '\n';
    }

    return 0;
}
