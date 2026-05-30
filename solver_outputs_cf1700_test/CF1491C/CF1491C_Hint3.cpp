// Hint3
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

        vector<long long> extra(n + 1, 0);
        long long ans = 0;

        for (int i = 0; i < n; ++i) {
            if (i > 0) extra[i] += extra[i - 1];
            long long cur = S[i] + extra[i];
            if (cur > 1) {
                long long need = cur - 1;
                ans += need;
                if (i + 2 < n) {
                    extra[i + 2] += 1;
                    int to = min(n, i + (int)S[i] + 1);
                    if (to < n) extra[to] -= 1;
                }
                // extra passes beyond what S[i] can handle
                if (need > S[i] - 1) {
                    long long leftover = need - (S[i] - 1);
                    if (i + 1 < n) {
                        extra[i + 1] += leftover;
                        extra[i + 2] -= leftover;
                    }
                }
            }
        }

        cout << ans << '\n';
    }

    return 0;
}
