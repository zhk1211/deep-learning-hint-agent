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

    long long total_download = accumulate(a.begin(), a.end(), 0LL);
    long long total_watch = n; // 1 minute per video

    // We need to find the maximum number of videos that can be stored simultaneously
    // This is equivalent to finding the maximum k such that sum of k smallest a_i <= m
    long long sum = 0;
    int k = 0;
    while (k < n && sum + a[k] <= m) {
        sum += a[k];
        ++k;
    }

    // The minimum total time is total_download + total_watch + (n - k)
    // because we need to wait for free space for the remaining n - k videos
    long long ans = total_download + total_watch + (n - k);
    cout << ans << '\n';

    return 0;
}
