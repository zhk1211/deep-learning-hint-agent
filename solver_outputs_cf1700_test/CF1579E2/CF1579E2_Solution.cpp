#include <bits/stdc++.h>
using namespace std;

// Fenwick tree (Binary Indexed Tree) for order statistics
struct Fenwick {
    int n;
    vector<int> bit;
    Fenwick(int n) : n(n), bit(n + 1, 0) {}
    void add(int idx, int delta) {
        for (; idx <= n; idx += idx & -idx)
            bit[idx] += delta;
    }
    int sum(int idx) {
        int res = 0;
        for (; idx > 0; idx -= idx & -idx)
            res += bit[idx];
        return res;
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

        // Coordinate compression
        vector<int> vals = a;
        sort(vals.begin(), vals.end());
        vals.erase(unique(vals.begin(), vals.end()), vals.end());
        int m = vals.size();

        Fenwick bit(m);
        long long inversions = 0;

        for (int i = 0; i < n; ++i) {
            int idx = lower_bound(vals.begin(), vals.end(), a[i]) - vals.begin() + 1;
            int less = bit.sum(idx - 1);
            int greater = bit.range_sum(idx + 1, m);
            inversions += min(less, greater);
            bit.add(idx, 1);
        }

        cout << inversions << '\n';
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}
