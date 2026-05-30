// Solution
#include <bits/stdc++.h>
using namespace std;

bool good_triple(int x, int y, int z) {
    if (x <= y && y <= z) return false;
    if (x >= y && y >= z) return false;
    return true;
}

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
        long long ans = n; // length 1 subarrays
        if (n >= 2) ans += n - 1; // length 2

        // length 3 subarrays
        for (int i = 0; i + 2 < n; ++i) {
            if (good_triple(a[i], a[i+1], a[i+2])) {
                ++ans;
            }
        }

        // length 4 subarrays
        for (int i = 0; i + 3 < n; ++i) {
            bool bad = false;
            if (!good_triple(a[i], a[i+1], a[i+2])) bad = true;
            if (!good_triple(a[i], a[i+1], a[i+3])) bad = true;
            if (!good_triple(a[i], a[i+2], a[i+3])) bad = true;
            if (!good_triple(a[i+1], a[i+2], a[i+3])) bad = true;
            if (!bad) ++ans;
        }

        cout << ans << '\n';
    }
    return 0;
}
