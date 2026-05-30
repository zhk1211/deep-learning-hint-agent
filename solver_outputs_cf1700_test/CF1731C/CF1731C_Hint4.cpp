// Hint4
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
        vector<int> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }

        // Maximum XOR value is less than 2*n
        int max_xor = 1;
        while (max_xor <= 2 * n) max_xor <<= 1;
        max_xor = min(max_xor, 1 << 19); // safe upper bound

        vector<int> freq(max_xor, 0);
        freq[0] = 1;
        int pref = 0;
        long long total_subarrays = (long long)n * (n + 1) / 2;
        long long odd_div_subarrays = 0;

        // Precompute perfect squares up to max_xor - 1
        vector<int> squares;
        for (int i = 0; i * i < max_xor; ++i) {
            squares.push_back(i * i);
        }

        for (int i = 0; i < n; ++i) {
            pref ^= a[i];
            // Count subarrays ending at i with XOR equal to a perfect square
            for (int sq : squares) {
                int target = pref ^ sq;
                if (target < max_xor) {
                    odd_div_subarrays += freq[target];
                }
            }
            freq[pref]++;
        }

        long long ans = total_subarrays - odd_div_subarrays;
        cout << ans << '\n';
    }
    return 0;
}
