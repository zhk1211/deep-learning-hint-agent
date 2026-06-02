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
        vector<int> freq(MAX_A + 1, 0);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
            freq[a[i]]++;
        }

        long long ans = 0;

        // Case 1: b = 1, all three equal
        for (int x = 1; x <= MAX_A; ++x) {
            long long c = freq[x];
            if (c >= 3) {
                ans += c * (c - 1) * (c - 2);
            }
        }

        // Case 2: b > 1, so a_i < a_j < a_k
        // a_j = a_i * b, a_k = a_i * b * b
        for (int i = 1; i <= MAX_A; ++i) {
            if (freq[i] == 0) continue;
            for (int b = 2; i * b * b <= MAX_A; ++b) {
                int j = i * b;
                int k = j * b;
                if (freq[j] > 0 && freq[k] > 0) {
                    ans += 1LL * freq[i] * freq[j] * freq[k];
                }
            }
        }

        cout << ans << '\n';
    }
    return 0;
}
