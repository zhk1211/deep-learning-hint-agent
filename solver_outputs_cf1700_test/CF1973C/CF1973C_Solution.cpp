#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> p(n);
    for (int i = 0; i < n; ++i) {
        cin >> p[i];
    }

    // Find position of n (1-indexed)
    int pos_n = -1;
    for (int i = 0; i < n; ++i) {
        if (p[i] == n) {
            pos_n = i;
            break;
        }
    }

    vector<int> q(n);
    if (pos_n % 2 == 0) {
        // n is at odd position (0-indexed even)
        // We want local maximums at odd indices (0-indexed even) except first
        vector<pair<int, int>> odd_indices, even_indices;
        for (int i = 0; i < n; ++i) {
            if (i % 2 == 0) {
                odd_indices.emplace_back(p[i], i);
            } else {
                even_indices.emplace_back(p[i], i);
            }
        }
        // Sort odd indices by p_i ascending
        sort(odd_indices.begin(), odd_indices.end());
        // Sort even indices by p_i descending
        sort(even_indices.begin(), even_indices.end(), greater<pair<int, int>>());

        int val = n;
        for (auto &pr : odd_indices) {
            q[pr.second] = val--;
        }
        val = 1;
        for (auto &pr : even_indices) {
            q[pr.second] = val++;
        }
    } else {
        // n is at even position (0-indexed odd)
        // Reverse p, compute q for reversed, then reverse q back
        reverse(p.begin(), p.end());
        vector<pair<int, int>> odd_indices, even_indices;
        for (int i = 0; i < n; ++i) {
            if (i % 2 == 0) {
                odd_indices.emplace_back(p[i], i);
            } else {
                even_indices.emplace_back(p[i], i);
            }
        }
        sort(odd_indices.begin(), odd_indices.end());
        sort(even_indices.begin(), even_indices.end(), greater<pair<int, int>>());

        int val = n;
        for (auto &pr : odd_indices) {
            q[pr.second] = val--;
        }
        val = 1;
        for (auto &pr : even_indices) {
            q[pr.second] = val++;
        }
        reverse(q.begin(), q.end());
    }

    for (int i = 0; i < n; ++i) {
        cout << q[i] << " \n"[i == n - 1];
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
