// Hint5
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

    for (int qi = 0; qi < q; ++qi) {
        long long k = queries[qi];
        long long ans = 0;
        vector<long long> cur = a;

        for (int bit = MAX_BIT; bit >= 0; --bit) {
            long long need = 0;
            long long mask = (1LL << bit) - 1;
            for (int i = 0; i < n; ++i) {
                long long target = ans | (1LL << bit);
                if ((cur[i] & target) != target) {
                    long long add = target - (cur[i] & target);
                    need += add;
                    if (need > k) break;
                }
            }
            if (need <= k) {
                k -= need;
                ans |= (1LL << bit);
                for (int i = 0; i < n; ++i) {
                    long long target = ans;
                    if ((cur[i] & target) != target) {
                        cur[i] += target - (cur[i] & target);
                    }
                }
            }
        }
        cout << ans << '\n';
    }

    return 0;
}
