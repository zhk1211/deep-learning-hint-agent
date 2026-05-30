// Hint5
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
        vector<pair<int, int>> pts(n);
        vector<int> idx_x(n), idx_y(n);
        for (int i = 0; i < n; ++i) {
            cin >> pts[i].first >> pts[i].second;
            idx_x[i] = i;
            idx_y[i] = i;
        }

        // Sort indices by x coordinate
        sort(idx_x.begin(), idx_x.end(), [&](int a, int b) {
            return pts[a].first < pts[b].first;
        });
        // Sort indices by y coordinate
        sort(idx_y.begin(), idx_y.end(), [&](int a, int b) {
            return pts[a].second < pts[b].second;
        });

        // Determine sets
        vector<bool> in_Xl(n, false), in_Yl(n, false);
        for (int i = 0; i < n / 2; ++i) {
            in_Xl[idx_x[i]] = true;
            in_Yl[idx_y[i]] = true;
        }

        // Build bipartite graph: left = Xl, right = Xr
        // We need a perfect matching where each left is matched to a right.
        // Also each Yl must be matched to a Yr.
        // We can construct a matching greedily using a queue of available right-side points.
        // We'll iterate over left points (Xl) and match them with any available right point.
        // To satisfy Y condition, we can prioritize matching Yl with Yr, but any matching works
        // as long as we pair Xl with Xr and Yl with Yr. The existence is guaranteed by Hall's theorem.
        // Simple approach: collect left and right sets, then pair them arbitrarily.

        vector<int> left, right;
        for (int i = 0; i < n; ++i) {
            if (in_Xl[i]) left.push_back(i);
            else right.push_back(i);
        }

        // We need to pair left with right such that Yl are paired with Yr.
        // We can just output left[i] with right[i] after sorting left and right appropriately?
        // Not necessarily, but we can construct a valid pairing by matching Yl left with Yr right first.
        // Since both sets have size n/2, we can just output any pairing that respects the Y condition.
        // Let's separate left into Yl_left and Yr_left, and right into Yl_right and Yr_right.
        vector<int> Yl_left, Yr_left, Yl_right, Yr_right;
        for (int x : left) {
            if (in_Yl[x]) Yl_left.push_back(x);
            else Yr_left.push_back(x);
        }
        for (int x : right) {
            if (in_Yl[x]) Yl_right.push_back(x);
            else Yr_right.push_back(x);
        }

        // Match Yl_left with Yr_right, and Yr_left with Yl_right.
        // This ensures Yl paired with Yr.
        vector<pair<int, int>> ans;
        for (size_t i = 0; i < Yl_left.size(); ++i) {
            ans.emplace_back(Yl_left[i], Yr_right[i]);
        }
        for (size_t i = 0; i < Yr_left.size(); ++i) {
            ans.emplace_back(Yr_left[i], Yl_right[i]);
        }

        for (auto [a, b] : ans) {
            cout << a + 1 << ' ' << b + 1 << '\n';
        }
    }
    return 0;
}
