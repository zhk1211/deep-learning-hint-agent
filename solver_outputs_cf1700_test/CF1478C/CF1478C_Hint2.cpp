// Hint2
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void solve() {
    int n;
    cin >> n;
    vector<ll> d(2 * n);
    for (int i = 0; i < 2 * n; ++i) {
        cin >> d[i];
    }
    sort(d.begin(), d.end());
    
    // Check that each value appears exactly twice
    for (int i = 0; i < 2 * n; i += 2) {
        if (d[i] != d[i + 1]) {
            cout << "NO\n";
            return;
        }
    }
    
    // Remove duplicates
    vector<ll> uniq;
    for (int i = 0; i < 2 * n; i += 2) {
        uniq.push_back(d[i]);
    }
    
    // Now uniq has size n, sorted ascending
    // We will reconstruct a_n, a_{n-1}, ..., a_1 (positive half)
    vector<ll> a(n);
    ll sum_a = 0;
    for (int i = n - 1; i >= 0; --i) {
        // uniq[i] = 2 * (i+1) * a[i] + 2 * sum_a
        // where sum_a = a_{i+1} + ... + a_{n-1}
        ll numerator = uniq[i] - 2 * sum_a;
        ll denominator = 2 * (i + 1);
        if (numerator <= 0 || numerator % denominator != 0) {
            cout << "NO\n";
            return;
        }
        a[i] = numerator / denominator;
        sum_a += a[i];
    }
    
    // Check that all a[i] are distinct and positive
    // Since we constructed them in strictly increasing order? 
    // Actually a[i] should be positive and strictly increasing with i.
    // We can check that a[0] > 0 and a[i] < a[i+1]
    for (int i = 0; i < n; ++i) {
        if (a[i] <= 0) {
            cout << "NO\n";
            return;
        }
        if (i > 0 && a[i] <= a[i - 1]) {
            cout << "NO\n";
            return;
        }
    }
    
    cout << "YES\n";
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
