// Hint1
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long m;
    cin >> n >> m;
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    sort(a.begin(), a.end());

    long long total_download = 0;
    for (auto x : a) total_download += x;

    // We want to maximize the number of adjacent pairs (i, i+1) in the watching order
    // such that a_i + a_{i+1} <= m. This allows us to save 1 minute per such pair.
    // The maximum number of such pairs is achieved by a greedy pairing:
    // pair the smallest with the largest possible that fits.
    int pairs = 0;
    int l = 0, r = n - 1;
    vector<bool> used(n, false);
    while (l < r) {
        if (a[l] + a[r] <= m) {
            ++pairs;
            used[l] = used[r] = true;
            ++l;
            --r;
        } else {
            --r;
        }
    }

    // The total time = total_download + n (watching) - pairs (saved minutes)
    long long ans = total_download + n - pairs;
    cout << ans << '\n';

    return 0;
}
