// Hint6
#include <bits/stdc++.h>
using namespace std;

const int MAXV = 2000005;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n, m;
        cin >> n >> m;
        vector<int> a(n), b(m);
        for (int i = 0; i < n; ++i) cin >> a[i];
        for (int i = 0; i < m; ++i) cin >> b[i];

        int max_val = 0;
        for (int x : a) max_val = max(max_val, x);
        for (int x : b) max_val = max(max_val, x);

        vector<int> cnt_a(max_val + 1, 0);
        for (int x : a) cnt_a[x]++;

        vector<int> cnt_b(max_val + 1, 0);
        for (int x : b) cnt_b[x]++;

        // c[i] = number of divisors of i present in a
        vector<int> c(max_val + 1, 0);
        for (int d = 1; d <= max_val; ++d) {
            if (cnt_a[d] == 0) continue;
            for (int multiple = d; multiple <= max_val; multiple += d) {
                c[multiple] += cnt_a[d];
            }
        }

        int alice_moves = 0, bob_moves = 0;
        for (int y = 1; y <= max_val; ++y) {
            if (cnt_b[y] == 0) continue;
            if (c[y] == n) {
                // all a_i divide y -> Alice can take this y
                alice_moves += cnt_b[y];
            } else if (c[y] == 0) {
                // no a_i divides y -> Bob can take this y
                bob_moves += cnt_b[y];
            }
            // otherwise y is "neutral" and can be taken by either player
            // but only when forced; they don't affect the outcome directly
        }

        // The game reduces to: Alice has alice_moves "safe" moves,
        // Bob has bob_moves "safe" moves, and the rest are neutral.
        // The player who runs out of their safe moves first loses.
        // Since Alice starts, if alice_moves > bob_moves, Alice wins.
        // If alice_moves <= bob_moves, Bob wins.
        if (alice_moves > bob_moves) {
            cout << "Alice\n";
        } else {
            cout << "Bob\n";
        }
    }
    return 0;
}
