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
        if (n == 2) {
            cout << "2 1\n";
            continue;
        }
        if (n == 3) {
            cout << "2 1 3\n";
            continue;
        }
        // We want c_i to be prime for as many i as possible.
        // Strategy: put n at position 2, then fill the rest.
        // p[1] = 2, p[2] = n, then fill remaining numbers in increasing order.
        p[0] = 2;
        p[1] = n;
        int cur = 1;
        for (int i = 2; i < n; ++i) {
            if (cur == 2) cur++;
            if (cur == n) cur++;
            p[i] = cur++;
        }
        for (int i = 0; i < n; ++i) {
            cout << p[i] << " \n"[i == n - 1];
        }
    }
    return 0;
}
