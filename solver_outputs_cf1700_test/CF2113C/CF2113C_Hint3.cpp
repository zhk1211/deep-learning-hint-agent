// Hint3
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, m, k;
    cin >> n >> m >> k;
    vector<string> grid(n);
    for (int i = 0; i < n; ++i) cin >> grid[i];

    // prefix sum of gold
    vector<vector<int>> pref(n + 1, vector<int>(m + 1, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            pref[i + 1][j + 1] = pref[i][j + 1] + pref[i + 1][j] - pref[i][j] + (grid[i][j] == 'g' ? 1 : 0);
        }
    }

    auto get_sum = [&](int r1, int c1, int r2, int c2) {
        if (r1 < 0) r1 = 0;
        if (c1 < 0) c1 = 0;
        if (r2 > n) r2 = n;
        if (c2 > m) c2 = m;
        if (r1 >= r2 || c1 >= c2) return 0;
        return pref[r2][c2] - pref[r1][c2] - pref[r2][c1] + pref[r1][c1];
    };

    int total_gold = get_sum(0, 0, n, m);
    int min_loss = total_gold; // we want to minimize gold lost (inside, not on boundary)

    // For each possible empty cell where we can detonate
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] != '.') continue;
            // square boundaries: rows [i-k, i+k], cols [j-k, j+k]
            // boundary gold: rows i-k and i+k, cols j-k to j+k; plus cols j-k and j+k, rows i-k+1 to i+k-1
            // inside gold: rows i-k+1 to i+k-1, cols j-k+1 to j+k-1
            int inside = get_sum(i - k + 1, j - k + 1, i + k, j + k);
            min_loss = min(min_loss, inside);
        }
    }

    cout << total_gold - min_loss << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) solve();
    return 0;
}
