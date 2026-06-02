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
        // b = 1 case
        for (int i = 0; i < n; ++i) {
            int c = cnt[a[i]];
            if (c >= 3) {
                ans += 1LL * (c - 1) * (c - 2);
            }
        }
        // b >= 2 case
        for (int i = 0; i < n; ++i) {
            int x = a[i];
            // iterate b such that x * b * b <= MAX_A
            for (int b = 2; 1LL * x * b * b <= MAX_A; ++b) {
                int y = x * b;
                int z = y * b;
                if (y <= MAX_A && z <= MAX_A) {
                    ans += 1LL * cnt[y] * cnt[z];
                }
            }
        }

        cout << ans << '\n';
    }
    return 0;
}
