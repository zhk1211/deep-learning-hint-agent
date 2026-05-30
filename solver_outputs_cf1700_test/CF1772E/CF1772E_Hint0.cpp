// Hint0
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
        vector<int> p(n);
        for (int i = 0; i < n; ++i) {
            cin >> p[i];
        }

        int first_wins = 0, second_wins = 0;
        int first_mismatch = 0, second_mismatch = 0;

        for (int i = 0; i < n; ++i) {
            if (p[i] != i + 1) first_mismatch++;
            if (p[i] != n - i) second_mismatch++;
        }

        if (first_mismatch == 0) {
            cout << "First\n";
            continue;
        }
        if (second_mismatch == 0) {
            cout << "Second\n";
            continue;
        }

        int common = 0;
        for (int i = 0; i < n; ++i) {
            if (p[i] != i + 1 && p[i] != n - i) common++;
        }

        int only_first = first_mismatch - common;
        int only_second = second_mismatch - common;

        int first_turns = only_first + common;
        int second_turns = only_second + common;

        if (first_turns <= second_turns) {
            if (first_turns <= only_second) {
                cout << "First\n";
            } else {
                cout << "Tie\n";
            }
        } else {
            if (second_turns < only_first) {
                cout << "Second\n";
            } else {
                cout << "Tie\n";
            }
        }
    }
    return 0;
}
