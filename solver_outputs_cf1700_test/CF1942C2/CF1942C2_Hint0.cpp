// Hint0
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        long long n;
        int x, y;
        cin >> n >> x >> y;
        vector<long long> a(x);
        for (int i = 0; i < x; ++i) {
            cin >> a[i];
        }
        sort(a.begin(), a.end());

        long long ans = x - 2; // triangles from chosen vertices alone
        vector<long long> gaps;
        for (int i = 0; i < x; ++i) {
            long long d;
            if (i + 1 < x) {
                d = a[i + 1] - a[i];
            } else {
                d = a[0] + n - a[i];
            }
            if (d == 2) {
                ans += 1;
            } else if (d > 2) {
                gaps.push_back(d);
            }
        }

        sort(gaps.begin(), gaps.end(), [](long long d1, long long d2) {
            int p1 = (d1 % 2 == 0) ? 1 : 0;
            int p2 = (d2 % 2 == 0) ? 1 : 0;
            if (p1 != p2) return p1 < p2;
            return d1 < d2;
        });

        for (long long d : gaps) {
            if (y == 0) break;
            long long need = d / 2;
            if (d % 2 == 0) {
                need = d / 2 - 1;
                if (y >= need) {
                    y -= need;
                    ans += d - 1;
                } else {
                    ans += 2 * y;
                    y = 0;
                }
            } else {
                if (y >= need) {
                    y -= need;
                    ans += d - 1;
                } else {
                    ans += 2 * y;
                    y = 0;
                }
            }
        }

        cout << ans << '\n';
    }
    return 0;
}
