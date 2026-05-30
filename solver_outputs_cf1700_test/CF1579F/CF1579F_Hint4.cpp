// Hint4
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n, d;
        cin >> n >> d;
        vector<int> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }

        int g = gcd(n, d);
        int len = n / g;
        int ans = 0;

        for (int start = 0; start < g; ++start) {
            vector<int> seq;
            for (int i = 0; i < len; ++i) {
                seq.push_back(a[(start + i * d) % n]);
            }

            bool all_zero = true;
            for (int x : seq) {
                if (x == 1) {
                    all_zero = false;
                    break;
                }
            }
            if (all_zero) continue;

            bool all_one = true;
            for (int x : seq) {
                if (x == 0) {
                    all_one = false;
                    break;
                }
            }
            if (all_one) {
                ans = -1;
                break;
            }

            vector<int> doubled = seq;
            doubled.insert(doubled.end(), seq.begin(), seq.end());
            int max_ones = 0, cur = 0;
            for (int x : doubled) {
                if (x == 1) {
                    cur++;
                    max_ones = max(max_ones, cur);
                } else {
                    cur = 0;
                }
            }
            ans = max(ans, max_ones);
        }

        cout << ans << '\n';
    }
    return 0;
}
