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
        // find longest prefix that is already sorted ascending
        int pref = 0;
        while (pref < n && p[pref] == pref + 1) {
            ++pref;
        }
        // find longest suffix that is already sorted descending
        int suff = 0;
        while (suff < n && p[n - 1 - suff] == n - suff) {
            ++suff;
        }
        // count elements that are in correct position for ascending
        int cnt_asc = 0;
        for (int i = 0; i < n; ++i) {
            if (p[i] == i + 1) cnt_asc++;
        }
        // count elements that are in correct position for descending
        int cnt_desc = 0;
        for (int i = 0; i < n; ++i) {
            if (p[i] == n - i) cnt_desc++;
        }
        // First wins if he can force ascending before Second can force descending.
        // The game reduces to comparing how many moves each needs to fix their order,
        // assuming optimal play with coloring and swapping.
        // First needs to fix all elements not in ascending position, except those
        // that are already fixed for descending? Actually, the known solution:
        // Let a = number of elements not in correct ascending position.
        // Let b = number of elements not in correct descending position.
        // But careful: elements that are correct for both don't need coloring.
        // The standard solution from editorial:
        // Count x = number of positions where p[i] != i+1 and p[i] != n-i.
        // Count y = number of positions where p[i] != i+1 but p[i] == n-i.
        // Count z = number of positions where p[i] == i+1 but p[i] != n-i.
        // Then First needs to color all x+y elements that are not in ascending position.
        // Second needs to color all x+z elements that are not in descending position.
        // But they can also swap already colored elements.
        // The result depends on comparison of these counts.
        int x = 0, y = 0, z = 0;
        for (int i = 0; i < n; ++i) {
            bool asc = (p[i] == i + 1);
            bool desc = (p[i] == n - i);
            if (!asc && !desc) x++;
            else if (!asc && desc) y++;
            else if (asc && !desc) z++;
        }
        // First wins if x + y <= z (he can color all his needed before Second can finish)
        // Second wins if x + z < y (strictly less because First moves first)
        // Otherwise Tie.
        if (x + y <= z) {
            cout << "First\n";
        } else if (x + z < y) {
            cout << "Second\n";
        } else {
            cout << "Tie\n";
        }
    }
    return 0;
}
