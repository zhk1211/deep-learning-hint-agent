// Solution
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, m, k;
    cin >> n >> m >> k;
    vector<string> grid(n);
    for (int i = 0; i < n; ++i) {
        cin >> grid[i];
    }
    
    // prefix sum for gold
    vector<vector<int>> pref(n + 1, vector<int>(m + 1, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            pref[i + 1][j + 1] = pref[i][j + 1] + pref[i + 1][j] - pref[i][j] + (grid[i][j] == 'g' ? 1 : 0);
        }
    }
    
    auto get_sum = [&](int r1, int c1, int r2, int c2) -> int {
        if (r1 < 0) r1 = 0;
        if (c1 < 0) c1 = 0;
        if (r2 >= n) r2 = n - 1;
        if (c2 >= m) c2 = m - 1;
        if (r1 > r2 || c1 > c2) return 0;
        return pref[r2 + 1][c2 + 1] - pref[r1][c2 + 1] - pref[r2 + 1][c1] + pref[r1][c1];
    };
    
    int total_gold = get_sum(0, 0, n - 1, m - 1);
    int min_loss = total_gold; // maximum possible loss is all gold
    
    // iterate over all possible centers for the first explosion
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] != '.') continue;
            // square boundaries: rows [i-k, i+k], cols [j-k, j+k]
            int r1 = i - k, r2 = i + k;
            int c1 = j - k, c2 = j + k;
            // gold strictly inside: rows [i-k+1, i+k-1], cols [j-k+1, j+k-1]
            int inside = get_sum(r1 + 1, c1 + 1, r2 - 1, c2 - 1);
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
    while (t--) {
        solve();
    }
    return 0;
}
