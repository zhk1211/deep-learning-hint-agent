// Hint5
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
    
    // Check that elements come in pairs
    for (int i = 0; i < 2 * n; i += 2) {
        if (d[i] != d[i + 1]) {
            cout << "NO\n";
            return;
        }
    }
    
    // Remove duplicates, keep only one of each pair
    vector<ll> uniq;
    for (int i = 0; i < 2 * n; i += 2) {
        uniq.push_back(d[i]);
    }
    
    // Now uniq has n elements, sorted, corresponding to d for positive a's
    // We'll reconstruct a_n, a_{n-1}, ..., a_1
    vector<ll> a(n);
    ll sum_a = 0;
    
    for (int k = n - 1; k >= 0; --k) {
        // d for the k-th largest positive element (0-indexed from largest)
        // uniq[k] = 2 * (k+1) * a_k + 2 * sum_{j=k+1}^{n-1} a_j
        // Actually careful: uniq is sorted ascending, so uniq[n-1] is largest d.
        // Let's re-index: let b[0] < b[1] < ... < b[n-1] be the positive a's.
        // Then d for b[i] is 2*(i+1)*b[i] + 2*sum_{j=i+1}^{n-1} b[j].
        // We have uniq sorted ascending, so uniq[i] corresponds to b[i].
        // We'll compute from largest b to smallest.
        // Let i = n-1 down to 0.
        ll val = uniq[k];
        val -= 2 * sum_a;
        if (val <= 0 || val % (2 * (k + 1)) != 0) {
            cout << "NO\n";
            return;
        }
        a[k] = val / (2 * (k + 1));
        sum_a += a[k];
    }
    
    // Check that all a_i are distinct and positive
    for (int i = 0; i < n; ++i) {
        if (a[i] <= 0) {
            cout << "NO\n";
            return;
        }
    }
    for (int i = 1; i < n; ++i) {
        if (a[i] == a[i - 1]) {
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
