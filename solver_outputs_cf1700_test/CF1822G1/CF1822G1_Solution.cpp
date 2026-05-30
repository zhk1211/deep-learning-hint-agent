#include <bits/stdc++.h>
using namespace std;

const int MAXV = 1000000;
int cnt[MAXV + 5];
vector<int> used;

void solve() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<int> a(n);
        int M = 0;
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
            if (a[i] > M) M = a[i];
        }
        // clear frequency array from previous test case
        for (int x : used) cnt[x] = 0;
        used.clear();
        for (int x : a) {
            if (cnt[x] == 0) used.push_back(x);
            cnt[x]++;
        }
        long long ans = 0;
        // b = 1 case: all three elements equal
        for (int x : used) {
            int c = cnt[x];
            if (c >= 3) {
                ans += 1LL * c * (c - 1) * (c - 2);
            }
        }
        // b >= 2 case: strictly increasing values
        for (int x : used) {
            int cx = cnt[x];
            for (int b = 2; 1LL * x * b * b <= M; ++b) {
                int y = x * b;
                int z = y * b;
                // z <= M is guaranteed by loop condition
                if (cnt[y] && cnt[z]) {
                    ans += 1LL * cx * cnt[y] * cnt[z];
                }
            }
        }
        cout << ans << '\n';
    }
}

int main() {
    solve();
    return 0;
}
