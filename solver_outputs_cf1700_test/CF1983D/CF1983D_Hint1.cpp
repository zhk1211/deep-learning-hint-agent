// Hint1
#include <bits/stdc++.h>
using namespace std;

// Fenwick tree for counting inversions
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
};

// Count inversions modulo 2
int inversions_parity(const vector<int>& a) {
    int n = a.size();
    // coordinate compression
    vector<int> sorted = a;
    sort(sorted.begin(), sorted.end());
    sorted.erase(unique(sorted.begin(), sorted.end()), sorted.end());
    Fenwick ft(sorted.size());
    int inv = 0;
    for (int i = n - 1; i >= 0; --i) {
        int pos = lower_bound(sorted.begin(), sorted.end(), a[i]) - sorted.begin() + 1;
        inv ^= (ft.sum(pos - 1) & 1);
        ft.add(pos, 1);
    }
    return inv;
}

void solve() {
    int n;
    cin >> n;
    vector<int> a(n), b(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    for (int i = 0; i < n; ++i) cin >> b[i];

    // Check if multisets are equal
    vector<int> sa = a, sb = b;
    sort(sa.begin(), sa.end());
    sort(sb.begin(), sb.end());
    if (sa != sb) {
        cout << "NO\n";
        return;
    }

    // If there are duplicate elements, we can always achieve same arrays
    // because we can swap duplicates to adjust parity.
    // But problem says distinct integers in each array, so no duplicates.
    // However, the multiset check already ensures elements are same set.
    // Since elements are distinct, we just need parity of inversions to match.
    if (inversions_parity(a) == inversions_parity(b)) {
        cout << "YES\n";
    } else {
        cout << "NO\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}
