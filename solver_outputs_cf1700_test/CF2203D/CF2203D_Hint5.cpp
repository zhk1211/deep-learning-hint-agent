// Hint5
#include <bits/stdc++.h>
using namespace std;

const int MAXV = 2000005; // n+m <= 2e6

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    // Pre-allocate vectors for frequency and divisor sums
    vector<int> freq_a(MAXV, 0);
    vector<int> freq_b(MAXV, 0);
    vector<int> div_sum(MAXV, 0); // sum of frequencies of divisors in a

    while (t--) {
        int n, m;
        cin >> n >> m;

        int max_val = 0;
        for (int i = 0; i < n; ++i) {
            int x;
            cin >> x;
            freq_a[x]++;
            max_val = max(max_val, x);
        }
        for (int i = 0; i < m; ++i) {
            int x;
            cin >> x;
            freq_b[x]++;
            max_val = max(max_val, x);
        }

        // Compute divisor sums for all numbers up to max_val
        // div_sum[y] = sum_{d|y} freq_a[d]
        for (int i = 1; i <= max_val; ++i) {
            if (freq_a[i] == 0) continue;
            for (int j = i; j <= max_val; j += i) {
                div_sum[j] += freq_a[i];
            }
        }

        // Classify numbers in b
        int alice_count = 0; // numbers y where all a are divisors of y
        int bob_count = 0;   // numbers y where no a divides y
        int neutral_count = 0; // others

        for (int y = 1; y <= max_val; ++y) {
            if (freq_b[y] == 0) continue;
            int cnt = freq_b[y];
            if (div_sum[y] == n) {
                alice_count += cnt;
            } else if (div_sum[y] == 0) {
                bob_count += cnt;
            } else {
                neutral_count += cnt;
            }
        }

        // Determine winner
        // Alice wins if she can force a win; otherwise Bob
        // Key observation: Alice can always take her numbers, Bob can always take his.
        // Neutral numbers can be taken by Alice only if she has a divisor in a.
        // The game reduces to: Alice can take any number with div_sum[y] > 0.
        // Bob can take any number with div_sum[y] < n.
        // The only numbers Bob cannot take are those with div_sum[y] == n (Alice's exclusive).
        // The only numbers Alice cannot take are those with div_sum[y] == 0 (Bob's exclusive).
        // Neutral numbers can be taken by both.
        // Optimal play: Alice will try to exhaust Bob's exclusive numbers by forcing him to take neutral ones.
        // Actually, the game is determined by the counts of exclusive numbers.
        // If alice_count > bob_count, Alice wins. If alice_count <= bob_count, Bob wins.
        // But careful: Alice moves first.
        // Let's simulate: Alice takes one of her exclusive numbers if available, else a neutral.
        // Bob takes one of his exclusive numbers if available, else a neutral.
        // The player who runs out of moves loses.
        // Since Alice goes first, if alice_count > bob_count, she has more exclusive moves, so she wins.
        // If alice_count <= bob_count, Bob has at least as many exclusive moves, and since Alice moves first,
        // Bob will have the last move (or Alice runs out first). So Bob wins.
        // This matches the sample.

        if (alice_count > bob_count) {
            cout << "Alice\n";
        } else {
            cout << "Bob\n";
        }

        // Clear frequencies for next test case
        for (int i = 1; i <= max_val; ++i) {
            freq_a[i] = 0;
            freq_b[i] = 0;
            div_sum[i] = 0;
        }
    }

    return 0;
}
