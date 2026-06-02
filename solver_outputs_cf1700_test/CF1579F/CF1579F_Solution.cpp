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
        bool all_ones_cycle = false;

        vector<bool> visited(n, false);
        for (int start = 0; start < n; ++start) {
            if (visited[start]) continue;
            int cur = start;
            vector<int> seq;
            while (!visited[cur]) {
                visited[cur] = true;
                seq.push_back(a[cur]);
                cur = (cur + d) % n;
            }
            // seq is a cycle of length len
            // find max consecutive ones in this cycle
            // since it's cyclic, we can duplicate the sequence
            int m = seq.size();
            bool all_ones = true;
            for (int x : seq) if (x == 0) { all_ones = false; break; }
            if (all_ones) {
                all_ones_cycle = true;
                break;
            }
            // find max consecutive ones in cyclic array
            int max_ones = 0;
            int cur_ones = 0;
            for (int i = 0; i < 2 * m; ++i) {
                if (seq[i % m] == 1) {
                    cur_ones++;
                    max_ones = max(max_ones, cur_ones);
                } else {
                    cur_ones = 0;
                }
            }
            ans = max(ans, max_ones);
        }

        if (all_ones_cycle) {
            cout << -1 << '\n';
        } else {
            cout << ans << '\n';
        }
    }
    return 0;
}
