#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n, x;
        cin >> n >> x;
        vector<int> p(n + 1);
        int pos_x = -1;
        for (int i = 1; i <= n; ++i) {
            cin >> p[i];
            if (p[i] == x) pos_x = i;
        }

        int l = 1, r = n + 1;
        while (r - l > 1) {
            int m = (l + r) / 2;
            if (p[m] <= x) l = m;
            else r = m;
        }

        if (p[l] == x) {
            cout << "0\n";
            continue;
        }

        // We need to make p[l] == x after at most 2 swaps.
        // First, swap x into position l.
        cout << "1\n";
        cout << pos_x << " " << l << "\n";
        swap(p[pos_x], p[l]);
        pos_x = l;

        // Now run binary search again to see if it works.
        l = 1, r = n + 1;
        while (r - l > 1) {
            int m = (l + r) / 2;
            if (p[m] <= x) l = m;
            else r = m;
        }

        if (p[l] == x) {
            // Already correct after one swap.
            continue;
        }

        // Need a second swap: swap x (now at pos_x) with the element at the new l.
        cout << "1\n";
        cout << pos_x << " " << l << "\n";
    }
    return 0;
}
