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
        iota(p.begin(), p.end(), 0);

        if (n % 2 == 0) {
            for (int i = 2; i <= n; i += 2) {
                swap(p[i], p[i + 1]);
            }
        } else {
            if (n >= 5) {
                swap(p[2], p[3]);
                swap(p[4], p[5]);
                for (int i = 6; i <= n; i += 2) {
                    swap(p[i], p[i + 1]);
                }
            }
        }

        for (int i = 1; i <= n; ++i) {
            cout << p[i] << " \n"[i == n];
        }
    }
    return 0;
}
