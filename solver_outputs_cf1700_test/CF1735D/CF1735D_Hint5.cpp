// Hint5
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<vector<int>> cards(n, vector<int>(k));
    map<vector<int>, int> idx;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < k; ++j) {
            cin >> cards[i][j];
        }
        idx[cards[i]] = i;
    }

    // For each pair, find the third card that completes the set
    // third[i][j] = index of third card, or -1 if not present
    vector<vector<int>> third(n, vector<int>(n, -1));
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            vector<int> need(k);
            for (int f = 0; f < k; ++f) {
                if (cards[i][f] == cards[j][f]) {
                    need[f] = cards[i][f];
                } else {
                    // the third value is the one not present among 0,1,2
                    need[f] = 3 - cards[i][f] - cards[j][f];
                }
            }
            auto it = idx.find(need);
            if (it != idx.end()) {
                int t = it->second;
                third[i][j] = t;
                third[j][i] = t;
            }
        }
    }

    // Count number of sets each card belongs to
    vector<int> set_count(n, 0);
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (third[i][j] != -1) {
                int t = third[i][j];
                if (t > j) { // count each set once
                    set_count[i]++;
                    set_count[j]++;
                    set_count[t]++;
                }
            }
        }
    }

    long long ans = 0;
    // For each pair of cards that have a third, they form a set.
    // A meta-set is exactly 5 cards containing exactly 2 sets sharing exactly one card.
    // So we can iterate over the shared card, and count pairs of sets that share it.
    for (int i = 0; i < n; ++i) {
        // number of sets containing card i
        long long c = set_count[i];
        ans += c * (c - 1) / 2;
    }

    cout << ans << '\n';
    return 0;
}
