// Hint3
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
        
        // Count frequencies of each value
        map<int, int> freq;
        for (int x : a) {
            freq[x]++;
        }
        
        // Collect distinct values in descending order
        vector<int> vals;
        for (auto& p : freq) {
            vals.push_back(p.first);
        }
        sort(vals.begin(), vals.end(), greater<int>());
        
        long long alice = 0, bob = 0;
        int turn = 0; // 0 for Alice, 1 for Bob
        
        // Process values from largest to smallest
        for (int v : vals) {
            int cnt = freq[v];
            if (v % 2 == 0) {
                // Even value: the player whose turn it is will take all points
                if (turn == 0) {
                    alice += 1LL * cnt * (v / 2);
                    bob += 1LL * cnt * (v / 2);
                } else {
                    bob += 1LL * cnt * (v / 2);
                    alice += 1LL * cnt * (v / 2);
                }
                // After processing even v, turn does not change because both get equal moves
                // Actually, each even number contributes v moves, half by each player
                // The turn order for the next value depends on parity of total moves so far
                // But since both players get exactly v/2 moves each, the turn toggles v times
                // v is even, so turn toggles even times -> remains same
            } else {
                // Odd value: the first player to act on this value gets (v+1)/2 moves,
                // the other gets (v-1)/2 moves.
                int moves_first = (v + 1) / 2;
                int moves_second = v / 2;
                if (turn == 0) {
                    alice += 1LL * cnt * moves_first;
                    bob += 1LL * cnt * moves_second;
                } else {
                    bob += 1LL * cnt * moves_first;
                    alice += 1LL * cnt * moves_second;
                }
                // After odd v, total moves = v (odd), so turn toggles odd times -> flips
                turn ^= 1;
            }
        }
        
        cout << alice << ' ' << bob << '\n';
    }
    
    return 0;
}
