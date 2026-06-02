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
        vector<int> freq(MAX_A + 1, 0);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
            freq[a[i]]++;
        }

        long long ans = 0;
        for (int j_val = 1; j_val <= MAX_A; ++j_val) {
            int cnt_j = freq[j_val];
            if (cnt_j == 0) continue;

            // b = 1 case: i, j, k all equal value
            if (cnt_j >= 3) {
                ans += 1LL * cnt_j * (cnt_j - 1) * (cnt_j - 2);
            }

            // b > 1 case
            for (int b = 2; 1LL * j_val * b <= MAX_A; ++b) {
                int i_val = j_val / b;
                if (i_val * b != j_val) continue; // not divisible
                int k_val = j_val * b;
                if (k_val > MAX_A) break;
                int cnt_i = freq[i_val];
                int cnt_k = freq[k_val];
                if (cnt_i > 0 && cnt_k > 0) {
                    ans += 1LL * cnt_i * cnt_j * cnt_k;
                }
            }
        }
        cout << ans << '\n';
    }
    return 0;
}
