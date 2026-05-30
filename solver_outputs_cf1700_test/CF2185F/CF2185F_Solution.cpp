// Solution
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n, q;
        cin >> n >> q;
        int m = 1 << n;
        vector<int> a(m);
        for (int i = 0; i < m; ++i) {
            cin >> a[i];
        }

        vector<int> pref(m + 1, 0);
        for (int i = 0; i < m; ++i) {
            pref[i + 1] = pref[i] ^ a[i];
        }

        while (q--) {
            int b, c;
            cin >> b >> c;
            --b;
            int orig = a[b];
            a[b] = c;

            int pos = b;
            int ans = 0;
            int block_size = 1;
            int block_start = pos & ~(block_size - 1);
            int block_xor = 0;
            for (int i = block_start; i < block_start + block_size; ++i) {
                block_xor ^= a[i];
            }

            while (block_size < m) {
                int other_start;
                if (pos % (2 * block_size) == block_start) {
                    other_start = block_start + block_size;
                } else {
                    other_start = block_start - block_size;
                }
                int other_xor = 0;
                for (int i = other_start; i < other_start + block_size; ++i) {
                    other_xor ^= a[i];
                }

                if (block_xor > other_xor || (block_xor == other_xor && block_start < other_start)) {
                    if (pos >= other_start && pos < other_start + block_size) {
                        ans += block_size;
                    }
                    block_size *= 2;
                    block_start = min(block_start, other_start);
                    block_xor ^= other_xor;
                } else {
                    if (pos >= block_start && pos < block_start + block_size) {
                        ans += block_size;
                    }
                    block_size *= 2;
                    block_start = min(block_start, other_start);
                    block_xor ^= other_xor;
                }
            }

            cout << ans << '\n';
            a[b] = orig;
        }
    }
    return 0;
}
