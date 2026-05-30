// Hint3
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

        vector<int> left(n, -1), right(n, -1);
        vector<int> stk;
        for (int i = 0; i < n; ++i) {
            int last = -1;
            while (!stk.empty() && a[stk.back()] < a[i]) {
                last = stk.back();
                stk.pop_back();
            }
            if (!stk.empty()) {
                right[stk.back()] = i;
            }
            if (last != -1) {
                left[i] = last;
            }
            stk.push_back(i);
        }

        int root = stk.empty() ? -1 : stk[0];

        vector<int> depth(n);
        function<void(int)> dfs = [&](int u) {
            if (left[u] != -1) {
                depth[left[u]] = depth[u] + 1;
                dfs(left[u]);
            }
            if (right[u] != -1) {
                depth[right[u]] = depth[u] + 1;
                dfs(right[u]);
            }
        };
        if (root != -1) {
            depth[root] = 0;
            dfs(root);
        }

        int max_depth = 0;
        for (int d : depth) {
            max_depth = max(max_depth, d);
        }

        cout << max_depth << '\n';
    }
    return 0;
}
