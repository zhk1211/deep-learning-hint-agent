#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        long long n;
        cin >> n;
        if (n == 1) {
            cout << 2 << "\n";
            continue;
        }
        long long low = 2, high = 2000000, ans = high;
        while (low <= high) {
            long long mid = (low + high) / 2;
            long long k = mid / 3;
            long long rem = mid % 3;
            long long triangles = 3 * k * (k + 1);
            if (rem == 1) {
                triangles += 2 * (k + 1);
            } else if (rem == 2) {
                triangles += 2 * (k + 1) + (2 * k + 1);
            }
            if (triangles >= n) {
                ans = mid;
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }
        cout << ans << "\n";
    }
    return 0;
}
