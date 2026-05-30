// Hint4
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    if (n == 1) {
        cout << a[0] << "\n";
        return 0;
    }

    vector<long long> left(n), right(n);
    left[0] = a[0];
    for (int i = 1; i < n; ++i) {
        left[i] = max(left[i - 1], a[i] + i);
    }
    right[n - 1] = a[n - 1] + (n - 1);
    for (int i = n - 2; i >= 0; --i) {
        right[i] = max(right[i + 1], a[i] + i);
    }

    long long ans = LLONG_MAX;
    for (int i = 0; i < n; ++i) {
        long long cand = a[i];
        if (i > 0) {
            cand = max(cand, left[i - 1] - i + n - 1);
        }
        if (i < n - 1) {
            cand = max(cand, right[i + 1] - i + n - 1);
        }
        ans = min(ans, cand);
    }

    cout << ans << "\n";
    return 0;
}
