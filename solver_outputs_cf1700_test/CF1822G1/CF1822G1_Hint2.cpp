// Hint2
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int MAX_A = 1000000;

    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<int> a(n);
        vector<int> cnt(MAX_A + 1, 0);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
            cnt[a[i]]++;
        }

        long long ans = 0;

        // b = 1 case
        for (int x = 1; x <= MAX_A; ++x) {
            if (cnt[x] >= 3) {
                ans += 1LL * cnt[x] * (cnt[x] - 1) * (cnt[x] - 2);
            }
        }

        // b >= 2 case
        for (int x = 1; x <= MAX_A; ++x) {
            if (cnt[x] == 0) continue;
            for (int b = 2; 1LL * x * b * b <= MAX_A; ++b) {
                int y = x * b;
                int z = y * b;
                if (z > MAX_A) break;
                if (cnt[y] > 0 && cnt[z] > 0) {
                    ans += 1LL * cnt[x] * cnt[y] * cnt[z];
                }
            }
        }

        cout << ans << '\n';
    }

    return 0;
}
