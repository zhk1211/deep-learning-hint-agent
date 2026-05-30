#include <bits/stdc++.h>
using namespace std;

const long long INF = 1e18;

void solve() {
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<vector<long long>> h(n, vector<long long>(n));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                cin >> h[i][j];
            }
        }
        vector<long long> a(n), b(n);
        for (int i = 0; i < n; ++i) cin >> a[i];
        for (int i = 0; i < n; ++i) cin >> b[i];

        // rows DP
        long long row_min = INF;
        {
            vector<long long> dp = {0, a[0]};
            for (int i = 0; i < n - 1; ++i) {
                bool can[2][2] = {};
                // check all 4 transitions
                for (int s1 = 0; s1 <= 1; ++s1) {
                    for (int s2 = 0; s2 <= 1; ++s2) {
                        int d = s2 - s1; // -1, 0, or 1
                        bool ok = true;
                        for (int j = 0; j < n; ++j) {
                            if (h[i][j] - h[i+1][j] == d) {
                                ok = false;
                                break;
                            }
                        }
                        can[s1][s2] = ok;
                    }
                }
                vector<long long> ndp = {INF, INF};
                for (int s1 = 0; s1 <= 1; ++s1) {
                    if (dp[s1] >= INF) continue;
                    for (int s2 = 0; s2 <= 1; ++s2) {
                        if (can[s1][s2]) {
                            long long cost = dp[s1] + (s2 ? a[i+1] : 0);
                            ndp[s2] = min(ndp[s2], cost);
                        }
                    }
                }
                dp = move(ndp);
            }
            row_min = min(dp[0], dp[1]);
        }

        // columns DP
        long long col_min = INF;
        {
            vector<long long> dp = {0, b[0]};
            for (int j = 0; j < n - 1; ++j) {
                bool can[2][2] = {};
                for (int s1 = 0; s1 <= 1; ++s1) {
                    for (int s2 = 0; s2 <= 1; ++s2) {
                        int d = s2 - s1;
                        bool ok = true;
                        for (int i = 0; i < n; ++i) {
                            if (h[i][j] - h[i][j+1] == d) {
                                ok = false;
                                break;
                            }
                        }
                        can[s1][s2] = ok;
                    }
                }
                vector<long long> ndp = {INF, INF};
                for (int s1 = 0; s1 <= 1; ++s1) {
                    if (dp[s1] >= INF) continue;
                    for (int s2 = 0; s2 <= 1; ++s2) {
                        if (can[s1][s2]) {
                            long long cost = dp[s1] + (s2 ? b[j+1] : 0);
                            ndp[s2] = min(ndp[s2], cost);
                        }
                    }
                }
                dp = move(ndp);
            }
            col_min = min(dp[0], dp[1]);
        }

        if (row_min >= INF/2 || col_min >= INF/2) {
            cout << -1 << '\n';
        } else {
            cout << row_min + col_min << '\n';
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}
