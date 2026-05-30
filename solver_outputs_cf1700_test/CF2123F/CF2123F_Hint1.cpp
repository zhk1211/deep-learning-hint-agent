// Hint1
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

        // Place 1 at position 1
        p[1] = 1;
        used[1] = true;

        // For each position i from 2 to n, find smallest unused number j such that gcd(i, j) > 1
        for (int i = 2; i <= n; ++i) {
            bool placed = false;
            for (int j = 1; j <= n; ++j) {
                if (!used[j] && gcd(i, j) > 1) {
                    p[i] = j;
                    used[j] = true;
                    placed = true;
                    break;
                }
            }
            if (!placed) {
                // Should not happen for n >= 2
                for (int j = 1; j <= n; ++j) {
                    if (!used[j]) {
                        p[i] = j;
                        used[j] = true;
                        break;
                    }
                }
            }
        }

        for (int i = 1; i <= n; ++i) {
            cout << p[i] << " \n"[i == n];
        }
    }
    return 0;
}
