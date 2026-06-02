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
            long long extra = max(0LL, S[i] - 1 - C[i]);
            ans += extra;
            long long cur = C[i] + extra;

            if (i + 1 < n) {
                C[i + 1] += max(0LL, cur - (S[i] - 1));
            }

            for (int j = i + 2; j < n && j <= i + S[i]; ++j) {
                C[j]++;
            }
        }

        cout << ans << '\n';
    }

    return 0;
}
