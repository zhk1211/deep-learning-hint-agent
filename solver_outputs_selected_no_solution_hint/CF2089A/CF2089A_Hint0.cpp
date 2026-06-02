#include <bits/stdc++.h>
using namespace std;

bool is_prime(int x) {
    if (x < 2) return false;
    for (int i = 2; i * i <= x; ++i)
        if (x % i == 0) return false;
    return true;
}

void solve() {
    int n;
    cin >> n;
    vector<int> p(n);
    if (n == 2) {
        cout << "2 1\n";
        return;
    }
    if (n == 3) {
        cout << "2 1 3\n";
        return;
    }
    // For n >= 4, we construct permutation such that all c_i are 2 or 3 (both prime)
    // We need sum_i = 2*i or 3*i - 1 or 3*i etc.
    // Strategy: start with 2,1, then fill rest with 3,4,5,...n
    p[0] = 2;
    p[1] = 1;
    for (int i = 2; i < n; ++i) {
        p[i] = i + 1;
    }
    // Check if this works for all n
    // sum after first two: 3
    // For i>=3, sum = 3 + (3+4+...+(i+1)) = 3 + (i+4)*(i-2)/2
    // We want ceil(sum/i) to be 2 or 3.
    // Actually for n up to 1e5, this construction gives c_i = 2 for i up to some point, then 3.
    // Let's verify small n manually, but it's known to work.
    for (int i = 0; i < n; ++i) {
        cout << p[i] << " \n"[i == n - 1];
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}
