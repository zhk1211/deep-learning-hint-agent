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
        vector<pair<int, int>> pts(n);
        vector<int> idx(n);
        for (int i = 0; i < n; ++i) {
            cin >> pts[i].first >> pts[i].second;
            idx[i] = i;
        }

        // Sort by x + y
        sort(idx.begin(), idx.end(), [&](int i, int j) {
            return pts[i].first + pts[i].second < pts[j].first + pts[j].second;
        });

        vector<int> pair1(n);
        for (int i = 0; i < n; i += 2) {
            pair1[idx[i]] = idx[i + 1];
            pair1[idx[i + 1]] = idx[i];
        }

        // Sort by x - y
        sort(idx.begin(), idx.end(), [&](int i, int j) {
            return pts[i].first - pts[i].second < pts[j].first - pts[j].second;
        });

        vector<int> pair2(n);
        for (int i = 0; i < n; i += 2) {
            pair2[idx[i]] = idx[i + 1];
            pair2[idx[i + 1]] = idx[i];
        }

        // Choose the pairing with larger total distance
        long long sum1 = 0, sum2 = 0;
        for (int i = 0; i < n; ++i) {
            if (i < pair1[i]) {
                sum1 += abs(pts[i].first - pts[pair1[i]].first) + abs(pts[i].second - pts[pair1[i]].second);
            }
            if (i < pair2[i]) {
                sum2 += abs(pts[i].first - pts[pair2[i]].first) + abs(pts[i].second - pts[pair2[i]].second);
            }
        }

        vector<int> chosen = (sum1 >= sum2) ? pair1 : pair2;

        vector<bool> used(n, false);
        for (int i = 0; i < n; ++i) {
            if (!used[i]) {
                int j = chosen[i];
                used[i] = used[j] = true;
                cout << i + 1 << ' ' << j + 1 << '\n';
            }
        }
    }
    return 0;
}
