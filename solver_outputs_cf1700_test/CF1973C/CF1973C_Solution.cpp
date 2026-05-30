// Solution
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> p(n);
    for (int i = 0; i < n; ++i) {
        cin >> p[i];
    }

    vector<int> q(n);
    vector<int> odd_idx, even_idx;
    for (int i = 0; i < n; ++i) {
        if (i % 2 == 0) odd_idx.push_back(i);
        else even_idx.push_back(i);
    }

    // Check if n is at odd position (0-indexed even)
    bool n_at_odd = false;
    for (int i = 0; i < n; ++i) {
        if (p[i] == n && i % 2 == 0) {
            n_at_odd = true;
            break;
        }
    }

    if (n_at_odd) {
        // Place large numbers (n/2+1 .. n) at odd indices
        // Sort odd indices by p[i] ascending
        sort(odd_idx.begin(), odd_idx.end(), [&](int i, int j) {
            return p[i] < p[j];
        });
        // Place small numbers (1 .. n/2) at even indices
        // Sort even indices by p[i] descending
        sort(even_idx.begin(), even_idx.end(), [&](int i, int j) {
            return p[i] > p[j];
        });

        int val = n;
        for (int idx : odd_idx) {
            q[idx] = val--;
        }
        val = 1;
        for (int idx : even_idx) {
            q[idx] = val++;
        }
    } else {
        // n is at even position, reverse p, compute q, reverse back
        reverse(p.begin(), p.end());
        vector<int> q_rev(n);
        odd_idx.clear();
        even_idx.clear();
        for (int i = 0; i < n; ++i) {
            if (i % 2 == 0) odd_idx.push_back(i);
            else even_idx.push_back(i);
        }
        sort(odd_idx.begin(), odd_idx.end(), [&](int i, int j) {
            return p[i] < p[j];
        });
        sort(even_idx.begin(), even_idx.end(), [&](int i, int j) {
            return p[i] > p[j];
        });
        int val = n;
        for (int idx : odd_idx) {
            q_rev[idx] = val--;
        }
        val = 1;
        for (int idx : even_idx) {
            q_rev[idx] = val++;
        }
        reverse(q_rev.begin(), q_rev.end());
        q = q_rev;
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
