// Hint1
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

        // A number has an odd number of divisors iff it is a perfect square.
        // XOR of a subarray has even divisors iff it is NOT a perfect square.
        // We count total subarrays and subtract those with XOR being a perfect square.
        // Since a_i <= n, max XOR value is < 2^18 (since n <= 2e5 < 2^18).
        // We can maintain prefix XOR and count frequencies.

        const int MAX_VAL = 1 << 18; // 262144
        vector<int> freq(MAX_VAL, 0);
        freq[0] = 1;
        int pref = 0;
        long long total = (long long)n * (n + 1) / 2;
        long long odd_div = 0;

        // Precompute squares up to MAX_VAL
        vector<int> squares;
        for (int i = 0; i * i < MAX_VAL; ++i) {
            squares.push_back(i * i);
        }

        for (int i = 0; i < n; ++i) {
            pref ^= a[i];
            // Count subarrays ending at i with XOR equal to a perfect square
            for (int sq : squares) {
                int need = pref ^ sq;
                if (need < MAX_VAL) {
                    odd_div += freq[need];
                }
            }
            freq[pref]++;
        }

        cout << total - odd_div << '\n';
    }
    return 0;
}
