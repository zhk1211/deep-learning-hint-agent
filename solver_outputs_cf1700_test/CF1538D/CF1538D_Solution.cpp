// Solution
#include <bits/stdc++.h>
using namespace std;

vector<int> primes;

void sieve() {
    const int MAX = 40000;
    vector<bool> is_prime(MAX + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i <= MAX; ++i) {
        if (is_prime[i]) {
            primes.push_back(i);
            if ((long long)i * i <= MAX) {
                for (int j = i * i; j <= MAX; j += i)
                    is_prime[j] = false;
            }
        }
    }
}

int count_factors(int n) {
    int cnt = 0;
    for (int p : primes) {
        if ((long long)p * p > n) break;
        while (n % p == 0) {
            ++cnt;
            n /= p;
        }
    }
    if (n > 1) ++cnt;
    return cnt;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    sieve();
    int t;
    cin >> t;
    while (t--) {
        int a, b, k;
        cin >> a >> b >> k;
        // maximum possible exponents sum for a,b ≤ 1e9 is 58
        if (k > 60) {
            cout << "NO\n";
            continue;
        }
        int n = count_factors(a) + count_factors(b);
        int m;
        if (a == b)
            m = 0;
        else if (a % b == 0 || b % a == 0)
            m = 1;
        else
            m = 2;
        bool ans;
        if (k == 1)
            ans = (m == 1);
        else
            ans = (m <= k && k <= n);
        cout << (ans ? "YES" : "NO") << "\n";
    }
    return 0;
}
