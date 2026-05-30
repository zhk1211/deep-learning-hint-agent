// Hint6
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
        for (int i = 0; i < n; ++i) {
            cin >> p[i];
        }

        vector<int> idx(n);
        iota(idx.begin(), idx.end(), 0);
        sort(idx.begin(), idx.end(), [&](int i, int j) {
            return p[i] < p[j];
        });

        vector<int> q(n);
        int half = n / 2;
        for (int i = 0; i < half; ++i) {
            q[idx[i]] = n - i;
        }
        for (int i = half; i < n; ++i) {
            q[idx[i]] = n - (i - half);
        }

        for (int i = 0; i < n; ++i) {
            cout << q[i] << " \n"[i == n - 1];
        }
    }
    return 0;
}
