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

        // Count elements that are in correct position for ascending order
        int fixed_asc = 0;
        for (int i = 0; i < n; ++i) {
            if (p[i] == i + 1) {
                ++fixed_asc;
            }
        }

        // Count elements that are in correct position for descending order
        int fixed_desc = 0;
        for (int i = 0; i < n; ++i) {
            if (p[i] == n - i) {
                ++fixed_desc;
            }
        }

        // The number of elements that are not fixed for ascending
        int bad_asc = n - fixed_asc;
        // The number of elements that are not fixed for descending
        int bad_desc = n - fixed_desc;

        // Determine the result based on the counts
        if (bad_asc <= pref) {
            cout << "First\n";
        } else if (bad_desc <= suff) {
            cout << "Second\n";
        } else {
            cout << "Tie\n";
        }
    }

    return 0;
}
