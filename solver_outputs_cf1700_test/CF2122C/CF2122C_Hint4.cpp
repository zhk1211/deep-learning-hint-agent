// Hint4
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
        vector<int> idx(n);
        for (int i = 0; i < n; ++i) {
            cin >> pts[i].first >> pts[i].second;
            idx[i] = i;
        }

        // Sort by x-coordinate
        sort(idx.begin(), idx.end(), [&](int i, int j) {
            return pts[i].first < pts[j].first;
        });

        vector<int> left_half, right_half;
        for (int i = 0; i < n / 2; ++i) left_half.push_back(idx[i]);
        for (int i = n / 2; i < n; ++i) right_half.push_back(idx[i]);

        // Sort left half by y-coordinate
        sort(left_half.begin(), left_half.end(), [&](int i, int j) {
            return pts[i].second < pts[j].second;
        });
        // Sort right half by y-coordinate
        sort(right_half.begin(), right_half.end(), [&](int i, int j) {
            return pts[i].second < pts[j].second;
        });

        // Pair smallest y from left with smallest y from right, etc.
        for (int i = 0; i < n / 2; ++i) {
            cout << left_half[i] + 1 << ' ' << right_half[i] + 1 << '\n';
        }
    }
    return 0;
}
