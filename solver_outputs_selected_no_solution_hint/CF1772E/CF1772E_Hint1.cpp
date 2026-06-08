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

        // Find longest prefix that is already sorted ascending
        int pref = 0;
        while (pref < n && p[pref] == pref + 1) {
            ++pref;
        }

        // Find longest suffix that is already sorted descending
        int suff = 0;
        while (suff < n && p[n - 1 - suff] == n - suff) {
            ++suff;
        }

        // Check if the remaining middle part is sorted ascending or descending
        bool middle_asc = true;
        for (int i = pref; i + 1 < n - suff; ++i) {
            if (p[i] > p[i + 1]) {
                middle_asc = false;
                break;
            }
        }

        bool middle_desc = true;
        for (int i = pref; i + 1 < n - suff; ++i) {
            if (p[i] < p[i + 1]) {
                middle_desc = false;
                break;
            }
        }

        if (pref == n) {
            // Already sorted ascending, but problem says not sorted either way initially
            // So this case won't happen
            cout << "First\n";
        } else if (suff == n) {
            // Already sorted descending, same
            cout << "Second\n";
        } else if (middle_asc) {
            // Middle is sorted ascending
            if (pref >= suff) {
                cout << "First\n";
            } else {
                cout << "Tie\n";
            }
        } else if (middle_desc) {
            // Middle is sorted descending
            if (suff >= pref) {
                cout << "Second\n";
            } else {
                cout << "Tie\n";
            }
        } else {
            cout << "Tie\n";
        }
    }

    return 0;
}
