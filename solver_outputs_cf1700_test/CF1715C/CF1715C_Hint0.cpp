// Hint0
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
    a[0] = a[n + 1] = 0;

    ll total_subsegments = (ll)n * (n + 1) / 2;
    ll current_sum = 0;

    auto contribution = [&](int i) -> ll {
        if (a[i] != a[i - 1]) {
            return (ll)i * (n - i + 1);
        }
        return 0;
    };

    for (int i = 1; i <= n; ++i) {
        current_sum += contribution(i);
    }

    while (m--) {
        int idx, x;
        cin >> idx >> x;

        if (a[idx] != x) {
            set<int> affected = {idx};
            if (idx + 1 <= n) affected.insert(idx + 1);

            for (int pos : affected) {
                current_sum -= contribution(pos);
            }

            a[idx] = x;

            for (int pos : affected) {
                current_sum += contribution(pos);
            }
        }

        cout << total_subsegments + current_sum << '\n';
    }

    return 0;
}
