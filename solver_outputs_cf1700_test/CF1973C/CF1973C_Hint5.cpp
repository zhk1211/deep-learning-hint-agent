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
        vector<int> p(n);
        for (int i = 0; i < n; ++i) {
            cin >> p[i];
        }

        // We want to maximize the number of local maxima in a_i = p_i + q_i.
        // The maximum possible number of local maxima is n/2 - 1.
        // Strategy: assign large q values to positions where p is small,
        // and small q values to positions where p is large, so that the sums
        // alternate high-low-high-low... with peaks at positions where p is small.
        // Specifically, we want peaks at indices i where p_i <= n/2 (the smaller half),
        // and valleys at indices where p_i > n/2 (the larger half).
        // Then we assign q values: for peaks, assign from n down to n/2+1 (large),
        // for valleys, assign from 1 to n/2 (small). To ensure strict inequalities,
        // we sort the peaks by p_i ascending and assign the largest q to the smallest p,
        // and sort valleys by p_i descending and assign the smallest q to the largest p.

        vector<int> peaks, valleys;
        for (int i = 0; i < n; ++i) {
            if (p[i] <= n / 2) {
                peaks.push_back(i);
            } else {
                valleys.push_back(i);
            }
        }

        // Sort peaks by p[i] ascending
        sort(peaks.begin(), peaks.end(), [&](int i, int j) {
            return p[i] < p[j];
        });
        // Sort valleys by p[i] descending
        sort(valleys.begin(), valleys.end(), [&](int i, int j) {
            return p[i] > p[j];
        });

        vector<int> q(n);
        int val = n;
        for (int idx : peaks) {
            q[idx] = val--;
        }
        val = 1;
        for (int idx : valleys) {
            q[idx] = val++;
        }

        for (int i = 0; i < n; ++i) {
            cout << q[i] << " \n"[i == n - 1];
        }
    }
    return 0;
}
