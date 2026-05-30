// Hint1
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

// Fenwick tree for coordinate compressed values
struct Fenwick {
    int n;
    vector<int> bit;
    Fenwick(int n) : n(n), bit(n + 1, 0) {}
    void add(int idx, int delta) {
        for (; idx <= n; idx += idx & -idx) bit[idx] += delta;
    }
    int sum(int idx) {
        int s = 0;
        for (; idx > 0; idx -= idx & -idx) s += bit[idx];
        return s;
    }
    int range_sum(int l, int r) {
        if (l > r) return 0;
        return sum(r) - sum(l - 1);
    }
};

void solve() {
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<int> a(n);
        for (int i = 0; i < n; ++i) cin >> a[i];

        // coordinate compression
        vector<int> vals = a;
        sort(vals.begin(), vals.end());
        vals.erase(unique(vals.begin(), vals.end()), vals.end());
        int m = vals.size();
        auto get_idx = [&](int x) {
            return lower_bound(vals.begin(), vals.end(), x) - vals.begin() + 1;
        };

        Fenwick bit(m);
        ll ans = 0;
        // We process elements one by one.
        // For each element, we can put it either at front or back.
        // The number of new inversions created with already placed elements
        // if placed at front = number of already placed elements that are smaller than current
        // if placed at back = number of already placed elements that are larger than current
        // We choose the minimum.
        for (int i = 0; i < n; ++i) {
            int idx = get_idx(a[i]);
            int less = bit.sum(idx - 1);
            int greater = bit.range_sum(idx + 1, m);
            ans += min(less, greater);
            bit.add(idx, 1);
        }
        cout << ans << '\n';
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}
