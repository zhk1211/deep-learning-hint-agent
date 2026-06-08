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

        // Group by value >> 2 (i.e., remove last two bits)
        map<int, vector<int>> groups;
        map<int, vector<int>> indices;
        for (int i = 0; i < n; ++i) {
            int key = a[i] >> 2;
            groups[key].push_back(a[i]);
            indices[key].push_back(i);
        }

        vector<int> ans(n);
        for (auto &[key, vals] : groups) {
            sort(vals.begin(), vals.end());
            auto &idx = indices[key];
            for (size_t j = 0; j < vals.size(); ++j) {
                ans[idx[j]] = vals[j];
            }
        }

        for (int i = 0; i < n; ++i) {
            cout << ans[i] << " \n"[i == n - 1];
        }
    }
    return 0;
}
