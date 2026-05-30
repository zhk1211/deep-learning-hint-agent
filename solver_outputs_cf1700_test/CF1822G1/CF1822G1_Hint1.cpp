// Hint1
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int MAX_A = 1000000;
    vector<int> cnt(MAX_A + 1, 0);

    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<int> a(n);
        int max_val = 0;
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
            cnt[a[i]]++;
            max_val = max(max_val, a[i]);
        }

        long long ans = 0;

        // Case 1: b = 1, all three equal
        for (int x = 1; x <= max_val; ++x) {
            if (cnt[x] >= 3) {
                long long c = cnt[x];
                ans += c * (c - 1) * (c - 2);
            }
        }

        // Case 2: b > 1, distinct values or two equal
        for (int x = 1; x <= max_val; ++x) {
            if (cnt[x] == 0) continue;
            for (int b = 2; x * b * b <= max_val; ++b) {
                int y = x * b;
                int z = y * b;
                if (z > max_val) break;
                if (cnt[y] > 0 && cnt[z] > 0) {
                    ans += 1LL * cnt[x] * cnt[y] * cnt[z];
                }
            }
        }

        cout << ans << '\n';

        // Reset cnt for next test case
        for (int x : a) {
            cnt[x] = 0;
        }
    }

    return 0;
}
