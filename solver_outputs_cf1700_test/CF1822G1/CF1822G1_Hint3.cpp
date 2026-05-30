// Hint3
#include <bits/stdc++.h>
using namespace std;

const int MAX_A = 1000000;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

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

        // b = 1
        for (int x = 1; x <= MAX_A; ++x) {
            if (cnt[x] >= 3) {
                long long c = cnt[x];
                ans += c * (c - 1) * (c - 2);
            }
        }

        // b >= 2
        int max_val = *max_element(a.begin(), a.end());
        int sqrt_max = (int)sqrt(max_val);
        for (int b = 2; b <= sqrt_max; ++b) {
            for (int x = 1; x * b * b <= max_val; ++x) {
                if (cnt[x] && cnt[x * b] && cnt[x * b * b]) {
                    ans += 1LL * cnt[x] * cnt[x * b] * cnt[x * b * b];
                }
            }
        }

        cout << ans << '\n';
    }
    return 0;
}
