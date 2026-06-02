#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<long long> a(n + 2);
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
    }

    long long ans = 0;
    auto add_joint = [&](int i) {
        if (i >= 1 && i < n && a[i] != a[i + 1]) {
            ans += 1LL * i * (n - i);
        }
    };
    auto remove_joint = [&](int i) {
        if (i >= 1 && i < n && a[i] != a[i + 1]) {
            ans -= 1LL * i * (n - i);
        }
    };

    for (int i = 1; i < n; ++i) {
        if (a[i] != a[i + 1]) {
            ans += 1LL * i * (n - i);
        }
    }

    long long total_subsegments = 1LL * n * (n + 1) / 2;
    while (m--) {
        int i, x;
        cin >> i >> x;

        if (i > 1) remove_joint(i - 1);
        remove_joint(i);

        a[i] = x;

        if (i > 1) add_joint(i - 1);
        add_joint(i);

        cout << ans + total_subsegments << '\n';
    }

    return 0;
}
