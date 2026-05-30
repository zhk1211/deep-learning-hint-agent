// Hint2
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    int coins = 0;
    int i = 0;
    while (i < n) {
        if (a[i] == 0) {
            ++i;
            continue;
        }
        // start of a non-zero segment
        int j = i;
        bool has_two = false;
        while (j < n && a[j] != 0) {
            if (a[j] == 2) has_two = true;
            ++j;
        }
        // segment [i, j-1] is non-zero, surrounded by zeros or boundaries
        int len = j - i;
        if (!has_two) {
            // all ones
            coins += len;
        } else {
            // has at least one 2
            // we can paint the whole segment with one coin if we start from a 2
            // but we need to check if we can extend to adjacent zeros
            bool left_zero = (i > 0 && a[i-1] == 0);
            bool right_zero = (j < n && a[j] == 0);
            if (left_zero && right_zero) {
                // we can paint the segment and both adjacent zeros with one coin
                // but we need to ensure the segment has a 2 to propagate
                coins += 1;
                // mark the zeros as painted by skipping them
                if (i > 0) a[i-1] = -1; // painted
                if (j < n) a[j] = -1;
            } else if (left_zero || right_zero) {
                // one adjacent zero can be painted together
                coins += 1;
                if (left_zero) a[i-1] = -1;
                if (right_zero) a[j] = -1;
            } else {
                // no adjacent zeros, just paint the segment
                coins += 1;
            }
        }
        i = j;
    }

    // count remaining zeros that were not painted
    for (int x : a) {
        if (x == 0) ++coins;
    }

    cout << coins << '\n';
    return 0;
}
