// Hint2
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

        // Place 1 at position 1 (fixed point)
        p[1] = 1;
        used[1] = true;

        // For each prime > 1, we can form cycles of numbers that are multiples of that prime
        // We'll process numbers from 2 to n, grouping by smallest prime factor
        vector<int> primes;
        vector<int> spf(n + 1, 0); // smallest prime factor
        for (int i = 2; i <= n; i++) {
            if (spf[i] == 0) {
                spf[i] = i;
                primes.push_back(i);
            }
            for (int p : primes) {
                if (p > spf[i] || i * p > n) break;
                spf[i * p] = p;
            }
        }

        // Group numbers by their smallest prime factor
        vector<vector<int>> groups(n + 1);
        for (int i = 2; i <= n; i++) {
            groups[spf[i]].push_back(i);
        }

        // For each prime group, form a cycle
        for (int prime : primes) {
            auto& group = groups[prime];
            if (group.empty()) continue;
            // Cycle the group: each element points to the next, last points to first
            int sz = group.size();
            for (int j = 0; j < sz; j++) {
                int cur = group[j];
                int nxt = group[(j + 1) % sz];
                p[cur] = nxt;
                used[cur] = true;
            }
        }

        // Output permutation
        for (int i = 1; i <= n; i++) {
            cout << p[i] << " \n"[i == n];
        }
    }
    return 0;
}
