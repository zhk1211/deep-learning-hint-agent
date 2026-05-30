// Hint3
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
    
    int ans = 0;
    int i = 0;
    while (i < n) {
        if (a[i] == 0) {
            ++i;
            continue;
        }
        // start of a non-zero segment
        int j = i;
        while (j < n && a[j] != 0) {
            ++j;
        }
        // segment [i, j-1] is all non-zero
        int len = j - i;
        bool has2 = false;
        for (int k = i; k < j; ++k) {
            if (a[k] == 2) has2 = true;
        }
        if (has2) {
            // one coin can cover the whole segment and possibly extend to adjacent zeros
            ++ans;
            // mark zeros adjacent to segment as covered
            if (i > 0 && a[i-1] == 0) a[i-1] = -1; // covered
            if (j < n && a[j] == 0) a[j] = -1;
        } else {
            // all ones: need ceil(len/2) coins, but also consider adjacent zeros
            // Actually, a segment of ones can be covered with ceil(len/2) coins,
            // and each coin can also cover one adjacent zero if available.
            // We'll handle zeros greedily later.
            ans += (len + 1) / 2;
            // The ones segment can cover up to len/2 zeros on each side? 
            // Actually, each coin spent on a 1 can cover one adjacent zero.
            // But we need to be careful: we might have already covered some zeros.
            // Let's just mark zeros that can be covered.
            int coins = (len + 1) / 2;
            int left_zeros = 0, right_zeros = 0;
            if (i > 0 && a[i-1] == 0) left_zeros = 1;
            if (j < n && a[j] == 0) right_zeros = 1;
            // We can cover up to coins zeros total from both sides.
            int cover = min(coins, left_zeros + right_zeros);
            if (cover > 0) {
                if (left_zeros && right_zeros) {
                    if (cover >= 2) {
                        a[i-1] = -1;
                        a[j] = -1;
                    } else {
                        // cover one, prefer left? doesn't matter
                        a[i-1] = -1;
                    }
                } else if (left_zeros) {
                    a[i-1] = -1;
                } else if (right_zeros) {
                    a[j] = -1;
                }
            }
        }
        i = j;
    }
    
    // Count remaining zeros that are not covered
    for (int i = 0; i < n; ++i) {
        if (a[i] == 0) ++ans;
    }
    
    cout << ans << '\n';
    return 0;
}
