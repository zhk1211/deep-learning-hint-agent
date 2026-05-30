// Hint1
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

        vector<int> vals(n);
        for (int i = 0; i < n; ++i) {
            vals[i] = a[i] + i + 1;
        }

        sort(vals.begin(), vals.end(), greater<int>());

        vector<int> ans;
        ans.push_back(vals[0]);
        for (int i = 1; i < n; ++i) {
            int cur = min(vals[i], ans.back() - 1);
            if (cur > 0) {
                ans.push_back(cur);
            }
        }

        for (int i = 0; i < (int)ans.size(); ++i) {
            cout << ans[i] << " \n"[i == (int)ans.size() - 1];
        }
    }
    return 0;
}
