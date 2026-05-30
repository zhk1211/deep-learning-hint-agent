// Hint0
#include <bits/stdc++.h>
using namespace std;

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

        int max_val = *max_element(a.begin(), a.end());
        max_val = max(max_val, *max_element(b.begin(), b.end()));

        vector<int> cnt_a(max_val + 1, 0);
        for (int x : a) cnt_a[x]++;

        vector<int> cnt_b(max_val + 1, 0);
        for (int y : b) cnt_b[y]++;

        vector<int> good(max_val + 1, 0);
        for (int x = 1; x <= max_val; ++x) {
            if (cnt_a[x] == 0) continue;
            for (int y = x; y <= max_val; y += x) {
                good[y] += cnt_a[x];
            }
        }

        long long alice_moves = 0;
        long long bob_moves = 0;
        for (int y = 1; y <= max_val; ++y) {
            if (cnt_b[y] == 0) continue;
            if (good[y] > 0) {
                alice_moves += cnt_b[y];
            } else {
                bob_moves += cnt_b[y];
            }
        }

        if (alice_moves > bob_moves) {
            cout << "Alice\n";
        } else if (bob_moves > alice_moves) {
            cout << "Bob\n";
        } else {
            cout << (alice_moves % 2 == 1 ? "Alice" : "Bob") << "\n";
        }
    }
    return 0;
}
