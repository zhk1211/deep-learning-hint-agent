// Hint0
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
        vector<long long> s(n);
        for (int i = 0; i < n; ++i) {
            cin >> s[i];
        }

        vector<long long> extra(n + 1, 0);
        long long ans = 0;

        for (int i = 0; i < n; ++i) {
            if (i > 0) extra[i] += extra[i - 1];
            long long cur = s[i] - 1;
            long long need = max(0LL, cur - extra[i]);
            ans += need;
            extra[i + 1] += max(0LL, extra[i] - cur);
            if (i + 2 < n) {
                int r = min(n - 1, i + (int)s[i]);
                if (r >= i + 2) {
                    extra[i + 2]++;
                    if (r + 1 < n) extra[r + 1]--;
                }
            }
        }
        cout << ans << '\n';
    }
    return 0;
}
