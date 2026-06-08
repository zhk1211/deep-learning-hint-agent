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

        // Group indices by the value of a[i] >> 2
        // Elements can be swapped if their XOR < 4, which means they differ only in the last 2 bits.
        // So elements with the same higher bits (a[i] >> 2) can be freely permuted among themselves.
        map<int, vector<int>> groups;
        for (int i = 0; i < n; ++i) {
            groups[a[i] >> 2].push_back(i);
        }

        vector<int> ans(n);
        for (auto& [key, indices] : groups) {
            // Collect the values in this group
            vector<int> vals;
            for (int idx : indices) {
                vals.push_back(a[idx]);
            }
            // Sort values to place smallest possible at smallest index
            sort(vals.begin(), vals.end());
            // Sort indices to assign sorted values in order
            sort(indices.begin(), indices.end());
            for (size_t i = 0; i < indices.size(); ++i) {
                ans[indices[i]] = vals[i];
            }
        }

        for (int i = 0; i < n; ++i) {
            cout << ans[i] << " \n"[i == n - 1];
        }
    }
    return 0;
}
