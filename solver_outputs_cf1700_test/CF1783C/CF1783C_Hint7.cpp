// Hint7
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n, m;
        cin >> n >> m;
        vector<int> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }

        vector<int> sorted_a = a;
        sort(sorted_a.begin(), sorted_a.end());

        int wins = 0;
        long long sum = 0;
        for (int i = 0; i < n; ++i) {
            if (sum + sorted_a[i] <= m) {
                sum += sorted_a[i];
                ++wins;
            } else {
                break;
            }
        }

        if (wins == n) {
            cout << 1 << '\n';
            continue;
        }

        int place = n - wins + 1;
        if (wins > 0 && a[wins] <= sorted_a[wins - 1] + (m - sum)) {
            place = n - wins;
        } else if (wins > 0 && a[wins] <= m) {
            // check if we can replace the largest taken with a[wins]
            if (sum - sorted_a[wins - 1] + a[wins] <= m) {
                place = n - wins;
            }
        } else if (wins == 0 && a[0] <= m) {
            place = n;
        }

        cout << place << '\n';
    }

    return 0;
}
