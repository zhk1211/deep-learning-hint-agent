// Hint4
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

        long long ans = x - 2; // base triangles from chosen vertices
        vector<long long> gaps;
        for (int i = 0; i < x; ++i) {
            long long cur = a[i];
            long long nxt = a[(i + 1) % x];
            long long diff = (nxt - cur + n) % n;
            if (diff >= 2) {
                gaps.push_back(diff);
            }
        }

        // Separate gaps into even and odd (excluding 1)
        vector<long long> even_gaps, odd_gaps;
        for (long long g : gaps) {
            if (g % 2 == 0) {
                even_gaps.push_back(g);
            } else if (g > 1) {
                odd_gaps.push_back(g);
            }
        }

        // Process even gaps first (they give more triangles per y used)
        sort(even_gaps.begin(), even_gaps.end());
        for (long long g : even_gaps) {
            long long need = g / 2 - 1;
            if (y >= need) {
                y -= need;
                ans += g - 1; // triangles from this gap
            } else {
                ans += 2 * y;
                y = 0;
                break;
            }
        }

        // Process odd gaps
        if (y > 0) {
            sort(odd_gaps.begin(), odd_gaps.end());
            for (long long g : odd_gaps) {
                long long need = g / 2;
                if (y >= need) {
                    y -= need;
                    ans += g - 1;
                } else {
                    ans += 2 * y;
                    y = 0;
                    break;
                }
            }
        }

        cout << ans << '\n';
    }
    return 0;
}
