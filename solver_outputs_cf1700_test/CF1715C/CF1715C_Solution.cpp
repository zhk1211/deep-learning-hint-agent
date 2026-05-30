// Solution
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<long long> a(n + 2);
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
    }

    long long total_subsegments = (long long)n * (n + 1) / 2;
    long long joints_sum = 0;
    for (int i = 1; i < n; ++i) {
        if (a[i] != a[i + 1]) {
            joints_sum += (long long)i * (n - i);
        }
    }

    while (m--) {
        int i;
        long long x;
        cin >> i >> x;

        if (i > 1) {
            if (a[i - 1] != a[i]) {
                joints_sum -= (long long)(i - 1) * (n - (i - 1));
            }
        }
        if (i < n) {
            if (a[i] != a[i + 1]) {
                joints_sum -= (long long)i * (n - i);
            }
        }

        a[i] = x;

        if (i > 1) {
            if (a[i - 1] != a[i]) {
                joints_sum += (long long)(i - 1) * (n - (i - 1));
            }
        }
        if (i < n) {
            if (a[i] != a[i + 1]) {
                joints_sum += (long long)i * (n - i);
            }
        }

        cout << total_subsegments + joints_sum << '\n';
    }

    return 0;
}
