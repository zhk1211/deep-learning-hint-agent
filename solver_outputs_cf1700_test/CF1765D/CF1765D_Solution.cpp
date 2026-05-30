#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    ll m;
    cin >> n >> m;
    vector<ll> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    sort(a.begin(), a.end());
    ll total_sum = accumulate(a.begin(), a.end(), 0LL);

    auto check = [&](int k) {
        int L = n - k;
        if (L <= 1) return true;
        ll max_sum = 0;
        for (int i = 0; i < L / 2; ++i) {
            max_sum = max(max_sum, a[i] + a[L - 1 - i]);
        }
        return max_sum <= m;
    };

    int low = 0, high = n - 1, ans = n - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (check(mid)) {
            ans = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    cout << total_sum + ans + 1 << '\n';
    return 0;
}
