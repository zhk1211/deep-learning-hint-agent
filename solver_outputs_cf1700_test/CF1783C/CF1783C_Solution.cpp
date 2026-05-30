#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        int n;
        ll m;
        cin >> n >> m;
        vector<ll> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }
        vector<ll> sorted_a = a;
        sort(sorted_a.begin(), sorted_a.end());
        vector<ll> pref(n + 1, 0);
        for (int i = 0; i < n; ++i) {
            pref[i + 1] = pref[i] + sorted_a[i];
        }
        // max number of wins x
        int x = upper_bound(pref.begin(), pref.end(), m) - pref.begin() - 1;
        int ans;
        if (x == n) {
            ans = 1;
        } else if (x == 0) {
            ans = n + 1;
        } else {
            if (a[x] <= sorted_a[x - 1]) {
                ans = n - x;
            } else {
                ll sum_x = pref[x];
                ll new_sum = sum_x - sorted_a[x - 1] + a[x];
                if (new_sum <= m) {
                    ans = n - x;
                } else {
                    ans = n - x + 1;
                }
            }
        }
        cout << ans << '\n';
    }
    return 0;
}
