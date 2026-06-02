#include <bits/stdc++.h>
using namespace std;

bool is_prime(int x) {
    if (x < 2) return false;
    for (int i = 2; i * i <= x; ++i)
        if (x % i == 0) return false;
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<long long> a(n);
        for (int i = 0; i < n; ++i)
            cin >> a[i];

        // Check pairwise distinct
        sort(a.begin(), a.end());
        bool distinct = true;
        for (int i = 1; i < n; ++i) {
            if (a[i] == a[i - 1]) {
                distinct = false;
                break;
            }
        }
        if (!distinct) {
            cout << "NO\n";
            continue;
        }

        bool possible = true;
        // Check primes up to n/2
        for (int p = 2; p <= n / 2; ++p) {
            if (!is_prime(p)) continue;
            vector<int> cnt(p, 0);
            for (long long x : a)
                cnt[x % p]++;
            int mn = *min_element(cnt.begin(), cnt.end());
            if (mn >= 2) {
                possible = false;
                break;
            }
        }
        cout << (possible ? "YES" : "NO") << '\n';
    }
    return 0;
}
