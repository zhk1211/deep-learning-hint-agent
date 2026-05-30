// Hint4
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
    ll ans = total_subsegments; // base: each subsegment has at least 1 block

    // contribution of joints: for each i from 1 to n-1, if a[i] != a[i+1], 
    // then for any subsegment that starts <= i and ends >= i+1, we add 1 extra block.
    // Number of such subsegments is i * (n - i).
    auto joint_contrib = [&](int i) -> ll {
        return (ll)i * (n - i);
    };

    for (int i = 1; i < n; ++i) {
        if (a[i] != a[i + 1]) {
            ans += joint_contrib(i);
        }
    }

    auto update = [&](int pos, int val) {
        if (pos < 1 || pos > n) return;
        // check joint between pos-1 and pos
        if (pos > 1) {
            bool old_joint = (a[pos - 1] != a[pos]);
            bool new_joint = (a[pos - 1] != val);
            if (old_joint != new_joint) {
                if (new_joint) ans += joint_contrib(pos - 1);
                else ans -= joint_contrib(pos - 1);
            }
        }
        // check joint between pos and pos+1
        if (pos < n) {
            bool old_joint = (a[pos] != a[pos + 1]);
            bool new_joint = (val != a[pos + 1]);
            if (old_joint != new_joint) {
                if (new_joint) ans += joint_contrib(pos);
                else ans -= joint_contrib(pos);
            }
        }
        a[pos] = val;
    };

    while (m--) {
        int i, x;
        cin >> i >> x;
        update(i, x);
        cout << ans << '\n';
    }

    return 0;
}
