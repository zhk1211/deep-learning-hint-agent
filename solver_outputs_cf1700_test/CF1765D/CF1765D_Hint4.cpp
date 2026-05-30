// Hint4
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

    // Check how many good pairs we can form
    int good_pairs = 0;
    int l = 0, r = n - 1;
    while (l < r) {
        if (a[l] + a[r] <= m) {
            ++good_pairs;
            ++l;
            --r;
        } else {
            --r;
        }
    }

    // Total time = sum of download times + n (watching time) + number of bad pairs
    // Number of bad pairs = (n - 1) - good_pairs
    long long sum_download = accumulate(a.begin(), a.end(), 0LL);
    long long bad_pairs = (n - 1) - good_pairs;
    long long ans = sum_download + n + bad_pairs;
    cout << ans << '\n';

    return 0;
}
