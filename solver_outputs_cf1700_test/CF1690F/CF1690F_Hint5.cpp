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
        string s;
        cin >> s;
        vector<int> p(n);
        for (int i = 0; i < n; ++i) {
            cin >> p[i];
            --p[i];
        }

        vector<bool> visited(n, false);
        long long ans = 1;

        for (int i = 0; i < n; ++i) {
            if (!visited[i]) {
                vector<int> cycle;
                int cur = i;
                while (!visited[cur]) {
                    visited[cur] = true;
                    cycle.push_back(cur);
                    cur = p[cur];
                }

                int len = cycle.size();
                string orig;
                for (int idx : cycle) orig += s[idx];

                // find minimal shift k (1 <= k <= len) such that shifting orig by k gives orig
                int min_shift = len;
                for (int k = 1; k <= len; ++k) {
                    bool ok = true;
                    for (int j = 0; j < len; ++j) {
                        if (orig[j] != orig[(j + k) % len]) {
                            ok = false;
                            break;
                        }
                    }
                    if (ok) {
                        min_shift = k;
                        break;
                    }
                }

                ans = lcm(ans, (long long)min_shift);
            }
        }

        cout << ans << '\n';
    }

    return 0;
}
