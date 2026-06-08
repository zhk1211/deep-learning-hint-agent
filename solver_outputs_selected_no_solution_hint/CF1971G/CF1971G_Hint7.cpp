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
        vector<int> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }

        // Group by value >> 2 (i.e., all bits except the last two)
        // Elements in the same group can be swapped because their XOR < 4
        // iff they differ only in the last two bits.
        map<int, vector<int>> groups;
        map<int, vector<int>> indices;
        for (int i = 0; i < n; ++i) {
            int key = a[i] >> 2;
            groups[key].push_back(a[i]);
            indices[key].push_back(i);
        }

        vector<int> ans(n);
        for (auto& [key, vals] : groups) {
            sort(vals.begin(), vals.end());
            const auto& idxs = indices[key];
            for (size_t i = 0; i < vals.size(); ++i) {
                ans[idxs[i]] = vals[i];
            }
        }

        for (int i = 0; i < n; ++i) {
            cout << ans[i] << " \n"[i == n - 1];
        }
    }
    return 0;
}
