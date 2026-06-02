#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int MAX_A = 1000000;
    vector<int> freq(MAX_A + 1, 0);

    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<int> a(n);
        int max_val = 0;
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
            freq[a[i]]++;
            if (a[i] > max_val) max_val = a[i];
        }

        long long ans = 0;

        // Case 1: b = 1, all three equal
        for (int x = 1; x <= max_val; ++x) {
            long long c = freq[x];
            if (c >= 3) {
                ans += c * (c - 1) * (c - 2);
            }
        }

        // Case 2: b > 1, center is j, a_i = a_j / b, a_k = a_j * b
        for (int j_val = 1; j_val <= max_val; ++j_val) {
            if (freq[j_val] == 0) continue;
            // b starts from 2, because b=1 handled above
            for (int b = 2; j_val * b <= max_val; ++b) {
                int i_val = j_val / b;
                if (i_val * b != j_val) continue; // not divisible
                int k_val = j_val * b;
                if (k_val > max_val) break;
                if (freq[i_val] && freq[k_val]) {
                    ans += (long long)freq[i_val] * freq[j_val] * freq[k_val];
                }
            }
        }

        cout << ans << '\n';

        // reset frequencies
        for (int x : a) freq[x] = 0;
    }
    return 0;
}
