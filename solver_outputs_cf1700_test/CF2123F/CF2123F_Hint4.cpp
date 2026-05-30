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

        vector<int> p(n + 1);
        vector<bool> used(n + 1, false);

        // Fixed points: 1 and all primes > n/2
        p[1] = 1;
        used[1] = true;

        // Find primes > n/2
        vector<bool> is_prime(n + 1, true);
        is_prime[0] = is_prime[1] = false;
        for (int i = 2; i * i <= n; ++i) {
            if (is_prime[i]) {
                for (int j = i * i; j <= n; j += i) {
                    is_prime[j] = false;
                }
            }
        }

        for (int i = 2; i <= n; ++i) {
            if (is_prime[i] && i > n / 2) {
                p[i] = i;
                used[i] = true;
            }
        }

        // Group remaining numbers by smallest prime factor
        vector<vector<int>> groups;
        for (int i = 2; i <= n; ++i) {
            if (!used[i]) {
                // Find smallest prime factor
                int spf = -1;
                for (int d = 2; d * d <= i; ++d) {
                    if (i % d == 0) {
                        spf = d;
                        break;
                    }
                }
                if (spf == -1) spf = i; // prime itself, but already handled if > n/2

                // Find or create group for this spf
                bool found = false;
                for (auto& g : groups) {
                    if (!g.empty()) {
                        int rep = g[0];
                        int rep_spf = -1;
                        for (int d = 2; d * d <= rep; ++d) {
                            if (rep % d == 0) {
                                rep_spf = d;
                                break;
                            }
                        }
                        if (rep_spf == -1) rep_spf = rep;
                        if (rep_spf == spf) {
                            g.push_back(i);
                            found = true;
                            break;
                        }
                    }
                }
                if (!found) {
                    groups.push_back({i});
                }
            }
        }

        // For each group, rotate elements
        for (auto& g : groups) {
            int sz = g.size();
            if (sz == 1) {
                // Should not happen because primes > n/2 are fixed, others have at least one multiple
                // But just in case, assign to itself (but it's not used yet)
                p[g[0]] = g[0];
            } else {
                for (int i = 0; i < sz; ++i) {
                    p[g[i]] = g[(i + 1) % sz];
                }
            }
        }

        // Output
        for (int i = 1; i <= n; ++i) {
            cout << p[i] << " \n"[i == n];
        }
    }

    return 0;
}
