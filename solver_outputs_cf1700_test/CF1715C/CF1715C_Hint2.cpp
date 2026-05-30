// Hint2
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<int> a(n + 2);
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
    }

    // total sum of awesomeness over all subsegments
    // initial formula: sum_{l<=r} (1 + number of i in [l, r-1] such that a[i] != a[i+1])
    // = n*(n+1)/2 + sum_{i=1}^{n-1} i * (n - i) * [a[i] != a[i+1]]
    ll total = (ll)n * (n + 1) / 2;
    auto contrib = [&](int i) -> ll {
        if (i < 1 || i >= n) return 0;
        return (ll)i * (n - i);
    };

    for (int i = 1; i < n; ++i) {
        if (a[i] != a[i + 1]) {
            total += contrib(i);
        }
    }

    while (m--) {
        int idx, x;
        cin >> idx >> x;

        // positions that might change: idx-1 and idx
        for (int j : {idx - 1, idx}) {
            if (j >= 1 && j < n) {
                if (a[j] != a[j + 1]) {
                    total -= contrib(j);
                }
            }
        }

        a[idx] = x;

        for (int j : {idx - 1, idx}) {
            if (j >= 1 && j < n) {
                if (a[j] != a[j + 1]) {
                    total += contrib(j);
                }
            }
        }

        cout << total << '\n';
    }

    return 0;
}
