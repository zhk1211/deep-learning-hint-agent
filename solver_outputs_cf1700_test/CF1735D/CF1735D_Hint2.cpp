// Hint2
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;
    vector<vector<int>> cards(n, vector<int>(k));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < k; ++j) {
            cin >> cards[i][j];
        }
    }

    // Map each card to its index for O(1) lookup
    map<vector<int>, int> idx;
    for (int i = 0; i < n; ++i) {
        idx[cards[i]] = i;
    }

    // For each pair, find the third card that completes the set
    // and count how many sets each pair belongs to
    vector<vector<int>> set_count(n, vector<int>(n, 0));

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            vector<int> third(k);
            for (int f = 0; f < k; ++f) {
                if (cards[i][f] == cards[j][f]) {
                    third[f] = cards[i][f];
                } else {
                    // The third value is the one not present among 0,1,2
                    third[f] = 3 - cards[i][f] - cards[j][f];
                }
            }
            if (idx.count(third)) {
                int t = idx[third];
                if (t > j) { // ensure each set is counted once
                    set_count[i][j]++;
                    set_count[i][t]++;
                    set_count[j][t]++;
                }
            }
        }
    }

    long long ans = 0;
    // For each pair, if it belongs to at least 2 sets, then those two sets
    // share this pair and form a meta-set of 5 cards (the pair + 2 other cards + the third card of each set)
    // Actually, a meta-set is exactly 5 cards with exactly 2 sets.
    // Each meta-set has exactly one pair that belongs to both sets.
    // So we count pairs that are in at least 2 sets, and each such pair
    // contributes C(cnt, 2) meta-sets.
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            int c = set_count[i][j];
            if (c >= 2) {
                ans += 1LL * c * (c - 1) / 2;
            }
        }
    }

    cout << ans << '\n';
    return 0;
}
