// Hint5
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n, m;
        cin >> n >> m;
        vector<int> s(m);
        for (int i = 0; i < m; ++i) {
            cin >> s[i];
        }

        vector<int> a(n + 1, -1);
        bool possible = true;

        for (int i = 1; i <= n; ++i) {
            if (a[i] != -1) continue;
            // find the largest element in S that can be placed at i
            // and all multiples of i
            int best = -1;
            for (int val : s) {
                bool ok = true;
                // check if val divides all already assigned multiples of i
                for (int j = i; j <= n; j += i) {
                    if (a[j] != -1 && a[j] % val != 0) {
                        ok = false;
                        break;
                    }
                }
                if (ok) best = val;
            }
            if (best == -1) {
                possible = false;
                break;
            }
            // assign best to all unassigned multiples of i
            for (int j = i; j <= n; j += i) {
                if (a[j] == -1) a[j] = best;
            }
        }

        if (!possible) {
            cout << "-1\n";
        } else {
            for (int i = 1; i <= n; ++i) {
                cout << a[i] << " \n"[i == n];
            }
        }
    }
    return 0;
}
