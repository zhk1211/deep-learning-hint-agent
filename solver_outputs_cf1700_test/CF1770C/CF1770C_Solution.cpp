// Solution
#include <bits/stdc++.h>
using namespace std;

bool is_prime(int p) {
    if (p < 2) return false;
    for (int i = 2; i * i <= p; ++i) {
        if (p % i == 0) return false;
    }
    return true;
}

void solve() {
    int n;
    cin >> n;
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }
    
    // Check pairwise distinct
    sort(a.begin(), a.end());
    for (int i = 1; i < n; ++i) {
        if (a[i] == a[i-1]) {
            cout << "NO\n";
            return;
        }
    }
    
    // Check primes up to n/2
    for (int p = 2; p <= n / 2; ++p) {
        if (!is_prime(p)) continue;
        vector<int> cnt(p, 0);
        for (int i = 0; i < n; ++i) {
            int rem = a[i] % p;
            cnt[rem]++;
        }
        int mn = *min_element(cnt.begin(), cnt.end());
        if (mn >= 2) {
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
