// Hint4
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

    const int MAX_BIT = 60;
    vector<long long> queries(q);
    for (int i = 0; i < q; ++i) {
        cin >> queries[i];
    }

    for (long long k : queries) {
        long long ans = 0;
        vector<long long> cur = a;
        for (int bit = MAX_BIT; bit >= 0; --bit) {
            long long need = 0;
            long long mask = ans | (1LL << bit);
            for (long long x : cur) {
                if ((x & mask) != mask) {
                    need += mask - (x & mask);
                    if (need > k) break;
                }
            }
            if (need <= k) {
                ans = mask;
                k -= need;
                for (long long &x : cur) {
                    if ((x & mask) != mask) {
                        x += mask - (x & mask);
                    }
                }
            }
        }
        cout << ans << '\n';
    }

    return 0;
}
