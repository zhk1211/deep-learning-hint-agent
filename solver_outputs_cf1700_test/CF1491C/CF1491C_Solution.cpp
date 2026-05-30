// Solution
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
        vector<long long> S(n);
        for (int i = 0; i < n; ++i) {
            cin >> S[i];
        }

        vector<long long> C(n + 1, 0);
        long long ans = 0;

        for (int i = 0; i < n; ++i) {
            if (S[i] > C[i] + 1) {
                ans += S[i] - C[i] - 1;
                C[i] = S[i] - 1;
            }

            long long extra = C[i] - (S[i] - 1);
            if (extra > 0) {
                if (i + 1 < n) {
                    C[i + 1] += extra;
                }
            }

            for (int j = 2; j <= S[i] && i + j < n; ++j) {
                C[i + j]++;
            }
        }

        cout << ans << '\n';
    }

    return 0;
}
