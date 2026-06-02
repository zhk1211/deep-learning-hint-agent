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
        int max_val = n + m;
        vector<int> cnt(max_val + 1, 0);
        for (int i = 0; i < n; ++i) {
            int x;
            cin >> x;
            if (x <= max_val) cnt[x]++;
        }
        vector<int> b(m);
        for (int i = 0; i < m; ++i) {
            cin >> b[i];
        }

        vector<int> c(max_val + 1, 0);
        for (int i = 1; i <= max_val; ++i) {
            if (cnt[i] == 0) continue;
            for (int j = i; j <= max_val; j += i) {
                c[j] += cnt[i];
            }
        }

        int alice_only = 0, bob_only = 0, common = 0;
        for (int y : b) {
            if (c[y] == n) {
                alice_only++;
            } else if (c[y] == 0) {
                bob_only++;
            } else {
                common++;
            }
        }

        // Alice starts
        // Optimal play: both take common first, then their own
        // Alice can take common on her turn, Bob on his.
        // After common exhausted, they take their own.
        // Alice wins if she can make the last move.
        // Let's simulate turns: Alice takes common if any, else her own.
        // Bob takes common if any, else his own.
        // Actually, the game ends when a player cannot move.
        // Since Alice goes first, she wins if total moves is odd.
        // Moves: common can be taken by either, but optimal is to take common first.
        // So Alice will take common on her turns, Bob on his.
        // After common runs out, Alice takes alice_only, Bob takes bob_only.
        // So total moves Alice can make: ceil(common/2) + alice_only
        // Total moves Bob can make: floor(common/2) + bob_only
        // Alice wins if her total moves > Bob's total moves.
        int alice_moves = (common + 1) / 2 + alice_only;
        int bob_moves = common / 2 + bob_only;
        cout << (alice_moves > bob_moves ? "Alice" : "Bob") << '\n';
    }
    return 0;
}
