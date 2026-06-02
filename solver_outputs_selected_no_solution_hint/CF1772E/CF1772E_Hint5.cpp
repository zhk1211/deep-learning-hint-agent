#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<int> p(n);
        for (int i = 0; i < n; ++i) {
            cin >> p[i];
        }

        // Check if already sorted ascending or descending (should not happen per problem)
        // but we can still handle safely.

        // Count inversions parity
        // Use Fenwick tree or just count directly? n up to 5e5, sum n 5e5, so O(n log n) is fine.
        // We only need parity, so we can use order statistic tree or simple BIT.
        // BIT size n
        vector<int> bit(n + 1, 0);
        auto add = [&](int idx, int val) {
            for (; idx <= n; idx += idx & -idx) bit[idx] += val;
        };
        auto sum = [&](int idx) {
            int res = 0;
            for (; idx > 0; idx -= idx & -idx) res += bit[idx];
            return res;
        };

        long long inv = 0;
        for (int i = n - 1; i >= 0; --i) {
            inv += sum(p[i] - 1);
            add(p[i], 1);
        }

        // Check if permutation is already sorted ascending or descending
        bool asc = true, desc = true;
        for (int i = 0; i < n; ++i) {
            if (p[i] != i + 1) asc = false;
            if (p[i] != n - i) desc = false;
        }

        if (asc) {
            cout << "First\n";
            continue;
        }
        if (desc) {
            cout << "Second\n";
            continue;
        }

        // According to hints: parity of inversions determines winner
        if (inv % 2 == 0) {
            cout << "Second\n";
        } else {
            cout << "First\n";
        }
    }
    return 0;
}
